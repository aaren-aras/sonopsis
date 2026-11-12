# SONOPSIS 

Going to try synthetically generating raw ultrasound radio frequency (RF) data with 
[k-wave-python](https://github.com/waltsims/k-wave-python/tree/master) (Python implementation 
of MATLAB's k-Wave toolbox), processing (beamforming) and filtering it on a **PYNQ-Z2** FPGA 
SoC, then streaming the outputs to a PC for 2D/3D image reconstruction with **CUDA** kernels.

The goal is to eventually replicate the whole ultrasound process with a physical transducer 
probe, pulser, analog front-end, and ADC, but I'm broke.

Files in the `host/` directory run and ingest data on the CPU with CPU memory. Files in the 
`device/` directory run and process data on the GPU with GPU memory.

## Prerequisites
NVIDIA's [CUDA Toolkit](https://developer.nvidia.com/cuda-downloads) is required for
compiling the GPU beamforming and filtering kernels in `device/kernels` *(choose a version 
that matches your GPU's architecture)*. It goes without saying that access to an **NVIDIA GPU**, 
*either local or remote*, is also required.

On Windows, you'll need to install the standard Linux version of the toolkit within a 
[WSL2](https://learn.microsoft.com/en-us/windows/wsl/install) environment (Ubuntu **22.04 
LTS** recommended) equipped with **Python 3.x**, **GCC 13+**, **Make**, **CMake 3.30+**, since 
native Windows doesn't support POSIX socket headers (like those used in `host/receiver.c`). 
This also ensures environmental parity with k-wave-python's C++ binaries.

Ensure you have the latest NVIDIA [display driver](https://www.nvidia.com/en-us/drivers/) 
installed as well (on your **host machine**, NOT inside WSL). I'm finding I need about
**6** GB of disk space total, most of which is coming from the `nvidia-cuda-toolkit` package
through Ubuntu/WSL.

## Setup
If you've already installed the prequisites, jump ahead to [Running Locally](#running-locally).

Inside WSL2 (Ubuntu) terminal, clone the repository, and navigate to the project directory:

```bash
git clone https://github.com/aaren-aras/oncer.git 
cd sonopsis
```

Or, clone the repository outside WSL and navigate to your directory through `/mnt/c/`.

### Installing Prerequisites
If you are running Ubuntu or WSL 2, the default package manager often provides an older CMake version. 
Install modern CMake via Kitware's official repository along with the build essentials and CUDA toolkit:

```bash
# Update package lists and install basic build tools
sudo apt update && sudo apt install -y \
	software-properties-common lsb-release wget gpg \
	build-essential gcc g++ \ 
	cmake python3-venv python3-pip

# Add Kitware's GPG key and official repository for CMake 3.30+
sudo mkdir -p /etc/apt/keyrings
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | gpg --dearmor | sudo tee /etc/apt/keyrings/kitware-archive-keyring.gpg >/dev/null
echo "deb [signed-by=/etc/apt/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null

# Install modern CMake
sudo apt update
sudo apt install -y kitware-archive-keyring cmake
```

To compile the CUDA kernels, install the Linux CUDA toolkit package matching your environment 
(avoiding monolithic driver packages inside WSL):

```bash
wget https://developer.download.nvidia.com/compute/cuda/repos/wsl-ubuntu/x86_64/cuda-keyring_1.1-1_all.deb
sudo dpkg -i cuda-keyring_1.1-1_all.deb
sudo apt update
sudo apt install -y cuda-toolkit
```

Permanently add the CUDA binaries and libraries to your shell path by appending them to your ~/.bashrc:

```bash
echo 'export PATH=/usr/local/cuda/bin:$PATH' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc
```

Finally, verify your installations:

```bash
cmake --version   # Must be >= 3.30
nvcc --version    # Must locate the CUDA compiler
nvidia-smi        # Verify GPU visibility
```

If you're running into issues with the official package, try `nvidia-cuda-toolkit` instead.

### Running Locally

From the project root, inside a WSL2 (Ubuntu) terminal, install the core Python 
dependencies in a virtual environment (Python **3.11/3.12** recommended):

```bash
python3 -m venv .venv
source .venv/bin/activate 
.venv/bin/pip install --upgrade pip
.venv/bin/pip install -r requirements.txt
```

Then, configure the `build/` directory and compile the project executables and CUDA kernels:

```bash
cmake -B build
cmake --build build
```

Clear old build folders with `rm -rf build`, and run `ctest --verbose --test-dir build` 
to run the test suite (see `scripts/`).

## Retrospective