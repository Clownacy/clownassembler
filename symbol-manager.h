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

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include "global.h"
#include "symbol-dictionary.h"
#include "syntactic.h"

class SymbolManager
{
private:
	Global &global;
	const std::shared_ptr<SymbolDictionary> &macro_dictionary;

	SymbolDictionary dictionary = {global.options->case_insensitive};
	std::string last_global_label;

	std::string_view ExpandIdentifier(const std::string_view &identifier, std::string &expanded_identifier);
	SymbolDictionary::Entry& LookupSymbolAndCreateIfNotExistWithoutTypeCheck(std::string_view identifier, SymbolDictionary **found_dictionary);

public:
	bool fixup_needed = false;

	SymbolManager(
		Global &global,
		const std::shared_ptr<SymbolDictionary> &macro_dictionary
	)
		: global(global)
		, macro_dictionary(macro_dictionary)
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

	SymbolDictionary::Entry* LookupSymbol(std::string_view identifier, SymbolDictionary **found_dictionary);

	template<typename SymbolType>
	SymbolDictionary::Entry* LookupSymbolAndCreateIfNotExist(const std::string_view &identifier, SymbolDictionary **found_dictionary)
	{
		SymbolDictionary::Entry &dictionary_entry = LookupSymbolAndCreateIfNotExistWithoutTypeCheck(identifier, found_dictionary);

		if (!std::holds_alternative<SymbolNone>(dictionary_entry) && !std::holds_alternative<SymbolType>(dictionary_entry))
		{
			global.error_manager.Error("Attempted to redefine symbol '{}' as a different type.", identifier);
			return nullptr;
		}

		return &dictionary_entry;
	}

	std::optional<unsigned long> GetSymbolInteger(const std::string_view &identifier, const bool must_evaluate_on_first_pass);
	void PurgeMacro(std::string_view identifier, const bool allow_undefined);

	template<typename... Args>
	void FilterDictionary(Args &&...args)
	{
		return dictionary.Filter(std::forward<Args>(args)...);
	}

	bool AddSymbol(const std::string_view &identifier, SymbolVariable symbol_contents);
	bool AddSymbol(const std::string_view &identifier, SymbolConstant symbol_contents);
	bool AddSymbol(const std::string_view &identifier, SymbolLabel symbol_contents);
	bool AddSymbol(const std::string_view &identifier, SymbolExpressionConstant symbol_contents);
	bool AddSymbol(SymbolMacro symbol_contents);
};

#endif /* SYMBOL_MANAGER_H */
