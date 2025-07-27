/*
 * Copyright (C) 2023-2025 Clownacy
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

/* Documentation of AS's code file format can be found here:
   http://john.ccac.rwth-aachen.de:8000/as/as_EN.html#sect_5_1_ */

/* Terminology in this code reflects the above documentation. */

#include "p2bin.h"

#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE *input_file;
static const ClownAssembler_BinaryOutput *output_file;
static const ClownAssembler_TextOutput *error_callbacks;
static jmp_buf jump_buffer;
static unsigned char padding_buffer[0x1000];
static unsigned long maximum_address = 0;
static unsigned int padding_value = 0;

static unsigned int ReadByte(void)
{
	const int byte = fgetc(input_file);

	if (byte == EOF)
	{
		TextOutput_fputs("Error: File ended prematurely.\n", error_callbacks);
		longjmp(jump_buffer, 1);
	}

	return (unsigned long)byte;
}

static void ReadBytes(unsigned char* const buffer, const unsigned int total_bytes)
{
	if (fread(buffer, total_bytes, 1, input_file) == 0)
	{
		TextOutput_fputs("Error: File ended prematurely.\n", error_callbacks);
		longjmp(jump_buffer, 1);
	}
}

static unsigned long ReadInteger(const unsigned int total_bytes)
{
	unsigned long value;
	unsigned int i;

	value = 0;

	for (i = 0; i < total_bytes; ++i)
		value |= (unsigned long)ReadByte() << (i * 8);

	return value;
}

static unsigned int ReadWord(void)
{
	return ReadInteger(2);
}

static unsigned long ReadLongInt(void)
{
	return ReadInteger(4);
}

static void ProcessSegment(void)
{
	static unsigned char copy_buffer[0x1000];

	unsigned long i;

	const unsigned long start_address = ReadLongInt();
	const unsigned int length = ReadWord();
	const unsigned long end_address = start_address + length;

	if (start_address > maximum_address)
	{
		/* Set padding bytes between segments. */
		const unsigned long padding_length = start_address - maximum_address;

		BinaryOutput_fseek(output_file, maximum_address);

		for (i = 0; i < padding_length; i += sizeof(padding_buffer))
			BinaryOutput_fwrite(padding_buffer, CC_MIN(sizeof(padding_buffer), padding_length - i), 1, output_file);
	}
	else
	{
		BinaryOutput_fseek(output_file, start_address);
	}

	/* Copy segment data. We do some batching using a buffer to improve performance. */
	for (i = 0; i < length; i += sizeof(copy_buffer))
	{
		const unsigned long bytes_to_do = CC_MIN(sizeof(copy_buffer), length - i);

		ReadBytes(copy_buffer, bytes_to_do);
		BinaryOutput_fwrite(copy_buffer, bytes_to_do, 1, output_file);
	}

	if (end_address > maximum_address)
		maximum_address = end_address;
}

static cc_bool ProcessRecords(void)
{
	memset(padding_buffer, padding_value, sizeof(padding_buffer));

	if (setjmp(jump_buffer) == 0)
	{
		for (;;)
		{
			const unsigned int record_header = ReadByte();
			unsigned int granularity;

			switch (record_header)
			{
				case 0:
					/* Creator string. This marks the end of the file. */
					return cc_true;

				case 0x80:
					/* Entry point. We don't care about this. */
					ReadLongInt();
					break;

				case 0x81:
					/* Arbitrary segment. */
					ReadByte(); /* Processor family. We don't care about this. */
					ReadByte(); /* Segment. We don't care about this. */
					granularity = ReadByte();

					if (granularity != 1)
					{
						TextOutput_fprintf(error_callbacks, "Error: Unsupported granularity of %u (only 1 is supported).\n", granularity);
						return cc_false;
					}

					ProcessSegment();

					break;

				default:
					if (record_header >= 0x80)
					{
						TextOutput_fprintf(error_callbacks, "Error: Unrecognised record header value (0x%02X).\n", record_header);
						return cc_false;
					}

					/* Legacy CODE segment. */
					ProcessSegment();

					break;
			}
		}
	}

	return cc_false;
}

cc_bool ConvertObjectFileToFlatBinary(FILE* const input_file_parameter, const ClownAssembler_BinaryOutput* const output_file_parameter, const ClownAssembler_TextOutput* const error_callbacks_parameter)
{
	unsigned char magic[2];

	cc_bool success = cc_false;

	input_file = input_file_parameter;
	output_file = output_file_parameter;
	error_callbacks = error_callbacks_parameter;

	/* Read and check the header's magic number. */
	if (fread(magic, sizeof(magic), 1, input_file) == 0)
		TextOutput_fputs("Error: Could not read header magic value.\n", error_callbacks);
	else if (magic[0] != 0x89 || magic[1] != 0x14)
		TextOutput_fprintf(error_callbacks, "Error: Invalid header magic value - expected 0x8914 but got 0x%02X%02X.\nInput file is either corrupt or not a valid AS code file.\n", magic[0], magic[1]);
	else if (ProcessRecords())
		success = cc_true;

	return success;
}
