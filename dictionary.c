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

#include "dictionary.h"

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "clowncommon.h"

/* TODO - Destructors */

typedef enum SearchResult
{
	SEARCH_RESULT_FOUND,
	SEARCH_RESULT_NOT_FOUND_WOULD_BE_LEFT_CHILD,
	SEARCH_RESULT_NOT_FOUND_WOULD_BE_RIGHT_CHILD,
	SEARCH_RESULT_BUCKET_EMPTY
} SearchResult;

/* Like strcasecmp, but doesn't check for null characters. */
static int memcasecmp(const void *lhs, const void *rhs, size_t count)
{
	const unsigned char *lhs_uc;
	const unsigned char *rhs_uc;
	int delta;
	size_t i;

	lhs_uc = lhs;
	rhs_uc = rhs;
	delta = 0;

	for (i = 0; i < count; ++i)
	{
		const int lhs_character = tolower(*lhs_uc++);
		const int rhs_character = tolower(*rhs_uc++);

		delta = lhs_character - rhs_character;

		if (delta != 0)
			break;
	}

	return delta;
}

static Dictionary_Bucket* GetBucket(Dictionary_State *state, const char *identifier, size_t identifier_length)
{
	unsigned int hash;
	size_t i;

	/* Hash the identifier, using djb2. */
	/* http://www.cse.yorku.ca/~oz/hash.html */
	hash = 5381;

	for (i = 0; i < identifier_length; ++i)
		hash = hash * 33 + tolower(*identifier++); /* Hash the identifier in lower-case form, so that case-insensitive mode works. */

	return &state->hash_table[hash % TOTAL_HASH_TABLE_ENTRIES];
}

static SearchResult SearchBucket(Dictionary_State *state, Dictionary_Bucket *bucket, const char *identifier, size_t identifier_length, Dictionary_Node **node_pointer)
{
	SearchResult search_result;

	if (bucket->binary_search_tree == NULL)
	{
		search_result = SEARCH_RESULT_BUCKET_EMPTY;
	}
	else
	{
		Dictionary_Node *node;

		node = bucket->binary_search_tree;

		for (;;)
		{
			int comparison_result;

			if (identifier_length < node->identifier_length)
				comparison_result = -1;
			else if (identifier_length > node->identifier_length)
				comparison_result = 1;
			else /*if (identifier_length == node->identifier_length)*/
				comparison_result = state->compare_identifiers(identifier, &node->identifier, identifier_length);

			if (comparison_result < 0)
			{
				if (node->left_child == NULL)
				{
					search_result = SEARCH_RESULT_NOT_FOUND_WOULD_BE_LEFT_CHILD;
					break;
				}

				node = node->left_child;
			}
			else if (comparison_result > 0)
			{
				if (node->right_child == NULL)
				{
					search_result = SEARCH_RESULT_NOT_FOUND_WOULD_BE_RIGHT_CHILD;
					break;
				}

				node = node->right_child;
			}
			else /*if (comparison_result == 0)*/
			{
				search_result = SEARCH_RESULT_FOUND;
				break;
			}
		}

		*node_pointer = node;
	}

	return search_result;
}

static void RemoveNodeFromBucket(Dictionary_Bucket *bucket, Dictionary_Node *node)
{
	Dictionary_Node **binary_search_tree_pointer;

	/* Remove node from binary search tree. */
	if (node->parent == NULL)
		binary_search_tree_pointer = &bucket->binary_search_tree;
	else if (node->parent->left_child == node)
		binary_search_tree_pointer = &node->parent->left_child;
	else /*if (node->parent->right_child == node)*/
		binary_search_tree_pointer = &node->parent->right_child;

	if (node->left_child != NULL && node->right_child != NULL)
	{
		/* Find successor. */
		Dictionary_Node *successor_parent;
		Dictionary_Node *successor;

		successor_parent = node;

		while (successor_parent->left_child->left_child != NULL)
			successor_parent = successor_parent->left_child;

		successor = successor_parent->left_child;

		/* Replace the successor with its own right child. */
		successor_parent->left_child = successor->right_child;
		successor_parent->left_child->parent = successor_parent;

		/* Give the successor the node's children. */
		successor->left_child = node->left_child;
		successor->left_child->parent = successor;

		if (node->right_child != successor)
		{
			successor->right_child = node->right_child;
			successor->right_child->parent = successor;
		}

		/* Replace the node with its successor. */
		*binary_search_tree_pointer = successor;
		successor->parent = node->parent;
	}
	else if (node->left_child != NULL)
	{
		/* Replace the node with its left child. */
		*binary_search_tree_pointer = node->left_child;
		node->left_child->parent = node->parent;
	}
	else if (node->right_child != NULL)
	{
		/* Replace the node with its right child. */
		*binary_search_tree_pointer = node->right_child;
		node->right_child->parent = node->parent;
	}
	else /*if (node->left_child == NULL && node->right_child == NULL)*/
	{
		/* Destroy the node. */
		*binary_search_tree_pointer = NULL;
	}

	/* Remove node from linked-list. */
	if (node->next != NULL)
		node->next->previous = node->previous;

	if (node->previous != NULL)
		node->previous->next = node->next;
	else
		bucket->linked_list = node->next;

	free(node);
}

