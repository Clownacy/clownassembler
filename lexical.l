%option never-interactive
%option nounistd
%option nounput noinput noyywrap
%option reentrant bison-bridge
%option prefix="m68kasm_"
%option caseless

%{

#include <stdlib.h>
#include <string.h>

#include "syntactic.h"

#ifdef PRINT
#include <stdio.h>
#define TOKEN(x) fputs("Token: TOKEN_" #x "\n", stdout);
#define CHARACTER_TOKEN(x) fprintf(stdout, "Token: TOKEN_" #x ", character: %c\n", yytext[1]);
#define NUMBER_TOKEN(x) fprintf(stdout, "Token: TOKEN_" #x ", number: %s\n", yytext);
#define REAL_NUMBER_TOKEN(x) fprintf(stdout, "Token: TOKEN_" #x ", number: %s\n", yytext);
#define IDENTIFIER_TOKEN(x) fputs("Token: TOKEN_" #x "\n", stdout);
#else
#define TOKEN(x) return TOKEN_ ## x;
#define CHARACTER_TOKEN(x) yylval.iVal = yytext[1]; return TOKEN_ ## x;
#define NUMBER_TOKEN(x) yylval.iVal = strtol(yytext, NULL, 10); return TOKEN_ ## x;
#define REAL_NUMBER_TOKEN(x) strncpy(yylval.fVal, yytext, REALLENGTH - 1); yylval.fVal[REALLENGTH - 1] = '\0'; return TOKEN_ ## x;
#define IDENTIFIER_TOKEN(x) yylval.iVal = installId(yytext); return TOKEN_ ## x;
#endif

void m68kasm_error(void *scanner, Statement *statement, const char *message);

%}

binary_digit      [0-1]
decimal_digit     [0-9]
hexadecimal_digit [0-9a-fA-F]
binary            {binary_digit}+
decimal           {decimal_digit}+
hexadecimal       {hexadecimal_digit}+

%%

