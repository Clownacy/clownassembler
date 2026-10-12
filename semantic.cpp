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

#include <assert.h>
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
#include "p2bin.h"
#include "string.h"
#include "substitute.h"
#include "syntactic.h"
#define YY_NO_UNISTD_H
#include "lexical.h"
#include "utilities.h"

using SourceLineList = std::deque<std::string>;

struct Macro
{
	cc_bool is_short, uses_label;
	std::string name;
	IdentifierList parameter_names;
	SourceLineList source_line_list;
};

struct SymbolNone {};

struct SymbolConstant
{
	unsigned long value;
};

struct SymbolVariable
{
	unsigned long value;
};

struct SymbolLabel
{
	unsigned long value;
};

struct SymbolMacro
{
	Macro macro;
};

struct SymbolSpecial
{
	enum class Type
	{
		RS,
		NARG,
	} type;
};

struct SymbolStringConstant
{
	std::string value;
};

struct SymbolExpressionConstant
{
	Expression expression;
};

using SymbolDictionaryEntry = std::variant<SymbolNone, SymbolConstant, SymbolVariable, SymbolLabel, SymbolMacro, SymbolSpecial, SymbolStringConstant, SymbolExpressionConstant>;

using SymbolDictionary = Dictionary<SymbolDictionaryEntry>;

struct Location
{
	std::string file_path; /* TODO: This should really be a `std::string_view`, but fix-ups need deep copies... */
	unsigned long line_number;
};

struct FixUp
{
	Statement statement;
	std::shared_ptr<SymbolDictionary> macro_dictionary;
	unsigned long program_counter;
	std::size_t output_position;
	std::size_t segment_length;
	std::string last_global_label;
	std::string source_line;
	std::deque<Location> location;
	std::string label;
	Options options;
};

struct SemanticState_Macro
{
	const Macro *metadata = nullptr;
	std::shared_ptr<SymbolDictionary> dictionary;
	Substitute substitutions;
	Substitute::CustomSearch custom_search;
	std::vector<std::string> argument_list;
	std::size_t total_arguments = 0, total_arguments_at_start = 0;
	unsigned int starting_if_level = 0;
	unsigned int depth = 0;
	cc_bool active = cc_false;
};

struct SemanticState
{
	class Flex
	{
	private:
		yyscan_t internal;

	public:
		Flex(struct SemanticState &state)
		{
			if (m68kasm_lex_init_extra(&state, &internal) != 0)
				throw std::runtime_error("m68kasm_lex_init failed.");
		}

		~Flex()
		{
			m68kasm_lex_destroy(internal);
		}

		Flex(const Flex &other) = delete;
		Flex& operator=(const Flex &other) = delete;

		yyscan_t& operator*()
		{
			return internal;
		}
	};

	cc_bool success = cc_true;

	/* Callbacks. */
	std::istream *input_stream = nullptr;
	std::ostream *output_stream = nullptr;
	std::ostream *listing_stream = nullptr;
	std::ostream *error_stream = nullptr;

	/* Global stuff. */
	OptionsStack options;
	SymbolDictionary dictionary;
	std::string last_global_label;
	std::deque<Location> location;
	Flex flex_state = {*this};
	Statement statement;
	m68kasm::parser parser = {*flex_state, statement};
	std::vector<char> line_buffer = std::vector<char>(0x100); // Room for 256 characters by default; will expand when necessary.
	std::string_view source_line;
	Substitute substitutions;
	std::stack<std::string> string_stack;
	SemanticState_Macro macro;
	std::size_t output_position = 0, previous_segment_length_output_position = 0;
	std::size_t segment_position = 0, segment_length = 0;
	unsigned long program_counter = 0, program_counter_of_statement = 0, program_counter_of_expression = 0;
	unsigned long current_macro_invocation = 0;
	unsigned long rs = 0;
	cc_bool end = cc_false;

	/* Fix-ups. */
	std::deque<FixUp> fixup_list;
	cc_bool fixup_needed = cc_false;
	cc_bool doing_fixup_pass = cc_false;

	/* If-statements. */
	unsigned int current_if_level = 0;
	unsigned int false_if_level = 0;
	cc_bool true_already_found = cc_false;

	/* OBJ directive. */
	cc_bool obj_active = cc_false;
	unsigned long obj_delta = 0;

	/* Listing. */
	unsigned int listing_counter = 0;
	cc_bool line_listed = cc_false;
	cc_bool suppress_listing = cc_false;
	cc_bool listing_macro = cc_false;

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

	std::variant<ModeNormal, ModeRept, ModeMacro, ModeWhile> mode;

	SemanticState(Options default_options)
		: options(std::move(default_options))
		, dictionary(options->case_insensitive)
	{}
};

/* Some forward declarations that are needed because some functions recurse into each other. */
static void AssembleFile(SemanticState *state);
static void AssembleLine(SemanticState *state, const std::string_view &source_line, const cc_bool write_line_to_listing_file);

static void OutputWriteRawByte(SemanticState* const state, const unsigned char byte)
{
	++state->output_position;
	state->output_stream->put(byte);
}

static void OutputSeek(SemanticState* const state, const std::size_t position)
{
	state->output_position = position;
	state->output_stream->seekp(position);
}

static void TerminatePreviousRecordSegment(SemanticState *state)
{
	if (state->previous_segment_length_output_position != 0)
	{
		if (state->segment_length == 0)
		{
			/* Previous segment contained nothing; make the new segment overwrite it. */
			OutputSeek(state, state->previous_segment_length_output_position - 5);
		}
		else
		{
			/* Write length of previous record. */
			const std::size_t previous_output_position = state->output_position;
			OutputSeek(state, state->previous_segment_length_output_position);
			OutputWriteRawByte(state, state->segment_length >> (8 * 0) & 0xFF);
			OutputWriteRawByte(state, state->segment_length >> (8 * 1) & 0xFF);
			OutputSeek(state, previous_output_position);
		}
	}
}

/* TODO: Don't write empty segments! */
static void WriteRecordSegment(SemanticState *state)
{
	TerminatePreviousRecordSegment(state);

	/* CPU architecture (68000). */
	OutputWriteRawByte(state, 0x01);
	/* Start address. */
	OutputWriteRawByte(state, state->segment_position >> (8 * 0) & 0xFF);
	OutputWriteRawByte(state, state->segment_position >> (8 * 1) & 0xFF);
	OutputWriteRawByte(state, state->segment_position >> (8 * 2) & 0xFF);
	OutputWriteRawByte(state, state->segment_position >> (8 * 3) & 0xFF);
	/* Length (placeholder). */
	state->previous_segment_length_output_position = state->output_position;
	OutputWriteRawByte(state, 0);
	OutputWriteRawByte(state, 0);
}

static void OutputWriteSegmentByte(SemanticState* const state, const unsigned char byte)
{
	if (state->segment_length == 0xFFFF)
	{
		state->segment_position += state->segment_length;

		/* Avoid rewriting the record header when doing fix-ups,
		   as the length field will be left blank. */
		if (state->doing_fixup_pass)
			OutputSeek(state, state->output_position + 1 + 4 + 2);
		else
			WriteRecordSegment(state);

		state->segment_length = 0;
	}

	++state->segment_length;

	OutputWriteRawByte(state, byte);
}

/* Other stuff */

static void ErrorMessageCommon(SemanticState *state)
{
	for (const auto &location : state->location)
		TextOutput_Format(*state->error_stream, "\nOn line {} of '{}'...", location.line_number, location.file_path);

	TextOutput_Format(*state->error_stream, "\n{}\n\n", state->source_line.empty() ? "[No source line]" : state->source_line);
}

template<typename... Args>
static void SemanticWarning(SemanticState *state, std::format_string<Args...> &&format_string, Args &&...args)
{
	if (state->options->warnings_enabled)
	{
		*state->error_stream << "Warning: ";

		TextOutput_Format(*state->error_stream, std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);

		ErrorMessageCommon(state);
	}
}

template<typename... Args>
static void SemanticError(SemanticState *state, std::format_string<Args...> &&format_string, Args &&...args)
{
	*state->error_stream << "Error: ";

	TextOutput_Format(*state->error_stream, std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);

	ErrorMessageCommon(state);

	state->success = cc_false;
}

template<typename... Args>
static void InternalError(SemanticState *state, std::format_string<Args...> &&format_string, Args &&...args)
{
	*state->error_stream << "Internal error: ";

	TextOutput_Format(*state->error_stream, std::forward<std::format_string<Args...>>(format_string), std::forward<Args>(args)...);

	ErrorMessageCommon(state);

	state->success = cc_false;
}

void m68kasm_warning(void *scanner, const std::string_view &message)
{
	SemanticState *state = (SemanticState*)m68kasm_get_extra(scanner);

	if (state->options->warnings_enabled)
	{
		TextOutput_Format(*state->error_stream, "Warning: {}", message);

		ErrorMessageCommon(state);
	}
}

void m68kasm_warning_pedantic(void *scanner, const std::string_view &message)
{
	SemanticState *state = (SemanticState*)m68kasm_get_extra(scanner);

	if (state->options->pedantic_warnings_enabled)
		m68kasm_warning(scanner, message);
}

void m68kasm_error(void *scanner, const std::string_view &message)
{
	SemanticState *state = (SemanticState*)m68kasm_get_extra(scanner);

	TextOutput_Format(*state->error_stream, "Error: {}", message);

	ErrorMessageCommon(state);
}

static std::string DecimalIntegerToString(const unsigned long integer)
{
	return std::format("{}", integer);
}

static std::string HexadecimalIntegerToString(const unsigned long integer)
{
	return std::format("{:X}", integer);
}

static std::string ComputeUniqueMacroSuffix(const SemanticState* const state)
{
	return std::format("_{}", state->current_macro_invocation);
}

static std::string ToStandardPath(const std::string_view &input_path)
{
	std::string output_path(input_path);

	for (auto &character : output_path)
		if (character == '\\')
			character = '/';

	return output_path;
}

static void ListSourceLine(SemanticState *state)
{
	/* Output line to listing file. */
	if (state->listing_stream != nullptr)
	{
		if (!state->line_listed)
		{
			unsigned int i;

			state->line_listed = cc_true;

			for (i = state->listing_counter; i < 26; ++i)
				state->listing_stream->put(' ');

			state->listing_stream->put(state->listing_macro ? 'M' : ' ');

			TextOutput_Format(*state->listing_stream, " {}\n", state->source_line);
		}
	}
}

static void ListIdentifierValue(SemanticState *state, unsigned long value)
{
	/* Output line to listing file. */
	if (state->listing_stream != nullptr && !state->doing_fixup_pass && !state->suppress_listing)
	{
		TextOutput_Format(*state->listing_stream, " ={:08X}", value);

		state->listing_counter += 2 + 8;
	}
}

/* TODO: Duplicate this and use function pointers. */
static void OutputByte(SemanticState *state, unsigned int byte)
{
	/* Write to listing file. */
	if (state->listing_stream != nullptr && !state->doing_fixup_pass && !state->suppress_listing)
	{
		/* We can only write up to 10 bytes. */
		if (state->listing_counter <= 25)
		{
			if (state->listing_counter == 25)
			{
				/* After the last byte, we output a '+' to signal there's more. */
				state->listing_stream->put('+');
				++state->listing_counter;
			}
			else
			{
				if (state->listing_counter % 5 == 0)
				{
					state->listing_stream->put(' ');
					++state->listing_counter;
				}

				TextOutput_Format(*state->listing_stream, "{:02X}", byte);
				state->listing_counter += 2;
			}
		}
	}

	/* Write to output file. */
	OutputWriteSegmentByte(state, byte);
}

static cc_bool CurrentlyExpandingMacro(const SemanticState* const state)
{
	return state->macro.active;
}

static std::string_view ExpandIdentifier(SemanticState *state, const std::string_view &identifier, std::string &expanded_identifier)
{
	if (identifier.empty() || (identifier[0] != state->options->local_signifier))
		return identifier;

	if (state->last_global_label.empty())
		return identifier.substr(1);

	expanded_identifier = std::format("{}{}", state->last_global_label, identifier);

	return expanded_identifier;
}

static SymbolDictionary::Entry* LookupSymbol(SemanticState *state, std::string_view identifier, SymbolDictionary **dictionary)
{
	SymbolDictionary::Entry *dictionary_entry = nullptr;

	/* TODO: Avoid this allocation entirely by hashing each half of the identifier separately. */
	std::string expanded_identifier;
	identifier = ExpandIdentifier(state, identifier, expanded_identifier);

	if (state->macro.dictionary != nullptr)
	{
		if (dictionary != nullptr)
			*dictionary = &*state->macro.dictionary;

		dictionary_entry = state->macro.dictionary->LookUp(identifier);
	}

	if (dictionary_entry == nullptr)
	{
		if (dictionary != nullptr)
			*dictionary = &state->dictionary;

		dictionary_entry = state->dictionary.LookUp(identifier);

		if (dictionary_entry == nullptr)
			if (dictionary != nullptr)
				*dictionary = nullptr;
	}

	return dictionary_entry;
}

template<typename SymbolType>
static SymbolDictionary::Entry* LookupSymbolAndCreateIfNotExist(SemanticState *state, std::string_view identifier, SymbolDictionary **dictionary)
{
	/* TODO: Deduplicate this with `LookupSymbol`. */
	SymbolDictionary::Entry *dictionary_entry = nullptr;

	/* TODO: Avoid this allocation entirely by hashing each half of the identifier separately. */
	std::string expanded_identifier;
	identifier = ExpandIdentifier(state, identifier, expanded_identifier);

	if (state->macro.dictionary != nullptr)
	{
		if (dictionary != nullptr)
			*dictionary = &*state->macro.dictionary;

		dictionary_entry = state->macro.dictionary->LookUp(identifier);
	}

	if (dictionary_entry == nullptr)
	{
		if (dictionary != nullptr)
			*dictionary = &state->dictionary;

		dictionary_entry = &state->dictionary.LookUpAndCreateIfNotExist(identifier);
	}

	if (!std::holds_alternative<SymbolNone>(*dictionary_entry) && !std::holds_alternative<SymbolType>(*dictionary_entry))
	{
		SemanticError(state, "Attempted to redefine symbol '{}' as a different type.", identifier);
		dictionary_entry = nullptr;
	}

	return dictionary_entry;
}

static cc_bool ResolveExpression(SemanticState *state, Expression *expression, unsigned long *value, cc_bool fold = cc_true);

static cc_bool ResolveExpressionWithoutFolding(SemanticState *state, Expression *expression, unsigned long *value)
{
	return ResolveExpression(state, expression, value, cc_false);
}

static cc_bool GetSymbolInteger(SemanticState *state, const std::string_view &identifier, const cc_bool must_evaluate_on_first_pass, unsigned long* const value)
{
	SymbolDictionary::Entry *dictionary_entry;

	dictionary_entry = LookupSymbol(state, identifier, nullptr);

	if (dictionary_entry == nullptr || std::holds_alternative<SymbolNone>(*dictionary_entry))
	{
		if (must_evaluate_on_first_pass)
			SemanticError(state, "Symbol '{}' must be evaluable on first pass.", identifier);
		else if (state->doing_fixup_pass)
			SemanticError(state, "Symbol '{}' does not exist.", identifier);
		else
			state->fixup_needed = cc_true;

		return cc_false;
	}

	return std::visit(
		CC_VisitHelper{
			[&](SymbolSpecial &symbol)
			{
				switch (symbol.type)
				{
					case SymbolSpecial::Type::RS:
						*value = state->rs;
						break;

					case SymbolSpecial::Type::NARG:
						if (!CurrentlyExpandingMacro(state))
						{
							SemanticWarning(state, "Symbol '{}' cannot be used outside of a macro.", identifier);
							return cc_false;
						}

						*value = state->macro.total_arguments_at_start;

						break;

					default:
						// Should never happen.
						assert(cc_false);
						return cc_false;
				}

				return cc_true;
			},
			[&](SymbolVariable &symbol)
			{
				if (state->doing_fixup_pass)
					SemanticWarning(state, "Symbol '{}' forward-referenced despite being a redefineable variable.", identifier);

				*value = symbol.value;
				return cc_true;
			},
			[&](SymbolLabel &symbol)
			{
				*value = symbol.value;
				return cc_true;
			},
			[&](SymbolConstant &symbol)
			{
				*value = symbol.value;
				return cc_true;
			},
			[&](SymbolExpressionConstant &symbol)
			{
				if (!ResolveExpression(state, &symbol.expression, value))
					return cc_false;

				return cc_true;
			},
			[&](auto&)
			{
				SemanticError(state, "Symbol '{}' is not a label, constant, or variable.", identifier);
				return cc_false;
			}
		}, *dictionary_entry
	);
}

