/*
 * Copyright (C) 2022 Clownacy
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

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clowncommon/clowncommon.h"

#include "semantic.h"

#define ERROR(message) do { fputs("Error: " message "\n", stderr); exit_code = EXIT_FAILURE;} while (0)

static int total_arguments;
static char **arguments;

static void DefinitionCallback(void *internal, void* const user_data, const ClownAssembler_AddDefinition add_definition)
{
	int i;

	(void)user_data;

	for (i = 1; i < total_arguments; ++i)
	{
		if (arguments[i][0] == '-' && arguments[i][1] == 'e')
		{
			const char* const identifier = arguments[i + 1];
			const size_t identifier_length = strcspn(identifier, "=");

			unsigned long value;
			char *str_end;

			if (identifier[identifier_length] != '=')
			{
				value = 0; /* Exactly what asm68k does... */
			}
			else
			{
				value = strtoul(&identifier[identifier_length + 1], &str_end, 0);

				if (str_end < strchr(identifier, '\0'))
					fprintf(stderr, "Error: Value of argument '-e %s' is invalid.\n", arguments[i + 1]);
			}

			add_definition(internal, identifier, identifier_length, value);

			++i; /* Skip argument. */
		}
	}
}

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	cc_bool print_usage;
	const char *input_file_path;
	const char *output_file_path;
	const char *listing_file_path;
	const char *symbol_file_path;
	ClownAssembler_Settings settings = {0};
	int i;

	total_arguments = argc;
	arguments = argv;

	print_usage = cc_false;
	input_file_path = NULL;
	output_file_path = NULL;
	listing_file_path = NULL;
	symbol_file_path = NULL;
	settings.case_insensitive = cc_false;
	settings.debug = cc_false;
	settings.equ_set_descope_local_labels = cc_false;
	settings.output_local_labels_to_sym_file = cc_false;
	settings.warnings_enabled = cc_true;
	settings.expand_all_macros = cc_false;

	for (i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'h':
					print_usage = cc_true;
					continue;

				case 'i':
					if (i < argc && argv[i + 1][0] != '-')
					{
						++i;
						input_file_path = argv[i];
					}

					continue;

				case 'o':
					if (i < argc && argv[i + 1][0] != '-')
					{
						++i;
						output_file_path = argv[i];
					}

					continue;

				case 'l':
					if (i < argc && argv[i + 1][0] != '-')
					{
						++i;
						listing_file_path = argv[i];
					}

					continue;

				case 's':
					if (i < argc && argv[i + 1][0] != '-')
					{
						++i;
						symbol_file_path = argv[i];
					}

					continue;

				case 'c':
					settings.case_insensitive = cc_true;
					continue;

				case 'b':
					settings.debug = cc_true;
					continue;

				case 'd':
					settings.equ_set_descope_local_labels = cc_true;
					continue;

				case 'e':
					/* We'll deal with this later, in DefinitionCallback. */
					if (i < argc && argv[i + 1][0] != '-')
						++i;

					continue;

				case 'm':
					settings.expand_all_macros = cc_true;
					continue;

				case 'v':
					settings.output_local_labels_to_sym_file = cc_true;
					continue;

				case 'w':
					settings.warnings_enabled = cc_false;
					continue;
			}
		}

		fprintf(stderr, "Error: Unrecognised option '%s'.\n", argv[i]);
		exit_code = EXIT_FAILURE;
	}

	if (argc < 2 || print_usage)
	{
		fputs(
			"clownassembler - An assembler for the Motorola 68000.\n"
			"\n"
			"Options:\n"
			" -i [path] - Input file. If not specified, STDIN is used instead.\n"
			" -o [path] - Output file.\n"
			" -l [path] - Listing file. Optional.\n"
			" -s [path] - asm68k-style symbol file. Optional.\n"
			" -c        - Enable case-insensitive mode.\n"
			" -b        - Enable Bison's debug output.\n"
			" -d        - Allow EQU/SET to descope local labels.\n"
			" -e X=Y    - Defines symbol X to value Y.\n"
			" -v        - Include local labels in symbol file.\n"
			" -w        - Silence warnings.\n"
			, stdout);
	}
	else
	{
		if (output_file_path == NULL)
		{
			ERROR("Output file path must be specified with '-o'.");
		}
		else
		{
			FILE *input_file;

			if (input_file_path == NULL)
				input_file = stdin;
			else
				input_file = fopen(input_file_path, "r");

			if (input_file == NULL)
			{
				ERROR("Could not open input file.");
			}
			else
			{
				FILE *output_file;

				output_file = fopen(output_file_path, "wb");

				if (output_file == NULL)
				{
					ERROR("Could not open output file.");
				}
				else
				{
					FILE *listing_file;
					FILE *symbol_file;
					cc_bool success;

					if (listing_file_path == NULL)
					{
						listing_file = NULL;
					}
					else
					{
						listing_file = fopen(listing_file_path, "w");

						if (listing_file == NULL)
							ERROR("Could not open listing file.");
					}

					if (symbol_file_path == NULL)
					{
						symbol_file = NULL;
					}
					else
					{
						symbol_file = fopen(symbol_file_path, "wb");

						if (symbol_file == NULL)
							ERROR("Could not open symbol file.");
					}

					success = ClownAssembler_AssembleFile(input_file, output_file, stderr, listing_file, symbol_file, input_file_path != NULL ? input_file_path : "STDIN", &settings, DefinitionCallback, NULL);

					if (listing_file != NULL)
						fclose(listing_file);

					fclose(output_file);

					if (!success)
					{
						ERROR("Could not assemble.");
						/* Delete the output file; it will be junk anyway.
						   Also, leaving a junk file will confuse Make, which will think that the assembler had previously succeeded. */
						remove(output_file_path);
					}
				}

				fclose(input_file);
			}
		}
	}

	return exit_code;
}
