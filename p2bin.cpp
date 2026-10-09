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

#include <exception>
#include <stddef.h>
#include <string.h>

static std::istream *input_stream;
static std::ostream *output_stream;
static const ClownAssembler_TextOutput *error_callbacks;
static cc_bool base_address_found = cc_false;
static unsigned long base_address;

static unsigned int ReadByte(void)
{
	const int byte = input_stream->get();

	return (unsigned int)byte;
}

static void ReadBytes(unsigned char* const buffer, const unsigned int total_bytes)
{
	input_stream->read(reinterpret_cast<char*>(buffer), total_bytes);
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
	/* TODO: 'malloc' this? */
	static unsigned char copy_buffer[0x1000];

	unsigned long i;

	const unsigned long start_address = ReadLongInt();
	const unsigned int length = ReadWord();
	
	if (!base_address_found)
	{
		base_address_found = cc_true;
		base_address = start_address;
	}
	else if (start_address < base_address)
	{
		throw std::runtime_error("Error: Segment located before start of file.\n");
	}
	else
	{
		output_stream->seekp(start_address - base_address);
	}

	/* Copy segment data. We do some batching using a buffer to improve performance. */
	for (i = 0; i < length; i += sizeof(copy_buffer))
	{
		const unsigned long bytes_to_do = CC_MIN(sizeof(copy_buffer), length - i);

		ReadBytes(copy_buffer, bytes_to_do);
		output_stream->write(reinterpret_cast<const char*>(copy_buffer), bytes_to_do);
	}
}

static void ProcessRecords(void)
{
	for (;;)
	{
		const unsigned int record_header = ReadByte();
		unsigned int granularity;

		switch (record_header)
		{
			case 0:
				/* Creator string. This marks the end of the file. */
				return;

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
					throw std::runtime_error(std::format("Error: Unsupported granularity of {} (only 1 is supported).\n", granularity));

				ProcessSegment();

				break;

			default:
				if (record_header >= 0x80)
					throw std::runtime_error(std::format("Error: Unrecognised record header value (0x{:02X}).\n", record_header));

				/* Legacy CODE segment. */
				ProcessSegment();

				break;
		}
	}
}

cc_bool ConvertObjectFileToFlatBinary(std::istream &input_stream_parameter, std::ostream &output_stream_parameter, const ClownAssembler_TextOutput* const error_callbacks_parameter)
{
	unsigned char magic[2];

	cc_bool success = cc_false;

	input_stream = &input_stream_parameter;
	output_stream = &output_stream_parameter;
	error_callbacks = error_callbacks_parameter;

	const auto previous_exceptions = input_stream->exceptions();
	input_stream->exceptions(input_stream->badbit | input_stream->eofbit | input_stream->failbit);

	try
	{
		/* Read and check the header's magic number. */
		input_stream->read(reinterpret_cast<char*>(magic), sizeof(magic));

		if (magic[0] != 0x89 || magic[1] != 0x14)
			throw std::runtime_error(std::format("Error: Invalid header magic value - expected 0x8914 but got 0x{:02X}{:02X}.\nInput file is either corrupt or not a valid AS code file.\n", magic[0], magic[1]));

		ProcessRecords();

		success = cc_true;
	}
	catch (const std::runtime_error &error)
	{
		TextOutput_fputs(error.what(), error_callbacks);
	}

	input_stream->exceptions(previous_exceptions);

	return success;
}
