#include "dictionary.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "clowncommon.h"

static Dictionary_BucketNode** GetBucket(Dictionary_State *state, const char *identifier)
{
	unsigned int hash, character;

	/* Hash the identifier, using djb2. */
	/* http://www.cse.yorku.ca/~oz/hash.html */
	hash = 5381;

	while ((character = (unsigned int)*identifier++) != '\0')
		hash = hash * 33 + character;

	return &state->buckets[hash % CC_COUNT_OF(state->buckets)];
}

static Dictionary_BucketNode** FindEntry(Dictionary_State *state, const char *identifier)
{
	Dictionary_BucketNode **node;

	for (node = GetBucket(state, identifier); *node != NULL; node = &(*node)->next)
		if (strcmp((*node)->identifier, identifier) == 0)
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

cc_bool Dictionary_LookUpAndCreateIfNotExist(Dictionary_State *state, const char *identifier, Dictionary_Entry **entry)
{
	cc_bool success = cc_true;

	Dictionary_BucketNode **node_pointer = FindEntry(state, identifier);

	if (*node_pointer == NULL)
	{
		const size_t identifier_size = strlen(identifier) + 1;
		Dictionary_BucketNode *node = malloc(sizeof(Dictionary_BucketNode) + identifier_size);

		if (node == NULL)
		{
			success = cc_false;
		}
		else
		{
			node->next = NULL;
			*node_pointer = node;

			node->identifier = (char*)(node + 1);
			memcpy(node->identifier, identifier, identifier_size);
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

Dictionary_Entry* Dictionary_LookUp(Dictionary_State *state, const char *identifier)
{
	Dictionary_Entry *entry;

	Dictionary_BucketNode *node_pointer = *FindEntry(state, identifier);

	if (node_pointer == NULL)
		entry = NULL;
	else
		entry = &node_pointer->entry;

	return entry;
}

cc_bool Dictionary_Remove(Dictionary_State *state, const char *identifier)
{
	cc_bool success = cc_true;

	Dictionary_BucketNode **node_pointer = FindEntry(state, identifier);

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
