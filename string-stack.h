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

#ifndef STRING_STACK_H
#define STRING_STACK_H

#include "clowncommon/clowncommon.h"

#include "string.h"

typedef struct StringStack_ListEntry
{
	struct StringStack_ListEntry *next;

	String string;
} StringStack_ListEntry;

typedef struct StringStack_State
{
	StringStack_ListEntry *list;
} StringStack_State;

void StringStack_Initialise(StringStack_State *state);
void StringStack_Deinitialise(StringStack_State *state);

cc_bool StringStack_Push(StringStack_State *state, const StringView *view);
cc_bool StringStack_Pop(StringStack_State *state, String *string);

#endif /* STRING_STACK_H */
