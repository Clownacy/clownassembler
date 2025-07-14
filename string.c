#include "string.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static const char string_dummy_buffer[] = "";

void StringView_Create(StringView* const view, const char* const source_buffer, const size_t source_length)
{
	view->buffer = source_buffer;
	view->length = source_length;
}

cc_bool StringView_Compare(const StringView* const view, const StringView* const other_view)
{
	return StringView_Length(view) == StringView_Length(other_view) && memcmp(StringView_Data(view), StringView_Data(other_view), StringView_Length(view)) == 0;
}

void String_CreateBlank(String* const string)
{
	StringView_Create(&string->view, string_dummy_buffer, 0);
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
			return success;
		}
	}

	String_CreateBlank(string);
	return success;
}

void String_CreateMove(String* const string, String* const other_string)
{
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
