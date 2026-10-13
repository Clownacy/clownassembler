/*
 * Copyright (C) 2022-2025 Clownacy
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
#include <fstream>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "case-insensitive.h"
#include "semantic.h"

static constexpr CaseInsensitive::ToLower to_lower(true);

static int total_arguments;
static char **arguments;

static void DefinitionCallback(void *internal, void* const user_data, const ClownAssembler::AddDefinition add_definition)
{
	(void)user_data;

	for (int i = 1; i < total_arguments; ++i)
	{
		if (arguments[i][0] == '/' && to_lower(arguments[i][1]) == 'e')
		{
			const char* const identifier = arguments[i + 1];
			const std::size_t identifier_length = strcspn(identifier, "=");

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

			add_definition(internal, {identifier, identifier_length}, value);

			++i; /* Skip argument. */
		}
	}
}

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	char *combined_arguments;
	ClownAssembler::Settings settings = {};
	char *source_file_path;
	char *object_file_path;
	char *symbol_file_path;
	char *listing_file_path;
	std::ifstream source_file;
	std::ofstream object_file;
	std::ofstream symbol_file;
	std::ofstream listing_file;
	enum
	{
		OUTPUT_EXECUTABLE,
		OUTPUT_FLAT_BINARY,
		OUTPUT_S_RECORD
	} output_mode;

	total_arguments = argc;
	arguments = argv;

	combined_arguments = nullptr;

	settings.local_signifier = '@';
	settings.debug = false;
	settings.case_insensitive = true;
	settings.equ_set_descope_local_labels = false;
	settings.output_local_labels_to_sym_file = false;
	settings.warnings_enabled = true;
	settings.pedantic_warnings_enabled = false;
	settings.expand_all_macros = false;
	settings.automatic_even = true;
	settings.optimisation.zero_displacement = false;

	source_file_path = object_file_path = symbol_file_path = listing_file_path = nullptr;

	output_mode = OUTPUT_EXECUTABLE;

	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '/')
		{
			/* This is a switch. */
			switch (to_lower(argv[i][1]))
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
					settings.expand_all_macros = true;
					break;

				case 'p': /* And 'ps'. */
					if (to_lower(argv[i][2]) == 's')
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

						for (option = strtok(argv[i], ","); option != nullptr; option = strtok(nullptr, ","))
						{
							if (CaseInsensitive::CompareStrings(option, "ae+"))
							{
								settings.automatic_even = true;
							}
							else if (CaseInsensitive::CompareStrings(option, "ae-"))
							{
								settings.automatic_even = false;
							}
							else if (CaseInsensitive::CompareStrings(option, "an+"))
							{
								/* Quietly accept this even though it isn't configurable. */
							}
							else if (CaseInsensitive::CompareStrings(option, "an-"))
							{
								fprintf(stderr, "Warning: Option '%s' has no effect, as alternate numeric specifiers are always enabled.\n", option);
							}
							else if (CaseInsensitive::CompareStrings(option, "c+"))
							{
								settings.case_insensitive = false;
							}
							else if (CaseInsensitive::CompareStrings(option, "c-"))
							{
								settings.case_insensitive = true;
							}
							else if (CaseInsensitive::CompareStrings(option, "d+"))
							{
								settings.equ_set_descope_local_labels = true;
							}
							else if (CaseInsensitive::CompareStrings(option, "d-"))
							{
								settings.equ_set_descope_local_labels = false;
							}
							else if (CaseInsensitive::CompareStrings(option, "e+"))
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", option);
								exit_code = EXIT_FAILURE;
							}
							else if (CaseInsensitive::CompareStrings(option, "e-"))
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", option);
								exit_code = EXIT_FAILURE;
							}
							else if (to_lower(option[0]) == 'l')
							{
								const char signifier = to_lower(option[1]);

								switch (signifier)
								{
									case '+':
										settings.local_signifier = '.';
										break;

									case '-':
										settings.local_signifier = '@';
										break;

									default:
										settings.local_signifier = signifier;
										break;
								}
							}
							else if (CaseInsensitive::CompareStrings(option, "s+"))
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", option);
								exit_code = EXIT_FAILURE;
							}
							else if (CaseInsensitive::CompareStrings(option, "s-"))
							{
								/* This is always disabled. */
							}
							else if (CaseInsensitive::CompareStrings(option, "v+"))
							{
								settings.output_local_labels_to_sym_file = true;
							}
							else if (CaseInsensitive::CompareStrings(option, "v-"))
							{
								settings.output_local_labels_to_sym_file = false;
							}
							else if (CaseInsensitive::CompareStrings(option, "w+"))
							{
								settings.warnings_enabled = true;
							}
							else if (CaseInsensitive::CompareStrings(option, "w-"))
							{
								settings.warnings_enabled = false;
							}
							else if (CaseInsensitive::CompareStrings(option, "wp+"))
							{
								settings.pedantic_warnings_enabled = true;
							}
							else if (CaseInsensitive::CompareStrings(option, "wp-"))
							{
								settings.pedantic_warnings_enabled = false;
							}
							else if (CaseInsensitive::CompareStrings(option, "ws+"))
							{
								/* This is always enabled. */
							}
							else if (CaseInsensitive::CompareStrings(option, "ws-"))
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", option);
								exit_code = EXIT_FAILURE;
							}
							else if (CaseInsensitive::CompareStrings(option, "x+"))
							{
								fprintf(stderr, "Error: Unsupported option: '%s'.\n", option);
								exit_code = EXIT_FAILURE;
							}
							else if (CaseInsensitive::CompareStrings(option, "x-"))
							{
								/* This is always disabled. */
							}
							else if (CaseInsensitive::CompareStrings(option, "op+"))
							{
								fprintf(stderr, "Warning: Unsupported option: '%s'.\n", option);
							}
							else if (CaseInsensitive::CompareStrings(option, "op-"))
							{
								/* This is always disabled. */
							}
							else if (CaseInsensitive::CompareStrings(option, "os+"))
							{
								fprintf(stderr, "Warning: Unsupported option: '%s'.\n", option);
							}
							else if (CaseInsensitive::CompareStrings(option, "os-"))
							{
								/* This is always disabled. */
							}
							else if (CaseInsensitive::CompareStrings(option, "ow+"))
							{
								fprintf(stderr, "Warning: Unsupported option: '%s'.\n", option);
							}
							else if (CaseInsensitive::CompareStrings(option, "ow-"))
							{
								/* This is always disabled. */
							}
							else if (CaseInsensitive::CompareStrings(option, "oz+"))
							{
								settings.optimisation.zero_displacement = true;
							}
							else if (CaseInsensitive::CompareStrings(option, "oz-"))
							{
								settings.optimisation.zero_displacement = false;
							}
							else if (CaseInsensitive::CompareStrings(option, "oaq+"))
							{
								fprintf(stderr, "Warning: Unsupported option: '%s'.\n", option);
							}
							else if (CaseInsensitive::CompareStrings(option, "oaq-"))
							{
								/* This is always disabled. */
							}
							else if (CaseInsensitive::CompareStrings(option, "osq+"))
							{
								fprintf(stderr, "Warning: Unsupported option: '%s'.\n", option);
							}
							else if (CaseInsensitive::CompareStrings(option, "osq-"))
							{
								/* This is always disabled. */
							}
							else if (CaseInsensitive::CompareStrings(option, "omq+"))
							{
								fprintf(stderr, "Warning: Unsupported option: '%s'.\n", option);
							}
							else if (CaseInsensitive::CompareStrings(option, "omq-"))
							{
								/* This is always disabled. */
							}
							else
							{
								fprintf(stderr, "Warning: Unrecognised option: '%s'.\n", option);
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
			std::size_t combined_arguments_size;
			int j;

			combined_arguments_size = 1; /* The '1' is for the null character. */

			/* We've encountered the file list. */
			for (j = i; j < argc; ++j)
				combined_arguments_size += strlen(argv[j]);

			combined_arguments = new char[combined_arguments_size];

			char *comma;

			combined_arguments[0] = '\0';

			for (j = i; j < argc; ++j)
				strcat(combined_arguments, argv[j]);

			source_file_path = combined_arguments;
			comma = strchr(source_file_path, ',');

			if (comma != nullptr)
			{
				comma[0] = '\0';

				object_file_path = comma + 1;
				comma = strchr(object_file_path, ',');

				if (comma != nullptr)
				{
					comma[0] = '\0';

					symbol_file_path = comma + 1;
					comma = strchr(symbol_file_path, ',');

					if (comma != nullptr)
					{
						comma[0] = '\0';

						listing_file_path = comma + 1;
						comma = strchr(listing_file_path, ',');

						if (comma != nullptr)
							comma[0] = '\0';
					}
				}
			}

			break;
		}
	}

	/* Open files. */
	if (source_file_path == nullptr || source_file_path[0] == '\0')
	{
		fprintf(stderr, "Error: Source file path must be specified.\n");
		exit_code = EXIT_FAILURE;
	}
	else
	{
		source_file.open(source_file_path);
		if (!source_file.is_open())
		{
			fprintf(stderr, "Error: Source file could not be opened.\n");
			exit_code = EXIT_FAILURE;
		}
	}

	if (object_file_path == nullptr || object_file_path[0] == '\0')
	{
		fprintf(stderr, "Error: Object file path must be specified.\n");
		exit_code = EXIT_FAILURE;
	}
	else
	{
		object_file.open(object_file_path, object_file.binary);
		if (!object_file.is_open())
		{
			fprintf(stderr, "Error: Object file could not be opened.\n");
			exit_code = EXIT_FAILURE;
		}
	}

	std::ostream *symbol_stream = nullptr;
	if (symbol_file_path != nullptr && symbol_file_path[0] != '\0')
	{
		symbol_stream = &symbol_file;
		symbol_file.open(symbol_file_path, symbol_file.binary);
	}

	std::ostream *listing_stream = nullptr;
	if (listing_file_path != nullptr && listing_file_path[0] != '\0')
	{
		listing_stream = &listing_file;
		listing_file.open(listing_file_path);
	}

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
		if (!ClownAssembler::Assemble(source_file, object_file, std::cout, listing_stream, symbol_stream, source_file_path, settings, DefinitionCallback, nullptr))
		{
			fprintf(stderr, "Error: Failed to assemble.\n");
			exit_code = EXIT_FAILURE;
		}
	}

	delete[] combined_arguments;

	if (source_file.is_open())
		source_file.close();

	if (object_file.is_open())
	{
		object_file.close();

		if (exit_code == EXIT_FAILURE)
		{
			/* Delete the output file; it will be junk anyway.
			   Also, leaving a junk file will confuse Make, which will think that the assembler had previously succeeded. */
			remove(object_file_path);
		}
	}

	if (listing_file.is_open())
		listing_file.close();

	if (symbol_file.is_open())
		symbol_file.close();

	return exit_code;
}
