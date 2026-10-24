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

#include <cstring>
#include <fstream>

#include "../semantic.h"
#include "../utilities/case-insensitive.h"
#include "../utilities/path.h"
#include "../utilities/string.h"
#include "common.h"

static constexpr Utilities::CaseInsensitive::ToLower to_lower(true);

static int total_arguments;
static char **arguments;

static void DefinitionCallback(const ClownAssembler::AddDefinition &add_definition)
{
	for (int i = 1; i < total_arguments; ++i)
	{
		const std::string_view command_argument = arguments[i];

		if (!Utilities::CaseInsensitive::CompareStrings(command_argument, "/e"))
			continue;

		const std::string_view value_argument = arguments[i + 1];
		const auto &definition = DecodeDefinition(value_argument);

		if (!definition.has_value())
			Error("Value of argument '/e {}' is invalid.", value_argument);
		else
			add_definition(definition->first, definition->second);

		++i; /* Skip argument. */
	}
}

int main(int argc, char **argv)
{
	ClownAssembler::Settings settings = {};
	std::string_view source_file_path_string;
	std::filesystem::path source_file_path;
	std::filesystem::path object_file_path;
	std::filesystem::path symbol_file_path;
	std::filesystem::path listing_file_path;
	enum class OutputMode
	{
		Executable,
		FlatBinary,
		SRecord,
	} output_mode;

	total_arguments = argc;
	arguments = argv;

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

	output_mode = OutputMode::Executable;

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
					Warning("Unsupported option: '{}'.", argv[i]);
					break;

				case 'm':
					settings.expand_all_macros = true;
					break;

				case 'p': /* And 'ps'. */
					if (to_lower(argv[i][2]) == 's')
						output_mode = OutputMode::SRecord;
					else
						output_mode = OutputMode::FlatBinary;

					break;

				case 'o':
					/* Parse the argument after this one. */
					++i;

					if (i < argc)
					{
						for (const char *option = std::strtok(argv[i], ","); option != nullptr; option = strtok(nullptr, ","))
						{
							if (Utilities::CaseInsensitive::CompareStrings(option, "ae+"))
							{
								settings.automatic_even = true;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "ae-"))
							{
								settings.automatic_even = false;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "an+"))
							{
								/* Quietly accept this even though it isn't configurable. */
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "an-"))
							{
								Warning("Option '{}' has no effect, as alternate numeric specifiers are always enabled.", option);
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "c+"))
							{
								settings.case_insensitive = false;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "c-"))
							{
								settings.case_insensitive = true;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "d+"))
							{
								settings.equ_set_descope_local_labels = true;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "d-"))
							{
								settings.equ_set_descope_local_labels = false;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "e+"))
							{
								ErrorAndExit("Unsupported option: '{}'.", option);
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "e-"))
							{
								ErrorAndExit("Unsupported option: '{}'.", option);
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
							else if (Utilities::CaseInsensitive::CompareStrings(option, "s+"))
							{
								ErrorAndExit("Unsupported option: '{}'.", option);
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "s-"))
							{
								/* This is always disabled. */
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "v+"))
							{
								settings.output_local_labels_to_sym_file = true;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "v-"))
							{
								settings.output_local_labels_to_sym_file = false;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "w+"))
							{
								settings.warnings_enabled = true;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "w-"))
							{
								settings.warnings_enabled = false;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "wp+"))
							{
								settings.pedantic_warnings_enabled = true;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "wp-"))
							{
								settings.pedantic_warnings_enabled = false;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "ws+"))
							{
								/* This is always enabled. */
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "ws-"))
							{
								ErrorAndExit("Unsupported option: '{}'.", option);
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "x+"))
							{
								ErrorAndExit("Unsupported option: '{}'.", option);
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "x-"))
							{
								/* This is always disabled. */
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "op+"))
							{
								Warning("Unsupported option: '{}'.", option);
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "op-"))
							{
								/* This is always disabled. */
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "os+"))
							{
								Warning("Unsupported option: '{}'.", option);
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "os-"))
							{
								/* This is always disabled. */
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "ow+"))
							{
								Warning("Unsupported option: '{}'.", option);
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "ow-"))
							{
								/* This is always disabled. */
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "oz+"))
							{
								settings.optimisation.zero_displacement = true;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "oz-"))
							{
								settings.optimisation.zero_displacement = false;
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "oaq+"))
							{
								Warning("Unsupported option: '{}'.", option);
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "oaq-"))
							{
								/* This is always disabled. */
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "osq+"))
							{
								Warning("Unsupported option: '{}'.", option);
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "osq-"))
							{
								/* This is always disabled. */
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "omq+"))
							{
								Warning("Unsupported option: '{}'.", option);
							}
							else if (Utilities::CaseInsensitive::CompareStrings(option, "omq-"))
							{
								/* This is always disabled. */
							}
							else
							{
								Warning("Unrecognised option: '{}'.", option);
							}
						}
					}

					break;

				default:
					ErrorAndExit("Unrecognised switch: '{}'.", argv[i]);
			}
		}
		else
		{
			std::string combined_arguments;

			/* We've encountered the file list. */
			for (int j = i; j < argc; ++j)
				combined_arguments += argv[j];

			std::string_view combined_arguments_subview = combined_arguments;

			const auto &DoPath = [&combined_arguments_subview](std::string_view* const path_string = nullptr)
			{
				const auto length = combined_arguments_subview.find(',');
				const auto path = combined_arguments_subview.substr(0, length);

				combined_arguments_subview.remove_prefix(std::min(combined_arguments_subview.length(), path.length() + 1));

				if (path_string != nullptr)
					*path_string = path;

				return Utilities::Path::ToStandardPath(path);
			};

			source_file_path = DoPath(&source_file_path_string);
			object_file_path = DoPath();
			symbol_file_path = DoPath();
			listing_file_path = DoPath();

			break;
		}
	}

	/* Open files. */
	if (source_file_path.empty())
		ErrorAndExit("Source file path must be specified.");

	std::ifstream source_file(source_file_path);
	if (!source_file.is_open())
		ErrorAndExit("Source file could not be opened.");

	if (object_file_path.empty())
		ErrorAndExit("Object file path must be specified.");

	std::ofstream object_file(object_file_path, object_file.binary);
	if (!object_file.is_open())
		ErrorAndExit("Object file could not be opened.");

	std::ofstream symbol_file;
	std::ostream *symbol_stream = nullptr;
	if (!symbol_file_path.empty())
	{
		symbol_stream = &symbol_file;
		symbol_file.open(symbol_file_path, symbol_file.binary);
	}

	std::ofstream listing_file;
	std::ostream *listing_stream = nullptr;
	if (!listing_file_path.empty())
	{
		listing_stream = &listing_file;
		listing_file.open(listing_file_path);
	}

	/* Make sure that we're using a supported output format. */
	switch (output_mode)
	{
		case OutputMode::Executable:
			ErrorAndExit("Executable output format is not supported: use '/p' for outputting a flat binary instead.");

		case OutputMode::FlatBinary:
			break;

		case OutputMode::SRecord:
			ErrorAndExit("S-record output format is not supported: use '/p' for outputting a flat binary instead.");
	}

	/* Finally, run the assembler. */
	if (!ClownAssembler::Assemble(source_file, object_file, std::cout, listing_stream, symbol_stream, source_file_path_string, settings, DefinitionCallback))
	{
		/* Delete the output file; it will be junk anyway.
		   Also, leaving a junk file will confuse CMake, which will think that the assembler had previously succeeded. */
		object_file.close();
		std::filesystem::remove(object_file_path);
		ErrorAndExit("Failed to assemble.");
	}

	return EXIT_SUCCESS;
}