static cc_bool ResolveExpression(SemanticState *state, Expression *expression, unsigned long *value, cc_bool fold)
{
	if (!std::visit(
		CC_VisitHelper{
			[&](ExpressionArithmetic2Operand &expression)
			{
				unsigned long left_value;
				unsigned long right_value;

				/* Use a binary OR so that both expressions are properly folded (C short-circuiting could cause the second call to never occur if we used a logical OR). */
				if (!ResolveExpression(state, &expression.subexpressions[0], &left_value, fold) | !ResolveExpression(state, &expression.subexpressions[1], &right_value, fold))
					return cc_false;

				switch (expression.type)
				{
					default:
						/* This code should never be ran. */
						assert(cc_false);
						break;

					case ExpressionArithmetic2Operand::Type::SUBTRACT:
						*value = left_value - right_value;
						break;

					case ExpressionArithmetic2Operand::Type::ADD:
						*value = left_value + right_value;
						break;

					case ExpressionArithmetic2Operand::Type::MULTIPLY:
					case ExpressionArithmetic2Operand::Type::DIVIDE:
					case ExpressionArithmetic2Operand::Type::MODULO:
					{
						const cc_bool left_value_was_negative  = left_value  >= 0x80000000;
						const cc_bool right_value_was_negative = right_value >= 0x80000000;

						/* Make inputs absolute, so we can emulate signed arithmetic with unsigned arithmetic. */
						left_value  = (left_value  ^ (-(unsigned long)left_value_was_negative  & 0xFFFFFFFF)) + left_value_was_negative;
						right_value = (right_value ^ (-(unsigned long)right_value_was_negative & 0xFFFFFFFF)) + right_value_was_negative;

						switch (expression.type)
						{
							default:
								assert(cc_false);
								return cc_false;

							case ExpressionArithmetic2Operand::Type::MULTIPLY:
								*value = left_value * right_value;

								if (left_value_was_negative != right_value_was_negative)
									*value = -*value;

								break;

							case ExpressionArithmetic2Operand::Type::DIVIDE:
								if (right_value == 0)
								{
									SemanticError(state, "Cannot divide by zero.");
									return cc_false;
								}

								*value = left_value / right_value;

								if (left_value_was_negative != right_value_was_negative)
									*value = -*value;

								break;

							case ExpressionArithmetic2Operand::Type::MODULO:
								if (right_value == 0)
								{
									SemanticError(state, "Cannot modulo by zero.");
									return cc_false;
								}

								*value = left_value % right_value;

								if (left_value_was_negative)
									*value = -*value;

								break;
						}

						break;
					}

					case ExpressionArithmetic2Operand::Type::LOGICAL_OR:
						*value = left_value != 0 || right_value != 0 ? -1 : 0;
						break;

					case ExpressionArithmetic2Operand::Type::LOGICAL_AND:
						*value = left_value != 0 && right_value != 0 ? -1 : 0;
						break;

					case ExpressionArithmetic2Operand::Type::BITWISE_OR:
						*value = left_value | right_value;
						break;

					case ExpressionArithmetic2Operand::Type::BITWISE_XOR:
						*value = left_value ^ right_value;
						break;

					case ExpressionArithmetic2Operand::Type::BITWISE_AND:
						*value = left_value & right_value;
						break;

					case ExpressionArithmetic2Operand::Type::EQUALITY:
						*value = left_value == right_value ? -1 : 0;
						break;

					case ExpressionArithmetic2Operand::Type::INEQUALITY:
						*value = left_value != right_value ? -1 : 0;
						break;

					case ExpressionArithmetic2Operand::Type::LESS_THAN:
					case ExpressionArithmetic2Operand::Type::LESS_OR_EQUAL:
					case ExpressionArithmetic2Operand::Type::MORE_THAN:
					case ExpressionArithmetic2Operand::Type::MORE_OR_EQUAL:
						/* Flip the sign bit so we can emulate signed comparisons with unsigned integers. */
						left_value  ^= 0x80000000;
						right_value ^= 0x80000000;

						switch (expression.type)
						{
							default:
								assert(cc_false);
								return cc_false;

							case ExpressionArithmetic2Operand::Type::LESS_THAN:
								*value = left_value < right_value;
								break;

							case ExpressionArithmetic2Operand::Type::LESS_OR_EQUAL:
								*value = left_value <= right_value;
								break;

							case ExpressionArithmetic2Operand::Type::MORE_THAN:
								*value = left_value > right_value;
								break;

							case ExpressionArithmetic2Operand::Type::MORE_OR_EQUAL:
								*value = left_value >= right_value;
								break;
						}

						*value = -*value;
						break;

					/* asm68k does 32-bit arithmetic, so it masks the shift amount to 0-31 (though whether this is intentional or just a side-effect of using x86 shift instructions that do this is unknown).
					   (note: this is also needed to prevent undefined behaviour in C from shifting by an amount greater than or equal to the width of the type). */
					/* TODO: Print a warning here. */
					case ExpressionArithmetic2Operand::Type::LEFT_SHIFT:
						*value = left_value << (right_value % 32);
						break;

					case ExpressionArithmetic2Operand::Type::RIGHT_SHIFT:
						*value = left_value >> (right_value % 32);
						break;
				}

				/* Prevent 'bleeding' out of the 68k's 32-bit range. */
				*value &= 0xFFFFFFFF;
				return cc_true;
			},
			[&](ExpressionArithmetic1Operand &expression)
			{
				if (!ResolveExpression(state, &*expression.subexpressions, value, fold))
					return cc_false;

				switch (expression.type)
				{
					default:
						/* This code should never be ran. */
						assert(cc_false);
						return cc_false;

					case ExpressionArithmetic1Operand::Type::NEGATE:
						*value = 0 - *value;
						break;

					case ExpressionArithmetic1Operand::Type::BITWISE_NOT:
						*value = ~*value;
						break;

					case ExpressionArithmetic1Operand::Type::LOGICAL_NOT:
						*value = *value == 0 ? -1 : 0;
						break;
				}

				/* Prevent 'bleeding' out of the 68k's 32-bit range. */
				*value &= 0xFFFFFFFF;
				return cc_true;
			},
			[&](ExpressionNumber &expression)
			{
				*value = expression.value;
				return cc_true;
			},
			[&](ExpressionIdentifier &expression)
			{
				if (!GetSymbolInteger(state, expression.string, cc_false, value))
					return cc_false;

				return cc_true;
			},
			[&](ExpressionString &expression)
			{
				const std::size_t length = expression.string.length();

				if (length > 4)
				{
					SemanticError(state, "Character literals cannot be more than 4 characters long.");
					return cc_false;
				}

				*value = 0;

				for (std::size_t i = 0; i < length; ++i)
				{
					*value <<= 8;
					*value |= expression.string[i];
				}

				return cc_true;
			},
			[&](ExpressionProgramCounterOfStatement&)
			{
				*value = state->program_counter_of_statement;
				return cc_true;
			},
			[&](ExpressionProgramCounterOfExpression&)
			{
				*value = state->program_counter_of_expression;
				return cc_true;
			},
			[&](ExpressionStrlen &expression)
			{
				*value = expression.string.length();
				return cc_true;
			},
			[&](ExpressionStrcmp &expression)
			{
				*value = expression.string_a == expression.string_b ? -1 : 0;
				return cc_true;
			},
			[&](ExpressionInstr &expression)
			{
				unsigned long position;

				if (!ResolveExpression(state, &expression.subexpressions[0], &position, fold))
					return cc_false;

				const auto expression_string_a = std::get_if<ExpressionString>(&expression.subexpressions[1]);
				const auto expression_string_b = std::get_if<ExpressionString>(&expression.subexpressions[2]);

				if (expression_string_a == nullptr || expression_string_b == nullptr)
				{
					SemanticError(state, "Last two arguments to `INSTR` must be strings.");
					return cc_false;
				}

				/* TODO: What if 'position' is 0? */
				const std::size_t found_position = CaseInsensitive::FindString(expression_string_a->string, expression_string_b->string, position - 1, state->options->case_insensitive);
				*value = found_position == expression_string_a->string.npos ? 0 : found_position + 1;
				return cc_true;
			},
			[&](ExpressionDef &expression)
			{
				*value = LookupSymbol(state, expression.string, nullptr) != nullptr ? -1 : 0;
				return cc_true;
			},
			[&](ExpressionTypeWithIdentifier&)
			{
				*value = 0; /* TODO: This is a placeholder! */
				return cc_true;
			},
			[&](ExpressionTypeWithNumber&)
			{
				*value = 1; /* TODO: This is a placeholder! */
				return cc_true;
			},
			[&](ExpressionFilesize &expression)
			{
				const std::string standard_path = ToStandardPath(expression.string);

				if (!std::filesystem::exists(standard_path))
				{
					SemanticError(state, "File '{}' does not exist.", expression.string);
					return cc_false;
				}

				*value = std::filesystem::file_size(standard_path);
				return cc_true;
			}
		}, *expression
	))
		return cc_false;

	/* Now that we have resolved the value, let's hardcode it here so that we don't ever have to calculate it again. */
	/* This is especially useful for fix-ups, which may otherwise depend on identifiers that no longer exist at the time the value is resolved again. */
	if (fold)
		expression->emplace<ExpressionNumber>(*value);

	return cc_true;
}

static void TerminateIf(SemanticState *state)
{
	if (state->false_if_level == state->current_if_level || state->false_if_level == 0)
	{
		state->true_already_found = cc_true;
		state->false_if_level = 0;
	}

	--state->current_if_level;
}

static void TerminateRept(SemanticState *state, SemanticState::ModeRept &rept_state)
{
	/* Back-up some state into local variables, in case a nested REPT statement clobbers it. */
	auto rept_state_copy = std::move(rept_state);

	/* Exit REPT mode. */
	state->mode.emplace<SemanticState::ModeNormal>();

	while (rept_state_copy.repetitions-- != 0)
	{
		/* Rewind back to the line number of the start of the REPT. */
		state->location.front().line_number = rept_state_copy.line_number;

		/* Process the REPT's nested statements. */
		for (const auto &source_line : rept_state_copy.source_line_list)
			AssembleLine(state, source_line, state->options->expand_all_macros);
	}

	/* Increment past the ENDR line number. */
	++state->location.front().line_number;
}

static void PurgeMacro(SemanticState* const state, std::string_view identifier, const cc_bool allow_undefined)
{
	SymbolDictionary *dictionary;
	SymbolDictionary::Entry* const entry = LookupSymbol(state, identifier, &dictionary);

	if (entry == nullptr)
	{
		if (!allow_undefined)
			SemanticError(state, "Cannot purge '{}' as it does not exist.\n", identifier);
	}
	else if (!std::holds_alternative<SymbolMacro>(*entry))
	{
		SemanticError(state, "Cannot purge '{}' as it is not a macro.\n", identifier);
	}
	else if (CurrentlyExpandingMacro(state))
	{
		/* TODO: I don't know if that could be meaningfully supported in some way but for now this just seems like a
		   "how to crash and burn into UAF/double free" without a bunch of changes to support it, for not much benefit.
		   We don't check for the name of the currently-expanded macro because one could e.g. invoke a, which invokes b,
		   which redefines a, and there is currently no simple wey to check the entire macro expansion stack. */
		SemanticError(state, "Cannot purge '{}' during macro expansion.\n", identifier);
	}
	else
	{
		/* TODO: Avoid this allocation entirely by hashing each half of the identifier separately. */
		std::string expanded_identifier;
		identifier = ExpandIdentifier(state, identifier, expanded_identifier);

		dictionary->Remove(identifier);
	}
}

static cc_bool AddToSymbolTable(SemanticState *state, const std::string_view &identifier, SymbolVariable symbol_contents)
{
	SymbolDictionary::Entry *symbol = LookupSymbolAndCreateIfNotExist<SymbolVariable>(state, identifier, nullptr);

	if (symbol == nullptr)
		return cc_false;

	symbol->emplace<SymbolVariable>(std::move(symbol_contents));

	return cc_true;
}

static cc_bool AddToSymbolTable(SemanticState *state, const std::string_view &identifier, SymbolConstant symbol_contents)
{
	SymbolDictionary::Entry *symbol = LookupSymbolAndCreateIfNotExist<SymbolConstant>(state, identifier, nullptr);

	if (symbol == nullptr)
		return cc_false;

	const auto constant_symbol = std::get_if<SymbolConstant>(symbol);

	if (constant_symbol != nullptr && constant_symbol->value != symbol_contents.value)
	{
		SemanticError(state, "Constant '{}' cannot be redefined to a different value.", identifier);
		return cc_false;
	}

	symbol->emplace<SymbolConstant>(std::move(symbol_contents));

	return cc_true;
}

static cc_bool AddToSymbolTable(SemanticState *state, const std::string_view &identifier, SymbolLabel symbol_contents)
{
	SymbolDictionary::Entry *symbol = LookupSymbolAndCreateIfNotExist<SymbolLabel>(state, identifier, nullptr);

	if (symbol == nullptr)
		return cc_false;

	if (std::holds_alternative<SymbolLabel>(*symbol))
	{
		SemanticError(state, "Label '{}' cannot be redefined.", identifier);
		return cc_false;
	}

	symbol->emplace<SymbolLabel>(std::move(symbol_contents));

	return cc_true;
}

static cc_bool AddToSymbolTable(SemanticState *state, const std::string_view &identifier, SymbolExpressionConstant symbol_contents)
{
	SymbolDictionary::Entry *symbol = LookupSymbolAndCreateIfNotExist<SymbolExpressionConstant>(state, identifier, nullptr);

	if (symbol == nullptr)
		return cc_false;

	symbol->emplace<SymbolExpressionConstant>(std::move(symbol_contents));

	return cc_true;
}

static cc_bool AddToSymbolTable(SemanticState *state, SymbolMacro symbol_contents)
{
	const std::string_view &identifier = symbol_contents.macro.name;

	SymbolDictionary::Entry *symbol = LookupSymbolAndCreateIfNotExist<SymbolMacro>(state, identifier, nullptr);

	if (symbol == nullptr)
		return cc_false;

	if (std::holds_alternative<SymbolMacro>(*symbol))
	{
		SemanticError(state, "Macro '{}' cannot be redefined.", identifier);
		return cc_false;
	}

	symbol->emplace<SymbolMacro>(std::move(symbol_contents));

	return cc_true;
}

static cc_bool AddToSymbolTable(SemanticState *state, const std::string_view &identifier, SymbolSpecial symbol_contents)
{
	SymbolDictionary::Entry *symbol = LookupSymbolAndCreateIfNotExist<SymbolSpecial>(state, identifier, nullptr);

	if (symbol == nullptr)
		return cc_false;

	symbol->emplace<SymbolSpecial>(std::move(symbol_contents));

	return cc_true;
}

static void TerminateMacro(SemanticState *state, SemanticState::ModeMacro &macro_state)
{
	const std::string_view &identifier = macro_state.metadata.name;

	PurgeMacro(state, identifier, cc_true);

	if (CurrentlyExpandingMacro(state))
	{
		/* See comment in PurgeMacro after the check for the same condition for more details. */
		SemanticError(state, "Cannot define macro '{}' during macro expansion.\n", identifier);
	}
	else if (!identifier.empty())
	{
		/* Add the macro to the symbol table. */
		AddToSymbolTable(state, SymbolMacro{std::move(macro_state.metadata)});
	}

	/* Exit macro mode. */
	state->mode.emplace<SemanticState::ModeNormal>();
}

static void TerminateWhile(SemanticState *state, SemanticState::ModeWhile &while_state)
{
	const std::string_view old_source_line = state->source_line;

	/* Back-up some state into local variables, in case a nested WHILE statement clobbers it. */
	auto while_state_copy = std::move(while_state);

	/* Exit WHILE mode. */
	state->mode.emplace<SemanticState::ModeNormal>();

	state->suppress_listing = cc_true;

	for (;;)
	{
		unsigned long value;

		state->source_line = while_state_copy.source_line;

		/* We absolutely do not want to fold (cache) this expression, because
		   while-loops depend on variables mutating from previous values;
		   without this, while-loops will never terminate. */
		if (!ResolveExpressionWithoutFolding(state, &while_state_copy.expression, &value))
		{
			SemanticError(state, "Expression must be evaluable on the first pass.");
			break;
		}

		if (value == 0)
			break;

		/* Rewind back to the line number of the start of the WHILE. */
		state->location.front().line_number = while_state_copy.line_number;

		/* Process the WHILE's nested statements. */
		for (const auto &source_line : while_state_copy.source_line_list)
			AssembleLine(state, source_line, cc_false);
	}

	/* Increment past the ENDW line number. */
	++state->location.front().line_number;

	state->source_line = old_source_line;
}

static unsigned int ConstructSizeBits(Size size)
{
	switch (size)
	{
		case SIZE_BYTE:
		case SIZE_SHORT:
			return 0x0000;

		case SIZE_UNDEFINED:
		case SIZE_WORD:
			return 0x0040;

		case SIZE_LONGWORD:
			return 0x0080;
	}

	assert(cc_false);
	return 0x0000;
}

static unsigned int ConstructEffectiveAddressBits(SemanticState *state, const Operand *operand)
{
	unsigned int m, xn;

	switch (operand->type)
	{
		default:
		case OPERAND_NONE:
			assert(cc_false);
			m = 0;
			xn = 0;
			break;

		case OPERAND_DATA_REGISTER:
			m = 0; /* 000 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER:
			m = 1; /* 001 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER_INDIRECT:
			m = 2; /* 010 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT:
			m = 3; /* 011 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT:
			m = 4; /* 100 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT:
			m = 5; /* 101 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
			m = 6; /* 110 */
			xn = operand->main_register;
			break;

		case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT:
			m = 7;  /* 111 */
			xn = 2; /* 010 */
			break;

		case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
			m = 7;  /* 111 */
			xn = 3; /* 011 */
			break;

		case OPERAND_ADDRESS:
		case OPERAND_ADDRESS_ABSOLUTE:
			m = 7; /* 111 */

			switch (operand->size)
			{
				default:
				case SIZE_WORD:
					xn = 0; /* 000 */
					break;

				case SIZE_BYTE:
				case SIZE_SHORT:
					SemanticError(state, "Absolute address can only be word- or longword-sized.");
					/* Fallthrough */
				case SIZE_UNDEFINED:
				case SIZE_LONGWORD:
					xn = 1; /* 001 */
					break;
			}

			break;

		case OPERAND_LITERAL:
			m = 7;  /* 111 */
			xn = 4; /* 100 */
			break;

		case OPERAND_STATUS_REGISTER:
		case OPERAND_CONDITION_CODE_REGISTER:
		case OPERAND_USER_STACK_POINTER_REGISTER:
		case OPERAND_REGISTER_LIST:
			SemanticError(state, "Invalid operand type: register lists, USP, SR, and CCR cannot be used here.");
			/* Just pretend it's data register 0 to keep things moving along. */
			m = 0;
			xn = 0;
			break;
	}

	return (m << 3) | (xn << 0);
}

static unsigned int ToAlternateEffectiveAddressBits(unsigned int bits)
{
	const unsigned int m = (bits >> 3) & 7;
	const unsigned int dn = (bits >> 0) & 7;

	return (m << 6) | (dn << 9);
}

static void SetLastGlobalLabel(SemanticState *state, const std::string_view &label)
{
	const char first_character = label[0];

	if (first_character != state->options->local_signifier)
	{
		/* This is a global label - cache it for later. */
		state->last_global_label = label;
	}
}

