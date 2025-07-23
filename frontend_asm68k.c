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

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "semantic.h"
#include "strcmpci.h"

static int total_arguments;
static char **arguments;

static void DefinitionCallback(void *internal, void* const user_data, const ClownAssembler_AddDefinition add_definition)
{
	int i;

	(void)user_data;

	for (i = 1; i < total_arguments; ++i)
	{
		if (arguments[i][0] == '/' && tolower(arguments[i][1]) == 'e')
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
				/* SN 68k (asm68k) supports XX, 0xXX, and $XX. */
				if (identifier[identifier_length + 1] == '$')
					value = strtoul(&identifier[identifier_length + 1 + 1], &str_end, 0x10);
				else
					value = strtoul(&identifier[identifier_length + 1], &str_end, 0);

				if (str_end < strchr(identifier, '\0'))
					fprintf(stderr, "Error: Value of argument '/e %s' is invalid.\n", arguments[i + 1]);
			}

			add_definition(internal, identifier, identifier_length, value);

			++i; /* Skip argument. */
		}
	}
}

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	int i;
	char *combined_arguments;
	ClownAssembler_Settings settings = {0};
	const char *source_file_path;
	const char *object_file_path;
	const char *symbol_file_path;
	const char *listing_file_path;
	FILE *source_file;
	FILE *object_file;
	FILE *symbol_file;
	FILE *listing_file;
	enum
	{
		OUTPUT_EXECUTABLE,
		OUTPUT_FLAT_BINARY,
		OUTPUT_S_RECORD
	} output_mode;

	total_arguments = argc;
	arguments = argv;

	combined_arguments = NULL;

	settings.debug = cc_false;
	settings.case_insensitive = cc_true;
	settings.equ_set_descope_local_labels = cc_false;
	settings.output_local_labels_to_sym_file = cc_false;
	settings.warnings_enabled = cc_true;
	settings.pedantic_warnings_enabled = cc_false;
	settings.expand_all_macros = cc_false;

	source_file_path = object_file_path = symbol_file_path = listing_file_path = NULL;
	source_file = object_file = symbol_file = listing_file = NULL;

	output_mode = OUTPUT_EXECUTABLE;

	for (i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '/')
		{
			/* This is a switch. */
			switch (tolower(argv[i][1]))
			{
				case 'e':
					/* We'll deal with this later, in DefinitionCallback. */
					++i;
					break;

				case 'j':
					/* Skip argument. */
					++i;
					/* Fallthrough */
				case 'c':
				case 'd':
				case 'g':
				case 'k':
				case 'l':
				case 'w':
				case 'z': /* And 'zd'. */
					fprintf(stderr, "Warning: Unsupported option: '%s'.\n", argv[i]);
					break;

				case 'm':
					settings.expand_all_macros = cc_true;
					break;

				case 'p': /* And 'ps'. */
					if (tolower(argv[i][2]) == 's')
						output_mode = OUTPUT_S_RECORD;
					else
						output_mode = OUTPUT_FLAT_BINARY;

					break;

				case 'o':
					/* Parse the argument after this one. */
					++i;

					if (i < argc)
					{
						const char *option;

						for (option = strtok(argv[i], ","); option != NULL; option = strtok(NULL, ","))
						{
							if (strcmpci(option, "ae+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "ae-") == 0)
							{
								/* TODO - AE should be enabled by default. */
								/* Already disabled by default but okay. */
							}
							else if (strcmpci(option, "an+") == 0)
							{
								/* Quietly accept this even though it isn't configurable. */
							}
							else if (strcmpci(option, "an-") == 0)
							{
								fprintf(stderr, "Warning: Option '%s' has no effect, as alternate numeric specifiers are always enabled.\n", argv[i]);
							}
							else if (strcmpci(option, "c+") == 0)
							{
								settings.case_insensitive = cc_false;
							}
							else if (strcmpci(option, "c-") == 0)
							{
								settings.case_insensitive = cc_true;
							}
							else if (strcmpci(option, "d+") == 0)
							{
								settings.equ_set_descope_local_labels = cc_true;
							}
							else if (strcmpci(option, "d-") == 0)
							{
								settings.equ_set_descope_local_labels = cc_false;
							}
							else if (strcmpci(option, "e+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "e-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (tolower(argv[i][0]) == 'l')
							{
								switch (tolower(argv[i][1]))
								{
									case '+':
									case '-':
										fprintf(stderr, "Warning: Option '%s' has no effect, as '@' and '.' always specify local labels.\n", argv[i]);
										break;

									default:
										fprintf(stderr, "Error: Option '%s' is unsupported: local labels can only be specified with '@' or '.'.\n", argv[i]);
										exit_code = EXIT_FAILURE;
										break;
								}
							}
							else if (strcmpci(option, "s+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "s-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "v+") == 0)
							{
								settings.output_local_labels_to_sym_file = cc_true;
							}
							else if (strcmpci(option, "v-") == 0)
							{
								settings.output_local_labels_to_sym_file = cc_false;
							}
							else if (strcmpci(option, "w+") == 0)
							{
								settings.warnings_enabled = cc_true;
							}
							else if (strcmpci(option, "w-") == 0)
							{
								settings.warnings_enabled = cc_false;
							}
							else if (strcmpci(option, "wp+") == 0)
							{
								settings.pedantic_warnings_enabled = cc_true;
							}
							else if (strcmpci(option, "wp-") == 0)
							{
								settings.pedantic_warnings_enabled = cc_false;
							}
							else if (strcmpci(option, "ws+") == 0)
							{
								/* This is always enabled. */
							}
							else if (strcmpci(option, "ws-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "x+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "x-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "op+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "op-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "os+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "os-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "ow+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "ow-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "oz+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "oz-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "oaq+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "oaq-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "osq+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "osq-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "omq+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(option, "omq-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else
							{
								fprintf(stderr, "Error: Unrecognised option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
						}
					}

					break;

				default:
					fprintf(stderr, "Error: Unrecognised switch: '%s'.\n", argv[i]);
					exit_code = EXIT_FAILURE;
					break;
			}
		}
		else
		{
			size_t combined_arguments_size;
			int j;

			combined_arguments_size = 1; /* The '1' is for the null character. */

			/* We've encountered the file list. */
			for (j = i; j < argc; ++j)
				combined_arguments_size += strlen(argv[j]);

			combined_arguments = (char*)malloc(combined_arguments_size);

			if (combined_arguments == NULL)
			{
				fprintf(stderr, "Error: Could not allocate memory for combined argument string.\n");
				exit_code = EXIT_FAILURE;
			}
			else
			{
				char *comma;

				combined_arguments[0] = '\0';

				for (j = i; j < argc; ++j)
					strcat(combined_arguments, argv[j]);

				source_file_path = combined_arguments;
				comma = strchr(source_file_path, ',');

				if (comma != NULL)
				{
					comma[0] = '\0';

					object_file_path = comma + 1;
					comma = strchr(object_file_path, ',');

					if (comma != NULL)
					{
						comma[0] = '\0';

						symbol_file_path = comma + 1;
						comma = strchr(symbol_file_path, ',');

						if (comma != NULL)
						{
							comma[0] = '\0';

							listing_file_path = comma + 1;
							comma = strchr(listing_file_path, ',');

							if (comma != NULL)
								comma[0] = '\0';
						}
					}
				}
			}

			break;
		}
	}

	/* Open files. */
	if (source_file_path == NULL || source_file_path[0] == '\0')
	{
		fprintf(stderr, "Error: Source file path must be specified.\n");
		exit_code = EXIT_FAILURE;
	}
	else if ((source_file = fopen(source_file_path, "r")) == NULL)
	{
		fprintf(stderr, "Error: Source file could not be opened.\n");
		exit_code = EXIT_FAILURE;
	}

	if (object_file_path == NULL || object_file_path[0] == '\0')
	{
		fprintf(stderr, "Error: Object file path must be specified.\n");
		exit_code = EXIT_FAILURE;
	}
	else if ((object_file = fopen(object_file_path, "wb")) == NULL)
	{
		fprintf(stderr, "Error: Object file could not be opened.\n");
		exit_code = EXIT_FAILURE;
	}

	if (symbol_file_path != NULL && symbol_file_path[0] != '\0')
		symbol_file = fopen(symbol_file_path, "wb");

	if (listing_file_path != NULL && listing_file_path[0] != '\0')
		listing_file = fopen(listing_file_path, "w");

	/* Make sure that we're using a supported output format. */
	switch (output_mode)
	{
		case OUTPUT_EXECUTABLE:
			fprintf(stderr, "Error: Executable output format is not supported: use '/p' for outputting a flat binary instead.\n");
			exit_code = EXIT_FAILURE;
			break;

		case OUTPUT_FLAT_BINARY:
			break;

		case OUTPUT_S_RECORD:
			fprintf(stderr, "Error: S-record output format is not supported: use '/p' for outputting a flat binary instead.\n");
			exit_code = EXIT_FAILURE;
			break;
	}

	/* If we've gotten this far without any errors, then finally run the assembler. */
	if (exit_code != EXIT_FAILURE)
	{
		if (!ClownAssembler_AssembleFile(source_file, object_file, stdout, listing_file, symbol_file, source_file_path, &settings, DefinitionCallback, NULL))
		{
			fprintf(stderr, "Error: Failed to assemble.\n");
			exit_code = EXIT_FAILURE;
		}
	}

	free(combined_arguments);

	if (source_file != NULL)
		fclose(source_file);

	if (object_file != NULL)
	{
		fclose(object_file);

		if (exit_code == EXIT_FAILURE)
		{
			/* Delete the output file; it will be junk anyway.
			   Also, leaving a junk file will confuse Make, which will think that the assembler had previously succeeded. */
			remove(object_file_path);
		}
	}

	if (listing_file != NULL)
		fclose(listing_file);

	if (symbol_file != NULL)
		fclose(symbol_file);

	return exit_code;
}
