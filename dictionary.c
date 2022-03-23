#include "dictionary.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "clowncommon.h"

/* TODO - Destructors */

static Dictionary_BucketNode** GetBucket(Dictionary_State *state, const char *identifier, size_t identifier_length)
{
	unsigned int hash;
	size_t i;

	/* Hash the identifier, using djb2. */
	/* http://www.cse.yorku.ca/~oz/hash.html */
	hash = 5381;

	for (i = 0; i < identifier_length; ++i)
		hash = hash * 33 + (unsigned int)*identifier++;

	return &state->buckets[hash % CC_COUNT_OF(state->buckets)];
}

static Dictionary_BucketNode** FindEntry(Dictionary_State *state, const char *identifier, size_t identifier_length)
{
	Dictionary_BucketNode **node;

	for (node = GetBucket(state, identifier, identifier_length); *node != NULL; node = &(*node)->next)
		if ((*node)->identifier_length == identifier_length && memcmp((*node)->identifier, identifier, identifier_length) == 0)
			break;

	return node;
}

void Dictionary_Init(Dictionary_State *state)
{
	size_t i;

	for (i = 0; i < CC_COUNT_OF(state->buckets); ++i)
		state->buckets[i] = NULL;
}

void Dictionary_Deinit(Dictionary_State *state)
{
	size_t i;

	for (i = 0; i < CC_COUNT_OF(state->buckets); ++i)
	{
		Dictionary_BucketNode *node = state->buckets[i];

		while (node != NULL)
		{
			Dictionary_BucketNode *next_node = node->next;

			free(node);

			node = next_node;
		}
	}
}

cc_bool Dictionary_LookUpAndCreateIfNotExist(Dictionary_State *state, const char *identifier, size_t identifier_length, Dictionary_Entry **entry)
{
	cc_bool success = cc_true;

	Dictionary_BucketNode **node_pointer = FindEntry(state, identifier, identifier_length);

	if (*node_pointer == NULL)
	{
		Dictionary_BucketNode *node = malloc(sizeof(Dictionary_BucketNode) + identifier_length);

		if (node == NULL)
		{
			success = cc_false;
		}
		else
		{
			node->next = NULL;
			*node_pointer = node;

			node->identifier = (char*)(node + 1);
			memcpy(node->identifier, identifier, identifier_length);
			node->identifier_length = identifier_length;
			node->entry.type = -1;

			if (entry != NULL)
				*entry = &node->entry;
		}
	}
	else
	{
		if (entry != NULL)
			*entry = &(*node_pointer)->entry;
	}

	return success;
}

Dictionary_Entry* Dictionary_LookUp(Dictionary_State *state, const char *identifier, size_t identifier_length)
{
	Dictionary_Entry *entry;

	Dictionary_BucketNode *node_pointer = *FindEntry(state, identifier, identifier_length);

	if (node_pointer == NULL)
		entry = NULL;
	else
		entry = &node_pointer->entry;

	return entry;
}

cc_bool Dictionary_Remove(Dictionary_State *state, const char *identifier, size_t identifier_length)
{
	cc_bool success = cc_true;

	Dictionary_BucketNode **node_pointer = FindEntry(state, identifier, identifier_length);

	if (*node_pointer == NULL)
	{
		success = cc_false;
	}
	else
	{
		Dictionary_BucketNode *next_node = (*node_pointer)->next;

		free(*node_pointer);

		*node_pointer = next_node;
	}

	return success;
}

void Dictionary_Filter(Dictionary_State *state, cc_bool (*filter_function)(Dictionary_Entry *entry, const char *identifier, size_t identifier_length, void *user_data), void *user_data)
{
	size_t i;

	for (i = 0; i < CC_COUNT_OF(state->buckets); ++i)
	{
		Dictionary_BucketNode **node_pointer = &state->buckets[i];

		while (*node_pointer != NULL)
		{
			Dictionary_BucketNode *node = *node_pointer;

			if (!filter_function(&node->entry, node->identifier, node->identifier_length, user_data))
			{
				*node_pointer = node->next;
				free(node);
			}
			else
			{
				node_pointer = &node->next;
			}
		}
	}
}
