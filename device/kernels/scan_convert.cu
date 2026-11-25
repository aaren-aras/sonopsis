//// Scan conversion (pixel->polar mapping)
//__global__ void scan_convert_kernel(uchar* img_out, float* rf_polar, int width, int height) {
//    int x = blockDim.x * blockIdx.x + threadIdx.x;
//    int y = blockDim.y * blockIdx.y + threadIdx.y;
//    
//    if (x < width && y < height) {
//        // Convert cartesian screen coordinates (x,y) to polar (radius, theta)
//        float r = compute_radius(x, y);
//        float theta = compute_theta(x, y);
//        
//        // Bilinear interpolate from the polar RF matrix
//        img_out[y * width + x] = sample_polar_grid(rf_polar, r, theta);
//    }
//}