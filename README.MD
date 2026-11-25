# SONOPSIS 

Going to try (**1**) synthetically generating raw ultrasound radio frequency data with 
`k-wave-python` (a Python implementation of MATLAB's `k-Wave` toolbox), (**2**) processing (*beamforming*) and
filtering it on a **[PYNQ-Z2](https://www.amd.com/en/corporate/university-program/aup-boards/pynq-z2.html)** 
FPGA SoC, then (**3**) streaming the outputs to a host GPU for 2D/3D image reconstruction with 
**[CUDA](https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/writing-cuda-kernels.html)** kernels.
Files in the `host/` and `device/` directories ingest and process data on the **CPU** and **GPU** respectively.

The goal is to eventually transition to a physical transducer probe, pulser, analog front-end, and ADC, 
but I'm broke. Was inspired to work on this after a really awesome elective from my last term!

## Prerequisites
NVIDIA's [CUDA Toolkit](https://developer.nvidia.com/cuda-downloads) is required for
compiling the beamforming and filtering kernels in `device/kernels`, so choose a version 
that matches your GPU's architecture. Needless to say, access to an **NVIDIA GPU**
*(either local or remote)* is also required.

On Windows, you'll need to install the Linux version of the toolkit within a 
**[WSL2](https://learn.microsoft.com/en-us/windows/wsl/install)** environment (Ubuntu **22.04 
LTS** recommended) equipped with **Python 3.x**, **GCC 13+**, **Make**, and **CMake 3.30+**, since 
native Windows doesn't support POSIX socket headers (like the ones in `host/receiver.c`). 
This also ensures parity with `k-wave-python`'s C++ binaries.

Ensure you have the latest NVIDIA [display drivers](https://www.nvidia.com/en-us/drivers/) 
installed on your **host machine** as well (NOT inside WSL). I'm finding I need ~**6** GB of disk space 
for the project, mostly coming from the `nvidia-cuda-toolkit` package in Ubuntu/WSL.

## Setup
If you've already installed the prequisites, jump ahead to [Running Locally](#running-locally).

Inside a **WSL2** (**Ubuntu**) terminal, clone the repository, and navigate to the project directory:

```bash
git clone https://github.com/aaren-aras/oncer.git 
cd sonopsis
```

Or, clone the repository outside WSL and navigate to your directory through `/mnt/c/`.

### Installing Prerequisites
If you're running Ubuntu or WSL 2, the default package manager often provides an older CMake version. 
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
cmake --version  
nvcc --version  
nvidia-smi     
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