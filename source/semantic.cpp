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
#include <charconv>
#include <deque>
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

#include "clowncommon/clowncommon.h"

#include "dictionary.h"
#include "error.h"
#include "expression-processor.h"
#include "global.h"
#include "instruction-processor.h"
#include "listing.h"
#include "macro-state.h"
#include "object-writer.h"
#include "p2bin.h"
#include "parser.h"
#include "source-line-reader.h"
#include "substitute.h"
#include "symbol-manager.h"
#include "utilities/io.h"
#include "utilities/path.h"
#include "utilities/string.h"

// Hide in an anonymous namespace to avoid external visibility, improving optimisation.
namespace
{
	class SemanticState
	{
	private:
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

		Global global;

		/* Preprocessor-specific global stuff. */
		SourceLineReader source_line_reader;
		ListingWriter listing_writer;
		ObjectWriter object_writer;
		InstructionProcessor instruction_processor = {global, object_writer};
		SymbolManager symbol_manager = {global, macro.dictionary};
		Parser parser = {global.error_manager};
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
			std::string name;
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

		std::variant<ModeNormal, ModeRept, ModeMacro, ModeWhile> mode = ModeNormal{};

		void TerminateIf();
		void TerminateRept(ModeRept &rept_state);
		void TerminateMacro(ModeMacro &macro_state);
		void TerminateWhile(ModeWhile &while_state);
		void PushSubstitute(const std::string_view &identifier, const std::string_view &value);
		void PushMacroArgumentSubstitutions();
		void OutputDcValue(Size size, unsigned long value);
		void PerformEven();
		bool ShouldPerformAutomaticEven(Size size);
		void ProcessDc(StatementDc &dc);
		void ProcessDcb(StatementDcb &dcb);
		void ProcessDs(StatementDs &ds);
		void ProcessInclude(const StatementInclude &include);
		void ProcessIncbin(StatementIncbin &incbin);
		void ProcessRept(StatementRept &rept);
		void ProcessMacro(StatementMacro &macro, const std::string_view &label, bool is_short);
		void ProcessIf(Expression &expression);
		void ProcessStatement(Statement &statement, const std::string_view &label);
		void ParseLine(const std::string_view &label, const std::string_view &directive_and_operands);
		void PerformSubstitutionsOnSubString(std::string &string, std::string_view &view_to_search, bool allow_implicit_matches);
		void PerformSubstitutions(std::string &string, bool allow_implicit_matches);
		void PerformSubstitutionsExcludingQuotedStrings(std::string &source_line, std::size_t starting_position, bool allow_implicit_matches);
		void SubstituteAndParseLine(const std::string_view &label, const std::string_view &directive_and_operands, const std::string_view &directive);
		void InvokeMacro(const Macro &macro, const std::string_view &label, std::string_view source_line);
		void AssembleLine(const std::string_view &source_line_raw, const bool write_line_to_listing_file);
		void AssembleFile(SourceLineReader &source_line_reader);

	public:
		SemanticState(
			std::istream &input_stream,
			std::ostream &output_stream,
			std::ostream &error_stream,
			std::ostream* const listing_stream,
			const std::string_view &input_file_path,
			Options default_options
		)
			: global(std::move(default_options), error_stream, location, source_line, symbol_manager, macro)
			, source_line_reader(input_stream)
			, listing_writer(listing_stream, global.doing_fixup_pass)
			, object_writer(global, output_stream, listing_writer)
		{
			/* For error messages, we want to maintain a list of filenames and line
			   numbers that describe the location and nesting of the erroneous
			   line. This is the first entry in that list. */
			location.emplace_front(input_file_path, 0);
		}

		bool AssembleToObjectFile(
			std::ostream* const symbol_stream,
			const ClownAssembler::DefinitionCallback &definition_callback);
	};
}

/* Other stuff */

void SemanticState::TerminateIf()
{
	if (false_if_level == current_if_level || false_if_level == 0)
	{
		true_already_found = true;
		false_if_level = 0;
	}

	--current_if_level;
}

void SemanticState::TerminateRept(SemanticState::ModeRept &rept_state)
{
	/* Back-up some state into local variables, in case a nested REPT statement clobbers it. */
	auto rept_state_copy = std::move(rept_state);

	/* Exit REPT mode. */
	mode.emplace<SemanticState::ModeNormal>();

	while (rept_state_copy.repetitions-- != 0)
	{
		/* Rewind back to the line number of the start of the REPT. */
		location.front().line_number = rept_state_copy.line_number;

		/* Process the REPT's nested statements. */
		for (const auto &source_line : rept_state_copy.source_line_list)
			AssembleLine(source_line, global.options->expand_all_macros);
	}

	/* Increment past the ENDR line number. */
	++location.front().line_number;
}

void SemanticState::TerminateMacro(SemanticState::ModeMacro &macro_state)
{
	const std::string_view &identifier = macro_state.name;

	if (!identifier.empty())
	{
		if (macro.ExpansionInProgress())
		{
			/* See comment in the StatementPurge handler after the check for the same condition for more details. */
			global.error_manager.Error("Cannot define macro '{}' during macro expansion.", identifier);
		}
		else
		{
			/* Add the macro to the symbol table. */
			symbol_manager.AddSymbol(identifier, SymbolMacro{std::move(macro_state.metadata)});
		}
	}

	/* Exit macro mode. */
	mode.emplace<SemanticState::ModeNormal>();
}

void SemanticState::TerminateWhile(SemanticState::ModeWhile &while_state)
{
	const std::string_view old_source_line = source_line;

	/* Back-up some state into local variables, in case a nested WHILE statement clobbers it. */
	auto while_state_copy = std::move(while_state);

	/* Exit WHILE mode. */
	mode.emplace<SemanticState::ModeNormal>();

	listing_writer.SuppressLine();

	for (;;)
	{
		source_line = while_state_copy.source_line;

		/* We absolutely do not want to fold (cache) this expression, because
		   while-loops depend on variables mutating from previous values;
		   without this, while-loops will never terminate. */
		const auto &value = global.expression_processor.ResolveExpressionWithoutFolding(while_state_copy.expression, "Expression must be evaluable on the first pass.").value_or(0);

		if (value == 0)
			break;

		/* Rewind back to the line number of the start of the WHILE. */
		location.front().line_number = while_state_copy.line_number;

		/* Process the WHILE's nested statements. */
		for (const auto &source_line : while_state_copy.source_line_list)
			AssembleLine(source_line, false);
	}

	/* Increment past the ENDW line number. */
	++location.front().line_number;

	source_line = old_source_line;
}

