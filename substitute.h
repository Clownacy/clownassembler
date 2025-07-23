#ifndef SUBSTITUTE_H
#define SUBSTITUTE_H

#include "clowncommon/clowncommon.h"

#include "string.h"

typedef struct Substitute_ListEntry
{
	struct Substitute_ListEntry *next;
	String identifier, value;
} Substitute_ListEntry;

typedef struct Substitute_State
{
	Substitute_ListEntry *list_head;
} Substitute_State;

typedef const StringView* (*Substitute_CustomSearch)(void *user_data, const String *string_to_search, size_t starting_position, size_t* const found_position, size_t* const found_length);

void Substitute_Initialise(Substitute_State *state);
void Substitute_Deinitialise(Substitute_State *state);

cc_bool Substitute_PushSubstitute(Substitute_State *state, const StringView *identifier, const StringView *value);
void Substitute_PopSubstitute(Substitute_State *state);

void Substitute_ProcessString(Substitute_State *state, String *string, Substitute_CustomSearch custom_search_callback, const void *custom_search_user_data);

cc_bool Substitute_IsSubstituteBlockingCharacter(const char character);

#endif /* SUBSTITUTE_H */
