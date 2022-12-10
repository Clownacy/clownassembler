/*
 * Copyright (C) 2022 Clownacy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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

void m68kasm_error(void *scanner, Statement *statement, const char *message);

%}

/* Regular expression. */
binary_digit      [0-1]
decimal_digit     [0-9]
hexadecimal_digit [0-9a-fA-F]
binary            {binary_digit}+
decimal           {decimal_digit}+
hexadecimal       {hexadecimal_digit}+

%s OPERANDS PATH

%%

 /* Ignore whitespace */
[ \t]+          ;

 /* Terminate at comment or newline. When terminating, revert to initial state. */
<<EOF>>         |
[;\r\n]         BEGIN(INITIAL); yyterminate();

 /* Directives. */
<INITIAL>ori             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ORI;
<INITIAL>andi            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ANDI;
<INITIAL>subi            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SUBI;
<INITIAL>addi            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ADDI;
<INITIAL>eori            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_EORI;
<INITIAL>cmpi            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_CMPI;
<INITIAL>btst            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BTST;
<INITIAL>bchg            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BCHG;
<INITIAL>bclr            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BCLR;
<INITIAL>bset            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BSET;
<INITIAL>movep           BEGIN(OPERANDS); return TOKEN_DIRECTIVE_MOVEP;
<INITIAL>movea           BEGIN(OPERANDS); return TOKEN_DIRECTIVE_MOVEA;
<INITIAL>move            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_MOVE;
<INITIAL>negx            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_NEGX;
<INITIAL>clr             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_CLR;
<INITIAL>neg             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_NEG;
<INITIAL>not             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_NOT;
<INITIAL>ext             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_EXT;
<INITIAL>nbcd            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_NBCD;
<INITIAL>swap            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SWAP;
<INITIAL>pea             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_PEA;
<INITIAL>illegal         BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ILLEGAL;
<INITIAL>tas             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_TAS;
<INITIAL>tst             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_TST;
<INITIAL>trap            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_TRAP;
<INITIAL>link            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_LINK;
<INITIAL>unlk            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_UNLK;
<INITIAL>reset           BEGIN(OPERANDS); return TOKEN_DIRECTIVE_RESET;
<INITIAL>nop             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_NOP;
<INITIAL>stop            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_STOP;
<INITIAL>rte             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_RTE;
<INITIAL>rts             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_RTS;
<INITIAL>trapv           BEGIN(OPERANDS); return TOKEN_DIRECTIVE_TRAPV;
<INITIAL>rtr             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_RTR;
<INITIAL>jsr             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_JSR;
<INITIAL>jmp             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_JMP;
<INITIAL>movem           BEGIN(OPERANDS); return TOKEN_DIRECTIVE_MOVEM;
<INITIAL>lea             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_LEA;
<INITIAL>chk             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_CHK;
<INITIAL>addq            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ADDQ;
<INITIAL>subq            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SUBQ;
<INITIAL>st              BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ST;
<INITIAL>sf              BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SF;
<INITIAL>shi             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SHI;
<INITIAL>sls             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SLS;
<INITIAL>scc             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SCC;
<INITIAL>scs             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SCS;
<INITIAL>sne             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SNE;
<INITIAL>seq             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SEQ;
<INITIAL>svc             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SVC;
<INITIAL>svs             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SVS;
<INITIAL>spl             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SPL;
<INITIAL>smi             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SMI;
<INITIAL>sge             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SGE;
<INITIAL>slt             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SLT;
<INITIAL>sgt             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SGT;
<INITIAL>sle             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SLE;
<INITIAL>shs             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SCC;
<INITIAL>slo             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SCS;
<INITIAL>dbt             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBT;
<INITIAL>dbf             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBF;
<INITIAL>dbhi            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBHI;
<INITIAL>dbls            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBLS;
<INITIAL>dbcc            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBCC;
<INITIAL>dbcs            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBCS;
<INITIAL>dbne            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBNE;
<INITIAL>dbeq            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBEQ;
<INITIAL>dbvc            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBVC;
<INITIAL>dbvs            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBVS;
<INITIAL>dbpl            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBPL;
<INITIAL>dbmi            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBMI;
<INITIAL>dbge            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBGE;
<INITIAL>dblt            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBLT;
<INITIAL>dbgt            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBGT;
<INITIAL>dble            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBLE;
<INITIAL>dbhs            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBCC;
<INITIAL>dblo            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBCS;
<INITIAL>dbra            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DBF;
<INITIAL>bra             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BRA;
<INITIAL>bsr             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BSR;
<INITIAL>bhi             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BHI;
<INITIAL>bls             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BLS;
<INITIAL>bcc             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BCC;
<INITIAL>bcs             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BCS;
<INITIAL>bne             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BNE;
<INITIAL>beq             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BEQ;
<INITIAL>bvc             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BVC;
<INITIAL>bvs             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BVS;
<INITIAL>bpl             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BPL;
<INITIAL>bmi             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BMI;
<INITIAL>bge             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BGE;
<INITIAL>blt             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BLT;
<INITIAL>bgt             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BGT;
<INITIAL>ble             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BLE;
<INITIAL>bhs             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BCC;
<INITIAL>blo             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_BCS;
<INITIAL>moveq           BEGIN(OPERANDS); return TOKEN_DIRECTIVE_MOVEQ;
<INITIAL>divu            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DIVU;
<INITIAL>divs            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DIVS;
<INITIAL>sbcd            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SBCD;
<INITIAL>or              BEGIN(OPERANDS); return TOKEN_DIRECTIVE_OR;
<INITIAL>sub             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SUB;
<INITIAL>subx            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SUBX;
<INITIAL>suba            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SUBA;
<INITIAL>eor             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_EOR;
<INITIAL>cmpm            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_CMPM;
<INITIAL>cmp             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_CMP;
<INITIAL>cmpa            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_CMPA;
<INITIAL>mulu            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_MULU;
<INITIAL>muls            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_MULS;
<INITIAL>abcd            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ABCD;
<INITIAL>exg             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_EXG;
<INITIAL>and             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_AND;
<INITIAL>add             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ADD;
<INITIAL>addx            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ADDX;
<INITIAL>adda            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ADDA;
<INITIAL>asl             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ASL;
<INITIAL>asr             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ASR;
<INITIAL>lsl             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_LSL;
<INITIAL>lsr             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_LSR;
<INITIAL>roxl            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ROXL;
<INITIAL>roxr            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ROXR;
<INITIAL>rol             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ROL;
<INITIAL>ror             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ROR;
<INITIAL>dc              BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DC;
<INITIAL>dcb             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_DCB;
<INITIAL>rept            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_REPT;
<INITIAL>endr            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ENDR;
<INITIAL>macro           BEGIN(OPERANDS); return TOKEN_DIRECTIVE_MACRO;
<INITIAL>macros          BEGIN(OPERANDS); return TOKEN_DIRECTIVE_MACROS;
<INITIAL>endm            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ENDM;
<INITIAL>include         BEGIN(PATH); return TOKEN_DIRECTIVE_INCLUDE;
<INITIAL>incbin          BEGIN(PATH); return TOKEN_DIRECTIVE_INCBIN;
<INITIAL>binclude        BEGIN(PATH); return TOKEN_DIRECTIVE_INCBIN;
<INITIAL>equ             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_EQU;
<INITIAL>set             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_SET;
<INITIAL>if              BEGIN(OPERANDS); return TOKEN_DIRECTIVE_IF;
<INITIAL>elseif          BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ELSEIF;
<INITIAL>else            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ELSE;
<INITIAL>endc            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ENDC;
<INITIAL>endif           BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ENDC;
<INITIAL>while           BEGIN(OPERANDS); return TOKEN_DIRECTIVE_WHILE;
<INITIAL>endw            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ENDW;
<INITIAL>even            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_EVEN;
<INITIAL>cnop            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_CNOP;
<INITIAL>inform          BEGIN(OPERANDS); return TOKEN_DIRECTIVE_INFORM;
<INITIAL>end             BEGIN(OPERANDS); return TOKEN_DIRECTIVE_END;
<INITIAL>rs              BEGIN(OPERANDS); return TOKEN_DIRECTIVE_RS;
<INITIAL>rsset           BEGIN(OPERANDS); return TOKEN_DIRECTIVE_RSSET;
<INITIAL>rsreset         BEGIN(OPERANDS); return TOKEN_DIRECTIVE_RSRESET;

 /* Sizes. */