void SemanticState::PushSubstitute(const std::string_view &identifier, const std::string_view &value)
{
	const auto &[it, dictionary] = symbol_manager.LookupSymbolAndCreateIfNotExist<SymbolStringConstant>(identifier);

	if (it != dictionary.end())
	{
		CC_Visit(
			it->second,
			[&](SymbolNone&)
			{
				auto &symbol = it->second.emplace<SymbolStringConstant>();
				symbol.value = value;
				auto &substitute = macro.dictionary != nullptr && &dictionary == &*macro.dictionary ? macro.expansion->substitutions : substitutions;
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
			}
		);
	}
}

void SemanticState::PushMacroArgumentSubstitutions()
{
	const std::string_view blank_argument;

	for (std::size_t argument_index = 0; argument_index < macro.expansion->metadata->parameter_names.size(); ++argument_index)
	{
		const std::string_view &parameter = macro.expansion->metadata->parameter_names[argument_index];
		const std::string_view &argument = argument_index >= macro.expansion->total_arguments ? blank_argument : macro.expansion->argument_list[argument_index];

		SymbolDictionary::mapped_type &dictionary_entry = (*macro.dictionary)[parameter];

		/* TODO: De-duplicate this with the 'PushSubstitute' function. */
		CC_Visit(
			dictionary_entry,
			[&](SymbolNone&)
			{
				auto &symbol = dictionary_entry.emplace<SymbolStringConstant>();
				symbol.value = argument;
				macro.expansion->substitutions.PushSubstitute(parameter, symbol.value);
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
				global.error_manager.Error("Attempted to redefine symbol '{}' as a string constant.", parameter);
			}
		);
	}
}

void SemanticState::OutputDcValue(const Size size, unsigned long value)
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
				global.error_manager.Error("Value cannot be higher than $FF or lower than -$100, but was ${:X}.", value);

			bytes_to_write = 1;

			break;

		case SIZE_WORD:
			if (value > 0xFFFF && value < 0xFFFF0000)
				global.error_manager.Error("Value cannot be higher than $FFFF or lower than -$10000, but was ${:X}.", value);

			bytes_to_write = 2;

			break;

		case SIZE_LONGWORD:
			bytes_to_write = 4;
			break;
	}

	object_writer.WriteIntegerBE(value, bytes_to_write);
}

void SemanticState::PerformEven()
{
	/* Pad to the nearest even address. */
	if ((global.program_counter & 1) != 0)
		object_writer.WriteU8(0);
}

bool SemanticState::ShouldPerformAutomaticEven(const Size size)
{
	return global.options->automatic_even && size != SIZE_BYTE;
}

void SemanticState::ProcessDc(StatementDc &dc)
{
	if (ShouldPerformAutomaticEven(dc.size))
		PerformEven();

	for (auto &expression : dc.values)
	{
		const auto expression_string = std::get_if<ExpressionString>(&expression);

		if (expression_string != nullptr && (dc.size == SIZE_BYTE || dc.size == SIZE_SHORT))
		{
			/* If this is a 'dc.b', and the value is a raw string, then output each character as a single byte. */
			/* This allows 'dc.b' to be used to embed strings into the output. */
			for (const char character : expression_string->string)
				OutputDcValue(dc.size, character);
		}
		else
		{
			/* Update the program counter symbol in between values, to keep it up to date. */
			global.expression_processor.program_counter_of_expression = global.program_counter;

			OutputDcValue(dc.size, global.expression_processor.ResolveExpression(expression).value_or(0));
		}
	}
}

void SemanticState::ProcessDcb(StatementDcb &dcb)
{
	if (ShouldPerformAutomaticEven(dcb.size))
		PerformEven();

	const auto &repetitions = global.expression_processor.ResolveExpression(dcb.repetitions, "Repetition value must be evaluable on first pass.").value_or(0);
	const auto &value = global.expression_processor.ResolveExpression(dcb.value).value_or(0);

	for (unsigned long i = 0; i < repetitions; ++i)
		OutputDcValue(dcb.size, value);
}

void SemanticState::ProcessDs(StatementDs &ds)
{
	if (ShouldPerformAutomaticEven(ds.size))
		PerformEven();

	const auto &length = global.expression_processor.ResolveExpression(ds.length, "Repetition value must be evaluable on first pass.").value_or(0);

	for (unsigned long i = 0; i < length; ++i)
		OutputDcValue(ds.size, 0);
}

void SemanticState::ProcessInclude(const StatementInclude &include)
{
	std::ifstream input_stream(Path::FromDOSPath(include.path));

	if (!input_stream.is_open())
	{
		global.error_manager.Error("File '{}' could not be opened.", include.path);
		return;
	}

	/* Add file path and line number to the location list. */
	location.emplace_front(include.path, 0);

	SourceLineReader source_line_reader(input_stream);
	AssembleFile(source_line_reader);

	location.pop_front();
}

void SemanticState::ProcessIncbin(StatementIncbin &incbin)
{
	std::ifstream input_file(Path::FromDOSPath(incbin.path), input_file.binary);

	if (!input_file.is_open())
	{
		global.error_manager.Error("File '{}' could not be opened.", incbin.path);
		return;
	}

	const auto &value = global.expression_processor.ResolveExpression(incbin.start, "Start value must be evaluable on the first pass.").value_or(0);

	input_file.seekg(value);

	if (input_file.fail())
		global.error_manager.Error("Start value is not a valid location in the file.");

	if (incbin.has_length)
	{
		const auto &length = global.expression_processor.ResolveExpression(incbin.length, "Length value must be evaluable on the first pass.").value_or(0);

		for (unsigned long i = 0; i < length; ++i)
		{
			const int character = input_file.get();

			if (input_file.gcount() == 0)
			{
				global.error_manager.Error("Length value is larger than the remaining number of bytes in the file.");
				break;
			}

			object_writer.WriteU8WithoutListing(character);
		}
	}
	else
	{
		for (;;)
		{
			const int character = input_file.get();

			if (input_file.gcount() == 0)
				break;

			object_writer.WriteU8WithoutListing(character);
		}
	}
}

void SemanticState::ProcessRept(StatementRept &rept)
{
	/* Enter REPT mode. */
	auto &rept_mode = mode.emplace<SemanticState::ModeRept>();

	rept_mode.repetitions = global.expression_processor.ResolveExpression(rept.repetitions, "Repetition value must be evaluable on the first pass.").value_or(1);
	rept_mode.line_number = location.front().line_number;
	rept_mode.nesting = 0;
}

