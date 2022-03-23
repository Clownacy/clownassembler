#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdio.h>

#include "clowncommon.h"

#include "syntactic.h"

cc_bool ClownAssembler_Assemble(FILE *input_file, FILE *output_file, FILE *listing_file, const char *input_file_path, cc_bool debug, cc_bool case_insensitive);

#endif /* SEMANTIC_H */
