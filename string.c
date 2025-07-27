/*
 * Copyright (C) 2025 Clownacy
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

#include "string.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "strcmpci.h"

static char string_dummy_buffer[] = "";

void StringView_Create(StringView* const view, const char* const source_buffer, const size_t source_length)
{
	assert(view != NULL);

	view->buffer = source_buffer;
	view->length = source_length;
}

cc_bool StringView_Compare(const StringView* const view, const StringView* const other_view)
{
	assert(view != NULL && other_view != NULL);

	return StringView_Length(view) == StringView_Length(other_view) && memcmp(StringView_Data(view), StringView_Data(other_view), StringView_Length(view)) == 0;
}

cc_bool StringView_CompareCStrCaseInsensitive(const StringView* const view, const char* const c_string)
{
	assert(view != NULL && c_string != NULL);

	return StringView_Length(view) != 0 && strncmpci(StringView_Data(view), c_string, StringView_Length(view)) == 0;
}

size_t StringView_Find(const StringView* const view, const StringView* const sub_view, const size_t position)
{
	size_t i;

	assert(view != NULL && sub_view != NULL);

	if (StringView_Length(view) < StringView_Length(sub_view))
		return STRING_POSITION_INVALID;

	for (i = position; i <= StringView_Length(view) - StringView_Length(sub_view); ++i)
		if (memcmp(&StringView_At(view, i), StringView_Data(sub_view), StringView_Length(sub_view)) == 0)
			return i;

	return STRING_POSITION_INVALID;
}

size_t StringView_FindCharacter(const StringView* const view, const char character, const size_t position)
{
	const char* const found_pointer = (const char*)memchr(StringView_Data(view) + position, character, StringView_Length(view) - position);

	if (found_pointer == NULL)
		return STRING_POSITION_INVALID;

	return found_pointer - StringView_Data(view);
}

void String_CreateBlank(String* const string)
{
	assert(string != NULL);

	StringView_Create(&string->view, string_dummy_buffer, sizeof(string_dummy_buffer) - 1);
	string->capacity = StringView_Length(&string->view);
}

cc_bool String_CreateInternal(String* const string, const char* const source_1_buffer, const size_t source_1_length, const char* const source_2_buffer, const size_t source_2_length)
{
	cc_bool success = cc_true;
	const size_t output_length = source_1_length + source_2_length;

	assert(string != NULL);
	assert((source_1_buffer != NULL || source_1_length == 0) && (source_2_buffer != NULL || source_2_length == 0));

	if (output_length != 0)
	{
		char* const output_buffer = (char*)malloc(output_length + 1);

		if (output_buffer == NULL)
		{
			success = cc_false;
		}
		else
		{
			if (source_1_length != 0)
				memcpy(output_buffer, source_1_buffer, source_1_length);
			if (source_2_length != 0)
				memcpy(output_buffer + source_1_length, source_2_buffer, source_2_length);
			output_buffer[output_length] = '\0';

			StringView_Create(&string->view, output_buffer, output_length);
			string->capacity = StringView_Length(&string->view);
			return success;
		}
	}

	String_CreateBlank(string);
	return success;
}

cc_bool String_CreateCopyView(String* const string, const StringView* const source)
{
	assert(string != NULL && source != NULL);

	return String_CreateInternal(string, StringView_Data(source), StringView_Length(source), NULL, 0);
}

cc_bool String_CreateAppendView(String* const string, const StringView* const source_1, const StringView* const source_2)
{
	assert(string != NULL && source_1 != NULL && source_2 != NULL);

	return String_CreateInternal(string, StringView_Data(source_1), StringView_Length(source_1), StringView_Data(source_2), StringView_Length(source_2));
}

void String_CreateMove(String* const string, String* const other_string)
{
	assert(string != NULL && other_string != NULL);

	*string = *other_string;
	String_CreateBlank(other_string);
}

void String_Destroy(String* const string)
{
	assert(string != NULL);

	if (string->view.buffer == string_dummy_buffer)
		return;

	free((char*)string->view.buffer);

	String_CreateBlank(string);
}

cc_bool String_Replace(String* const string, const size_t position, const size_t count, const StringView* const other_string)
{
	assert(string != NULL);
	assert(other_string != NULL);
	assert(position <= String_Length(string));
	assert(position + count <= String_Length(string));

	{
		const size_t first_half_length = position;
		const size_t parameter_length = StringView_Length(other_string);
		const size_t second_half_length = String_Length(string) - (position + count);
		const size_t new_length = first_half_length + parameter_length + second_half_length;

		char* const new_buffer = (char*)malloc(new_length + 1);

		if (new_buffer == NULL)
			return cc_false;

		memcpy(new_buffer, String_Data(string), first_half_length);
		memcpy(new_buffer + first_half_length, StringView_Data(other_string), parameter_length);
		memcpy(new_buffer + first_half_length + parameter_length, &String_At(string, position + count), second_half_length);
		new_buffer[first_half_length + parameter_length + second_half_length] = '\0';

		String_Destroy(string);
		StringView_Create(&string->view, new_buffer, new_length);
		string->capacity = StringView_Length(&string->view);
		return cc_true;
	}
}

cc_bool String_Reserve(String* const string, const size_t size)
{
	assert(string != NULL);

	{
		/* As an optimisation, we do not shrink the buffer. */
		const size_t new_capacity = CC_MAX(size, string->capacity);
		char* const old_buffer = String_Data(string) == string_dummy_buffer ? NULL : (char*)string->view.buffer;
		char* const new_buffer = realloc(old_buffer, new_capacity + 1);

		if (new_buffer == NULL)
			return cc_false;

		string->view.buffer = new_buffer;
		string->capacity = new_capacity;

		return cc_true;
	}
}

cc_bool String_Resize(String* const string, const size_t size)
{
	assert(string != NULL);

	{
		const size_t old_size = String_Length(string);

		if (!String_ResizeNoFill(string, size))
			return cc_false;

		/* Fill the new space with null characters. */
		if (size > old_size)
			memset(&String_At(string, old_size), '\0', size - old_size);

		return cc_true;
	}
}

cc_bool String_ResizeNoFill(String* const string, const size_t size)
{
	assert(string != NULL);

	if (!String_Reserve(string, size))
		return cc_false;

	/* Update the view with the new size. */
	string->view.length = size;

	/* Add the terminating null character. */
	String_At(string, String_Length(string)) = '\0';

	return cc_true;
}
