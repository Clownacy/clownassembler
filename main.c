#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "clowncommon.h"

#include "semantic.h"

#define ERROR(message) do { fputs("Error: " message "\n", stderr); exit_code = EXIT_FAILURE;} while (0)

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	if (argc < 4)
	{
		ERROR("Input, output, and listing file paths must be passed as parameters");
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
				FILE *listing_file = fopen(argv[3], "w");

				if (listing_file == NULL)
				{
					ERROR("Could not open listing file");
				}
				else
				{
					if (!ClownAssembler_Assemble(input_file, output_file, listing_file, argv[1], argc > 4))
						ERROR("Could not assemble");

					fclose(listing_file);
				}

				fclose(output_file);
			}

			fclose(input_file);
		}
	}

	return exit_code;
}
