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

/* TODO: Optimise EQUS by having the substitution logic search for the '\' rather than the string itself. */

#include "semantic.h"

#include <array>
#include <deque>
#include <filesystem>
#include <fstream>
#include <limits.h>
#include <memory>
#include <sstream>
#include <stack>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "clowncommon/clowncommon.h"

#include "dictionary.h"
#include "error.h"
#include "expression-processor.h"
#include "instruction-processor.h"
#include "listing.h"
#include "macro-state.h"
#include "object.h"
#include "object-writer.h"
#include "p2bin.h"
#include "parser.h"
#include "program-counter.h"
#include "source-line-reader.h"
#include "substitute.h"
#include "symbol-manager.h"
#include "utilities/io.h"
#include "utilities/path.h"
#include "utilities/string.h"

struct FixUp
{
	Statement statement;
	std::shared_ptr<SymbolDictionary> macro_dictionary;
	unsigned long program_counter;
	Object::Writer::Position object_writer_position;
	std::string last_global_label;
	std::string source_line;
	std::string label;
	Options options;
	std::deque<ErrorManager::Location> location;
};

struct SemanticState
{
	/* Global stuff. */
	OptionsStack options;
	ErrorManager error_manager;
	SourceLineReader source_line_reader;
	ListingWriter listing_writer;
	ObjectWriter object_writer;
	ProgramCounter program_counter;
	InstructionProcessor instruction_processor = {object_writer, error_manager, expression_processor, program_counter, options};
	SymbolManager symbol_manager;
	ExpressionProcessor expression_processor;
	Parser parser;
	std::deque<ErrorManager::Location> location;
	std::string_view source_line;
	Substitute substitutions;
	std::stack<std::string> string_stack;
	MacroState macro;
	unsigned int macro_depth = 0;
	unsigned long current_macro_invocation = 0;
	bool end = false;

	/* Fix-ups. */
	std::deque<FixUp> fixup_list;
	bool doing_fixup_pass = false;

	/* If-statements. */
	unsigned int current_if_level = 0;
	unsigned int false_if_level = 0;
	bool true_already_found = false;

	/* OBJ directive. */
	std::optional<unsigned long> obj_delta;

	/* Modes. */
	struct ModeNormal {};
	struct ModeRept
	{
		unsigned long repetitions = 0;
		unsigned long line_number = 0;
		unsigned int nesting = 0;
		SourceLineList source_line_list;
	};
	struct ModeMacro
	{
		unsigned long line_number = 0;
		Macro metadata;
	};
	struct ModeWhile
	{
		Expression expression;
		std::string source_line;
		unsigned long line_number = 0;
		unsigned int nesting = 0;
		SourceLineList source_line_list;
	};

	std::variant<ModeNormal, ModeRept, ModeMacro, ModeWhile> mode = SemanticState::ModeNormal{};

	SemanticState(
		std::istream &input_stream,
		std::ostream &output_stream,
		std::ostream &error_stream,
		std::ostream* const listing_stream,
		const std::string_view &input_file_path,
		Options default_options
	)
		: options(std::move(default_options))
		, error_manager(error_stream, location, source_line, options->warnings_enabled, options->pedantic_warnings_enabled)
		, source_line_reader(input_stream)
		, listing_writer(listing_stream, doing_fixup_pass)
		, object_writer(output_stream, listing_writer, program_counter, doing_fixup_pass)
		, symbol_manager(error_manager, expression_processor, options, macro, doing_fixup_pass)
		, expression_processor(error_manager, symbol_manager, options)
		, parser(error_manager)
	{
		/* For error messages, we want to maintain a list of filenames and line
		   numbers that describe the location and nesting of the erroneous
		   line. This is the first entry in that list. */
		location.emplace_front(input_file_path, 0);
	}
};

/* Some forward declarations that are needed because some functions recurse into each other. */
static void AssembleFile(SemanticState &state, SourceLineReader &source_line_reader);
static void AssembleLine(SemanticState &state, const std::string_view &source_line, const bool write_line_to_listing_file);

/* Other stuff */

static void TerminateIf(SemanticState &state)
{
	if (state.false_if_level == state.current_if_level || state.false_if_level == 0)
	{
		state.true_already_found = true;
		state.false_if_level = 0;
	}

	--state.current_if_level;
}

static void TerminateRept(SemanticState &state, SemanticState::ModeRept &rept_state)
{
	/* Back-up some state into local variables, in case a nested REPT statement clobbers it. */
	auto rept_state_copy = std::move(rept_state);

	/* Exit REPT mode. */
	state.mode.emplace<SemanticState::ModeNormal>();

	while (rept_state_copy.repetitions-- != 0)
	{
		/* Rewind back to the line number of the start of the REPT. */
		state.location.front().line_number = rept_state_copy.line_number;

		/* Process the REPT's nested statements. */
		for (const auto &source_line : rept_state_copy.source_line_list)
			AssembleLine(state, source_line, state.options->expand_all_macros);
	}

	/* Increment past the ENDR line number. */
	++state.location.front().line_number;
}

static bool AddToSymbolTable(SemanticState &state, const std::string_view &identifier, SymbolVariable symbol_contents)
{
	SymbolDictionary::Entry *symbol = state.symbol_manager.LookupSymbolAndCreateIfNotExist<SymbolVariable>(identifier, nullptr);

	if (symbol == nullptr)
		return false;

	symbol->emplace<SymbolVariable>(std::move(symbol_contents));

	return true;
}

static bool AddToSymbolTable(SemanticState &state, const std::string_view &identifier, SymbolConstant symbol_contents)
{
	SymbolDictionary::Entry *symbol = state.symbol_manager.LookupSymbolAndCreateIfNotExist<SymbolConstant>(identifier, nullptr);

	if (symbol == nullptr)
		return false;

	const auto constant_symbol = std::get_if<SymbolConstant>(symbol);

	if (constant_symbol != nullptr && constant_symbol->value != symbol_contents.value)
	{
		state.error_manager.Error("Constant '{}' cannot be redefined to a different value.", identifier);
		return false;
	}

	symbol->emplace<SymbolConstant>(std::move(symbol_contents));

	return true;
}

static bool AddToSymbolTable(SemanticState &state, const std::string_view &identifier, SymbolLabel symbol_contents)
{
	SymbolDictionary::Entry *symbol = state.symbol_manager.LookupSymbolAndCreateIfNotExist<SymbolLabel>(identifier, nullptr);

	if (symbol == nullptr)
		return false;

	if (std::holds_alternative<SymbolLabel>(*symbol))
	{
		state.error_manager.Error("Label '{}' cannot be redefined.", identifier);
		return false;
	}

	symbol->emplace<SymbolLabel>(std::move(symbol_contents));

	return true;
}

static bool AddToSymbolTable(SemanticState &state, const std::string_view &identifier, SymbolExpressionConstant symbol_contents)
{
	SymbolDictionary::Entry *symbol = state.symbol_manager.LookupSymbolAndCreateIfNotExist<SymbolExpressionConstant>(identifier, nullptr);

	if (symbol == nullptr)
		return false;

	symbol->emplace<SymbolExpressionConstant>(std::move(symbol_contents));

	return true;
}

static bool AddToSymbolTable(SemanticState &state, SymbolMacro symbol_contents)
{
	const std::string_view &identifier = symbol_contents.macro.name;

	SymbolDictionary::Entry *symbol = state.symbol_manager.LookupSymbolAndCreateIfNotExist<SymbolMacro>(identifier, nullptr);

	if (symbol == nullptr)
		return false;

	if (std::holds_alternative<SymbolMacro>(*symbol))
	{
		state.error_manager.Error("Macro '{}' cannot be redefined.", identifier);
		return false;
	}

	symbol->emplace<SymbolMacro>(std::move(symbol_contents));

	return true;
}

