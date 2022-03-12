#include <stdio.h>
#include <stdlib.h>

#include "clowncommon.h"

#include "syntactic.h"
#define YY_NO_UNISTD_H
#include "lexical.h"
#include "semantic.h"

#define ERROR(message) do { fputs("Error: " message "\n", stderr); exit_code = EXIT_FAILURE; } while (0)

void m68kasm_error(M68KASM_LTYPE *location, void *scanner, StatementListNode **statement_list_head, const char *message)
{
	(void)scanner;
	(void)statement_list_head;

	fprintf(stderr, "Bison error at line %d: %s\n", location->first_line, message);
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
		FILE *file = fopen(argv[1], "r");

		if (file == NULL)
		{
			ERROR("Could not open input file");
		}
		else
		{
			yyscan_t flex_state;
			if (m68kasm_lex_init(&flex_state) != 0)
			{
				fclose(file);

				ERROR("m68kasm_lex_init failed");
			}
			else
			{
				StatementListNode *statement_list_head;

				m68kasm_set_in(file, flex_state);

				/*m68kasm_lex(); */

			#if M68KASM_DEBUG
				m68kasm_debug = 1;
			#endif

				statement_list_head = NULL;

				if (m68kasm_parse(flex_state, &statement_list_head) != 0)
					exit_code = EXIT_FAILURE;

				if (m68kasm_lex_destroy(flex_state) != 0)
					ERROR("m68kasm_lex_destroy failed");

				fclose(file);

				if (!ProcessParseTree(statement_list_head))
					exit_code = EXIT_FAILURE;
			}
		}
	}

	return exit_code;
}
