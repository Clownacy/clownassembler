cmake_minimum_required(VERSION 3.20)

project(clownassembler LANGUAGES C)

add_executable(clownassembler
	"main.c"
	"lexer.c"
	"parser.c"
)

set_target_properties(clownassembler PROPERTIES
	C_STANDARD 90
	C_STANDARD_REQUIRED YES
	C_EXTENSIONS OFF
)

#target_compile_definitions(clownassembler PRIVATE PRINT)
