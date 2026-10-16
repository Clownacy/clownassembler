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

#ifndef OBJECT_WRITER_H
#define OBJECT_WRITER_H

#include <iostream>

#include "listing.h"
#include "program-counter.h"

// Extends `Object::Writer` with an integer-writing interface, and
// handles listing as well as incrementing the program counter.
class ObjectWriter : public IO::BinaryWriter, public Object::Writer
{
private:
	ListingWriter &listing_writer;
	ProgramCounter &program_counter;
	bool &doing_fixup_pass;

public:
	ObjectWriter(
		std::ostream &stream,
		ListingWriter &listing_writer,
		ProgramCounter &program_counter,
		bool &doing_fixup_pass
	)
		: Object::Writer(stream)
		, listing_writer(listing_writer)
		, program_counter(program_counter)
		, doing_fixup_pass(doing_fixup_pass)
	{}

	void WriteU8(const unsigned int value) override final
	{
		/* Write to listing file. */
		listing_writer.WriteByte(value);

		/* Write to output file. */
		WriteU8WithoutListing(value);
	}

	void WriteU8WithoutListing(const unsigned int value)
	{
		Object::Writer::WriteByte(value, doing_fixup_pass);
		program_counter += 1;
	}

	// Do not allow user code to bypass `WriteU8` and `WriteU8WithoutListing`.
	void WriteByte(unsigned int value, bool doing_fixup_pass) = delete;
};

#endif /* OBJECT_WRITER_H */
