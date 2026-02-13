cmake_minimum_required(VERSION 3.16.3)

project(clownassembler VERSION 1.1.1.1 LANGUAGES C)

# Core
add_library(clownassembler_core STATIC
	"dictionary.c"
	"dictionary.h"
	"io.c"
	"io.h"
	"lexical.c"
	"lexical.h"
	"options.c"
	"options.h"
	"p2bin.c"
	"p2bin.h"
	"semantic.c"
	"semantic.h"
	"shared-memory.c"
	"shared-memory.h"
	"strcmpci.c"
	"strcmpci.h"
	"string.c"
	"string.h"
	"string-stack.c"
	"string-stack.h"
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
add_test(NAME misc_assemble COMMAND clownassembler -p -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/shim.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/misc.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/")

add_test(NAME misc_compare COMMAND clownassembler_hash "${CMAKE_CURRENT_BINARY_DIR}/misc.bin" "030f66ef9fbda09fbb677644aa2fc93b")
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
add_test(NAME sonic_1_assemble COMMAND clownassembler -p -.. -i "sonic.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/s1built.bin" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests/s1disasm")

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
# (these often are intended to still fail, but not crash, given the crash was is often in how erroneous code is handled, though a few do actually succeed compilation)
add_test(NAME crash_comment_line_followed_by_erroring_line COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/comment-line-followed-by-erroring-line.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST crash_comment_line_followed_by_erroring_line PROPERTY WILL_FAIL true)
add_test(NAME crash_div_by_0 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/div-by-0.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST crash_div_by_0 PROPERTY WILL_FAIL true)
add_test(NAME crash_mod_by_0 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/mod-by-0.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST crash_mod_by_0 PROPERTY WILL_FAIL true)
add_test(NAME crash_redefine_macro_parameter_in_macro_body COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/redefine-macro-parameter-in-macro-body.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST crash_redefine_macro_parameter_in_macro_body PROPERTY WILL_FAIL true)
add_test(NAME crash_shift_by_massive_val COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/shift-by-massive-val.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
add_test(NAME crash_shift_no_args COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/shift-no-args.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST crash_shift_no_args PROPERTY WILL_FAIL true)
add_test(NAME leak_equal_with_trailing_garbage_identifier COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-leaking/equal-with-trailing-garbage-identifier.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST leak_equal_with_trailing_garbage_identifier PROPERTY WILL_FAIL true)
add_test(NAME leak_label_definition_followed_by_redefinition_as_macro COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-leaking/label-definition-followed-by-redefinition-as-macro.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST leak_label_definition_followed_by_redefinition_as_macro PROPERTY WILL_FAIL true)
add_test(NAME leak_label_definition_followed_by_redefinition_as_macro_with_parameter COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-leaking/label-definition-followed-by-redefinition-as-macro-with-parameter.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST leak_label_definition_followed_by_redefinition_as_macro_with_parameter PROPERTY WILL_FAIL true)
add_test(NAME leak_label_definition_followed_by_redefinition_as_macro_with_code_line COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-leaking/label-definition-followed-by-redefinition-as-macro-with-code-line.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST leak_label_definition_followed_by_redefinition_as_macro_with_code_line PROPERTY WILL_FAIL true)
add_test(NAME crash_redefine_macro_in_macro_body COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/redefine-macro-in-macro-body.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST crash_redefine_macro_in_macro_body PROPERTY WILL_FAIL true)
add_test(NAME crash_define_global_label_then_redefine_at_sign_as_macro_twice COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/define-global-label-then-redefine-at-sign-as-macro-twice.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST crash_define_global_label_then_redefine_at_sign_as_macro_twice PROPERTY WILL_FAIL true)
add_test(NAME crash_purge_macro COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/purge-macro.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST crash_purge_macro PROPERTY WILL_FAIL true)
add_test(NAME crash_redefine_macro_as_another_macro_while_it_is_being_expanded COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/redefine-macro-as-another-macro-while-it-is-being-expanded.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST crash_redefine_macro_as_another_macro_while_it_is_being_expanded PROPERTY WILL_FAIL true)
add_test(NAME crash_macro_infinite_recursion COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/macro-infinite-recursion.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST crash_macro_infinite_recursion PROPERTY WILL_FAIL true)
add_test(NAME crash_substr_out_of_bounds COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/substr-out-of-bounds.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
add_test(NAME leak_multiple_unterminated_repts COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-leaking/multiple-unterminated-repts.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST leak_multiple_unterminated_repts PROPERTY WILL_FAIL true)
add_test(NAME leak_unterminated_rept_and_while COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-leaking/unterminated-rept-and-while.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST leak_unterminated_rept_and_while PROPERTY WILL_FAIL true)
add_test(NAME crash_cnop_size_boundary_0 COMMAND clownassembler -i "${CMAKE_CURRENT_SOURCE_DIR}/tests/previously-crashing/cnop-size-boundary-0.asm" -o "${CMAKE_CURRENT_BINARY_DIR}/output.bin")
set_property(TEST crash_cnop_size_boundary_0 PROPERTY WILL_FAIL true)