static bool AddToSymbolTable(SemanticState &state, const std::string_view &identifier, SymbolSpecial symbol_contents)
{
	SymbolDictionary::Entry *symbol = state.symbol_manager.LookupSymbolAndCreateIfNotExist<SymbolSpecial>(identifier, nullptr);

	if (symbol == nullptr)
		return false;

	symbol->emplace<SymbolSpecial>(std::move(symbol_contents));

	return true;
}

static void TerminateMacro(SemanticState &state, SemanticState::ModeMacro &macro_state)
{
	const std::string_view &identifier = macro_state.metadata.name;

	state.symbol_manager.PurgeMacro(identifier, true);

	if (!identifier.empty())
	{
		if (state.macro.ExpansionInProgress())
		{
			/* See comment in PurgeMacro after the check for the same condition for more details. */
			state.error_manager.Error("Cannot define macro '{}' during macro expansion.", identifier);
		}
		else
		{
			/* Add the macro to the symbol table. */
			AddToSymbolTable(state, SymbolMacro{std::move(macro_state.metadata)});
		}
	}

	/* Exit macro mode. */
	state.mode.emplace<SemanticState::ModeNormal>();
}

static void TerminateWhile(SemanticState &state, SemanticState::ModeWhile &while_state)
{
	const std::string_view old_source_line = state.source_line;

	/* Back-up some state into local variables, in case a nested WHILE statement clobbers it. */
	auto while_state_copy = std::move(while_state);

	/* Exit WHILE mode. */
	state.mode.emplace<SemanticState::ModeNormal>();

	state.listing_writer.SuppressLine();

	for (;;)
	{
		state.source_line = while_state_copy.source_line;

		/* We absolutely do not want to fold (cache) this expression, because
		   while-loops depend on variables mutating from previous values;
		   without this, while-loops will never terminate. */
		const auto &value = state.expression_processor.ResolveExpressionWithoutFolding(while_state_copy.expression, "Expression must be evaluable on the first pass.").value_or(0);

		if (value == 0)
			break;

		/* Rewind back to the line number of the start of the WHILE. */
		state.location.front().line_number = while_state_copy.line_number;

		/* Process the WHILE's nested statements. */
		for (const auto &source_line : while_state_copy.source_line_list)
			AssembleLine(state, source_line, false);
	}

	/* Increment past the ENDW line number. */
	++state.location.front().line_number;

	state.source_line = old_source_line;
}

static void PushSubstitute(SemanticState &state, const std::string_view &identifier, const std::string_view &value)
{
	SymbolDictionary *dictionary;
	SymbolDictionary::Entry* const dictionary_entry = state.symbol_manager.LookupSymbolAndCreateIfNotExist<SymbolStringConstant>(identifier, &dictionary);

	if (dictionary_entry != nullptr)
	{
		std::visit(
			CC_VisitHelper{
				[&](SymbolNone&)
				{
					auto &symbol = dictionary_entry->emplace<SymbolStringConstant>();
					symbol.value = value;
					auto &substitute = state.macro.dictionary != nullptr && dictionary == &*state.macro.dictionary ? state.macro.expansion->substitutions : state.substitutions;
					substitute.PushSubstitute(identifier, symbol.value);
				},
				[&](SymbolStringConstant &symbol)
				{
					symbol.value = value;
				},
				[&](auto&)
				{
					// Should never be ran, since `LookupSymbolAndCreateIfNotExist` rejects mismatched types.
					CC_UNREACHABLE;
				},
			}, *dictionary_entry
		);
	}
}

static void PushMacroArgumentSubstitutions(SemanticState &state)
{
	const std::string_view blank_argument;

	for (std::size_t argument_index = 0; argument_index < state.macro.expansion->metadata->parameter_names.size(); ++argument_index)
	{
		const std::string_view &parameter = state.macro.expansion->metadata->parameter_names[argument_index];
		const std::string_view &argument = argument_index >= state.macro.expansion->total_arguments ? blank_argument : state.macro.expansion->argument_list[argument_index];

		SymbolDictionary::Entry &dictionary_entry = state.macro.dictionary->LookUpAndCreateIfNotExist(parameter);

		/* TODO: De-duplicate this with the 'PushSubstitute' function. */
		std::visit(
			CC_VisitHelper{
				[&](SymbolNone&)
				{
					auto &symbol = dictionary_entry.emplace<SymbolStringConstant>();
					symbol.value = argument;
					state.macro.expansion->substitutions.PushSubstitute(parameter, symbol.value);
				},
				[&](SymbolStringConstant &symbol)
				{
					symbol.value = argument;
				},
				[&](auto&)
				{
					/* TODO: This is not a common case, but it can occur if e.g. someone redefines a parameter name as a normal symbol in the macro body.
					   See tests/previously-crashing/redefine-macro-parameter-in-macro-body.asm for an example of this.
					   That'll already have caused an error to be emitted, so this error might be somewhat cascading,
					   but I'm not certain it's not possible to hit this in other ways,
					   and in any case if we get here I'm pretty sure something has gone wrong, so we should emit an error in case one hasn't already been emitted. */
					state.error_manager.Error("Attempted to redefine symbol '{}' as a string constant.", parameter);
				},
			}, dictionary_entry
		);
	}
}

static void OutputDcValue(SemanticState &state, const Size size, unsigned long value)
{
	unsigned int bytes_to_write = 0;

	switch (size)
	{
		default:
			CC_UNREACHABLE;
			/* Fallthrough */
		case SIZE_BYTE:
		case SIZE_SHORT:
			if (value > 0xFF && value < 0xFFFFFF00)
				state.error_manager.Error("Value cannot be higher than $FF or lower than -$100, but was ${:X}.", value);

			bytes_to_write = 1;

			break;

		case SIZE_WORD:
			if (value > 0xFFFF && value < 0xFFFF0000)
				state.error_manager.Error("Value cannot be higher than $FFFF or lower than -$10000, but was ${:X}.", value);

			bytes_to_write = 2;

			break;

		case SIZE_LONGWORD:
			bytes_to_write = 4;
			break;
	}

	state.object_writer.WriteIntegerBE(value, bytes_to_write);
}

static void PerformEven(SemanticState &state)
{
	/* Pad to the nearest even address. */
	if ((state.program_counter & 1) != 0)
		state.object_writer.WriteU8(0);
}

static bool ShouldPerformAutomaticEven(SemanticState &state, const Size size)
{
	return state.options->automatic_even && size != SIZE_BYTE;
}

static void ProcessDc(SemanticState &state, StatementDc &dc)
{
	if (ShouldPerformAutomaticEven(state, dc.size))
		PerformEven(state);

	for (auto &expression : dc.values)
	{
		const auto expression_string = std::get_if<ExpressionString>(&expression);

		if (expression_string != nullptr && (dc.size == SIZE_BYTE || dc.size == SIZE_SHORT))
		{
			/* If this is a 'dc.b', and the value is a raw string, then output each character as a single byte. */
			/* This allows 'dc.b' to be used to embed strings into the output. */
			for (const char character : expression_string->string)
				OutputDcValue(state, dc.size, character);
		}
		else
		{
			/* Update the program counter symbol in between values, to keep it up to date. */
			state.expression_processor.program_counter_of_expression = state.program_counter;

			OutputDcValue(state, dc.size, state.expression_processor.ResolveExpression(expression).value_or(0));
		}
	}
}

static void ProcessDcb(SemanticState &state, StatementDcb &dcb)
{
	if (ShouldPerformAutomaticEven(state, dcb.size))
		PerformEven(state);

	const auto &repetitions = state.expression_processor.ResolveExpression(dcb.repetitions, "Repetition value must be evaluable on first pass.").value_or(0);
	const auto &value = state.expression_processor.ResolveExpression(dcb.value).value_or(0);

	for (unsigned long i = 0; i < repetitions; ++i)
		OutputDcValue(state, dcb.size, value);
}

static void ProcessDs(SemanticState &state, StatementDs &ds)
{
	if (ShouldPerformAutomaticEven(state, ds.size))
		PerformEven(state);

	const auto &length = state.expression_processor.ResolveExpression(ds.length, "Repetition value must be evaluable on first pass.").value_or(0);

	for (unsigned long i = 0; i < length; ++i)
		OutputDcValue(state, ds.size, 0);
}

