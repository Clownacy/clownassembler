#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>

#include "clowncommon.h"

#include "syntactic.h"

cc_bool ClownAssembler_Assemble(FILE *input_file, FILE *output_file, const char *input_file_path);

#endif /* SEMANTIC_H */
