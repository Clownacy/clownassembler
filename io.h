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

#ifndef IO_H
#define IO_H

#include <iostream>
#include <format>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "clowncommon/clowncommon.h"

typedef char* (*ClownAssembler_ReadLine)(void *user_data, char *buffer, size_t buffer_size);
typedef void (*ClownAssembler_Seek)(void *user_data, size_t position);
typedef int (*ClownAssembler_ReadCharacter)(void *user_data);
typedef size_t (*ClownAssembler_ReadCharacters)(void *user_data, void *characters, size_t total_characters);
typedef void (*ClownAssembler_WriteCharacter)(void *user_data, int character);
typedef void (*ClownAssembler_WriteCharacters)(void *user_data, const void *characters, size_t total_characters);
typedef void (*ClownAssembler_WriteString)(void *user_data, const char *string);

typedef struct ClownAssembler_TextInput
{
	void *user_data;
	ClownAssembler_ReadLine read_line;
} ClownAssembler_TextInput;

typedef struct ClownAssembler_TextOutput
{
	void *user_data;
	ClownAssembler_WriteCharacter write_character;
	ClownAssembler_WriteString write_string;
} ClownAssembler_TextOutput;

typedef struct ClownAssembler_BinaryStream
{
	void *user_data;
	ClownAssembler_ReadCharacter read_character;
	ClownAssembler_ReadCharacters read_characters;
	ClownAssembler_WriteCharacter write_character;
	ClownAssembler_WriteCharacters write_characters;
	ClownAssembler_Seek seek;
} ClownAssembler_BinaryStream;

void TextInput_OpenFILE(ClownAssembler_TextInput *callbacks, std::istream &file);
cc_bool TextInput_OpenFile(ClownAssembler_TextInput *callbacks, const char *path);
void TextInput_CloseFile(const ClownAssembler_TextInput *callbacks);
char* TextInput_fgets(char *buffer, size_t buffer_size, const ClownAssembler_TextInput *callbacks);

void TextOutput_OpenFILE(ClownAssembler_TextOutput *callbacks, std::ostream &file);
cc_bool TextOutput_exists(const ClownAssembler_TextOutput *callbacks);
void TextOutput_fputs(const char *string, const ClownAssembler_TextOutput *callbacks);
void TextOutput_fputc(int character, const ClownAssembler_TextOutput *callbacks);

template<typename... Args>
void TextOutput_Format(const ClownAssembler_TextOutput *callbacks, std::format_string<Args...> &&format_string, Args &&...args)
{
	const auto string = std::format(std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);
	TextOutput_fputs(string.c_str(), callbacks);
}

void BinaryStream_OpenFILE(ClownAssembler_BinaryStream *callbacks, std::iostream &file);
cc_bool BinaryStream_OpenMemory(ClownAssembler_BinaryStream *callbacks);
void BinaryStream_CloseMemory(ClownAssembler_BinaryStream *callbacks);
cc_bool BinaryStream_exists(const ClownAssembler_BinaryStream *callbacks);
void BinaryStream_fseek(const ClownAssembler_BinaryStream *callbacks, size_t position);
#define BinaryStream_rewind(CALLBACKS) BinaryStream_fseek(CALLBACKS, 0)
int BinaryStream_fgetc(const ClownAssembler_BinaryStream *callbacks);
size_t BinaryStream_fread(void *buffer, size_t size, size_t count, const ClownAssembler_BinaryStream *callbacks);
void BinaryStream_fputc(int character, const ClownAssembler_BinaryStream *callbacks);
void BinaryStream_fwrite(const void *buffer, size_t size, size_t count, const ClownAssembler_BinaryStream *callbacks);

#endif /* IO_H */
