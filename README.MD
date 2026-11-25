# SONOPSIS 

Going to try (**1**) synthetically generating raw ultrasound radio frequency data with 
`k-wave-python` (a Python implementation of MATLAB's `k-Wave` toolbox), (**2**) processing (*beamforming*) and
filtering it on a **[PYNQ-Z2](https://www.amd.com/en/corporate/university-program/aup-boards/pynq-z2.html)** 
FPGA SoC, then (**3**) streaming the outputs to a host GPU for 2D/3D image reconstruction with 
**[CUDA](https://docs.nvidia.com/cuda/cuda-programming-guide/02-basics/writing-cuda-kernels.html)** kernels.
Files in the `host/` and `device/` directories ingest and process data on the **CPU** and **GPU** respectively.

The goal is to eventually transition to a physical transducer probe, pulser, analog front-end, and ADC, 
but I'm broke. Was inspired to build this after taking a really awesome ultrasound elective in my last term.

## Prerequisites
NVIDIA's [CUDA Toolkit](https://developer.nvidia.com/cuda-downloads) is required for
compiling the beamforming and filtering kernels in `device/kernels`, so choose a version 
that matches your GPU's architecture. Needless to say, an **NVIDIA GPU**
*(either local or remote)* is also required.

On Windows, you'll need to install the Linux version of the toolkit within a 
**[WSL 2](https://learn.microsoft.com/en-us/windows/wsl/install)** environment (Ubuntu **22.04 
LTS** recommended) equipped with **Python 3.x**, **GCC 13+**, **Make**, and **CMake 3.30+**, since 
native Windows doesn't support POSIX socket headers (like the ones in `host/receiver.c`). 

Ensure you also have the latest NVIDIA [display drivers](https://www.nvidia.com/en-us/drivers/) 
installed on your **host machine** (NOT inside WSL 2). I'm finding I need ~**6** GB of 
disk space for the project, most of which is coming from the `nvidia-cuda-toolkit` package.

## Setup
Begin by cloning the repository and navigating to the project directory:

```bash
git clone https://github.com/aaren-aras/sonopsis.git 
cd sonopsis
```

If you've already installed the prequisites, jump ahead to [Running Locally](#running-locally).

### Installing Prerequisites
**APT** (the default **Ubuntu** package manager) usually ships an older **CMake** version by default,
so install modern CMake through Kitware's official repository along with the build essentials:

```bash
sudo apt update && sudo apt install -y \
	software-properties-common lsb-release wget gpg \
	build-essential gcc g++ \ 
	cmake python3-venv python3-pip
```

Next, import Kitware's GPG signing key *(so APT can verify package authenticity)* and their repository:

```bash
sudo mkdir -p /etc/apt/keyrings
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc | gpg --dearmor | sudo tee /etc/apt/keyrings/kitware-archive-keyring.gpg >/dev/null
echo "deb [signed-by=/etc/apt/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null

sudo apt update
sudo apt install -y kitware-archive-keyring cmake
```

Next, install the official `cuda-toolkit` package for compiling the kernels:

```bash
wget https://developer.download.nvidia.com/compute/cuda/repos/wsl-ubuntu/x86_64/cuda-keyring_1.1-1_all.deb
sudo dpkg -i cuda-keyring_1.1-1_all.deb
sudo apt update
sudo apt install -y cuda-toolkit
```

Add the CUDA binaries and libraries to your shell path by appending them to your `~/.bashrc`:

```bash
echo 'export PATH=/usr/local/cuda/bin:$PATH' >> ~/.bashrc
echo 'export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH' >> ~/.bashrc
source ~/.bashrc
```

Lastly, verify your installations before proceeding:

```bash
cmake --version  
nvcc --version  
nvidia-smi     
```

If you're running into issues with `cuda-toolkit`, try `nvidia-cuda-toolkit` instead.

### Running Locally

From the project root, inside a **WSL 2** terminal, install core Python dependencies (`requirements.txt`)
in a `.venv`:

```bash
python3 -m venv .venv # Python 3.11/3.12 recommended
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
for testing (see `scripts/`).

## Retrospective
 - Avoid installing the full `cuda` package since it bundles conflicting Linux display drivers
