%option never-interactive
%option nounistd

%{

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

%}

whitespace      [ \t]
letter          [A-Za-z]
digit           [0-9]
identifier      {letter}({letter}|{digit})*
number          {digit}+
newline         \r\n|\r|\n
comment         \;.*$

%%

{whitespace}    ; /* Ignore whitespace */
{comment}       ; /* Ignore comments */
"."             TOKEN(DOT)
","             TOKEN(COMMA)
{newline}       TOKEN(NEWLINE)
move            TOKEN(OPCODE_MOVE)
add             TOKEN(OPCODE_ADD)
"("             TOKEN(LEFT_PARENTHESIS)
")"             TOKEN(RIGHT_PARENTHESIS)
"b"             TOKEN(SIZE_BYTE)
"s"             TOKEN(SIZE_SHORT)
"w"             TOKEN(SIZE_WORD)
"l"             TOKEN(SIZE_LONG)
"$"             TOKEN(DOLLAR)
[Dd]{number}    TOKEN(DATA_REGISTER)
[Aa]{number}    TOKEN(ADDRESS_REGISTER)
{number}        TOKEN(NUMBER)

.               return yytext[0]; /* Make Bison signal a syntax error for unrecognised symbols */

%%
