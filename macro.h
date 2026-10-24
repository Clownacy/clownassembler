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

#include "parser.h"

using SourceLineList = std::deque<std::string>;

struct Macro
{
	bool is_short, uses_label;
	std::string name;
	IdentifierList parameter_names;
	SourceLineList source_line_list;
};

#endif // MACRO_H
