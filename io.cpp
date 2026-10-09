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
typedef ClownAssembler_TextOutput TextOutput;
typedef ClownAssembler_BinaryStream BinaryStream;

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

static void File_Seek(void* const user_data, const size_t position)
{
	auto &stream = *static_cast<std::iostream*>(user_data);
	stream.seekg(position);
	stream.seekp(position);
}

static int File_ReadCharacter(void* const user_data)
{
	auto &stream = *static_cast<std::iostream*>(user_data);

	const auto character = stream.get();

	if (stream.gcount() == 0)
		return EOF;

	return character;
}

static size_t File_ReadCharacters(void* const user_data, void* const characters, const size_t total_characters)
{
	auto &stream = *static_cast<std::iostream*>(user_data);

	stream.read(static_cast<char*>(characters), total_characters);

	return stream.gcount();
}

static void File_WriteCharacterTextOutput(void* const user_data, const int character)
{
	auto &stream = *static_cast<std::ostream*>(user_data);

	stream.put(character);
}

static void File_WriteCharacter(void* const user_data, const int character)
{
	auto &stream = *static_cast<std::iostream*>(user_data);

	stream.put(character);
}

static void File_WriteCharacters(void* const user_data, const void* const characters, const size_t total_characters)
{
	auto &stream = *static_cast<std::iostream*>(user_data);

	stream.write(static_cast<const char*>(characters), total_characters);
}

static void File_WriteString(void* const user_data, const char* const string)
{
	auto &stream = *static_cast<std::ostream*>(user_data);

	stream << string;
}

/* Memory-based IO callbacks */

typedef struct MemoryIO
{
	std::stringstream stream;
} MemoryIO;

static void Memory_Seek(void* const user_data, const size_t position)
{
	MemoryIO* const state = (MemoryIO*)user_data;

	state->stream.seekg(position);
	state->stream.seekp(position);
}

static int Memory_ReadCharacter(void* const user_data)
{
	MemoryIO* const state = (MemoryIO*)user_data;

	const auto character = state->stream.get();

	if (state->stream.gcount() == 0)
		return EOF;

	return character;
}

static size_t Memory_ReadCharacters(void* const user_data, void* const characters, const size_t total_characters)
{
	MemoryIO* const state = (MemoryIO*)user_data;

	state->stream.read(static_cast<char*>(characters), total_characters);

	return state->stream.gcount();
}

static void Memory_WriteCharacters(void* const user_data, const void* const characters, const size_t total_characters)
{
	MemoryIO* const state = (MemoryIO*)user_data;

	state->stream.write(static_cast<const char*>(characters), total_characters);
}

static void Memory_WriteCharacter(void* const user_data, const int character)
{
	MemoryIO* const state = (MemoryIO*)user_data;

	state->stream.put(character);
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

/* TextOutput */

void TextOutput_OpenFILE(TextOutput* const callbacks, std::ostream &file)
{
	callbacks->user_data = &file;
	callbacks->write_character = File_WriteCharacterTextOutput;
	callbacks->write_string = File_WriteString;
}

cc_bool TextOutput_exists(const TextOutput* const callbacks)
{
	return callbacks != NULL && callbacks->user_data != NULL;
}

void TextOutput_fputs(const char* const string, const TextOutput* const callbacks)
{
	if (TextOutput_exists(callbacks))
		callbacks->write_string((void*)callbacks->user_data, string);
}

void TextOutput_fputc(const int character, const TextOutput* const callbacks)
{
	if (TextOutput_exists(callbacks))
		callbacks->write_character((void*)callbacks->user_data, character);
}

/* BinaryStream */

void BinaryStream_OpenFILE(BinaryStream* const callbacks, std::iostream &file)
{
	callbacks->user_data = &file;
	callbacks->read_character = File_ReadCharacter;
	callbacks->read_characters = File_ReadCharacters;
	callbacks->write_character = File_WriteCharacter;
	callbacks->write_characters = File_WriteCharacters;
	callbacks->seek = File_Seek;
}

cc_bool BinaryStream_OpenMemory(BinaryStream* const callbacks)
{
	MemoryIO* const state = new MemoryIO;

	callbacks->user_data = state;
	callbacks->read_character = Memory_ReadCharacter;
	callbacks->read_characters = Memory_ReadCharacters;
	callbacks->write_character = Memory_WriteCharacter;
	callbacks->write_characters = Memory_WriteCharacters;
	callbacks->seek = Memory_Seek;

	return cc_true;
}

void BinaryStream_CloseMemory(BinaryStream* const callbacks)
{
	MemoryIO* const state = (MemoryIO*)callbacks->user_data;

	delete state;
}

cc_bool BinaryStream_exists(const BinaryStream* const callbacks)
{
	return callbacks != NULL && callbacks->user_data != NULL;
}

void BinaryStream_fseek(const BinaryStream* const callbacks, const size_t position)
{
	callbacks->seek((void*)callbacks->user_data, position);
}

int BinaryStream_fgetc(const BinaryStream* const callbacks)
{
	return callbacks->read_character((void*)callbacks->user_data);
}

size_t BinaryStream_fread(void* const buffer, const size_t size, const size_t count, const BinaryStream* const callbacks)
{
	return callbacks->read_characters((void*)callbacks->user_data, (char*)buffer, size * count);
}

void BinaryStream_fputc(const int character, const BinaryStream* const callbacks)
{
	callbacks->write_character((void*)callbacks->user_data, character);
}

void BinaryStream_fwrite(const void* const buffer, const size_t size, const size_t count, const BinaryStream* const callbacks)
{
	callbacks->write_characters((void*)callbacks->user_data, (const char*)buffer, size * count);
}
