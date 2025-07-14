#include "string.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static const char string_dummy_buffer[] = "";

cc_bool String_CreateInternal(String* const string, const char* const source_1_buffer, const size_t source_1_length, const char* const source_2_buffer, const size_t source_2_length)
{
	cc_bool success = cc_true;
	const size_t output_length = source_1_length + source_2_length;

	assert(string != NULL);
	assert((source_1_buffer != NULL || source_1_length == 0) && (source_2_buffer != NULL || source_2_length == 0));

	if (output_length != 0)
	{
		string->buffer = (char*)malloc(output_length + 1);

		if (string->buffer == NULL)
		{
			success = cc_false;
		}
		else
		{
			if (source_1_length != 0)
				memcpy(string->buffer, source_1_buffer, source_1_length);
			if (source_2_length != 0)
				memcpy(string->buffer + source_1_length, source_2_buffer, source_2_length);
			string->buffer[output_length] = '\0';
			string->length = output_length;
			return success;
		}
	}

	string->buffer = (char*)string_dummy_buffer;
	string->length = 0;

	return success;
}

void String_Destroy(String* const string)
{
	assert(string != NULL);

	if (string->buffer == string_dummy_buffer)
		return;

	free(string->buffer);

	string->buffer = (char*)string_dummy_buffer;
	string->length = 0;
}

cc_bool String_Compare(const String* const string, const String* const other_string)
{
	return String_Length(string) == String_Length(other_string) && memcmp(String_Data(string), String_Data(other_string), String_Length(string)) == 0;
}
