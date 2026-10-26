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

#ifndef SYMBOL_MANAGER_H
#define SYMBOL_MANAGER_H

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <variant>

#include "global.h"
#include "parser.h"
#include "symbol-dictionary.h"

struct MacroStateStack;

class SymbolManager
{
public:
	using LookupResult = std::pair<SymbolDictionary::iterator, SymbolDictionary&>;

private:
	Global &global;
	MacroStateStack &macro;

	SymbolDictionary dictionary = {global.options->case_insensitive};
	std::string last_global_label;

	DictionaryLookupKey ExpandIdentifier(const std::string_view &identifier);
	LookupResult LookupSymbolWithExpandedIndentifier(const DictionaryLookupKey &expanded_identifier);
	LookupResult LookupSymbolAndCreateIfNotExistWithoutTypeCheck(const std::string_view &identifier);

public:
	bool fixup_needed = false;

	SymbolManager(
		Global &global,
		MacroStateStack &macro
	)
		: global(global)
		, macro(macro)
	{}

	const auto& GetLastGlobalLabel() const
	{
		return last_global_label;
	}

	void SetLastGlobalLabel(const std::string_view &label);

	// TODO: Get rid of this dumb hack.
	void SetLastGlobalLabelDirect(std::string label)
	{
		last_global_label = std::move(label);
	}

	LookupResult LookupSymbol(const std::string_view &identifier);

	template<typename SymbolType>
	LookupResult LookupSymbolAndCreateIfNotExist(const std::string_view &identifier)
	{
		const auto &[it, dictionary] = LookupSymbolAndCreateIfNotExistWithoutTypeCheck(identifier);

		if (!std::holds_alternative<SymbolNone>(it->second) && !std::holds_alternative<SymbolType>(it->second))
		{
			global.error_manager.Error("Attempted to redefine symbol '{}' as a different type.", identifier);
			return {dictionary.end(), dictionary};
		}

		return {it, dictionary};
	}

	std::optional<unsigned long> GetSymbolInteger(const std::string_view &identifier, const bool must_evaluate_on_first_pass);
	bool DestroySymbol(const std::string_view &identifier);

	void FilterDictionary(const auto &filter_function)
	{
		std::erase_if(dictionary,
			[&](auto &entry)
			{
				return !filter_function(entry.second, entry.first);
			}
		);
	}

	bool AddSymbol(const std::string_view &identifier, SymbolVariable symbol_contents);
	bool AddSymbol(const std::string_view &identifier, SymbolConstant symbol_contents);
	bool AddSymbol(const std::string_view &identifier, SymbolLabel symbol_contents);
	bool AddSymbol(const std::string_view &identifier, SymbolExpressionConstant symbol_contents);
	bool AddSymbol(const std::string_view &identifier, SymbolMacro symbol_contents);

	void WriteSymbolFile(std::ostream &stream);
};

#endif /* SYMBOL_MANAGER_H */