static void ProcessInclude(SemanticState &state, const StatementInclude &include)
{
	std::ifstream input_stream(Utilities::Path::ToStandardPath(include.path));

	if (!input_stream.is_open())
	{
		state.error_manager.Error("File '{}' could not be opened.", include.path);
		return;
	}

	/* Add file path and line number to the location list. */
	state.location.emplace_front(include.path, 0);

	SourceLineReader source_line_reader(input_stream);
	AssembleFile(state, source_line_reader);

	state.location.pop_front();
}

static void ProcessIncbin(SemanticState &state, StatementIncbin &incbin)
{
	std::ifstream input_file(Utilities::Path::ToStandardPath(incbin.path), input_file.binary);

	if (!input_file.is_open())
	{
		state.error_manager.Error("File '{}' could not be opened.", incbin.path);
		return;
	}

	const auto &value = state.expression_processor.ResolveExpression(incbin.start, "Start value must be evaluable on the first pass.").value_or(0);

	input_file.seekg(value);

	if (input_file.fail())
		state.error_manager.Error("Start value is not a valid location in the file.");

	if (incbin.has_length)
	{
		const auto &length = state.expression_processor.ResolveExpression(incbin.length, "Length value must be evaluable on the first pass.").value_or(0);

		for (unsigned long i = 0; i < length; ++i)
		{
			const int character = input_file.get();

			if (input_file.gcount() == 0)
			{
				state.error_manager.Error("Length value is larger than the remaining number of bytes in the file.");
				break;
			}

			state.object_writer.WriteU8WithoutListing(character);
		}
	}
	else
	{
		for (;;)
		{
			const int character = input_file.get();

			if (input_file.gcount() == 0)
				break;

			state.object_writer.WriteU8WithoutListing(character);
		}
	}
}

static void ProcessRept(SemanticState &state, StatementRept &rept)
{
	/* Enter REPT mode. */
	auto &mode = state.mode.emplace<SemanticState::ModeRept>();

	mode.repetitions = state.expression_processor.ResolveExpression(rept.repetitions, "Repetition value must be evaluable on the first pass.").value_or(1);
	mode.line_number = state.location.front().line_number;
	mode.nesting = 0;
}

static void ProcessMacro(SemanticState &state, StatementMacro &macro, const std::string_view &label, bool is_short)
{
	/* Enter MACRO mode. */
	auto &mode = state.mode.emplace<SemanticState::ModeMacro>();

	mode.metadata.name = label;
	mode.line_number = state.location.front().line_number;
	mode.metadata.parameter_names = std::move(macro.parameter_names);
	mode.metadata.is_short = is_short;
	mode.metadata.uses_label = macro.uses_label;
}

static void ProcessIf(SemanticState &state, Expression &expression)
{
	++state.current_if_level;

	const auto &value = state.expression_processor.ResolveExpression(expression, "Condition must be evaluable on the first pass.").value_or(1);

	/* If this condition is false, then mark this as the false if-level. */
	if (value == 0)
		state.false_if_level = state.current_if_level;

	state.true_already_found = value != 0;
}

