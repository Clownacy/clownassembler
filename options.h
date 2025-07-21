#ifndef OPTIONS_H
#define OPTIONS_H

#include "clowncommon/clowncommon.h"

typedef struct Options
{
	cc_bool debug;
	cc_bool case_insensitive;
	cc_bool equ_set_descope_local_labels;
	cc_bool output_local_labels_to_sym_file;
	cc_bool warnings_enabled;
	cc_bool expand_all_macros;
} Options;

typedef struct Options_ListEntry
{
	struct Options_ListEntry *next;
	Options options;
} Options_ListEntry;

typedef struct Options_State
{
	Options_ListEntry *list;
} Options_State;

cc_bool Options_Initialise(Options_State *state, const Options *options);
void Options_Deinitialise(Options_State *state);

cc_bool Options_Push(Options_State *state);
cc_bool Options_Pop(Options_State *state);

#define Options_Get(STATE) (&(STATE)->list->options)

#endif /* OPTIONS_H */
