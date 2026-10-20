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

#include "expression-processor.h"

#include <filesystem>
#include <optional>

#include "clowncommon/clowncommon.h"

#include "error.h"
#include "options.h"
#include "symbol-manager.h"
#include "utilities/path.h"

std::optional<unsigned long> ExpressionProcessor::ResolveExpressionCommon(Expression &expression, bool fold, const std::string_view &message)
{
	const auto &value = ResolveExpressionCommon(expression, fold);

	if (!value.has_value() && !message.empty())
		error_manager.Error(message);

	return value;
}

std::optional<unsigned long> ExpressionProcessor::ResolveExpressionCommon(Expression &expression, const bool fold)
{
	const auto &value = std::visit(
		CC_VisitHelper{
			[&](ExpressionArithmetic2Operand &expression) -> std::optional<unsigned long>
			{
				const auto &left_value_optional  = ResolveExpressionCommon(expression.subexpressions[0], fold);
				const auto &right_value_optional = ResolveExpressionCommon(expression.subexpressions[1], fold);

				if (!left_value_optional.has_value() || !right_value_optional.has_value())
					return std::nullopt;

				auto left_value  = *left_value_optional;
				auto right_value = *right_value_optional;

				unsigned long value;

				switch (expression.type)
				{
					default:
						CC_UNREACHABLE;
						/* Fallthrough */
					case ExpressionArithmetic2Operand::Type::SUBTRACT:
						value = left_value - right_value;
						break;

					case ExpressionArithmetic2Operand::Type::ADD:
						value = left_value + right_value;
						break;

					case ExpressionArithmetic2Operand::Type::MULTIPLY:
					case ExpressionArithmetic2Operand::Type::DIVIDE:
					case ExpressionArithmetic2Operand::Type::MODULO:
					{
						const bool left_value_was_negative  = left_value  >= 0x80000000;
						const bool right_value_was_negative = right_value >= 0x80000000;

						/* Make inputs absolute, so we can emulate signed arithmetic with unsigned arithmetic. */
						left_value  = (left_value  ^ (-(unsigned long)left_value_was_negative  & 0xFFFFFFFF)) + left_value_was_negative;
						right_value = (right_value ^ (-(unsigned long)right_value_was_negative & 0xFFFFFFFF)) + right_value_was_negative;

						switch (expression.type)
						{
							default:
								CC_UNREACHABLE;
								/* Fallthrough */
							case ExpressionArithmetic2Operand::Type::MULTIPLY:
								value = left_value * right_value;

								if (left_value_was_negative != right_value_was_negative)
									value = -value;

								break;

							case ExpressionArithmetic2Operand::Type::DIVIDE:
								if (right_value == 0)
								{
									error_manager.Error("Cannot divide by zero.");
									return std::nullopt;
								}

								value = left_value / right_value;

								if (left_value_was_negative != right_value_was_negative)
									value = -value;

								break;

							case ExpressionArithmetic2Operand::Type::MODULO:
								if (right_value == 0)
								{
									error_manager.Error("Cannot modulo by zero.");
									return std::nullopt;
								}

								value = left_value % right_value;

								if (left_value_was_negative)
									value = -value;

								break;
						}

						break;
					}

					case ExpressionArithmetic2Operand::Type::LOGICAL_OR:
						value = left_value != 0 || right_value != 0 ? -1 : 0;
						break;

					case ExpressionArithmetic2Operand::Type::LOGICAL_AND:
						value = left_value != 0 && right_value != 0 ? -1 : 0;
						break;

					case ExpressionArithmetic2Operand::Type::BITWISE_OR:
						value = left_value | right_value;
						break;

					case ExpressionArithmetic2Operand::Type::BITWISE_XOR:
						value = left_value ^ right_value;
						break;

					case ExpressionArithmetic2Operand::Type::BITWISE_AND:
						value = left_value & right_value;
						break;

					case ExpressionArithmetic2Operand::Type::EQUALITY:
						value = left_value == right_value ? -1 : 0;
						break;

					case ExpressionArithmetic2Operand::Type::INEQUALITY:
						value = left_value != right_value ? -1 : 0;
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
								CC_UNREACHABLE;
								/* Fallthrough */
							case ExpressionArithmetic2Operand::Type::LESS_THAN:
								value = left_value < right_value;
								break;

							case ExpressionArithmetic2Operand::Type::LESS_OR_EQUAL:
								value = left_value <= right_value;
								break;

							case ExpressionArithmetic2Operand::Type::MORE_THAN:
								value = left_value > right_value;
								break;

							case ExpressionArithmetic2Operand::Type::MORE_OR_EQUAL:
								value = left_value >= right_value;
								break;
						}

						value = -value;
						break;

					/* asm68k does 32-bit arithmetic, so it masks the shift amount to 0-31 (though whether this is intentional or just a side-effect of using x86 shift instructions that do this is unknown).
					   (note: this is also needed to prevent undefined behaviour in C from shifting by an amount greater than or equal to the width of the type). */
					/* TODO: Print a warning here. */
					case ExpressionArithmetic2Operand::Type::LEFT_SHIFT:
						value = left_value << (right_value % 32);
						break;

					case ExpressionArithmetic2Operand::Type::RIGHT_SHIFT:
						value = left_value >> (right_value % 32);
						break;
				}

				/* Prevent 'bleeding' out of the 68k's 32-bit range. */
				return value & 0xFFFFFFFF;
			},
			[&](ExpressionArithmetic1Operand &expression) -> std::optional<unsigned long>
			{
				const auto &value_optional = ResolveExpressionCommon(*expression.subexpressions, fold);

				if (!value_optional.has_value())
					return std::nullopt;

				auto value = *value_optional;

				switch (expression.type)
				{
					default:
						CC_UNREACHABLE;
						/* Fallthrough */
					case ExpressionArithmetic1Operand::Type::NEGATE:
						value = 0 - value;
						break;

					case ExpressionArithmetic1Operand::Type::BITWISE_NOT:
						value = ~value;
						break;

					case ExpressionArithmetic1Operand::Type::LOGICAL_NOT:
						value = value == 0 ? -1 : 0;
						break;
				}

				/* Prevent 'bleeding' out of the 68k's 32-bit range. */
				return value & 0xFFFFFFFF;
			},
			[&](ExpressionNumber &expression) -> std::optional<unsigned long>
			{
				return expression.value;
			},
			[&](ExpressionIdentifier &expression) -> std::optional<unsigned long>
			{
				return symbol_manager.GetSymbolInteger(expression.string, false);
			},
			[&](ExpressionString &expression) -> std::optional<unsigned long>
			{
				const std::size_t length = expression.string.length();

				if (length > 4)
				{
					error_manager.Error("Character literals cannot be more than 4 characters long.");
					return std::nullopt;
				}

				unsigned long value = 0;

				for (std::size_t i = 0; i < length; ++i)
				{
					value <<= 8;
					value |= expression.string[i];
				}

				return value;
			},
			[&](ExpressionProgramCounterOfStatement&) -> std::optional<unsigned long>
			{
				return program_counter_of_statement;
			},
			[&](ExpressionProgramCounterOfExpression&) -> std::optional<unsigned long>
			{
				return program_counter_of_expression;
			},
			[&](ExpressionStrlen &expression) -> std::optional<unsigned long>
			{
				return expression.string.length();
			},
			[&](ExpressionStrcmp &expression) -> std::optional<unsigned long>
			{
				return expression.string_a == expression.string_b ? -1 : 0;
			},
			[&](ExpressionInstr &expression) -> std::optional<unsigned long>
			{
				const auto &position_optional = ResolveExpressionCommon(expression.subexpressions[0], fold);

				if (!position_optional.has_value())
					return std::nullopt;

				const auto &position = *position_optional;

				const auto expression_string_a = std::get_if<ExpressionString>(&expression.subexpressions[1]);
				const auto expression_string_b = std::get_if<ExpressionString>(&expression.subexpressions[2]);

				if (expression_string_a == nullptr || expression_string_b == nullptr)
				{
					error_manager.Error("Last two arguments to `INSTR` must be strings.");
					return std::nullopt;
				}

				/* TODO: What if 'position' is 0? */
				const std::size_t found_position = Utilities::CaseInsensitive::FindString(expression_string_a->string, expression_string_b->string, position - 1, options->case_insensitive);
				return found_position == expression_string_a->string.npos ? 0 : found_position + 1;
			},
			[&](ExpressionDef &expression) -> std::optional<unsigned long>
			{
				return symbol_manager.LookupSymbol(expression.string, nullptr) != nullptr ? -1 : 0;
			},
			[&](ExpressionTypeWithIdentifier&) -> std::optional<unsigned long>
			{
				return 0; /* TODO: This is a placeholder! */
			},
			[&](ExpressionTypeWithNumber&) -> std::optional<unsigned long>
			{
				return 1; /* TODO: This is a placeholder! */
			},
			[&](ExpressionFilesize &expression) -> std::optional<unsigned long>
			{
				const auto &standard_path = Utilities::Path::ToStandardPath(expression.string);

				if (!std::filesystem::exists(standard_path))
				{
					error_manager.Error("File '{}' does not exist.", expression.string);
					return std::nullopt;
				}

				return std::filesystem::file_size(standard_path);
			}
		}, expression
	);

	/* Now that we have resolved the value, let's hardcode it here so that we don't ever have to calculate it again. */
	/* This is especially useful for fix-ups, which may otherwise depend on identifiers that no longer exist at the time the value is resolved again. */
	if (value.has_value() && fold)
		expression.emplace<ExpressionNumber>(*value);

	return value;
}
