cmake_minimum_required(VERSION 3.16.3)

project(clownassembler VERSION 2.0 LANGUAGES CXX)

# Core
add_library(clownassembler_core STATIC
	"source/backup-stack.h"
	"source/dictionary.h"
	"source/frontend/common.h"
	"source/error.h"
	"source/expression-processor.cpp"
	"source/expression-processor.h"
	"source/global.h"
	"source/instruction-processor.cpp"
	"source/instruction-processor.h"
	"source/listing.cpp"
	"source/listing.h"
	"source/macro.h"
	"source/macro-state.h"
	"source/object.cpp"
	"source/object.h"
	"source/object-writer.h"
	"source/options.h"
	"source/p2bin.cpp"
	"source/p2bin.h"
	"source/parser/lexical.cpp"
	"source/parser/lexical.h"
	"source/parser/syntactic.cpp"
	"source/parser/syntactic.h"
	"source/parser.h"
	"source/program-counter.h"
	"source/semantic.cpp"
	"source/semantic.h"
	"source/source-line-reader.cpp"
	"source/source-line-reader.h"
	"source/substitute.cpp"
	"source/substitute.h"
	"source/symbol-manager.cpp"
	"source/symbol-manager.h"
	"source/utilities/case-insensitive.h"
	"source/utilities/io.h"
	"source/utilities/path.h"
	"source/utilities/string.h"
	"source/version.h"
)

set_target_properties(clownassembler_core PROPERTIES CXX_STANDARD 20)

# Custom frontend
add_executable(clownassembler
	"source/frontend/custom.cpp"
)

set_target_properties(clownassembler PROPERTIES CXX_STANDARD 20)
target_link_libraries(clownassembler clownassembler_core)

# asm68k frontend
add_executable(clownassembler_asm68k
	"source/frontend/asm68k.cpp"
)

set_target_properties(clownassembler_asm68k PROPERTIES CXX_STANDARD 20)
target_link_libraries(clownassembler_asm68k clownassembler_core)

# Tests
include(CTest)

add_executable(clownassembler_hash "source/hash/hash.cpp")
set_target_properties(clownassembler_hash PROPERTIES CXX_STANDARD 20)

# Misc. test
add_test(NAME misc_assemble COMMAND clownassembler -p -v -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/shim.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/misc.bin" -s "${CMAKE_CURRENT_BINARY_DIR}/misc.sym" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/")

