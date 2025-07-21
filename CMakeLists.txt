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
	"string.c"
	"string.h"
	"substitute.c"
	"substitute.h"
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

add_executable(clownassembler_hash "tests/hash/hash.c")

# Misc. test
add_test(NAME misc_assemble COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/shim.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/misc.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/")

add_test(NAME misc_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/misc.bin" "aa6cd6ed5e86a38e573f07d202c75562")
set_tests_properties(misc_compare PROPERTIES DEPENDS misc_assemble)

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
add_test(NAME valid_instructions_assemble COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/valid instructions.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/valid instructions.bin")

# Test that all valid combinations of instructions and operands produce the correct machine code.
add_test(NAME valid_instructions_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/valid instructions.bin" "48d926b7701f312b3d9d8878b213184c")
set_tests_properties(valid_instructions_compare PROPERTIES DEPENDS valid_instructions_assemble)

# Test that the Sonic 1 disassembly assembles.
add_test(NAME sonic_1_assemble COMMAND clownassembler -i "sonic.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/s1built.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/s1disasm")

# Test that the Sonic 1 disassembly produces the correct machine code.
add_test(NAME sonic_1_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/s1built.bin" "09dadb5071eb35050067a32462e39c5f")
set_tests_properties(sonic_1_compare PROPERTIES DEPENDS sonic_1_assemble)

# Test that the 68 Katy's BASIC environment assembles.
add_test(NAME 68_katy_basic_assemble COMMAND clownassembler -i "Basic68s3.52.x68" -o "${CMAKE_CURRENT_BINARY_DIR}/68-katy-basic.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/68-katy-basic")

# Test that the 68 Katy's BASIC environment produces the correct machine code.
add_test(NAME 68_katy_basic_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/68-katy-basic.bin" "b75983db72bb2418a196d40f7142e70a")
set_tests_properties(68_katy_basic_compare PROPERTIES DEPENDS 68_katy_basic_assemble)

# Test that the Sonic Spinball disassembly assembles.
add_test(NAME sonic_spinball_assemble COMMAND clownassembler -i "spinball.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/sbbuilt.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/spindisasm")

# Test that the Sonic 2 August 21st disassembly assembles.
add_test(NAME sonic_2_august_21st_assemble COMMAND clownassembler -i "sonic2alpha.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/sonic2alpha.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/Sonic-2-Aug-21st-Disassembly")

# Test that the Sonic 2 August 21st disassembly produces the correct machine code.
add_test(NAME sonic_2_august_21st_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/sonic2alpha.bin" "4f779c3e0ee0bd0686aab8f9e16a4002")
set_tests_properties(sonic_2_august_21st_compare PROPERTIES DEPENDS sonic_2_august_21st_assemble)

# Test that the Mean Bean Machine disassembly assembles.
add_test(NAME mean_bean_machine_assemble COMMAND clownassembler -c -i "main.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/bean.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/Dr-Robotnik-Mean-Bean-Machine-Disassembly")

# Test that the Mean Bean Machine disassembly produces the correct machine code.
add_test(NAME mean_bean_machine_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/bean.bin" "23bd83928b990dfb84c149a77ef613cf")
set_tests_properties(mean_bean_machine_compare PROPERTIES DEPENDS mean_bean_machine_assemble)
