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

#ifndef LISTING_H
#define LISTING_H

#include <iostream>
#include <string_view>

#include "io.h"

class ListingWriter
{
private:
	std::ostream* const listing_stream;
	bool &doing_fixup_pass;

	unsigned int listing_counter = 0;
	bool line_in_progress = false;
	bool suppress_listing = false;

public:
	bool listing_macro = false;

	ListingWriter(std::ostream* const listing_stream, bool &doing_fixup_pass)
		: listing_stream(listing_stream)
		, doing_fixup_pass(doing_fixup_pass)
	{}

	void BeginLine(const unsigned long program_counter)
	{
		if (listing_stream == nullptr)
			return;

		line_in_progress = true;

		listing_counter = 0;
		suppress_listing = false;
		IO::Print(*listing_stream, "{:08X}", program_counter);
	}

	void SuppressLine()
	{
		suppress_listing = true;
	}

	void WriteByte(const unsigned int value)
	{
		if (listing_stream == nullptr || doing_fixup_pass || suppress_listing)
			return;

		/* We can only write up to 10 bytes. */
		if (listing_counter <= 25)
		{
			if (listing_counter == 25)
			{
				/* After the last byte, we output a '+' to signal there's more. */
				listing_stream->put('+');
				++listing_counter;
			}
			else
			{
				if (listing_counter % 5 == 0)
				{
					listing_stream->put(' ');
					++listing_counter;
				}

				IO::Print(*listing_stream, "{:02X}", value);
				listing_counter += 2;
			}
		}
	}

	void WriteIdentifierValue(const unsigned long value)
	{
		/* Output line to listing file. */
		if (listing_stream == nullptr || doing_fixup_pass || suppress_listing)
			return;

		IO::Print(*listing_stream, " ={:08X}", value);

		listing_counter += 2 + 8;
	}

	void EndLine(const std::string_view &source_line)
	{
		/* Output line to listing file. */
		if (listing_stream == nullptr)
			return;

		if (line_in_progress)
		{
			line_in_progress = false;

			for (unsigned int i = listing_counter; i < 26; ++i)
				listing_stream->put(' ');

			listing_stream->put(listing_macro ? 'M' : ' ');

			IO::Print(*listing_stream, " {}\n", source_line);
		}
	}
};

#endif /* LISTING_H */
