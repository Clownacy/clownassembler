#include "string-stack.h"

#include <stddef.h>
#include <stdlib.h>

void StringStack_Initialise(StringStack_State* const state)
{
	state->list = NULL;
}

void StringStack_Deinitialise(StringStack_State* const state)
{
	String string;

	while (StringStack_Pop(state, &string))
		String_Destroy(&string);
}

cc_bool StringStack_Push(StringStack_State* const state, const StringView* const view)
{
	StringStack_ListEntry* const list_entry = (StringStack_ListEntry*)malloc(sizeof(StringStack_ListEntry));

	if (list_entry == NULL)
		return cc_false;

	list_entry->next = state->list;
	state->list = list_entry;

	String_CreateCopyView(&list_entry->string, view);

	return cc_true;
}

cc_bool StringStack_Pop(StringStack_State* const state, String* const string)
{
	StringStack_ListEntry* const list_entry = state->list;

	if (list_entry == NULL)
		return cc_false;

	state->list = list_entry->next;

	String_CreateMove(string, &list_entry->string);
	String_Destroy(&list_entry->string);

	free(list_entry);

	return cc_true;
}
