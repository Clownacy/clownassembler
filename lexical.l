/*
 * Copyright (C) 2022-2025 Clownacy
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
%option nounput noinput noyywrap nodefault
%option reentrant
%option prefix="m68kasm_"
%option caseless

%{

#include <format>
#include <stdlib.h>
#include <string.h>
#include <string_view>

#include "syntactic.h"

void m68kasm_warning(void *scanner, Statement *statement, const char *message);
void m68kasm_error(void *scanner, Statement *statement, const char *message);

static cc_bool CharacterToInteger(unsigned int* const value, const unsigned int character)
{
	if (character >= '0' && character <= '9')
		*value = 0x0 + (character - '0');
	else if (character >= 'A' && character <= 'Z')
		*value = 0xA + (character - 'A');
	else if (character >= 'a' && character <= 'z')
		*value = 0xA + (character - 'a');
	else
		return cc_false;

	return cc_true;
}

/* Like `strtoul`, except this one returns a truncated value when parsing a number that is too large. */
static cc_bool StringTo32BitInteger(unsigned long* const value, const char* const string_start, const char** const string_end, const unsigned long base)
{
	cc_bool success = cc_true;
	const unsigned long limit = 0xFFFFFFFF / base;
	const char* string_pointer;

	*value = 0;

	for (string_pointer = string_start; ; ++string_pointer)
	{
		unsigned int digit;
		if (!CharacterToInteger(&digit, *string_pointer) || digit >= base)
			break;

		if (*value > limit)
			success = cc_false;

		*value *= base;
		*value += digit;
	}

	*string_end = string_pointer;
	return success;
}

static unsigned long ParseNumber(void* const scanner, const char* const string_start, const size_t string_length, const unsigned long base)
{
	unsigned long value;
	const char *string_end;

	if (!StringTo32BitInteger(&value, string_start, &string_end, base))
	{
		/* S.N. 68k silently truncates values that are too large. This is relied upon by old versions of the Sonic 2 August 21st prototype disassembly. */
		m68kasm_warning(scanner, NULL, "Number is too large and will be truncated.");
	}

	if (string_end != string_start + string_length)
		throw m68kasm::parser::syntax_error("Number is invalid and cannot be parsed.");

	return value;
}

%}

/* Regular expression. */
binary_digit      [0-1]
decimal_digit     [0-9]
hexadecimal_digit [0-9a-fA-F]
binary            {binary_digit}+
decimal           {decimal_digit}+
hexadecimal       {hexadecimal_digit}+

%s OPERANDS PATH OPTIONS MACRO_PARAMETERS

%%

 /* Ignore whitespace */
[ \t]+          ;

 /* Terminate at comment or newline. When terminating, revert to initial state. */
<<EOF>>         |
[;\r\n]         BEGIN(INITIAL); return m68kasm::parser::make_YYEOF();

 /* Directives. */
