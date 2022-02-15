#include <stdio.h>

#define YY_NO_UNISTD_H
#include "lexer.h"
#include "parser.h"

int yywrap(void)
{
	return 1;
}

int main(int argc, char **argv)
{
	yyin = fopen(argv[1], "r");

	if (yyin == NULL)
	{
		fputs("Could not open file\n", stderr);
	}
	else
	{
		/*yylex(); */

	#if YYDEBUG
		yydebug = 1;
	#endif

		(void)yyparse();


		fclose(yyin);
	}

	return 0;
}

void yyerror(char *s)
{
    fprintf(stderr, "Error : Exiting %s\n", s);
}
