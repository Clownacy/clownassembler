cmake_minimum_required(VERSION 3.16.3)

project(clownassembler-cpp LANGUAGES C CXX)

add_library(clownassembler-cpp STATIC
	"clownassembler.cpp"
	"clownassembler.h"
)

add_subdirectory("../" EXCLUDE_FROM_ALL "clownassembler")

target_link_libraries(clownassembler-cpp PRIVATE clownassembler_core)

set_target_properties(clownassembler-cpp PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED YES
)
