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

#include "source-line-reader.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <string_view>

void SourceLineReader::GrowLineBuffer()
{
	line_buffer.resize(line_buffer.size() * 2);
}

bool SourceLineReader::ReadRaw(std::size_t &line_buffer_write_position)
{
	if (input_stream.eof())
		return false;

	for (;;)
	{
		input_stream.getline(&line_buffer[line_buffer_write_position], line_buffer.size() - line_buffer_write_position);
		line_buffer_write_position += input_stream.gcount();

		if (input_stream.eof())
			break;

		/* Do not count the newline. */
		--line_buffer_write_position;

		/* Failure indicates the buffer was not big enough; if it
		   is not set, then we have read a full line and can exit. */
		if (!input_stream.fail())
			break;

		/* Make more room for the line. */
		GrowLineBuffer();

		/* Now loop back around and read more of the line! */
	}

	return true;
}

std::optional<std::string_view> SourceLineReader::Read()
{
	bool data_read = false;
	std::size_t line_buffer_read_position = 0, line_buffer_write_position = 0;

	while (ReadRaw(line_buffer_write_position))
	{
		data_read = true;

		/* Find where the code ends and a comment begins.
		   Note that terminating at ';' prevents a trailing '&' from being recognised
		   and causing a line to be continued on the next line.
		   This is needed for compatibility with S.N. 68k (asm68k). */
		/* TODO: Does S.N. 68k actually concatenate when a '&' occurs before a comment? */
		char quote_character = '\0';

		for (; line_buffer_read_position < line_buffer_write_position; ++line_buffer_read_position)
		{
			const char character = line_buffer[line_buffer_read_position];

			if (quote_character == '\0')
			{
				if (character == '"' || character == '\'')
					quote_character = character;
				else if (character == ';')
					break;
			}
			else if (character == quote_character)
			{
				quote_character = '\0';
			}
		}

		/* An '&' at the end of a line is like a '\' at the end of a line in C:
		   it signals that the current line is continued on the next line. */
		if (line_buffer_read_position == 0 || line_buffer[line_buffer_read_position - 1] != '&')
			break;

		/* Discard the '&'. */
		--line_buffer_read_position;
		--line_buffer_write_position;

		/* Go back and get another line. */
	}

	if (!data_read)
		return std::nullopt;

	/* Flex requires that its input be padded with two null characters. */
	if (line_buffer_read_position + 2 > line_buffer.size())
		GrowLineBuffer();

	line_buffer[line_buffer_read_position++] = '\0';
	line_buffer[line_buffer_read_position++] = '\0';

	return std::string_view{line_buffer.data(), line_buffer_read_position};
}
