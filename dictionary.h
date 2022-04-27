/*
 * Copyright (C) 2022 Clownacy
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

#include <stddef.h>

#include "clowncommon.h"

#define TOTAL_HASH_TABLE_ENTRIES 0x1000

typedef struct Dictionary_Entry
{
	int type;
	union
	{
		unsigned long unsigned_long;
		long signed_long;
		void *pointer;
	} shared; 
} Dictionary_Entry;

typedef struct Dictionary_Node
{
	struct Dictionary_Node *next;
	struct Dictionary_Node *previous;

	struct Dictionary_Node *parent;
	struct Dictionary_Node *left_child;
	struct Dictionary_Node *right_child;

	Dictionary_Entry entry;

	size_t identifier_length;
	char identifier; /* Actually a C89 flexible array member that *shouldn't* be undefined behaviour. */
} Dictionary_Node;

typedef struct Dictionary_Bucket
{
	Dictionary_Node *binary_search_tree;
	Dictionary_Node *linked_list;
} Dictionary_Bucket;

typedef struct Dictionary_State
{
	Dictionary_Bucket *hash_table;
	int (*compare_identifiers)(const void *lhs, const void *rhs, size_t count);
} Dictionary_State;

cc_bool Dictionary_Init(Dictionary_State *state, cc_bool case_insensitive);
void Dictionary_Deinit(Dictionary_State *state);

cc_bool Dictionary_LookUpAndCreateIfNotExist(Dictionary_State *state, const char *identifier, size_t identifier_length, Dictionary_Entry **entry_pointer);
Dictionary_Entry* Dictionary_LookUp(Dictionary_State *state, const char *identifier, size_t identifier_length);
cc_bool Dictionary_Remove(Dictionary_State *state, const char *identifier, size_t identifier_length);
void Dictionary_Filter(Dictionary_State *state, cc_bool (*filter_function)(Dictionary_Entry *entry, const char *identifier, size_t identifier_length, void *user_data), void *user_data);

#endif /* DICTIONARY_H */
