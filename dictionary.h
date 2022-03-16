#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "clowncommon.h"

typedef struct Dictionary_Entry
{
	int type;
	union
	{
		unsigned long unsigned_integer;
		long signed_integer;
		void *pointer;
	} data; 
} Dictionary_Entry;

typedef struct Dictionary_BucketNode
{
	struct Dictionary_BucketNode *next;

	char *identifier;
	Dictionary_Entry entry;
} Dictionary_BucketNode;

typedef struct Dictionary_State
{
	Dictionary_BucketNode *buckets[0x100];
} Dictionary_State;

void Dictionary_Init(Dictionary_State *state);
void Dictionary_Deinit(Dictionary_State *state);

cc_bool Dictionary_LookUpAndCreateIfNotExist(Dictionary_State *state, const char *identifier, Dictionary_Entry **entry);
Dictionary_Entry* Dictionary_LookUp(Dictionary_State *state, const char *identifier);
cc_bool Dictionary_Remove(Dictionary_State *state, const char *identifier);

#endif /* DICTIONARY_H */