static void ProcessStatement(SemanticState &state, Statement &statement, const std::string_view &label)
{
	/* Bizarrely, depending on the context, the program counter can be one
	   of two values, so we need to maintain two separate copies of it. */
	state.expression_processor.program_counter_of_statement = state.program_counter;
	state.expression_processor.program_counter_of_expression = state.program_counter;

	if (CC_VariantHoldsAnyOf<
		StatementEmpty,
		StatementInstruction,
		StatementDc,
		StatementDcb,
		StatementDs,
		StatementInclude,
		StatementIncbin,
		StatementRept,
		StatementIf,
		StatementWhile,
		StatementEven,
		StatementCnop,
		StatementInform,
		StatementFail,
		StatementEnd,
		StatementPusho,
		StatementPopo,
		StatementOpt,
		StatementPushp,
		StatementPopp,
		StatementShift,
		StatementMexit,
		StatementLocal,
		StatementPurge
	>(statement))
	{
		if (!label.empty() && !state.doing_fixup_pass)
		{
			/* Handle the label here, instead of passing it onto a later function. */
			state.symbol_manager.SetLastGlobalLabel(label);
			AddToSymbolTable(state, label, SymbolLabel{state.program_counter});
		}
	}
	else if (CC_VariantHoldsAnyOf<
		StatementEndr,
		StatementEndm,
		StatementElseif,
		StatementElse,
		StatementEndc,
		StatementEndw,
		StatementRsset,
		StatementRsreset,
		StatementObj,
		StatementObjend,
		StatementOrg
	>(statement))
	{
		if (!label.empty())
			state.error_manager.Error("There cannot be a label on this type of statement.");
	}
	else if (CC_VariantHoldsAnyOf<
		StatementMacro,
		StatementMacros,
		StatementEqu,
		StatementEqusString,
		StatementEqusIdentifier,
		StatementSubstr,
		StatementSet
	>(statement))
	{
		if (label.empty())
		{
			state.error_manager.Error("This type of statement must have a label.");
			/* Bail, to avoid null pointer dereferences. */
			return;
		}
	}
	else if (CC_VariantHoldsAnyOf<
		StatementRs
	>(statement))
	{
		/* Handled later. */
	}

	std::visit(
		CC_VisitHelper{
			[&](StatementEmpty&) {},
			[&](StatementInstruction &statement)
			{
				state.instruction_processor.Process(statement);
			},
			[&](StatementDc &statement)
			{
				ProcessDc(state, statement);
			},
			[&](StatementDcb &statement)
			{
				ProcessDcb(state, statement);
			},
			[&](StatementDs &statement)
			{
				ProcessDs(state, statement);
			},
			[&](StatementInclude &statement)
			{
				ProcessInclude(state, statement);
			},
			[&](StatementIncbin &statement)
			{
				ProcessIncbin(state, statement);
			},
			[&](StatementRept &statement)
			{
				ProcessRept(state, statement);
			},
			[&](StatementEndr&)
			{
				/* Exit REPT mode. */
				const auto mode = std::get_if<SemanticState::ModeRept>(&state.mode);

				if (mode == nullptr)
					state.error_manager.Error("This stray ENDR has no preceeding REPT.");
				else
					TerminateRept(state, *mode);
			},
			[&](StatementMacro &statement)
			{
				ProcessMacro(state, statement, label, false);
			},
			[&](StatementMacros &statement)
			{
				ProcessMacro(state, statement.macro, label, true);
			},
			[&](StatementEndm&)
			{
				/* Exit MACRO mode. */
				const auto mode = std::get_if<SemanticState::ModeMacro>(&state.mode);

				if (mode == nullptr)
					state.error_manager.Error("This stray ENDM has no preceeding MACRO.");
				else if (mode->metadata.is_short)
					state.error_manager.Error("ENDM cannot be used in a short macro.");
				else
					TerminateMacro(state, *mode);
			},
			[&](StatementEqu &statement)
			{
				if (state.options->equ_set_descope_local_labels)
					state.symbol_manager.SetLastGlobalLabel(label);

				const auto &value_optional = state.expression_processor.ResolveExpression(statement.expression);

				/* Hack: We ABSOLUTELY DO NOT want this line adding to the fix-up list!!! */
				state.symbol_manager.fixup_needed = false;

				if (!value_optional.has_value())
				{
					AddToSymbolTable(state, label, SymbolExpressionConstant{std::move(statement.expression)});
					// TODO: Why does removing this result in a crash?
					statement.expression.emplace<ExpressionNumber>(0);
					return;
				}

				const auto &value = *value_optional;

				AddToSymbolTable(state, label, SymbolConstant{value});
				state.listing_writer.WriteIdentifierValue(value);
			},
			[&](StatementEqusString &statement)
			{
				PushSubstitute(state, label, statement.string);
			},
			[&](StatementEqusIdentifier &statement)
			{
				const SymbolDictionary::Entry* const dictionary_entry = state.symbol_manager.LookupSymbol(statement.string, nullptr);

				const SymbolStringConstant* const symbol_string_constant = dictionary_entry == nullptr ? nullptr : std::get_if<SymbolStringConstant>(dictionary_entry);

				if (symbol_string_constant == nullptr)
					state.error_manager.Error("String constant can only be defined as a string or another string constant.");
				else
					PushSubstitute(state, label, symbol_string_constant->value);
			},
			[&](StatementSubstr &statement)
			{
				auto start = state.expression_processor.ResolveExpression(statement.start, "Start must be evaluable on the first pass.").value_or(0);
				auto end = state.expression_processor.ResolveExpression(statement.end, "End must be evaluable on the first pass.").value_or(statement.string.length());

				if (start == 0)
				{
					state.error_manager.Error("Start cannot be zero.");
					return;
				}

				if (end < start)
					end = start - 1;

				/* Clamp start and end to make it so that, if the string would go beyond the end, it is truncated appropriately instead of going out-of-bounds */
				/* (this matches asm68k, as far as I can see). */
				if (start > statement.string.length())
					start = statement.string.length() + 1;
				if (end > statement.string.length())
					end = statement.string.length();

				PushSubstitute(state, label, statement.string.substr(start - 1, end - start + 1));
			},
			[&](StatementSet &statement)
			{
				if (state.options->equ_set_descope_local_labels)
					state.symbol_manager.SetLastGlobalLabel(label);

				const auto &value = state.expression_processor.ResolveExpression(statement.expression, "Value must be evaluable on the first pass.").value_or(0);

				AddToSymbolTable(state, label, SymbolVariable{value});
				state.listing_writer.WriteIdentifierValue(value);
			},
			[&](StatementIf &statement)
			{
				ProcessIf(state, statement.expression);
			},
			[&](StatementElseif &statement)
			{
				if (state.current_if_level == 0)
				{
					state.error_manager.Error("This stray ELSEIF has no preceeding IF.");
					return;
				}

				if (state.false_if_level == state.current_if_level || state.false_if_level == 0)
				{
					const auto &value = state.expression_processor.ResolveExpression(statement.expression, "Condition must be evaluable on the first pass.").value_or(1);

					/* If this condition is false, then mark this as the false if-level. */
					if (state.true_already_found || value == 0)
					{
						state.false_if_level = state.current_if_level;
					}
					else
					{
						state.false_if_level = 0;
						state.true_already_found = true;
					}
				}
			},
			[&](StatementElse&)
			{
				if (state.current_if_level == 0)
				{
					state.error_manager.Error("This stray ELSE has no preceeding IF.");
					return;
				}

				if (state.false_if_level == state.current_if_level || state.false_if_level == 0)
				{
					if (state.true_already_found)
					{
						state.false_if_level = state.current_if_level;
					}
					else
					{
						/* If this is the false if-level, then it isn't anymore. */
						/* Likewise, if there is no false if-level, then there is now. */
						if (state.false_if_level == state.current_if_level)
							state.false_if_level = 0;
						else /*if (state.false_if_level == 0)*/
							state.false_if_level = state.current_if_level;
					}
				}
			},
			[&](StatementEndc&)
			{
				if (state.current_if_level == 0)
					state.error_manager.Error("This stray ENDC/ENDIF has no preceeding IF.");
				else
					TerminateIf(state);
			},
			[&](StatementWhile &statement)
			{
				/* Enter WHILE mode. */
				auto &mode = state.mode.emplace<SemanticState::ModeWhile>();

				/* Dummy-out the statement's expression, since the state now owns it, and we don't
				   want its sub-expressions to be automatically freed when the statement is freed. */
				mode.expression.emplace<ExpressionNumber>(0);
				std::swap(mode.expression, statement.expression);
				mode.source_line = state.source_line;

				mode.line_number = state.location.front().line_number;
				mode.nesting = 0;
			},
			[&](StatementEndw&)
			{
				/* Exit WHILE mode. */
				const auto mode = std::get_if<SemanticState::ModeWhile>(&state.mode);

				if (mode == nullptr)
					state.error_manager.Error("This stray ENDW has no preceeding WHILE.");
				else
					TerminateWhile(state, *mode);
			},
			[&](StatementEven&)
			{
				PerformEven(state);
			},
			[&](StatementCnop &statement)
			{
				const auto &offset = state.expression_processor.ResolveExpression(statement.offset, "Offset must be evaluable on the first pass.").value_or(0);
				const auto &size_boundary = state.expression_processor.ResolveExpression(statement.size_boundary, "Size boundary must be evaluable on the first pass.").value_or(1);

				if (size_boundary == 0)
				{
					/* I don't know if there's a way to make this have meaningfully useful semantics. asm68k errors on this, so for now we will too. */
					state.error_manager.Error("Size boundary cannot be zero.");
					return;
				}

				const unsigned long position_within_boundary = state.program_counter % size_boundary;

				unsigned long target = state.program_counter - position_within_boundary + offset;

				if (position_within_boundary > offset)
					target += size_boundary;

				/* Pad to the desired offset. */
				while (state.program_counter != target)
					state.object_writer.WriteU8(0);
			},
			[&](StatementInform &statement)
			{
				/* Definitely an 'INFORM'... */
				const auto &severity = state.expression_processor.ResolveExpression(statement.severity, "Severity must be evaluable on the first pass.").value_or(0);

				/* TODO - Everything... */
				switch (severity)
				{
					case 0:
						state.error_manager.Information("INFORM: '{}'", statement.message);
						break;

					case 1:
						state.error_manager.Warning("INFORM: '{}'", statement.message);
						break;

					case 2:
					case 3: /* TODO: Halt assembly. */
						state.error_manager.Error("INFORM: '{}'", statement.message);
						break;
				}
			},
			[&](StatementFail&)
			{
				/* TODO: Halt assembly. */
				state.error_manager.Error("Assembly Failed");
			},
			[&](StatementEnd&)
			{
				state.end = true;
			},
			[&](StatementRs &statement)
			{
				if (ShouldPerformAutomaticEven(state, statement.size))
					if ((state.symbol_manager.rs & 1) != 0)
						++state.symbol_manager.rs;

				auto length = state.expression_processor.ResolveExpression(statement.length, "Length must be evaluable on the first pass.").value_or(0);

				/* Add label to symbol table. */
				if (!label.empty())
				{
					AddToSymbolTable(state, label, SymbolConstant{state.symbol_manager.rs});
					state.listing_writer.WriteIdentifierValue(state.symbol_manager.rs);
				}

				/* Advance '__rs' by the specified amount. */
				switch (statement.size)
				{
					default:
						CC_UNREACHABLE;
						/* Fallthrough */
					case SIZE_BYTE:
					case SIZE_SHORT:
						length *= 1;
						break;

					case SIZE_WORD:
						length *= 2;
						break;

					case SIZE_LONGWORD:
						length *= 4;
						break;
				}

				state.symbol_manager.rs += length;
			},
			[&](StatementRsset &statement)
			{
				/* Set '__rs' to value. */
				state.symbol_manager.rs = state.expression_processor.ResolveExpression(statement.expression, "Value must be evaluable on the first pass.").value_or(state.symbol_manager.rs);
			},
			[&](StatementRsreset&)
			{
				/* Set '__rs' to 0. */
				state.symbol_manager.rs = 0;
			},
			[&](StatementObj &statement)
			{
				if (state.obj_delta.has_value())
				{
					state.error_manager.Error("OBJ statements cannot be nested.");
					return;
				}

				const auto &value = state.expression_processor.ResolveExpression(statement.expression, "Value must be evaluable on the first pass.").value_or(state.program_counter);

				state.obj_delta = value - state.program_counter;
				state.program_counter += *state.obj_delta;
			},
			[&](StatementObjend&)
			{
				if (!state.obj_delta.has_value())
				{
					state.error_manager.Error("This stray OBJEND has no preceeding OBJ.");
					return;
				}

				state.program_counter -= *state.obj_delta;
				state.obj_delta = std::nullopt;
			},
			[&](StatementOrg &statement)
			{
				const auto &value = state.expression_processor.ResolveExpression(statement.expression, "Value must be evaluable on the first pass.").value_or(state.program_counter);

				if (value > 0xFFFFFFFF)
				{
					state.error_manager.Error("Value cannot be larger than 0xFFFFFFFF (was 0x{:X}).", value);
					return;
				}

				state.object_writer.BeginNewSegment(value);

				state.program_counter = value;
			},
			[&](StatementPusho&)
			{
				state.options.Push();
			},
			[&](StatementPopo&)
			{
				state.options.Pop();
			},
			[&](StatementOpt &statement)
			{
				for (const auto &option : statement.options)
				{
					/* TODO: A lot of these are unimplemented... */
					if (Utilities::CaseInsensitive::CompareStrings(option, "ae+"))
						state.options->automatic_even = true;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "ae-"))
						state.options->automatic_even = false;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "an+"))
						;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "an-"))
						;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "c+"))
						state.options->case_insensitive = false;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "c-"))
						state.options->case_insensitive = true;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "d+"))
						state.options->equ_set_descope_local_labels = true;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "d-"))
						state.options->equ_set_descope_local_labels = false;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "e+"))
						;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "e-"))
						;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "l+"))
						state.options->local_signifier = '.';
					else if (Utilities::CaseInsensitive::CompareStrings(option, "l-"))
						state.options->local_signifier = '@';
					else if (option.length() == 2 && (option[0] == 'l' || option[0] == 'L'))
						state.options->local_signifier = option[1];
					/*else if (Utilities::CaseInsensitive::CompareStrings(option, "s+"))
						;*/
					else if (Utilities::CaseInsensitive::CompareStrings(option, "s-"))
						;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "m+"))
						state.options->expand_all_macros = true;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "m-"))
						state.options->expand_all_macros = false;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "oz+"))
						state.options->optimisation.zero_displacement = true;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "oz-"))
						state.options->optimisation.zero_displacement = false;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "v+"))
						state.options->output_local_labels_to_sym_file = true;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "v-"))
						state.options->output_local_labels_to_sym_file = false;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "w+"))
						state.options->warnings_enabled = true;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "w-"))
						state.options->warnings_enabled = false;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "wp+"))
						state.options->pedantic_warnings_enabled = true;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "wp-"))
						state.options->pedantic_warnings_enabled = false;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "ws+"))
						;
					else if (Utilities::CaseInsensitive::CompareStrings(option, "ws-"))
						;
					else
						state.error_manager.Warning("Unrecognised option '{}'.", option);
				}
			},
			[&](StatementPushp &statement)
			{
				state.string_stack.emplace(std::move(statement.string));
			},
			[&](StatementPopp &statement)
			{
				if (state.string_stack.empty())
				{
					state.error_manager.Error("POPP used but the string stack was empty.");
					return;
				}

				PushSubstitute(state, statement.string, state.string_stack.top());
				state.string_stack.pop();
			},
			[&](StatementShift&)
			{
				if (!state.macro.ExpansionInProgress())
				{
					state.error_manager.Error("SHIFT used outside of macro.");
					return;
				}

				if (state.macro.expansion->total_arguments == 0)
				{
					/* TODO: What if there are already 0 arguments? */
					/* This prevents crashing for now but perhaps it could be handled differently. */
					state.error_manager.Error("SHIFT used but there are no macro arguments to shift.");
					return;
				}

				--state.macro.expansion->total_arguments;
				for (unsigned int i = 0; i < state.macro.expansion->total_arguments; ++i)
					state.macro.expansion->argument_list[i] = std::move(state.macro.expansion->argument_list[i + 1]);
				state.macro.expansion->argument_list.pop_back();
				PushMacroArgumentSubstitutions(state);
			},
			[&](StatementMexit&)
			{
				state.macro.expansion = std::nullopt;
			},
			[&](StatementLocal &statement)
			{
				if (!state.macro.ExpansionInProgress())
				{
					state.error_manager.Error("LOCAL used outside of macro.");
					return;
				}

				for (const auto &identifier : statement.identifiers)
					state.macro.dictionary->LookUpAndCreateIfNotExist(identifier);
			},
			[&](StatementPurge &statement)
			{
				state.symbol_manager.PurgeMacro(statement.string, false);
			}
		}, statement
	);

	/* Update both copies of the program counter again, so that things like WHILE statements don't use stale values in their expressions. */
	state.expression_processor.program_counter_of_statement = state.program_counter;
	state.expression_processor.program_counter_of_expression = state.program_counter;
}

