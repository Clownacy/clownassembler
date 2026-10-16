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

	void SuppressLine()
	{
		suppress_listing = true;
	}

	void BeginLine(unsigned long program_counter);
	void WriteByte(unsigned int value);
	void WriteIdentifierValue(unsigned long value);
	void EndLine(const std::string_view &source_line);
};

#endif /* LISTING_H */
