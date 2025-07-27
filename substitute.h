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

#ifndef SUBSTITUTE_H
#define SUBSTITUTE_H

#include "clowncommon/clowncommon.h"

#include "string.h"

typedef struct Substitute_ListEntry
{
	struct Substitute_ListEntry *next;
	String identifier;
	const StringView *value;
} Substitute_ListEntry;

typedef struct Substitute_State
{
	Substitute_ListEntry *list_head;
} Substitute_State;

typedef const StringView* (*Substitute_CustomSearch)(void *user_data, const StringView *view_to_search, size_t starting_position, size_t* const found_position, size_t* const found_length);

void Substitute_Initialise(Substitute_State *state);
void Substitute_Deinitialise(Substitute_State *state);

cc_bool Substitute_PushSubstitute(Substitute_State *state, const StringView *identifier, const StringView *value);
void Substitute_PopSubstitute(Substitute_State *state);

void Substitute_ProcessSubString(Substitute_State *state, String *string, StringView *view_to_search, Substitute_CustomSearch custom_search_callback, const void *custom_search_user_data, cc_bool allow_implicit_matches);
void Substitute_ProcessString(Substitute_State *state, String *string, Substitute_CustomSearch custom_search_callback, const void *custom_search_user_data, cc_bool allow_implicit_matches);

cc_bool Substitute_IsSubstituteBlockingCharacter(char character);
cc_bool Substitute_IsWhitespaceCharacter(char character);

#endif /* SUBSTITUTE_H */
