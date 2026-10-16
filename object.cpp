/*
 * Copyright (C) 2026 Clownacy
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

#include "object.h"

#include <iostream>

#include "io.h"
#include "version.h"

void Object::Writer::TerminatePreviousRecordSegment()
{
	if (previous_segment_length_output_position != 0)
	{
		if (segment_length == 0)
		{
			/* Previous segment contained nothing; make the new segment overwrite it. */
			output.stream.seekp(previous_segment_length_output_position);
			output.stream.seekp(-5, output.stream.cur);
		}
		else
		{
			/* Write length of previous record. */
			const auto previous_output_position = output.stream.tellp();
			output.stream.seekp(previous_segment_length_output_position);
			output.WriteU16LE(segment_length);
			output.stream.seekp(previous_output_position);
		}
	}
}

void Object::Writer::WriteRecordSegment()
{
	TerminatePreviousRecordSegment();

	/* CPU architecture (68000). */
	output.WriteU8(0x01);
	/* Start address. */
	output.WriteU32LE(segment_position);
	/* Length (placeholder). */
	previous_segment_length_output_position = output.stream.tellp();
	output.WriteU16LE(0x0000);
}

void Object::Writer::WriteHeader()
{
	/* Object file magic number. */
	output.WriteU16BE(0x8914);

	/* Begin first segment. */
	WriteRecordSegment();
}

void Object::Writer::WriteFooter()
{
	/* Finalise the segment record data. */
	TerminatePreviousRecordSegment();

	/* Write terminating record. */
	output.WriteU8(0x00);
	output.stream << CLOWNASSEMBLER_VERSION_STRING;
}

void Object::Writer::WriteByte(const unsigned int value, const bool doing_fixup_pass)
{
	if (segment_length == 0xFFFF)
	{
		segment_position += segment_length;

		/* Avoid rewriting the record header when doing fix-ups,
		   as the length field will be left blank. */
		/* TODO: Make this boolean less dumb. */
		if (doing_fixup_pass)
			output.stream.seekp(1 + 4 + 2, output.stream.cur);
		else
			WriteRecordSegment();

		segment_length = 0;
	}

	++segment_length;

	output.WriteU8(value);
}

void Object::Writer::BeginNewSegment(const std::size_t position)
{
	/* Begin a new segment! */
	segment_position = position;
	WriteRecordSegment();
	segment_length = 0;
}