[ \t]+          ; /* Ignore whitespace */
[;\r\n]         yyterminate(); ; /* Terminate at comment or newline */
ori             TOKEN(OPCODE_ORI)
andi            TOKEN(OPCODE_ANDI)
subi            TOKEN(OPCODE_SUBI)
addi            TOKEN(OPCODE_ADDI)
eori            TOKEN(OPCODE_EORI)
cmpi            TOKEN(OPCODE_CMPI)
btst            TOKEN(OPCODE_BTST)
bchg            TOKEN(OPCODE_BCHG)
bclr            TOKEN(OPCODE_BCLR)
bset            TOKEN(OPCODE_BSET)
movep           TOKEN(OPCODE_MOVEP)
movea           TOKEN(OPCODE_MOVEA)
move            TOKEN(OPCODE_MOVE)
negx            TOKEN(OPCODE_NEGX)
clr             TOKEN(OPCODE_CLR)
neg             TOKEN(OPCODE_NEG)
not             TOKEN(OPCODE_NOT)
ext             TOKEN(OPCODE_EXT)
nbcd            TOKEN(OPCODE_NBCD)
swap            TOKEN(OPCODE_SWAP)
pea             TOKEN(OPCODE_PEA)
illegal         TOKEN(OPCODE_ILLEGAL)
tas             TOKEN(OPCODE_TAS)
tst             TOKEN(OPCODE_TST)
trap            TOKEN(OPCODE_TRAP)
link            TOKEN(OPCODE_LINK)
unlk            TOKEN(OPCODE_UNLK)
reset           TOKEN(OPCODE_RESET)
nop             TOKEN(OPCODE_NOP)
stop            TOKEN(OPCODE_STOP)
rte             TOKEN(OPCODE_RTE)
rts             TOKEN(OPCODE_RTS)
trapv           TOKEN(OPCODE_TRAPV)
rtr             TOKEN(OPCODE_RTR)
jsr             TOKEN(OPCODE_JSR)
jmp             TOKEN(OPCODE_JMP)
movem           TOKEN(OPCODE_MOVEM)
lea             TOKEN(OPCODE_LEA)
chk             TOKEN(OPCODE_CHK)
addq            TOKEN(OPCODE_ADDQ)
subq            TOKEN(OPCODE_SUBQ)
st              TOKEN(OPCODE_ST)
sf              TOKEN(OPCODE_SF)
shi             TOKEN(OPCODE_SHI)
sls             TOKEN(OPCODE_SLS)
scc             TOKEN(OPCODE_SCC)
scs             TOKEN(OPCODE_SCS)
sne             TOKEN(OPCODE_SNE)
seq             TOKEN(OPCODE_SEQ)
svc             TOKEN(OPCODE_SVC)
svs             TOKEN(OPCODE_SVS)
spl             TOKEN(OPCODE_SPL)
smi             TOKEN(OPCODE_SMI)
sge             TOKEN(OPCODE_SGE)
slt             TOKEN(OPCODE_SLT)
sgt             TOKEN(OPCODE_SGT)
sle             TOKEN(OPCODE_SLE)
shs             TOKEN(OPCODE_SCC)
slo             TOKEN(OPCODE_SCS)
dbt             TOKEN(OPCODE_DBT)
dbf             TOKEN(OPCODE_DBF)
dbhi            TOKEN(OPCODE_DBHI)
dbls            TOKEN(OPCODE_DBLS)
dbcc            TOKEN(OPCODE_DBCC)
dbcs            TOKEN(OPCODE_DBCS)
dbne            TOKEN(OPCODE_DBNE)
dbeq            TOKEN(OPCODE_DBEQ)
dbvc            TOKEN(OPCODE_DBVC)
dbvs            TOKEN(OPCODE_DBVS)
dbpl            TOKEN(OPCODE_DBPL)
dbmi            TOKEN(OPCODE_DBMI)
dbge            TOKEN(OPCODE_DBGE)
dblt            TOKEN(OPCODE_DBLT)
dbgt            TOKEN(OPCODE_DBGT)
dble            TOKEN(OPCODE_DBLE)
dbhs            TOKEN(OPCODE_DBCC)
dblo            TOKEN(OPCODE_DBCS)
dbra            TOKEN(OPCODE_DBF)
bra             TOKEN(OPCODE_BRA)
bsr             TOKEN(OPCODE_BSR)
bhi             TOKEN(OPCODE_BHI)
bls             TOKEN(OPCODE_BLS)
bcc             TOKEN(OPCODE_BCC)
bcs             TOKEN(OPCODE_BCS)
bne             TOKEN(OPCODE_BNE)
beq             TOKEN(OPCODE_BEQ)
bvc             TOKEN(OPCODE_BVC)
bvs             TOKEN(OPCODE_BVS)
bpl             TOKEN(OPCODE_BPL)
bmi             TOKEN(OPCODE_BMI)
bge             TOKEN(OPCODE_BGE)
blt             TOKEN(OPCODE_BLT)
bgt             TOKEN(OPCODE_BGT)
ble             TOKEN(OPCODE_BLE)
bhs             TOKEN(OPCODE_BCC)
blo             TOKEN(OPCODE_BCS)
moveq           TOKEN(OPCODE_MOVEQ)
divu            TOKEN(OPCODE_DIVU)
divs            TOKEN(OPCODE_DIVS)
sbcd            TOKEN(OPCODE_SBCD)
or              TOKEN(OPCODE_OR)
sub             TOKEN(OPCODE_SUB)
subx            TOKEN(OPCODE_SUBX)
suba            TOKEN(OPCODE_SUBA)
eor             TOKEN(OPCODE_EOR)
cmpm            TOKEN(OPCODE_CMPM)
cmp             TOKEN(OPCODE_CMP)
cmpa            TOKEN(OPCODE_CMPA)
mulu            TOKEN(OPCODE_MULU)
muls            TOKEN(OPCODE_MULS)
abcd            TOKEN(OPCODE_ABCD)
exg             TOKEN(OPCODE_EXG)
and             TOKEN(OPCODE_AND)
add             TOKEN(OPCODE_ADD)
addx            TOKEN(OPCODE_ADDX)
adda            TOKEN(OPCODE_ADDA)
asl             TOKEN(OPCODE_ASL)
asr             TOKEN(OPCODE_ASR)
lsl             TOKEN(OPCODE_LSL)
lsr             TOKEN(OPCODE_LSR)
roxl            TOKEN(OPCODE_ROXL)
roxr            TOKEN(OPCODE_ROXR)
rol             TOKEN(OPCODE_ROL)
ror             TOKEN(OPCODE_ROR)
dc              TOKEN(DIRECTIVE_DC)
dcb             TOKEN(DIRECTIVE_DCB)
rept            TOKEN(DIRECTIVE_REPT)
endr            TOKEN(DIRECTIVE_ENDR)
macro           TOKEN(DIRECTIVE_MACRO)
endm            TOKEN(DIRECTIVE_ENDM)
include         TOKEN(DIRECTIVE_INCLUDE)
incbin          TOKEN(DIRECTIVE_INCBIN)
binclude        TOKEN(DIRECTIVE_INCBIN)
equ             TOKEN(DIRECTIVE_EQU)
set             TOKEN(DIRECTIVE_SET)
if              TOKEN(DIRECTIVE_IF)
else            TOKEN(DIRECTIVE_ELSE)
endc            TOKEN(DIRECTIVE_ENDC)
endif           TOKEN(DIRECTIVE_ENDC)
even            TOKEN(DIRECTIVE_EVEN)
cnop            TOKEN(DIRECTIVE_CNOP)
inform          TOKEN(DIRECTIVE_INFORM)
end             TOKEN(DIRECTIVE_END)
rs              TOKEN(DIRECTIVE_RS)
rsset           TOKEN(DIRECTIVE_RSSET)
rsreset         TOKEN(DIRECTIVE_RSRESET)
"."             return yytext[0];
","             return yytext[0];
"("             return yytext[0];
")"             return yytext[0];
".b"            TOKEN(SIZE_BYTE)
".s"            TOKEN(SIZE_SHORT)
".w"            TOKEN(SIZE_WORD)
".l"            TOKEN(SIZE_LONGWORD)
"$"             return yytext[0];
"+"             return yytext[0];
"-"             return yytext[0];
"*"             return yytext[0];
"/"             return yytext[0];
"="             return yytext[0];
"&&"            TOKEN(LOGICAL_AND)
"||"            TOKEN(LOGICAL_OR)
"=="            TOKEN(EQUALITY)
"<>"            TOKEN(INEQUALITY)
"<="            TOKEN(LESS_OR_EQUAL)
">="            TOKEN(MORE_OR_EQUAL)
"<<"            TOKEN(LEFT_SHIFT)
">>"            TOKEN(RIGHT_SHIFT)
sr              TOKEN(STATUS_REGISTER)
ccr             TOKEN(CONDITION_CODE_REGISTER)
sp              yylval->generic.integer = 7; return TOKEN_ADDRESS_REGISTER;
usp             TOKEN(USER_STACK_POINTER_REGISTER)
pc              TOKEN(PROGRAM_COUNTER)

