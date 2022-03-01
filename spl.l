%option never-interactive
%option nounistd

%{

#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "parser.h"

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

void yyerror (char *);

%}

whitespace        [ \t]+
letter            [A-Za-z]
binary_digit      [0-1]
decimal_digit     [0-9]
hexadecimal_digit [0-9a-fA-F]
identifier        {letter}({letter}|{decimal_digit})*
binary            {binary_digit}+
decimal           {decimal_digit}+
hexadecimal       {hexadecimal_digit}+
newline           \r\n|\r|\n
comment           \;.*$

%%

{whitespace}    TOKEN(WHITESPACE); /* Ignore whitespace */
{comment}       ; /* Ignore comments */
"."             return yytext[0];
","             return yytext[0];
{newline}       TOKEN(NEWLINE)
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
"("             return yytext[0];
")"             return yytext[0];
"b"             TOKEN(SIZE_BYTE)
"s"             TOKEN(SIZE_SHORT)
"w"             TOKEN(SIZE_WORD)
"l"             TOKEN(SIZE_LONG)
"$"             return yytext[0];
":"             return yytext[0];
"+"             return yytext[0];
"-"             return yytext[0];
"/"             return yytext[0];
sr              TOKEN(STATUS_REGISTER)
ccr             TOKEN(CONDITION_CODE_REGISTER)
sp              yylval.generic.integer = 7; return TOKEN_ADDRESS_REGISTER;
usp             TOKEN(USER_STACK_POINTER_REGISTER)
pc              TOKEN(PROGRAM_COUNTER)
[Dd]{decimal}   yylval.generic.integer = yytext[1] - '0'; return TOKEN_DATA_REGISTER;
[Aa]{decimal}   yylval.generic.integer = yytext[1] - '0'; return TOKEN_ADDRESS_REGISTER;
{decimal}       {
                  char *string_end;
                  yylval.generic.integer = strtoul(yytext, &string_end, 10);

                  if (string_end[0] != '\0')
                  {
                    yyerror("Could not parse number");
                    return YYerror;
                  }

                  return TOKEN_NUMBER;
                }
\${hexadecimal} {
                  char *string_end;
                  yylval.generic.integer = strtoul(yytext + 1, &string_end, 16);

                  if (string_end[0] != '\0')
                  {
                    yyerror("Could not parse number");
                    return YYerror;
                  }

                  return TOKEN_NUMBER;
                }
\%{binary}      {
                  char *string_end;
                  yylval.generic.integer = strtoul(yytext + 1, &string_end, 2);

                  if (string_end[0] != '\0')
                  {
                    yyerror("Could not parse number");
                    return YYerror;
                  }

                  return TOKEN_NUMBER;
                }
{identifier}    {
                  const size_t length = strlen(yytext) + 1;
                  yylval.generic.string = malloc(length);
                  if (yylval.generic.string == NULL)
                  {
                    yyerror("Could not allocate memory for generic string");
                    return YYerror;
                  }
                  else
                  {
                    memcpy(yylval.generic.string, yytext, length);
                    return TOKEN_IDENTIFIER;
                  }
                }

.               return yytext[0]; /* Make Bison signal a syntax error for unrecognised symbols */

%%
