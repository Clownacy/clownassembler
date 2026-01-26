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

#include <stdio.h>

#include "io.h"

typedef ClownAssembler_TextInput TextInput;
typedef ClownAssembler_BinaryInputOutput BinaryInputOutput;
typedef ClownAssembler_TextOutput TextOutput;

/* Default FILE-based IO callbacks */

static char* ReadLine(void* const user_data, char* const buffer, const size_t buffer_size)
{
	return fgets(buffer, buffer_size, (FILE*)user_data);
}

static void Seek(void* const user_data, const size_t position)
{
	fseek((FILE*)user_data, position, SEEK_SET);
}

static int ReadCharacter(void* const user_data)
{
	return fgetc((FILE*)user_data);
}

static size_t ReadCharacters(void* const user_data, char* const characters, const size_t total_characters)
{
	return fread(characters, 1, total_characters, (FILE*)user_data);
}

static void WriteCharacter(void* const user_data, const int character)
{
	fputc(character, (FILE*)user_data);
}

static void WriteCharacters(void* const user_data, const char* const characters, const size_t total_characters)
{
	fwrite(characters, 1, total_characters, (FILE*)user_data);
}

static void WriteString(void* const user_data, const char* const string)
{
	fputs(string, (FILE*)user_data);
}

static void PrintFormatted(void* const user_data, const char* const format, va_list args)
{
	vfprintf((FILE*)user_data, format, args);
}

void TextInput_OpenFILE(TextInput* const callbacks, FILE* const file)
{
	callbacks->user_data = file;
	callbacks->read_line = ReadLine;
}

cc_bool TextInput_OpenFile(TextInput* const callbacks, const char* const path)
{
	FILE* const input_file = fopen(path, "r");

	if (input_file == NULL)
		return cc_false;

	TextInput_OpenFILE(callbacks, input_file);
	return cc_true;
}

void TextInput_CloseFile(const TextInput* const callbacks)
{
	fclose((FILE*)callbacks->user_data);
}

char* TextInput_fgets(char* const buffer, const size_t buffer_size, const TextInput* const callbacks)
{
	return callbacks->read_line((void*)callbacks->user_data, buffer, buffer_size);
}

void BinaryInputOutput_OpenFILE(BinaryInputOutput* const callbacks, FILE *file)
{
	callbacks->user_data = file;
	callbacks->read_character = ReadCharacter;
	callbacks->read_characters = ReadCharacters;
	callbacks->write_character = WriteCharacter;
	callbacks->write_characters = WriteCharacters;
	callbacks->seek = Seek;
}

cc_bool BinaryInputOutput_OpenFile(BinaryInputOutput* const callbacks, const char* const path, const char* const mode)
{
	FILE* const output_file = fopen(path, mode);

	if (output_file == NULL)
		return cc_false;

	BinaryInputOutput_OpenFILE(callbacks, output_file);
	return cc_true;
}

void BinaryInputOutput_CloseFile(const BinaryInputOutput* const callbacks)
{
	fclose((FILE*)callbacks->user_data);
}

cc_bool BinaryInputOutput_exists(const BinaryInputOutput* const callbacks)
{
	return callbacks != NULL && callbacks->user_data != NULL;
}

void BinaryInputOutput_fseek(const BinaryInputOutput* const callbacks, const size_t position)
{
	callbacks->seek((void*)callbacks->user_data, position);
}

int BinaryInputOutput_fgetc(const BinaryInputOutput* const callbacks)
{
	return callbacks->read_character((void*)callbacks->user_data);
}

size_t BinaryInputOutput_fread(void* const buffer, const size_t size, const size_t count, const BinaryInputOutput* const callbacks)
{
	return callbacks->read_characters((void*)callbacks->user_data, (char*)buffer, size * count);
}

void BinaryInputOutput_fputc(const int character, const BinaryInputOutput* const callbacks)
{
	callbacks->write_character((void*)callbacks->user_data, character);
}

void BinaryInputOutput_fwrite(const void* const buffer, const size_t size, const size_t count, const BinaryInputOutput* const callbacks)
{
	callbacks->write_characters((void*)callbacks->user_data, (const char*)buffer, size * count);
}

void TextOutput_OpenFILE(TextOutput* const callbacks, FILE *file)
{
	callbacks->user_data = file;
	callbacks->print_formatted = PrintFormatted;
	callbacks->write_character = WriteCharacter;
	callbacks->write_string = WriteString;
}

cc_bool TextOutput_exists(const TextOutput* const callbacks)
{
	return callbacks != NULL && callbacks->user_data != NULL;
}

void TextOutput_vfprintf(const TextOutput* const callbacks, const char* const format, va_list args)
{
	if (TextOutput_exists(callbacks))
		callbacks->print_formatted((void*)callbacks->user_data, format, args);
}

void TextOutput_fprintf(const TextOutput* const callbacks, const char* const format, ...)
{
	if (TextOutput_exists(callbacks))
	{
		va_list args;
		va_start(args, format);
		TextOutput_vfprintf(callbacks, format, args);
		va_end(args);
	}
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
