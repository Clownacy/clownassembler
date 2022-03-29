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

#define OPCODE_TOKEN(x) BEGIN(OPERANDS); return TOKEN_ ## x;

void m68kasm_error(void *scanner, Statement *statement, const char *message);

%}

binary_digit      [0-1]
decimal_digit     [0-9]
hexadecimal_digit [0-9a-fA-F]
binary            {binary_digit}+
decimal           {decimal_digit}+
hexadecimal       {hexadecimal_digit}+

%s OPERANDS

%%

[ \t]+          ; /* Ignore whitespace */
<<EOF>>         |
[;\r\n]         BEGIN(INITIAL); yyterminate(); /* Terminate at comment or newline */
<INITIAL>ori             OPCODE_TOKEN(OPCODE_ORI)
<INITIAL>andi            OPCODE_TOKEN(OPCODE_ANDI)
<INITIAL>subi            OPCODE_TOKEN(OPCODE_SUBI)
<INITIAL>addi            OPCODE_TOKEN(OPCODE_ADDI)
<INITIAL>eori            OPCODE_TOKEN(OPCODE_EORI)
<INITIAL>cmpi            OPCODE_TOKEN(OPCODE_CMPI)
<INITIAL>btst            OPCODE_TOKEN(OPCODE_BTST)
<INITIAL>bchg            OPCODE_TOKEN(OPCODE_BCHG)
<INITIAL>bclr            OPCODE_TOKEN(OPCODE_BCLR)
<INITIAL>bset            OPCODE_TOKEN(OPCODE_BSET)
<INITIAL>movep           OPCODE_TOKEN(OPCODE_MOVEP)
<INITIAL>movea           OPCODE_TOKEN(OPCODE_MOVEA)
<INITIAL>move            OPCODE_TOKEN(OPCODE_MOVE)
<INITIAL>negx            OPCODE_TOKEN(OPCODE_NEGX)
<INITIAL>clr             OPCODE_TOKEN(OPCODE_CLR)
<INITIAL>neg             OPCODE_TOKEN(OPCODE_NEG)
<INITIAL>not             OPCODE_TOKEN(OPCODE_NOT)
<INITIAL>ext             OPCODE_TOKEN(OPCODE_EXT)
<INITIAL>nbcd            OPCODE_TOKEN(OPCODE_NBCD)
<INITIAL>swap            OPCODE_TOKEN(OPCODE_SWAP)
<INITIAL>pea             OPCODE_TOKEN(OPCODE_PEA)
<INITIAL>illegal         OPCODE_TOKEN(OPCODE_ILLEGAL)
<INITIAL>tas             OPCODE_TOKEN(OPCODE_TAS)
<INITIAL>tst             OPCODE_TOKEN(OPCODE_TST)
<INITIAL>trap            OPCODE_TOKEN(OPCODE_TRAP)
<INITIAL>link            OPCODE_TOKEN(OPCODE_LINK)
<INITIAL>unlk            OPCODE_TOKEN(OPCODE_UNLK)
<INITIAL>reset           OPCODE_TOKEN(OPCODE_RESET)
<INITIAL>nop             OPCODE_TOKEN(OPCODE_NOP)
<INITIAL>stop            OPCODE_TOKEN(OPCODE_STOP)
<INITIAL>rte             OPCODE_TOKEN(OPCODE_RTE)
<INITIAL>rts             OPCODE_TOKEN(OPCODE_RTS)
<INITIAL>trapv           OPCODE_TOKEN(OPCODE_TRAPV)
<INITIAL>rtr             OPCODE_TOKEN(OPCODE_RTR)
<INITIAL>jsr             OPCODE_TOKEN(OPCODE_JSR)
<INITIAL>jmp             OPCODE_TOKEN(OPCODE_JMP)
<INITIAL>movem           OPCODE_TOKEN(OPCODE_MOVEM)
<INITIAL>lea             OPCODE_TOKEN(OPCODE_LEA)
<INITIAL>chk             OPCODE_TOKEN(OPCODE_CHK)
<INITIAL>addq            OPCODE_TOKEN(OPCODE_ADDQ)
<INITIAL>subq            OPCODE_TOKEN(OPCODE_SUBQ)
<INITIAL>st              OPCODE_TOKEN(OPCODE_ST)
<INITIAL>sf              OPCODE_TOKEN(OPCODE_SF)
<INITIAL>shi             OPCODE_TOKEN(OPCODE_SHI)
<INITIAL>sls             OPCODE_TOKEN(OPCODE_SLS)
<INITIAL>scc             OPCODE_TOKEN(OPCODE_SCC)
<INITIAL>scs             OPCODE_TOKEN(OPCODE_SCS)
<INITIAL>sne             OPCODE_TOKEN(OPCODE_SNE)
<INITIAL>seq             OPCODE_TOKEN(OPCODE_SEQ)
<INITIAL>svc             OPCODE_TOKEN(OPCODE_SVC)
<INITIAL>svs             OPCODE_TOKEN(OPCODE_SVS)
<INITIAL>spl             OPCODE_TOKEN(OPCODE_SPL)
<INITIAL>smi             OPCODE_TOKEN(OPCODE_SMI)
<INITIAL>sge             OPCODE_TOKEN(OPCODE_SGE)
<INITIAL>slt             OPCODE_TOKEN(OPCODE_SLT)
<INITIAL>sgt             OPCODE_TOKEN(OPCODE_SGT)
<INITIAL>sle             OPCODE_TOKEN(OPCODE_SLE)
<INITIAL>shs             OPCODE_TOKEN(OPCODE_SCC)
<INITIAL>slo             OPCODE_TOKEN(OPCODE_SCS)
<INITIAL>dbt             OPCODE_TOKEN(OPCODE_DBT)
<INITIAL>dbf             OPCODE_TOKEN(OPCODE_DBF)
<INITIAL>dbhi            OPCODE_TOKEN(OPCODE_DBHI)
<INITIAL>dbls            OPCODE_TOKEN(OPCODE_DBLS)
<INITIAL>dbcc            OPCODE_TOKEN(OPCODE_DBCC)
<INITIAL>dbcs            OPCODE_TOKEN(OPCODE_DBCS)
<INITIAL>dbne            OPCODE_TOKEN(OPCODE_DBNE)
<INITIAL>dbeq            OPCODE_TOKEN(OPCODE_DBEQ)
<INITIAL>dbvc            OPCODE_TOKEN(OPCODE_DBVC)
<INITIAL>dbvs            OPCODE_TOKEN(OPCODE_DBVS)
<INITIAL>dbpl            OPCODE_TOKEN(OPCODE_DBPL)
<INITIAL>dbmi            OPCODE_TOKEN(OPCODE_DBMI)
<INITIAL>dbge            OPCODE_TOKEN(OPCODE_DBGE)
<INITIAL>dblt            OPCODE_TOKEN(OPCODE_DBLT)
<INITIAL>dbgt            OPCODE_TOKEN(OPCODE_DBGT)
<INITIAL>dble            OPCODE_TOKEN(OPCODE_DBLE)
<INITIAL>dbhs            OPCODE_TOKEN(OPCODE_DBCC)
<INITIAL>dblo            OPCODE_TOKEN(OPCODE_DBCS)
<INITIAL>dbra            OPCODE_TOKEN(OPCODE_DBF)
<INITIAL>bra             OPCODE_TOKEN(OPCODE_BRA)
<INITIAL>bsr             OPCODE_TOKEN(OPCODE_BSR)
<INITIAL>bhi             OPCODE_TOKEN(OPCODE_BHI)
<INITIAL>bls             OPCODE_TOKEN(OPCODE_BLS)
<INITIAL>bcc             OPCODE_TOKEN(OPCODE_BCC)
<INITIAL>bcs             OPCODE_TOKEN(OPCODE_BCS)
<INITIAL>bne             OPCODE_TOKEN(OPCODE_BNE)
<INITIAL>beq             OPCODE_TOKEN(OPCODE_BEQ)
<INITIAL>bvc             OPCODE_TOKEN(OPCODE_BVC)
<INITIAL>bvs             OPCODE_TOKEN(OPCODE_BVS)
<INITIAL>bpl             OPCODE_TOKEN(OPCODE_BPL)
<INITIAL>bmi             OPCODE_TOKEN(OPCODE_BMI)
<INITIAL>bge             OPCODE_TOKEN(OPCODE_BGE)
<INITIAL>blt             OPCODE_TOKEN(OPCODE_BLT)
<INITIAL>bgt             OPCODE_TOKEN(OPCODE_BGT)
<INITIAL>ble             OPCODE_TOKEN(OPCODE_BLE)
<INITIAL>bhs             OPCODE_TOKEN(OPCODE_BCC)
<INITIAL>blo             OPCODE_TOKEN(OPCODE_BCS)
<INITIAL>moveq           OPCODE_TOKEN(OPCODE_MOVEQ)
<INITIAL>divu            OPCODE_TOKEN(OPCODE_DIVU)
<INITIAL>divs            OPCODE_TOKEN(OPCODE_DIVS)
<INITIAL>sbcd            OPCODE_TOKEN(OPCODE_SBCD)
<INITIAL>or              OPCODE_TOKEN(OPCODE_OR)
<INITIAL>sub             OPCODE_TOKEN(OPCODE_SUB)
<INITIAL>subx            OPCODE_TOKEN(OPCODE_SUBX)
<INITIAL>suba            OPCODE_TOKEN(OPCODE_SUBA)
<INITIAL>eor             OPCODE_TOKEN(OPCODE_EOR)
<INITIAL>cmpm            OPCODE_TOKEN(OPCODE_CMPM)
<INITIAL>cmp             OPCODE_TOKEN(OPCODE_CMP)
<INITIAL>cmpa            OPCODE_TOKEN(OPCODE_CMPA)
<INITIAL>mulu            OPCODE_TOKEN(OPCODE_MULU)
<INITIAL>muls            OPCODE_TOKEN(OPCODE_MULS)
<INITIAL>abcd            OPCODE_TOKEN(OPCODE_ABCD)
<INITIAL>exg             OPCODE_TOKEN(OPCODE_EXG)
<INITIAL>and             OPCODE_TOKEN(OPCODE_AND)
<INITIAL>add             OPCODE_TOKEN(OPCODE_ADD)
<INITIAL>addx            OPCODE_TOKEN(OPCODE_ADDX)
<INITIAL>adda            OPCODE_TOKEN(OPCODE_ADDA)
<INITIAL>asl             OPCODE_TOKEN(OPCODE_ASL)
<INITIAL>asr             OPCODE_TOKEN(OPCODE_ASR)
<INITIAL>lsl             OPCODE_TOKEN(OPCODE_LSL)
<INITIAL>lsr             OPCODE_TOKEN(OPCODE_LSR)
<INITIAL>roxl            OPCODE_TOKEN(OPCODE_ROXL)
<INITIAL>roxr            OPCODE_TOKEN(OPCODE_ROXR)
<INITIAL>rol             OPCODE_TOKEN(OPCODE_ROL)
<INITIAL>ror             OPCODE_TOKEN(OPCODE_ROR)
<INITIAL>dc              OPCODE_TOKEN(DIRECTIVE_DC)
<INITIAL>dcb             OPCODE_TOKEN(DIRECTIVE_DCB)
<INITIAL>rept            OPCODE_TOKEN(DIRECTIVE_REPT)
<INITIAL>endr            OPCODE_TOKEN(DIRECTIVE_ENDR)
<INITIAL>macro           OPCODE_TOKEN(DIRECTIVE_MACRO)
<INITIAL>macros          OPCODE_TOKEN(DIRECTIVE_MACROS)
<INITIAL>endm            OPCODE_TOKEN(DIRECTIVE_ENDM)
<INITIAL>include         OPCODE_TOKEN(DIRECTIVE_INCLUDE)
<INITIAL>incbin          OPCODE_TOKEN(DIRECTIVE_INCBIN)
<INITIAL>binclude        OPCODE_TOKEN(DIRECTIVE_INCBIN)
<INITIAL>equ             OPCODE_TOKEN(DIRECTIVE_EQU)
<INITIAL>set             OPCODE_TOKEN(DIRECTIVE_SET)
<INITIAL>if              OPCODE_TOKEN(DIRECTIVE_IF)
<INITIAL>else            OPCODE_TOKEN(DIRECTIVE_ELSE)
<INITIAL>endc            OPCODE_TOKEN(DIRECTIVE_ENDC)
<INITIAL>endif           OPCODE_TOKEN(DIRECTIVE_ENDC)
<INITIAL>even            OPCODE_TOKEN(DIRECTIVE_EVEN)
<INITIAL>cnop            OPCODE_TOKEN(DIRECTIVE_CNOP)
<INITIAL>inform          OPCODE_TOKEN(DIRECTIVE_INFORM)
<INITIAL>end             OPCODE_TOKEN(DIRECTIVE_END)
<INITIAL>rs              OPCODE_TOKEN(DIRECTIVE_RS)
<INITIAL>rsset           OPCODE_TOKEN(DIRECTIVE_RSSET)
<INITIAL>rsreset         OPCODE_TOKEN(DIRECTIVE_RSRESET)
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
"@"             return yytext[0];
"&&"            TOKEN(LOGICAL_AND)
"||"            TOKEN(LOGICAL_OR)
"=="            TOKEN(EQUALITY)   /* An assembler extension, for programmers that are familiar with C. */
"!="            TOKEN(INEQUALITY) /* An assembler extension, for programmers that are familiar with C. */
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

[A-Za-z_][A-Za-z_0-9\?]* {
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

[@\.][A-Za-z_0-9\?]+ {
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
