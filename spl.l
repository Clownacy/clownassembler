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

whitespace      [ \t]+
letter          [A-Za-z]
digit           [0-9]
identifier      {letter}({letter}|{digit})*
number          {digit}+
newline         \r\n|\r|\n
comment         \;.*$

%%

{whitespace}    TOKEN(WHITESPACE); /* Ignore whitespace */
{comment}       ; /* Ignore comments */
"."             return yytext[0];
","             return yytext[0];
{newline}       TOKEN(NEWLINE)
move            TOKEN(OPCODE_MOVE)
add             TOKEN(OPCODE_ADD)
"("             return yytext[0];
")"             return yytext[0];
"b"             TOKEN(SIZE_BYTE)
"s"             TOKEN(SIZE_SHORT)
"w"             TOKEN(SIZE_WORD)
"l"             TOKEN(SIZE_LONG)
"$"             return yytext[0];
":"             return yytext[0];
sr              TOKEN(STATUS_REGISTER)
ccr             TOKEN(CONDITION_CODE_REGISTER)
[Dd]{number}    yylval.generic.integer = yytext[1] - '0'; return TOKEN_DATA_REGISTER;
[Aa]{number}    yylval.generic.integer = yytext[1] - '0'; return TOKEN_ADDRESS_REGISTER;
{number}        {
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
                    return TOKEN_NUMBER;
                  }
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
