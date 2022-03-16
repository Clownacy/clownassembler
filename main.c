#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "clowncommon.h"

#include "clownassembler.h"

#define ERROR(message) do { fputs("Error: " message "\n", stderr); exit_code = EXIT_FAILURE;} while (0)

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
				if (!ClownAssembler_Assemble(input_file, output_file))
					ERROR("Could not assemble");

				fclose(output_file);
			}

			fclose(input_file);
		}
	}

	return exit_code;
}