static void ResolveInstructionAmbiguity(SemanticState* const state, StatementInstruction* const instruction)
{
	/* Some instructions are ambiguous, so figure them out fully here. */
	for (;;)
	{
		switch (instruction->opcode.type)
		{
			#define ALIAS_AND_LOOP(ALIAS) {instruction->opcode.type = ALIAS; continue;}
			#define WARN_ALIAS_AND_LOOP(ALIAS, MESSAGE) {if (state->options->pedantic_warnings_enabled) SemanticWarning(state, MESSAGE); ALIAS_AND_LOOP(ALIAS)}

			case OPCODE_ORI:
				if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ORI_TO_CCR)
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ORI_TO_SR)

				break;

			case OPCODE_ANDI:
				if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ANDI_TO_CCR)
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ANDI_TO_SR)

				break;

			case OPCODE_EORI:
				if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_EORI_TO_CCR)
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_EORI_TO_SR)

				break;

			case OPCODE_BTST_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BTST_DYNAMIC)

				break;

			case OPCODE_BCHG_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BCHG_DYNAMIC)

				break;

			case OPCODE_BCLR_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BCLR_DYNAMIC)

				break;

			case OPCODE_BSET_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BSET_DYNAMIC)

				break;

			case OPCODE_MOVEP_TO_REG:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVEP_FROM_REG)

				break;

			case OPCODE_MOVE:
				if (instruction->operands[0].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_FROM_SR)
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_TO_SR)
				else if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_TO_CCR)
				else if (instruction->operands[0].type == OPERAND_USER_STACK_POINTER_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_FROM_USP)
				else if (instruction->operands[1].type == OPERAND_USER_STACK_POINTER_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_TO_USP)
				else if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_MOVEA, "MOVE should be MOVEA.")
				else if (instruction->operands[0].type == OPERAND_REGISTER_LIST)
					WARN_ALIAS_AND_LOOP(OPCODE_MOVEM_FROM_REGS, "MOVE should be MOVEM.")
				else if (instruction->operands[1].type == OPERAND_REGISTER_LIST)
					WARN_ALIAS_AND_LOOP(OPCODE_MOVEM_TO_REGS, "MOVE should be MOVEM.")

				break;

			case OPCODE_MOVEM_TO_REGS:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER || instruction->operands[0].type == OPERAND_ADDRESS_REGISTER || instruction->operands[0].type == OPERAND_REGISTER_LIST)
					ALIAS_AND_LOOP(OPCODE_MOVEM_FROM_REGS)

				break;

			case OPCODE_SBCD_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_SBCD_ADDRESS_REGS)

				break;

			case OPCODE_OR_TO_REG:
				if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_ORI, "OR should be ORI.")
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_OR_FROM_REG)

				break;

			case OPCODE_SUB_TO_REG:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_SUBA, "SUB should be SUBA.")
				else if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_SUBI, "SUB should be SUBI.")
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_SUB_FROM_REG)

				break;

			case OPCODE_SUBI:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_SUBA, "SUBI should be SUBA.")

				break;

			case OPCODE_SUBX_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_SUBX_ADDRESS_REGS)

				break;

			case OPCODE_ABCD_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_ABCD_ADDRESS_REGS)

				break;

			case OPCODE_AND_TO_REG:
				if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_ANDI, "AND should be ANDI.")
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_AND_FROM_REG)

				break;

			case OPCODE_ADD_TO_REG:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_ADDA, "ADD should be ADDA.")
				else if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_ADDI, "ADD should be ADDI.")
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ADD_FROM_REG)

				break;

			case OPCODE_ADDI:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_ADDA, "ADDI should be ADDA.")

				break;

			case OPCODE_ADDX_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_ADDX_ADDRESS_REGS)

				break;

			case OPCODE_CMP:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_CMPA, "CMP should be CMPA.")
				else if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_CMPI, "CMP should be CMPI.")
				else if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT && instruction->operands[1].type == OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT)
					WARN_ALIAS_AND_LOOP(OPCODE_CMPM, "CMP should be CMPM.")

				break;

			case OPCODE_EOR:
				if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_EORI, "EOR should be EORI.")

				break;

			case OPCODE_ASL_STATIC:
			case OPCODE_ASR_STATIC:
			case OPCODE_LSL_STATIC:
			case OPCODE_LSR_STATIC:
			case OPCODE_ROXL_STATIC:
			case OPCODE_ROXR_STATIC:
			case OPCODE_ROL_STATIC:
			case OPCODE_ROR_STATIC:
				if (instruction->operands[1].type != 0)
				{
					if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					{
						switch (instruction->opcode.type)
						{
							case OPCODE_ASL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ASL_DYNAMIC)
								break;

							case OPCODE_ASR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ASR_DYNAMIC)
								break;

							case OPCODE_LSL_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSL_DYNAMIC)
								break;

							case OPCODE_LSR_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSR_DYNAMIC)
								break;

							case OPCODE_ROXL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXL_DYNAMIC)
								break;

							case OPCODE_ROXR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXR_DYNAMIC)
								break;

							case OPCODE_ROL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROL_DYNAMIC)
								break;

							case OPCODE_ROR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROR_DYNAMIC)
								break;

							default:
								break;
						}
					}
				}
				else
				{
					if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					{
						/* If I remember correctly, this syntax is actually officially valid. */
						instruction->operands[1] = std::move(instruction->operands[0]);
						instruction->operands[0].type = OPERAND_LITERAL;
						instruction->operands[0].literal.emplace<ExpressionNumber>(1);
					}
					else
					{
						switch (instruction->opcode.type)
						{
							case OPCODE_ASL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ASL_SINGLE)
								break;

							case OPCODE_ASR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ASR_SINGLE)
								break;

							case OPCODE_LSL_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSL_SINGLE)
								break;

							case OPCODE_LSR_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSR_SINGLE)
								break;

							case OPCODE_ROXL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXL_SINGLE)
								break;

							case OPCODE_ROXR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXR_SINGLE)
								break;

							case OPCODE_ROL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROL_SINGLE)
								break;

							case OPCODE_ROR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROR_SINGLE)
								break;

							default:
								break;
						}
					}
				}

				break;

			default:
				break;
		}

		break;
	}
}

static void OptimiseInstruction(SemanticState* const state, StatementInstruction* const instruction)
{
	if (state->options->optimisation.zero_displacement)
	{
		unsigned int i;

		for (i = 0; i < CC_COUNT_OF(instruction->operands); ++i)
		{
			Operand* const operand = &instruction->operands[i];

			if (operand->type == OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT)
			{
				unsigned long value;

				if (ResolveExpression(state, &operand->literal, &value) && value == 0)
					operand->type = OPERAND_ADDRESS_REGISTER_INDIRECT;
			}
		}
	}
}

struct InstructionMetadata
{
	const char *name;

	unsigned int allowed_sizes;

	unsigned int allowed_operands[2];
};

/* The order of this array absolutely must match the order of the OperandType enum! */
static const InstructionMetadata instruction_metadata_all[] = {
	{	/* OPCODE_ORI_TO_CCR */
		"ORI",
		SIZE_BYTE | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_CONDITION_CODE_REGISTER,
		}
	},
	{	/* OPCODE_ORI_TO_SR */
		"ORI",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_STATUS_REGISTER,
		}
	},
	{	/* OPCODE_ORI */
		"ORI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_ANDI_TO_CCR */
		"ANDI",
		SIZE_BYTE | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_CONDITION_CODE_REGISTER,
		}
	},
	{	/* OPCODE_ANDI_TO_SR */
		"ANDI",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_STATUS_REGISTER,
		}
	},
	{	/* OPCODE_ANDI */
		"ANDI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUBI */
		"SUBI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_ADDI */
		"ADDI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_EORI_TO_CCR */
		"EORI",
		SIZE_BYTE | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_CONDITION_CODE_REGISTER,
		}
	},
	{	/* OPCODE_EORI_TO_SR */
		"EORI",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_STATUS_REGISTER,
		}
	},
	{	/* OPCODE_EORI */
		"EORI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_CMPI */
		"CMPI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_BTST_STATIC */
		"BTST",
		SIZE_BYTE | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
		}
	},
	{	/* OPCODE_BCHG_STATIC */
		"BCHG",
		SIZE_BYTE | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
		}
	},
	{	/* OPCODE_BCLR_STATIC */
		"BCLR",
		SIZE_BYTE | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
		}
	},
	{	/* OPCODE_BSET_STATIC */
		"BSET",
		SIZE_BYTE | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
		}
	},
	{	/* OPCODE_BTST_DYNAMIC */
		"BTST",
		SIZE_BYTE | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
		}
	},
	{	/* OPCODE_BCHG_DYNAMIC */
		"BCHG",
		SIZE_BYTE | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
		}
	},
	{	/* OPCODE_BCLR_DYNAMIC */
		"BCLR",
		SIZE_BYTE | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
		}
	},
	{	/* OPCODE_BSET_DYNAMIC */
		"BSET",
		SIZE_BYTE | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
		}
	},
	{	/* OPCODE_MOVEP_TO_REG */
		"MOVEP",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_MOVEP_FROM_REG */
		"MOVEP",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT,
		}
	},
	{	/* OPCODE_MOVEA */
		"MOVEA",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_ADDRESS_REGISTER,
		}
	},
	{	/* OPCODE_MOVE */
		"MOVE",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_MOVE_FROM_SR */
		"MOVE",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_STATUS_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_MOVE_TO_CCR */
		"MOVE",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_CONDITION_CODE_REGISTER,
		}
	},
	{	/* OPCODE_MOVE_TO_SR */
		"MOVE",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_STATUS_REGISTER,
		}
	},
	{	/* OPCODE_NEGX */
		"NEGX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_CLR */
		"CLR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_NEG */
		"NEG",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_NOT */
		"NOT",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_EXT */
		"EXT",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			0,
		}
	},
	{	/* OPCODE_NBCD */
		"NBCD",
		SIZE_BYTE | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_SWAP */
		"SWAP",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			0,
		}
	},
	{	/* OPCODE_PEA */
		"PEA",
		SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0,
		}
	},
	{	/* OPCODE_ILLEGAL */
		"ILLEGAL",
		SIZE_UNDEFINED,
		{
			0,
			0,
		}
	},
	{	/* OPCODE_TAS */
		"TAS",
		SIZE_BYTE | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_TST */
		"TST",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_TRAP */
		"TRAP",
		SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			0,
		}
	},
	{	/* OPCODE_LINK */
		"LINK",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER,
			OPERAND_LITERAL,
		}
	},
	{	/* OPCODE_UNLK */
		"UNLK",
		SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER,
			0,
		}
	},
	{	/* OPCODE_MOVE_TO_USP */
		"MOVE",
		SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER,
			OPERAND_USER_STACK_POINTER_REGISTER,
		}
	},
	{	/* OPCODE_MOVE_FROM_USP */
		"MOVE",
		SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_USER_STACK_POINTER_REGISTER,
			OPERAND_ADDRESS_REGISTER,
		}
	},
	{	/* OPCODE_RESET */
		"RESET",
		SIZE_UNDEFINED,
		{
			0,
			0,
		}
	},
	{	/* OPCODE_NOP */
		"NOP",
		SIZE_UNDEFINED,
		{
			0,
			0,
		}
	},
	{	/* OPCODE_STOP */
		"STOP",
		SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			0,
		}
	},
	{	/* OPCODE_RTE */
		"RTE",
		SIZE_UNDEFINED,
		{
			0,
			0,
		}
	},
	{	/* OPCODE_RTS */
		"RTS",
		SIZE_UNDEFINED,
		{
			0,
			0,
		}
	},
	{	/* OPCODE_TRAPV */
		"TRAPV",
		SIZE_UNDEFINED,
		{
			0,
			0,
		}
	},
	{	/* OPCODE_RTR */
		"RTR",
		SIZE_UNDEFINED,
		{
			0,
			0,
		}
	},
	{	/* OPCODE_JSR */
		"JSR",
		SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0,
		}
	},
	{	/* OPCODE_JMP */
		"JMP",
		SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0,
		}
	},
	{	/* OPCODE_MOVEM_TO_REGS */
		"MOVEM",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_REGISTER_LIST,
		}
	},
	{	/* OPCODE_MOVEM_FROM_REGS */
		"MOVEM",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_REGISTER_LIST,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_LEA */
		"LEA",
		SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_ADDRESS_REGISTER,
		}
	},
	{	/* OPCODE_CHK */
		"CHK",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ADDQ */
		"ADDQ",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUBQ */
		"SUBQ",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_Scc */
		"Scc",
		SIZE_BYTE | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_DBcc */
		"DBcc",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS,
		}
	},
	{	/* OPCODE_BRA */
		"BRA",
		SIZE_BYTE | SIZE_SHORT | SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS,
			0,
		}
	},
	{	/* OPCODE_BSR */
		"BSR",
		SIZE_BYTE | SIZE_SHORT | SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS,
			0,
		}
	},
	{	/* OPCODE_Bcc */
		"Bcc",
		SIZE_BYTE | SIZE_SHORT | SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS,
			0,
		}
	},
	{	/* OPCODE_MOVEQ */
		"MOVEQ",
		SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_DIVU */
		"DIVU",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_DIVS */
		"DIVS",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_SBCD_DATA_REGS */
		"SBCD",
		SIZE_BYTE | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_SBCD_ADDRESS_REGS */
		"SBCD",
		SIZE_BYTE | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
		}
	},
	{	/* OPCODE_OR_TO_REG */
		"OR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_OR_FROM_REG */
		"OR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUB_TO_REG */
		"SUB",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_SUB_FROM_REG */
		"SUB",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUBX_DATA_REGS */
		"SUBX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_SUBX_ADDRESS_REGS */
		"SUBX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
		}
	},
	{	/* OPCODE_SUBA */
		"SUBA",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_ADDRESS_REGISTER,
		}
	},
	{	/* OPCODE_EOR */
		"EOR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_CMPM */
		"CMPM",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT,
			OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT,
		}
	},
	{	/* OPCODE_CMP */
		"CMP",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_CMPA */
		"CMPA",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_ADDRESS_REGISTER,
		}
	},
	{	/* OPCODE_MULU */
		"MULU",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_MULS */
		"MULS",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ABCD_DATA_REGS */
		"ABCD",
		SIZE_BYTE | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ABCD_ADDRESS_REGS */
		"ABCD",
		SIZE_BYTE | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
		}
	},
	{	/* OPCODE_EXG */
		"EXG",
		SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER,
		}
	},
	{	/* OPCODE_AND_TO_REG */
		"AND",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_AND_FROM_REG */
		"AND",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_ADD_TO_REG */
		"ADD",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ADD_FROM_REG */
		"ADD",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_ADDX_DATA_REGS */
		"ADDX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ADDX_ADDRESS_REGS */
		"ADDX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
		}
	},
	{	/* OPCODE_ADDA */
		"ADDA",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE
				| OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_ADDRESS_REGISTER,
		}
	},
	{	/* OPCODE_ASL_STATIC */
		"ASL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ASR_STATIC */
		"ASR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSL_STATIC */
		"LSL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSR_STATIC */
		"LSR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXL_STATIC */
		"ROXL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXR_STATIC */
		"ROXR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROL_STATIC */
		"ROL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROR_STATIC */
		"ROR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ASL_DYNAMIC */
		"ASL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ASR_DYNAMIC */
		"ASR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSL_DYNAMIC */
		"LSL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSR_DYNAMIC */
		"LSR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXL_DYNAMIC */
		"ROXL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXR_DYNAMIC */
		"ROXR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROL_DYNAMIC */
		"ROL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROR_DYNAMIC */
		"ROR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ASL_SINGLE */
		"ASL",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_ASR_SINGLE */
		"ASR",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_LSL_SINGLE */
		"LSL",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_LSR_SINGLE */
		"LSR",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_ROXL_SINGLE */
		"ROXL",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_ROXR_SINGLE */
		"ROXR",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_ROL_SINGLE */
		"ROL",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_ROR_SINGLE */
		"ROR",
		SIZE_WORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
};

static void AdvanceProgramCounter(SemanticState* const state, const unsigned long delta)
{
	state->program_counter += delta;
	state->program_counter &= 0xFFFFFFFF;
}

static void RegressProgramCounter(SemanticState* const state, const unsigned long delta)
{
	state->program_counter -= delta;
	state->program_counter &= 0xFFFFFFFF;
}

