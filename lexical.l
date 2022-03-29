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
<INITIAL>ori             BEGIN(OPERANDS); return TOKEN_OPCODE_ORI;
<INITIAL>andi            BEGIN(OPERANDS); return TOKEN_OPCODE_ANDI;
<INITIAL>subi            BEGIN(OPERANDS); return TOKEN_OPCODE_SUBI;
<INITIAL>addi            BEGIN(OPERANDS); return TOKEN_OPCODE_ADDI;
<INITIAL>eori            BEGIN(OPERANDS); return TOKEN_OPCODE_EORI;
<INITIAL>cmpi            BEGIN(OPERANDS); return TOKEN_OPCODE_CMPI;
<INITIAL>btst            BEGIN(OPERANDS); return TOKEN_OPCODE_BTST;
<INITIAL>bchg            BEGIN(OPERANDS); return TOKEN_OPCODE_BCHG;
<INITIAL>bclr            BEGIN(OPERANDS); return TOKEN_OPCODE_BCLR;
<INITIAL>bset            BEGIN(OPERANDS); return TOKEN_OPCODE_BSET;
<INITIAL>movep           BEGIN(OPERANDS); return TOKEN_OPCODE_MOVEP;
<INITIAL>movea           BEGIN(OPERANDS); return TOKEN_OPCODE_MOVEA;
<INITIAL>move            BEGIN(OPERANDS); return TOKEN_OPCODE_MOVE;
<INITIAL>negx            BEGIN(OPERANDS); return TOKEN_OPCODE_NEGX;
<INITIAL>clr             BEGIN(OPERANDS); return TOKEN_OPCODE_CLR;
<INITIAL>neg             BEGIN(OPERANDS); return TOKEN_OPCODE_NEG;
<INITIAL>not             BEGIN(OPERANDS); return TOKEN_OPCODE_NOT;
<INITIAL>ext             BEGIN(OPERANDS); return TOKEN_OPCODE_EXT;
<INITIAL>nbcd            BEGIN(OPERANDS); return TOKEN_OPCODE_NBCD;
<INITIAL>swap            BEGIN(OPERANDS); return TOKEN_OPCODE_SWAP;
<INITIAL>pea             BEGIN(OPERANDS); return TOKEN_OPCODE_PEA;
<INITIAL>illegal         BEGIN(OPERANDS); return TOKEN_OPCODE_ILLEGAL;
<INITIAL>tas             BEGIN(OPERANDS); return TOKEN_OPCODE_TAS;
<INITIAL>tst             BEGIN(OPERANDS); return TOKEN_OPCODE_TST;
<INITIAL>trap            BEGIN(OPERANDS); return TOKEN_OPCODE_TRAP;
<INITIAL>link            BEGIN(OPERANDS); return TOKEN_OPCODE_LINK;
<INITIAL>unlk            BEGIN(OPERANDS); return TOKEN_OPCODE_UNLK;
<INITIAL>reset           BEGIN(OPERANDS); return TOKEN_OPCODE_RESET;
<INITIAL>nop             BEGIN(OPERANDS); return TOKEN_OPCODE_NOP;
<INITIAL>stop            BEGIN(OPERANDS); return TOKEN_OPCODE_STOP;
<INITIAL>rte             BEGIN(OPERANDS); return TOKEN_OPCODE_RTE;
<INITIAL>rts             BEGIN(OPERANDS); return TOKEN_OPCODE_RTS;
<INITIAL>trapv           BEGIN(OPERANDS); return TOKEN_OPCODE_TRAPV;
<INITIAL>rtr             BEGIN(OPERANDS); return TOKEN_OPCODE_RTR;
<INITIAL>jsr             BEGIN(OPERANDS); return TOKEN_OPCODE_JSR;
<INITIAL>jmp             BEGIN(OPERANDS); return TOKEN_OPCODE_JMP;
<INITIAL>movem           BEGIN(OPERANDS); return TOKEN_OPCODE_MOVEM;
<INITIAL>lea             BEGIN(OPERANDS); return TOKEN_OPCODE_LEA;
<INITIAL>chk             BEGIN(OPERANDS); return TOKEN_OPCODE_CHK;
<INITIAL>addq            BEGIN(OPERANDS); return TOKEN_OPCODE_ADDQ;
<INITIAL>subq            BEGIN(OPERANDS); return TOKEN_OPCODE_SUBQ;
<INITIAL>st              BEGIN(OPERANDS); return TOKEN_OPCODE_ST;
<INITIAL>sf              BEGIN(OPERANDS); return TOKEN_OPCODE_SF;
<INITIAL>shi             BEGIN(OPERANDS); return TOKEN_OPCODE_SHI;
<INITIAL>sls             BEGIN(OPERANDS); return TOKEN_OPCODE_SLS;
<INITIAL>scc             BEGIN(OPERANDS); return TOKEN_OPCODE_SCC;
<INITIAL>scs             BEGIN(OPERANDS); return TOKEN_OPCODE_SCS;
<INITIAL>sne             BEGIN(OPERANDS); return TOKEN_OPCODE_SNE;
<INITIAL>seq             BEGIN(OPERANDS); return TOKEN_OPCODE_SEQ;
<INITIAL>svc             BEGIN(OPERANDS); return TOKEN_OPCODE_SVC;
<INITIAL>svs             BEGIN(OPERANDS); return TOKEN_OPCODE_SVS;
<INITIAL>spl             BEGIN(OPERANDS); return TOKEN_OPCODE_SPL;
<INITIAL>smi             BEGIN(OPERANDS); return TOKEN_OPCODE_SMI;
<INITIAL>sge             BEGIN(OPERANDS); return TOKEN_OPCODE_SGE;
<INITIAL>slt             BEGIN(OPERANDS); return TOKEN_OPCODE_SLT;
<INITIAL>sgt             BEGIN(OPERANDS); return TOKEN_OPCODE_SGT;
<INITIAL>sle             BEGIN(OPERANDS); return TOKEN_OPCODE_SLE;
<INITIAL>shs             BEGIN(OPERANDS); return TOKEN_OPCODE_SCC;
<INITIAL>slo             BEGIN(OPERANDS); return TOKEN_OPCODE_SCS;
<INITIAL>dbt             BEGIN(OPERANDS); return TOKEN_OPCODE_DBT;
<INITIAL>dbf             BEGIN(OPERANDS); return TOKEN_OPCODE_DBF;
<INITIAL>dbhi            BEGIN(OPERANDS); return TOKEN_OPCODE_DBHI;
<INITIAL>dbls            BEGIN(OPERANDS); return TOKEN_OPCODE_DBLS;
<INITIAL>dbcc            BEGIN(OPERANDS); return TOKEN_OPCODE_DBCC;
<INITIAL>dbcs            BEGIN(OPERANDS); return TOKEN_OPCODE_DBCS;
<INITIAL>dbne            BEGIN(OPERANDS); return TOKEN_OPCODE_DBNE;
<INITIAL>dbeq            BEGIN(OPERANDS); return TOKEN_OPCODE_DBEQ;
<INITIAL>dbvc            BEGIN(OPERANDS); return TOKEN_OPCODE_DBVC;
<INITIAL>dbvs            BEGIN(OPERANDS); return TOKEN_OPCODE_DBVS;
<INITIAL>dbpl            BEGIN(OPERANDS); return TOKEN_OPCODE_DBPL;
<INITIAL>dbmi            BEGIN(OPERANDS); return TOKEN_OPCODE_DBMI;
<INITIAL>dbge            BEGIN(OPERANDS); return TOKEN_OPCODE_DBGE;
<INITIAL>dblt            BEGIN(OPERANDS); return TOKEN_OPCODE_DBLT;
<INITIAL>dbgt            BEGIN(OPERANDS); return TOKEN_OPCODE_DBGT;
<INITIAL>dble            BEGIN(OPERANDS); return TOKEN_OPCODE_DBLE;
<INITIAL>dbhs            BEGIN(OPERANDS); return TOKEN_OPCODE_DBCC;
<INITIAL>dblo            BEGIN(OPERANDS); return TOKEN_OPCODE_DBCS;
<INITIAL>dbra            BEGIN(OPERANDS); return TOKEN_OPCODE_DBF;
<INITIAL>bra             BEGIN(OPERANDS); return TOKEN_OPCODE_BRA;
<INITIAL>bsr             BEGIN(OPERANDS); return TOKEN_OPCODE_BSR;
<INITIAL>bhi             BEGIN(OPERANDS); return TOKEN_OPCODE_BHI;
<INITIAL>bls             BEGIN(OPERANDS); return TOKEN_OPCODE_BLS;
<INITIAL>bcc             BEGIN(OPERANDS); return TOKEN_OPCODE_BCC;
<INITIAL>bcs             BEGIN(OPERANDS); return TOKEN_OPCODE_BCS;
<INITIAL>bne             BEGIN(OPERANDS); return TOKEN_OPCODE_BNE;
<INITIAL>beq             BEGIN(OPERANDS); return TOKEN_OPCODE_BEQ;
<INITIAL>bvc             BEGIN(OPERANDS); return TOKEN_OPCODE_BVC;
<INITIAL>bvs             BEGIN(OPERANDS); return TOKEN_OPCODE_BVS;
<INITIAL>bpl             BEGIN(OPERANDS); return TOKEN_OPCODE_BPL;
<INITIAL>bmi             BEGIN(OPERANDS); return TOKEN_OPCODE_BMI;
<INITIAL>bge             BEGIN(OPERANDS); return TOKEN_OPCODE_BGE;
<INITIAL>blt             BEGIN(OPERANDS); return TOKEN_OPCODE_BLT;
<INITIAL>bgt             BEGIN(OPERANDS); return TOKEN_OPCODE_BGT;
<INITIAL>ble             BEGIN(OPERANDS); return TOKEN_OPCODE_BLE;
<INITIAL>bhs             BEGIN(OPERANDS); return TOKEN_OPCODE_BCC;
<INITIAL>blo             BEGIN(OPERANDS); return TOKEN_OPCODE_BCS;
<INITIAL>moveq           BEGIN(OPERANDS); return TOKEN_OPCODE_MOVEQ;
<INITIAL>divu            BEGIN(OPERANDS); return TOKEN_OPCODE_DIVU;
<INITIAL>divs            BEGIN(OPERANDS); return TOKEN_OPCODE_DIVS;
<INITIAL>sbcd            BEGIN(OPERANDS); return TOKEN_OPCODE_SBCD;
<INITIAL>or              BEGIN(OPERANDS); return TOKEN_OPCODE_OR;
<INITIAL>sub             BEGIN(OPERANDS); return TOKEN_OPCODE_SUB;
<INITIAL>subx            BEGIN(OPERANDS); return TOKEN_OPCODE_SUBX;
<INITIAL>suba            BEGIN(OPERANDS); return TOKEN_OPCODE_SUBA;
<INITIAL>eor             BEGIN(OPERANDS); return TOKEN_OPCODE_EOR;
<INITIAL>cmpm            BEGIN(OPERANDS); return TOKEN_OPCODE_CMPM;
<INITIAL>cmp             BEGIN(OPERANDS); return TOKEN_OPCODE_CMP;
<INITIAL>cmpa            BEGIN(OPERANDS); return TOKEN_OPCODE_CMPA;
<INITIAL>mulu            BEGIN(OPERANDS); return TOKEN_OPCODE_MULU;
<INITIAL>muls            BEGIN(OPERANDS); return TOKEN_OPCODE_MULS;
<INITIAL>abcd            BEGIN(OPERANDS); return TOKEN_OPCODE_ABCD;
<INITIAL>exg             BEGIN(OPERANDS); return TOKEN_OPCODE_EXG;
<INITIAL>and             BEGIN(OPERANDS); return TOKEN_OPCODE_AND;
<INITIAL>add             BEGIN(OPERANDS); return TOKEN_OPCODE_ADD;
<INITIAL>addx            BEGIN(OPERANDS); return TOKEN_OPCODE_ADDX;
<INITIAL>adda            BEGIN(OPERANDS); return TOKEN_OPCODE_ADDA;
<INITIAL>asl             BEGIN(OPERANDS); return TOKEN_OPCODE_ASL;
<INITIAL>asr             BEGIN(OPERANDS); return TOKEN_OPCODE_ASR;
<INITIAL>lsl             BEGIN(OPERANDS); return TOKEN_OPCODE_LSL;
<INITIAL>lsr             BEGIN(OPERANDS); return TOKEN_OPCODE_LSR;
<INITIAL>roxl            BEGIN(OPERANDS); return TOKEN_OPCODE_ROXL;
<INITIAL>roxr            BEGIN(OPERANDS); return TOKEN_OPCODE_ROXR;
<INITIAL>rol             BEGIN(OPERANDS); return TOKEN_OPCODE_ROL;
<INITIAL>ror             BEGIN(OPERANDS); return TOKEN_OPCODE_ROR;
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
<INITIAL>else            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ELSE;
<INITIAL>endc            BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ENDC;
<INITIAL>endif           BEGIN(OPERANDS); return TOKEN_DIRECTIVE_ENDC;
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

 /* Hexaecimal number. */
\${hexadecimal} {
	char *string_end;
	yylval->unsigned_long = strtoul(yytext + 1, &string_end, 16);

	if (string_end[0] != '\0')
	{
		m68kasm_error(yyscanner, NULL, "This hexacdecimal number is invalid and cannot be parsed.");
		return M68KASM_error;
	}

	return TOKEN_NUMBER;
}

 /* Binary number. */
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

 /* String. */
\"[^\"]+\" |
\'[^\']+\' {
	const size_t length = strlen(yytext) - 2;
	yylval->string = malloc(length + 1);
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

	yylval->string = malloc(length + 1);

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
	yylval->string = malloc(length + 1);
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
	yylval->string = malloc(length + 1);
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
