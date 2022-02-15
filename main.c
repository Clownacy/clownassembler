#include <stdio.h>
#include <stdlib.h>

#include "types.h"

#define YY_NO_UNISTD_H
#include "lexer.h"
#include "parser.h"

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
			ERROR("Could not open file");
		}
		else
		{
			/*yylex(); */

		#if YYDEBUG
			yydebug = 1;
		#endif

			(void)yyparse();

			fclose(yyin);

			/* Parse the parse tree */
			{
				StatementListNode *statement_list_node;

				for (statement_list_node = statement_list_head; statement_list_node != NULL; statement_list_node = statement_list_node->next)
				{
					fprintf(stderr, "What do we have here? A statement of type %d!\n", statement_list_node->statement.type);

					switch (statement_list_node->statement.type)
					{
						case STATEMENT_TYPE_EMPTY:
							fprintf(stderr, "What do we have here? An empty statement!\n");
							break;

						case STATEMENT_TYPE_INSTRUCTION:
							fprintf(stderr, "What do we have here? An instruction of type %d!\n", statement_list_node->statement.data.instruction.opcode.type);
							break;

						case STATEMENT_TYPE_MACRO:
							fprintf(stderr, "What do we have here? A macro!\n");
							break;
					}
				}
			}
		}
	}

	return exit_code;
}

void yyerror(char *s)
{
    fprintf(stderr, "Error : Exiting %s\n", s);
}