static void ProcessInstruction(SemanticState *state, StatementInstruction *instruction)
{
	unsigned int machine_code;
	Size literal_operand_size;
	Operand *operands_to_output[2];
	Operand custom_operands[2];
	const InstructionMetadata *instruction_metadata;
	unsigned int i;

	/* Default to NOP in case errors occur later on and we can't get the correct machine code. */
	machine_code = 0x4E71;

	literal_operand_size = instruction->opcode.size;
	operands_to_output[0] = &instruction->operands[0];
	operands_to_output[1] = &instruction->operands[1];

	/* Instructions on odd addresses cause exceptions on the 68k, so warn the user if they are produced. */
	if ((state->program_counter & 1) != 0)
		SemanticWarning(state, "Instruction is at an odd address.");

	AdvanceProgramCounter(state, 2);

	ResolveInstructionAmbiguity(state, instruction);
	OptimiseInstruction(state, instruction);

	instruction_metadata = &instruction_metadata_all[instruction->opcode.type];

	/* Check if the instruction is a valid size. */
	if ((instruction->opcode.size & ~instruction_metadata->allowed_sizes) != 0)
	{
		const char *newline_byte, *newline_short, *newline_word, *newline_longword, *newline_undefined;
		const char *opcode_byte, *opcode_short, *opcode_word, *opcode_longword, *opcode_undefined;
		const char *size_byte, *size_short, *size_word, *size_longword, *size_undefined;

		newline_byte = newline_short = newline_word = newline_longword = newline_undefined = opcode_byte = opcode_short = opcode_word = opcode_longword = opcode_undefined = size_byte = size_short = size_word = size_longword = size_undefined = "";

		if (instruction_metadata->allowed_sizes & SIZE_BYTE)
		{
			newline_byte = "\n";
			opcode_byte = instruction_metadata->name;
			size_byte = ".B";
		}

		if (instruction_metadata->allowed_sizes & SIZE_SHORT)
		{
			newline_short = "\n";
			opcode_short = instruction_metadata->name;
			size_short = ".S";
		}

		if (instruction_metadata->allowed_sizes & SIZE_WORD)
		{
			newline_word = "\n";
			opcode_word = instruction_metadata->name;
			size_word = ".W";
		}

		if (instruction_metadata->allowed_sizes & SIZE_LONGWORD)
		{
			newline_longword = "\n";
			opcode_longword = instruction_metadata->name;
			size_longword = ".L";
		}

		if (instruction_metadata->allowed_sizes & SIZE_UNDEFINED)
		{
			newline_undefined = "\n";
			opcode_undefined = instruction_metadata->name;
			size_undefined = " (no size)";
		}

		SemanticError(state, "Instruction cannot be this size: allowed sizes are...{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}", newline_byte, opcode_byte, size_byte, newline_short, opcode_short, size_short, newline_word, opcode_word, size_word, newline_longword, opcode_longword, size_longword, newline_undefined, opcode_undefined, size_undefined);
	}
	else
	{
		unsigned int total_operands_wanted;
		unsigned int total_operands_have;

		/* If the size is undefined, and the instruction has only one valid size, then set the size to that. */
		if (instruction->opcode.size == SIZE_UNDEFINED)
		{
			switch (instruction_metadata->allowed_sizes & ~SIZE_UNDEFINED)
			{
				case SIZE_BYTE:
				case SIZE_SHORT:
				case SIZE_WORD:
				case SIZE_LONGWORD:
					instruction->opcode.size = (Size)(instruction_metadata->allowed_sizes & ~SIZE_UNDEFINED);
					break;
			}
		}

		/* Count operands that we want. */
		total_operands_wanted = 0;

		while (total_operands_wanted < CC_COUNT_OF(instruction_metadata->allowed_operands) && instruction_metadata->allowed_operands[total_operands_wanted] != 0)
			++total_operands_wanted;

		/* Count operands that we have. */
		total_operands_have = 0;

		while (total_operands_have < CC_COUNT_OF(instruction->operands) && instruction->operands[total_operands_have].type != 0)
			++total_operands_have;

		if (total_operands_wanted != total_operands_have)
		{
			SemanticError(state, "Instruction has {} operands, but it should have {}.", total_operands_have, total_operands_wanted);
		}
		else
		{
			cc_bool good_operands = cc_true;

			/* Check whether the operands are of the correct types. */
			for (i = 0; i < total_operands_have && instruction_metadata->allowed_operands[i] != 0; ++i)
			{
				if ((instruction->operands[i].type & ~instruction_metadata->allowed_operands[i]) != 0)
				{
					const char *operand_string;

					switch (instruction->operands[i].type)
					{
						default:
						case OPERAND_NONE:
							assert(cc_false);
							/* Fallthrough */
						case OPERAND_DATA_REGISTER:
							operand_string = "a data register";
							break;

						case OPERAND_ADDRESS_REGISTER:
							operand_string = "an address register";
							break;

						case OPERAND_ADDRESS_REGISTER_INDIRECT:
							operand_string = "an indirect address register";
							break;

						case OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT:
							operand_string = "a post-increment indirect address register";
							break;

						case OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT:
							operand_string = "a pre-decrement indirect address register";
							break;

						case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT:
							operand_string = "an indirect address register with displacement";
							break;

						case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
							operand_string = "an indirect address register with displacement and index register";
							break;

						case OPERAND_ADDRESS:
							operand_string = "an address";
							break;

						case OPERAND_ADDRESS_ABSOLUTE:
							operand_string = "an absolute address";
							break;

						case OPERAND_LITERAL:
							operand_string = "a literal";
							break;

						case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT:
							operand_string = "the program counter with displacement";
							break;

						case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
							operand_string = "the program counter with displacement and index register";
							break;

						case OPERAND_STATUS_REGISTER:
							operand_string = "the status register";
							break;

						case OPERAND_CONDITION_CODE_REGISTER:
							operand_string = "the condition code register";
							break;

						case OPERAND_USER_STACK_POINTER_REGISTER:
							operand_string = "the user stack pointer register";
							break;

						case OPERAND_REGISTER_LIST:
							operand_string = "a register list";
							break;
					}

					SemanticError(state, "{} operand cannot be {}.", i == 0 ? "First" : "Second", operand_string);
					good_operands = cc_false;
				}
			}

			if (good_operands)
			{
				/* Determine the machine code for the opcode and perform sanity-checking. */
				switch (instruction->opcode.type)
				{
					case OPCODE_ORI_TO_CCR:
						machine_code = 0x003C;
						break;

					case OPCODE_ORI_TO_SR:
						machine_code = 0x007C;
						break;

					case OPCODE_ORI:
						machine_code = 0x0000;
						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);
						break;

					case OPCODE_ANDI_TO_CCR:
						machine_code = 0x023C;
						break;

					case OPCODE_ANDI_TO_SR:
						machine_code = 0x027C;
						break;

					case OPCODE_ANDI:
						machine_code = 0x0200;
						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);
						break;

					case OPCODE_SUBI:
						machine_code = 0x0400;
						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);
						break;

					case OPCODE_ADDI:
						machine_code = 0x0600;
						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);
						break;

					case OPCODE_EORI_TO_CCR:
						machine_code = 0x0A3C;
						break;

					case OPCODE_EORI_TO_SR:
						machine_code = 0x0A7C;
						break;

					case OPCODE_EORI:
						machine_code = 0x0A00;
						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);
						break;

					case OPCODE_CMPI:
						machine_code = 0x0C00;
						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);
						break;

					case OPCODE_BTST_STATIC:
					case OPCODE_BCHG_STATIC:
					case OPCODE_BCLR_STATIC:
					case OPCODE_BSET_STATIC:
					case OPCODE_BTST_DYNAMIC:
					case OPCODE_BCHG_DYNAMIC:
					case OPCODE_BCLR_DYNAMIC:
					case OPCODE_BSET_DYNAMIC:
						switch (instruction->opcode.type)
						{
							case OPCODE_BTST_STATIC:
							case OPCODE_BCHG_STATIC:
							case OPCODE_BCLR_STATIC:
							case OPCODE_BSET_STATIC:
							{
								unsigned long value;

								if (!ResolveExpression(state, &instruction->operands[0].literal, &value))
									value = 0;

								/* Check whether the literal value will wrap or not, and warn the user if so. */
								if (instruction->operands[1].type == OPERAND_DATA_REGISTER)
								{
									if (value >= 32)
										SemanticWarning(state, "The bit index will be modulo 32.");
								}
								else
								{
									if (value >= 8)
										SemanticWarning(state, "The bit index will be modulo 8.");
								}

								machine_code = 0x0800;

								break;
							}

							case OPCODE_BTST_DYNAMIC:
							case OPCODE_BCHG_DYNAMIC:
							case OPCODE_BCLR_DYNAMIC:
							case OPCODE_BSET_DYNAMIC:
								machine_code = 0x0100 | (instruction->operands[0].main_register << 9);
								break;

							default:
								assert(cc_false);
								break;
						}

						switch (instruction->opcode.type)
						{
							case OPCODE_BTST_STATIC:
							case OPCODE_BTST_DYNAMIC:
								machine_code |= 0x0000;
								break;

							case OPCODE_BCHG_STATIC:
							case OPCODE_BCHG_DYNAMIC:
								machine_code |= 0x0040;
								break;

							case OPCODE_BCLR_STATIC:
							case OPCODE_BCLR_DYNAMIC:
								machine_code |= 0x0080;
								break;

							case OPCODE_BSET_STATIC:
							case OPCODE_BSET_DYNAMIC:
								machine_code |= 0x00C0;
								break;

							default:
								assert(cc_false);
								break;
						}

						/* Check that the opcode size is suitable for the destination operand. */
						if (instruction->operands[1].type == OPERAND_DATA_REGISTER)
						{
							if (instruction->opcode.size != SIZE_LONGWORD && instruction->opcode.size != SIZE_UNDEFINED)
								SemanticError(state, "Instruction must be longword-sized when its destination operand is a data register.");
						}
						else
						{
							if (instruction->opcode.size != SIZE_BYTE && instruction->opcode.size != SIZE_SHORT && instruction->opcode.size != SIZE_UNDEFINED)
								SemanticError(state, "Instruction must be byte-sized when its destination operand is memory.");
						}

						/* Prevent 'btst.l #0,d0' from outputting a longword-sized literal. */
						literal_operand_size = SIZE_WORD;

						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);

						break;

					case OPCODE_MOVEP_TO_REG:
					case OPCODE_MOVEP_FROM_REG:
					{
						unsigned int data_register;
						unsigned int address_register;

						if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
						{
							data_register = instruction->operands[0].main_register;
							address_register = instruction->operands[1].main_register;
						}
						else /*if (instruction->operands[1].type == OPERAND_DATA_REGISTER)*/
						{
							address_register = instruction->operands[0].main_register;
							data_register = instruction->operands[1].main_register;
						}

						machine_code = 0x0108;
						machine_code |= data_register << 9;
						machine_code |= (instruction->operands[0].type == OPERAND_DATA_REGISTER) << 7;
						machine_code |= (instruction->opcode.size == SIZE_LONGWORD) << 6;
						machine_code |= address_register;

						break;
					}

					case OPCODE_MOVEA:
					case OPCODE_MOVE:
						if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER && instruction->opcode.size == SIZE_BYTE)
							SemanticError(state, "This instruction cannot be byte-sized when its source is an address register.");

						switch (instruction->opcode.size)
						{
							case SIZE_BYTE:
							case SIZE_SHORT:
								machine_code = 0x1000;
								break;

							case SIZE_UNDEFINED:
							case SIZE_WORD:
								machine_code = 0x3000;
								break;

							case SIZE_LONGWORD:
								machine_code = 0x2000;
								break;
						}

						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
						machine_code |= ToAlternateEffectiveAddressBits(ConstructEffectiveAddressBits(state, &instruction->operands[1]));

						break;

					case OPCODE_MOVE_FROM_SR:
						machine_code = 0x40C0;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);
						break;

					case OPCODE_MOVE_TO_CCR:
						machine_code = 0x44C0;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
						break;

					case OPCODE_MOVE_TO_SR:
						machine_code = 0x46C0;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
						break;

					case OPCODE_NEGX:
					case OPCODE_CLR:
					case OPCODE_NEG:
					case OPCODE_NOT:
					case OPCODE_TST:
						switch (instruction->opcode.type)
						{
							case OPCODE_NEGX:
								machine_code = 0x4000;
								break;

							case OPCODE_CLR:
								machine_code = 0x4200;
								break;

							case OPCODE_NEG:
								machine_code = 0x4400;
								break;

							case OPCODE_NOT:
								machine_code = 0x4600;
								break;

							case OPCODE_TST:
								machine_code = 0x4A00;
								break;

							default:
								assert(cc_false);
								break;
						}

						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);

						break;

					case OPCODE_EXT:
						machine_code = 0x4880;
						machine_code |= (instruction->opcode.size == SIZE_LONGWORD) << 6;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
						break;

					case OPCODE_NBCD:
						machine_code = 0x4800;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
						break;

					case OPCODE_SWAP:
						machine_code = 0x4840;
						machine_code |= instruction->operands[0].main_register;
						break;

					case OPCODE_PEA:
						machine_code = 0x4840;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
						break;

					case OPCODE_ILLEGAL:
						machine_code = 0x4AFC;
						break;

					case OPCODE_TAS:
						machine_code = 0x4AC0;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
						break;

					case OPCODE_TRAP:
					{
						unsigned long value;

						machine_code = 0x4E40;

						if (!ResolveExpression(state, &instruction->operands[0].literal, &value))
							value = 0;

						if (value > 15)
							SemanticError(state, "The vector cannot be higher than 15.");
						else
							machine_code |= value;

						/* The operand is embedded directly into the machine code, so we don't need to output it separately. */
						operands_to_output[0] = nullptr;

						break;
					}

					case OPCODE_LINK:
						machine_code = 0x4E50;
						machine_code |= instruction->operands[0].main_register;
						break;

					case OPCODE_UNLK:
						machine_code = 0x4E58;
						machine_code |= instruction->operands[0].main_register;
						break;

					case OPCODE_MOVE_TO_USP:
						machine_code = 0x4E60;
						machine_code |= instruction->operands[0].main_register;
						break;

					case OPCODE_MOVE_FROM_USP:
						machine_code = 0x4E68;
						machine_code |= instruction->operands[1].main_register;
						break;

					case OPCODE_RESET:
						machine_code = 0x4E70;
						break;

					case OPCODE_NOP:
						machine_code = 0x4E71;
						break;

					case OPCODE_STOP:
						machine_code = 0x4E72;
						break;

					case OPCODE_RTE:
						machine_code = 0x4E73;
						break;

					case OPCODE_RTS:
						machine_code = 0x4E75;
						break;

					case OPCODE_TRAPV:
						machine_code = 0x4E76;
						break;

					case OPCODE_RTR:
						machine_code = 0x4E77;
						break;

					case OPCODE_JSR:
						machine_code = 0x4E80;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
						break;

					case OPCODE_JMP:
						machine_code = 0x4EC0;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
						break;

					case OPCODE_MOVEM_TO_REGS:
					case OPCODE_MOVEM_FROM_REGS:
						machine_code = 0x4880;
						machine_code |= (instruction->opcode.size == SIZE_LONGWORD) << 6;

						if (instruction->opcode.type == OPCODE_MOVEM_TO_REGS)
						{
							machine_code |= 1 << 10;

							/* Place register list before the other operand. */
							operands_to_output[0] = &instruction->operands[1];
							operands_to_output[1] = &instruction->operands[0];
						}

						machine_code |= ConstructEffectiveAddressBits(state, operands_to_output[1]);

						custom_operands[0] = std::move(*operands_to_output[0]);
						operands_to_output[0] = &custom_operands[0];

						/* Convert lone registers to register_lists. */
						if (custom_operands[0].type == OPERAND_DATA_REGISTER)
						{
							custom_operands[0].type = OPERAND_REGISTER_LIST;
							custom_operands[0].main_register = 1 << (0 + custom_operands[0].main_register);
						}
						else if (custom_operands[0].type == OPERAND_ADDRESS_REGISTER)
						{
							custom_operands[0].type = OPERAND_REGISTER_LIST;
							custom_operands[0].main_register = 1 << (8 + custom_operands[0].main_register);
						}

						if (operands_to_output[1]->type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
						{
							/* Reverse the register list. */
							static const unsigned int reverse_nibble[0x10] = {0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE, 0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF};

							custom_operands[0].main_register = reverse_nibble[(custom_operands[0].main_register >> (4 * 0)) & 0xF] << (4 * 3)
											 | reverse_nibble[(custom_operands[0].main_register >> (4 * 1)) & 0xF] << (4 * 2)
											 | reverse_nibble[(custom_operands[0].main_register >> (4 * 2)) & 0xF] << (4 * 1)
											 | reverse_nibble[(custom_operands[0].main_register >> (4 * 3)) & 0xF] << (4 * 0);
						}

						machine_code |= ConstructEffectiveAddressBits(state, operands_to_output[1]);

						break;

					case OPCODE_LEA:
					case OPCODE_CHK:
					case OPCODE_DIVU:
					case OPCODE_DIVS:
					case OPCODE_MULU:
					case OPCODE_MULS:
						switch (instruction->opcode.type)
						{
							case OPCODE_LEA:
								machine_code = 0x41C0;
								break;

							case OPCODE_CHK:
								machine_code = 0x4180;
								break;

							case OPCODE_DIVU:
								machine_code = 0x80C0;
								break;

							case OPCODE_DIVS:
								machine_code = 0x81C0;
								break;

							case OPCODE_MULU:
								machine_code = 0xC0C0;
								break;

							case OPCODE_MULS:
								machine_code = 0xC1C0;
								break;

							default:
								break;
						}

						machine_code |= instruction->operands[1].main_register << 9;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);

						break;

					case OPCODE_ADDQ:
					case OPCODE_SUBQ:
					{
						unsigned long value;

						if (instruction->opcode.type == OPCODE_ADDQ)
							machine_code = 0x5000;
						else /*if (instruction->opcode.type == OPCODE_SUBQ)*/
							machine_code = 0x5100;

						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);

						if (!ResolveExpression(state, &instruction->operands[0].literal, &value))
							value = 1;

						if (value < 1 || value > 8)
							SemanticError(state, "The value to add/subtract cannot be lower than 1 or higher than 8.");

						machine_code |= (value & 7) << 9;

						/* Skip the immediate operand since that goes in the machine code instead. */
						operands_to_output[0] = nullptr;

						break;
					}

					case OPCODE_Scc:
						machine_code = 0x50C0;
						machine_code |= instruction->opcode.condition << 8;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
						break;

					case OPCODE_DBcc:
					{
						unsigned long value;

						machine_code = 0x50C8;
						machine_code |= instruction->opcode.condition << 8;
						machine_code |= instruction->operands[0].main_register;

						/* Obtain the destination address. */
						if (!ResolveExpression(state, &instruction->operands[1].literal, &value))
							value = state->program_counter - 2;

						/* Construct a custom operand to hold the destination offset. */
						operands_to_output[0] = &custom_operands[0];
						operands_to_output[1] = nullptr;
						custom_operands[0].type = OPERAND_LITERAL;

						/* Calculate the destination offset. */
						if (value >= state->program_counter)
						{
							const unsigned long offset = value - state->program_counter;

							if (offset > 0x7FFF)
								SemanticError(state, "The destination is too far away: it must be less than $8000 bytes after the start of the instruction, but instead it was ${:X} bytes away.", offset);

							custom_operands[0].literal.emplace<ExpressionNumber>(offset);
						}
						else
						{
							const unsigned long offset = state->program_counter - value;

							if (offset > 0x8000)
								SemanticError(state, "The destination is too far away: it must be less than $8001 bytes before the start of the instruction, but instead it was ${:X} bytes away.", offset);

							custom_operands[0].literal.emplace<ExpressionNumber>(0 - offset);
						}

						if ((value & 1) != 0)
							SemanticWarning(state, "Branch leads to odd address 0x{:X}.", value);

						break;
					}

					case OPCODE_BRA:
					case OPCODE_BSR:
					case OPCODE_Bcc:
					{
						unsigned long offset;
						unsigned long value;

						machine_code = 0x6000;

						switch (instruction->opcode.type)
						{
							case OPCODE_BRA:
								machine_code |= 0x0000;
								break;

							case OPCODE_BSR:
								machine_code |= 0x0100;
								break;

							case OPCODE_Bcc:
								machine_code |= instruction->opcode.condition << 8;
								break;

							default:
								assert(cc_false);
								break;
						}

						/* Obtain the destination address. */
						if (!ResolveExpression(state, &instruction->operands[0].literal, &value))
							value = state->program_counter - 2;

						/* Calculate the destination offset. */
						if (value >= state->program_counter)
						{
							offset = value - state->program_counter;

							if (instruction->opcode.size == SIZE_BYTE || instruction->opcode.size == SIZE_SHORT)
							{
								if (offset == 0)
									SemanticError(state, "The destination cannot be 0 bytes away when using a short-sized branch: use a word-sized branch instead.");
								else if (offset > 0x7F)
									SemanticError(state, "The destination is too far away: it must be less than $80 bytes after the start of the instruction, but instead it was ${:X} bytes away.", offset);
							}
							else
							{
								if (offset > 0x7FFF)
									SemanticError(state, "The destination is too far away: it must be less than $8000 bytes after the start of the instruction, but instead it was ${:X} bytes away.", offset);
							}
						}
						else
						{
							offset = state->program_counter - value;

							if (instruction->opcode.size == SIZE_BYTE || instruction->opcode.size == SIZE_SHORT)
							{
								if (offset > 0x80)
									SemanticError(state, "The destination is too far away: it must be less than $81 bytes before the start of the instruction, but instead it was ${:X} bytes away.", offset);
							}
							else
							{
								if (offset > 0x8000)
									SemanticError(state, "The destination is too far away: it must be less than $8001 bytes before the start of the instruction, but instead it was ${:X} bytes away.", offset);
							}

							offset = 0 - offset;
						}

						if ((value & 1) != 0)
							SemanticWarning(state, "Branch leads to odd address 0x{:X}.", value);

						if (instruction->opcode.size == SIZE_BYTE || instruction->opcode.size == SIZE_SHORT)
						{
							machine_code |= offset & 0xFF;

							/* The operand is embedded directly into the machine code, so we don't need to output it separately. */
							operands_to_output[0] = nullptr;
						}
						else
						{
							/* Construct a custom operand to hold the destination offset. */
							operands_to_output[0] = &custom_operands[0];
							custom_operands[0].type = OPERAND_LITERAL;
							custom_operands[0].literal.emplace<ExpressionNumber>(offset);
						}

						break;
					}

					case OPCODE_MOVEQ:
					{
						unsigned long value;

						machine_code = 0x7000;

						/* Obtain the value to be moved. */
						if (!ResolveExpression(state, &instruction->operands[0].literal, &value))
							value = 0;

						if (value > 0x7F && value < 0xFFFFFF80)
							SemanticError(state, "Value is too large: it must be between -$80 and $7F, or $FFFFFF80 and $FFFFFFFF.");

						machine_code |= value & 0xFF;

						machine_code |= instruction->operands[1].main_register << 9;

						/* MOVEQ's operands are embedded directly into the machine code, so we don't need to output them separately. */
						operands_to_output[0] = nullptr;
						operands_to_output[1] = nullptr;

						break;
					}

					case OPCODE_SBCD_DATA_REGS:
					case OPCODE_SBCD_ADDRESS_REGS:
					case OPCODE_SUBX_DATA_REGS:
					case OPCODE_SUBX_ADDRESS_REGS:
					case OPCODE_ABCD_DATA_REGS:
					case OPCODE_ABCD_ADDRESS_REGS:
					case OPCODE_ADDX_DATA_REGS:
					case OPCODE_ADDX_ADDRESS_REGS:
						switch (instruction->opcode.type)
						{
							case OPCODE_SBCD_DATA_REGS:
								machine_code = 0x8100;
								break;

							case OPCODE_SBCD_ADDRESS_REGS:
								machine_code = 0x8108;
								break;

							case OPCODE_SUBX_DATA_REGS:
								machine_code = 0x9100;
								break;

							case OPCODE_SUBX_ADDRESS_REGS:
								machine_code = 0x9108;
								break;

							case OPCODE_ABCD_DATA_REGS:
								machine_code = 0xC100;
								break;

							case OPCODE_ABCD_ADDRESS_REGS:
								machine_code = 0xC108;
								break;

							case OPCODE_ADDX_DATA_REGS:
								machine_code = 0xD100;
								break;

							case OPCODE_ADDX_ADDRESS_REGS:
								machine_code = 0xD108;
								break;

							default:
								assert(cc_false);
								break;
						}

						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= instruction->operands[0].main_register << 0;
						machine_code |= instruction->operands[1].main_register << 9;

						break;

					case OPCODE_OR_TO_REG:
					case OPCODE_OR_FROM_REG:
					case OPCODE_SUB_TO_REG:
					case OPCODE_SUB_FROM_REG:
					case OPCODE_CMP:
					case OPCODE_AND_TO_REG:
					case OPCODE_AND_FROM_REG:
					case OPCODE_ADD_TO_REG:
					case OPCODE_ADD_FROM_REG:
						if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER && instruction->opcode.size == SIZE_BYTE)
							SemanticError(state, "This instruction cannot be byte-sized when its destination is an address register.");

						switch (instruction->opcode.type)
						{
							case OPCODE_OR_TO_REG:
								machine_code = 0x8000;
								break;

							case OPCODE_OR_FROM_REG:
								machine_code = 0x8100;
								break;

							case OPCODE_SUB_TO_REG:
								machine_code = 0x9000;
								break;

							case OPCODE_SUB_FROM_REG:
								machine_code = 0x9100;
								break;

							case OPCODE_CMP:
								machine_code = 0xB000;
								break;

							case OPCODE_AND_TO_REG:
								machine_code = 0xC000;
								break;

							case OPCODE_AND_FROM_REG:
								machine_code = 0xC100;
								break;

							case OPCODE_ADD_TO_REG:
								machine_code = 0xD000;
								break;

							case OPCODE_ADD_FROM_REG:
								machine_code = 0xD100;
								break;

							default:
								assert(cc_false);
								break;
						}

						machine_code |= ConstructSizeBits(instruction->opcode.size);

						if (instruction->operands[1].type == OPERAND_DATA_REGISTER)
						{
							machine_code |= instruction->operands[1].main_register << 9;
							machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
						}
						else
						{
							machine_code |= instruction->operands[0].main_register << 9;
							machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);
						}

						break;

					case OPCODE_SUBA:
					case OPCODE_CMPA:
					case OPCODE_ADDA:
						switch (instruction->opcode.type)
						{
							case OPCODE_SUBA:
								machine_code = 0x90C0;
								break;

							case OPCODE_CMPA:
								machine_code = 0xB0C0;
								break;

							case OPCODE_ADDA:
								machine_code = 0xD0C0;
								break;

							default:
								assert(cc_false);
								break;
						}

						machine_code |= (instruction->opcode.size == SIZE_LONGWORD) << 8;
						machine_code |= instruction->operands[1].main_register << 9;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);

						break;

					case OPCODE_EOR:
						machine_code = 0xB100;
						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= instruction->operands[0].main_register << 9;
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);
						break;

					case OPCODE_CMPM:
						machine_code = 0xB108;
						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= instruction->operands[0].main_register << 0;
						machine_code |= instruction->operands[1].main_register << 9;

						break;

					case OPCODE_EXG:
						machine_code = 0xC100;

						if (instruction->operands[0].type == OPERAND_DATA_REGISTER && instruction->operands[1].type == OPERAND_DATA_REGISTER)
						{
							machine_code |= 0x0040;
							machine_code |= instruction->operands[0].main_register << 9;
							machine_code |= instruction->operands[1].main_register << 0;
						}
						else if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER && instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
						{
							machine_code |= 0x0048;
							machine_code |= instruction->operands[0].main_register << 9;
							machine_code |= instruction->operands[1].main_register << 0;
						}
						else if (instruction->operands[0].type == OPERAND_DATA_REGISTER && instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
						{
							machine_code |= 0x0088;
							machine_code |= instruction->operands[0].main_register << 9;
							machine_code |= instruction->operands[1].main_register << 0;
						}
						else /*if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER && instruction->operands[1].type == OPERAND_DATA_REGISTER)*/
						{
							machine_code |= 0x0088;
							machine_code |= instruction->operands[1].main_register << 9;
							machine_code |= instruction->operands[0].main_register << 0;
						}

						break;

					/* TODO - Split these opcode: this code is a bloody mess. */
					case OPCODE_ASL_STATIC:
					case OPCODE_ASR_STATIC:
					case OPCODE_LSL_STATIC:
					case OPCODE_LSR_STATIC:
					case OPCODE_ROXL_STATIC:
					case OPCODE_ROXR_STATIC:
					case OPCODE_ROL_STATIC:
					case OPCODE_ROR_STATIC:
					case OPCODE_ASL_DYNAMIC:
					case OPCODE_ASR_DYNAMIC:
					case OPCODE_LSL_DYNAMIC:
					case OPCODE_LSR_DYNAMIC:
					case OPCODE_ROXL_DYNAMIC:
					case OPCODE_ROXR_DYNAMIC:
					case OPCODE_ROL_DYNAMIC:
					case OPCODE_ROR_DYNAMIC:
					case OPCODE_ASL_SINGLE:
					case OPCODE_ASR_SINGLE:
					case OPCODE_LSL_SINGLE:
					case OPCODE_LSR_SINGLE:
					case OPCODE_ROXL_SINGLE:
					case OPCODE_ROXR_SINGLE:
					case OPCODE_ROL_SINGLE:
					case OPCODE_ROR_SINGLE:
					{
						unsigned int identifier = 0;

						switch (instruction->opcode.type)
						{
							case OPCODE_ASL_STATIC:
							case OPCODE_ASR_STATIC:
							case OPCODE_ASL_DYNAMIC:
							case OPCODE_ASR_DYNAMIC:
							case OPCODE_ASL_SINGLE:
							case OPCODE_ASR_SINGLE:
								identifier = 0;
								break;

							case OPCODE_LSL_STATIC:
							case OPCODE_LSR_STATIC:
							case OPCODE_LSL_DYNAMIC:
							case OPCODE_LSR_DYNAMIC:
							case OPCODE_LSL_SINGLE:
							case OPCODE_LSR_SINGLE:
								identifier = 1;
								break;

							case OPCODE_ROXL_STATIC:
							case OPCODE_ROXR_STATIC:
							case OPCODE_ROXL_DYNAMIC:
							case OPCODE_ROXR_DYNAMIC:
							case OPCODE_ROXL_SINGLE:
							case OPCODE_ROXR_SINGLE:
								identifier = 2;
								break;

							case OPCODE_ROL_STATIC:
							case OPCODE_ROR_STATIC:
							case OPCODE_ROL_DYNAMIC:
							case OPCODE_ROR_DYNAMIC:
							case OPCODE_ROL_SINGLE:
							case OPCODE_ROR_SINGLE:
								identifier = 3;
								break;

							default:
								assert(cc_false);
								break;
						}

						machine_code = 0xE000;

						switch (instruction->opcode.type)
						{
							case OPCODE_ASR_STATIC:
							case OPCODE_ASR_DYNAMIC:
							case OPCODE_ASR_SINGLE:
							case OPCODE_LSR_STATIC:
							case OPCODE_LSR_DYNAMIC:
							case OPCODE_LSR_SINGLE:
							case OPCODE_ROXR_STATIC:
							case OPCODE_ROXR_DYNAMIC:
							case OPCODE_ROXR_SINGLE:
							case OPCODE_ROR_STATIC:
							case OPCODE_ROR_DYNAMIC:
							case OPCODE_ROR_SINGLE:
								machine_code |= 0x0000;
								break;

							case OPCODE_ASL_STATIC:
							case OPCODE_ASL_DYNAMIC:
							case OPCODE_ASL_SINGLE:
							case OPCODE_LSL_STATIC:
							case OPCODE_LSL_DYNAMIC:
							case OPCODE_LSL_SINGLE:
							case OPCODE_ROXL_STATIC:
							case OPCODE_ROXL_DYNAMIC:
							case OPCODE_ROXL_SINGLE:
							case OPCODE_ROL_STATIC:
							case OPCODE_ROL_DYNAMIC:
							case OPCODE_ROL_SINGLE:
								machine_code |= 0x0100;
								break;

							default:
								assert(cc_false);
								break;
						}

						switch (instruction->opcode.type)
						{
							case OPCODE_ASL_STATIC:
							case OPCODE_ASR_STATIC:
							case OPCODE_LSL_STATIC:
							case OPCODE_LSR_STATIC:
							case OPCODE_ROXL_STATIC:
							case OPCODE_ROXR_STATIC:
							case OPCODE_ROL_STATIC:
							case OPCODE_ROR_STATIC:
							{
								unsigned long value;

								if (!ResolveExpression(state, &instruction->operands[0].literal, &value))
									value = 0;

								if (value > 8 || value < 1)
									SemanticError(state, "The shift value must not be greater than 8 or lower than 1.");

								machine_code |= (value & 7) << 9;

								machine_code |= identifier << 3;
								machine_code |= instruction->operands[1].main_register << 0;
								machine_code |= ConstructSizeBits(instruction->opcode.size);

								/* Skip the immediate operand since that goes in the machine code instead. */
								operands_to_output[0] = nullptr;

								break;
							}

							case OPCODE_ASL_DYNAMIC:
							case OPCODE_ASR_DYNAMIC:
							case OPCODE_LSL_DYNAMIC:
							case OPCODE_LSR_DYNAMIC:
							case OPCODE_ROXL_DYNAMIC:
							case OPCODE_ROXR_DYNAMIC:
							case OPCODE_ROL_DYNAMIC:
							case OPCODE_ROR_DYNAMIC:
								machine_code |= identifier << 3;
								machine_code |= instruction->operands[0].main_register << 9;
								machine_code |= 0x0020;
								machine_code |= instruction->operands[1].main_register << 0;
								machine_code |= ConstructSizeBits(instruction->opcode.size);
								break;

							case OPCODE_ASL_SINGLE:
							case OPCODE_ASR_SINGLE:
							case OPCODE_LSL_SINGLE:
							case OPCODE_LSR_SINGLE:
							case OPCODE_ROXL_SINGLE:
							case OPCODE_ROXR_SINGLE:
							case OPCODE_ROL_SINGLE:
							case OPCODE_ROR_SINGLE:
								machine_code |= identifier << 9;
								machine_code |= 0x00C0;
								machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[0]);
								break;

							default:
								assert(cc_false);
								break;
						}

						break;
					}
				}
			}
		}
	}

	/* Output the machine code for the opcode. */
	for (i = 2; i-- > 0; )
		OutputByte(state, (machine_code >> (8 * i)) & 0xFF);

	/* Output the data for the operands. */
	for (i = 0; i < CC_COUNT_OF(operands_to_output); ++i)
	{
		Operand* const operand = operands_to_output[i];

		if (operand != nullptr)
		{
			switch (operand->type)
			{
				case OPERAND_NONE:
				case OPERAND_DATA_REGISTER:
				case OPERAND_ADDRESS_REGISTER:
				case OPERAND_ADDRESS_REGISTER_INDIRECT:
				case OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT:
				case OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT:
				case OPERAND_STATUS_REGISTER:
				case OPERAND_CONDITION_CODE_REGISTER:
				case OPERAND_USER_STACK_POINTER_REGISTER:
					break;

				case OPERAND_ADDRESS:
				case OPERAND_ADDRESS_ABSOLUTE:
				case OPERAND_LITERAL:
				case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT:
				case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
				case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT:
				case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
				{
					unsigned int bytes_to_write = 2;
					unsigned long value;

					if (!ResolveExpression(state, &operand->literal, &value))
					{
						if (operand->type == OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT || operand->type == OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER)
							value = state->program_counter; /* Prevent out-of-range displacements later on. */
						else
							value = 0;
					}

					switch (operand->type)
					{
						case OPERAND_NONE:
							assert(cc_false);
							break;

						case OPERAND_DATA_REGISTER:
						case OPERAND_ADDRESS_REGISTER:
						case OPERAND_ADDRESS_REGISTER_INDIRECT:
						case OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT:
						case OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT:
						case OPERAND_STATUS_REGISTER:
						case OPERAND_CONDITION_CODE_REGISTER:
						case OPERAND_USER_STACK_POINTER_REGISTER:
						case OPERAND_REGISTER_LIST:
							break;

						case OPERAND_ADDRESS:
						case OPERAND_ADDRESS_ABSOLUTE:
							switch (operand->size)
							{
								case SIZE_BYTE:
								case SIZE_SHORT:
									SemanticError(state, "The address cannot be byte-sized.");
									bytes_to_write = 2;
									break;

								case SIZE_WORD:
									bytes_to_write = 2;

									if (value >= 0x8000 && value < 0xFFFF8000)
										SemanticError(state, "Word-sized addresses cannot be higher than $7FFF or lower than $FFFF8000, but was ${:X}.", value);

									break;

								case SIZE_UNDEFINED:
								case SIZE_LONGWORD:
									bytes_to_write = 4;
									break;
							}

							break;

						case OPERAND_LITERAL:
							switch (literal_operand_size)
							{
								case SIZE_BYTE:
								case SIZE_SHORT:
									bytes_to_write = 2;

									if (value >= 0x100 && value < 0xFFFFFF00)
										SemanticError(state, "Byte-sized literals cannot be larger than $FF or smaller than -$100, but was ${:X}.", value);

									value &= 0xFF;

									break;

								case SIZE_UNDEFINED:
								case SIZE_WORD:
									bytes_to_write = 2;

									if (value >= 0x10000 && value < 0xFFFF0000)
										SemanticError(state, "Word-sized literals cannot be larger than $FFFF or smaller than -$10000, but was ${:X}.", value);

									break;

								case SIZE_LONGWORD:
									bytes_to_write = 4;
									break;
							}

							break;

						case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
							/* Turn the address into a displacement. */
							value -= state->program_counter;
							/* Fallthrough */
						case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
							bytes_to_write = 2;

							if (value >= 0x80 && value < 0xFFFFFF80)
								SemanticError(state, "Displacement values cannot be larger than $7F or smaller than -$80, but was ${:X}.", value);

							if (operand->size == SIZE_BYTE || operand->size == SIZE_SHORT)
								SemanticError(state, "Index registers cannot be byte-sized.");

							value &= 0xFF;

							value |= operand->index_register << 12;

							if (operand->size == SIZE_LONGWORD)
								value |= 0x800;

							if (operand->index_register_is_address_register)
								value |= 0x8000;

							break;

						case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT:
							value -= state->program_counter;
							/* Fallthrough */
						case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT:
							bytes_to_write = 2;

							if (value >= 0x8000 && value < 0xFFFF8000)
								SemanticError(state, "Displacement values cannot be larger than $7FFF or smaller than -$8000, but was ${:X}.", value);

							break;
					}

					AdvanceProgramCounter(state, bytes_to_write);

					while (bytes_to_write-- > 0)
						OutputByte(state, (value >> (8 * bytes_to_write)) & 0xFF);

					break;
				}

				case OPERAND_REGISTER_LIST:
				{
					unsigned int bytes_to_write;

					AdvanceProgramCounter(state, 2);

					for (bytes_to_write = 2; bytes_to_write-- > 0; )
						OutputByte(state, (operand->main_register >> (8 * bytes_to_write)) & 0xFF);

					break;
				}
			}
		}
	}
}

