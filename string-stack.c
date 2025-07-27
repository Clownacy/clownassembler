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

#include "string-stack.h"

#include <stddef.h>
#include <stdlib.h>

void StringStack_Initialise(StringStack_State* const state)
{
	state->list = NULL;
}

void StringStack_Deinitialise(StringStack_State* const state)
{
	String string;

	while (StringStack_Pop(state, &string))
		String_Destroy(&string);
}

cc_bool StringStack_Push(StringStack_State* const state, const StringView* const view)
{
	StringStack_ListEntry* const list_entry = (StringStack_ListEntry*)malloc(sizeof(StringStack_ListEntry));

	if (list_entry == NULL)
		return cc_false;

	list_entry->next = state->list;
	state->list = list_entry;

	String_CreateCopyView(&list_entry->string, view);

	return cc_true;
}

cc_bool StringStack_Pop(StringStack_State* const state, String* const string)
{
	StringStack_ListEntry* const list_entry = state->list;

	if (list_entry == NULL)
		return cc_false;

	state->list = list_entry->next;

	String_CreateMove(string, &list_entry->string);
	String_Destroy(&list_entry->string);

	free(list_entry);

	return cc_true;
}
