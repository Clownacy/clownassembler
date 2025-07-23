#ifndef STRING_STACK_H
#define STRING_STACK_H

#include "clowncommon/clowncommon.h"

#include "string.h"

typedef struct StringStack_ListEntry
{
	struct StringStack_ListEntry *next;

	String string;
} StringStack_ListEntry;

typedef struct StringStack_State
{
	StringStack_ListEntry *list;
} StringStack_State;

void StringStack_Initialise(StringStack_State *state);
void StringStack_Deinitialise(StringStack_State *state);

cc_bool StringStack_Push(StringStack_State *state, const StringView *view);
cc_bool StringStack_Pop(StringStack_State *state, String *string);

#endif /* STRING_STACK_H */