void SemanticState::ProcessMacro(StatementMacro &macro, const std::string_view &label, bool is_short)
{
	/* Enter MACRO mode. */
	mode.emplace<SemanticState::ModeMacro>(
		std::string(label),
		Macro{
			is_short,
			macro.uses_label,
			location.front(),
			std::move(macro.parameter_names)
		}
	);
}

void SemanticState::ProcessIf(Expression &expression)
{
	++current_if_level;

	const auto &value = global.expression_processor.ResolveExpression(expression, "Condition must be evaluable on the first pass.").value_or(1);

	/* If this condition is false, then mark this as the false if-level. */
	if (value == 0)
		false_if_level = current_if_level;

	true_already_found = value != 0;
}

void SemanticState::ProcessStatement(Statement &statement, const std::string_view &label)
{
	/* Bizarrely, depending on the context, the program counter can be one
	   of two values, so we need to maintain two separate copies of it. */
	global.expression_processor.program_counter_of_statement = global.program_counter;
	global.expression_processor.program_counter_of_expression = global.program_counter;

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
		if (!label.empty() && !global.doing_fixup_pass)
		{
			/* Handle the label here, instead of passing it onto a later function. */
			symbol_manager.SetLastGlobalLabel(label);
			symbol_manager.AddSymbol(label, SymbolLabel{global.program_counter});
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
			global.error_manager.Error("There cannot be a label on this type of statement.");
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
			global.error_manager.Error("This type of statement must have a label.");
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

	CC_Visit(
		statement,
		[&](StatementEmpty&) {},
		[&](StatementInstruction &statement)
		{
			instruction_processor.Process(statement);
		},
		[&](StatementDc &statement)
		{
			ProcessDc(statement);
		},
		[&](StatementDcb &statement)
		{
			ProcessDcb(statement);
		},
		[&](StatementDs &statement)
		{
			ProcessDs(statement);
		},
		[&](StatementInclude &statement)
		{
			ProcessInclude(statement);
		},
		[&](StatementIncbin &statement)
		{
			ProcessIncbin(statement);
		},
		[&](StatementRept &statement)
		{
			ProcessRept(statement);
		},
		[&](StatementEndr&)
		{
			/* Exit REPT mode. */
			const auto &rept_mode = std::get_if<SemanticState::ModeRept>(&mode);

			if (rept_mode == nullptr)
				global.error_manager.Error("This stray ENDR has no preceeding REPT.");
			else
				TerminateRept(*rept_mode);
		},
		[&](StatementMacro &statement)
		{
			ProcessMacro(statement, label, false);
		},
		[&](StatementMacros &statement)
		{
			ProcessMacro(statement.macro, label, true);
		},
		[&](StatementEndm&)
		{
			/* Exit MACRO mode. */
			const auto &macro_mode = std::get_if<SemanticState::ModeMacro>(&mode);

			if (macro_mode == nullptr)
				global.error_manager.Error("This stray ENDM has no preceeding MACRO.");
			else if (macro_mode->metadata.is_short)
				global.error_manager.Error("ENDM cannot be used in a short macro.");
			else
				TerminateMacro(*macro_mode);
		},
		[&](StatementEqu &statement)
		{
			if (global.options->equ_set_descope_local_labels)
				symbol_manager.SetLastGlobalLabel(label);

			const auto &value_optional = global.expression_processor.ResolveExpression(statement.expression);

			/* Hack: We ABSOLUTELY DO NOT want this line adding to the fix-up list!!! */
			symbol_manager.fixup_needed = false;

			if (!value_optional.has_value())
			{
				symbol_manager.AddSymbol(label, SymbolExpressionConstant{std::move(statement.expression)});
				// TODO: Why does removing this result in a crash?
				statement.expression.emplace<ExpressionNumber>(0);
				return;
			}

			const auto &value = *value_optional;

			symbol_manager.AddSymbol(label, SymbolConstant{value});
			listing_writer.WriteIdentifierValue(value);
		},
		[&](StatementEqusString &statement)
		{
			PushSubstitute(label, statement.string);
		},
		[&](StatementEqusIdentifier &statement)
		{
			const auto &[it, dictionary] = symbol_manager.LookupSymbol(statement.string);

			const SymbolStringConstant* const symbol_string_constant = it == dictionary.end() ? nullptr : std::get_if<SymbolStringConstant>(&it->second);

			if (symbol_string_constant == nullptr)
				global.error_manager.Error("String constant can only be defined as a string or another string constant.");
			else
				PushSubstitute(label, symbol_string_constant->value);
		},
		[&](StatementSubstr &statement)
		{
			auto start = global.expression_processor.ResolveExpression(statement.start, "Start must be evaluable on the first pass.").value_or(0);
			auto end = global.expression_processor.ResolveExpression(statement.end, "End must be evaluable on the first pass.").value_or(statement.string.length());

			if (start == 0)
			{
				global.error_manager.Error("Start cannot be zero.");
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

			PushSubstitute(label, String::Subview(statement.string, start - 1, end - start + 1));
		},
		[&](StatementSet &statement)
		{
			if (global.options->equ_set_descope_local_labels)
				symbol_manager.SetLastGlobalLabel(label);

			const auto &value = global.expression_processor.ResolveExpression(statement.expression, "Value must be evaluable on the first pass.").value_or(0);

			symbol_manager.AddSymbol(label, SymbolVariable{value});
			listing_writer.WriteIdentifierValue(value);
		},
		[&](StatementIf &statement)
		{
			ProcessIf(statement.expression);
		},
		[&](StatementElseif &statement)
		{
			if (current_if_level == 0)
			{
				global.error_manager.Error("This stray ELSEIF has no preceeding IF.");
				return;
			}

			if (false_if_level == current_if_level || false_if_level == 0)
			{
				const auto &value = global.expression_processor.ResolveExpression(statement.expression, "Condition must be evaluable on the first pass.").value_or(1);

				/* If this condition is false, then mark this as the false if-level. */
				if (true_already_found || value == 0)
				{
					false_if_level = current_if_level;
				}
				else
				{
					false_if_level = 0;
					true_already_found = true;
				}
			}
		},
		[&](StatementElse&)
		{
			if (current_if_level == 0)
			{
				global.error_manager.Error("This stray ELSE has no preceeding IF.");
				return;
			}

			if (false_if_level == current_if_level || false_if_level == 0)
			{
				if (true_already_found)
				{
					false_if_level = current_if_level;
				}
				else
				{
					/* If this is the false if-level, then it isn't anymore. */
					/* Likewise, if there is no false if-level, then there is now. */
					if (false_if_level == current_if_level)
						false_if_level = 0;
					else /*if (false_if_level == 0)*/
						false_if_level = current_if_level;
				}
			}
		},
		[&](StatementEndc&)
		{
			if (current_if_level == 0)
				global.error_manager.Error("This stray ENDC/ENDIF has no preceeding IF.");
			else
				TerminateIf();
		},
		[&](StatementWhile &statement)
		{
			/* Enter WHILE mode. */
			auto &while_mode = mode.emplace<SemanticState::ModeWhile>();

			/* Dummy-out the statement's expression, since the state now owns it, and we don't
			   want its sub-expressions to be automatically freed when the statement is freed. */
			while_mode.expression.emplace<ExpressionNumber>(0);
			std::swap(while_mode.expression, statement.expression);
			while_mode.source_line = source_line;

			while_mode.line_number = location.front().line_number;
			while_mode.nesting = 0;
		},
		[&](StatementEndw&)
		{
			/* Exit WHILE mode. */
			const auto &while_mode = std::get_if<SemanticState::ModeWhile>(&mode);

			if (while_mode == nullptr)
				global.error_manager.Error("This stray ENDW has no preceeding WHILE.");
			else
				TerminateWhile(*while_mode);
		},
		[&](StatementEven&)
		{
			PerformEven();
		},
		[&](StatementCnop &statement)
		{
			const auto &offset = global.expression_processor.ResolveExpression(statement.offset, "Offset must be evaluable on the first pass.").value_or(0);
			const auto &size_boundary = global.expression_processor.ResolveExpression(statement.size_boundary, "Size boundary must be evaluable on the first pass.").value_or(1);

			if (size_boundary == 0)
			{
				/* I don't know if there's a way to make this have meaningfully useful semantics. asm68k errors on this, so for now we will too. */
				global.error_manager.Error("Size boundary cannot be zero.");
				return;
			}

			const unsigned long position_within_boundary = global.program_counter % size_boundary;

			unsigned long target = global.program_counter - position_within_boundary + offset;

			if (position_within_boundary > offset)
				target += size_boundary;

			/* Pad to the desired offset. */
			while (global.program_counter != target)
				object_writer.WriteU8(0);
		},
		[&](StatementInform &statement)
		{
			/* Definitely an 'INFORM'... */
			const auto &severity = global.expression_processor.ResolveExpression(statement.severity, "Severity must be evaluable on the first pass.").value_or(0);

			/* TODO - Everything... */
			switch (severity)
			{
				case 0:
					global.error_manager.Information("INFORM: '{}'", statement.message);
					break;

				case 1:
					global.error_manager.Warning("INFORM: '{}'", statement.message);
					break;

				case 2:
				case 3: /* TODO: Halt assembly. */
					global.error_manager.Error("INFORM: '{}'", statement.message);
					break;
			}
		},
		[&](StatementFail&)
		{
			/* TODO: Halt assembly. */
			global.error_manager.Error("Assembly Failed");
		},
		[&](StatementEnd&)
		{
			end = true;
		},
		[&](StatementRs &statement)
		{
			if (ShouldPerformAutomaticEven(statement.size))
				if ((global.expression_processor.rs & 1) != 0)
					++global.expression_processor.rs;

			auto length = global.expression_processor.ResolveExpression(statement.length, "Length must be evaluable on the first pass.").value_or(0);

			/* Add label to symbol table. */
			if (!label.empty())
			{
				symbol_manager.AddSymbol(label, SymbolConstant{global.expression_processor.rs});
				listing_writer.WriteIdentifierValue(global.expression_processor.rs);
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

			global.expression_processor.rs += length;
		},
		[&](StatementRsset &statement)
		{
			/* Set '__rs' to value. */
			global.expression_processor.rs = global.expression_processor.ResolveExpression(statement.expression, "Value must be evaluable on the first pass.").value_or(global.expression_processor.rs);
		},
		[&](StatementRsreset&)
		{
			/* Set '__rs' to 0. */
			global.expression_processor.rs = 0;
		},
		[&](StatementObj &statement)
		{
			if (obj_delta.has_value())
			{
				global.error_manager.Error("OBJ statements cannot be nested.");
				return;
			}

			const auto &value = global.expression_processor.ResolveExpression(statement.expression, "Value must be evaluable on the first pass.").value_or(global.program_counter);

			obj_delta = value - global.program_counter;
			global.program_counter += *obj_delta;
		},
		[&](StatementObjend&)
		{
			if (!obj_delta.has_value())
			{
				global.error_manager.Error("This stray OBJEND has no preceeding OBJ.");
				return;
			}

			global.program_counter -= *obj_delta;
			obj_delta = std::nullopt;
		},
		[&](StatementOrg &statement)
		{
			const auto &value = global.expression_processor.ResolveExpression(statement.expression, "Value must be evaluable on the first pass.").value_or(global.program_counter);

			if (value > 0xFFFFFFFF)
			{
				global.error_manager.Error("Value cannot be larger than 0xFFFFFFFF (was 0x{:X}).", value);
				return;
			}

			object_writer.BeginNewSegment(value);

			global.program_counter = value;
		},
		[&](StatementPusho&)
		{
			global.options.Push();
		},
		[&](StatementPopo&)
		{
			global.options.Pop();
		},
		[&](StatementOpt &statement)
		{
			for (const auto &option : statement.options)
			{
				/* TODO: A lot of these are unimplemented... */
				if (CaseInsensitive::CompareStrings(option, "ae+"))
					global.options->automatic_even = true;
				else if (CaseInsensitive::CompareStrings(option, "ae-"))
					global.options->automatic_even = false;
				else if (CaseInsensitive::CompareStrings(option, "an+"))
					;
				else if (CaseInsensitive::CompareStrings(option, "an-"))
					;
				else if (CaseInsensitive::CompareStrings(option, "c+"))
					global.options->case_insensitive = false;
				else if (CaseInsensitive::CompareStrings(option, "c-"))
					global.options->case_insensitive = true;
				else if (CaseInsensitive::CompareStrings(option, "d+"))
					global.options->equ_set_descope_local_labels = true;
				else if (CaseInsensitive::CompareStrings(option, "d-"))
					global.options->equ_set_descope_local_labels = false;
				else if (CaseInsensitive::CompareStrings(option, "e+"))
					;
				else if (CaseInsensitive::CompareStrings(option, "e-"))
					;
				else if (CaseInsensitive::CompareStrings(option, "l+"))
					global.options->local_signifier = '.';
				else if (CaseInsensitive::CompareStrings(option, "l-"))
					global.options->local_signifier = '@';
				else if (option.length() == 2 && (option[0] == 'l' || option[0] == 'L'))
					global.options->local_signifier = option[1];
				/*else if (CaseInsensitive::CompareStrings(option, "s+"))
					;*/
				else if (CaseInsensitive::CompareStrings(option, "s-"))
					;
				else if (CaseInsensitive::CompareStrings(option, "m+"))
					global.options->expand_all_macros = true;
				else if (CaseInsensitive::CompareStrings(option, "m-"))
					global.options->expand_all_macros = false;
				else if (CaseInsensitive::CompareStrings(option, "oz+"))
					global.options->optimisation.zero_displacement = true;
				else if (CaseInsensitive::CompareStrings(option, "oz-"))
					global.options->optimisation.zero_displacement = false;
				else if (CaseInsensitive::CompareStrings(option, "v+"))
					global.options->output_local_labels_to_sym_file = true;
				else if (CaseInsensitive::CompareStrings(option, "v-"))
					global.options->output_local_labels_to_sym_file = false;
				else if (CaseInsensitive::CompareStrings(option, "w+"))
					global.options->warnings_enabled = true;
				else if (CaseInsensitive::CompareStrings(option, "w-"))
					global.options->warnings_enabled = false;
				else if (CaseInsensitive::CompareStrings(option, "wp+"))
					global.options->pedantic_warnings_enabled = true;
				else if (CaseInsensitive::CompareStrings(option, "wp-"))
					global.options->pedantic_warnings_enabled = false;
				else if (CaseInsensitive::CompareStrings(option, "ws+"))
					;
				else if (CaseInsensitive::CompareStrings(option, "ws-"))
					;
				else
					global.error_manager.Warning("Unrecognised option '{}'.", option);
			}
		},
		[&](StatementPushp &statement)
		{
			string_stack.emplace(std::move(statement.string));
		},
		[&](StatementPopp &statement)
		{
			if (string_stack.empty())
			{
				global.error_manager.Error("POPP used but the string stack was empty.");
				return;
			}

			PushSubstitute(statement.string, string_stack.top());
			string_stack.pop();
		},
		[&](StatementShift&)
		{
			if (!macro.ExpansionInProgress())
			{
				global.error_manager.Error("SHIFT used outside of macro.");
				return;
			}

			if (macro.expansion->total_arguments == 0)
			{
				/* TODO: What if there are already 0 arguments? */
				/* This prevents crashing for now but perhaps it could be handled differently. */
				global.error_manager.Error("SHIFT used but there are no macro arguments to shift.");
				return;
			}

			--macro.expansion->total_arguments;
			for (unsigned int i = 0; i < macro.expansion->total_arguments; ++i)
				macro.expansion->argument_list[i] = std::move(macro.expansion->argument_list[i + 1]);
			macro.expansion->argument_list.pop_back();
			PushMacroArgumentSubstitutions();
		},
		[&](StatementMexit&)
		{
			macro.expansion = std::nullopt;
		},
		[&](StatementLocal &statement)
		{
			if (!macro.ExpansionInProgress())
			{
				global.error_manager.Error("LOCAL used outside of macro.");
				return;
			}

			for (const auto &identifier : statement.identifiers)
				macro.dictionary->emplace(identifier);
		},
		[&](StatementPurge &statement)
		{
			if (macro.ExpansionInProgress())
			{
				/* TODO: I don't know if that could be meaningfully supported in some way but for now this just seems like a
				   "how to crash and burn into UAF/double free" without a bunch of changes to support it, for not much benefit.
				   We don't check for the name of the currently-expanded macro because one could e.g. invoke a, which invokes b,
				   which redefines a, and there is currently no simple wey to check the entire macro expansion stack. */
				global.error_manager.Error("Cannot purge '{}' during macro expansion.", statement.string);
				return;
			}

			const auto &[it, dictionary] = symbol_manager.LookupSymbol(statement.string);

			if (it == dictionary.end())
			{
				global.error_manager.Error("Cannot purge '{}' as it does not exist.", statement.string);
				return;
			}

			if (!std::holds_alternative<SymbolMacro>(it->second))
			{
				global.error_manager.Error("Cannot purge '{}' as it is not a macro.", statement.string);
				return;
			}

			dictionary.erase(it);
		}
	);

	/* Update both copies of the program counter again, so that things like WHILE statements don't use stale values in their expressions. */
	global.expression_processor.program_counter_of_statement = global.program_counter;
	global.expression_processor.program_counter_of_expression = global.program_counter;
}

void SemanticState::ParseLine(const std::string_view &label, const std::string_view &directive_and_operands)
{
	/* This is a normal assembly line. */

	/* Back these up, before they get a chance to be modified by ProcessStatement. */
	const unsigned long starting_program_counter = global.program_counter;
	const auto starting_object_writer_position = object_writer.Tell();

	Statement* const statement = parser.ParseStatement(directive_and_operands);

	if (statement == nullptr)
		return;

	ProcessStatement(*statement, label);

	if (!symbol_manager.fixup_needed)
		return;

	/* If the statement cannot currently be processed because of undefined symbols,
	   add it to the fix-up list so we can try again later. */
	fixup_list.emplace_back(
		/* Backup the statement. */
		std::move(*statement),

		/* Backup some  */
		macro.dictionary,
		starting_program_counter,
		starting_object_writer_position,
		symbol_manager.GetLastGlobalLabel(),
		std::string(source_line),
		std::string(label),
		*global.options,

		/* Clone the location. */
		location
	);

	symbol_manager.fixup_needed = false;
}

#define DIRECTIVE_OR_MACRO_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?!_"
#define LABEL_CHARS DIRECTIVE_OR_MACRO_CHARS ".@"

void SemanticState::PerformSubstitutionsOnSubString(std::string &string, std::string_view &view_to_search, const bool allow_implicit_matches)
{
	const bool case_insensitive = global.options->case_insensitive;

	if (macro.ExpansionInProgress())
		substitutions.ProcessSubString(&macro.expansion->substitutions, string, view_to_search, macro.expansion->custom_search, allow_implicit_matches, case_insensitive);
	else
		substitutions.ProcessSubString(nullptr, string, view_to_search, nullptr, allow_implicit_matches, case_insensitive);
}

void SemanticState::PerformSubstitutions(std::string &string, const bool allow_implicit_matches)
{
	std::string_view view = string;
	PerformSubstitutionsOnSubString(string, view, allow_implicit_matches);
}

static std::optional<std::string_view> FindQuotedStringInSourceLine(const std::string_view &string)
{
	const auto start_position = string.find_first_of("'\"");

	if (start_position == string.npos)
		return std::nullopt;

	const auto end_position = string.find_first_of(string[start_position], start_position + 1);

	if (end_position == string.npos)
		return std::nullopt;

	return String::Subview(string, start_position, end_position - start_position);
}

/* Performs string substitutions only in parts of the string that are not wrapped in quotations. */
/* This is needed to simulate how SN 68k does not expand string constants in strings, only expressions and operands. */
void SemanticState::PerformSubstitutionsExcludingQuotedStrings(std::string &source_line, const std::size_t starting_position, const bool allow_implicit_matches)
{
	std::string_view source_line_subview = String::Subview(source_line, starting_position);

	for (;;)
	{
		/* If no string was found, then pretend there's a string at the end of the line, so that we can repurpose the code below. */
		const auto quoted_string = FindQuotedStringInSourceLine(source_line_subview).value_or(String::Subview(source_line_subview, source_line_subview.length()));

		/* We've found a string! Perform substitutions on the part of the line before it. */

		/* Define the region that we will perform substitutions on. */
		auto view_to_search = String::Subview(source_line_subview, 0, quoted_string.data() - source_line_subview.data());

		/* Perform the substitutions. */
		PerformSubstitutionsOnSubString(source_line, view_to_search, allow_implicit_matches);

		if (quoted_string.empty())
			break;

		/* Move to after the string so that we can continue searching. */
		source_line_subview = String::Subview(source_line_subview, view_to_search.length() + quoted_string.length());
	}
}

void SemanticState::SubstituteAndParseLine(const std::string_view &label, const std::string_view &directive_and_operands, const std::string_view &directive)
{
	std::string modified_directive_and_operands;

	/* Because of the below hack, we have to filter-out directives that do not use expressions or operands,
	   to prevent them from accidentally being substituted. This is needed for the Natsumi Z80 macros. */
	if (CaseInsensitive::CompareStrings(directive, "binclude")
	 || CaseInsensitive::CompareStrings(directive, "equs")
	 || CaseInsensitive::CompareStrings(directive, "incbin")
	 || CaseInsensitive::CompareStrings(directive, "include")
	 || CaseInsensitive::CompareStrings(directive, "macro")
	 || CaseInsensitive::CompareStrings(directive, "macros")
	 || CaseInsensitive::CompareStrings(directive, "opt")
	 || CaseInsensitive::CompareStrings(directive, "popp"))
	{
		ParseLine(label, directive_and_operands);
		return;
	}

	/* Perform implicit (backslash-less) string substitutions on the operands.
	   SN 68k achieves this by expanding string constants in its operand and expression evaluators,
	   which is absolutely insane, so this trick approximates it instead. */
	modified_directive_and_operands = directive_and_operands;
	PerformSubstitutionsExcludingQuotedStrings(modified_directive_and_operands, directive.length(), true);

	ParseLine(label, modified_directive_and_operands);
}

void SemanticState::InvokeMacro(const Macro &macro_metadata, const std::string_view &label, std::string_view source_line)
{
	if (macro_depth >= 0x100)
	{
		global.error_manager.Error("Macro expansion depth exceeded.");
		return;
	}

	++macro_depth;

	const auto starting_if_level = current_if_level;

	MacroState previous_macro_state = std::move(macro);

	/* TODO: We should not be caching 'case_insensitive'; the OPT directive can change this at any time! */
	macro.dictionary = std::make_shared<SymbolDictionary>(global.options->case_insensitive);

	macro.expansion.emplace(macro_metadata);

	++current_macro_invocation;

	listing_writer.SuppressLine();

	String::RemoveLeading(source_line, DIRECTIVE_OR_MACRO_CHARS);

	if (!label.empty())
	{
		symbol_manager.SetLastGlobalLabel(label);

		if (!macro_metadata.uses_label)
			symbol_manager.AddSymbol(label, SymbolLabel{global.program_counter});
	}

	std::string_view size;

	/* Extract and store the macro size specifier, if one exists. */
	if (!source_line.empty() && source_line.front() == '.')
	{
		/* Skip the '.' character. */
		source_line.remove_prefix(1);

		size = String::ExtractLeading(source_line, DIRECTIVE_OR_MACRO_CHARS);

		/* Advance past the size specifier. */
		source_line.remove_prefix(size.length());
	}

	/* Extract combined arguments string. */
	std::string_view arguments = String::Subview(source_line, 0, source_line.find(';'));

	/* Remove surrounding whitespace. */
	String::RemoveLeadingAndTrailing(arguments, " \t");

	const std::string unique_suffix = std::format("_{}", current_macro_invocation);
	std::string symbol_value_string;

	macro.expansion->custom_search = [this, &size, &arguments, &label, &unique_suffix, &symbol_value_string](const std::string_view &view_to_search, std::size_t starting_position, [[maybe_unused]] bool case_insensitive, std::size_t &found_position, std::size_t &found_length) -> std::optional<std::string_view>
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
					const std::size_t identifier_length = String::GetLeadingLength(view_to_search, DIRECTIVE_OR_MACRO_CHARS, identifier_position_start);
					const std::size_t identifier_position_end = identifier_position_start + identifier_length;

					const std::string_view identifier = String::Subview(view_to_search, identifier_position_start, identifier_length);

					found_length += identifier.length();

					/* Absorb trailing backslash. */
					if (identifier_position_end < view_to_search.length() && view_to_search[identifier_position_end] == '\\')
						++found_length;

					const auto &value = symbol_manager.GetSymbolInteger(identifier, true).value_or(0);

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

					if (parameter_index >= macro.expansion->total_arguments)
						return "";

					return macro.expansion->argument_list[parameter_index];
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
					std::string_view argument = String::Subview(arguments, argument_string_start, i - argument_string_start);
					argument_string_start = i + 1;

					/* Remove surrounding whitespace. */
					String::RemoveLeadingAndTrailing(argument, " \t");

					/* Do not bother pushing an empty first argument if it is the only argument, as that just means there are no arguments. */
					if (argument.empty() && at_the_end && macro.expansion->total_arguments == 0)
						break;

					/* Add to argument list. */
					macro.expansion->argument_list.emplace_back(argument);

					if (!argument.empty() && argument.front() != '"' && argument.front() != '\'' && global.options->case_insensitive)
						String::ToLower(macro.expansion->argument_list[macro.expansion->total_arguments]);

					++macro.expansion->total_arguments;
					++macro.expansion->total_arguments_at_start;
				}

				break;
		}
	}

	/* Add identifier substitute. */
	PushMacroArgumentSubstitutions();

	/* Finally, invoke the macro. */
	const bool previous_listing_macro = listing_writer.listing_macro;

	/* Flush line before changing this so that the macro line is not given the 'M' label. */
	listing_writer.EndLine(source_line);
	listing_writer.listing_macro = true;

	/* Push a new location (this macro).*/
	location.emplace_front(macro_metadata.location);

	/* Iterate over each line of the macro, sending it to be processed. */
	for (const auto &source_line : macro_metadata.source_line_list)
	{
		AssembleLine(source_line, global.options->expand_all_macros);

		/* Detect 'MEXIT'. */
		if (!macro.ExpansionInProgress())
			break;
	}

	if (!macro_metadata.is_short)
	{
		/* 'MEXIT' may have ended us midway through an if-statement, so unwind to the original if-level here. */
		while (current_if_level != starting_if_level)
			TerminateIf();
	}

	/* Pop location. */
	location.pop_front();

	listing_writer.listing_macro = previous_listing_macro;

	macro = std::move(previous_macro_state);

	--macro_depth;
}