".b"            return TOKEN_SIZE_BYTE;
".s"            return TOKEN_SIZE_SHORT;
".w"            return TOKEN_SIZE_WORD;
".l"            return TOKEN_SIZE_LONGWORD;

 /* Misc. symbols. */
"."             return yytext[0];
","             return yytext[0];
"("             return yytext[0];
")"             return yytext[0];
"$"             return yytext[0];
"+"             return yytext[0];
"-"             return yytext[0];
"*"             return yytext[0];
"/"             return yytext[0];
"="             return yytext[0];
"@"             return yytext[0];

 /* Operators. */
"&&"            return TOKEN_LOGICAL_AND;
"||"            return TOKEN_LOGICAL_OR;
"=="            return TOKEN_EQUALITY;   /* An assembler extension, for programmers that are familiar with C. */
"!="            return TOKEN_INEQUALITY; /* An assembler extension, for programmers that are familiar with C. */
"<>"            return TOKEN_INEQUALITY;
"><"            return TOKEN_INEQUALITY;
"<="            return TOKEN_LESS_OR_EQUAL;
">="            return TOKEN_MORE_OR_EQUAL;
"<<"            return TOKEN_LEFT_SHIFT;
">>"            return TOKEN_RIGHT_SHIFT;

 /* Operands. */