static void PushSubstitute(SemanticState* const state, const std::string_view &identifier, const std::string_view &value)
{
	SymbolDictionary *dictionary;
	SymbolDictionary::Entry* const dictionary_entry = LookupSymbolAndCreateIfNotExist<SymbolStringConstant>(state, identifier, &dictionary);

	if (dictionary_entry != nullptr)
	{
		std::visit(
			CC_VisitHelper{
				[&](SymbolNone&)
				{
					auto &symbol = dictionary_entry->emplace<SymbolStringConstant>();
					symbol.value = value;
					auto &substitute = dictionary == &state->dictionary ? state->substitutions : state->macro.substitutions;
					substitute.PushSubstitute(identifier, symbol.value);
				},
				[&](SymbolStringConstant &symbol)
				{
					symbol.value = value;
				},
				[&](auto&)
				{
					// Should never be ran, since `LookupSymbolAndCreateIfNotExist` rejects mismatched types.
					assert(cc_false);
				},
			}, *dictionary_entry
		);
	}
}

static void PushMacroArgumentSubstitutions(SemanticState* const state)
{
	const std::string_view blank_argument;

	for (std::size_t argument_index = 0; argument_index < state->macro.metadata->parameter_names.size(); ++argument_index)
	{
		const std::string_view &parameter = state->macro.metadata->parameter_names[argument_index];
		const std::string_view &argument = argument_index >= state->macro.total_arguments ? blank_argument : state->macro.argument_list[argument_index];

		SymbolDictionary::Entry &dictionary_entry = state->macro.dictionary->LookUpAndCreateIfNotExist(parameter);

		/* TODO: De-duplicate this with the 'PushSubstitute' function. */
		std::visit(
			CC_VisitHelper{
				[&](SymbolNone&)
				{
					auto &symbol = dictionary_entry.emplace<SymbolStringConstant>();
					symbol.value = argument;
					state->macro.substitutions.PushSubstitute(parameter, symbol.value);
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
					SemanticError(state, "Attempted to redefine symbol '{}' as a string constant.", parameter);
				},
			}, dictionary_entry
		);
	}
}

