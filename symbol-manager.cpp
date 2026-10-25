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

DictionaryLookupKey SymbolManager::ExpandIdentifier(const std::string_view &identifier)
{
	if (identifier.empty() || (identifier.front() != global.options->local_signifier))
		return {identifier};

	return {identifier, last_global_label};
}

SymbolManager::LookupResult SymbolManager::LookupSymbolWithExpandedIndentifier(const DictionaryLookupKey &expanded_identifier)
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

SymbolManager::LookupResult SymbolManager::LookupSymbol(const std::string_view &identifier)
{
	const auto &expanded_identifier = ExpandIdentifier(identifier);

	return LookupSymbolWithExpandedIndentifier(expanded_identifier);
}

SymbolManager::LookupResult SymbolManager::LookupSymbolAndCreateIfNotExistWithoutTypeCheck(const std::string_view &identifier)
{
	const auto &expanded_identifier = ExpandIdentifier(identifier);

	const auto &[it, found_dictionary] = LookupSymbolWithExpandedIndentifier(expanded_identifier);

	if (it != found_dictionary.end())
		return {it, found_dictionary};

	return {dictionary.emplace(expanded_identifier).first, dictionary};
}

std::optional<unsigned long> SymbolManager::GetSymbolInteger(const std::string_view &identifier, const bool must_evaluate_on_first_pass)
{
	const auto &[it, dictionary] = LookupSymbol(identifier);

	if (it == dictionary.end() || std::holds_alternative<SymbolNone>(it->second))
	{
		if (must_evaluate_on_first_pass)
			global.error_manager.Error("Symbol '{}' must be evaluable on first pass.", identifier);
		else if (global.doing_fixup_pass)
			global.error_manager.Error("Symbol '{}' does not exist.", identifier);
		else
			fixup_needed = true;

		return std::nullopt;
	}

	return CC_Visit(
		it->second,
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
	);
}

bool SymbolManager::DestroySymbol(const std::string_view &identifier)
{
	const auto &[it, dictionary] = LookupSymbol(identifier);

	if (it == dictionary.end())
		return false;

	dictionary.erase(it);
	return true;
}

bool SymbolManager::AddSymbol(const std::string_view &identifier, SymbolVariable symbol_contents)
{
	const auto &[it, dictionary] = LookupSymbolAndCreateIfNotExist<SymbolVariable>(identifier);

	if (it == dictionary.end())
		return false;

	it->second.emplace<SymbolVariable>(std::move(symbol_contents));

	return true;
}

bool SymbolManager::AddSymbol(const std::string_view &identifier, SymbolConstant symbol_contents)
{
	const auto &[it, dictionary] = LookupSymbolAndCreateIfNotExist<SymbolConstant>(identifier);

	if (it == dictionary.end())
		return false;

	const auto constant_symbol = std::get_if<SymbolConstant>(&it->second);

	if (constant_symbol != nullptr && constant_symbol->value != symbol_contents.value)
	{
		global.error_manager.Error("Constant '{}' cannot be redefined to a different value.", identifier);
		return false;
	}

	it->second.emplace<SymbolConstant>(std::move(symbol_contents));

	return true;
}

bool SymbolManager::AddSymbol(const std::string_view &identifier, SymbolLabel symbol_contents)
{
	const auto &[it, dictionary] = LookupSymbolAndCreateIfNotExist<SymbolLabel>(identifier);

	if (it == dictionary.end())
		return false;

	if (std::holds_alternative<SymbolLabel>(it->second))
	{
		global.error_manager.Error("Label '{}' cannot be redefined.", identifier);
		return false;
	}

	it->second.emplace<SymbolLabel>(std::move(symbol_contents));

	return true;
}

bool SymbolManager::AddSymbol(const std::string_view &identifier, SymbolExpressionConstant symbol_contents)
{
	const auto &[it, dictionary] = LookupSymbolAndCreateIfNotExist<SymbolExpressionConstant>(identifier);

	if (it == dictionary.end())
		return false;

	it->second.emplace<SymbolExpressionConstant>(std::move(symbol_contents));

	return true;
}

bool SymbolManager::AddSymbol(const std::string_view &identifier, SymbolMacro symbol_contents)
{
	const auto &[it, dictionary] = LookupSymbolAndCreateIfNotExist<SymbolMacro>(identifier);

	if (it == dictionary.end())
		return false;

	it->second.emplace<SymbolMacro>(std::move(symbol_contents));

	return true;
}

void SymbolManager::WriteSymbolFile(std::ostream &stream)
{
	Utilities::IO::StreamBinaryWriter symbol_writer(stream);

	/* Some kind of header. */
	symbol_writer.WriteU8('M');
	symbol_writer.WriteU8('N');
	symbol_writer.WriteU8('D');
	symbol_writer.WriteU8(1);
	symbol_writer.WriteU8(0);
	symbol_writer.WriteU8(0);
	symbol_writer.WriteU8(0);
	symbol_writer.WriteU8(0);

	for (const auto &[identifier, entry] : dictionary)
	{
		/* The symbol file only contains labels. */
		/* Labels longer than 0xFF characters are silently ignored. */
		/* Local labels are also silently ignored (unless 'v+' is specified). */
		const SymbolLabel* const symbol_label = std::get_if<SymbolLabel>(&entry);

		if (symbol_label == nullptr)
			continue;

		const auto local_symbol_position = identifier.find_last_of(global.options->local_signifier);
		const std::string_view label = identifier.substr(local_symbol_position == identifier.npos ? 0 : local_symbol_position);

		const bool is_local_label = label.length() != identifier.length();

		if (label.length() >= 0x100 || (is_local_label && !global.options->output_local_labels_to_sym_file))
			continue;

		/* Output the address of the label. */
		symbol_writer.WriteU8(symbol_label->value);

		/* I have no idea what this means. */
		symbol_writer.WriteU8(is_local_label ? 6 : 2);

		/* Output the length of the label. */
		symbol_writer.WriteU8(label.length());

		/* Output the label itself. */
		stream << label;
	}
}