add_test(NAME misc_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/misc.bin" "e581e15b39cfc3141fe361a057454213")
set_tests_properties(misc_compare PROPERTIES DEPENDS misc_assemble)

# Test valid MOVE_TO_SR instructions (this is made redundant by the 'valid_instructions' test and will likely be removed in the future).
add_test(NAME INSTURCTION_MOVE_TO_SR_VALID COMMAND clownassembler -p -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/valid.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")

# Test invalid MOVE_TO_SR instructions.
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_1 COMMAND clownassembler -p -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/1.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_1 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_2 COMMAND clownassembler -p -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/2.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_2 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_3 COMMAND clownassembler -p -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/3.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_3 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_4 COMMAND clownassembler -p -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/4.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_4 PROPERTY WILL_FAIL true)
add_test(NAME INSTURCTION_MOVE_TO_SR_INVALID_5 COMMAND clownassembler -p -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/instructions/MOVE_TO_SR/invalid/5.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST INSTURCTION_MOVE_TO_SR_INVALID_5 PROPERTY WILL_FAIL true)

# Test that all valid combinations of instructions and operands assemble.
add_test(NAME valid_instructions_assemble COMMAND clownassembler -p -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/valid instructions.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/valid instructions.bin")

# Test that all valid combinations of instructions and operands produce the correct machine code.
add_test(NAME valid_instructions_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/valid instructions.bin" "48d926b7701f312b3d9d8878b213184c")
set_tests_properties(valid_instructions_compare PROPERTIES DEPENDS valid_instructions_assemble)

# Test that the Sonic 1 disassembly assembles.
add_test(NAME sonic_1_assemble COMMAND clownassembler_asm68k /k /m /p /o "ae-,oz+,c+,l+" "sonic.asm" , "${CMAKE_CURRENT_BINARY_DIR}/s1built.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/s1disasm")

# Test that the Sonic 1 disassembly produces the correct machine code.
add_test(NAME sonic_1_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/s1built.bin" "09dadb5071eb35050067a32462e39c5f")
set_tests_properties(sonic_1_compare PROPERTIES DEPENDS sonic_1_assemble)

# Test that the 68 Katy's BASIC environment assembles.
add_test(NAME 68_katy_basic_assemble COMMAND clownassembler -p -i "Basic68s3.52.x68" -o "${CMAKE_CURRENT_BINARY_DIR}/68-katy-basic.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/68-katy-basic")

# Test that the 68 Katy's BASIC environment produces the correct machine code.
add_test(NAME 68_katy_basic_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/68-katy-basic.bin" "b75983db72bb2418a196d40f7142e70a")
set_tests_properties(68_katy_basic_compare PROPERTIES DEPENDS 68_katy_basic_assemble)

# Test that the Sonic Spinball disassembly assembles.
add_test(NAME sonic_spinball_assemble COMMAND clownassembler -p -i "spinball.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/sbbuilt.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/spindisasm")

# Test that the Sonic 2 August 21st disassembly assembles.
add_test(NAME sonic_2_august_21st_assemble COMMAND clownassembler -p -i "sonic2alpha.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/sonic2alpha.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/Sonic-2-Aug-21st-Disassembly")

# Test that the Sonic 2 August 21st disassembly produces the correct machine code.
add_test(NAME sonic_2_august_21st_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/sonic2alpha.bin" "4f779c3e0ee0bd0686aab8f9e16a4002")
set_tests_properties(sonic_2_august_21st_compare PROPERTIES DEPENDS sonic_2_august_21st_assemble)

# Test that the Mean Bean Machine disassembly assembles.
add_test(NAME mean_bean_machine_assemble COMMAND clownassembler -c -p -.. -i "main.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/bean.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/Dr-Robotnik-Mean-Bean-Machine-Disassembly")

# Test that the Mean Bean Machine disassembly produces the correct machine code.
add_test(NAME mean_bean_machine_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/bean.bin" "23bd83928b990dfb84c149a77ef613cf")
set_tests_properties(mean_bean_machine_compare PROPERTIES DEPENDS mean_bean_machine_assemble)

# Z80 macro tests.
add_test(NAME z80_gems_assemble COMMAND clownassembler -c -p -i "TEST.ASM" -o "${CMAKE_CURRENT_BINARY_DIR}/z80-gems.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/ASM68K-Z80-extension/GEMS")
add_test(NAME z80_gems_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/z80-gems.bin" "85e18682e195dc4b7cd64673f1d879bd")
set_tests_properties(z80_gems_compare PROPERTIES DEPENDS z80_gems_assemble)

add_test(NAME z80_megapcm_assemble COMMAND clownassembler -c -p -i "MegaPCM.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/z80-megapcm.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/ASM68K-Z80-extension/MegaPCM")
add_test(NAME z80_megapcm_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/z80-megapcm.bin" "9c871049e5065b2991f9ac7982a9fbd8")
set_tests_properties(z80_megapcm_compare PROPERTIES DEPENDS z80_megapcm_assemble)

add_test(NAME z80_s_and_k_assemble COMMAND clownassembler -c -p -i "TEST.ASM" -o "${CMAKE_CURRENT_BINARY_DIR}/z80-s-and-k.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/ASM68K-Z80-extension/Sonic & Knuckles")
add_test(NAME z80_s_and_k_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/z80-s-and-k.bin" "c787ad7c4d1f952d32092bda221b843f")
set_tests_properties(z80_s_and_k_compare PROPERTIES DEPENDS z80_s_and_k_assemble)

add_test(NAME z80_sonic1_assemble COMMAND clownassembler -c -p -i "sound/z80/z80.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/z80-sonic1.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/ASM68K-Z80-extension/Sonic 1")
add_test(NAME z80_sonic1_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/z80-sonic1.bin" "6ae9dc82ef1c4890c5fc55fc6a47ee54")
set_tests_properties(z80_sonic1_compare PROPERTIES DEPENDS z80_sonic1_assemble)

add_test(NAME z80_westone_assemble COMMAND clownassembler -c -p -i "TEST.ASM" -o "${CMAKE_CURRENT_BINARY_DIR}/z80-westone.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/ASM68K-Z80-extension/Westone")
add_test(NAME z80_westone_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/z80-westone.bin" "ee34bb568b2590632d4837fbe1531f06")
set_tests_properties(z80_westone_compare PROPERTIES DEPENDS z80_westone_assemble)

# Test a bunch of cases that would previously cause crashes/leaks
# (these often are intended to still fail, but not crash, given the crash is often in how erroneous code is handled, though a few do actually succeed compilation).

# The hash parameter is meaningless if will_fail is true.
function(add_regression_test kind source_file will_fail hash)
	set(test_name "${kind}_${source_file}")

	add_test(NAME "${test_name}" COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-${kind}/${source_file}" -o "${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bin")

	if (will_fail)
		set_property(TEST "${test_name}" PROPERTY WILL_FAIL true)
	else()
		add_test(NAME "${test_name}_compare" COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/${test_name}.bin" "${hash}")
		set_tests_properties("${test_name}_compare" PROPERTIES DEPENDS "${test_name}")
	endif()
endfunction()

function(add_regression_test_expected_to_error kind source_file)
	add_regression_test("${kind}" "${source_file}" true "")
endfunction()

function(add_regression_test_expected_to_succeed kind source_file hash)
	add_regression_test("${kind}" "${source_file}" false "${hash}")
endfunction()

add_regression_test_expected_to_error("crashing" "comment-line-followed-by-erroring-line.asm")
add_regression_test_expected_to_error("crashing" "div-by-0.asm")
add_regression_test_expected_to_error("crashing" "mod-by-0.asm")
add_regression_test_expected_to_error("crashing" "redefine-macro-parameter-in-macro-body.asm")
add_regression_test_expected_to_error("crashing" "redefine-macro-parameter-in-macro-body-within-macro.asm")
add_regression_test_expected_to_succeed("crashing" "shift-by-massive-val.asm" "f1d3ff8443297732862df21dc4e57262")
add_regression_test_expected_to_error("crashing" "shift-no-args.asm")
add_regression_test_expected_to_error("leaking" "equal-with-trailing-garbage-identifier.asm")
add_regression_test_expected_to_error("leaking" "label-definition-followed-by-redefinition-as-macro.asm")
add_regression_test_expected_to_error("leaking" "label-definition-followed-by-redefinition-as-macro-with-parameter.asm")
add_regression_test_expected_to_error("leaking" "label-definition-followed-by-redefinition-as-macro-with-code-line.asm")
add_regression_test_expected_to_error("crashing" "redefine-macro-in-macro-body.asm")
add_regression_test_expected_to_error("crashing" "define-global-label-then-redefine-at-sign-as-macro-twice.asm")
add_regression_test_expected_to_error("crashing" "purge-macro.asm")
add_regression_test_expected_to_error("crashing" "redefine-macro-as-another-macro-while-it-is-being-expanded.asm")
add_regression_test_expected_to_error("crashing" "macro-infinite-recursion.asm")
add_regression_test_expected_to_succeed("crashing" "substr-out-of-bounds.asm" "d41d8cd98f00b204e9800998ecf8427e")
add_regression_test_expected_to_error("leaking" "multiple-unterminated-repts.asm")
add_regression_test_expected_to_error("leaking" "unterminated-rept-and-while.asm")
add_regression_test_expected_to_error("crashing" "cnop-size-boundary-0.asm")
add_regression_test_expected_to_error("leaking" "inform-with-complex-severity-expression.asm")
add_regression_test_expected_to_error("crashing" "redefinition-during-mutually-recursive-infinite-macro-expansion.asm")
