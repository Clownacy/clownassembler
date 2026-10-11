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

#include <algorithm>
#include <cassert>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>

#include "case-insensitive.h"
#include "strcmpci.h"

#define STRING_POSITION_INVALID ((size_t)-1)

using String = std::string;
using StringView = std::string_view;

#define STRING_VIEW_INITIALISER(x) x
#define STRING_VIEW_INITIALISER_BLANK {}
#define StringView_Data(x) ((x).data())
#define StringView_Length(x) ((x).length())
#define StringView_Empty(x) ((x).empty())
#define StringView_At(x, i) ((x)[i])
#define StringView_Front(x) ((x).front())
#define StringView_Back(x) ((x).back())
#define String_At(x, i) ((x)[i])
#define String_CStr(x) ((x).c_str())
#define String_Data(x) ((x).data())
#define String_Empty(x) ((x).empty())
#define String_Length(x) ((x).length())
#define String_Reserve(x, size) ((x).reserve(size))
#define String_Capacity(x) ((x).capacity())

inline void String_CreateCopyView(String &string, const StringView &view)
{
	string = view;
}

inline void String_CreateCopy(String &string, const String &other_string)
{
	string = other_string;
}

inline void String_CreateMove(String &string, String &other_string)
{
	string = std::move(other_string);
}

inline void String_CreateAppend(String &string, const String &other_string_a, const String &other_string_b)
{
	string = other_string_a + other_string_b;
}

#define String_View(x) (std::string_view((x)))
#define String_Replace(string, position, count, other_string) ((string).replace(position, count, other_string))
#define String_Compare(a, b) ((a) == (b))
#define StringView_Create(x, buffer, length) ((x) = std::string_view(buffer, length))
#define StringView_CreateBlank(x) ((x) = {})
inline void StringView_SubStr(StringView &view, const StringView &other_view, const std::size_t position, const std::size_t length = StringView::npos)
{
	view = other_view.substr(position, length);
}
inline size_t StringView_StrSpn(const StringView &view, const char* const characters, const std::size_t position = 0)
{
	return std::min(view.length(), view.find_first_not_of(characters, position));
}
#define StringView_RemovePrefix(VIEW, COUNT) ((VIEW).remove_prefix(COUNT))
#define StringView_RemoveSuffix(VIEW, COUNT) ((VIEW).remove_suffix(COUNT))

inline void String_ToLower(String &string)
{
	constexpr CaseInsensitive::ToLower to_lower(true);

	for (auto &character : string)
		character = to_lower(character);
}

#define String_CompareCStrCaseInsensitive(string, c_string) StringView_CompareCStrCaseInsensitive(String_View(string), c_string)

inline size_t StringView_Find(const StringView &view, const StringView &sub_view, const size_t position, const cc_bool case_insensitive)
{
	const auto it = std::search(std::begin(view) + position, std::end(view), std::begin(sub_view), std::end(sub_view), CaseInsensitive::CharComparer(case_insensitive));

	if (it == std::end(view))
		return STRING_POSITION_INVALID;

	return std::distance(std::begin(view), it);
}

inline size_t StringView_FindCharacter(const StringView &view, const char character, const size_t position)
{
	const auto found_position = view.find(character, position);

	if (found_position == view.npos)
		return STRING_POSITION_INVALID;

	return found_position;
}

#define String_Find(string, sub_view, position, case_insensitive) StringView_Find(String_View(string), sub_view, position, case_insensitive)
#define String_StrSpn(STRING, CHARACTERS) StringView_StrSpn(String_View(STRING), CHARACTERS)

#endif /* STRING_H */
