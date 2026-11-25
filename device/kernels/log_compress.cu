//// CUDA Kernel for parallel log compression
//__global__ void log_compress_kernel(float* d_input, float* d_output, int num_samples, float scale) {
//    int idx = blockDim.x * blockIdx.x + threadIdx.x;
//    if (idx < num_samples) {
//        // Compress dynamic range: log mapping + normalization
//        float val = d_input[idx];
//        d_output[idx] = 20.0f * log10f(fmaxf(val, 1e-6f)) * scale; 
//    }
//}