/*
 * Copyright (C) 2022-2025 Clownacy
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

#include "dictionary.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "clowncommon/clowncommon.h"

#include "strcmpci.h"

cc_bool Dictionary_Init(Dictionary_State *state, cc_bool case_insensitive)
{
	state->internal.emplace();

	return cc_true;
}

void Dictionary_Deinit(Dictionary_State *state)
{
	state->internal.reset();
}

cc_bool Dictionary_LookUpAndCreateIfNotExist(Dictionary_State *state, const StringView &identifier, Dictionary_Entry **entry_pointer)
{
	*entry_pointer = &(*state->internal)[{identifier.buffer, identifier.length}];
	return cc_true;
}

Dictionary_Entry* Dictionary_LookUp(Dictionary_State *state, const StringView &identifier)
{
	auto it = state->internal->find({identifier.buffer, identifier.length});

	if (it == state->internal->end())
		return nullptr;

	return &it->second;
}

cc_bool Dictionary_Remove(Dictionary_State *state, const StringView &identifier)
{
	auto it = state->internal->find({identifier.buffer, identifier.length});

	if (it == state->internal->end())
		return cc_false;

	state->internal->erase(it);

	return cc_true;
}

void Dictionary_Filter(Dictionary_State *state, cc_bool (*filter_function)(Dictionary_Entry *entry, const char *identifier, size_t identifier_length, void *user_data), const void *user_data)
{
	for (auto it = state->internal->begin(); it != state->internal->end(); ++it)
		if (filter_function(&it->second, it->first.data(), it->first.length(), const_cast<void*>(user_data)))
			state->internal->erase(it);
}
