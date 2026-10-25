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

#ifndef INSTRUCTION_PROCESSOR_H
#define INSTRUCTION_PROCESSOR_H

#include "parser.h"

class Global;
class ObjectWriter;

class InstructionProcessor
{
private:
	Global &global;
	ObjectWriter &object_writer;

	static unsigned int ConstructSizeBits(Size size);
	unsigned int ConstructEffectiveAddressBits(const Operand *operand);
	static unsigned int ToAlternateEffectiveAddressBits(unsigned int bits);
	void ResolveInstructionAmbiguity(StatementInstruction &instruction);
	void OptimiseInstruction(StatementInstruction &instruction);

public:
	InstructionProcessor(
		Global &global,
		ObjectWriter &object_writer
	)
		: global(global)
		, object_writer(object_writer)
	{}

	void Process(StatementInstruction &instruction);
};

#endif /* INSTRUCTION_PROCESSOR_H */
