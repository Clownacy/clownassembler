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

#include <stdarg.h>
#include <stddef.h>

#include "clowncommon/clowncommon.h"

struct FILE;

typedef char* (*ClownAssembler_ReadLine)(void *user_data, char *buffer, size_t buffer_size);
typedef void (*ClownAssembler_Seek)(void *user_data, size_t position);
typedef int (*ClownAssembler_ReadCharacter)(void *user_data);
typedef size_t (*ClownAssembler_ReadCharacters)(void *user_data, char *characters, size_t total_characters);
typedef void (*ClownAssembler_WriteCharacter)(void *user_data, int character);
typedef void (*ClownAssembler_WriteCharacters)(void *user_data, const char *characters, size_t total_characters);
typedef void (*ClownAssembler_WriteString)(void *user_data, const char *string);
typedef void (*ClownAssembler_PrintFormatted)(void *user_data, const char *format, va_list args);

typedef struct ClownAssembler_TextInput
{
	const void *user_data;
	ClownAssembler_ReadLine read_line;
} ClownAssembler_TextInput;

typedef struct ClownAssembler_BinaryInputOutput
{
	const void *user_data;
	ClownAssembler_ReadCharacter read_character;
	ClownAssembler_ReadCharacters read_characters;
	ClownAssembler_WriteCharacter write_character;
	ClownAssembler_WriteCharacters write_characters;
	ClownAssembler_Seek seek;
} ClownAssembler_BinaryInputOutput;

typedef struct ClownAssembler_TextOutput
{
	const void *user_data;
	ClownAssembler_PrintFormatted print_formatted;
	ClownAssembler_WriteCharacter write_character;
	ClownAssembler_WriteString write_string;
} ClownAssembler_TextOutput;

void TextInput_OpenFILE(ClownAssembler_TextInput *callbacks, FILE *file);
cc_bool TextInput_OpenFile(ClownAssembler_TextInput *callbacks, const char *path);
void TextInput_CloseFile(const ClownAssembler_TextInput *callbacks);
char* TextInput_fgets(char *buffer, size_t buffer_size, const ClownAssembler_TextInput *callbacks);

void BinaryInputOutput_OpenFILE(ClownAssembler_BinaryInputOutput *callbacks, FILE *file);
cc_bool BinaryInputOutput_OpenFile(ClownAssembler_BinaryInputOutput *callbacks, const char *path, const char *mode);
void BinaryInputOutput_CloseFile(const ClownAssembler_BinaryInputOutput *callbacks);
cc_bool BinaryInputOutput_exists(const ClownAssembler_BinaryInputOutput *callbacks);
void BinaryInputOutput_fseek(const ClownAssembler_BinaryInputOutput *callbacks, size_t position);
#define BinaryInputOutput_rewind(CALLBACKS) BinaryInputOutput_fseek(CALLBACKS, 0)
int BinaryInputOutput_fgetc(const ClownAssembler_BinaryInputOutput *callbacks);
size_t BinaryInputOutput_fread(void *buffer, size_t size, size_t count, const ClownAssembler_BinaryInputOutput *callbacks);
void BinaryInputOutput_fputc(int character, const ClownAssembler_BinaryInputOutput *callbacks);
void BinaryInputOutput_fwrite(const void *buffer, size_t size, size_t count, const ClownAssembler_BinaryInputOutput *callbacks);

void TextOutput_OpenFILE(ClownAssembler_TextOutput *callbacks, FILE *file);
cc_bool TextOutput_exists(const ClownAssembler_TextOutput *callbacks);
void TextOutput_vfprintf(const ClownAssembler_TextOutput *callbacks, const char *format, va_list args);
CC_ATTRIBUTE_PRINTF(2, 3) void TextOutput_fprintf(const ClownAssembler_TextOutput *callbacks, const char *format, ...);
void TextOutput_fputs(const char *string, const ClownAssembler_TextOutput *callbacks);
void TextOutput_fputc(int character, const ClownAssembler_TextOutput *callbacks);

#endif /* IO_H */
