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
	std::ostream *errors = nullptr,
	std::ostream *listing = nullptr,
	std::ostream *symbols = nullptr,
	const char *input_file_path = "[Unspecified]",
	const ClownAssembler_Settings &settings = {},
	const std::function<void(void *internal, ClownAssembler_AddDefinition add_definition)> *definition_callback = nullptr
);

}

#endif // CLOWNASSEMBLER_CPP_H
