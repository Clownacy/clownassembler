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

char* TextInput_fgets(char* const buffer, const size_t buffer_size, std::istream &stream)
{
	// This function needs to emulate 'fgets', which is why
	// it does so many bizarre things.
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

	return buffer;}
