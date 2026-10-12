/*
 * Copyright (C) 2022-2025 Clownacy
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

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <iostream>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#include "clowncommon/clowncommon.h"

#include "io.h"
#include "options.h"

#define CLOWNASSEMBLER_VERSION_STRING "ClownAssembler v2.0"

using ClownAssembler_Settings = Options;

using ClownAssembler_AddDefinition = void (*)(void *internal, const char *identifier, std::size_t identifier_length, unsigned long value);
using ClownAssembler_DefinitionCallback = void (*)(void *internal, void *user_data, ClownAssembler_AddDefinition add_definition);

cc_bool ClownAssembler_AssembleFile(
	std::istream &input_callbacks,
	std::ostream &output_callbacks,
	std::ostream &error_callbacks,
	std::ostream &listing_callbacks,
	std::ostream *symbol_callbacks,
	const char *input_file_path,
	ClownAssembler_Settings settings,
	ClownAssembler_DefinitionCallback definition_callback,
	const void *user_data);

cc_bool ClownAssembler_Assemble(
	std::istream &input_callbacks,
	std::ostream &output_callbacks,
	std::ostream &error_callbacks,
	std::ostream *listing_callbacks,
	std::ostream *symbol_callbacks,
	const char *input_file_path,
	ClownAssembler_Settings settings,
	ClownAssembler_DefinitionCallback definition_callback,
	const void *user_data);

#endif /* SEMANTIC_H */
