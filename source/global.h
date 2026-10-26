/*
 * Copyright (C) 2026 Clownacy
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

#ifndef GLOBAL_H
#define GLOBAL_H

#include <deque>
#include <iostream>
#include <string_view>

#include "error.h"
#include "expression-processor.h"
#include "options.h"
#include "program-counter.h"

struct MacroState;
class SymbolManager;

struct Global
{
	OptionsStack options;
	ErrorManager error_manager;
	ExpressionProcessor expression_processor;
	ProgramCounter program_counter;
	bool doing_fixup_pass = false;

	Global(
		Options default_options,
		std::ostream &error_stream,
		const std::deque<ErrorManager::Location> &locations,
		const std::string_view &source_line,
		SymbolManager &symbol_manager,
		const MacroState &macro
	)
		: options(std::move(default_options))
		, error_manager(error_stream, locations, source_line, options)
		, expression_processor(error_manager, symbol_manager, options, macro)
	{}
};

#endif /* GLOBAL_H */
