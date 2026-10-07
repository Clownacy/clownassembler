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

#include <cassert>
#include <optional>
#include <string>
#include <string_view>

#include "strcmpci.h"

#define STRING_POSITION_INVALID ((size_t)-1)

using String = std::string;
using StringView = std::string_view;

#define STRING_VIEW_INITIALISER(x) x
#define STRING_VIEW_INITIALISER_BLANK {}
#define StringView_Data(x) ((x).data())
#define StringView_Length(x) ((x).length())
#define StringView_Empty(x) ((x).empty())
#define StringView_At(x, i) ((x).data()[i])
#define StringView_Back(x) ((x).back())
#define String_At(x, i) ((x).data()[i])
#define String_CStr(x) ((x).c_str())
#define String_Data(x) ((x).data())
#define String_Empty(x) ((x).empty())
#define String_Length(x) ((x).length())
#define String_Reserve(x, size) ((x).reserve(size))
#define String_Capacity(x) ((x).capacity())
inline cc_bool String_ResizeNoFill(String &string, std::size_t size)
{
	string.resize(size);
	return cc_true;
}

inline cc_bool String_CreateCopyView(String &string, const StringView &view)
{
	string = view;
	return cc_true;
}

inline cc_bool String_Create(String &string, const char *buffer, size_t length)
{
	string = {buffer, length};
	return cc_true;
}

inline cc_bool String_CreateCopy(String &string, const String &other_string)
{
	string = other_string;
	return cc_true;
}

inline cc_bool String_CreateMove(String &string, String &other_string)
{
	string = std::move(other_string);
	return cc_true;
}

inline cc_bool String_CreateAppend(String &string, const String &other_string_a, const String &other_string_b)
{
	string = other_string_a + other_string_b;
	return cc_true;
}

#define String_View(x) (std::string_view((x)))
#define String_Replace(string, position, count, other_string) ((string).replace(position, count, other_string))
#define String_Compare(a, b) ((a) == (b))
#define StringView_Create(x, buffer, length) ((x) = std::string_view(buffer, length))
#define StringView_CreateBlank(x) ((x) = {})
#define StringView_SubStr(VIEW, OTHER_VIEW, POSITION, LENGTH) StringView_Create(VIEW, &StringView_At(OTHER_VIEW, POSITION), LENGTH)

inline void String_ToLower(String &string)
{
	for (auto &character : string)
		character = std::tolower(character);
}

inline cc_bool StringView_CompareCStrCaseInsensitive(const StringView &view, const char* const c_string)
{
	assert(c_string != NULL);

	return StringView_Length(view) != 0 && strncmpci(StringView_Data(view), c_string, StringView_Length(view)) == 0;
}

#define String_CompareCStrCaseInsensitive(string, c_string) StringView_CompareCStrCaseInsensitive(String_View(string), c_string)

inline size_t StringView_Find(const StringView &view, const StringView &sub_view, const size_t position, const cc_bool case_insensitive)
{
	size_t i;

	const MemoryComparisionFunction compare = case_insensitive ? memcasecmp : memcmp;

	if (StringView_Length(view) < StringView_Length(sub_view))
		return STRING_POSITION_INVALID;

	for (i = position; i <= StringView_Length(view) - StringView_Length(sub_view); ++i)
		if (compare(&StringView_At(view, i), StringView_Data(sub_view), StringView_Length(sub_view)) == 0)
			return i;

	return STRING_POSITION_INVALID;
}

inline size_t StringView_FindCharacter(const StringView &view, const char character, const size_t position)
{
	const char* const found_pointer = (const char*)memchr(StringView_Data(view) + position, character, StringView_Length(view) - position);

	if (found_pointer == NULL)
		return STRING_POSITION_INVALID;

	return found_pointer - StringView_Data(view);
}

#define String_Find(string, sub_view, position, case_insensitive) StringView_Find(String_View(string), sub_view, position, case_insensitive)

#endif /* STRING_H */
