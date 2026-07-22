# SONOPSIS 

#### Etymology
'Sis' is six in Catalan.

## Log
Going to try synthetically generating raw ultrasound radio frequency (RF) data with a Python implementation 
of MATLAB's k-Wave toolbox, processing (beamforming) and filtering it on a **PYNQ-Z2** FPGA SoC, then 
streaming the outputs to a PC for 2D/3D image reconstruction with **CUDA** kernels.

The goal is to eventually replicate the whole ultrasound process with a physical transducer probe, 
pulser, analog front-end, and ADC, but I'm broke.
