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

#include "error.h"
#include "object-writer.h"
#include "options.h"
#include "program-counter.h"
#include "syntactic.h"

class ExpressionProcessor;

class InstructionProcessor
{
private:
	ObjectWriter &object_writer;
	ErrorManager &error_manager;
	ExpressionProcessor &expression_processor;
	ProgramCounter &program_counter;
	const OptionsStack &options;

	static unsigned int ConstructSizeBits(Size size);
	unsigned int ConstructEffectiveAddressBits(const Operand *operand);
	static unsigned int ToAlternateEffectiveAddressBits(unsigned int bits);
	void ResolveInstructionAmbiguity(StatementInstruction &instruction);
	void OptimiseInstruction(StatementInstruction &instruction);

public:
	InstructionProcessor(
		ObjectWriter &object_writer,
		ErrorManager &error_manager,
		ExpressionProcessor &expression_processor,
		ProgramCounter &program_counter,
		OptionsStack &options
	)
		: object_writer(object_writer)
		, error_manager(error_manager)
		, expression_processor(expression_processor)
		, program_counter(program_counter)
		, options(options)
	{}

	void Process(StatementInstruction &instruction);
};

#endif /* INSTRUCTION_PROCESSOR_H */