static void ParseLine(SemanticState &state, const std::string_view &label, const std::string_view &directive_and_operands)
{
	/* This is a normal assembly line. */

	/* Back these up, before they get a chance to be modified by ProcessStatement. */
	const unsigned long starting_program_counter = state.program_counter;
	const auto starting_object_writer_position = state.object_writer.Tell();

	Statement* const statement = state.parser.ParseStatement(directive_and_operands);

	if (statement == nullptr)
		return;

	ProcessStatement(state, *statement, label);

	if (!state.symbol_manager.fixup_needed)
		return;

	/* If the statement cannot currently be processed because of undefined symbols,
	   add it to the fix-up list so we can try again later. */
	state.fixup_list.emplace_back(
		/* Backup the statement. */
		std::move(*statement),

		/* Backup some state. */
		state.macro.dictionary,
		starting_program_counter,
		starting_object_writer_position,
		state.symbol_manager.GetLastGlobalLabel(),
		std::string(state.source_line),
		std::string(label),
		*state.options,

		/* Clone the location. */
		state.location
	);

	state.symbol_manager.fixup_needed = false;
}

#define DIRECTIVE_OR_MACRO_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?_"
#define LABEL_CHARS DIRECTIVE_OR_MACRO_CHARS ".@"

static void PerformSubstitutionsOnSubString(SemanticState &state, std::string &string, std::string_view &view_to_search, const bool allow_implicit_matches)
{
	const bool case_insensitive = state.options->case_insensitive;

	if (state.macro.ExpansionInProgress())
		state.substitutions.ProcessSubString(&state.macro.expansion->substitutions, string, view_to_search, state.macro.expansion->custom_search, allow_implicit_matches, case_insensitive);
	else
		state.substitutions.ProcessSubString(nullptr, string, view_to_search, nullptr, allow_implicit_matches, case_insensitive);
}

static void PerformSubstitutions(SemanticState &state, std::string &string, const bool allow_implicit_matches)
{
	std::string_view view = string;
	PerformSubstitutionsOnSubString(state, string, view, allow_implicit_matches);
}

static std::optional<std::string_view> FindQuotedStringInSourceLine(const std::string_view &string)
{
	const auto start_position = string.find_first_of("'\"");

	if (start_position == string.npos)
		return std::nullopt;

	const auto end_position = string.find_first_of(string[start_position], start_position + 1);

	if (end_position == string.npos)
		return std::nullopt;

	return string.substr(start_position, end_position - start_position);
}

/* Performs string substitutions only in parts of the string that are not wrapped in quotations. */
/* This is needed to simulate how SN 68k does not expand string constants in strings, only expressions and operands. */
static void PerformSubstitutionsExcludingQuotedStrings(SemanticState &state, std::string &source_line, const std::size_t starting_position, const bool allow_implicit_matches)
{
	std::string_view source_line_subview = std::string_view(source_line).substr(starting_position);

	for (;;)
	{
		/* If no string was found, then pretend there's a string at the end of the line, so that we can repurpose the code below. */
		const auto quoted_string = FindQuotedStringInSourceLine(source_line_subview).value_or(source_line_subview.substr(source_line_subview.length()));

		/* We've found a string! Perform substitutions on the part of the line before it. */

		/* Define the region that we will perform substitutions on. */
		auto view_to_search = source_line_subview.substr(0, quoted_string.data() - source_line_subview.data());

		/* Perform the substitutions. */
		PerformSubstitutionsOnSubString(state, source_line, view_to_search, allow_implicit_matches);

		if (quoted_string.empty())
			break;

		/* Move to after the string so that we can continue searching. */
		source_line_subview = source_line_subview.substr(view_to_search.length() + quoted_string.length());
	}
}