sr              return TOKEN_STATUS_REGISTER;
ccr             return TOKEN_CONDITION_CODE_REGISTER;
sp              yylval->unsigned_long = 7; return TOKEN_ADDRESS_REGISTER;
usp             return TOKEN_USER_STACK_POINTER_REGISTER;
pc              return TOKEN_PROGRAM_COUNTER;

 /* Functions. */
strlen          return TOKEN_STRLEN;

 /* Data register. */
[Dd]{decimal} {
	char *string_end;
	yylval->unsigned_long = strtoul(yytext + 1, &string_end, 10);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This decimal number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	if (yylval->unsigned_long > 7)
	{
		m68kasm_error(yyscanner, NULL, "The only valid data registers are d0 to d7.");
		return M68KASM_error;
	}

	return TOKEN_DATA_REGISTER;
}

 /* Address register. */
[Aa]{decimal} {
	char *string_end;
	yylval->unsigned_long = strtoul(yytext + 1, &string_end, 10);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This decimal number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	if (yylval->unsigned_long > 7)
	{
		m68kasm_error(yyscanner, NULL, "The only valid address registers are a0 to a7.");
		return M68KASM_error;
	}

	return TOKEN_ADDRESS_REGISTER;
}

 /* Decimal number. */
{decimal} {
	char *string_end;
	yylval->unsigned_long = strtoul(yytext, &string_end, 10);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This decimal number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	return TOKEN_NUMBER;
}

 /* Hexadecimal number (68k). */
\${hexadecimal} {
	char *string_end;
	yylval->unsigned_long = strtoul(yytext + 1, &string_end, 16);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This hexadecimal number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	return TOKEN_NUMBER;
}

 /* Binary number (68k). */
\%{binary} {
	char *string_end;
	yylval->unsigned_long = strtoul(yytext + 1, &string_end, 2);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This binary number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	return TOKEN_NUMBER;
}

 /* Hexadecimal number (Z80). */
[0-9]{hexadecimal}?h {
	char *string_end;
	yylval->unsigned_long = strtoul(yytext, &string_end, 16);

	if (string_end[1] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This hexadecimal number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	return TOKEN_NUMBER;
}

 /* Binary number (Z80). */
{binary}b {
	char *string_end;
	yylval->unsigned_long = strtoul(yytext, &string_end, 2);

	if (string_end[1] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This binary number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	return TOKEN_NUMBER;
}

 /* Hexadecimal number (C). */
0x{hexadecimal} {
	char *string_end;
	yylval->unsigned_long = strtoul(yytext + 2, &string_end, 16);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This hexadecimal number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	return TOKEN_NUMBER;
}

 /* Binary number (GNU C). */
0b{binary} {
	char *string_end;
	yylval->unsigned_long = strtoul(yytext + 2, &string_end, 2);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This binary number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	return TOKEN_NUMBER;
}

 /* String. */
\"[^\"]+\" |
\'[^\']+\' {
	const size_t length = strlen(yytext) - 2;
	yylval->string = (char*)malloc(length + 1);
	if (yylval->string == NULL)
	{
		m68kasm_error(yyscanner, NULL, "Could not allocate memory for generic string.");
		return M68KASM_error;
	}
	else
	{
		memcpy(yylval->string, yytext + 1, length);
		yylval->string[length] = '\0';
		return TOKEN_STRING;
	}
}

 /* File path (like a string, but it doesn't need to be wrapped in quotation marks). */
<PATH>[^, \t;]([^,;]*[^, \t;])? {
	const size_t length = strlen(yytext);

	BEGIN(OPERANDS);

	yylval->string = (char*)malloc(length + 1);

	if (yylval->string == NULL)
	{
		m68kasm_error(yyscanner, NULL, "Could not allocate memory for generic string.");
		return M68KASM_error;
	}
	else
	{
		memcpy(yylval->string, yytext, length + 1);
		return TOKEN_STRING;
	}
}

 /* Identifier. */
[A-Za-z_][A-Za-z_0-9\?]* {
	const size_t length = strlen(yytext);
	yylval->string = (char*)malloc(length + 1);
	if (yylval->string == NULL)
	{
		m68kasm_error(yyscanner, NULL, "Could not allocate memory for generic string.");
		return M68KASM_error;
	}
	else
	{
		memcpy(yylval->string, yytext, length + 1);
		return TOKEN_IDENTIFIER;
	}
}

 /* Local label. */
[@\.][A-Za-z_0-9\?]+ {
	const size_t length = strlen(yytext);
	yylval->string = (char*)malloc(length + 1);
	if (yylval->string == NULL)
	{
		m68kasm_error(yyscanner, NULL, "Could not allocate memory for generic string.");
		return M68KASM_error;
	}
	else
	{
		memcpy(yylval->string, yytext, length + 1);
		return TOKEN_LOCAL_IDENTIFIER;
	}
}

 /* Make Bison signal a syntax error for unrecognised symbols */
.               return yytext[0];

%%
