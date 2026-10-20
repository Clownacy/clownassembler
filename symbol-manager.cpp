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

#include "symbol-manager.h"

#include <format>
#include <string_view>

#include "clowncommon/clowncommon.h"

#include "error.h"
#include "expression-processor.h"
#include "options.h"
#include "symbol-dictionary.h"

void SymbolManager::SetLastGlobalLabel(const std::string_view &label)
{
	if (label.front() != options->local_signifier)
	{
		/* This is a global label - cache it for later. */
		last_global_label = label;
	}
}

std::string_view SymbolManager::ExpandIdentifier(const std::string_view &identifier, std::string &expanded_identifier)
{
	if (identifier.empty() || (identifier.front() != options->local_signifier))
		return identifier;

	if (last_global_label.empty())
		return identifier.substr(1);

	expanded_identifier = std::format("{}{}", last_global_label, identifier);

	return expanded_identifier;
}

SymbolDictionary::Entry* SymbolManager::LookupSymbol(std::string_view identifier, SymbolDictionary **found_dictionary)
{
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

		dictionary_entry = dictionary.LookUp(identifier);

		if (dictionary_entry == nullptr)
			if (found_dictionary != nullptr)
				*found_dictionary = nullptr;
	}

	return dictionary_entry;
}

std::optional<unsigned long> SymbolManager::GetSymbolInteger(const std::string_view &identifier, const bool must_evaluate_on_first_pass)
{
	SymbolDictionary::Entry* const dictionary_entry = LookupSymbol(identifier, nullptr);

	if (dictionary_entry == nullptr || std::holds_alternative<SymbolNone>(*dictionary_entry))
	{
		if (must_evaluate_on_first_pass)
			error_manager.Error("Symbol '{}' must be evaluable on first pass.", identifier);
		else if (doing_fixup_pass)
			error_manager.Error("Symbol '{}' does not exist.", identifier);
		else
			fixup_needed = true;

		return std::nullopt;
	}

	return std::visit(
		CC_VisitHelper{
			[&](SymbolVariable &symbol) -> std::optional<unsigned long>
			{
				if (doing_fixup_pass)
					error_manager.Warning("Symbol '{}' forward-referenced despite being a redefineable variable.", identifier);

				return symbol.value;
			},
			[&](SymbolLabel &symbol) -> std::optional<unsigned long>
			{
				return symbol.value;
			},
			[&](SymbolConstant &symbol) -> std::optional<unsigned long>
			{
				return symbol.value;
			},
			[&](SymbolExpressionConstant &symbol) -> std::optional<unsigned long>
			{
				return expression_processor.ResolveExpression(symbol.expression);
			},
			[&](auto&) -> std::optional<unsigned long>
			{
				error_manager.Error("Symbol '{}' is not a label, constant, or variable.", identifier);
				return std::nullopt;
			}
		}, *dictionary_entry
	);
}

void SymbolManager::PurgeMacro(std::string_view identifier, const bool allow_undefined)
{
	SymbolDictionary *dictionary;
	SymbolDictionary::Entry* const entry = LookupSymbol(identifier, &dictionary);

	if (entry == nullptr)
	{
		if (!allow_undefined)
			error_manager.Error("Cannot purge '{}' as it does not exist.", identifier);

		return;
	}

	if (!std::holds_alternative<SymbolMacro>(*entry))
	{
		error_manager.Error("Cannot purge '{}' as it is not a macro.", identifier);
		return;
	}

	/* TODO: Avoid this allocation entirely by hashing each half of the identifier separately. */
	std::string expanded_identifier;
	identifier = ExpandIdentifier(identifier, expanded_identifier);

	dictionary->Remove(identifier);
}
