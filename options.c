#include "options.h"

#include <stdlib.h>

static cc_bool Options_PushInternal(Options_State* const state, const Options* const options)
{
	Options_ListEntry* const list_entry = (Options_ListEntry*)malloc(sizeof(Options_ListEntry));

	if (list_entry == NULL)
		return cc_false;

	list_entry->next = state->list;
	state->list = list_entry;

	list_entry->options = *options;

	return cc_true;
}

static void Options_PopInternal(Options_State* const state)
{
	Options_ListEntry* const list_entry = state->list;

	state->list = list_entry->next;

	free(list_entry);
}

cc_bool Options_Initialise(Options_State* const state, const Options* const options)
{
	state->list = NULL;
	return Options_PushInternal(state, options);
}

void Options_Deinitialise(Options_State *state)
{
	while (state->list != NULL)
		Options_PopInternal(state);
}

cc_bool Options_Push(Options_State *state)
{
	return Options_PushInternal(state, Options_Get(state));
}

cc_bool Options_Pop(Options_State *state)
{
	if (state->list->next == NULL)
		return cc_false;

	Options_PopInternal(state);

	return cc_true;
}
