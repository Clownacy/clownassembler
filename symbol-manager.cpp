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

#include "symbol-dictionary.h"

void SymbolManager::SetLastGlobalLabel(const std::string_view &label)
{
	if (label.front() != global.options->local_signifier)
	{
		/* This is a global label - cache it for later. */
		last_global_label = label;
	}
}

std::string_view SymbolManager::ExpandIdentifier(const std::string_view &identifier, std::string &expanded_identifier)
{
	if (identifier.empty() || (identifier.front() != global.options->local_signifier))
		return identifier;

	if (last_global_label.empty())
		return identifier.substr(1);

	expanded_identifier = std::format("{}{}", last_global_label, identifier);

	return expanded_identifier;
}

SymbolManager::LookupResult SymbolManager::LookupSymbolWithExpandedIndentifier(const std::string_view &expanded_identifier)
{
	if (macro_dictionary != nullptr)
	{
		const auto &it = macro_dictionary->find(expanded_identifier);

		if (it != macro_dictionary->end())
			return {it, *macro_dictionary};
	}

	const auto &it = dictionary.find(expanded_identifier);
	return {it, dictionary};
}

SymbolDictionary::Entry* SymbolManager::LookupSymbol(std::string_view identifier, SymbolDictionary** const found_dictionary)
{
	/* TODO: Avoid this allocation entirely by hashing each half of the identifier separately. */
	std::string expanded_identifier;
	identifier = ExpandIdentifier(identifier, expanded_identifier);

	const auto &[it, found_dictionary_2_electric_boogaloo] = LookupSymbolWithExpandedIndentifier(identifier);

	if (it != found_dictionary_2_electric_boogaloo.end())
	{
		if (found_dictionary != nullptr)
			*found_dictionary = &found_dictionary_2_electric_boogaloo;

		return &it->second;
	}

	if (found_dictionary != nullptr)
		*found_dictionary = nullptr;

	return nullptr;
}

SymbolDictionary::Entry& SymbolManager::LookupSymbolAndCreateIfNotExistWithoutTypeCheck(std::string_view identifier, SymbolDictionary** const found_dictionary)
{
	/* TODO: Avoid this allocation entirely by hashing each half of the identifier separately. */
	std::string expanded_identifier;
	identifier = ExpandIdentifier(identifier, expanded_identifier);

	const auto &[it, found_dictionary_2_electric_boogaloo] = LookupSymbolWithExpandedIndentifier(identifier);

	if (it != found_dictionary_2_electric_boogaloo.end())
	{
		if (found_dictionary != nullptr)
			*found_dictionary = &found_dictionary_2_electric_boogaloo;

		return it->second;
	}

	if (found_dictionary != nullptr)
		*found_dictionary = &dictionary;

	return dictionary.emplace(identifier).first->second;
}

std::optional<unsigned long> SymbolManager::GetSymbolInteger(const std::string_view &identifier, const bool must_evaluate_on_first_pass)
{
	SymbolDictionary::Entry* const dictionary_entry = LookupSymbol(identifier, nullptr);

	if (dictionary_entry == nullptr || std::holds_alternative<SymbolNone>(*dictionary_entry))
	{
		if (must_evaluate_on_first_pass)
			global.error_manager.Error("Symbol '{}' must be evaluable on first pass.", identifier);
		else if (global.doing_fixup_pass)
			global.error_manager.Error("Symbol '{}' does not exist.", identifier);
		else
			fixup_needed = true;

		return std::nullopt;
	}

	return std::visit(
		CC_VisitHelper{
			[&](SymbolVariable &symbol) -> std::optional<unsigned long>
			{
				if (global.doing_fixup_pass)
					global.error_manager.Warning("Symbol '{}' forward-referenced despite being a redefineable variable.", identifier);

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
				return global.expression_processor.ResolveExpression(symbol.expression);
			},
			[&](auto&) -> std::optional<unsigned long>
			{
				global.error_manager.Error("Symbol '{}' is not a label, constant, or variable.", identifier);
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
			global.error_manager.Error("Cannot purge '{}' as it does not exist.", identifier);

		return;
	}

	if (!std::holds_alternative<SymbolMacro>(*entry))
	{
		global.error_manager.Error("Cannot purge '{}' as it is not a macro.", identifier);
		return;
	}

	/* TODO: Avoid this allocation entirely by hashing each half of the identifier separately. */
	std::string expanded_identifier;
	identifier = ExpandIdentifier(identifier, expanded_identifier);

	dictionary->Remove(identifier);
}

bool SymbolManager::AddSymbol(const std::string_view &identifier, SymbolVariable symbol_contents)
{
	SymbolDictionary::Entry *symbol = LookupSymbolAndCreateIfNotExist<SymbolVariable>(identifier, nullptr);

	if (symbol == nullptr)
		return false;

	symbol->emplace<SymbolVariable>(std::move(symbol_contents));

	return true;
}

bool SymbolManager::AddSymbol(const std::string_view &identifier, SymbolConstant symbol_contents)
{
	SymbolDictionary::Entry *symbol = LookupSymbolAndCreateIfNotExist<SymbolConstant>(identifier, nullptr);

	if (symbol == nullptr)
		return false;

	const auto constant_symbol = std::get_if<SymbolConstant>(symbol);

	if (constant_symbol != nullptr && constant_symbol->value != symbol_contents.value)
	{
		global.error_manager.Error("Constant '{}' cannot be redefined to a different value.", identifier);
		return false;
	}

	symbol->emplace<SymbolConstant>(std::move(symbol_contents));

	return true;
}

bool SymbolManager::AddSymbol(const std::string_view &identifier, SymbolLabel symbol_contents)
{
	SymbolDictionary::Entry *symbol = LookupSymbolAndCreateIfNotExist<SymbolLabel>(identifier, nullptr);

	if (symbol == nullptr)
		return false;

	if (std::holds_alternative<SymbolLabel>(*symbol))
	{
		global.error_manager.Error("Label '{}' cannot be redefined.", identifier);
		return false;
	}

	symbol->emplace<SymbolLabel>(std::move(symbol_contents));

	return true;
}

bool SymbolManager::AddSymbol(const std::string_view &identifier, SymbolExpressionConstant symbol_contents)
{
	SymbolDictionary::Entry *symbol = LookupSymbolAndCreateIfNotExist<SymbolExpressionConstant>(identifier, nullptr);

	if (symbol == nullptr)
		return false;

	symbol->emplace<SymbolExpressionConstant>(std::move(symbol_contents));

	return true;
}

bool SymbolManager::AddSymbol(SymbolMacro symbol_contents)
{
	const std::string_view &identifier = symbol_contents.macro.name;

	SymbolDictionary::Entry *symbol = LookupSymbolAndCreateIfNotExist<SymbolMacro>(identifier, nullptr);

	if (symbol == nullptr)
		return false;

	if (std::holds_alternative<SymbolMacro>(*symbol))
	{
		global.error_manager.Error("Macro '{}' cannot be redefined.", identifier);
		return false;
	}

	symbol->emplace<SymbolMacro>(std::move(symbol_contents));

	return true;
}