<INITIAL>ori             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ORI();
<INITIAL>andi            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ANDI();
<INITIAL>subi            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SUBI();
<INITIAL>addi            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ADDI();
<INITIAL>eori            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_EORI();
<INITIAL>cmpi            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_CMPI();
<INITIAL>btst            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BTST();
<INITIAL>bchg            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BCHG();
<INITIAL>bclr            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BCLR();
<INITIAL>bset            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BSET();
<INITIAL>movep           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_MOVEP();
<INITIAL>movea           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_MOVEA();
<INITIAL>move            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_MOVE();
<INITIAL>negx            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_NEGX();
<INITIAL>clr             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_CLR();
<INITIAL>neg             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_NEG();
<INITIAL>not             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_NOT();
<INITIAL>ext             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_EXT();
<INITIAL>nbcd            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_NBCD();
<INITIAL>swap            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SWAP();
<INITIAL>pea             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_PEA();
<INITIAL>illegal         BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ILLEGAL();
<INITIAL>tas             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_TAS();
<INITIAL>tst             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_TST();
<INITIAL>trap            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_TRAP();
<INITIAL>link            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_LINK();
<INITIAL>unlk            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_UNLK();
<INITIAL>reset           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_RESET();
<INITIAL>nop             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_NOP();
<INITIAL>stop            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_STOP();
<INITIAL>rte             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_RTE();
<INITIAL>rts             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_RTS();
<INITIAL>trapv           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_TRAPV();
<INITIAL>rtr             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_RTR();
<INITIAL>jsr             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_JSR();
<INITIAL>jmp             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_JMP();
<INITIAL>movem           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_MOVEM();
<INITIAL>lea             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_LEA();
<INITIAL>chk             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_CHK();
<INITIAL>addq            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ADDQ();
<INITIAL>subq            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SUBQ();
<INITIAL>st              BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ST();
<INITIAL>sf              BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SF();
<INITIAL>shi             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SHI();
<INITIAL>sls             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SLS();
<INITIAL>scc             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SCC();
<INITIAL>scs             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SCS();
<INITIAL>sne             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SNE();
<INITIAL>seq             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SEQ();
<INITIAL>svc             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SVC();
<INITIAL>svs             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SVS();
<INITIAL>spl             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SPL();
<INITIAL>smi             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SMI();
<INITIAL>sge             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SGE();
<INITIAL>slt             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SLT();
<INITIAL>sgt             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SGT();
<INITIAL>sle             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SLE();
<INITIAL>shs             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SCC();
<INITIAL>slo             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SCS();
<INITIAL>dbt             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBT();
<INITIAL>dbf             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBF();
<INITIAL>dbhi            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBHI();
<INITIAL>dbls            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBLS();
<INITIAL>dbcc            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBCC();
<INITIAL>dbcs            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBCS();
<INITIAL>dbne            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBNE();
<INITIAL>dbeq            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBEQ();
<INITIAL>dbvc            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBVC();
<INITIAL>dbvs            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBVS();
<INITIAL>dbpl            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBPL();
<INITIAL>dbmi            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBMI();
<INITIAL>dbge            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBGE();
<INITIAL>dblt            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBLT();
<INITIAL>dbgt            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBGT();
<INITIAL>dble            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBLE();
<INITIAL>dbhs            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBCC();
<INITIAL>dblo            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBCS();
<INITIAL>dbra            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DBF();
<INITIAL>bra             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BRA();
<INITIAL>bsr             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BSR();
<INITIAL>bhi             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BHI();
<INITIAL>bls             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BLS();
<INITIAL>bcc             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BCC();
<INITIAL>bcs             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BCS();
<INITIAL>bne             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BNE();
<INITIAL>beq             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BEQ();
<INITIAL>bvc             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BVC();
<INITIAL>bvs             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BVS();
<INITIAL>bpl             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BPL();
<INITIAL>bmi             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BMI();
<INITIAL>bge             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BGE();
<INITIAL>blt             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BLT();
<INITIAL>bgt             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BGT();
<INITIAL>ble             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BLE();
<INITIAL>bhs             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BCC();
<INITIAL>blo             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_BCS();
<INITIAL>moveq           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_MOVEQ();
<INITIAL>divu            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DIVU();
<INITIAL>divs            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DIVS();
<INITIAL>sbcd            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SBCD();
<INITIAL>or              BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_OR();
<INITIAL>sub             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SUB();
<INITIAL>subx            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SUBX();
<INITIAL>suba            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SUBA();
<INITIAL>eor             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_EOR();
<INITIAL>cmpm            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_CMPM();
<INITIAL>cmp             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_CMP();
<INITIAL>cmpa            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_CMPA();
<INITIAL>mulu            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_MULU();
<INITIAL>muls            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_MULS();
<INITIAL>abcd            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ABCD();
<INITIAL>exg             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_EXG();
<INITIAL>and             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_AND();
<INITIAL>add             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ADD();
<INITIAL>addx            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ADDX();
<INITIAL>adda            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ADDA();
<INITIAL>asl             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ASL();
<INITIAL>asr             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ASR();
<INITIAL>lsl             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_LSL();
<INITIAL>lsr             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_LSR();
<INITIAL>roxl            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ROXL();
<INITIAL>roxr            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ROXR();
<INITIAL>rol             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ROL();
<INITIAL>ror             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ROR();
<INITIAL>dc              BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DC();
<INITIAL>dcb             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DCB();
<INITIAL>ds              BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_DS();
<INITIAL>rept            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_REPT();
<INITIAL>endr            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ENDR();
<INITIAL>macro           BEGIN(MACRO_PARAMETERS); return m68kasm::parser::make_DIRECTIVE_MACRO();
<INITIAL>macros          BEGIN(MACRO_PARAMETERS); return m68kasm::parser::make_DIRECTIVE_MACROS();
<INITIAL>endm            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ENDM();
<INITIAL>include         BEGIN(PATH); return m68kasm::parser::make_DIRECTIVE_INCLUDE();
<INITIAL>incbin          BEGIN(PATH); return m68kasm::parser::make_DIRECTIVE_INCBIN();
<INITIAL>binclude        BEGIN(PATH); return m68kasm::parser::make_DIRECTIVE_INCBIN();
<INITIAL>equ             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_EQU();
<INITIAL>equs            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_EQUS();
<INITIAL>substr          BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SUBSTR();
<INITIAL>set             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SET();
<INITIAL>if              BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_IF();
<INITIAL>elseif          BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ELSEIF();
<INITIAL>else            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ELSE();
<INITIAL>endc            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ENDC();
<INITIAL>endif           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ENDC();
<INITIAL>while           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_WHILE();
<INITIAL>endw            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ENDW();
<INITIAL>even            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_EVEN();
<INITIAL>cnop            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_CNOP();
<INITIAL>inform          BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_INFORM();
<INITIAL>end             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_END();
<INITIAL>rs              BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_RS();
<INITIAL>rsset           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_RSSET();
<INITIAL>rsreset         BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_RSRESET();
<INITIAL>obj             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_OBJ();
<INITIAL>objend          BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_OBJEND();
<INITIAL>org             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ORG();
<INITIAL>pusho           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_PUSHO();
<INITIAL>popo            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_POPO();
<INITIAL>opt             BEGIN(OPTIONS);  return m68kasm::parser::make_DIRECTIVE_OPT();
<INITIAL>pushp           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_PUSHP();
<INITIAL>popp            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_POPP();
<INITIAL>shift           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SHIFT();
<INITIAL>mexit           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_MEXIT();
<INITIAL>local           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_LOCAL();
<INITIAL>"="             BEGIN(OPERANDS); return m68kasm::parser::make_EQUAL();
<INITIAL>list            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_LIST();
<INITIAL>nolist          BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_NOLIST();
<INITIAL>purge           BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_PURGE();

 /* axm68k opcodes. */
 /* axm68k is a modified version of asm68k with renamed opcodes, so that
    it could better-support Z80 macros. */
