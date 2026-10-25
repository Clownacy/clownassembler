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

#include <string>
#include <string_view>

#include "utilities/case-insensitive.h"
#include "utilities/string.h"

void Substitute::PushSubstitute(const std::string_view &identifier, const std::string &value)
{
	list.emplace_front(std::string(identifier), value);
}

void Substitute::PopSubstitute()
{
	list.pop_front();
}

bool Substitute::FindSubstitute(const std::string_view &view_to_search, const std::size_t starting_position, const std::string_view &substitute, const bool allow_implicit_matches, const bool case_insensitive, std::size_t &found_position, std::size_t &found_length)
{
	/* Find identifier within string. */
	/* Obviously bail if the identifier wasn't found. */
	for (std::size_t match_start = starting_position; (match_start = Utilities::CaseInsensitive::FindString(view_to_search, substitute, match_start, case_insensitive)) != view_to_search.npos; ++match_start)
	{
		std::size_t match_length = substitute.length();

		const char character_before = match_start == 0 ? ' ' : view_to_search[match_start - 1];
		const char character_after = match_start + match_length == view_to_search.length() ? ' ' : view_to_search[match_start + match_length];

		/* If the identifier was in the middle of a larger block of letters/numbers, then don't replace it. */
		/* (This is what AS does, and the Sonic 1 disassembly relies on this). */
		if (IsBlockingCharacter(character_before) || IsBlockingCharacter(character_after))
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

		found_position = match_start;
		found_length = match_length;
		return true;
	}

	return false;
}

std::optional<std::string_view> Substitute::FindEarliestSubstitute(Substitute* const other_state, const std::string_view &view_to_search, const std::size_t starting_position, const CustomSearch &custom_search_callback, const bool allow_implicit_matches, const bool case_insensitive, std::size_t &earliest_found_position, std::size_t &earliest_found_length)
{
	std::optional<std::string_view> found_substitute;

	earliest_found_position = static_cast<std::size_t>(-1);
	earliest_found_length = 0;

	if (custom_search_callback)
	{
		std::size_t found_position, found_length;

		found_substitute = custom_search_callback(view_to_search, starting_position, case_insensitive, found_position, found_length);

		if (found_substitute.has_value())
		{
			earliest_found_position = found_position;
			earliest_found_length = found_length;

			/* Absorb trailing backslash. */
			/* TODO: Check if it already ends in a backslash? */
			if (found_position + found_length < view_to_search.length() && view_to_search[found_position + found_length] == '\\')
				++earliest_found_length;
		}
	}

	const auto &DoList = [&](const List &list)
	{
		/* Search all substitutes, looking for the earliest. */
		for (const auto &entry : list)
		{
			std::size_t found_position, found_length;

			if (FindSubstitute(view_to_search, starting_position, entry.identifier, allow_implicit_matches, case_insensitive, found_position, found_length))
			{
				/* Record if this substitute occurs first. */
				if (earliest_found_position > found_position)
				{
					found_substitute = entry.value;
					earliest_found_position = found_position;
					earliest_found_length = found_length;
				}
			}
		}
	};

	if (other_state != nullptr)
		DoList(other_state->list);

	DoList(list);

	return found_substitute;
}

void Substitute::ProcessSubString(Substitute* const other_state, std::string &string, std::string_view &view_to_search, const CustomSearch &custom_search_callback, const bool allow_implicit_matches, const bool case_insensitive)
{
	std::size_t starting_position = 0;

	const std::size_t offset_into_string = view_to_search.data() - string.data();

	for (;;)
	{
		/* Find a substitute. */
		std::size_t found_position, found_length;
		const std::optional<std::string_view> found_substitute = FindEarliestSubstitute(other_state, view_to_search, starting_position, custom_search_callback, allow_implicit_matches, case_insensitive, found_position, found_length);

		if (!found_substitute.has_value())
			break;

		/* Substitute it. */
		string.replace(offset_into_string + found_position, found_length, *found_substitute);

		/* Update the view. */
		view_to_search = Utilities::String::Subview(string, offset_into_string, view_to_search.length() - found_length + found_substitute->length());

		/* Limit the next search to after this. */
		starting_position = found_position + found_substitute->length();
	}
}

void Substitute::ProcessString(Substitute* const other_state, std::string &string, const CustomSearch &custom_search_callback, const bool allow_implicit_matches, const bool case_insensitive)
{
	std::string_view view = string;
	ProcessSubString(other_state, string, view, custom_search_callback, allow_implicit_matches, case_insensitive);
}

bool Substitute::IsBlockingCharacter(const char character)
{
	return (character >= '0' && character <= '9')
	    || (character >= 'a' && character <= 'z')
	    || (character >= 'A' && character <= 'Z')
	    ||  character == '$'
	    ||  character == '_'
	    ||  character == '.';
}
