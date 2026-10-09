/*
 * Copyright (C) 2024-2025 Clownacy
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

#include <cstring>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io.h"

typedef ClownAssembler_TextInput TextInput;

/* Default FILE-based IO callbacks */

static char* File_ReadLine(void* const user_data, char* const buffer, const size_t buffer_size)
{
	// This function needs to emulate 'fgets', which is why
	// it does so many bizarre things.
	auto &stream = *static_cast<std::istream*>(user_data);

	stream.clear();
	stream.getline(buffer, buffer_size);

	if (stream.fail())
	{
		// If we had room to read characters but read
		// nothing, then we reached the end of the file.
		if (buffer_size > 1 && buffer[0] == '\0')
			return nullptr;

		// Otherwise, we just could not fit the full line
		// in the buffer.
		return buffer;
	}

	// Append a newline, since we read a full line.
	// The buffer is guaranteed to not be full, as that
	// would have caused the fail bit to be set.
	const auto end = std::strlen(buffer);
	buffer[end + 0] = '\n';
	buffer[end + 1] = '\0';

	return buffer;
}


/* TextInput */

void TextInput_OpenFILE(TextInput* const callbacks, std::istream &file)
{
	callbacks->user_data = &file;
	callbacks->read_line = File_ReadLine;
}

cc_bool TextInput_OpenFile(TextInput* const callbacks, const char* const path)
{
	std::ifstream input_file(path);

	if (!input_file.is_open())
		return cc_false;

	TextInput_OpenFILE(callbacks, *new std::ifstream(std::move(input_file)));
	return cc_true;
}

void TextInput_CloseFile(const TextInput* const callbacks)
{
	delete static_cast<std::ifstream*>(callbacks->user_data);
}

char* TextInput_fgets(char* const buffer, const size_t buffer_size, const TextInput* const callbacks)
{
	return callbacks->read_line((void*)callbacks->user_data, buffer, buffer_size);
}
