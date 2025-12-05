/*
 * Copyright (C) 2025 Clownacy
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

#ifndef OPTIONS_H
#define OPTIONS_H

#include "clowncommon/clowncommon.h"

typedef struct Options
{
	char local_signifier;
	cc_bool debug;
	cc_bool case_insensitive;
	cc_bool equ_set_descope_local_labels;
	cc_bool output_local_labels_to_sym_file;
	cc_bool warnings_enabled;
	cc_bool pedantic_warnings_enabled;
	cc_bool expand_all_macros;
	cc_bool automatic_even;
	cc_bool allow_ifeq_directives;
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
