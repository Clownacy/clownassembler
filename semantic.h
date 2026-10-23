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

#include <functional>
#include <iostream>
#include <string_view>

#include "options.h"

namespace ClownAssembler
{
	using Settings = Options;

	using AddDefinition = std::function<void(const std::string_view &identifier, unsigned long value)>;
	using DefinitionCallback = std::function<void(const AddDefinition &add_definition)>;

	bool Assemble(
		std::istream &input_callbacks,
		std::ostream &output_callbacks,
		std::ostream &error_callbacks,
		std::ostream *listing_callbacks,
		std::ostream *symbol_callbacks,
		const std::string_view &input_file_path,
		Settings settings,
		const DefinitionCallback &definition_callback);
}

#endif /* SEMANTIC_H */
