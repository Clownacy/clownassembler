#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clowncommon.h"

#include "semantic.h"

#define ERROR(message) do { fputs("Error: " message "\n", stderr); exit_code = EXIT_FAILURE;} while (0)

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	const char *input_file_path;
	const char *output_file_path;
	const char *listing_file_path;
	cc_bool case_insensitive;
	cc_bool debug;
	int i;

	input_file_path = NULL;
	output_file_path = NULL;
	listing_file_path = NULL;
	case_insensitive = cc_false;
	debug = cc_false;

	for (i = 1; i < argc; ++i)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
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

				case 'c':
					case_insensitive = cc_true;
					continue;

				case 'd':
					debug = cc_true;
					continue;
			}
		}

		fprintf(stderr, "Error: Unrecognised option '%s'.\n", argv[i]);
		exit_code = EXIT_FAILURE;
	}

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

				if (!ClownAssembler_Assemble(input_file, output_file, listing_file, input_file_path != NULL ? input_file_path : "STDIN", debug, case_insensitive))
					ERROR("Could not assemble.");

				if (listing_file != NULL)
					fclose(listing_file);

				fclose(output_file);
			}

			fclose(input_file);
		}
	}

	return exit_code;
}