[Dd]{decimal} {
	char *string_end;
	yylval->generic.integer = strtoul(yytext + 1, &string_end, 10);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This decimal number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	if (yylval->generic.integer > 7)
	{
		m68kasm_error(yyscanner, NULL, "The only valid data registers are d0 to d7.");
		return M68KASM_error;
	}

	return TOKEN_DATA_REGISTER;
}

[Aa]{decimal} {
	char *string_end;
	yylval->generic.integer = strtoul(yytext + 1, &string_end, 10);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This decimal number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	if (yylval->generic.integer > 7)
	{
		m68kasm_error(yyscanner, NULL, "The only valid address registers are a0 to a7.");
		return M68KASM_error;
	}

	return TOKEN_ADDRESS_REGISTER;
}

{decimal} {
	char *string_end;
	yylval->generic.integer = strtoul(yytext, &string_end, 10);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This decimal number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	return TOKEN_NUMBER;
}

\${hexadecimal} {
	char *string_end;
	yylval->generic.integer = strtoul(yytext + 1, &string_end, 16);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This hexacdecimal number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	return TOKEN_NUMBER;
}

\%{binary} {
	char *string_end;
	yylval->generic.integer = strtoul(yytext + 1, &string_end, 2);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This binary number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	return TOKEN_NUMBER;
}

[A-Za-z_][A-Za-z_0-9]* {
	const size_t length = strlen(yytext);
	yylval->generic.string = malloc(length + 1);
	if (yylval->generic.string == NULL)
	{
		m68kasm_error(yyscanner, NULL, "Could not allocate memory for generic string.");
		return M68KASM_error;
	}
	else
	{
		memcpy(yylval->generic.string, yytext, length + 1);
		return TOKEN_IDENTIFIER;
	}
}

[@\.][A-Za-z_0-9]* {
	const size_t length = strlen(yytext);
	yylval->generic.string = malloc(length + 1);
	if (yylval->generic.string == NULL)
	{
		m68kasm_error(yyscanner, NULL, "Could not allocate memory for generic string.");
		return M68KASM_error;
	}
	else
	{
		memcpy(yylval->generic.string, yytext, length + 1);
		return TOKEN_LOCAL_IDENTIFIER;
	}
}

\"[^\"]+\" |
\'[^\']+\' {
	const size_t length = strlen(yytext) - 2;
	yylval->generic.string = malloc(length + 1);
	if (yylval->generic.string == NULL)
	{
		m68kasm_error(yyscanner, NULL, "Could not allocate memory for generic string.");
		return M68KASM_error;
	}
	else
	{
		memcpy(yylval->generic.string, yytext + 1, length);
		yylval->generic.string[length] = '\0';
		return TOKEN_STRING;
	}
}

.               return yytext[0]; /* Make Bison signal a syntax error for unrecognised symbols */

%%
