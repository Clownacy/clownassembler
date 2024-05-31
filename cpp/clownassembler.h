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
	bool debug,
	bool case_insensitive,
	bool equ_set_descope_local_labels,
	bool output_local_labels_to_sym_file,
	bool warnings_enabled,
	const std::function<void(void *internal, ClownAssembler_AddDefinition add_definition)> *definition_callback
);

}

#endif // CLOWNASSEMBLER_CPP_H
