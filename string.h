#ifndef STRING_H
#define STRING_H

#include <stddef.h>

#include "clowncommon/clowncommon.h"

typedef struct String
{
	char *buffer;
	size_t length;
} String;

cc_bool String_CreateInternal(String *string, const char *source_1_buffer, size_t source_1_length, const char *source_2_buffer, size_t source_2_length);
#define String_Create(OUTPUT, INPUT_BUFFER, INPUT_LENGTH) String_CreateInternal(OUTPUT, INPUT_BUFFER, INPUT_LENGTH, NULL, 0)
#define String_Copy(OUTPUT, INPUT) String_Create(OUTPUT, (INPUT)->buffer, (INPUT)->length)
#define String_Append(OUTPUT, INPUT_1, INPUT_2) String_CreateInternal(OUTPUT, (INPUT_1)->buffer, (INPUT_1)->length, (INPUT_2)->buffer, (INPUT_2)->length)
void String_Destroy(String *string);

cc_bool String_Compare(const String *string, const String *other_string);
#define String_Buffer(STRING) (STRING)->buffer
#define String_Length(STRING) (STRING)->length

#endif /* STRING_H */
