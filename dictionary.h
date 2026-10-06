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

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <cstddef>
#include <optional>
#include <stddef.h>
#include <string>
#include <string_view>
#include <unordered_map>

#include "clowncommon/clowncommon.h"

#include "strcmpci.h"
#include "string.h"

#define TOTAL_HASH_TABLE_ENTRIES 0x1000

struct Dictionary_StringHash
{
	using is_transparent = void;

	std::size_t operator()(const std::string_view &string) const { return std::hash<std::string_view>{}(string); }
};

typedef struct Dictionary_Entry
{
	int type = -1;
	union
	{
		unsigned long unsigned_long;
		long signed_long;
		void *pointer;
		String string;
	} shared; 
} Dictionary_Entry;

typedef struct Dictionary_State
{
	std::optional<std::unordered_map<std::string, Dictionary_Entry, Dictionary_StringHash, std::equal_to<>>> internal;
} Dictionary_State;

cc_bool Dictionary_Init(Dictionary_State *state, cc_bool case_insensitive);
void Dictionary_Deinit(Dictionary_State *state);

cc_bool Dictionary_LookUpAndCreateIfNotExist(Dictionary_State *state, const StringView &identifier, Dictionary_Entry **entry_pointer);
Dictionary_Entry* Dictionary_LookUp(Dictionary_State *state, const StringView &identifier);
cc_bool Dictionary_Remove(Dictionary_State *state, const StringView &identifier);
void Dictionary_Filter(Dictionary_State *state, cc_bool (*filter_function)(Dictionary_Entry *entry, const char *identifier, size_t identifier_length, void *user_data), const void *user_data);

#endif /* DICTIONARY_H */