cc_bool Dictionary_Init(Dictionary_State *state, cc_bool case_insensitive)
{
	cc_bool success = cc_true;

	state->hash_table = malloc(TOTAL_HASH_TABLE_ENTRIES * sizeof(*state->hash_table));

	if (state->hash_table == NULL)
	{
		success = cc_false;
	}
	else
	{
		size_t i;

		for (i = 0; i < TOTAL_HASH_TABLE_ENTRIES; ++i)
		{
			state->hash_table[i].binary_search_tree = NULL;
			state->hash_table[i].linked_list = NULL;
		}

		state->compare_identifiers = case_insensitive ? memcasecmp : memcmp;
	}

	return success;
}

void Dictionary_Deinit(Dictionary_State *state)
{
	Dictionary_Bucket *bucket;

	for (bucket = &state->hash_table[0]; bucket < &state->hash_table[TOTAL_HASH_TABLE_ENTRIES]; ++bucket)
	{
		Dictionary_Node *node;

		node = bucket->linked_list;

		while (node != NULL)
		{
			Dictionary_Node* const next_node = node->next;

			free(node);

			node = next_node;
		}
	}

	free(state->hash_table);
}

cc_bool Dictionary_LookUpAndCreateIfNotExist(Dictionary_State *state, const char *identifier, size_t identifier_length, Dictionary_Entry **entry_pointer)
{
	cc_bool success = cc_true;

	Dictionary_Node *found_node;

	Dictionary_Bucket* const bucket = GetBucket(state, identifier, identifier_length);
	const SearchResult search_result = SearchBucket(state, bucket, identifier, identifier_length, &found_node);

	if (search_result == SEARCH_RESULT_FOUND)
	{
		/* The node was found: return it. */
		if (entry_pointer != NULL)
			*entry_pointer = &found_node->entry;
	}
	else
	{
		/* The node was not found: create it instead. */
		Dictionary_Node *new_node = malloc(sizeof(Dictionary_Node) - 1 + identifier_length);

		if (new_node == NULL)
		{
			success = cc_false;
		}
		else
		{
			/* Initialise binary search tree pointers. */
			new_node->parent = NULL;
			new_node->left_child = NULL;
			new_node->right_child = NULL;

			/* Initialise linked-list pointers. */
			new_node->next = NULL;
			new_node->previous = NULL;

			/* Mark the dictionary entry as unused. */
			new_node->entry.type = -1;

			/* Copy the identifier. */
			new_node->identifier_length = identifier_length;
			memcpy(&new_node->identifier, identifier, identifier_length);

			if (search_result == SEARCH_RESULT_BUCKET_EMPTY)
			{
				/* Add the node to the root of the tree if it is empty. */
				bucket->binary_search_tree = new_node;
			}
			else
			{
				/* Insert node into correct part of binary search tree. */
				new_node->parent = found_node;

				if (search_result == SEARCH_RESULT_NOT_FOUND_WOULD_BE_LEFT_CHILD)
					new_node->parent->left_child = new_node;
				else /*if (search_result == SEARCH_RESULT_NOT_FOUND_WOULD_BE_RIGHT_CHILD)*/
					new_node->parent->right_child = new_node;
			}

			/* Insert node at start of current bucket's linked list. */
			new_node->next = bucket->linked_list;
			bucket->linked_list = new_node;

			if (entry_pointer != NULL)
				*entry_pointer = &new_node->entry;
		}
	}

	return success;
}

Dictionary_Entry* Dictionary_LookUp(Dictionary_State *state, const char *identifier, size_t identifier_length)
{
	Dictionary_Entry *entry;
	Dictionary_Node *node;

	Dictionary_Bucket* const bucket = GetBucket(state, identifier, identifier_length);

	if (SearchBucket(state, bucket, identifier, identifier_length, &node) != SEARCH_RESULT_FOUND)
		entry = NULL;
	else
		entry = &node->entry;

	return entry;
}

cc_bool Dictionary_Remove(Dictionary_State *state, const char *identifier, size_t identifier_length)
{
	cc_bool success = cc_true;

	Dictionary_Node *node;

	Dictionary_Bucket* const bucket = GetBucket(state, identifier, identifier_length);

	if (SearchBucket(state, bucket, identifier, identifier_length, &node) != SEARCH_RESULT_FOUND)
		success = cc_false;
	else
		RemoveNodeFromBucket(bucket, node);

	return success;
}

void Dictionary_Filter(Dictionary_State *state, cc_bool (*filter_function)(Dictionary_Entry *entry, const char *identifier, size_t identifier_length, void *user_data), void *user_data)
{
	Dictionary_Bucket *bucket;

	for (bucket = &state->hash_table[0]; bucket < &state->hash_table[TOTAL_HASH_TABLE_ENTRIES]; ++bucket)
	{
		Dictionary_Node *node;

		node = bucket->linked_list;

		while (node != NULL)
		{
			Dictionary_Node* const next_node = node->next;

			if (!filter_function(&node->entry, &node->identifier, node->identifier_length, user_data))
				RemoveNodeFromBucket(bucket, node);

			node = next_node;
		}
	}
}
