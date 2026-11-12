cmake_minimum_required(VERSION 3.30)
set(CMAKE_CUDA_ARCHITECTURES "120")
# set_property(TARGET sonopsis PROPERTY CUDA_ARCHITECTURES "120")
# project(sonopsis LANGUAGES C CXX CUDA)
project(sonopsis LANGUAGES C CXX)


# Discover CUDA targets
find_package(CUDAToolkit REQUIRED)

# Add source files to project executable
add_executable (sonopsis 
    "include/sonopsis/buffer.h"
	"include/sonopsis/config.h"
	"include/sonopsis/frame.h"
	"host/ring_buffer.c" 
	"host/receiver.c" 
	"host/frame_assembler.c"
	"host/controller.cpp" 
    "display/gl_display.cpp" 
    "display/overlay.cpp"
    "device/kernels/colormap.cu"
    "device/kernels/hilbert.cu"
    "device/kernels/log_compress.cu"
    "device/kernels/scan_convert.cu"
)

target_include_directories(sonopsis PRIVATE include)
set_property(TARGET sonopsis PROPERTY CXX_STANDARD 20)
set_property(TARGET sonopsis PROPERTY CUDA_STANDARD 17)
set_property(TARGET sonopsis PROPERTY CUDA_SEPARABLE_COMPILATION ON)
# set_property(TARGET sonopsis PROPERTY CUDA_ARCHITECTURES "120") # RTX 5070 Ti (Blackwell, sm_120)

# Link CUDA Runtime Library (as part of CUDA Toolkit)
target_link_libraries(sonopsis PRIVATE CUDA::cudart)

enable_testing()
add_test(
	NAME test_udp_send
	COMMAND python3 ${CMAKE_SOURCE_DIR}/scripts/test_send.py
)
add_test(
    NAME test_udp_roundtrip
    COMMAND bash ${CMAKE_SOURCE_DIR}/scripts/test_udp.sh
)

# Copy built binary to a system location when running `cmake --install`
install(TARGETS sonopsis DESTINATION bin)