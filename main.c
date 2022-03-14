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

	fprintf(stderr, "Lexical/syntax error on line %d: %s\n", location->first_line, message);
}

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	if (argc < 3)
	{
		ERROR("Input and output file paths must be passed as parameters");
	}
	else
	{
		FILE *input_file = fopen(argv[1], "r");

		if (input_file == NULL)
		{
			ERROR("Could not open input file");
		}
		else
		{
			FILE *output_file = fopen(argv[2], "wb");

			if (output_file == NULL)
			{
				ERROR("Could not open output file");
			}
			else
			{
				yyscan_t flex_state;

				if (m68kasm_lex_init(&flex_state) != 0)
				{
					ERROR("m68kasm_lex_init failed");
				}
				else
				{
					StatementListNode *statement_list_head;

					m68kasm_set_in(input_file, flex_state);

					/*m68kasm_lex(); */

				#if M68KASM_DEBUG
					m68kasm_debug = 1;
				#endif

					statement_list_head = NULL;

					if (m68kasm_parse(flex_state, &statement_list_head) != 0)
						exit_code = EXIT_FAILURE;

					if (m68kasm_lex_destroy(flex_state) != 0)
						ERROR("m68kasm_lex_destroy failed");

					if (!ProcessParseTree(output_file, statement_list_head))
						exit_code = EXIT_FAILURE;
				}

				fclose(output_file);
			}

			fclose(input_file);
		}
	}

	return exit_code;
}
