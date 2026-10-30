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

#ifndef SOURCE_LINE_READER_H
#define SOURCE_LINE_READER_H

#include <iostream>
#include <optional>
#include <string_view>
#include <vector>

class SourceLineReader
{
private:
	std::istream &input_stream;

	std::vector<char> line_buffer = std::vector<char>(0x100); // Room for 256 characters by default; will expand when necessary.

	void GrowLineBuffer();
	bool ReadRaw(std::size_t &line_buffer_write_position);

public:
	SourceLineReader(std::istream &input_stream)
		: input_stream(input_stream)
	{}

	std::optional<std::string_view> Read();
};

#endif /* SOURCE_LINE_READER_H */