static void SubstituteAndParseLine(SemanticState &state, const std::string_view &label, const std::string_view &directive_and_operands, const std::string_view &directive)
{
	std::string modified_directive_and_operands;

	/* Because of the below hack, we have to filter-out directives that do not use expressions or operands,
	   to prevent them from accidentally being substituted. This is needed for the Natsumi Z80 macros. */
	if (Utilities::CaseInsensitive::CompareStrings(directive, "binclude")
	 || Utilities::CaseInsensitive::CompareStrings(directive, "equs")
	 || Utilities::CaseInsensitive::CompareStrings(directive, "incbin")
	 || Utilities::CaseInsensitive::CompareStrings(directive, "include")
	 || Utilities::CaseInsensitive::CompareStrings(directive, "macro")
	 || Utilities::CaseInsensitive::CompareStrings(directive, "macros")
	 || Utilities::CaseInsensitive::CompareStrings(directive, "opt")
	 || Utilities::CaseInsensitive::CompareStrings(directive, "popp"))
	{
		ParseLine(state, label, directive_and_operands);
		return;
	}

	/* Perform implicit (backslash-less) string substitutions on the operands.
	   SN 68k achieves this by expanding string constants in its operand and expression evaluators,
	   which is absolutely insane, so this trick approximates it instead. */
	modified_directive_and_operands = directive_and_operands;
	PerformSubstitutionsExcludingQuotedStrings(state, modified_directive_and_operands, directive.length(), true);

	ParseLine(state, label, modified_directive_and_operands);
}

static void InvokeMacro(SemanticState &state, const Macro &macro, const std::string_view &label, std::string_view source_line)
{
	if (state.macro_depth >= 0x100)
	{
		state.error_manager.Error("Macro expansion depth exceeded.");
		return;
	}

	++state.macro_depth;

	const auto starting_if_level = state.current_if_level;

	MacroState previous_macro_state = std::move(state.macro);

	/* TODO: We should not be caching 'case_insensitive'; the OPT directive can change this at any time! */
	state.macro.dictionary = std::make_shared<SymbolDictionary>(state.options->case_insensitive);

	state.macro.expansion.emplace(macro);

	++state.current_macro_invocation;

	state.listing_writer.SuppressLine();

	Utilities::String::RemoveLeading(source_line, DIRECTIVE_OR_MACRO_CHARS);

	if (!label.empty())
	{
		state.symbol_manager.SetLastGlobalLabel(label);

		if (!macro.uses_label)
			AddToSymbolTable(state, label, SymbolLabel{state.program_counter});
	}

	std::string_view size;

	/* Extract and store the macro size specifier, if one exists. */
	if (!source_line.empty() && source_line.front() == '.')
	{
		/* Skip the '.' character. */
		source_line.remove_prefix(1);

		size = Utilities::String::ExtractLeading(source_line, DIRECTIVE_OR_MACRO_CHARS);

		/* Advance past the size specifier. */
		source_line.remove_prefix(size.length());
	}

	/* Extract combined arguments string. */
	std::string_view arguments = source_line.substr(0, source_line.find(';'));

	/* Remove surrounding whitespace. */
	Utilities::String::RemoveLeadingAndTrailing(arguments, " \t");

	const std::string unique_suffix = std::format("_{}", state.current_macro_invocation);
	std::string symbol_value_string;

	state.macro.expansion->custom_search = [&state, &size, &arguments, &label, &unique_suffix, &symbol_value_string](const std::string_view &view_to_search, std::size_t starting_position, [[maybe_unused]] bool case_insensitive, std::size_t &found_position, std::size_t &found_length) -> std::optional<std::string_view>
	{
		for (found_position = starting_position; (found_position = view_to_search.find('\\', found_position)) != view_to_search.npos; ++found_position)
		{
			const std::size_t symbol_position = found_position + 1;

			if (symbol_position == view_to_search.length())
				break;

			const char symbol = view_to_search[symbol_position];

			found_length = 2;

			switch (symbol)
			{
				case '0':
					return size;

				case '_':
					if (found_position + found_length < view_to_search.length() && Substitute::IsBlockingCharacter(view_to_search[found_position + found_length]))
						continue;

					return arguments;

				case '*':
					return label;

				case '@':
					return unique_suffix;

				case '#':
				case '$':
				{
					const std::size_t identifier_position_start = symbol_position + 1;
					const std::size_t identifier_length = Utilities::String::GetLeadingLength(view_to_search, DIRECTIVE_OR_MACRO_CHARS, identifier_position_start);
					const std::size_t identifier_position_end = identifier_position_start + identifier_length;

					const std::string_view identifier = view_to_search.substr(identifier_position_start, identifier_length);

					found_length += identifier.length();

					/* Absorb trailing backslash. */
					if (identifier_position_end < view_to_search.length() && view_to_search[identifier_position_end] == '\\')
						++found_length;

					const auto &value = state.symbol_manager.GetSymbolInteger(identifier, true).value_or(0);

					if (symbol == '#')
						symbol_value_string = std::format("{}", value);
					else /*if (symbol == '$')*/
						symbol_value_string = std::format("{:X}", value);

					return symbol_value_string;
				}

				default:
				{
					/* Obtain numerical index of the parameter. */
					unsigned int parameter_index;
					const auto start = view_to_search.data() + symbol_position;
					const auto [end, error_code] = std::from_chars(start, view_to_search.data() + view_to_search.length(), parameter_index);

					--parameter_index;

					/* Check if conversion failed. */
					if (error_code != std::errc{})
						continue;

					found_length = 1 + (end - start);

					if (parameter_index >= state.macro.expansion->total_arguments)
						return "";

					return state.macro.expansion->argument_list[parameter_index];
				}
			}
		}

		return std::nullopt;
	};

	/* Extract and store the individual macro arguments, if they exist. */
	std::size_t argument_string_start = 0;
	unsigned int argument_depth = 0;

	for (std::size_t i = 0; i <= arguments.length(); ++i)
	{
		const bool at_the_end = i == arguments.length();
		const char character = at_the_end ? ',' : arguments[i];

		switch (character)
		{
			case '(':
				++argument_depth;
				break;

			case ')':
				--argument_depth;
				break;

			case ',':
				if (argument_depth == 0)
				{
					/* Extract argument. */
					std::string_view argument = arguments.substr(argument_string_start, i - argument_string_start);
					argument_string_start = i + 1;

					/* Remove surrounding whitespace. */
					Utilities::String::RemoveLeadingAndTrailing(argument, " \t");

					/* Do not bother pushing an empty first argument if it is the only argument, as that just means there are no arguments. */
					if (argument.empty() && at_the_end && state.macro.expansion->total_arguments == 0)
						break;

					/* Add to argument list. */
					state.macro.expansion->argument_list.emplace_back(argument);

					if (!argument.empty() && argument.front() != '"' && argument.front() != '\'' && state.options->case_insensitive)
						Utilities::String::ToLower(state.macro.expansion->argument_list[state.macro.expansion->total_arguments]);

					++state.macro.expansion->total_arguments;
					++state.macro.expansion->total_arguments_at_start;
				}

				break;
		}
	}

	/* Add identifier substitute. */
	PushMacroArgumentSubstitutions(state);

	/* Finally, invoke the macro. */
	const bool previous_listing_macro = state.listing_writer.listing_macro;

	/* Flush line before changing this state, so that the macro line is not given the 'M' label. */
	state.listing_writer.EndLine(state.source_line);
	state.listing_writer.listing_macro = true;

	/* Push a new location (this macro).*/
	state.location.emplace_front(macro.name, 0);

	/* Iterate over each line of the macro, sending it to be processed. */
	for (const auto &source_line : macro.source_line_list)
	{
		/* TODO: This is redundant and wrong: these substitutions are already done later, and this should only be doing the macro arguments! */
		if (macro.is_short)
		{
			std::string expanded_source_line = source_line;
			PerformSubstitutions(state, expanded_source_line, false);
			AssembleLine(state, expanded_source_line, state.options->expand_all_macros);
		}
		else
		{
			AssembleLine(state, source_line, state.options->expand_all_macros);
		}

		if (!state.macro.ExpansionInProgress())
			break;
	}

	if (!macro.is_short)
	{
		/* 'MEXIT' may have ended us midway through an if-statement, so unwind to the original if-level here. */
		while (state.current_if_level != starting_if_level)
			TerminateIf(state);
	}

	/* Pop location. */
	state.location.pop_front();

	state.listing_writer.listing_macro = previous_listing_macro;

	state.macro = std::move(previous_macro_state);

	--state.macro_depth;
}

