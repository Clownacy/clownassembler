#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLOWNMD5_IMPLEMENTATION
#define CLOWNMD5_STATIC
#include "clownmd5.h"

typedef unsigned char Hash[16];

static void HashFile(FILE* const file, Hash hash)
{
	ClownMD5_State state;
	unsigned char buffer[16 * 4];

	ClownMD5_Init(&state);

	for (;;)
	{
		const size_t bytes_read = fread(buffer, 1, sizeof(buffer), file);

		if (bytes_read != sizeof(buffer))
		{
			ClownMD5_PushFinalData(&state, buffer, bytes_read * 8, hash);
			break;
		}

		ClownMD5_PushData(&state, buffer);
	}
}

static int HashFromString(const char* const string, Hash hash)
{
	unsigned int values[16];
	unsigned int i;

	const int values_read = sscanf(string, "%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X",
		&values[0],
		&values[1],
		&values[2],
		&values[3],
		&values[4],
		&values[5],
		&values[6],
		&values[7],
		&values[8],
		&values[9],
		&values[10],
		&values[11],
		&values[12],
		&values[13],
		&values[14],
		&values[15]
	);

	if (values_read != 16)
		return 0;

	for (i = 0; i < 16; ++i)
		hash[i] = values[i];

	return 1;
}

int main(const int argc, char** const argv)
{
	int exit_code = EXIT_FAILURE;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s filename md5hash\n", argv[0]);
		exit_code = EXIT_SUCCESS;
	}
	else
	{
		Hash expected_hash;

		if (!HashFromString(argv[2], expected_hash))
		{
			fputs("Could not parse hash.\n", stderr);
		}
		else
		{
			const char* const file_path = argv[1];
			FILE* const file = fopen(file_path, "rb");

			if (file == NULL)
			{
				fprintf(stderr, "Could not open file '%s'.\n", file_path);
			}
			else
			{
				Hash hash;

				HashFile(file, hash);

				fclose(file);

				if (memcmp(hash, expected_hash, sizeof(Hash)) == 0)
					exit_code = EXIT_SUCCESS;
			}
		}
	}

	return exit_code;
}
