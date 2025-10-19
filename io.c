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

#include "io.h"

typedef ClownAssembler_TextInput TextInput;
typedef ClownAssembler_BinaryOutput BinaryOutput;
typedef ClownAssembler_TextOutput TextOutput;

char* TextInput_fgets(char* const buffer, const size_t buffer_size, const TextInput* const callbacks)
{
	return callbacks->read_line((void*)callbacks->user_data, buffer, buffer_size);
}

cc_bool BinaryOutput_exists(const BinaryOutput* const callbacks)
{
	return callbacks != NULL && callbacks->user_data != NULL;
}

void BinaryOutput_fputc(const int character, const BinaryOutput* const callbacks)
{
	callbacks->write_character((void*)callbacks->user_data, character);
}

void BinaryOutput_fseek(const BinaryOutput* const callbacks, const size_t position)
{
	callbacks->seek((void*)callbacks->user_data, position);
}

void BinaryOutput_fwrite(const void* const buffer, const size_t size, const size_t count, const BinaryOutput* const callbacks)
{
	callbacks->write_characters((void*)callbacks->user_data, (const char*)buffer, size * count);
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