static void OutputDcValue(SemanticState *state, const Size size, unsigned long value)
{
	unsigned int bytes_to_write = 0;

	switch (size)
	{
		case SIZE_BYTE:
		case SIZE_SHORT:
			if (value > 0xFF && value < 0xFFFFFF00)
				SemanticError(state, "Value cannot be higher than $FF or lower than -$100, but was ${:X}.", value);

			bytes_to_write = 1;

			break;

		case SIZE_WORD:
			if (value > 0xFFFF && value < 0xFFFF0000)
				SemanticError(state, "Value cannot be higher than $FFFF or lower than -$10000, but was ${:X}.", value);

			bytes_to_write = 2;

			break;

		case SIZE_LONGWORD:
			bytes_to_write = 4;
			break;

		case SIZE_UNDEFINED:
			/* Should never occur. */
			assert(cc_false);
			break;
	}

	AdvanceProgramCounter(state, bytes_to_write);

	while (bytes_to_write-- != 0)
		OutputByte(state, (value >> (bytes_to_write * 8)) & 0xFF);
}

static void PerformEven(SemanticState* const state)
{
	/* Pad to the nearest even address. */
	if (state->program_counter & 1)
	{
		AdvanceProgramCounter(state, 1);
		OutputByte(state, 0);
	}
}

static cc_bool ShouldPerformAutomaticEven(SemanticState* const state, const Size size)
{
	return state->options->automatic_even && size != SIZE_BYTE;
}

static void ProcessDc(SemanticState *state, StatementDc *dc)
{
	if (ShouldPerformAutomaticEven(state, dc->size))
		PerformEven(state);

	for (auto &expression : dc->values)
	{
		const auto expression_string = std::get_if<ExpressionString>(&expression);

		if (expression_string != nullptr && (dc->size == SIZE_BYTE || dc->size == SIZE_SHORT))
		{
			/* If this is a 'dc.b', and the value is a raw string, then output each character as a single byte. */
			/* This allows 'dc.b' to be used to embed strings into the output. */
			for (const char character : expression_string->string)
				OutputDcValue(state, dc->size, character);
		}
		else
		{
			unsigned long value;

			/* Update the program counter symbol in between values, to keep it up to date. */
			state->program_counter_of_expression = state->program_counter;

			if (!ResolveExpression(state, &expression, &value))
				value = 0;

			OutputDcValue(state, dc->size, value);
		}
	}
}

static void ProcessDcb(SemanticState *state, StatementDcb *dcb)
{
	unsigned long repetitions;

	if (ShouldPerformAutomaticEven(state, dcb->size))
		PerformEven(state);

	if (!ResolveExpression(state, &dcb->repetitions, &repetitions))
	{
		SemanticError(state, "Repetition value must be evaluable on first pass.");
	}
	else
	{
		unsigned long value;
		unsigned long i;

		if (!ResolveExpression(state, &dcb->value, &value))
			value = 0;

		for (i = 0; i < repetitions; ++i)
			OutputDcValue(state, dcb->size, value);
	}
}

static void ProcessDs(SemanticState *state, StatementDs *ds)
{
	unsigned long length;

	if (ShouldPerformAutomaticEven(state, ds->size))
		PerformEven(state);

	if (!ResolveExpression(state, &ds->length, &length))
	{
		SemanticError(state, "Repetition value must be evaluable on first pass.");
	}
	else
	{
		unsigned long i;

		for (i = 0; i < length; ++i)
			OutputDcValue(state, ds->size, 0);
	}
}

static void ProcessInclude(SemanticState *state, const StatementInclude *include)
{
	std::ifstream input_stream(ToStandardPath(include->path));

	if (!input_stream.is_open())
	{
		SemanticError(state, "File '{}' could not be opened.", include->path);
		return;
	}

	std::istream* const previous_input_stream = state->input_stream;

	/* Add file path and line number to the location list. */
	state->location.emplace_front(include->path, 0);

	state->input_stream = &input_stream;

	AssembleFile(state);

	state->input_stream = previous_input_stream;

	state->location.pop_front();
}

static void ProcessIncbin(SemanticState *state, StatementIncbin *incbin)
{
	std::ifstream input_file(ToStandardPath(incbin->path), input_file.binary);

	if (!input_file.is_open())
	{
		SemanticError(state, "File '{}' could not be opened.", incbin->path);
		return;
	}

	unsigned long value;

	if (!ResolveExpression(state, &incbin->start, &value))
	{
		SemanticError(state, "Start value must be evaluable on the first pass.");
		value = 0;
	}

	input_file.seekg(value);

	if (input_file.fail())
		SemanticError(state, "Start value is not a valid location in the file.");

	if (incbin->has_length)
	{
		unsigned long length;
		unsigned long i;

		if (!ResolveExpression(state, &incbin->length, &length))
		{
			SemanticError(state, "Length value must be evaluable on the first pass.");
			length = 0;
		}

		for (i = 0; i < length; ++i)
		{
			const int character = input_file.get();

			if (input_file.gcount() == 0)
			{
				SemanticError(state, "Length value is larger than the remaining number of bytes in the file.");
				break;
			}

			AdvanceProgramCounter(state, 1);
			OutputWriteSegmentByte(state, character);
		}
	}
	else
	{
		/* TODO: Batch bytes to make this faster? */
		for (;;)
		{
			const int character = input_file.get();

			if (input_file.gcount() == 0)
				break;

			AdvanceProgramCounter(state, 1);
			OutputWriteSegmentByte(state, character);
		}
	}
}

static cc_bool ReadSourceLineRaw(SemanticState *state, std::size_t &line_buffer_write_position)
{
	if (state->input_stream->eof())
		return cc_false;

	for (;;)
	{
		state->input_stream->getline(&state->line_buffer[line_buffer_write_position], state->line_buffer.size() - line_buffer_write_position);
		line_buffer_write_position += state->input_stream->gcount();

		if (state->input_stream->eof())
			break;

		/* Do not count the newline. */
		--line_buffer_write_position;

		/* Failure indicates the buffer was not big enough; if it
		   is not set, then we have read a full line and can exit. */
		if (!state->input_stream->fail())
			break;

		/* Make more room for the line. */
		state->line_buffer.resize(state->line_buffer.size() * 2);

		/* Now loop back around and read more of the line! */
	}

	return cc_true;
}

static cc_bool ReadSourceLine(SemanticState *state, std::string_view &source_line)
{
	cc_bool data_read = cc_false;
	std::size_t line_buffer_read_position = 0, line_buffer_write_position = 0;

	while (ReadSourceLineRaw(state, line_buffer_write_position))
	{
		char quote_character;

		data_read = cc_true;

		/* Find where the code ends and a comment begins.
		   Note that terminating at ';' prevents a trailing '&' from being recognised
		   and causing a line to be continued on the next line.
		   This is needed for compatibility with S.N. 68k (asm68k). */
		/* TODO: Does S.N. 68k actually concatenate when a '&' occurs before a comment? */
		quote_character = '\0';

		for (; line_buffer_read_position < line_buffer_write_position; ++line_buffer_read_position)
		{
			const char character = state->line_buffer[line_buffer_read_position];

			if (quote_character == '\0')
			{
				if (character == '"' || character == '\'')
					quote_character = character;
				else if (character == ';')
					break;
			}
			else if (character == quote_character)
			{
				quote_character = '\0';
			}
		}

		/* An '&' at the end of a line is like a '\' at the end of a line in C:
		   it signals that the current line is continued on the next line. */
		if (line_buffer_read_position == 0 || state->line_buffer[line_buffer_read_position - 1] != '&')
			break;

		/* Discard the '&'. */
		--line_buffer_read_position;
		--line_buffer_write_position;

		/* Go back and get another line. */
	}

	source_line = {state->line_buffer.data(), line_buffer_read_position};

	return data_read;
}

static void ProcessRept(SemanticState *state, StatementRept *rept)
{
	/* Enter REPT mode. */
	auto &mode = state->mode.emplace<SemanticState::ModeRept>();

	if (!ResolveExpression(state, &rept->repetitions, &mode.repetitions))
	{
		SemanticError(state, "Repetition value must be evaluable on the first pass.");
		mode.repetitions = 1;
	}

	mode.line_number = state->location.front().line_number;
	mode.nesting = 0;
}

static void ProcessMacro(SemanticState *state, StatementMacro *macro, const std::string_view &label, cc_bool is_short)
{
	/* Enter MACRO mode. */
	auto &mode = state->mode.emplace<SemanticState::ModeMacro>();

	mode.metadata.name = label;
	mode.line_number = state->location.front().line_number;
	mode.metadata.parameter_names = std::move(macro->parameter_names);
	mode.metadata.is_short = is_short;
	mode.metadata.uses_label = macro->uses_label;
}

static void ProcessIf(SemanticState *state, Expression *expression)
{
	unsigned long value;

	++state->current_if_level;

	if (!ResolveExpression(state, expression, &value))
	{
		SemanticError(state, "Condition must be evaluable on the first pass.");
		value = 1;
	}

	/* If this condition is false, then mark this as the false if-level. */
	if (value == 0)
		state->false_if_level = state->current_if_level;

	state->true_already_found = value != 0;
}

