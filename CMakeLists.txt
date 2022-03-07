cmake_minimum_required(VERSION 3.20)

project(clownassembler LANGUAGES C)

add_executable(clownassembler
	"main.c"
	"lexer.c"
	"lexer.h"
	"parser.c"
	"parser.h"
	"symbols.c"
	"symbols.h"
	"types.h"
)

set_target_properties(clownassembler PROPERTIES
	C_STANDARD 90
	C_STANDARD_REQUIRED YES
	C_EXTENSIONS OFF
)

#target_compile_definitions(clownassembler PRIVATE PRINT)

enable_testing()

add_test(NAME misc COMMAND clownassembler "${CMAKE_CURRENT_SOURCE_DIR}/tests/misc.asm")
add_test(NAME INSTURCTION_MOVE_TO_SR_VALID COMMAND clownassembler "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/valid.asm")
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_1 COMMAND clownassembler "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/1.asm")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_1 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_2 COMMAND clownassembler "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/2.asm")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_2 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_3 COMMAND clownassembler "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/3.asm")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_3 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_4 COMMAND clownassembler "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/4.asm")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_4 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_5 COMMAND clownassembler "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/5.asm")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_5 PROPERTY WILL_FAIL true)
add_test(NAME valid_instructions COMMAND clownassembler "${CMAKE_CURRENT_SOURCE_DIR}/tests/valid instructions.asm")
