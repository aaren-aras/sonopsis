# CMakeList.txt : CMake project for sonopsis, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.10)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("sonopsis")

# Add source to this project's executable.
add_executable (sonopsis 
	"include/sonopsis/frame.h"
	"include/sonopsis/pipeline_config.h"
	"host/ring_buffer.c" 
	"host/receiver.c" 
	"host/frame_assembler.c"
)

target_include_directories(sonopsis PRIVATE include)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET sonopsis PROPERTY CXX_STANDARD 20)
endif()

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