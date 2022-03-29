#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stddef.h>

#include "clowncommon.h"

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

typedef struct Dictionary_BucketNode
{
	struct Dictionary_BucketNode *next;

	char *identifier;
	size_t identifier_length;
	Dictionary_Entry entry;
} Dictionary_BucketNode;

typedef struct Dictionary_State
{
	Dictionary_BucketNode *buckets[0x100];
	int (*compare_identifiers)(const void *lhs, const void *rhs, size_t count);
} Dictionary_State;

void Dictionary_Init(Dictionary_State *state, cc_bool case_insensitive);
void Dictionary_Deinit(Dictionary_State *state);

cc_bool Dictionary_LookUpAndCreateIfNotExist(Dictionary_State *state, const char *identifier, size_t identifier_length, Dictionary_Entry **entry);
Dictionary_Entry* Dictionary_LookUp(Dictionary_State *state, const char *identifier, size_t identifier_length);
cc_bool Dictionary_Remove(Dictionary_State *state, const char *identifier, size_t identifier_length);
void Dictionary_Filter(Dictionary_State *state, cc_bool (*filter_function)(Dictionary_Entry *entry, const char *identifier, size_t identifier_length, void *user_data), void *user_data);

#endif /* DICTIONARY_H */
