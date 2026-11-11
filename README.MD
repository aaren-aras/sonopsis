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
compiling the GPU beamforming and filterning kernels in `device/kernels`. 

On Windows, you'll need to install the standard Linux version of the toolkit within a 
[WSL2](https://learn.microsoft.com/en-us/windows/wsl/install) environment (Ubuntu **22.04 
LTS** recommended) equipped with **GCC**, **Make**, and **CMake**, as native MSVC doesn't
support POSIX socket headers (like those used in `host/receiver.c`).

## Setup

### Running Locally
Run `ctest --verbose` to run tests inside the `scripts/` directory.

## Retrospective