static void AssembleLine(SemanticState &state, const std::string_view &source_line_raw, const bool write_line_to_listing_file)
{
	const std::string_view old_source_line = state.source_line;

	/* Assert a few preconditions to ensure we're in a non-insane state. */
	assert(state.macro.ExpansionInProgress() == (state.macro_depth > 0));

	if (write_line_to_listing_file)
	{
		state.listing_writer.EndLine(state.source_line);
		state.listing_writer.BeginLine(state.program_counter);
	}

	/* Expand string constants and macro parameters only if we're actually parsing code;
	   it is important that the input of REPT and WHILE is unaltered, so that each iteration
	   can have unique expansions! */
	std::string source_line_expanded;

	if (std::holds_alternative<SemanticState::ModeNormal>(state.mode))
	{
		source_line_expanded = source_line_raw;
		PerformSubstitutions(state, source_line_expanded, false);
		state.source_line = source_line_expanded;
	}
	else
	{
		state.source_line = source_line_raw;
	}

	++state.location.front().line_number;

	if (!state.source_line.empty() && state.source_line.front() == '*')
	{
		/* This whole line is a comment. */
	}
	else
	{
		/* Despite the fact that we're using Flex and Bison to parse the
		   language for us, we unfortunately have to do quite a bit of
		   parsing manually because of inconsistent whitespace sensitivity
		   (label recognision) and manipulation of the input string
		   (macro expansion). */
		std::string_view source_line_subview = state.source_line;

		/* Let's begin by parsing the label (if there is one). */
		std::string_view label = source_line_subview;

		/* Determine the length of the label. */
		label = Utilities::String::ExtractLeading(label, LABEL_CHARS);

		if (!label.empty())
		{
			/* We've found a label! */
		}
		else
		{
			/* Skip whitespace. */
			Utilities::String::RemoveLeading(source_line_subview, " \t");

			/* Maybe the label has some whitespace before it: check again. */
			label = source_line_subview;

			/* Determine the length of the label again. */
			const auto label_length = Utilities::String::GetLeadingLength(label, LABEL_CHARS);

			if (!label.empty())
			{
				/* Since we've encountered whitespace, it's ambiguous whether this is a label or a statement.
				   To figure it out, we have to rely on the label having a ':' after it. */
				if (label_length < label.length() && label[label_length] == ':')
				{
					/* We've found the label. */
					label = label.substr(0, label_length);
				}
				else
				{
					/* Not a label. */
					label = {};
				}
			}
		}

		if (!label.empty())
		{
			/* Advance past the label in the source string. */
			source_line_subview.remove_prefix(label.length());

			/* Skip past the colon at the end, if one exists. */
			if (!source_line_subview.empty() && source_line_subview.front() == ':')
				source_line_subview.remove_prefix(1);
		}

		/* Skip the whitespace between the label and the directive. */
		Utilities::String::RemoveLeading(source_line_subview, " \t");

		/* Next let's process the directive. If it's an if-statement or a macro
		   invocation, then we'll need to handle it manually. If not, then we
		   can pass it to Flex and Bison. */

		const std::string_view directive_and_operands = source_line_subview;
		const std::string_view directive = Utilities::String::ExtractLeading(source_line_subview, DIRECTIVE_OR_MACRO_CHARS);

		/* This is either a directive, or a macro. */
		std::visit(
			CC_VisitHelper{
				[&](SemanticState::ModeNormal&)
				{
					/* If we are in the false part of an if-statement, then manually parse the
					   source code until we encounter an IF, ELSEIF, ELSE, ENDC, or ENDIF.
					   The reason for this is that the false part of an if-statement may contain
					   invalid code and we do not want it to cause errors.
					   This can get pretty complicated because we need to account for nesting. */
					if (state.false_if_level != 0)
					{
						/* TODO - Detect code after the keyword and error if any is found. */
						if (Utilities::CaseInsensitive::CompareStrings(directive, "if"))
						{
							/* If-statements that are nested within the false part of another if-statement
							   are themselves false, so create a false if-statement here and process it. */
							++state.current_if_level;
						}
						else if (Utilities::CaseInsensitive::CompareStrings(directive, "elseif")
						      || Utilities::CaseInsensitive::CompareStrings(directive, "else"  )
						      || Utilities::CaseInsensitive::CompareStrings(directive, "endc"  )
						      || Utilities::CaseInsensitive::CompareStrings(directive, "endif" ))
						{
							/* These can be processed normally too. */
							SubstituteAndParseLine(state, label, directive_and_operands, directive);
						}
						else
						{
							/* Drop the line completely, since it's inside the false half of an if statement and should be ignored. */
						}
					}
					else
					{
						/* Look up the directive in the dictionary to see if it's actually a macro. */
						const SymbolDictionary::Entry* const macro_dictionary_entry = state.symbol_manager.LookupSymbol(directive, nullptr);
						const SymbolMacro* const symbol_macro = macro_dictionary_entry == nullptr ? nullptr : std::get_if<SymbolMacro>(macro_dictionary_entry);
						const Macro* const macro = symbol_macro == nullptr ? nullptr : &symbol_macro->macro;

						if (macro != nullptr /*&& !macro->is_short*/)
						{
							/* Regular macros are only expanded when needed. */
							InvokeMacro(state, *macro, label, source_line_subview);
						}
						else
						{
							/* This is not a macro invocation: it's just a regular line that can be assembled as-is. */
							SubstituteAndParseLine(state, label, directive_and_operands, directive);
						}
					}
				},
				[&](SemanticState::ModeRept &rept_state)
				{
					/* If this line is an 'ENDR' directive, then exit REPT mode. Otherwise, add the line to the REPT. */
					if (Utilities::CaseInsensitive::CompareStrings(directive, "endr") && rept_state.nesting-- == 0)
					{
						/* TODO - Detect code after the keyword and error if any is found. */
						ParseLine(state, label, directive_and_operands);
					}
					else
					{
						if (Utilities::CaseInsensitive::CompareStrings(directive, "rept"))
							++rept_state.nesting;

						rept_state.source_line_list.emplace_back(state.source_line);
					}
				},
				[&](SemanticState::ModeMacro &macro_state)
				{
					/* If this line is an 'ENDM' directive, then exit macro mode. Otherwise, add the line to the macro. */
					if (Utilities::CaseInsensitive::CompareStrings(directive, "endm"))
					{
						/* TODO - Detect code after the keyword and error if any is found. */
						if (macro_state.metadata.is_short)
							state.error_manager.Error("Short macros shouldn't use ENDM.");
						else
							ParseLine(state, label, directive_and_operands);
					}
					else
					{
						if (macro_state.metadata.is_short)
						{
							/* Short macros automatically terminate after one statement. */
							const std::size_t first_nonwhitespace_position = Utilities::String::GetLeadingLength(state.source_line, " \t");
							const char first_nonwhitespace_character = first_nonwhitespace_position == state.source_line.length() ? '\0' : state.source_line[first_nonwhitespace_position];

							if (!label.empty())
								state.error_manager.Error("Short macros shouldn't create labels.");

							if (first_nonwhitespace_character == '\0' || first_nonwhitespace_character == ';')
							{
								/* This line is an empty statement: ignore it. */
							}
							else
							{
								macro_state.metadata.source_line_list.emplace_back(state.source_line);
								TerminateMacro(state, macro_state);
							}
						}
						else
						{
							macro_state.metadata.source_line_list.emplace_back(state.source_line);
						}
					}
				},
				[&](SemanticState::ModeWhile &while_state)
				{
					/* If this line is an 'ENDW' directive, then exit 'WHILE' mode. Otherwise, add the line to the 'WHILE'. */
					if (Utilities::CaseInsensitive::CompareStrings(directive, "endw") && while_state.nesting-- == 0)
					{
						/* TODO - Detect code after the keyword and error if any is found. */
						ParseLine(state, label, directive_and_operands);
					}
					else
					{
						if (Utilities::CaseInsensitive::CompareStrings(directive, "while"))
							++while_state.nesting;

						while_state.source_line_list.emplace_back(state.source_line);
					}
				}
			}, state.mode
		);
	}

	if (write_line_to_listing_file)
		state.listing_writer.EndLine(state.source_line);

	state.source_line = old_source_line;
}

