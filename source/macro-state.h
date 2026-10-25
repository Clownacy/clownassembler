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

#ifndef MACRO_STATE_H
#define MACRO_STATE_H

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "macro.h"
#include "substitute.h"
#include "symbol-dictionary.h"

struct MacroState
{
	struct Expansion
	{
		const Macro *metadata;
		Substitute substitutions;
		Substitute::CustomSearch custom_search;
		std::vector<std::string> argument_list;
		std::size_t total_arguments = 0, total_arguments_at_start = 0;

		Expansion(const Macro &metadata)
			: metadata(&metadata)
		{}
	};

	bool ExpansionInProgress() const
	{
		return expansion.has_value();
	}

	std::optional<Expansion> expansion;
	std::shared_ptr<SymbolDictionary> dictionary;
};

#endif // MACRO_STATE_H
