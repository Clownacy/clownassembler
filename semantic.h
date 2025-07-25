/*
 * Copyright (C) 2022 Clownacy
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

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "clowncommon/clowncommon.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "options.h"

typedef char* (*ClownAssembler_ReadLine)(void *user_data, char *buffer, size_t buffer_size);
typedef void (*ClownAssembler_Seek)(void *user_data, size_t position);
typedef void (*ClownAssembler_WriteCharacter)(void *user_data, int character);
typedef void (*ClownAssembler_WriteCharacters)(void *user_data, const char *characters, size_t total_characters);
typedef void (*ClownAssembler_WriteString)(void *user_data, const char *string);
typedef void (*ClownAssembler_PrintFormatted)(void *user_data, const char *format, va_list args);

typedef struct ClownAssembler_TextInput
{
	const void *user_data;
	ClownAssembler_ReadLine read_line;
} ClownAssembler_TextInput;

typedef struct ClownAssembler_BinaryOutput
{
	const void *user_data;
	ClownAssembler_WriteCharacter write_character;
	ClownAssembler_WriteCharacters write_characters;
	ClownAssembler_Seek seek;
} ClownAssembler_BinaryOutput;

typedef struct ClownAssembler_TextOutput
{
	const void *user_data;
	ClownAssembler_PrintFormatted print_formatted;
	ClownAssembler_WriteCharacter write_character;
	ClownAssembler_WriteString write_string;
} ClownAssembler_TextOutput;

typedef Options ClownAssembler_Settings;

typedef void (*ClownAssembler_AddDefinition)(void *internal, const char *identifier, size_t identifier_length, unsigned long value);
typedef void (*ClownAssembler_DefinitionCallback)(void *internal, void *user_data, ClownAssembler_AddDefinition add_definition);

cc_bool ClownAssembler_AssembleFile(
	FILE *input_callbacks,
	FILE *output_callbacks,
	FILE *error_callbacks,
	FILE *listing_callbacks,
	FILE *symbol_callbacks,
	const char *input_file_path,
	const ClownAssembler_Settings *settings,
	ClownAssembler_DefinitionCallback definition_callback,
	const void *user_data);

cc_bool ClownAssembler_Assemble(
	const ClownAssembler_TextInput *input_callbacks,
	const ClownAssembler_BinaryOutput *output_callbacks,
	const ClownAssembler_TextOutput *error_callbacks,
	const ClownAssembler_TextOutput *listing_callbacks,
	const ClownAssembler_BinaryOutput *symbol_callbacks,
	const char *input_file_path,
	const ClownAssembler_Settings *settings,
	ClownAssembler_DefinitionCallback definition_callback,
	const void *user_data);

#ifdef __cplusplus
}
#endif

#endif /* SEMANTIC_H */
