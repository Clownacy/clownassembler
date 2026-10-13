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

#include <format>
#include <fstream>
#include <stdlib.h>
#include <string.h>

#include "semantic.h"

static int total_arguments;
static char **arguments;

template<typename... Args>
[[noreturn]] static void ErrorAndExit(std::format_string<Args...> &&format_string, Args &&...args)
{
	std::cerr << "Error: " << std::format(std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...) << '\n';
	std::exit(EXIT_FAILURE);
}

static void DefinitionCallback(void *internal, void* const user_data, const ClownAssembler::AddDefinition add_definition)
{
	(void)user_data;

	for (int i = 1; i < total_arguments; ++i)
	{
		if (arguments[i][0] == '-' && arguments[i][1] == 'e')
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
				value = strtoul(&identifier[identifier_length + 1], &str_end, 0);

				if (str_end < strchr(identifier, '\0'))
					std::cerr << std::format("Error: Value of argument '-e {}' is invalid.\n", arguments[i + 1]);
			}

			add_definition(internal, identifier, identifier_length, value);

			++i; /* Skip argument. */
		}
	}
}

int main(int argc, char **argv)
{
	bool print_usage;
	const char *input_file_path;
	const char *output_file_path;
	const char *listing_file_path;
	const char *symbol_file_path;
	ClownAssembler::Settings settings = {};

	total_arguments = argc;
	arguments = argv;

	print_usage = false;
	input_file_path = nullptr;
	output_file_path = nullptr;
	listing_file_path = nullptr;
	symbol_file_path = nullptr;
	settings.local_signifier = '@';
	settings.case_insensitive = false;
	settings.debug = false;
	settings.equ_set_descope_local_labels = false;
	settings.output_local_labels_to_sym_file = false;
	settings.warnings_enabled = true;
	settings.pedantic_warnings_enabled = true;
	settings.expand_all_macros = false;
	settings.automatic_even = false;
	settings.optimisation.zero_displacement = false;

	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'h':
					print_usage = true;
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

				case 'a':
					settings.automatic_even = true;
					continue;

				case 'c':
					settings.case_insensitive = true;
					continue;

				case 'b':
					settings.debug = true;
					continue;

				case 'd':
					settings.equ_set_descope_local_labels = true;
					continue;

				case 'e':
					/* We'll deal with this later, in DefinitionCallback. */
					if (i < argc && argv[i + 1][0] != '-')
						++i;

					continue;

				case 'm':
					settings.expand_all_macros = true;
					continue;

				case 'p':
					settings.pedantic_warnings_enabled = false;
					continue;

				case 'v':
					settings.output_local_labels_to_sym_file = true;
					continue;

				case 'w':
					settings.warnings_enabled = false;
					continue;

				case '.':
					settings.local_signifier = argv[i][2];
					continue;
			}
		}

		ErrorAndExit("Unrecognised option '{}'.", argv[i]);
	}

	if (argc < 2 || print_usage)
	{
		std::cout <<
			CLOWNASSEMBLER_VERSION_STRING " - An assembler for the Motorola 68000.\n"
			"\n"
			"Options:\n"
			" -i [path] - Input file. If not specified, STDIN is used instead.\n"
			" -o [path] - Output file.\n"
			" -l [path] - Listing file. Optional.\n"
			" -s [path] - asm68k-style symbol file. Optional.\n"
			" -a        - Enable automatic-even mode.\n"
			" -c        - Enable case-insensitive mode.\n"
			" -b        - Enable Bison's debug output.\n"
			" -d        - Allow EQU/SET to descope local labels.\n"
			" -e X=Y    - Defines symbol X to value Y.\n"
			" -p        - Silence pedantic warnings.\n"
			" -v        - Include local labels in symbol file.\n"
			" -w        - Silence warnings.\n"
			" -.X       - Set local label signifier to character after dot (default '@').\n"
			;

		return EXIT_SUCCESS;
	}

	if (output_file_path == nullptr)
		ErrorAndExit("Output file path must be specified with '-o'.");

	// Open input file.

	std::ifstream input_file_stream;

	if (input_file_path != nullptr)
	{
		input_file_stream.open(input_file_path);

		if (!input_file_stream.is_open())
			ErrorAndExit("Could not open input file.");
	}

	std::istream &input_file = input_file_path == nullptr ? std::cin : input_file_stream;

	// Open output file.

	std::ofstream output_file(output_file_path, output_file.binary);

	if (!output_file.is_open())
		ErrorAndExit("Could not open output file.");

	// Open listing file.

	std::ofstream *listing_stream = nullptr;
	std::ofstream listing_file;

	if (listing_file_path != nullptr)
	{
		listing_stream = &listing_file;
		listing_file.open(listing_file_path);

		if (!listing_file.is_open())
			ErrorAndExit("Could not open listing file.");
	}

	// Open symbol file.

	std::ostream *symbol_stream = nullptr;
	std::ofstream symbol_file;

	if (symbol_file_path != nullptr)
	{
		symbol_stream = &symbol_file;
		symbol_file.open(symbol_file_path, symbol_file.binary);

		if (!symbol_file.is_open())
			ErrorAndExit("Could not open symbol file.");
	}

	if (!ClownAssembler::Assemble(input_file, output_file, std::cerr, listing_stream, symbol_stream, input_file_path != nullptr ? input_file_path : "STDIN", settings, DefinitionCallback, nullptr))
	{
		/* Delete the output file; it will be junk anyway.
		   Also, leaving a junk file will confuse Make, which will think that the assembler had previously succeeded. */
		remove(output_file_path);
		ErrorAndExit("Could not assemble.");
	}

	return EXIT_SUCCESS;
}
