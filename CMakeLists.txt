# CMakeList.txt : CMake project for sonopsis, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project (sonopsis LANGUAGES C CXX CUDA)

# Discover CUDA targets
find_package(CUDAToolkit REQUIRED)

# Add source files to project executable
add_executable (sonopsis 
	"include/sonopsis/frame.h"
	"include/sonopsis/pipeline_config.h"
	"host/ring_buffer.c" 
	"host/receiver.c" 
	"host/frame_assembler.c"
	"host/pipeline_controller.cpp" 
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
# Set CUDA architectures for RTX 5070 Ti (Blackwell / sm_100 or native)
set_property(TARGET sonopsis PROPERTY CUDA_ARCHITECTURES "100")

# Link the CUDA Runtime Library
target_link_libraries(sonopsis PRIVATE CUDA::cudart)

# TODO: Add tests and install targets if needed.
enable_testing()
add_test(
	NAME test_udp_send
	COMMAND python3 ${CMAKE_SOURCE_DIR}/tests/test_send.py
)
add_test(
    NAME test_udp_roundtrip
    COMMAND bash ${CMAKE_SOURCE_DIR}/tests/test_udp.sh
)

# Copy built binary to a system location when running `cmake --install`
install(TARGETS sonopsis DESTINATION bin)