void SemanticState::AssembleLine(const std::string_view &source_line_raw, const bool write_line_to_listing_file)
{
	const std::string_view old_source_line = source_line;

	/* Assert a few preconditions to ensure we're in a non-insane  */
	assert(macro.ExpansionInProgress() == (macro_depth > 0));

	if (write_line_to_listing_file)
	{
		listing_writer.EndLine(source_line);
		listing_writer.BeginLine(global.program_counter);
	}

	/* Expand string constants and macro parameters only if we're actually parsing code;
	   it is important that the input of REPT and WHILE is unaltered, so that each iteration
	   can have unique expansions! */
	std::string source_line_expanded;

	if (std::holds_alternative<SemanticState::ModeNormal>(mode))
	{
		source_line_expanded = source_line_raw;
		PerformSubstitutions(source_line_expanded, false);
		source_line = source_line_expanded;
	}
	else
	{
		source_line = source_line_raw;
	}

	++location.front().line_number;

	if (!source_line.empty() && source_line.front() == '*')
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
		std::string_view source_line_subview = source_line;

		/* Let's begin by parsing the label (if there is one). */
		std::string_view label = source_line_subview;

		/* Determine the length of the label. */
		label = String::ExtractLeading(label, LABEL_CHARS);

		if (!label.empty())
		{
			/* We've found a label! */
		}
		else
		{
			/* Skip whitespace. */
			String::RemoveLeading(source_line_subview, " \t");

			/* Maybe the label has some whitespace before it: check again. */
			label = source_line_subview;

			/* Determine the length of the label again. */
			const auto label_length = String::GetLeadingLength(label, LABEL_CHARS);

			if (!label.empty())
			{
				/* Since we've encountered whitespace, it's ambiguous whether this is a label or a statement.
				   To figure it out, we have to rely on the label having a ':' after it. */
				if (label_length < label.length() && label[label_length] == ':')
				{
					/* We've found the label. */
					label = String::Subview(label, 0, label_length);
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
		String::RemoveLeading(source_line_subview, " \t");

		/* Next let's process the directive. If it's an if-statement or a macro
		   invocation, then we'll need to handle it manually. If not, then we
		   can pass it to Flex and Bison. */

		const std::string_view directive_and_operands = source_line_subview;
		const std::string_view directive = String::ExtractLeading(source_line_subview, DIRECTIVE_OR_MACRO_CHARS);

		/* This is either a directive, or a macro. */
		CC_Visit(
			mode,
			[&](SemanticState::ModeNormal&)
			{
				/* If we are in the false part of an if-statement, then manually parse the
				   source code until we encounter an IF, ELSEIF, ELSE, ENDC, or ENDIF.
				   The reason for this is that the false part of an if-statement may contain
				   invalid code and we do not want it to cause errors.
				   This can get pretty complicated because we need to account for nesting. */
				if (false_if_level != 0)
				{
					/* TODO - Detect code after the keyword and error if any is found. */
					if (CaseInsensitive::CompareStrings(directive, "if"))
					{
						/* If-statements that are nested within the false part of another if-statement
						   are themselves false, so create a false if-statement here and process it. */
						++current_if_level;
					}
					else if (CaseInsensitive::CompareStrings(directive, "elseif")
					      || CaseInsensitive::CompareStrings(directive, "else"  )
					      || CaseInsensitive::CompareStrings(directive, "endc"  )
					      || CaseInsensitive::CompareStrings(directive, "endif" ))
					{
						/* These can be processed normally too. */
						SubstituteAndParseLine(label, directive_and_operands, directive);
					}
					else
					{
						/* Drop the line completely, since it's inside the false half of an if statement and should be ignored. */
					}
				}
				else
				{
					/* Look up the directive in the dictionary to see if it's actually a macro. */
					const auto &[it, dictionary] = symbol_manager.LookupSymbol(directive);
					const SymbolMacro* const symbol_macro = it == dictionary.end() ? nullptr : std::get_if<SymbolMacro>(&it->second);
					const Macro* const macro = symbol_macro == nullptr ? nullptr : &symbol_macro->macro;

					if (macro != nullptr /*&& !macro->is_short*/)
					{
						/* Regular macros are only expanded when needed. */
						InvokeMacro(*macro, label, source_line_subview);
					}
					else
					{
						/* This is not a macro invocation: it's just a regular line that can be assembled as-is. */

						/* AS allows directives to be preceeded with a '!' in order to
						   prevent indentically-named macros from overriding them. */
						if (!directive.empty() && directive.front() == '!')
							SubstituteAndParseLine(label, String::Subview(directive_and_operands, 1), String::Subview(directive, 1));
						else
							SubstituteAndParseLine(label, directive_and_operands, directive);
					}
				}
			},
			[&](SemanticState::ModeRept &rept_state)
			{
				/* If this line is an 'ENDR' directive, then exit REPT mode. Otherwise, add the line to the REPT. */
				if (CaseInsensitive::CompareStrings(directive, "endr") && rept_state.nesting-- == 0)
				{
					/* TODO - Detect code after the keyword and error if any is found. */
					ParseLine(label, directive_and_operands);
				}
				else
				{
					if (CaseInsensitive::CompareStrings(directive, "rept"))
						++rept_state.nesting;

					rept_state.source_line_list.emplace_back(source_line);
				}
			},
			[&](SemanticState::ModeMacro &macro_state)
			{
				/* If this line is an 'ENDM' directive, then exit macro mode. Otherwise, add the line to the macro. */
				if (CaseInsensitive::CompareStrings(directive, "endm"))
				{
					/* TODO - Detect code after the keyword and error if any is found. */
					if (macro_state.metadata.is_short)
						global.error_manager.Error("Short macros shouldn't use ENDM.");
					else
						ParseLine(label, directive_and_operands);
				}
				else
				{
					if (macro_state.metadata.is_short)
					{
						/* Short macros automatically terminate after one non-empty statement. */
						const std::size_t first_nonwhitespace_position = String::GetLeadingLength(source_line, " \t");
						const char first_nonwhitespace_character = first_nonwhitespace_position == source_line.length() ? '\0' : source_line[first_nonwhitespace_position];

						if (!label.empty())
							global.error_manager.Error("Short macros shouldn't create labels.");

						if (first_nonwhitespace_character == '\0' || first_nonwhitespace_character == ';')
						{
							/* This line is an empty statement: ignore it. */
						}
						else
						{
							macro_state.metadata.source_line_list.emplace_back(source_line);
							TerminateMacro(macro_state);
						}
					}
					else
					{
						macro_state.metadata.source_line_list.emplace_back(source_line);
					}
				}
			},
			[&](SemanticState::ModeWhile &while_state)
			{
				/* If this line is an 'ENDW' directive, then exit 'WHILE' mode. Otherwise, add the line to the 'WHILE'. */
				if (CaseInsensitive::CompareStrings(directive, "endw") && while_state.nesting-- == 0)
				{
					/* TODO - Detect code after the keyword and error if any is found. */
					ParseLine(label, directive_and_operands);
				}
				else
				{
					if (CaseInsensitive::CompareStrings(directive, "while"))
						++while_state.nesting;

					while_state.source_line_list.emplace_back(source_line);
				}
			}
		);
	}

	if (write_line_to_listing_file)
		listing_writer.EndLine(source_line);

	source_line = old_source_line;
}

void SemanticState::AssembleFile(SourceLineReader &source_line_reader)
{
	/* Read lines one at a time, feeding them to the 'AssembleLine' function. */
	for (std::optional<std::string_view> source_line; (source_line = source_line_reader.Read()).has_value() && !end; )
		AssembleLine(*source_line, true);

	/* If we're not in normal mode when a file ends, then something is wrong. */
	while (!std::holds_alternative<SemanticState::ModeNormal>(mode))
	{
		CC_Visit(
			mode,
			[&](SemanticState::ModeNormal&)
			{
				/* We can't reach this since we just checked for it. */
				CC_UNREACHABLE;
			},
			[&](SemanticState::ModeRept &rept_state)
			{
				global.error_manager.Error("REPT statement beginning at line {} is missing its ENDR.", rept_state.line_number);

				/* Terminate the REPT to hopefully avoid future complications. */
				TerminateRept(rept_state);
			},
			[&](SemanticState::ModeMacro &macro_state)
			{
				global.error_manager.Error("MACRO statement beginning at line {} is missing its ENDM.", macro_state.metadata.location.line_number);

				/* Terminate the macro to hopefully avoid future complications. */
				TerminateMacro(macro_state);
			},
			[&](SemanticState::ModeWhile &while_state)
			{
				global.error_manager.Error("WHILE statement beginning at line {} is missing its ENDW.", while_state.line_number);

				/* Terminate the while-statement to hopefully avoid future complications. */
				TerminateWhile(while_state);
			}
		);
	}
}

bool SemanticState::AssembleToObjectFile(
	std::ostream* const symbol_stream,
	const ClownAssembler::DefinitionCallback &definition_callback)
{
	if (definition_callback != nullptr)
	{
		definition_callback(
			[this](const std::string_view &identifier, const unsigned long value)
			{
				/* TODO: Does asm68k create a constant or a variable? */
				symbol_manager.AddSymbol(identifier, SymbolConstant{value});
			}
		);
	}

	/* Enable Bison debugging if available and requested by the user. */
#if M68KASM_DEBUG
	if (global.options->debug)
		m68kasm_debug = 1;
#endif

	/* Perform first pass of assembly, creating a list of fix-ups. */
	object_writer.WriteHeader();
	AssembleFile(source_line_reader);
	object_writer.WriteFooter();

	/* Perform some sanity checks to make sure we're not somehow in an invalid  */
	if (current_if_level != 0)
		global.error_manager.Error("An IF statement somewhere is missing its ENDC/ENDIF.");
	if (obj_delta.has_value())
		global.error_manager.Error("An OBJ statement somewhere is missing its OBJEND.");

	/* Process the fix-ups: reassemble instructions and reprocess
	   directives that could not be completed in the first pass. */
	global.doing_fixup_pass = true;

	while (!fixup_list.empty())
	{
		/* Pop fix-up from list. */
		FixUp &fixup = fixup_list.front();

		/* Reset some state to how it was at the time the statement was first processed. */
		macro.dictionary = fixup.macro_dictionary;
		global.program_counter = fixup.program_counter;
		object_writer.Seek(fixup.object_writer_position);
		symbol_manager.SetLastGlobalLabelDirect(std::move(fixup.last_global_label));
		source_line = fixup.source_line;
		location = std::move(fixup.location);
		*global.options = fixup.options;

		/* Re-process statement. */
		ProcessStatement(fixup.statement, fixup.label);

		/* We're done with this statement: delete it. */
		fixup_list.pop_front();
	}

	/* Produce asm68k symbol file, if requested. */
	if (symbol_stream != nullptr)
		symbol_manager.WriteSymbolFile(*symbol_stream);

	return !global.error_manager.WasErrorEncountered();
}

static bool AssembleToObjectFile(
	std::istream &input_stream,
	std::ostream &output_stream,
	std::ostream &error_stream,
	std::ostream* const listing_stream,
	std::ostream* const symbol_stream,
	const std::string_view &input_file_path,
	ClownAssembler::Settings initial_options,
	const ClownAssembler::DefinitionCallback &definition_callback)
{
	SemanticState state(
		input_stream,
		output_stream,
		error_stream,
		listing_stream,
		input_file_path,
		std::move(initial_options)
	);

	return state.AssembleToObjectFile(symbol_stream, definition_callback);
}

bool ClownAssembler::Assemble(
	std::istream &input_stream,
	std::ostream &output_stream,
	std::ostream &error_stream,
	std::ostream* const listing_stream,
	std::ostream* const symbol_stream,
	const std::string_view &input_file_path,
	Settings settings,
	const DefinitionCallback &definition_callback)
{
	std::stringstream object_stream;

	if (!AssembleToObjectFile(input_stream, object_stream, error_stream, listing_stream, symbol_stream, input_file_path, std::move(settings), definition_callback))
		return false;

	if (!ConvertObjectFileToFlatBinary(object_stream, output_stream, error_stream))
		return false;

	return true;
}
