cmake_minimum_required(VERSION 3.16.3)

project(clownassembler LANGUAGES C)

# Core
add_library(clownassembler_core STATIC
	"dictionary.c"
	"dictionary.h"
	"lexical.c"
	"lexical.h"
	"semantic.c"
	"semantic.h"
	"strcmpci.c"
	"strcmpci.h"
	"syntactic.c"
	"syntactic.h"
)

# Custom frontend
add_executable(clownassembler
	"frontend_custom.c"
)

target_link_libraries(clownassembler clownassembler_core)

# asm68k frontend
add_executable(clownassembler_asm68k
	"frontend_asm68k.c"
)

target_link_libraries(clownassembler_asm68k clownassembler_core)

# Tests
include(CTest)

# Misc. test
add_test(NAME misc COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/shim.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/")

# Test valid MOVE_TO_SR instructions (this is made redundant by the 'valid_instructions' test and will likely be removed in the future).
add_test(NAME INSTURCTION_MOVE_TO_SR_VALID COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/valid.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")

# Test invalid MOVE_TO_SR instructions.
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_1 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/1.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_1 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_2 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/2.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_2 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_3 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/3.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_3 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_4 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/4.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_4 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_5 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/5.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_5 PROPERTY WILL_FAIL true)

# Test that all valid combinations of instructions and operands assemble.
add_test(NAME valid_instructions_assemble COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/valid instructions.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")

# Test that all valid combinations of instructions and operands produce the correct machine code.
add_test(NAME valid_instructions_compare COMMAND ${CMAKE_COMMAND} -E compare_files "${CMAKE_CURRENT_SOURCE_DIR}/tests/valid instructions.bin" "${CMAKE_CURRENT_BINARY_DIR}/output.bin")

# Test that the Sonic 1 disassembly assembles.
add_test(NAME sonic_1_assemble COMMAND clownassembler -i "sonic.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/s1built.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/s1disasm")

# Test that the Sonic 1 disassembly produces the correct machine code.
add_test(NAME sonic_1_compare COMMAND ${CMAKE_COMMAND} -E compare_files "s1rev01.bin" "${CMAKE_CURRENT_BINARY_DIR}/s1built.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/s1disasm")

# Test that the 68 Katy's BASIC environment assembles.
add_test(NAME 68_katy_basic_assemble COMMAND clownassembler -i "Basic68s3.52.x68" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/68-katy-basic")

# Test that the 68 Katy's BASIC environment produces the correct machine code.
add_test(NAME 68_katy_basic_compare COMMAND ${CMAKE_COMMAND} -E compare_files "basic.bin" "${CMAKE_CURRENT_BINARY_DIR}/output.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/68-katy-basic")

# Test that the Sonic Spinball disassembly assembles.
add_test(NAME sonic_spinball_assemble COMMAND clownassembler -i "spinball.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/sbbuilt.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/spindisasm")

# Test that the Sonic 2 August 21st disassembly assembles.
add_test(NAME sonic_2_august_21st_assemble COMMAND clownassembler -i "sonic2alpha.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/sonic2alpha.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/Sonic-2-Aug-21st-Disassembly")

# Test that the Sonic 2 August 21st disassembly produces the correct machine code.
add_test(NAME sonic_2_august_21st_compare COMMAND ${CMAKE_COMMAND} -E compare_files "sonic2alpha.bin" "${CMAKE_CURRENT_BINARY_DIR}/sonic2alpha.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/Sonic-2-Aug-21st-Disassembly")