<INITIAL>axd             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ADD();
<INITIAL>axda            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ADDA();
<INITIAL>axdi            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ADDI();
<INITIAL>axdq            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ADDQ();
<INITIAL>axdx            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ADDX();
<INITIAL>anx             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_AND();
<INITIAL>anxi            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ANDI();
<INITIAL>nxg             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_NEG();
<INITIAL>nxp             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_NOP();
<INITIAL>ox              BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_OR();
<INITIAL>oxi             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_ORI();
<INITIAL>sxb             BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SUB();
<INITIAL>sxba            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SUBA();
<INITIAL>sxbi            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SUBI();
<INITIAL>sxbq            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SUBQ();
<INITIAL>sxbx            BEGIN(OPERANDS); return m68kasm::parser::make_DIRECTIVE_SUBX();

 /* Sizes. */
".b"            return m68kasm::parser::make_SIZE_BYTE();
".s"            return m68kasm::parser::make_SIZE_SHORT();
".w"            return m68kasm::parser::make_SIZE_WORD();
".l"            return m68kasm::parser::make_SIZE_LONGWORD();

 /* Misc. symbols. */
"."             return m68kasm::parser::make_PERIOD();
","             return m68kasm::parser::make_COMMA();
"("             return m68kasm::parser::make_PARENTHESIS_LEFT();
")"             return m68kasm::parser::make_PARENTHESIS_RIGHT();
"$"             return m68kasm::parser::make_DOLLAR();
"+"             return m68kasm::parser::make_PLUS();
"-"             return m68kasm::parser::make_MINUS();
"*"             return m68kasm::parser::make_ASTERIX();
"/"             return m68kasm::parser::make_FORWARD_SLASH();
"="             return m68kasm::parser::make_EQUAL();
"@"             return m68kasm::parser::make_AT();
"#"             return m68kasm::parser::make_HASH();
"&"             return m68kasm::parser::make_AMPERSAND();
"^"             return m68kasm::parser::make_CARET();
"!"             return m68kasm::parser::make_EXCLAMATION();
"|"             return m68kasm::parser::make_VERTICAL_BAR();
"<"             return m68kasm::parser::make_ANGLE_LEFT();
">"             return m68kasm::parser::make_ANGLE_RIGHT();
"%"             return m68kasm::parser::make_PERCENTAGE();
"~"             return m68kasm::parser::make_TILDE();

 /* Operators. */
"&&"            return m68kasm::parser::make_LOGICAL_AND();
"||"            return m68kasm::parser::make_LOGICAL_OR();
"=="            return m68kasm::parser::make_EQUALITY();   /* An assembler extension, for programmers that are familiar with C. */
"!="            return m68kasm::parser::make_INEQUALITY(); /* An assembler extension, for programmers that are familiar with C. */
"<>"            return m68kasm::parser::make_INEQUALITY();
"><"            return m68kasm::parser::make_INEQUALITY();
"<="            return m68kasm::parser::make_LESS_OR_EQUAL();
">="            return m68kasm::parser::make_MORE_OR_EQUAL();
"<<"            return m68kasm::parser::make_LEFT_SHIFT();
">>"            return m68kasm::parser::make_RIGHT_SHIFT();

 /* Operands. */
