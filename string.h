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
	size_t capacity;
} String;

#define STRING_POSITION_INVALID ((size_t)-1)

#define STRING_VIEW_INITIALISER(STRING_LITERAL) {STRING_LITERAL, sizeof(STRING_LITERAL) - 1}
#define STRING_VIEW_INITIALISER_BLANK STRING_VIEW_INITIALISER("")

void StringView_Create(StringView *view, const char *source_buffer, size_t source_length);
#define StringView_CreateBlank(VIEW) StringView_Create(VIEW, "", 0)
#define StringView_Data(VIEW) ((VIEW)->buffer)
#define StringView_Length(VIEW) ((VIEW)->length)
#define StringView_Empty(VIEW) (StringView_Length(VIEW) == 0)
#define StringView_At(VIEW, INDEX) StringView_Data(VIEW)[INDEX]
cc_bool StringView_Compare(const StringView *view, const StringView *other_view);
cc_bool StringView_CompareCStrCaseInsensitive(const StringView *view, const char *c_string);
size_t StringView_Find(const StringView *view, const StringView *sub_view, size_t position);
size_t StringView_FindCharacter(const StringView *view, char character, size_t position);
#define StringView_SubStr(VIEW, OTHER_VIEW, POSITION, LENGTH) StringView_Create(VIEW, &StringView_At(OTHER_VIEW, POSITION), LENGTH)
#define StringView_Front(VIEW) StringView_At(VIEW, 0)
#define StringView_Back(VIEW) StringView_At(VIEW, StringView_Length(VIEW) - 1)

#define STRING_INITIALISER(STRING_LITERAL) {STRING_VIEW_INITIALISER(STRING_LITERAL), sizeof(STRING_LITERAL) - 1}

void String_CreateBlank(String *string);
cc_bool String_CreateInternal(String *string, const char *source_1_buffer, size_t source_1_length, const char *source_2_buffer, size_t source_2_length);
#define String_Create(OUTPUT, INPUT_BUFFER, INPUT_LENGTH) String_CreateInternal(OUTPUT, INPUT_BUFFER, INPUT_LENGTH, NULL, 0)
cc_bool String_CreateCopyView(String *string, const StringView *source);
#define String_CreateCopy(OUTPUT, INPUT) String_CreateCopyView(OUTPUT, String_View(INPUT))
cc_bool String_CreateAppendView(String *string, const StringView *source_1, const StringView *source_2);
#define String_CreateAppend(OUTPUT, INPUT_1, INPUT_2) String_CreateAppendView(OUTPUT, String_View(INPUT_1), String_View(INPUT_2))
void String_CreateMove(String *string, String *other_string);
void String_Destroy(String *string);
cc_bool String_Replace(String *string, size_t position, size_t count, const StringView *other_string);
cc_bool String_Reserve(String *string, size_t size);
cc_bool String_Resize(String *string, size_t size);
cc_bool String_ResizeNoFill(String *string, size_t size);

#define String_View(STRING) ((const StringView*)&(STRING)->view)
#define String_Data(STRING) ((char*)StringView_Data(String_View(STRING)))
#define String_CStr String_Data
#define String_Length(STRING) StringView_Length(String_View(STRING))
#define String_Capacity(STRING) ((STRING)->capacity)
#define String_Empty(STRING) StringView_Empty(String_View(STRING))
#define String_At(STRING, INDEX) String_Data(STRING)[INDEX]
#define String_Compare(STRING, OTHER_STRING) StringView_Compare(String_View(STRING), String_View(OTHER_STRING))
#define String_CompareCStrCaseInsensitive(STRING, C_STRING) StringView_CompareCStrCaseInsensitive(String_View(STRING), C_STRING)
#define String_Find(STRING, SUB_VIEW, POSITION) StringView_Find(String_View(STRING), SUB_VIEW, POSITION)
#define String_FindCharacter(STRING, CHARACTER, POSITION) StringView_FindCharacter(String_View(STRING), CHARACTER, POSITION)
#define String_SubStr(STRING, VIEW, POSITION, LENGTH) String_Create(STRING, &StringView_At(VIEW, POSITION), LENGTH)
#define String_Front(STRING) StringView_Front(String_View(STRING))
#define String_Back(STRING) StringView_Back(String_View(STRING))

#endif /* STRING_H */