static void ProcessStatement(SemanticState *state, Statement *statement, const std::string_view &label)
{
	/* Bizarrely, depending on the context, the program counter can be one
	   of two values, so we need to maintain two separate copies of it. */
	state->program_counter_of_statement = state->program_counter;
	state->program_counter_of_expression = state->program_counter;

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
		StatementPushO,
		StatementPopO,
		StatementOpt,
		StatementPushP,
		StatementPopP,
		StatementShift,
		StatementMExit,
		StatementLocal,
		StatementPurge
	>(*statement))
	{
		if (!label.empty() && !state->doing_fixup_pass)
		{
			/* Handle the label here, instead of passing it onto a later function. */
			SetLastGlobalLabel(state, label);
			AddToSymbolTable(state, label, SymbolLabel{state->program_counter});
		}
	}
	else if (CC_VariantHoldsAnyOf<
		StatementEndR,
		StatementEndM,
		StatementElseIf,
		StatementElse,
		StatementEndC,
		StatementEndW,
		StatementRSSet,
		StatementRSReset,
		StatementObj,
		StatementObjEnd,
		StatementOrg
	>(*statement))
	{
		if (!label.empty())
			SemanticError(state, "There cannot be a label on this type of statement.");
	}
	else if (CC_VariantHoldsAnyOf<
		StatementMacro,
		StatementMacroS,
		StatementEqu,
		StatementEquSString,
		StatementEquSIdentifier,
		StatementSubstr,
		StatementSet
	>(*statement))
	{
		if (label.empty())
		{
			SemanticError(state, "This type of statement must have a label.");
			/* Bail, to avoid null pointer dereferences. */
			return;
		}
	}
	else if (CC_VariantHoldsAnyOf<
		StatementRS
	>(*statement))
	{
		/* Handled later. */
	}

	std::visit(
		CC_VisitHelper{
			[&](StatementEmpty&) {},
			[&](StatementInstruction &statement)
			{
				ProcessInstruction(state, &statement);
			},
			[&](StatementDc &statement)
			{
				ProcessDc(state, &statement);
			},
			[&](StatementDcb &statement)
			{
				ProcessDcb(state, &statement);
			},
			[&](StatementDs &statement)
			{
				ProcessDs(state, &statement);
			},
			[&](StatementInclude &statement)
			{
				ProcessInclude(state, &statement);
			},
			[&](StatementIncbin &statement)
			{
				ProcessIncbin(state, &statement);
			},
			[&](StatementRept &statement)
			{
				ProcessRept(state, &statement);
			},
			[&](StatementEndR&)
			{
				/* Exit REPT mode. */
				const auto mode = std::get_if<SemanticState::ModeRept>(&state->mode);

				if (mode == nullptr)
					SemanticError(state, "This stray ENDR has no preceeding REPT.");
				else
					TerminateRept(state, *mode);
			},
			[&](StatementMacro &statement)
			{
				ProcessMacro(state, &statement, label, cc_false);
			},
			[&](StatementMacroS &statement)
			{
				ProcessMacro(state, &statement.macro, label, cc_true);
			},
			[&](StatementEndM&)
			{
				/* Exit MACRO mode. */
				const auto mode = std::get_if<SemanticState::ModeMacro>(&state->mode);

				if (mode == nullptr)
					SemanticError(state, "This stray ENDM has no preceeding MACRO.");
				else if (mode->metadata.is_short)
					SemanticError(state, "ENDM cannot be used in a short macro.");
				else
					TerminateMacro(state, *mode);
			},
			[&](StatementEqu &statement)
			{
				unsigned long value;

				if (state->options->equ_set_descope_local_labels)
					SetLastGlobalLabel(state, label);

				auto &equ = statement;

				if (!ResolveExpression(state, &equ.expression, &value))
				{
					AddToSymbolTable(state, label, SymbolExpressionConstant{std::move(equ.expression)});
					// TODO: Why does removing this result in a crash?
					equ.expression.emplace<ExpressionNumber>(0);
				}
				else
				{
					AddToSymbolTable(state, label, SymbolConstant{value});
					ListIdentifierValue(state, value);
				}

				/* Hack: We ABSOLUTELY DO NOT want this line adding to the fix-up list!!! */
				state->fixup_needed = cc_false;
			},
			[&](StatementEquSString &statement)
			{
				PushSubstitute(state, label, statement.string);
			},
			[&](StatementEquSIdentifier &statement)
			{
				const SymbolDictionary::Entry* const dictionary_entry = LookupSymbol(state, statement.string, nullptr);

				const SymbolStringConstant* const symbol_string_constant = dictionary_entry == nullptr ? nullptr : std::get_if<SymbolStringConstant>(dictionary_entry);

				if (symbol_string_constant == nullptr)
					SemanticError(state, "String constant can only be defined as a string or another string constant.");
				else
					PushSubstitute(state, label, symbol_string_constant->value);
			},
			[&](StatementSubstr &statement)
			{
				unsigned long start, end;
				std::string_view substring;

				auto &substr = statement;

				if (!ResolveExpression(state, &substr.start, &start))
				{
					SemanticError(state, "Start must be evaluable on the first pass.");
					return;
				}

				if (!ResolveExpression(state, &substr.end, &end))
				{
					SemanticError(state, "End must be evaluable on the first pass.");
					return;
				}

				if (start == 0)
				{
					SemanticError(state, "Start cannot be zero.");
					return;
				}

				if (end < start)
					end = start - 1;

				/* Clamp start and end to make it so that, if the string would go beyond the end, it is truncated appropriately instead of going out-of-bounds */
				/* (this matches asm68k, as far as I can see). */
				if (start > substr.string.length())
					start = substr.string.length() + 1;
				if (end > substr.string.length())
					end = substr.string.length();

				substring = substr.string.substr(start - 1, end - start + 1);
				PushSubstitute(state, label, substring);
			},
			[&](StatementSet &statement)
			{
				unsigned long value;

				if (state->options->equ_set_descope_local_labels)
					SetLastGlobalLabel(state, label);

				if (!ResolveExpression(state, &statement.expression, &value))
				{
					SemanticError(state, "Value must be evaluable on the first pass.");
					return;
				}

				AddToSymbolTable(state, label, SymbolVariable{value});
				ListIdentifierValue(state, value);
			},
			[&](StatementIf &statement)
			{
				ProcessIf(state, &statement.expression);
			},
			[&](StatementElseIf &statement)
			{
				if (state->current_if_level == 0)
				{
					SemanticError(state, "This stray ELSEIF has no preceeding IF.");
				}
				else
				{
					if (state->false_if_level == state->current_if_level || state->false_if_level == 0)
					{
						unsigned long value;

						if (!ResolveExpression(state, &statement.expression, &value))
						{
							SemanticError(state, "Condition must be evaluable on the first pass.");
							value = 1;
						}

						/* If this condition is false, then mark this as the false if-level. */
						if (state->true_already_found || value == 0)
						{
							state->false_if_level = state->current_if_level;
						}
						else
						{
							state->false_if_level = 0;
							state->true_already_found = cc_true;
						}
					}
				}
			},
			[&](StatementElse&)
			{
				if (state->current_if_level == 0)
				{
					SemanticError(state, "This stray ELSE has no preceeding IF.");
				}
				else
				{
					if (state->false_if_level == state->current_if_level || state->false_if_level == 0)
					{
						if (state->true_already_found)
						{
							state->false_if_level = state->current_if_level;
						}
						else
						{
							/* If this is the false if-level, then it isn't anymore. */
							/* Likewise, if there is no false if-level, then there is now. */
							if (state->false_if_level == state->current_if_level)
								state->false_if_level = 0;
							else /*if (state->false_if_level == 0)*/
								state->false_if_level = state->current_if_level;
						}
					}
				}
			},
			[&](StatementEndC&)
			{
				if (state->current_if_level == 0)
					SemanticError(state, "This stray ENDC/ENDIF has no preceeding IF.");
				else
					TerminateIf(state);
			},
			[&](StatementWhile &statement)
			{
				/* Enter WHILE mode. */
				auto &mode = state->mode.emplace<SemanticState::ModeWhile>();

				auto &while_statement = statement;

				/* Dummy-out the statement's expression, since the state now owns it, and we don't
				   want its sub-expressions to be automatically freed when the statement is freed. */
				mode.expression.emplace<ExpressionNumber>(0);
				std::swap(mode.expression, while_statement.expression);
				mode.source_line = state->source_line;

				mode.line_number = state->location.front().line_number;
				mode.nesting = 0;
			},
			[&](StatementEndW&)
			{
				/* Exit WHILE mode. */
				const auto mode = std::get_if<SemanticState::ModeWhile>(&state->mode);

				if (mode == nullptr)
					SemanticError(state, "This stray ENDW has no preceeding WHILE.");
				else
					TerminateWhile(state, *mode);
			},
			[&](StatementEven&)
			{
				PerformEven(state);
			},
			[&](StatementCnop &statement)
			{
				auto &cnop = statement;

				unsigned long offset;

				if (!ResolveExpression(state, &cnop.offset, &offset))
				{
					SemanticError(state, "Offset must be evaluable on the first pass.");
				}
				else
				{
					unsigned long size_boundary;

					if (!ResolveExpression(state, &cnop.size_boundary, &size_boundary))
					{
						SemanticError(state, "Size boundary must be evaluable on the first pass.");
					}
					else if (size_boundary == 0)
					{
						/* I don't know if there's a way to make this have meaningfully useful semantics. asm68k errors on this, so for now we will too. */
						SemanticError(state, "Size boundary cannot be zero.");
					}
					else
					{
						unsigned long target;

						const unsigned long position_within_boundary = state->program_counter % size_boundary;

						target = state->program_counter - position_within_boundary + offset;

						if (position_within_boundary > offset)
							target += size_boundary;

						/* Pad to the desired offset. */
						while (state->program_counter != target)
						{
							OutputByte(state, 0);
							AdvanceProgramCounter(state, 1);
						}
					}
				}
			},
			[&](StatementInform &statement)
			{
				auto &inform = statement;

				unsigned long severity;

				/* Definitely an 'INFORM'... */
				if (!ResolveExpression(state, &inform.severity, &severity))
				{
					SemanticError(state, "Severity must be evaluable on the first pass.");
					severity = 0;
				}

				/* TODO - Everything... */
				switch (severity)
				{
					case 0:
						TextOutput_Format(*state->error_stream, "INFORM: '{}'\n", inform.message);
						break;

					case 1:
						SemanticWarning(state, "INFORM: '{}'", inform.message);
						break;

					case 2:
					case 3: /* TODO: Halt assembly. */
						SemanticError(state, "INFORM: '{}'", inform.message);
						break;
				}
			},
			[&](StatementFail&)
			{
				/* TODO: Halt assembly. */
				SemanticError(state, "Assembly Failed");
			},
			[&](StatementEnd&)
			{
				state->end = cc_true;
			},
			[&](StatementRS &statement)
			{
				auto &rs = statement;

				unsigned long length;

				if (ShouldPerformAutomaticEven(state, rs.size))
					if ((state->rs & 1) != 0)
						++state->rs;

				if (!ResolveExpression(state, &rs.length, &length))
				{
					SemanticError(state, "Length must be evaluable on the first pass.");
				}
				else
				{
					/* Add label to symbol table. */
					if (!label.empty())
					{
						AddToSymbolTable(state, label, SymbolConstant{state->rs});
						ListIdentifierValue(state, state->rs);
					}

					/* Advance '__rs' by the specified amount. */
					switch (rs.size)
					{
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

						case SIZE_UNDEFINED:
							/* Should never happen. */
							assert(cc_false);
							break;
					}

					state->rs += length;
				}
			},
			[&](StatementRSSet &statement)
			{
				/* Set '__rs' to value. */
				unsigned long value;

				if (!ResolveExpression(state, &statement.expression, &value))
					SemanticError(state, "Value must be evaluable on the first pass.");
				else
					state->rs = value;
			},
			[&](StatementRSReset&)
			{
				/* Set '__rs' to 0. */
				state->rs = 0;
			},
			[&](StatementObj &statement)
			{
				if (state->obj_active)
				{
					SemanticError(state, "OBJ statements cannot be nested.");
				}
				else
				{
					unsigned long value;

					if (!ResolveExpression(state, &statement.expression, &value))
						SemanticError(state, "Value must be evaluable on the first pass.");
					else
						state->obj_delta = value - state->program_counter;

					state->obj_active = cc_true;
					AdvanceProgramCounter(state, state->obj_delta);
				}
			},
			[&](StatementObjEnd&)
			{
				if (!state->obj_active)
				{
					SemanticError(state, "This stray OBJEND has no preceeding OBJ.");
				}
				else
				{
					RegressProgramCounter(state, state->obj_delta);
					state->obj_active = cc_false;
				}
			},
			[&](StatementOrg &statement)
			{
				unsigned long value;

				if (!ResolveExpression(state, &statement.expression, &value))
				{
					SemanticError(state, "Value must be evaluable on the first pass.");
				}
				else if (value > 0xFFFFFFFF)
				{
					SemanticError(state, "Value cannot be larger than 0xFFFFFFFF (was 0x{:X}).", value);
				}
				else
				{
					/* Begin a new segment! */
					state->segment_position = value;
					WriteRecordSegment(state);
					state->segment_length = 0;

					state->program_counter = value;
				}
			},
			[&](StatementPushO&)
			{
				state->options.Push();
			},
			[&](StatementPopO&)
			{
				state->options.Pop();
			},
			[&](StatementOpt &statement)
			{
				for (const auto &option : statement.options)
				{
					/* TODO: A lot of these are unimplemented... */
					if (CaseInsensitive::CompareStrings(option, "ae+"))
						state->options->automatic_even = cc_true;
					else if (CaseInsensitive::CompareStrings(option, "ae-"))
						state->options->automatic_even = cc_false;
					else if (CaseInsensitive::CompareStrings(option, "an+"))
						;
					else if (CaseInsensitive::CompareStrings(option, "an-"))
						;
					else if (CaseInsensitive::CompareStrings(option, "c+"))
						state->options->case_insensitive = cc_false;
					else if (CaseInsensitive::CompareStrings(option, "c-"))
						state->options->case_insensitive = cc_true;
					else if (CaseInsensitive::CompareStrings(option, "d+"))
						state->options->equ_set_descope_local_labels = cc_true;
					else if (CaseInsensitive::CompareStrings(option, "d-"))
						state->options->equ_set_descope_local_labels = cc_false;
					else if (CaseInsensitive::CompareStrings(option, "e+"))
						;
					else if (CaseInsensitive::CompareStrings(option, "e-"))
						;
					else if (CaseInsensitive::CompareStrings(option, "l+"))
						state->options->local_signifier = '.';
					else if (CaseInsensitive::CompareStrings(option, "l-"))
						state->options->local_signifier = '@';
					else if (option.length() == 2 && (option[0] == 'l' || option[0] == 'L'))
						state->options->local_signifier = option[1];
					/*else if (CaseInsensitive::CompareStrings(option, "s+"))
						;*/
					else if (CaseInsensitive::CompareStrings(option, "s-"))
						;
					else if (CaseInsensitive::CompareStrings(option, "m+"))
						state->options->expand_all_macros = cc_true;
					else if (CaseInsensitive::CompareStrings(option, "m-"))
						state->options->expand_all_macros = cc_false;
					else if (CaseInsensitive::CompareStrings(option, "oz+"))
						state->options->optimisation.zero_displacement = cc_true;
					else if (CaseInsensitive::CompareStrings(option, "oz-"))
						state->options->optimisation.zero_displacement = cc_false;
					else if (CaseInsensitive::CompareStrings(option, "v+"))
						state->options->output_local_labels_to_sym_file = cc_true;
					else if (CaseInsensitive::CompareStrings(option, "v-"))
						state->options->output_local_labels_to_sym_file = cc_false;
					else if (CaseInsensitive::CompareStrings(option, "w+"))
						state->options->warnings_enabled = cc_true;
					else if (CaseInsensitive::CompareStrings(option, "w-"))
						state->options->warnings_enabled = cc_false;
					else if (CaseInsensitive::CompareStrings(option, "wp+"))
						state->options->pedantic_warnings_enabled = cc_true;
					else if (CaseInsensitive::CompareStrings(option, "wp-"))
						state->options->pedantic_warnings_enabled = cc_false;
					else if (CaseInsensitive::CompareStrings(option, "ws+"))
						;
					else if (CaseInsensitive::CompareStrings(option, "ws-"))
						;
					else
						SemanticWarning(state, "Unrecognised option '{}'.", option);
				}
			},
			[&](StatementPushP &statement)
			{
				state->string_stack.push(statement.string);
			},
			[&](StatementPopP &statement)
			{
				if (state->string_stack.empty())
				{
					SemanticError(state, "POPP used but the string stack was empty.");
				}
				else
				{
					PushSubstitute(state, statement.string, state->string_stack.top());
					state->string_stack.pop();
				}
			},
			[&](StatementShift&)
			{
				if (!CurrentlyExpandingMacro(state))
				{
					SemanticError(state, "SHIFT used outside of macro.");
				}
				else if (state->macro.total_arguments == 0)
				{
					/* TODO: What if there are already 0 arguments? */
					/* This prevents crashing for now but perhaps it could be handled differently. */
					SemanticError(state, "SHIFT used but there are no macro arguments to shift.");
				}
				else
				{
					--state->macro.total_arguments;
					for (unsigned int i = 0; i < state->macro.total_arguments; ++i)
						state->macro.argument_list[i] = std::move(state->macro.argument_list[i + 1]);
					state->macro.argument_list.pop_back();
					PushMacroArgumentSubstitutions(state);
				}
			},
			[&](StatementMExit&)
			{
				state->macro.active = cc_false;
			},
			[&](StatementLocal &statement)
			{
				if (!CurrentlyExpandingMacro(state))
				{
					SemanticError(state, "LOCAL used outside of macro.");
				}
				else
				{
					for (const auto &identifier : statement.identifiers)
						state->macro.dictionary->LookUpAndCreateIfNotExist(identifier);
				}
			},
			[&](StatementPurge &statement)
			{
				PurgeMacro(state, statement.string, cc_false);
			}
		}, *statement
	);

	/* Update both copies of the program counter again, so that things like WHILE statements don't use stale values in their expressions. */
	state->program_counter_of_statement = state->program_counter;
	state->program_counter_of_expression = state->program_counter;
}

static cc_bool ParseStatement(SemanticState* const state, const std::string_view &view)
{
	/* Parse the source line with Flex and Bison (Lex and Yacc). */
	const YY_BUFFER_STATE buffer = m68kasm__scan_bytes(view.data(), view.length(), *state->flex_state);
	const int parse_result = state->parser.parse();
	m68kasm__delete_buffer(buffer, *state->flex_state);

	return parse_result == 0;
}

static void ParseLine(SemanticState* const state, const std::string_view &label, const std::string_view &directive_and_operands)
{
	/* This is a normal assembly line. */

	/* Back these up, before they get a chance to be modified by ProcessStatement. */
	const unsigned long starting_program_counter = state->program_counter;
	const std::size_t starting_output_position = state->output_position;
	const std::size_t starting_segment_length = state->segment_length;

	if (!ParseStatement(state, directive_and_operands))
	{
		/* An error message will have already been printed, so we don't need to do one here. */
		state->success = cc_false;
	}
	else
	{
		state->fixup_needed = cc_false;

		ProcessStatement(state, &state->statement, label);

		if (state->fixup_needed)
		{
			/* If the statement cannot currently be processed because of undefined symbols,
			   add it to the fix-up list so we can try again later. */
			FixUp &fixup = state->fixup_list.emplace_back();


			/* Backup the statement. */
			fixup.statement = std::move(state->statement);

			/* Backup some state. */
			fixup.macro_dictionary = state->macro.dictionary;
			fixup.program_counter = starting_program_counter;
			fixup.output_position = starting_output_position;
			fixup.segment_length = starting_segment_length;
			fixup.last_global_label = state->last_global_label;
			fixup.source_line = state->source_line;
			fixup.label = label;
			fixup.options = *state->options;

			/* Clone the location. */
			fixup.location = state->location;

			state->fixup_needed = cc_false;
		}
	}
}

#define DIRECTIVE_OR_MACRO_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?_"
#define LABEL_CHARS DIRECTIVE_OR_MACRO_CHARS ".@"

static void PerformSubstitutionsOnSubString(SemanticState* const state, std::string &string, std::string_view &view_to_search, const cc_bool allow_implicit_matches)
{
	const cc_bool case_insensitive = state->options->case_insensitive;

	if (state->macro.custom_search)
		state->substitutions.ProcessSubString(&state->macro.substitutions, string, view_to_search, state->macro.custom_search, allow_implicit_matches, case_insensitive);
	else
		state->substitutions.ProcessSubString(nullptr, string, view_to_search, nullptr, allow_implicit_matches, case_insensitive);
}

static void PerformSubstitutions(SemanticState* const state, std::string &string, const cc_bool allow_implicit_matches)
{
	std::string_view view = string;
	PerformSubstitutionsOnSubString(state, string, view, allow_implicit_matches);
}

static cc_bool FindStringInSourceLine(const std::string &string, const std::size_t starting_position, std::string_view &view)
{
	const auto start_position = string.find_first_of("'\"", starting_position);

	if (start_position == string.npos)
		return cc_false;

	const auto end_position = string.find_first_of(string[start_position], start_position + 1);

	if (end_position == string.npos)
		return cc_false;

	view = std::string_view(string).substr(start_position, end_position - start_position);
	return cc_true;
}

/* Performs string substitutions only in parts of the string that are not wrapped in quotations. */
/* This is needed to simulate how SN 68k does not expand string constants in strings, only expressions and operands. */
static void PerformSubstitutionsExcludingQuotedStrings(SemanticState* const state, std::string &string, const std::size_t starting_position, const cc_bool allow_implicit_matches)
{
	cc_bool string_in_source_line_found;
	std::size_t position = starting_position;

	do
	{
		std::string_view string_in_source_line;

		string_in_source_line_found = FindStringInSourceLine(string, position, string_in_source_line);

		/* Pretend there's a string at the end of the line, so that we can repurpose the code below. */
		if (!string_in_source_line_found)
			string_in_source_line = std::string_view(string).substr(string.length());

		/* We've found a string! Perform substitutions on the part of the line before it. */
		{
			/* Define the region that we will perform substitutions on. */
			const std::size_t search_start = position;
			const std::size_t search_end = string_in_source_line.data() - string.data();

			/* Perform the substitutions. */
			std::string_view view_to_search = std::string_view(string).substr(search_start, search_end - search_start);
			PerformSubstitutionsOnSubString(state, string, view_to_search, allow_implicit_matches);

			/* Move to after the string so that we can continue searching. */
			position += view_to_search.length() + string_in_source_line.length();
		}
	}
	while (string_in_source_line_found);
}

static void SubstituteAndParseLine(SemanticState *state, const std::string_view &label, const std::string_view &directive_and_operands, const std::string_view &directive)
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
		ParseLine(state, label, directive_and_operands);
		return;
	}

	/* Perform implicit (backslash-less) string substitutions on the operands.
	   SN 68k achieves this by expanding string constants in its operand and expression evaluators,
	   which is absolutely insane, so this trick approximates it instead. */
	modified_directive_and_operands = directive_and_operands;
	PerformSubstitutionsExcludingQuotedStrings(state, modified_directive_and_operands, directive.length(), cc_true);

	ParseLine(state, label, modified_directive_and_operands);
}

