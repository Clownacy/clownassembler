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

#include "error.h"
#include "options.h"
#include "symbol-dictionary.h"
#include "syntactic.h"

class ExpressionProcessor;

class SymbolManager
{
private:
	ErrorManager &error_manager;
	ExpressionProcessor &expression_processor;
	OptionsStack &options;
	const std::shared_ptr<SymbolDictionary> &macro_dictionary;
	bool &doing_fixup_pass;

	SymbolDictionary dictionary = {options->case_insensitive};
	std::string last_global_label;

	std::string_view ExpandIdentifier(const std::string_view &identifier, std::string &expanded_identifier);

public:
	bool fixup_needed = false;

	SymbolManager(
		ErrorManager &error_manager,
		ExpressionProcessor &expression_processor,
		OptionsStack &options,
		const std::shared_ptr<SymbolDictionary> &macro_dictionary,
		bool &doing_fixup_pass
	)
		: error_manager(error_manager)
		, expression_processor(expression_processor)
		, options(options)
		, macro_dictionary(macro_dictionary)
		, doing_fixup_pass(doing_fixup_pass)
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
	SymbolDictionary::Entry* LookupSymbolAndCreateIfNotExist(std::string_view identifier, SymbolDictionary **found_dictionary)
	{
		/* TODO: Deduplicate this with `LookupSymbol`. */
		SymbolDictionary::Entry *dictionary_entry = nullptr;

		/* TODO: Avoid this allocation entirely by hashing each half of the identifier separately. */
		std::string expanded_identifier;
		identifier = ExpandIdentifier(identifier, expanded_identifier);

		if (macro_dictionary != nullptr)
		{
			if (found_dictionary != nullptr)
				*found_dictionary = &*macro_dictionary;

			dictionary_entry = macro_dictionary->LookUp(identifier);
		}

		if (dictionary_entry == nullptr)
		{
			if (found_dictionary != nullptr)
				*found_dictionary = &dictionary;

			dictionary_entry = &dictionary.LookUpAndCreateIfNotExist(identifier);
		}

		if (!std::holds_alternative<SymbolNone>(*dictionary_entry) && !std::holds_alternative<SymbolType>(*dictionary_entry))
		{
			error_manager.Error("Attempted to redefine symbol '{}' as a different type.", identifier);
			return nullptr;
		}

		return dictionary_entry;
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
