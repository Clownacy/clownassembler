#include <stdio.h>
#include <stdlib.h>

#include "clowncommon.h"

#include "types.h"

#define YY_NO_UNISTD_H
#include "lexical.h"
#include "semantic.h"
#include "syntactic.h"

#define ERROR(message) do { fputs("Error: " message "\n", stderr); exit_code = EXIT_FAILURE; } while (0)

/* TODO - Stupid hack */
extern StatementListNode *statement_list_head;

int yywrap(void)
{
	return 1;
}

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	if (argc < 2)
	{
		ERROR("Input file must be passed as a parameter");
	}
	else
	{
		yyin = fopen(argv[1], "r");

		if (yyin == NULL)
		{
			ERROR("Could not open input file");
		}
		else
		{
			/*yylex(); */

		#if YYDEBUG
			yydebug = 1;
		#endif

			if (yyparse() != 0)
				exit_code = EXIT_FAILURE;

			fclose(yyin);

			if (!ProcessParseTree(statement_list_head))
				exit_code = EXIT_FAILURE;
		}
	}

	return exit_code;
}

void yyerror(char *s)
{
    fprintf(stderr, "Error : Exiting %s\n", s);
}