static void InvokeMacro(SemanticState* const state, const Macro* const macro, const std::string_view &label, std::string_view source_line)
{
	/* This is a macro invocation. */
	const SemanticState_Macro previous_macro_state = std::move(state->macro);

	if (state->macro.depth >= 0x100)
	{
		SemanticError(state, "Macro expansion depth exceeded.");
	}
	else
	{
		state->macro.metadata = macro;
		/* TODO: We should not be caching 'case_insensitive'; the OPT directive can change this at any time! */
		state->macro.dictionary = std::make_shared<SymbolDictionary>(state->options->case_insensitive);

		state->macro.argument_list = {};
		state->macro.total_arguments = 0;
		state->macro.total_arguments_at_start = 0;
		state->macro.starting_if_level = state->current_if_level;
		state->macro.active = cc_true;
		++state->macro.depth;

		++state->current_macro_invocation;

		state->suppress_listing = cc_true;

		Utilities::String::RemoveLeading(source_line, DIRECTIVE_OR_MACRO_CHARS);

		if (!label.empty())
		{
			SetLastGlobalLabel(state, label);

			if (!macro->uses_label)
				AddToSymbolTable(state, label, SymbolLabel{state->program_counter});
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

		const std::string unique_suffix = ComputeUniqueMacroSuffix(state);
		std::string symbol_value_string;

		state->macro.custom_search = [state, &size, &arguments, &label, &unique_suffix, &symbol_value_string](const std::string_view &view_to_search, std::size_t starting_position, cc_bool case_insensitive, std::size_t* const found_position, std::size_t* const found_length) -> std::optional<std::string_view>
		{
			/* `view_to_search` always points to a null-terminated string. */
			(void)case_insensitive;

			for (*found_position = starting_position; (*found_position = view_to_search.find('\\', *found_position)) != view_to_search.npos; ++(*found_position))
			{
				const std::size_t symbol_position = *found_position + 1;

				if (symbol_position == view_to_search.length())
					break;

				const char symbol = view_to_search[symbol_position];

				*found_length = 2;

				switch (symbol)
				{
					case '0':
						return size;

					case '_':
						if (*found_position + *found_length < view_to_search.length() && Substitute::IsBlockingCharacter(view_to_search[*found_position + *found_length]))
							continue;

						return arguments;

					case '*':
						return label;

					case '@':
						return unique_suffix;

					case '#':
					case '$':
					{
						unsigned long value;

						const std::size_t identifier_position_start = symbol_position + 1;
						const std::size_t identifier_position_end = Utilities::String::GetLeadingLength(view_to_search, DIRECTIVE_OR_MACRO_CHARS, identifier_position_start);

						const std::string_view identifier = view_to_search.substr(identifier_position_start, identifier_position_end - identifier_position_start);

						*found_length += identifier.length();

						/* Absorb trailing backslash. */
						if (identifier_position_end < view_to_search.length() && view_to_search[identifier_position_end] == '\\')
							++*found_length;

						if (!GetSymbolInteger(state, identifier, cc_true, &value))
							value = 0;

						if (symbol == '#')
							symbol_value_string = DecimalIntegerToString(value);
						else /*if (symbol == '$')*/
							symbol_value_string = HexadecimalIntegerToString(value);

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

						*found_length = 1 + (end - start);

						if (parameter_index >= state->macro.total_arguments)
							return "";

						return state->macro.argument_list[parameter_index];
					}
				}
			}

			return std::nullopt;
		};

		/* Extract and store the individual macro arguments, if they exist. */
		{
			std::size_t i;
			std::size_t argument_string_start = 0;
			unsigned int argument_depth = 0;

			for (i = 0; i <= arguments.length(); ++i)
			{
				const cc_bool at_the_end = i == arguments.length();
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
							if (argument.empty() && at_the_end && state->macro.total_arguments == 0)
								break;

							/* Add to argument list. */
							state->macro.argument_list.emplace_back(argument);

							if (argument.length() != 0 && argument[0] != '"' && argument[0] != '\'' && state->options->case_insensitive)
								Utilities::String::ToLower(state->macro.argument_list[state->macro.total_arguments]);

							++state->macro.total_arguments;
							++state->macro.total_arguments_at_start;
						}

						break;
				}
			}

			/* Add identifier substitute. */
			PushMacroArgumentSubstitutions(state);
		}

		/* Finally, invoke the macro. */
		{
			const cc_bool previous_listing_macro = state->listing_macro;

			/* Flush line before changing this state, so that the macro line is not given the 'M' label. */
			ListSourceLine(state);
			state->listing_macro = cc_true;

			/* Push a new location (this macro).*/
			state->location.emplace_front(macro->name, 0);

			/* Iterate over each line of the macro, sending it to be processed. */
			for (const auto &source_line : macro->source_line_list)
			{
				/* TODO: This is redundant and wrong: these substitutions are already done later, and this should only be doing the macro arguments! */
				if (macro->is_short)
				{
					std::string expanded_source_line = source_line;
					PerformSubstitutions(state, expanded_source_line, cc_false);
					AssembleLine(state, expanded_source_line, state->options->expand_all_macros);
				}
				else
				{
					AssembleLine(state, source_line, state->options->expand_all_macros);
				}

				if (!CurrentlyExpandingMacro(state))
					break;
			}

			if (!macro->is_short)
			{
				/* 'MEXIT' may have ended us midway through an if-statement, so unwind to the original if-level here. */
				while (state->current_if_level != state->macro.starting_if_level)
					TerminateIf(state);
			}

			/* Pop location. */
			state->location.pop_front();

			state->listing_macro = previous_listing_macro;
		}
	}

	state->macro = previous_macro_state;
}

static void AssembleLine(SemanticState *state, const std::string_view &source_line_raw, const cc_bool write_line_to_listing_file)
{
	const std::string_view old_source_line = state->source_line;

	/* Assert a few preconditions to ensure we're in a non-insane state. */
	assert(CurrentlyExpandingMacro(state) == (state->macro.depth > 0));

	if (write_line_to_listing_file)
	{
		ListSourceLine(state);

		/* Output program counter to listing file. */
		if (state->listing_stream != nullptr)
		{
			state->line_listed = cc_false;

			state->listing_counter = 0;
			state->suppress_listing = cc_false;
			TextOutput_Format(*state->listing_stream, "{:08X}", state->program_counter);
		}
	}

	/* Expand string constants and macro parameters only if we're actually parsing code;
	   it is important that the input of REPT and WHILE is unaltered, so that each iteration
	   can have unique expansions! */
	std::string source_line_expanded;

	if (std::holds_alternative<SemanticState::ModeNormal>(state->mode))
	{
		source_line_expanded = source_line_raw;
		PerformSubstitutions(state, source_line_expanded, cc_false);
		state->source_line = source_line_expanded;
	}
	else
	{
		state->source_line = source_line_raw;
	}

	++state->location.front().line_number;

	if (!state->source_line.empty() && state->source_line[0] == '*')
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
		std::string_view source_line_subview = state->source_line;

		/* Let's begin by parsing the label (if there is one). */
		std::string_view label = source_line_subview;

		/* Determine the length of the label. */
		label = Utilities::String::ExtractLeading(label, LABEL_CHARS);

		if (label.length() != 0)
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

			if (label.length() != 0)
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

		if (label.length() != 0)
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
					if (state->false_if_level != 0)
					{
						/* TODO - Detect code after the keyword and error if any is found. */
						if (CaseInsensitive::CompareStrings(directive, "if"))
						{
							/* If-statements that are nested within the false part of another if-statement
							   are themselves false, so create a false if-statement here and process it. */
							++state->current_if_level;
						}
						else if (CaseInsensitive::CompareStrings(directive, "elseif")
						      || CaseInsensitive::CompareStrings(directive, "else"  )
						      || CaseInsensitive::CompareStrings(directive, "endc"  )
						      || CaseInsensitive::CompareStrings(directive, "endif" ))
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
						const SymbolDictionary::Entry* const macro_dictionary_entry = LookupSymbol(state, directive, nullptr);
						const SymbolMacro* const symbol_macro = macro_dictionary_entry == nullptr ? nullptr : std::get_if<SymbolMacro>(macro_dictionary_entry);
						const Macro* const macro = symbol_macro == nullptr ? nullptr : &symbol_macro->macro;

						if (macro != nullptr /*&& !macro->is_short*/)
						{
							/* Regular macros are only expanded when needed. */
							InvokeMacro(state, macro, label, source_line_subview);
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
					if (CaseInsensitive::CompareStrings(directive, "endr") && rept_state.nesting-- == 0)
					{
						/* TODO - Detect code after the keyword and error if any is found. */
						ParseLine(state, label, directive_and_operands);
					}
					else
					{
						if (CaseInsensitive::CompareStrings(directive, "rept"))
							++rept_state.nesting;

						rept_state.source_line_list.emplace_back(state->source_line);
					}
				},
				[&](SemanticState::ModeMacro &macro_state)
				{
					/* If this line is an 'ENDM' directive, then exit macro mode. Otherwise, add the line to the macro. */
					if (CaseInsensitive::CompareStrings(directive, "endm"))
					{
						/* TODO - Detect code after the keyword and error if any is found. */
						if (macro_state.metadata.is_short)
							SemanticError(state, "Short macros shouldn't use ENDM.");
						else
							ParseLine(state, label, directive_and_operands);
					}
					else
					{
						if (macro_state.metadata.is_short)
						{
							/* Short macros automatically terminate after one statement. */
							const std::size_t first_nonwhitespace_position = Utilities::String::GetLeadingLength(state->source_line, " \t");
							const char first_nonwhitespace_character = first_nonwhitespace_position == state->source_line.length() ? '\0' : state->source_line[first_nonwhitespace_position];

							if (!label.empty())
								SemanticError(state, "Short macros shouldn't create labels.");

							if (first_nonwhitespace_character == '\0' || first_nonwhitespace_character == ';')
							{
								/* This line is an empty statement: ignore it. */
							}
							else
							{
								macro_state.metadata.source_line_list.emplace_back(state->source_line);
								TerminateMacro(state, macro_state);
							}
						}
						else
						{
							macro_state.metadata.source_line_list.emplace_back(state->source_line);
						}
					}
				},
				[&](SemanticState::ModeWhile &while_state)
				{
					/* If this line is an 'ENDW' directive, then exit 'WHILE' mode. Otherwise, add the line to the 'WHILE'. */
					if (CaseInsensitive::CompareStrings(directive, "endw") && while_state.nesting-- == 0)
					{
						/* TODO - Detect code after the keyword and error if any is found. */
						ParseLine(state, label, directive_and_operands);
					}
					else
					{
						if (CaseInsensitive::CompareStrings(directive, "while"))
							++while_state.nesting;

						while_state.source_line_list.emplace_back(state->source_line);
					}
				}
			}, state->mode
		);
	}

	if (write_line_to_listing_file)
		ListSourceLine(state);

	state->source_line = old_source_line;
}

static void AssembleFile(SemanticState *state)
{
	std::string_view source_line;

	/* Read lines one at a time, feeding them to the 'AssembleLine' function. */
	while (!state->end && ReadSourceLine(state, source_line))
		AssembleLine(state, source_line, cc_true);

	/* If we're not in normal mode when a file ends, then something is wrong. */
	while (!std::holds_alternative<SemanticState::ModeNormal>(state->mode))
	{
		std::visit(
			CC_VisitHelper{
				[&](SemanticState::ModeNormal&)
				{
					/* We can't reach this since we just checked for it. */
					assert(cc_false);
				},
				[&](SemanticState::ModeRept &rept_state)
				{
					SemanticError(state, "REPT statement beginning at line {} is missing its ENDR.", rept_state.line_number);

					/* Terminate the REPT to hopefully avoid future complications. */
					TerminateRept(state, rept_state);
				},
				[&](SemanticState::ModeMacro &macro_state)
				{
					SemanticError(state, "MACRO statement beginning at line {} is missing its ENDM.", macro_state.line_number);

					/* Terminate the macro to hopefully avoid future complications. */
					TerminateMacro(state, macro_state);
				},
				[&](SemanticState::ModeWhile &while_state)
				{
					SemanticError(state, "WHILE statement beginning at line {} is missing its ENDW.", while_state.line_number);

					/* Terminate the while-statement to hopefully avoid future complications. */
					TerminateWhile(state, while_state);
				}
			}, state->mode
		);
	}
}

static void AddDefinition(void* const internal, const char* const identifier_buffer, const std::size_t identifier_length, const unsigned long value)
{
	SemanticState* const state = (SemanticState*)internal;

	/* TODO: Does asm68k create a constant or a variable? */
	AddToSymbolTable(state, {identifier_buffer, identifier_length}, SymbolConstant{value});
}

static cc_bool ClownAssembler_AssembleToObjectFile(
	std::istream &input_stream,
	std::ostream &output_stream,
	std::ostream &error_stream,
	std::ostream* const listing_stream,
	std::ostream *symbol_stream,
	const char* const input_file_path,
	const ClownAssembler_Settings* const initial_options,
	const ClownAssembler_DefinitionCallback definition_callback,
	const void* const user_data)
{
	SemanticState state(*initial_options);

	/* For error messages, we want to maintain a list of filenames and line
	   numbers that describe the location and nesting of the erroneous
	   line. This is the first entry in that list. */
	state.location.emplace_front(input_file_path, 0);

	/* Initialise the state's non-default values. */
	state.input_stream = &input_stream;
	state.output_stream = &output_stream;
	state.error_stream = &error_stream;
	state.listing_stream = listing_stream;

	state.mode.emplace<SemanticState::ModeNormal>();

	if (AddToSymbolTable(&state, "__rs", SymbolSpecial{SymbolSpecial::Type::RS})
	 && AddToSymbolTable(&state, "narg", SymbolSpecial{SymbolSpecial::Type::NARG}))
	{
		if (definition_callback != nullptr)
			definition_callback(&state, (void*)user_data, AddDefinition);

		/* Enable Bison debugging if available and requested by the user. */
	#if M68KASM_DEBUG
		if (state.options->debug)
			m68kasm_debug = 1;
	#endif

		/* Object file magic number. */
		OutputWriteRawByte(&state, 0x89);
		OutputWriteRawByte(&state, 0x14);

		/* Begin first segment. */
		WriteRecordSegment(&state);

		/* Perform first pass of assembly, creating a list of fix-ups. */
		AssembleFile(&state);

		/* Finalise the segment record data. */
		TerminatePreviousRecordSegment(&state);

		/* Write terminating record. */
		OutputWriteRawByte(&state, 0x00);
		output_stream << CLOWNASSEMBLER_VERSION_STRING;

		/* Perform some sanity checks to make sure we're not somehow in an invalid state. */
		if (state.current_if_level != 0)
			SemanticError(&state, "An IF statement somewhere is missing its ENDC/ENDIF.");
		if (state.obj_active)
			SemanticError(&state, "An OBJ statement somewhere is missing its OBJEND.");

		/* Process the fix-ups: reassemble instructions and reprocess
		   directives that could not be completed in the first pass. */
		state.doing_fixup_pass = cc_true;

		while (!state.fixup_list.empty())
		{
			/* Pop fix-up from list. */
			FixUp &fixup = state.fixup_list.front();

			/* Reset some state to how it was at the time the statement was first processed. */
			state.macro.dictionary = fixup.macro_dictionary;
			state.program_counter = fixup.program_counter;
			OutputSeek(&state, fixup.output_position);
			state.segment_length = fixup.segment_length;
			state.last_global_label = std::move(fixup.last_global_label);
			state.source_line = fixup.source_line;
			state.location = std::move(fixup.location);
			*state.options = fixup.options;

			/* Re-process statement. */
			ProcessStatement(&state, &fixup.statement, fixup.label);

			/* We're done with this statement: delete it. */
			state.fixup_list.pop_front();
		}

		/* Produce asm68k symbol file, if requested. */
		if (symbol_stream != nullptr)
		{
			/* Some kind of header. */
			symbol_stream->put('M');
			symbol_stream->put('N');
			symbol_stream->put('D');
			symbol_stream->put(1);
			symbol_stream->put(0);
			symbol_stream->put(0);
			symbol_stream->put(0);
			symbol_stream->put(0);

			state.dictionary.Filter(
				[&](const SymbolDictionary::Entry &entry, const std::string_view &identifier) -> cc_bool
				{
					/* The symbol file only contains labels. */
					/* Labels longer than 0xFF characters are silently ignored. */
					/* Local labels are also silently ignored (unless 'v+' is specified). */
					const SymbolLabel* const symbol_label = std::get_if<SymbolLabel>(&entry);

					if (symbol_label != nullptr)
					{
						const auto local_symbol_position = identifier.find_last_of(state.options->local_signifier);
						const std::string_view label = identifier.substr(local_symbol_position == identifier.npos ? 0 : local_symbol_position);

						const cc_bool is_local_label = label.length() != identifier.length();

						if (label.length() < 0x100 && (!is_local_label || state.options->output_local_labels_to_sym_file))
						{
							unsigned int i;

							/* Output the address of the label. */
							for (i = 0; i < 4; ++i)
								symbol_stream->put((symbol_label->value >> (i * 8)) & 0xFF);

							/* I have no idea what this means. */
							symbol_stream->put(is_local_label ? 6 : 2);

							/* Output the length of the label. */
							symbol_stream->put(label.length());

							/* Output the label itself. */
							*symbol_stream << label;
						}
					}

					return cc_true;
				}
			);
		}
	}

	return state.success;
}

cc_bool ClownAssembler_Assemble(
	std::istream &input_stream,
	std::ostream &output_stream,
	std::ostream &error_stream,
	std::ostream* const listing_stream,
	std::ostream* const symbol_stream,
	const char* const input_file_path,
	const ClownAssembler_Settings* const initial_options,
	const ClownAssembler_DefinitionCallback definition_callback,
	const void* const user_data)
{
	cc_bool success = cc_false;

	std::stringstream object_stream;

	if (ClownAssembler_AssembleToObjectFile(input_stream, object_stream, error_stream, listing_stream, symbol_stream, input_file_path, initial_options, definition_callback, user_data))
	{
		if (ConvertObjectFileToFlatBinary(object_stream, output_stream, error_stream))
			success = cc_true;
	}

	return success;
}

cc_bool ClownAssembler_AssembleFile(
	std::istream &input_stream,
	std::ostream &output_stream,
	std::ostream &error_stream,
	std::ostream &listing_stream,
	std::ostream* const symbol_stream,
	const char* const input_file_path,
	const ClownAssembler_Settings* const settings,
	const ClownAssembler_DefinitionCallback definition_callback,
	const void* const user_data)
{
	return ClownAssembler_Assemble(input_stream, output_stream, error_stream, &listing_stream, symbol_stream, input_file_path, settings, definition_callback, user_data);
}
