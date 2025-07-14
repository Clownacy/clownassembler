#ifndef STRING_H
#define STRING_H

#include <stddef.h>

#include "clowncommon/clowncommon.h"

typedef struct StringView
{
	const char *buffer;
	size_t length;
} StringView;

typedef struct String
{
	StringView view;
} String;

void StringView_Create(StringView *view, const char *source_buffer, size_t source_length);
#define StringView_Data(VIEW) ((VIEW)->buffer)
#define StringView_Length(VIEW) ((VIEW)->length)
#define StringView_Empty(VIEW) (StringView_Length(VIEW) == 0)
#define StringView_At(VIEW, INDEX) (StringView_Data(VIEW)[INDEX])
cc_bool StringView_Compare(const StringView *view, const StringView *other_view);

void String_CreateBlank(String *string);
cc_bool String_CreateInternal(String *string, const char *source_1_buffer, size_t source_1_length, const char *source_2_buffer, size_t source_2_length);
#define String_Create(OUTPUT, INPUT_BUFFER, INPUT_LENGTH) String_CreateInternal(OUTPUT, INPUT_BUFFER, INPUT_LENGTH, NULL, 0)
#define String_CreateCopyView(OUTPUT, INPUT) String_Create(OUTPUT, StringView_Data(INPUT), StringView_Length(INPUT))
#define String_CreateCopy(OUTPUT, INPUT) String_CreateCopyView(OUTPUT, String_View(INPUT))
#define String_CreateAppendView(OUTPUT, INPUT_1, INPUT_2) String_CreateInternal(OUTPUT, StringView_Data(INPUT_1), StringView_Length(INPUT_1), StringView_Data(INPUT_2), StringView_Length(INPUT_2))
#define String_CreateAppend(OUTPUT, INPUT_1, INPUT_2) String_CreateAppendView(OUTPUT, String_View(INPUT_1), String_View(INPUT_2))
void String_CreateMove(String *string, String *other_string);
void String_Destroy(String *string);

#define String_View(STRING) ((const StringView*)&(STRING)->view)
#define String_Data(STRING) StringView_Data(String_View(STRING))
#define String_Length(STRING) StringView_Length(String_View(STRING))
#define String_Empty(STRING) StringView_Empty(String_View(STRING))
#define String_At(STRING, INDEX) StringView_At(String_View(STRING), INDEX)
#define String_Compare(STRING, OTHER_STRING) StringView_Compare(String_View(STRING), String_View(OTHER_STRING))

#endif /* STRING_H */
