cmake_minimum_required(VERSION 3.18.4)

project(clownassembler LANGUAGES C)

add_executable(clownassembler
	"frontend_custom.c"
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

set_target_properties(clownassembler PROPERTIES
	C_STANDARD 90
	C_STANDARD_REQUIRED YES
	C_EXTENSIONS OFF
)

add_executable(clownassembler_asm68k
	"frontend_asm68k.c"
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

set_target_properties(clownassembler_asm68k PROPERTIES
	C_STANDARD 90
	C_STANDARD_REQUIRED YES
	C_EXTENSIONS OFF
)

enable_testing()

# Misc. test
add_test(NAME misc COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/shim.asm" -o "output.bin")
set_property(TEST misc PROPERTY WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/")

# Test valid MOVE_TO_SR instructions (this is made redundant by the 'valid_instructions' test and will likely be removed in the future).
add_test(NAME INSTURCTION_MOVE_TO_SR_VALID COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/valid.asm" -o "output.bin")

# Test invalid MOVE_TO_SR instructions.
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_1 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/1.asm" -o "output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_1 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_2 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/2.asm" -o "output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_2 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_3 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/3.asm" -o "output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_3 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_4 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/4.asm" -o "output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_4 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_5 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/5.asm" -o "output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_5 PROPERTY WILL_FAIL true)

# Test that all valid combinations of instructions and operands assemble.
add_test(NAME valid_instructions_assemble COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/valid instructions.asm" -o "output.bin")

# Test that all valid combinations of instructions and operands produce the correct machine code.
add_test(NAME valid_instructions_compare COMMAND ${CMAKE_COMMAND} -E compare_files "${CMAKE_CURRENT_SOURCE_DIR}/tests/valid instructions.bin" "output.bin")

# Test that the Sonic 1 disassembly assembles.
add_test(NAME sonic_1_assemble COMMAND clownassembler -i "sonic.asm" -o "s1built.bin")
set_property(TEST sonic_1_assemble PROPERTY WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/s1disasm")

# Test that the Sonic 1 disassembly produces the correct machine code.
add_test(NAME sonic_1_compare COMMAND ${CMAKE_COMMAND} -E compare_files "s1rev01.bin" "s1built.bin")
set_property(TEST sonic_1_compare PROPERTY WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/s1disasm")
