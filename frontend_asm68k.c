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

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	int i;
	char *combined_arguments;
	cc_bool case_insensitive;
	cc_bool equ_set_descope_local_labels;
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

	combined_arguments = NULL;

	case_insensitive = cc_true;
	equ_set_descope_local_labels = cc_false;

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
				case 'j':
					/* Skip argument. */
					++i;
					/* Fallthrough */
				case 'c':
				case 'd':
				case 'g':
				case 'k':
				case 'l':
				case 'm':
				case 'w':
				case 'z': /* And 'zd'. */
					fprintf(stderr, "Warning: Unsupported option: '%s'.\n", argv[i]);
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
							if (strcmpci(argv[i], "ae+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "ae-") == 0)
							{
								/* TODO - AE should be enabled by default. */
								/* Already disabled by default but okay. */
							}
							else if (strcmpci(argv[i], "an+") == 0)
							{
								/* Quietly accept this even though it isn't configurable. */
							}
							else if (strcmpci(argv[i], "an-") == 0)
							{
								fprintf(stderr, "Warning: Option '%s' has no effect, as alternate numeric specifiers are always enabled.\n", argv[i]);
							}
							else if (strcmpci(argv[i], "c+") == 0)
							{
								case_insensitive = cc_false;
							}
							else if (strcmpci(argv[i], "c-") == 0)
							{
								case_insensitive = cc_true;
							}
							else if (strcmpci(argv[i], "d+") == 0)
							{
								equ_set_descope_local_labels = cc_true;
							}
							else if (strcmpci(argv[i], "d-") == 0)
							{
								equ_set_descope_local_labels = cc_false;
							}
							else if (strcmpci(argv[i], "e+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "e-") == 0)
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
							else if (strcmpci(argv[i], "s+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "s-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "v+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "v-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "w+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "w-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "ws+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "ws-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "x+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "x-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "op+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "op-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "os+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "os-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "ow+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "ow-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "oz+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "oz-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "oaq+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "oaq-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "osq+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "osq-") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "omq+") == 0)
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", argv[i]);
								exit_code = EXIT_FAILURE;
							}
							else if (strcmpci(argv[i], "omq-") == 0)
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
		if (!ClownAssembler_Assemble(source_file, object_file, listing_file, symbol_file, source_file_path, cc_false, case_insensitive, equ_set_descope_local_labels))
		{
			fprintf(stderr, "Error: Failed to assemble.\n");
			exit_code = EXIT_FAILURE;
		}
	}

	free(combined_arguments);

	return exit_code;
}
