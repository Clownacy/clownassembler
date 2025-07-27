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

#include "options.h"

#include <stdlib.h>

static cc_bool Options_PushInternal(Options_State* const state, const Options* const options)
{
	Options_ListEntry* const list_entry = (Options_ListEntry*)malloc(sizeof(Options_ListEntry));

	if (list_entry == NULL)
		return cc_false;

	list_entry->next = state->list;
	state->list = list_entry;

	list_entry->options = *options;

	return cc_true;
}

static void Options_PopInternal(Options_State* const state)
{
	Options_ListEntry* const list_entry = state->list;

	state->list = list_entry->next;

	free(list_entry);
}

cc_bool Options_Initialise(Options_State* const state, const Options* const options)
{
	state->list = NULL;
	return Options_PushInternal(state, options);
}

void Options_Deinitialise(Options_State *state)
{
	while (state->list != NULL)
		Options_PopInternal(state);
}

cc_bool Options_Push(Options_State *state)
{
	return Options_PushInternal(state, Options_Get(state));
}

cc_bool Options_Pop(Options_State *state)
{
	if (state->list->next == NULL)
		return cc_false;

	Options_PopInternal(state);

	return cc_true;
}
