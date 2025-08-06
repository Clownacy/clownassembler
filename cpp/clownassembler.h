#ifndef CLOWNASSEMBLER_CPP_H
#define CLOWNASSEMBLER_CPP_H

#include <functional>
#include <iostream>

#include "../semantic.h"

namespace ClownAssembler
{

bool Assemble(
	std::istream &input,
	std::ostream &output,
	std::ostream *errors,
	std::ostream *listing,
	std::ostream *symbols,
	const char *input_file_path,
	const ClownAssembler_Settings *settings,
	const std::function<void(void *internal, ClownAssembler_AddDefinition add_definition)> *definition_callback
);

}

#endif // CLOWNASSEMBLER_CPP_H
