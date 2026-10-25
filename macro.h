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

#ifndef MACRO_H
#define MACRO_H

#include <deque>
#include <string>

#include "error.h"
#include "parser.h"

using SourceLineList = std::deque<std::string>;

struct Macro
{
	const bool is_short, uses_label;
	const ErrorManager::Location location;
	const IdentifierList parameter_names;
	SourceLineList source_line_list;

	Macro(
		bool is_short,
		bool uses_label,
		ErrorManager::Location location,
		IdentifierList parameter_names
	)
		: is_short(is_short)
		, uses_label(uses_label)
		, location(std::move(location))
		, parameter_names(std::move(parameter_names))
	{}
};

#endif // MACRO_H