sr              return m68kasm::parser::make_STATUS_REGISTER();
ccr             return m68kasm::parser::make_CONDITION_CODE_REGISTER();
sp              return m68kasm::parser::make_ADDRESS_REGISTER(7);
usp             return m68kasm::parser::make_USER_STACK_POINTER_REGISTER();
pc              return m68kasm::parser::make_PROGRAM_COUNTER();

 /* Functions. */
<OPERANDS>strlen          return m68kasm::parser::make_STRLEN();
<OPERANDS>strcmp          return m68kasm::parser::make_STRCMP();
<OPERANDS>instr           return m68kasm::parser::make_INSTR();
<OPERANDS>def             return m68kasm::parser::make_DEF();
<OPERANDS>type            return m68kasm::parser::make_TYPE();
<OPERANDS>filesize        return m68kasm::parser::make_FILESIZE();
<OPERANDS>offset          return m68kasm::parser::make_OFFSET();
<OPERANDS>sectoff         return m68kasm::parser::make_OFFSET();

 /* Decimal number. */
{decimal} {
	return m68kasm::parser::make_NUMBER(ParseNumber(yyscanner, yytext, yyleng, 10));
}

 /* Hexadecimal number (68k). */
\${hexadecimal} {
	return m68kasm::parser::make_NUMBER(ParseNumber(yyscanner, yytext + 1, yyleng - 1, 16));
}

 /* Binary number (68k). */
\%{binary} {
	return m68kasm::parser::make_NUMBER(ParseNumber(yyscanner, yytext + 1, yyleng - 1, 2));
}

 /* Hexadecimal number (Z80). */
[0-9]{hexadecimal}?h {
	return m68kasm::parser::make_NUMBER(ParseNumber(yyscanner, yytext, yyleng - 1, 16));
}

 /* Binary number (Z80). */
{binary}b {
	return m68kasm::parser::make_NUMBER(ParseNumber(yyscanner, yytext, yyleng - 1, 2));
}

 /* Hexadecimal number (C). */
0x{hexadecimal} {
	return m68kasm::parser::make_NUMBER(ParseNumber(yyscanner, yytext + 2, yyleng - 2, 16));
}

 /* Binary number (GNU C). */
0b{binary} {
	return m68kasm::parser::make_NUMBER(ParseNumber(yyscanner, yytext + 2, yyleng - 2, 2));
}

 /* String. */
(?:\"[^\"]*\")+ |
(?:\'[^\']*\')+ {
	char previous_character;

	const char quotation_character = yytext[0];

	/* Escape PATH mode so that the following arguments use the correct rules. */
	BEGIN(OPERANDS);

	StringView view(yytext + 1, yyleng - 2);
	std::string string;

	string.reserve(StringView_Length(view));

	/* Copy string, turning escaped (paired) quotes into regular (lone) quotes. */
	previous_character = '\0';

	for (const char character : view)
	{
		if (character == quotation_character && previous_character == quotation_character)
		{
			previous_character = '\0';
			continue;
		}

		previous_character = character;
		string.push_back(character);
	}

	return m68kasm::parser::make_STRING(std::move(string));
}

 /* File path (like a string, but it doesn't need to be wrapped in quotation marks). */
<PATH>[^, \t;]([^,;]*[^, \t;])? {
	/* Escape PATH mode so that the following arguments use the correct rules. */
	BEGIN(OPERANDS);

	return m68kasm::parser::make_STRING({yytext, static_cast<std::size_t>(yyleng)});
}

 /* Identifier, data register, or address register. */
[A-Za-z_][A-Za-z_0-9\?]* {
	if (yyleng == 2 && (yytext[0] == 'd' || yytext[0] == 'D' || yytext[0] == 'a' || yytext[0] == 'A') && yytext[1] >= '0' && yytext[1] <= '7')
	{
		/* Data register or address register. */
		const unsigned int value = yytext[1] - '0';
		return yytext[0] == 'd' || yytext[0] == 'D' ? m68kasm::parser::make_DATA_REGISTER(value) : m68kasm::parser::make_ADDRESS_REGISTER(value);
	}
	else
	{
		/* Identifier. */
		return m68kasm::parser::make_IDENTIFIER({yytext, static_cast<std::size_t>(yyleng)});
	}
}

 /* Local label. */
[@\.][A-Za-z_0-9\?]+ {
	return m68kasm::parser::make_LOCAL_IDENTIFIER({yytext, static_cast<std::size_t>(yyleng)});
}

 /* Option. */
<OPTIONS>[^ \t\r\n,]+ {
	return m68kasm::parser::make_OPTION({yytext, static_cast<std::size_t>(yyleng)});
}

 /* Make Bison signal a syntax error for unrecognised symbols */
.        throw m68kasm::parser::syntax_error(std::format("Invalid character '{}'.", std::string_view{yytext, static_cast<std::size_t>(yyleng)}));

%%
