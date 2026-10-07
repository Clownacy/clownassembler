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

#include "substitute.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

cc_bool Substitute::PushSubstitute(const StringView &identifier, const String &value)
{
	list.emplace_front(std::string(identifier), &value);
	return cc_true;
}

void Substitute::PopSubstitute()
{
	list.pop_front();
}

cc_bool Substitute::FindSubstitute(const StringView &view_to_search, const size_t starting_position, const StringView &substitute, const cc_bool allow_implicit_matches, const cc_bool case_insensitive, size_t* const found_position, size_t* const found_length)
{
	size_t match_start;

	/* Find identifier within string. */
	/* Obviously bail if the identifier wasn't found. */
	for (match_start = starting_position; (match_start = StringView_Find(view_to_search, substitute, match_start, case_insensitive)) != STRING_POSITION_INVALID; ++match_start)
	{
		size_t match_length = StringView_Length(substitute);

		const char character_before = match_start == 0 ? ' ' : StringView_At(view_to_search, match_start - 1);
		const char character_after = match_start + match_length == StringView_Length(view_to_search) ? ' ' : StringView_At(view_to_search, match_start + match_length);

		/* If the identifier was in the middle of a larger block of letters/numbers, then don't replace it. */
		/* (This is what AS does, and the Sonic 1 disassembly relies on this). */
		if (IsSubstituteBlockingCharacter(character_before) || IsSubstituteBlockingCharacter(character_after))
			continue;

		/* If the parameter is surrounded by backslashes, then expand the match to replace those too. */
		/* asm68k allows backslashes before and after the parameter to separate them from surrounding characters. */
		if (character_before == '\\')
		{
			--match_start;
			++match_length;

			if (character_after == '\\')
				++match_length;
		}
		else if (!allow_implicit_matches)
		{
			continue;
		}

		*found_position = match_start;
		*found_length = match_length;
		return cc_true;
	}

	return cc_false;
}

std::optional<StringView> Substitute::FindEarliestSubstitute(Substitute* const other_state, const StringView &view_to_search, const size_t starting_position, const CustomSearch custom_search_callback, const void* const custom_search_user_data, const cc_bool allow_implicit_matches, const cc_bool case_insensitive, size_t* const earliest_found_position, size_t* const earliest_found_length)
{
	std::optional<StringView> found_substitute;

	*earliest_found_position = (size_t)-1;
	*earliest_found_length = 0;

	if (custom_search_callback != NULL)
	{
		size_t found_position, found_length;

		found_substitute = custom_search_callback((void*)custom_search_user_data, view_to_search, starting_position, case_insensitive, &found_position, &found_length);

		if (found_substitute.has_value())
		{
			*earliest_found_position = found_position;
			*earliest_found_length = found_length;

			/* Absorb trailing backslash. */
			/* TODO: Check if it already ends in a backslash? */
			if (StringView_At(view_to_search, found_position + found_length) == '\\')
				++*earliest_found_length;
		}
	}

	if (other_state != NULL)
	{
		/* Search all substitutes, looking for the earliest. */
		for (const auto &entry : other_state->list)
		{
			size_t found_position, found_length;

			if (FindSubstitute(view_to_search, starting_position, String_View(entry.identifier), allow_implicit_matches, case_insensitive, &found_position, &found_length))
			{
				/* Record if this substitute occurs first. */
				if (*earliest_found_position > found_position)
				{
					found_substitute = String_View(*entry.value);
					*earliest_found_position = found_position;
					*earliest_found_length = found_length;
				}
			}
		}
	}

	/* Search all substitutes, looking for the earliest. */
	for (const auto &entry : list)
	{
		size_t found_position, found_length;

		if (FindSubstitute(view_to_search, starting_position, String_View(entry.identifier), allow_implicit_matches, case_insensitive, &found_position, &found_length))
		{
			/* Record if this substitute occurs first. */
			if (*earliest_found_position > found_position)
			{
				found_substitute = String_View(*entry.value);
				*earliest_found_position = found_position;
				*earliest_found_length = found_length;
			}
		}
	}

	return found_substitute;
}

void Substitute::ProcessSubString(Substitute* const other_state, String &string, StringView &view_to_search, const CustomSearch custom_search_callback, const void* const custom_search_user_data, const cc_bool allow_implicit_matches, const cc_bool case_insensitive)
{
	size_t starting_position = 0;

	const size_t offset_into_string = StringView_Data(view_to_search) - String_Data(string);
	const char removed_character = String_At(string, offset_into_string + StringView_Length(view_to_search));

	/* Null-terminate the view, so that search logic can use C's silly null-terminated string functions. */
	String_At(string, offset_into_string + StringView_Length(view_to_search)) = '\0';

	for (;;)
	{
		/* Find a substitute. */
		size_t found_position, found_length;
		const std::optional<StringView> found_substitute = FindEarliestSubstitute(other_state, view_to_search, starting_position, custom_search_callback, custom_search_user_data, allow_implicit_matches, case_insensitive, &found_position, &found_length);

		if (!found_substitute.has_value())
			break;

		/* Substitute it. */
		String_Replace(string, offset_into_string + found_position, found_length, *found_substitute);

		/* Update the view. */
		StringView_Create(view_to_search, &String_At(string, offset_into_string), StringView_Length(view_to_search) - found_length + StringView_Length(*found_substitute));

		/* Limit the next search to after this. */
		starting_position = found_position + StringView_Length(*found_substitute);
	}

	/* Restore the character that we replaced with a null character earlier. */
	String_At(string, offset_into_string + StringView_Length(view_to_search)) = removed_character;
}

void Substitute::ProcessString(Substitute* const other_state, String &string, const CustomSearch custom_search_callback, const void* const custom_search_user_data, const cc_bool allow_implicit_matches, const cc_bool case_insensitive)
{
	StringView view = String_View(string);
	ProcessSubString(other_state, string, view, custom_search_callback, custom_search_user_data, allow_implicit_matches, case_insensitive);
}

cc_bool Substitute::IsSubstituteBlockingCharacter(const char character)
{
	return (isalpha((unsigned char)character) != 0
	     || isdigit((unsigned char)character) != 0
	     || character == '$'
	     || character == '_'
	     || character == '.');
}

cc_bool Substitute::IsWhitespaceCharacter(const char character)
{
	return isspace((unsigned char)character) != 0;
}