static void AssembleFile(SemanticState &state, SourceLineReader &source_line_reader)
{
	/* Read lines one at a time, feeding them to the 'AssembleLine' function. */
	for (std::optional<std::string_view> source_line; (source_line = source_line_reader.Read()).has_value() && !state.end; )
		AssembleLine(state, *source_line, true);

	/* If we're not in normal mode when a file ends, then something is wrong. */
	while (!std::holds_alternative<SemanticState::ModeNormal>(state.mode))
	{
		std::visit(
			CC_VisitHelper{
				[&](SemanticState::ModeNormal&)
				{
					/* We can't reach this since we just checked for it. */
					CC_UNREACHABLE;
				},
				[&](SemanticState::ModeRept &rept_state)
				{
					state.error_manager.Error("REPT statement beginning at line {} is missing its ENDR.", rept_state.line_number);

					/* Terminate the REPT to hopefully avoid future complications. */
					TerminateRept(state, rept_state);
				},
				[&](SemanticState::ModeMacro &macro_state)
				{
					state.error_manager.Error("MACRO statement beginning at line {} is missing its ENDM.", macro_state.line_number);

					/* Terminate the macro to hopefully avoid future complications. */
					TerminateMacro(state, macro_state);
				},
				[&](SemanticState::ModeWhile &while_state)
				{
					state.error_manager.Error("WHILE statement beginning at line {} is missing its ENDW.", while_state.line_number);

					/* Terminate the while-statement to hopefully avoid future complications. */
					TerminateWhile(state, while_state);
				}
			}, state.mode
		);
	}
}

static void AddDefinition(void* const internal, const std::string_view &identifier, const unsigned long value)
{
	SemanticState &state = *(SemanticState*)internal;

	/* TODO: Does asm68k create a constant or a variable? */
	AddToSymbolTable(state, identifier, SymbolConstant{value});
}

static bool AssembleToObjectFile(
	std::istream &input_stream,
	std::ostream &output_stream,
	std::ostream &error_stream,
	std::ostream* const listing_stream,
	std::ostream* const symbol_stream,
	const std::string_view &input_file_path,
	ClownAssembler::Settings initial_options,
	const ClownAssembler::DefinitionCallback definition_callback,
	const void* const user_data)
{
	SemanticState state(
		input_stream,
		output_stream,
		error_stream,
		listing_stream,
		input_file_path,
		std::move(initial_options)
	);

	if (!AddToSymbolTable(state, "__rs", SymbolSpecial{SymbolSpecial::Type::RS}))
		return false;

	if (!AddToSymbolTable(state, "narg", SymbolSpecial{SymbolSpecial::Type::NARG}))
		return false;

	if (definition_callback != nullptr)
		definition_callback(&state, (void*)user_data, AddDefinition);

	/* Enable Bison debugging if available and requested by the user. */
#if M68KASM_DEBUG
	if (state.options->debug)
		m68kasm_debug = 1;
#endif

	/* Perform first pass of assembly, creating a list of fix-ups. */
	state.object_writer.WriteHeader();
	AssembleFile(state, state.source_line_reader);
	state.object_writer.WriteFooter();

	/* Perform some sanity checks to make sure we're not somehow in an invalid state. */
	if (state.current_if_level != 0)
		state.error_manager.Error("An IF statement somewhere is missing its ENDC/ENDIF.");
	if (state.obj_delta.has_value())
		state.error_manager.Error("An OBJ statement somewhere is missing its OBJEND.");

	/* Process the fix-ups: reassemble instructions and reprocess
	   directives that could not be completed in the first pass. */
	state.doing_fixup_pass = true;

	while (!state.fixup_list.empty())
	{
		/* Pop fix-up from list. */
		FixUp &fixup = state.fixup_list.front();

		/* Reset some state to how it was at the time the statement was first processed. */
		state.macro.dictionary = fixup.macro_dictionary;
		state.program_counter = fixup.program_counter;
		state.object_writer.Seek(fixup.object_writer_position);
		state.symbol_manager.SetLastGlobalLabelDirect(std::move(fixup.last_global_label));
		state.source_line = fixup.source_line;
		state.location = std::move(fixup.location);
		*state.options = fixup.options;

		/* Re-process statement. */
		ProcessStatement(state, fixup.statement, fixup.label);

		/* We're done with this statement: delete it. */
		state.fixup_list.pop_front();
	}

	/* Produce asm68k symbol file, if requested. */
	if (symbol_stream != nullptr)
	{
		Utilities::IO::StreamBinaryWriter symbol_writer(*symbol_stream);

		/* Some kind of header. */
		symbol_writer.WriteU8('M');
		symbol_writer.WriteU8('N');
		symbol_writer.WriteU8('D');
		symbol_writer.WriteU8(1);
		symbol_writer.WriteU8(0);
		symbol_writer.WriteU8(0);
		symbol_writer.WriteU8(0);
		symbol_writer.WriteU8(0);

		state.symbol_manager.FilterDictionary(
			[&](const SymbolDictionary::Entry &entry, const std::string_view &identifier) -> bool
			{
				/* The symbol file only contains labels. */
				/* Labels longer than 0xFF characters are silently ignored. */
				/* Local labels are also silently ignored (unless 'v+' is specified). */
				const SymbolLabel* const symbol_label = std::get_if<SymbolLabel>(&entry);

				if (symbol_label != nullptr)
				{
					const auto local_symbol_position = identifier.find_last_of(state.options->local_signifier);
					const std::string_view label = identifier.substr(local_symbol_position == identifier.npos ? 0 : local_symbol_position);

					const bool is_local_label = label.length() != identifier.length();

					if (label.length() < 0x100 && (!is_local_label || state.options->output_local_labels_to_sym_file))
					{
						/* Output the address of the label. */
						symbol_writer.WriteU8(symbol_label->value);

						/* I have no idea what this means. */
						symbol_writer.WriteU8(is_local_label ? 6 : 2);

						/* Output the length of the label. */
						symbol_writer.WriteU8(label.length());

						/* Output the label itself. */
						*symbol_stream << label;
					}
				}

				return true;
			}
		);
	}

	return !state.error_manager.GetErrorEncountered();
}

bool ClownAssembler::Assemble(
	std::istream &input_stream,
	std::ostream &output_stream,
	std::ostream &error_stream,
	std::ostream* const listing_stream,
	std::ostream* const symbol_stream,
	const std::string_view &input_file_path,
	Settings settings,
	const DefinitionCallback definition_callback,
	const void* const user_data)
{
	std::stringstream object_stream;

	if (!AssembleToObjectFile(input_stream, object_stream, error_stream, listing_stream, symbol_stream, input_file_path, std::move(settings), definition_callback, user_data))
		return false;

	if (!ConvertObjectFileToFlatBinary(object_stream, output_stream, error_stream))
		return false;

	return true;
}
