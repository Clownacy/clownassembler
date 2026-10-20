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

#ifndef EXPRESSION_PROCESSOR_H
#define EXPRESSION_PROCESSOR_H

#include <optional>

#include "syntactic.h"

// TODO: HACK HACK HACK HACK HACK!!!
struct SemanticState;

std::optional<unsigned long> ResolveExpressionCommon(SemanticState &state, Expression &expression, bool fold);
std::optional<unsigned long> ResolveExpressionCommon(SemanticState &state, Expression &expression, bool fold, const std::string_view &message);

inline std::optional<unsigned long> ResolveExpression(SemanticState &state, Expression &expression, const std::string_view message = {})
{
	return ResolveExpressionCommon(state, expression, true, message);
}

inline std::optional<unsigned long> ResolveExpressionWithoutFolding(SemanticState &state, Expression &expression, const std::string_view message = {})
{
	return ResolveExpressionCommon(state, expression, false, message);
}


#endif /* EXPRESSION_PROCESSOR_H */
