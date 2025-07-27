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

void Substitute_Initialise(Substitute_State* const state)
{
	state->list_head = NULL;
}

void Substitute_Deinitialise(Substitute_State* const state)
{
	while (state->list_head != NULL)
		Substitute_PopSubstitute(state);
}

cc_bool Substitute_PushSubstitute(Substitute_State* const state, const StringView* const identifier, const StringView* const value)
{
	Substitute_ListEntry* const list_entry = (Substitute_ListEntry*)malloc(sizeof(Substitute_ListEntry));

	if (list_entry == NULL)
		return cc_false;

	list_entry->next = state->list_head;
	state->list_head = list_entry;

	String_CreateCopyView(&list_entry->identifier, identifier);
	String_CreateCopyView(&list_entry->value, value);

	return cc_true;
}

void Substitute_PopSubstitute(Substitute_State* const state)
{
	Substitute_ListEntry* const list_entry = state->list_head;

	state->list_head = list_entry->next;

	String_Destroy(&list_entry->identifier);
	String_Destroy(&list_entry->value);

	free(list_entry);
}

static cc_bool Substitute_FindSubstitute(const StringView* const view_to_search, const size_t starting_position, const StringView* const substitute, const cc_bool allow_implicit_matches, size_t* const found_position, size_t* const found_length)
{
	size_t match_start;

	/* Find identifier within string. */
	/* Obviously bail if the identifier wasn't found. */
	for (match_start = starting_position; (match_start = StringView_Find(view_to_search, substitute, match_start)) != STRING_POSITION_INVALID; ++match_start)
	{
		size_t match_length = StringView_Length(substitute);

		const char character_before = match_start == 0 ? ' ' : StringView_At(view_to_search, match_start - 1);
		const char character_after = match_start + match_length == StringView_Length(view_to_search) ? ' ' : StringView_At(view_to_search, match_start + match_length);

		/* If the identifier was in the middle of a larger block of letters/numbers, then don't replace it. */
		/* (This is what AS does, and the Sonic 1 disassembly relies on this). */
		if (Substitute_IsSubstituteBlockingCharacter(character_before) || Substitute_IsSubstituteBlockingCharacter(character_after))
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

static const StringView* Substitute_FindEarliestSubstitute(Substitute_State* const state, const StringView* const view_to_search, const size_t starting_position, const Substitute_CustomSearch custom_search_callback, const void* const custom_search_user_data, const cc_bool allow_implicit_matches, size_t* const earliest_found_position, size_t* const earliest_found_length)
{
	Substitute_ListEntry *list_entry;
	const StringView *found_substitute = NULL;

	*earliest_found_position = (size_t)-1;
	*earliest_found_length = 0;

	if (custom_search_callback != NULL)
	{
		size_t found_position, found_length;

		found_substitute = custom_search_callback((void*)custom_search_user_data, view_to_search, starting_position, &found_position, &found_length);

		if (found_substitute != NULL)
		{
			*earliest_found_position = found_position;
			*earliest_found_length = found_length;
		}
	}

	/* Search all substitutes, looking for the earliest. */
	for (list_entry = state->list_head; list_entry != NULL; list_entry = list_entry->next)
	{
		size_t found_position, found_length;

		if (Substitute_FindSubstitute(view_to_search, starting_position, String_View(&list_entry->identifier), allow_implicit_matches, &found_position, &found_length))
		{
			/* Record if this substitute occurs first. */
			if (*earliest_found_position > found_position)
			{
				found_substitute = String_View(&list_entry->value);
				*earliest_found_position = found_position;
				*earliest_found_length = found_length;
			}
		}
	}

	return found_substitute;
}

void Substitute_ProcessSubString(Substitute_State* const state, String* const string, StringView* const view_to_search, const Substitute_CustomSearch custom_search_callback, const void* const custom_search_user_data, const cc_bool allow_implicit_matches)
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
		const StringView* const found_substitute = Substitute_FindEarliestSubstitute(state, view_to_search, starting_position, custom_search_callback, custom_search_user_data, allow_implicit_matches, &found_position, &found_length);

		if (found_substitute == NULL)
			break;

		/* Substitute it. */
		String_Replace(string, offset_into_string + found_position, found_length, found_substitute);

		/* Update the view. */
		StringView_Create(view_to_search, &String_At(string, offset_into_string), StringView_Length(view_to_search) + (StringView_Length(found_substitute) - found_length));

		/* Limit the next search to after this. */
		starting_position = found_position + StringView_Length(found_substitute);
	}

	/* Restore the character that we replaced with a null character earlier. */
	String_At(string, offset_into_string + StringView_Length(view_to_search)) = removed_character;
}

void Substitute_ProcessString(Substitute_State* const state, String* const string, const Substitute_CustomSearch custom_search_callback, const void* const custom_search_user_data, const cc_bool allow_implicit_matches)
{
	StringView view = *String_View(string);
	Substitute_ProcessSubString(state, string, &view, custom_search_callback, custom_search_user_data, allow_implicit_matches);
}

cc_bool Substitute_IsSubstituteBlockingCharacter(const char character)
{
	return (isalpha((unsigned char)character)
	     || isdigit((unsigned char)character)
	     || character == '_'
	     || character == '.');
}

cc_bool Substitute_IsWhitespaceCharacter(const char character)
{
	return isspace((unsigned char)character);
}
