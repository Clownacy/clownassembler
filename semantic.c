/*
 * Copyright (C) 2022 Clownacy
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

#include "semantic.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clowncommon/clowncommon.h"

#include "dictionary.h"
#include "strcmpci.h"
#include "syntactic.h"
#define YY_NO_UNISTD_H
#include "lexical.h"

#define PROGRAM_COUNTER_OF_STATEMENT ",PROGRAM_COUNTER_OF_STATEMENT"
#define PROGRAM_COUNTER_OF_EXPRESSION ",PROGRAM_COUNTER_OF_EXPRESSION"

typedef ClownAssembler_TextInput TextInput;
typedef ClownAssembler_BinaryOutput BinaryOutput;
typedef ClownAssembler_TextOutput TextOutput;

typedef enum SymbolType
{
	SYMBOL_CONSTANT,
	SYMBOL_VARIABLE,
	SYMBOL_MACRO,
	SYMBOL_LABEL
} SymbolType;

typedef struct Location
{
	struct Location *previous;

	char *file_path;
	unsigned long line_number;
} Location;

typedef struct FixUp
{
	struct FixUp *next;

	Statement statement;
	unsigned long program_counter;
	size_t output_position;
	char *last_global_label;
	char *source_line;
	Location location;
	char *label;
} FixUp;

typedef struct SourceLineListNode
{
	struct SourceLineListNode *next;

	char *source_line;
} SourceLineListNode;

typedef struct SourceLineList
{
	SourceLineListNode *head;
	SourceLineListNode *tail;
} SourceLineList;

typedef enum Mode
{
	MODE_NORMAL,
	MODE_REPT,
	MODE_MACRO,
	MODE_WHILE
} Mode;

typedef struct SemanticState
{
	cc_bool success;
	const TextInput *input_callbacks;
	const BinaryOutput *output_callbacks;
	const TextOutput *listing_callbacks;
	const TextOutput *error_callbacks;
	cc_bool equ_set_descope_local_labels;
	cc_bool warnings_enabled;
	size_t output_position;
	cc_bool output_written_to;
	unsigned long start_position;
	unsigned long program_counter;
	cc_bool obj_active;
	unsigned long obj_delta;
	FixUp *fix_up_list_head;
	FixUp *fix_up_list_tail;
	cc_bool fix_up_needed;
	cc_bool doing_fix_up;
	cc_bool doing_final_pass;
	Dictionary_State dictionary;
	char *last_global_label;
	Location *location;
	yyscan_t flex_state;
	char line_buffer[1024];
	const char *source_line;
	unsigned int current_if_level;
	unsigned int false_if_level;
	cc_bool true_already_found;
	cc_bool end;
	unsigned int listing_counter;
	Mode mode;
	union
	{
		struct
		{
			unsigned long repetitions;
			unsigned long line_number;
			SourceLineList source_line_list;
		} rept;
		struct
		{
			char *name;
			unsigned long line_number;
			IdentifierList parameter_names;
			SourceLineList source_line_list;
			cc_bool is_short, uses_label;
		} macro;
		struct
		{
			Expression expression;
			char *source_line;
			unsigned long line_number;
			SourceLineList source_line_list;
		} while_statement;
	} shared;
} SemanticState;

typedef struct Macro
{
	cc_bool uses_label;
	char *name;
	IdentifierListNode *parameter_names;
	SourceLineListNode *source_line_list_head;
	unsigned int current_invocation;
} Macro;

/* Some forward declarations that are needed because some functions recurse into each other. */
static void AssembleFile(SemanticState *state);
static void AssembleLine(SemanticState *state, const char *source_line);

/* Prevent errors when '__attribute__((format(printf, X, X)))' is not supported. */
/* GCC 3.2 is the earliest version of GCC of which I can find proof of supporting this. */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 2))
#define ATTRIBUTE_PRINTF(a, b) __attribute__((format(printf, a, b)))
#else
#define ATTRIBUTE_PRINTF(a, b)
#endif

/* IO Callbacks */

static int TextInput_fgetc(const TextInput* const callbacks)
{
	return callbacks->read_character((void*)callbacks->user_data);
}

static char* TextInput_fgets(char* const buffer, const size_t buffer_size, const TextInput* const callbacks)
{
	return callbacks->read_line((void*)callbacks->user_data, buffer, buffer_size);
}

static cc_bool BinaryOutput_exists(const BinaryOutput* const callbacks)
{
	return callbacks != NULL && callbacks->user_data != NULL;
}

static void BinaryOutput_fputc(const int character, const BinaryOutput* const callbacks)
{
	callbacks->write_character((void*)callbacks->user_data, character);
}

static void BinaryOutput_fseek(SemanticState* const state, const BinaryOutput* const callbacks, const size_t position)
{
	state->output_position = position;
	callbacks->seek((void*)callbacks->user_data, position);
}

static void BinaryOutput_fwrite(const char* const buffer, const size_t size, const size_t count, const BinaryOutput* const callbacks)
{
	callbacks->write_characters((void*)callbacks->user_data, buffer, size * count);
}

static cc_bool TextOutput_exists(const TextOutput* const callbacks)
{
	return callbacks != NULL && callbacks->user_data != NULL;
}

static void TextOutput_vfprintf(const TextOutput* const callbacks, const char* const format, va_list args)
{
	if (TextOutput_exists(callbacks))
		callbacks->print_formatted((void*)callbacks->user_data, format, args);
}

ATTRIBUTE_PRINTF(2, 3) static void TextOutput_fprintf(const TextOutput* const callbacks, const char* const format, ...)
{
	if (TextOutput_exists(callbacks))
	{
		va_list args;
		va_start(args, format);
		TextOutput_vfprintf(callbacks, format, args);
		va_end(args);
	}
}

static void TextOutput_fputs(const char* const string, const TextOutput* const callbacks)
{
	if (TextOutput_exists(callbacks))
		callbacks->write_string((void*)callbacks->user_data, string);
}

static void TextOutput_fputc(const int character, const TextOutput* const callbacks)
{
	if (TextOutput_exists(callbacks))
		callbacks->write_character((void*)callbacks->user_data, character);
}

static void WriteOutputByte(SemanticState* const state, const unsigned char byte)
{
	state->output_written_to = cc_true;
	++state->output_position;
	BinaryOutput_fputc(byte, state->output_callbacks);
}

/* Default FILE-based IO callbacks */

static int ReadCharacter(void* const user_data)
{
	const int value = fgetc((FILE*)user_data);
	return value == EOF ? -1 : value;
}

static char* ReadLine(void* const user_data, char* const buffer, const size_t buffer_size)
{
	return fgets(buffer, buffer_size, (FILE*)user_data);
}

static void Seek(void* const user_data, const size_t position)
{
	fseek((FILE*)user_data, position, SEEK_SET);
}

static void WriteCharacter(void* const user_data, const int character)
{
	fputc(character, (FILE*)user_data);
}

static void WriteCharacters(void* const user_data, const char* const characters, const size_t total_characters)
{
	fwrite(characters, 1, total_characters, (FILE*)user_data);
}

static void WriteString(void* const user_data, const char* const string)
{
	fputs(string, (FILE*)user_data);
}

static void PrintFormatted(void* const user_data, const char* const format, va_list args)
{
	vfprintf((FILE*)user_data, format, args);
}

/* Other stuff */

static void ErrorMessageCommon(SemanticState *state)
{
	const Location *location;

	for (location = state->location; location != NULL; location = location->previous)
		TextOutput_fprintf(state->error_callbacks, "\nOn line %lu of '%s'...", location->line_number, location->file_path != NULL ? location->file_path : "[No path given]");

	TextOutput_fprintf(state->error_callbacks, "\n%s\n\n", state->source_line != NULL ? state->source_line : "[No source line]");
}

ATTRIBUTE_PRINTF(2, 3) static void SemanticWarning(SemanticState *state, const char *fmt, ...)
{
	if (state->warnings_enabled)
	{
		va_list args;

		TextOutput_fputs("Warning: ", state->error_callbacks);

		va_start(args, fmt);
		TextOutput_vfprintf(state->error_callbacks, fmt, args);
		va_end(args);

		ErrorMessageCommon(state);
	}
}

ATTRIBUTE_PRINTF(2, 3) static void SemanticError(SemanticState *state, const char *fmt, ...)
{
	va_list args;

	TextOutput_fputs("Error: ", state->error_callbacks);

	va_start(args, fmt);
	TextOutput_vfprintf(state->error_callbacks, fmt, args);
	va_end(args);

	ErrorMessageCommon(state);

	state->success = cc_false;
}

ATTRIBUTE_PRINTF(2, 3) static void InternalError(SemanticState *state, const char *fmt, ...)
{
	va_list args;

	TextOutput_fputs("Internal error: ", state->error_callbacks);

	va_start(args, fmt);
	TextOutput_vfprintf(state->error_callbacks, fmt, args);
	va_end(args);

	ErrorMessageCommon(state);

	state->success = cc_false;
}

static void OutOfMemoryError(SemanticState *state)
{
	TextOutput_fputs("Out-of-memory error.", state->error_callbacks);

	ErrorMessageCommon(state);

	state->success = cc_false;
}

void m68kasm_warning(void *scanner, Statement *statement, const char *message)
{
	SemanticState *state = (SemanticState*)m68kasm_get_extra(scanner);

	if (state->warnings_enabled)
	{
		(void)statement;

		TextOutput_fprintf(state->error_callbacks, "Warning: %s", message);

		ErrorMessageCommon(state);
	}
}

void m68kasm_error(void *scanner, Statement *statement, const char *message)
{
	SemanticState *state = (SemanticState*)m68kasm_get_extra(scanner);

	(void)statement;

	TextOutput_fprintf(state->error_callbacks, "Error: %s", message);

	ErrorMessageCommon(state);
}

static void* MallocAndHandleError(SemanticState *state, size_t size)
{
	void *memory = malloc(size);

	if (memory == NULL)
		OutOfMemoryError(state);

	return memory;
}

static char* DuplicateStringWithLength(SemanticState *state, const char *string, const size_t length)
{
	char *duplicated_string;

	duplicated_string = (char*)MallocAndHandleError(state, length + 1);

	if (duplicated_string != NULL)
	{
		memcpy(duplicated_string, string, length);
		duplicated_string[length] = '\0';
	}

	return duplicated_string;
}

static char* DuplicateString(SemanticState *state, const char *string)
{
	return DuplicateStringWithLength(state, string, strlen(string));
}

static unsigned int GetDecimalIntegerStringLength(unsigned int integer)
{
	unsigned int string_length;

	string_length = 1;

	while (integer >= 10)
	{
		integer /= 10;
		++string_length;
	}

	return string_length;
}

static unsigned int GetHexadecimalIntegerStringLength(unsigned int integer)
{
	unsigned int string_length;

	string_length = 1;

	while (integer >= 0x10)
	{
		integer /= 0x10;
		++string_length;
	}

	return string_length;
}

static char* DecimalIntegerToString(SemanticState *state, const unsigned int integer)
{
	char* const integer_string = (char*)MallocAndHandleError(state, GetDecimalIntegerStringLength(integer) + 1);

	if (integer_string != NULL)
		sprintf(integer_string, "%u", integer);

	return integer_string;
}

static char* HexadecimalIntegerToString(SemanticState *state, const unsigned int integer)
{
	char* const integer_string = (char*)MallocAndHandleError(state, GetHexadecimalIntegerStringLength(integer) + 1);

	if (integer_string != NULL)
		sprintf(integer_string, "%X", integer);

	return integer_string;
}

static char* ComputeUniqueMacroSuffix(SemanticState *state, Macro* const macro)
{
	const unsigned int integer_string_length = GetDecimalIntegerStringLength(macro->current_invocation);
	const unsigned int suffix_string_length = 1 + CC_MAX(3, integer_string_length);
	char* const suffix = (char*)MallocAndHandleError(state, suffix_string_length + 1);

	if (suffix != NULL)
		sprintf(suffix, "_%03u", macro->current_invocation);

	++macro->current_invocation;

	return suffix;
}

static FILE* fopen_backslash(SemanticState *state, const char *path, const char *mode)
{
	FILE *file;

	char* const path_copy = DuplicateString(state, path);

	if (path_copy == NULL)
	{
		file = NULL;
	}
	else
	{
		char *backslash;

		backslash = path_copy;

		while ((backslash = strchr(backslash, '\\')) != NULL)
			*backslash++ = '/';

		file = fopen(path_copy, mode);

		free(path_copy);
	}

	return file;
}

static cc_bool IsMacroBlockingCharacter(const char character)
{
	return ((character >= 'a' && character <= 'z')
	     || (character >= 'A' && character <= 'Z')
	     || (character >= '0' && character <= '9'));
}

static cc_bool FindMacroParameter(const char* const remaining_line, const char* const identifier, const char** const other_parameter_position_out, const char** const other_parameter_end_out)
{
	const char *other_parameter_position;
	const char *other_parameter_end = remaining_line;

	for (;;)
	{
		other_parameter_position = strstr(other_parameter_end, identifier);

		/* Obviously bail if the identifier wasn't found. */
		if (other_parameter_position == NULL)
			break;

		other_parameter_end = other_parameter_position + strlen(identifier);

		/* If the identifier was in the middle of a larger block of letters/numbers, then don't replace it. */
		/* (This is what AS does, and the Sonic 1 disassembly relies on this). */
		if ((other_parameter_position == remaining_line || !IsMacroBlockingCharacter(other_parameter_position[-1])) && !IsMacroBlockingCharacter(other_parameter_end[0]))
		{
			/* If the parameter is surrounded by backslashes, then expand the match to replace those too. */
			/* asm68k allows backslashes before and after the parameter to separate them from surrounding characters. */
			if (other_parameter_position != remaining_line && other_parameter_position[-1] == '\\')
			{
				--other_parameter_position;

				if (other_parameter_end[0] == '\\')
					++other_parameter_end;
			}

			*other_parameter_position_out = other_parameter_position;
			*other_parameter_end_out = other_parameter_end;

			return cc_true;
		}
	}

	return cc_false;
}

/* TODO: Duplicate this and use function pointers. */
static void OutputByte(SemanticState *state, unsigned int byte)
{
	/* Write to listing file. */
	if (!state->doing_fix_up && TextOutput_exists(state->listing_callbacks))
	{
		/* We can only write up to 10 bytes. */
		if (state->listing_counter <= 10)
		{
			if (state->listing_counter == 10)
			{
				/* After the last byte, we output a '+' to signal there's more. */
				TextOutput_fputc('+', state->listing_callbacks);
			}
			else
			{
				if (state->listing_counter % 2 == 0)
					TextOutput_fputc(' ', state->listing_callbacks);

				TextOutput_fprintf(state->listing_callbacks, "%02X", byte);
			}

			++state->listing_counter;
		}
	}

	/* Write to output file. */
	WriteOutputByte(state, byte);
}

static char* ExpandIdentifier(SemanticState *state, const char* const identifier, const size_t identifier_length, size_t* const expanded_identifier_length)
{
	char *expanded_identifier = NULL;

	if (identifier_length != 0 && (identifier[0] == '@' || identifier[0] == '.'))
	{
		if (state->last_global_label == NULL)
		{
			expanded_identifier = DuplicateStringWithLength(state, identifier + 1, identifier_length - 1);

			if (expanded_identifier_length != NULL)
				*expanded_identifier_length = identifier_length - 1;
		}
		else
		{
			const size_t prefix_length = strlen(state->last_global_label);
			const size_t suffix_length = identifier_length;
			expanded_identifier = (char*)MallocAndHandleError(state, prefix_length + suffix_length + 1);

			if (expanded_identifier != NULL)
			{
				memcpy(&expanded_identifier[0], state->last_global_label, prefix_length);
				memcpy(&expanded_identifier[prefix_length], identifier, suffix_length);
				expanded_identifier[prefix_length + suffix_length] = '\0';

				if (expanded_identifier_length != NULL)
					*expanded_identifier_length = prefix_length + suffix_length;
			}
		}
	}

	return expanded_identifier;
}

static Dictionary_Entry* LookupSymbol(SemanticState *state, const char *identifier, size_t identifier_length)
{
	Dictionary_Entry *dictionary_entry;
	size_t expanded_identifier_length;

	char* const expanded_identifier = ExpandIdentifier(state, identifier, identifier_length, &expanded_identifier_length);

	if (expanded_identifier != NULL)
	{
		identifier = expanded_identifier;
		identifier_length = expanded_identifier_length;
	}

	dictionary_entry = Dictionary_LookUp(&state->dictionary, identifier, identifier_length);

	free(expanded_identifier);

	return dictionary_entry;
}

static Dictionary_Entry* LookupSymbolAndCreateIfNotExist(SemanticState *state, const char *identifier, size_t identifier_length)
{
	Dictionary_Entry *dictionary_entry;
	size_t expanded_identifier_length;

	char* const expanded_identifier = ExpandIdentifier(state, identifier, identifier_length, &expanded_identifier_length);

	if (expanded_identifier != NULL)
	{
		identifier = expanded_identifier;
		identifier_length = expanded_identifier_length;
	}

	if (!Dictionary_LookUpAndCreateIfNotExist(&state->dictionary, identifier, identifier_length, &dictionary_entry))
	{
		OutOfMemoryError(state);
		dictionary_entry = NULL;
	}

	free(expanded_identifier);

	return dictionary_entry;
}

static Dictionary_Entry* CreateSymbol(SemanticState *state, const char *identifier, const size_t identifier_length)
{
	Dictionary_Entry *dictionary_entry;

	dictionary_entry = LookupSymbolAndCreateIfNotExist(state, identifier, identifier_length);

	if (dictionary_entry != NULL && dictionary_entry->type != -1)
	{
		SemanticError(state, "Symbol '%s' cannot be redefined.", identifier);
		dictionary_entry = NULL;
	}

	return dictionary_entry;
}

static cc_bool GetSymbolInteger(SemanticState *state, const char *identifier, const size_t identifier_length, const cc_bool must_evaluate_on_first_pass, unsigned long* const value)
{
	cc_bool success = cc_false;

	Dictionary_Entry *dictionary_entry;

	dictionary_entry = LookupSymbol(state, identifier, identifier_length);

	if (dictionary_entry == NULL || dictionary_entry->type == -1)
	{
		if (must_evaluate_on_first_pass)
			SemanticError(state, "Symbol '%s' must be evaluable on first pass.", identifier);
		else if (state->doing_final_pass)
			SemanticError(state, "Symbol '%s' does not exist.", identifier);
		else
			state->fix_up_needed = cc_true;
	}
	else if (dictionary_entry->type != SYMBOL_LABEL && dictionary_entry->type != SYMBOL_CONSTANT && dictionary_entry->type != SYMBOL_VARIABLE)
	{
		SemanticError(state, "Symbol '%s' is not a label, constant, or variable.", identifier);
	}
	else
	{
		success = cc_true;
		*value = dictionary_entry->shared.unsigned_long;
	}

	return success;
}

static void AddToSourceLineList(SemanticState *state, SourceLineList *source_line_list, const char *source_line)
{
	const size_t source_line_length = strlen(source_line);
	SourceLineListNode* const source_line_list_node = (SourceLineListNode*)MallocAndHandleError(state, sizeof(SourceLineListNode) + source_line_length + 1);

	if (source_line_list_node != NULL)
	{
		/* Append to the end of the list. */
		if (source_line_list->tail != NULL)
			source_line_list->tail->next = source_line_list_node;
		else
			source_line_list->head = source_line_list_node;

		source_line_list->tail = source_line_list_node;

		/* Initialise the list node. */
		source_line_list_node->next = NULL;
		source_line_list_node->source_line = (char*)(source_line_list_node + 1);
		memcpy(source_line_list_node->source_line, source_line, source_line_length + 1);
	}
}

static void FreeSourceLineList(SourceLineListNode *source_line_list_head)
{
	SourceLineListNode *source_line_list_node;

	source_line_list_node = source_line_list_head;

	while (source_line_list_node != NULL)
	{
		SourceLineListNode *next_source_line_list_node = source_line_list_node->next;

		free(source_line_list_node);

		source_line_list_node = next_source_line_list_node;
	}
}

static cc_bool ResolveExpression(SemanticState *state, Expression *expression, unsigned long *value, cc_bool fold)
{
	/* TODO - Maybe we should be doing signed arithmetic... that's what ProASM does, after all. */
	cc_bool success = cc_true;

	switch (expression->type)
	{
		case EXPRESSION_SUBTRACT:
		case EXPRESSION_ADD:
		case EXPRESSION_MULTIPLY:
		case EXPRESSION_DIVIDE:
		case EXPRESSION_MODULO:
		case EXPRESSION_LOGICAL_OR:
		case EXPRESSION_LOGICAL_AND:
		case EXPRESSION_BITWISE_OR:
		case EXPRESSION_BITWISE_XOR:
		case EXPRESSION_BITWISE_AND:
		case EXPRESSION_EQUALITY:
		case EXPRESSION_INEQUALITY:
		case EXPRESSION_LESS_THAN:
		case EXPRESSION_LESS_OR_EQUAL:
		case EXPRESSION_MORE_THAN:
		case EXPRESSION_MORE_OR_EQUAL:
		case EXPRESSION_LEFT_SHIFT:
		case EXPRESSION_RIGHT_SHIFT:
		{
			unsigned long left_value;
			unsigned long right_value;

			/* Resolve both of these separately so that they are each properly hardcoded (C short-circuiting could cause the second call to never occur). */
			if (!ResolveExpression(state, &expression->shared.subexpressions[0], &left_value, fold))
				success = cc_false;

			if (!ResolveExpression(state, &expression->shared.subexpressions[1], &right_value, fold))
				success = cc_false;

			if (success)
			{
				switch (expression->type)
				{
					case EXPRESSION_NUMBER:
					case EXPRESSION_IDENTIFIER:
					case EXPRESSION_STRING:
					case EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT:
					case EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION:
					case EXPRESSION_STRLEN:
					case EXPRESSION_STRCMP:
					case EXPRESSION_DEF:
					case EXPRESSION_NEGATE:
					case EXPRESSION_BITWISE_NOT:
					case EXPRESSION_LOGICAL_NOT:
						/* This code should never be ran. */
						break;

					case EXPRESSION_SUBTRACT:
						*value = left_value - right_value;
						break;

					case EXPRESSION_ADD:
						*value = left_value + right_value;
						break;

					case EXPRESSION_MULTIPLY:
						*value = left_value * right_value;
						break;

					case EXPRESSION_DIVIDE:
						*value = left_value / right_value;
						break;

					case EXPRESSION_MODULO:
						*value = left_value % right_value;
						break;

					case EXPRESSION_LOGICAL_OR:
						*value = left_value != 0 || right_value != 0 ? -1 : 0;
						break;

					case EXPRESSION_LOGICAL_AND:
						*value = left_value != 0 && right_value != 0 ? -1 : 0;
						break;

					case EXPRESSION_BITWISE_OR:
						*value = left_value | right_value;
						break;

					case EXPRESSION_BITWISE_XOR:
						*value = left_value ^ right_value;
						break;

					case EXPRESSION_BITWISE_AND:
						*value = left_value & right_value;
						break;

					case EXPRESSION_EQUALITY:
						*value = left_value == right_value ? -1 : 0;
						break;

					case EXPRESSION_INEQUALITY:
						*value = left_value != right_value ? -1 : 0;
						break;

					case EXPRESSION_LESS_THAN:
						*value = left_value < right_value ? -1 : 0;
						break;

					case EXPRESSION_LESS_OR_EQUAL:
						*value = left_value <= right_value ? -1 : 0;
						break;

					case EXPRESSION_MORE_THAN:
						*value = left_value > right_value ? -1 : 0;
						break;

					case EXPRESSION_MORE_OR_EQUAL:
						*value = left_value >= right_value ? -1 : 0;
						break;

					case EXPRESSION_LEFT_SHIFT:
						*value = left_value << right_value;
						break;

					case EXPRESSION_RIGHT_SHIFT:
						*value = left_value >> right_value;
						break;
				}

				/* Prevent 'bleeding' out of the 68k's 32-bit range. */
				*value &= 0xFFFFFFFF;
			}

			break;
		}

		case EXPRESSION_NEGATE:
		case EXPRESSION_BITWISE_NOT:
		case EXPRESSION_LOGICAL_NOT:
			if (!ResolveExpression(state, expression->shared.subexpressions, value, fold))
			{
				success = cc_false;
			}
			else
			{
				switch (expression->type)
				{
					case EXPRESSION_NUMBER:
					case EXPRESSION_IDENTIFIER:
					case EXPRESSION_STRING:
					case EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT:
					case EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION:
					case EXPRESSION_STRLEN:
					case EXPRESSION_STRCMP:
					case EXPRESSION_DEF:
					case EXPRESSION_SUBTRACT:
					case EXPRESSION_ADD:
					case EXPRESSION_MULTIPLY:
					case EXPRESSION_DIVIDE:
					case EXPRESSION_MODULO:
					case EXPRESSION_LOGICAL_OR:
					case EXPRESSION_LOGICAL_AND:
					case EXPRESSION_BITWISE_OR:
					case EXPRESSION_BITWISE_XOR:
					case EXPRESSION_BITWISE_AND:
					case EXPRESSION_EQUALITY:
					case EXPRESSION_INEQUALITY:
					case EXPRESSION_LESS_THAN:
					case EXPRESSION_LESS_OR_EQUAL:
					case EXPRESSION_MORE_THAN:
					case EXPRESSION_MORE_OR_EQUAL:
					case EXPRESSION_LEFT_SHIFT:
					case EXPRESSION_RIGHT_SHIFT:
						/* This code should never be ran. */
						break;

					case EXPRESSION_NEGATE:
						*value = 0 - *value;
						break;

					case EXPRESSION_BITWISE_NOT:
						*value = ~*value;
						break;

					case EXPRESSION_LOGICAL_NOT:
						*value = *value == 0 ? -1 : 0;
						break;
				}

				/* Prevent 'bleeding' out of the 68k's 32-bit range. */
				*value &= 0xFFFFFFFF;
			}

			break;

		case EXPRESSION_NUMBER:
			*value = expression->shared.unsigned_long;
			break;

		case EXPRESSION_IDENTIFIER:
			if (!GetSymbolInteger(state, expression->shared.string, strlen(expression->shared.string), cc_false, value))
				success = cc_false;

			break;

		case EXPRESSION_STRING:
		{
			const size_t length = strlen(expression->shared.string);

			if (length > 4)
			{
				success = cc_false;
				SemanticError(state, "Character literals cannot be more than 4 characters long.");
			}
			else
			{
				size_t i;

				*value = 0;

				for (i = 0; i < length; ++i)
				{
					*value <<= 8;
					*value |= expression->shared.string[i];
				}
			}

			break;
		}

		case EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT:
			*value = LookupSymbol(state, PROGRAM_COUNTER_OF_STATEMENT, sizeof(PROGRAM_COUNTER_OF_STATEMENT) - 1)->shared.unsigned_long;
			break;

		case EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION:
			*value = LookupSymbol(state, PROGRAM_COUNTER_OF_EXPRESSION, sizeof(PROGRAM_COUNTER_OF_EXPRESSION) - 1)->shared.unsigned_long;
			break;

		case EXPRESSION_STRLEN:
			*value = strlen(expression->shared.string);
			break;

		case EXPRESSION_STRCMP:
			*value = strcmp(expression->shared.subexpressions[0].shared.string, expression->shared.subexpressions[1].shared.string) == 0;
			break;

		case EXPRESSION_DEF:
			*value = LookupSymbol(state, expression->shared.string, strlen(expression->shared.string)) != NULL;
			break;
	}

	/* Now that we have resolved the value, let's hardcode it here so that we don't ever have to calculate it again. */
	/* This is especially useful for fix-ups, which may otherwise depend on identifiers that no longer exist at the time is value is resolved again. */
	if (success && fold)
	{
		switch (expression->type)
		{
			case EXPRESSION_SUBTRACT:
			case EXPRESSION_ADD:
			case EXPRESSION_MULTIPLY:
			case EXPRESSION_DIVIDE:
			case EXPRESSION_MODULO:
			case EXPRESSION_LOGICAL_OR:
			case EXPRESSION_LOGICAL_AND:
			case EXPRESSION_BITWISE_OR:
			case EXPRESSION_BITWISE_XOR:
			case EXPRESSION_BITWISE_AND:
			case EXPRESSION_EQUALITY:
			case EXPRESSION_INEQUALITY:
			case EXPRESSION_LESS_THAN:
			case EXPRESSION_LESS_OR_EQUAL:
			case EXPRESSION_MORE_THAN:
			case EXPRESSION_MORE_OR_EQUAL:
			case EXPRESSION_LEFT_SHIFT:
			case EXPRESSION_RIGHT_SHIFT:
			case EXPRESSION_NEGATE:
			case EXPRESSION_BITWISE_NOT:
			case EXPRESSION_LOGICAL_NOT:
			case EXPRESSION_STRCMP:
				free(expression->shared.subexpressions);
				break;

			case EXPRESSION_IDENTIFIER:
			case EXPRESSION_STRING:
			case EXPRESSION_STRLEN:
			case EXPRESSION_DEF:
				free(expression->shared.string);
				break;

			case EXPRESSION_NUMBER:
			case EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT:
			case EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION:
				break;
		}

		expression->type = EXPRESSION_NUMBER;
		expression->shared.unsigned_long = *value;
	}

	return success;
}

static void TerminateRept(SemanticState *state)
{
	/* Exit REPT mode. */
	state->mode = MODE_NORMAL;
}

static void TerminateMacro(SemanticState *state)
{
	/* Exit macro mode. */
	state->mode = MODE_NORMAL;

	if (state->shared.macro.name != NULL)
	{
		Dictionary_Entry* const symbol = CreateSymbol(state, state->shared.macro.name, strlen(state->shared.macro.name));

		/* Add the macro to the symbol table. */
		if (symbol != NULL)
		{
			Macro* const macro = (Macro*)MallocAndHandleError(state, sizeof(Macro));

			if (macro != NULL)
			{
				macro->uses_label = state->shared.macro.uses_label;
				macro->name = state->shared.macro.name;
				macro->parameter_names = state->shared.macro.parameter_names.head;
				macro->source_line_list_head = state->shared.macro.source_line_list.head;
				macro->current_invocation = 0;

				symbol->type = SYMBOL_MACRO;
				symbol->shared.pointer = macro;
			}
		}
	}
}

static void TerminateWhile(SemanticState *state)
{
	Expression expression;

	/* Back-up some state into local variables, in case a nested WHILE statement clobbers it. */
	char* const source_line = state->shared.while_statement.source_line;
	const unsigned long starting_line_number = state->shared.while_statement.line_number;
	SourceLineListNode* const source_line_list_head = state->shared.while_statement.source_line_list.head;

	expression = state->shared.while_statement.expression;

	/* Exit WHILE mode before we recurse into the WHILE's nested statements. */
	state->mode = MODE_NORMAL;

	for (;;)
	{
		unsigned long value;
		SourceLineListNode *source_line_list_node;

		state->source_line = source_line;

		if (!ResolveExpression(state, &expression, &value, cc_false))
		{
			SemanticError(state, "Expression must be evaluable on the first pass.");
			break;
		}

		if (value == 0)
			break;

		/* Rewind back to the line number of the start of the REPT. */
		state->location->line_number = starting_line_number;

		/* Process the WHILE's nested statements. */
		for (source_line_list_node = source_line_list_head; source_line_list_node != NULL; source_line_list_node = source_line_list_node->next)
			AssembleLine(state, source_line_list_node->source_line);
	}

	DestroyExpression(&expression);

	/* Increment past the ENDW line number. */
	++state->location->line_number;

	free(source_line);
	FreeSourceLineList(source_line_list_head);
}

static unsigned int ConstructSizeBits(Size size)
{
	switch (size)
	{
		case SIZE_BYTE:
		case SIZE_SHORT:
			return 0x0000;

		case SIZE_WORD:
			return 0x0040;

		case SIZE_LONGWORD:
			return 0x0080;

		case SIZE_UNDEFINED:
			assert(cc_false);
			break;
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

static void SetLastGlobalLabel(SemanticState *state, const char *label)
{
	if (label[0] != '@' && label[0] != '.')
	{
		/* This is a global label - cache it for later. */
		free(state->last_global_label);
		state->last_global_label = DuplicateString(state, label);
	}
}

static void AddIdentifierToSymbolTable(SemanticState *state, const char *label, unsigned long value, SymbolType type)
{
	Dictionary_Entry *symbol;

	symbol = NULL;

	/* Now add it to the symbol table. */
	switch (type)
	{
		case SYMBOL_VARIABLE:
		case SYMBOL_CONSTANT:
			symbol = LookupSymbolAndCreateIfNotExist(state, label, strlen(label));

			if (symbol != NULL)
			{
				if (symbol->type != -1 && (SymbolType)symbol->type != type)
					SemanticError(state, "Symbol redefined as a different type.");
				else if (type == SYMBOL_CONSTANT && symbol->type == SYMBOL_CONSTANT && symbol->shared.unsigned_long != value)
					SemanticError(state, "Constant cannot be redefined to a different value.");
			}

			break;

		case SYMBOL_LABEL:
			symbol = CreateSymbol(state, label, strlen(label));
			break;

		case SYMBOL_MACRO:
			/* This should not happen. */
			assert(cc_false);
			break;
	}

	if (symbol != NULL)
	{
		symbol->type = type;
		symbol->shared.unsigned_long = value;
	}
}

static void ResolveInstructionAmbiguity(SemanticState* const state, StatementInstruction* const instruction)
{
	/* Some instructions are ambiguous, so figure them out fully here. */
	for (;;)
	{
		switch (instruction->opcode.type)
		{
			#define ALIAS_AND_LOOP(ALIAS) do {instruction->opcode.type = ALIAS; continue;} while (0)
			#define WARN_ALIAS_AND_LOOP(ALIAS, MESSAGE) do {SemanticWarning(state, MESSAGE); ALIAS_AND_LOOP(ALIAS);} while (0)

			case OPCODE_ORI:
				if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ORI_TO_CCR);
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ORI_TO_SR);

				break;

			case OPCODE_ANDI:
				if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ANDI_TO_CCR);
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ANDI_TO_SR);

				break;

			case OPCODE_EORI:
				if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_EORI_TO_CCR);
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_EORI_TO_SR);

				break;

			case OPCODE_BTST_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BTST_DYNAMIC);

				break;

			case OPCODE_BCHG_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BCHG_DYNAMIC);

				break;

			case OPCODE_BCLR_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BCLR_DYNAMIC);

				break;

			case OPCODE_BSET_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BSET_DYNAMIC);

				break;

			case OPCODE_MOVEP_TO_REG:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVEP_FROM_REG);

				break;

			case OPCODE_MOVE:
				if (instruction->operands[0].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_FROM_SR);
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_TO_SR);
				else if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_TO_CCR);
				else if (instruction->operands[0].type == OPERAND_USER_STACK_POINTER_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_FROM_USP);
				else if (instruction->operands[1].type == OPERAND_USER_STACK_POINTER_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_TO_USP);
				else if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_MOVEA, "MOVE should be MOVEA.");

				break;

			case OPCODE_MOVEM_TO_REGS:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER || instruction->operands[0].type == OPERAND_ADDRESS_REGISTER || instruction->operands[0].type == OPERAND_REGISTER_LIST)
					ALIAS_AND_LOOP(OPCODE_MOVEM_FROM_REGS);

				break;

			case OPCODE_SBCD_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_SBCD_ADDRESS_REGS);

				break;

			case OPCODE_OR_TO_REG:
				if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_ORI, "OR should be ORI.");
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_OR_FROM_REG);

				break;

			case OPCODE_SUB_TO_REG:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_SUBA, "SUB should be SUBA.");
				else if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_SUBI, "SUB should be SUBI.");
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_SUB_FROM_REG);

				break;

			case OPCODE_SUBX_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_SUBX_ADDRESS_REGS);

				break;

			case OPCODE_ABCD_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_ABCD_ADDRESS_REGS);

				break;

			case OPCODE_AND_TO_REG:
				if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_ANDI, "AND should be ANDI.");
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_AND_FROM_REG);

				break;

			case OPCODE_ADD_TO_REG:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_ADDA, "ADD should be ADDA.");
				else if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_ADDI, "ADD should be ADDI.");
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ADD_FROM_REG);

				break;

			case OPCODE_ADDX_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_ADDX_ADDRESS_REGS);

				break;

			case OPCODE_CMP:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_CMPA, "CMP should be CMPA.");
				else if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_CMPI, "CMP should be CMPI.");
				else if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT && instruction->operands[1].type == OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT)
					WARN_ALIAS_AND_LOOP(OPCODE_CMPM, "CMP should be CMPM.");

				break;

			case OPCODE_EOR:
				if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_EORI, "EOR should be EORI.");

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
								ALIAS_AND_LOOP(OPCODE_ASL_DYNAMIC);
								break;

							case OPCODE_ASR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ASR_DYNAMIC);
								break;

							case OPCODE_LSL_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSL_DYNAMIC);
								break;

							case OPCODE_LSR_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSR_DYNAMIC);
								break;

							case OPCODE_ROXL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXL_DYNAMIC);
								break;

							case OPCODE_ROXR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXR_DYNAMIC);
								break;

							case OPCODE_ROL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROL_DYNAMIC);
								break;

							case OPCODE_ROR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROR_DYNAMIC);
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
						instruction->operands[1] = instruction->operands[0];
						instruction->operands[0].type = OPERAND_LITERAL;
						instruction->operands[0].literal.type = EXPRESSION_NUMBER;
						instruction->operands[0].literal.shared.unsigned_long = 1;
					}
					else
					{
						switch (instruction->opcode.type)
						{
							case OPCODE_ASL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ASL_SINGLE);
								break;

							case OPCODE_ASR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ASR_SINGLE);
								break;

							case OPCODE_LSL_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSL_SINGLE);
								break;

							case OPCODE_LSR_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSR_SINGLE);
								break;

							case OPCODE_ROXL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXL_SINGLE);
								break;

							case OPCODE_ROXR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXR_SINGLE);
								break;

							case OPCODE_ROL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROL_SINGLE);
								break;

							case OPCODE_ROR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROR_SINGLE);
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

typedef struct InstructionMetadata
{
	const char *name;

	unsigned int allowed_sizes;

	unsigned int allowed_operands[2];
} InstructionMetadata;

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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUBI */
		"SUBI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_ADDI */
		"ADDI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_CMPI */
		"CMPI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_MOVEP_FROM_REG */
		"MOVEP",
		SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT,
		}
	},
	{	/* OPCODE_MOVEA */
		"MOVEA",
		SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_CLR */
		"CLR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_NEG */
		"NEG",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_NOT */
		"NOT",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_EXT */
		"EXT",
		SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUBQ */
		"SUBQ",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUB_TO_REG */
		"SUB",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUBX_DATA_REGS */
		"SUBX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_SUBX_ADDRESS_REGS */
		"SUBX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
		}
	},
	{	/* OPCODE_SUBA */
		"SUBA",
		SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_CMPM */
		"CMPM",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT,
			OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT,
		}
	},
	{	/* OPCODE_CMP */
		"CMP",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_ADD_TO_REG */
		"ADD",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_ADDX_DATA_REGS */
		"ADDX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ADDX_ADDRESS_REGS */
		"ADDX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
		}
	},
	{	/* OPCODE_ADDA */
		"ADDA",
		SIZE_WORD | SIZE_LONGWORD,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ASR_STATIC */
		"ASR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSL_STATIC */
		"LSL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSR_STATIC */
		"LSR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXL_STATIC */
		"ROXL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXR_STATIC */
		"ROXR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROL_STATIC */
		"ROL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROR_STATIC */
		"ROR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ASL_DYNAMIC */
		"ASL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ASR_DYNAMIC */
		"ASR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSL_DYNAMIC */
		"LSL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSR_DYNAMIC */
		"LSR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXL_DYNAMIC */
		"ROXL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXR_DYNAMIC */
		"ROXR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROL_DYNAMIC */
		"ROL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROR_DYNAMIC */
		"ROR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
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

	state->program_counter += 2;

	ResolveInstructionAmbiguity(state, instruction);

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

		SemanticError(state, "Instruction cannot be this size: allowed sizes are...%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", newline_byte, opcode_byte, size_byte, newline_short, opcode_short, size_short, newline_word, opcode_word, size_word, newline_longword, opcode_longword, size_longword, newline_undefined, opcode_undefined, size_undefined);
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
			SemanticError(state, "Instruction has %u operands, but it should have %u.", total_operands_have, total_operands_wanted);
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

					SemanticError(state, "%s operand cannot be %s.", i == 0 ? "First" : "Second", operand_string);
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

								if (!ResolveExpression(state, &instruction->operands[0].literal, &value, cc_true))
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

							case SIZE_WORD:
								machine_code = 0x3000;
								break;

							case SIZE_LONGWORD:
								machine_code = 0x2000;
								break;

							case SIZE_UNDEFINED:
								/* Should never happen. */
								assert(cc_false);
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

						if (!ResolveExpression(state, &instruction->operands[0].literal, &value, cc_true))
							value = 0;

						if (value > 15)
							SemanticError(state, "The vector cannot be higher than 15.");
						else
							machine_code |= value;

						/* The operand is embedded directly into the machine code, so we don't need to output it separately. */
						operands_to_output[0] = NULL;

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

						custom_operands[0] = *operands_to_output[0];
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

						if (!ResolveExpression(state, &instruction->operands[0].literal, &value, cc_true))
							value = 1;

						if (value < 1 || value > 8)
							SemanticError(state, "The value to add/subtract cannot be lower than 1 or higher than 8.");

						machine_code |= (value & 7) << 9;

						/* Skip the immediate operand since that goes in the machine code instead. */
						operands_to_output[0] = NULL;

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
						if (!ResolveExpression(state, &instruction->operands[1].literal, &value, cc_true))
							value = state->program_counter - 2;

						/* Construct a custom operand to hold the destination offset. */
						operands_to_output[0] = &custom_operands[0];
						operands_to_output[1] = NULL;
						custom_operands[0].type = OPERAND_LITERAL;
						custom_operands[0].literal.type = EXPRESSION_NUMBER;

						/* Calculate the destination offset. */
						if (value >= state->program_counter)
						{
							const unsigned long offset = value - state->program_counter;

							if (offset > 0x7FFF)
								SemanticError(state, "The destination is too far away: it must be less than $8000 bytes after the start of the instruction, but instead it was $%lX bytes away.", offset);

							custom_operands[0].literal.shared.unsigned_long = offset;
						}
						else
						{
							const unsigned long offset = state->program_counter - value;

							if (offset > 0x8000)
								SemanticError(state, "The destination is too far away: it must be less than $8001 bytes before the start of the instruction, but instead it was $%lX bytes away.", offset);

							custom_operands[0].literal.shared.unsigned_long = 0 - offset;
						}

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
						if (!ResolveExpression(state, &instruction->operands[0].literal, &value, cc_true))
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
									SemanticError(state, "The destination is too far away: it must be less than $80 bytes after the start of the instruction, but instead it was $%lX bytes away.", offset);
							}
							else
							{
								if (offset > 0x7FFF)
									SemanticError(state, "The destination is too far away: it must be less than $8000 bytes after the start of the instruction, but instead it was $%lX bytes away.", offset);
							}
						}
						else
						{
							offset = state->program_counter - value;

							if (instruction->opcode.size == SIZE_BYTE || instruction->opcode.size == SIZE_SHORT)
							{
								if (offset > 0x80)
									SemanticError(state, "The destination is too far away: it must be less than $81 bytes before the start of the instruction, but instead it was $%lX bytes away.", offset);
							}
							else
							{
								if (offset > 0x8000)
									SemanticError(state, "The destination is too far away: it must be less than $8001 bytes before the start of the instruction, but instead it was $%lX bytes away.", offset);
							}

							offset = 0 - offset;
						}

						if (instruction->opcode.size == SIZE_BYTE || instruction->opcode.size == SIZE_SHORT)
						{
							machine_code |= offset & 0xFF;

							/* The operand is embedded directly into the machine code, so we don't need to output it separately. */
							operands_to_output[0] = NULL;
						}
						else
						{
							/* Construct a custom operand to hold the destination offset. */
							operands_to_output[0] = &custom_operands[0];
							custom_operands[0].type = OPERAND_LITERAL;
							custom_operands[0].literal.type = EXPRESSION_NUMBER;
							custom_operands[0].literal.shared.unsigned_long = offset;
						}

						break;
					}

					case OPCODE_MOVEQ:
					{
						unsigned long value;

						machine_code = 0x7000;

						/* Obtain the value to be moved. */
						if (!ResolveExpression(state, &instruction->operands[0].literal, &value, cc_true))
							value = 0;

						if (value > 0x7F && value < 0xFFFFFF80)
							SemanticError(state, "Value is too large: it must be between -$80 and $7F, or $FFFFFF80 and $FFFFFFFF.");

						machine_code |= value & 0xFF;

						machine_code |= instruction->operands[1].main_register << 9;

						/* MOVEQ's operands are embedded directly into the machine code, so we don't need to output them separately. */
						operands_to_output[0] = NULL;
						operands_to_output[1] = NULL;

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

								if (!ResolveExpression(state, &instruction->operands[0].literal, &value, cc_true))
									value = 0;

								if (value > 8 || value < 1)
									SemanticError(state, "The shift value must not be greater than 8 or lower than 1.");

								machine_code |= (value & 7) << 9;

								machine_code |= identifier << 3;
								machine_code |= instruction->operands[1].main_register << 0;
								machine_code |= ConstructSizeBits(instruction->opcode.size);

								/* Skip the immediate operand since that goes in the machine code instead. */
								operands_to_output[0] = NULL;

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

		if (operand != NULL)
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

					if (!ResolveExpression(state, &operand->literal, &value, cc_true))
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
										SemanticError(state, "Word-sized addresses cannot be higher than $7FFF or lower than $FFFF8000.");

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
										SemanticError(state, "Byte-sized literals cannot be larger than $FF or smaller than -$100.");

									value &= 0xFF;

									break;

								case SIZE_UNDEFINED:
								case SIZE_WORD:
									bytes_to_write = 2;

									if (value >= 0x10000 && value < 0xFFFF0000)
										SemanticError(state, "Word-sized literals cannot be larger than $FFFF or smaller than -$10000.");

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
								SemanticError(state, "Displacement values cannot be larger than $7F or smaller than -$80, but was $%lX.", value);

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
								SemanticError(state, "Displacement values cannot be larger than $7FFF or smaller than -$8000, but was $%lX.", value);

							break;
					}

					state->program_counter += bytes_to_write;

					while (bytes_to_write-- > 0)
						OutputByte(state, (value >> (8 * bytes_to_write)) & 0xFF);

					break;
				}

				case OPERAND_REGISTER_LIST:
				{
					unsigned int bytes_to_write;

					state->program_counter += 2;

					for (bytes_to_write = 2; bytes_to_write-- > 0; )
						OutputByte(state, (operand->main_register >> (8 * bytes_to_write)) & 0xFF);

					break;
				}
			}
		}
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
				SemanticError(state, "Value cannot be higher than $FF or lower than -$100, but was $%lu.", value);

			bytes_to_write = 1;

			break;

		case SIZE_WORD:
			if (value > 0xFFFF && value < 0xFFFF0000)
				SemanticError(state, "Value cannot be higher than $FFFF or lower than -$10000, but was $%lu.", value);

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

	state->program_counter += bytes_to_write;

	while (bytes_to_write-- != 0)
		OutputByte(state, (value >> (bytes_to_write * 8)) & 0xFF);
}

static void ProcessDc(SemanticState *state, StatementDc *dc)
{
	ExpressionListNode *expression_list_node;

	for (expression_list_node = dc->values.head; expression_list_node != NULL; expression_list_node = expression_list_node->next)
	{
		if (expression_list_node->expression.type == EXPRESSION_STRING && (dc->size == SIZE_BYTE || dc->size == SIZE_SHORT))
		{
			/* If this is a 'dc.b', and the value is a raw string, then output each character as a single byte. */
			/* This allows 'dc.b' to be used to embed strings into the output. */
			const char *character;

			for (character = expression_list_node->expression.shared.string; *character != '\0'; ++character)
				OutputDcValue(state, dc->size, *character);
		}
		else
		{
			unsigned long value;

			/* Update the program counter symbol in between values, to keep it up to date. */
			LookupSymbol(state, PROGRAM_COUNTER_OF_EXPRESSION, sizeof(PROGRAM_COUNTER_OF_EXPRESSION) - 1)->shared.unsigned_long = state->program_counter;

			if (!ResolveExpression(state, &expression_list_node->expression, &value, cc_true))
				value = 0;

			OutputDcValue(state, dc->size, value);
		}
	}
}

static void ProcessDcb(SemanticState *state, StatementDcb *dcb)
{
	unsigned long repetitions;

	if (!ResolveExpression(state, &dcb->repetitions, &repetitions, cc_true))
	{
		SemanticError(state, "Repetition value must be evaluable on first pass.");
	}
	else
	{
		unsigned long value;
		unsigned long i;

		if (!ResolveExpression(state, &dcb->value, &value, cc_true))
			value = 0;

		for (i = 0; i < repetitions; ++i)
			OutputDcValue(state, dcb->size, value);	
	}
}

static void ProcessInclude(SemanticState *state, const StatementInclude *include)
{
	FILE* const input_file = fopen_backslash(state, include->path, "r");

	if (input_file == NULL)
	{
		SemanticError(state, "File '%s' could not be opened.", include->path);
	}
	else
	{
		const TextInput* const previous_input_callbacks = state->input_callbacks;

		TextInput input_callbacks = {
			input_file,
			ReadCharacter,
			ReadLine
		};

		/* Add file path and line number to the location list. */
		Location location;

		location.file_path = include->path;
		location.line_number = 0;

		location.previous = state->location;
		state->location = &location;

		state->input_callbacks = &input_callbacks;

		AssembleFile(state);

		state->input_callbacks = previous_input_callbacks;

		state->location = state->location->previous;

		fclose(input_file);
	}
}

static void ProcessIncbin(SemanticState *state, StatementIncbin *incbin)
{
	FILE* const input_file = fopen_backslash(state, incbin->path, "rb");

	if (input_file == NULL)
	{
		SemanticError(state, "File '%s' could not be opened.", incbin->path);
	}
	else
	{
		unsigned long value;

		if (!ResolveExpression(state, &incbin->start, &value, cc_true))
		{
			SemanticError(state, "Start value must be evaluable on the first pass.");
			value = 0;
		}

		if (fseek(input_file, value, SEEK_SET) != 0)
			SemanticError(state, "Start value is not a valid location in the file.");

		if (incbin->has_length)
		{
			unsigned long length;
			unsigned long i;

			if (!ResolveExpression(state, &incbin->length, &length, cc_true))
			{
				SemanticError(state, "Length value must be evaluable on the first pass.");
				length = 0;
			}

			for (i = 0; i < length; ++i)
			{
				const int character = fgetc(input_file);

				if (character == EOF)
				{
					SemanticError(state, "Length value is larger than the remaining number of bytes in the file.");
					break;
				}

				++state->program_counter;
				WriteOutputByte(state, character);
			}
		}
		else
		{
			int character;

			/* TODO: Batch bytes to make this faster? */
			while ((character = fgetc(input_file)) != EOF)
			{
				++state->program_counter;
				WriteOutputByte(state, character);
			}
		}

		fclose(input_file);
	}
}

static cc_bool ReadSourceLine(SemanticState *state)
{
	char *line_buffer_write_pointer;

	line_buffer_write_pointer = state->line_buffer;

	/* Read lines one at a time, feeding them to the 'AssembleLine' function. */
	while (TextInput_fgets(line_buffer_write_pointer, &state->line_buffer[sizeof(state->line_buffer)] - line_buffer_write_pointer, state->input_callbacks) != NULL)
	{
		size_t newline_index;
		char newline_character;

		/* Find the end of the line. We terminate on '\0', '\r', '\n', and ';'.
		   Note that terminating at ';' prevents a trailing '&' from being recognised
		   and causing a line to be continued on the next line.
		   This is needed for compatibility with S.N. 68k (asm68k). */
		{
			char quote_character = '\0';
			for (newline_index = 0; ; ++newline_index)
			{
				const char character = state->line_buffer[newline_index];

				if (character == '\0' || character == '\r' || character == '\n')
					break;

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
		}

		newline_character = state->line_buffer[newline_index];

		line_buffer_write_pointer = state->line_buffer;

		/* If there is no newline, then we've either reached the end of the file,
		   or the source line was too long to fit in the buffer. */
		/* TODO: Is there no way to remove this limit? */
		if (newline_character == '\0')
		{
			int character = TextInput_fgetc(state->input_callbacks);

			if (character != -1)
			{
				InternalError(state, "The source line was too long to fit in the internal buffer.");

				/* Fast-forward through until the end of the line. */
				while (character != '\r' && character != '\n' && character != -1)
					character = TextInput_fgetc(state->input_callbacks);
			}
		}
		else if (newline_index != 0 && state->line_buffer[newline_index - 1] == '&')
		{
			/* An '&' at the end of a line is like a '\' at the end of a line in C:
			   it signals that the current line is continued on the next line. */

			/* Go back and get another line. */
			line_buffer_write_pointer = &state->line_buffer[newline_index - 1];
			continue;
		}

		/* Remove newlines from the string, so that they don't appear in the error message. */
		state->line_buffer[newline_index] = '\0';

		return cc_true;
	}

	return cc_false;
}

static void AssembleAndListLine(SemanticState *state)
{
	/* Output program counter to listing file. */
	if (TextOutput_exists(state->listing_callbacks))
	{
		state->listing_counter = 0;
		TextOutput_fprintf(state->listing_callbacks, "%08lX", state->program_counter);
	}

	AssembleLine(state, state->line_buffer);

	/* Output line to listing file. */
	if (TextOutput_exists(state->listing_callbacks))
	{
		unsigned int i;

		for (i = state->listing_counter * 2 + state->listing_counter / 2; i < 28; ++i)
			TextOutput_fputc(' ', state->listing_callbacks);

		TextOutput_fprintf(state->listing_callbacks, "%s\n", state->line_buffer);
	}
}

static void ProcessRept(SemanticState *state, StatementRept *rept)
{
	const Mode previous_mode = state->mode;
	const unsigned long previous_repetitions = state->shared.rept.repetitions;
	const unsigned long previous_line_number = state->location->line_number;
	const SourceLineList previous_source_line_list = state->shared.rept.source_line_list;

	/* Enter REPT mode. */
	state->mode = MODE_REPT;

	if (!ResolveExpression(state, &rept->repetitions, &state->shared.rept.repetitions, cc_true))
	{
		SemanticError(state, "Repetition value must be evaluable on the first pass.");
		state->shared.rept.repetitions = 1;
	}

	state->shared.rept.line_number = state->location->line_number;

	state->shared.rept.source_line_list.head = NULL;
	state->shared.rept.source_line_list.tail = NULL;

	for (;;)
	{
		if (!ReadSourceLine(state))
		{
			/* The file ended before an 'ENDR' could be found! */

			/* Terminate the REPT to hopefully avoid future complications. */
			TerminateRept(state);

			SemanticError(state, "REPT statement beginning at line %lu is missing its ENDR.", state->shared.rept.line_number);
		}
		else
		{
			AssembleLine(state, state->line_buffer);
		}

		if (state->mode != MODE_REPT)
		{
			/* An 'ENDR' must have been encountered. */

			/* Revert back to the previous state. */
			unsigned long repetitions = state->shared.rept.repetitions;
			const unsigned long line_number = state->location->line_number;
			const SourceLineList source_line_list = state->shared.rept.source_line_list;

			state->mode = previous_mode;
			state->shared.rept.repetitions = previous_repetitions;
			state->location->line_number = previous_line_number;
			state->shared.rept.source_line_list = previous_source_line_list;

			while (repetitions-- != 0)
			{
				SourceLineListNode *source_line_list_node;

				/* Rewind back to the line number of the start of the REPT. */
				state->location->line_number = line_number;

				/* Process the REPT's nested statements. */
				for (source_line_list_node = source_line_list.head; source_line_list_node != NULL; source_line_list_node = source_line_list_node->next)
					AssembleLine(state, source_line_list_node->source_line);
			}

			/* Increment past the ENDR line number. */
			++state->location->line_number;

			FreeSourceLineList(source_line_list.head);

			break;
		}
	}
}

static void ProcessMacro(SemanticState *state, StatementMacro *macro, const char *label, cc_bool is_short)
{
	/* Enter MACRO mode. */
	state->mode = MODE_MACRO;

	state->shared.macro.name = DuplicateString(state, label);
	state->shared.macro.line_number = state->location->line_number;
	state->shared.macro.parameter_names = macro->parameter_names;
	macro->parameter_names.head = NULL;
	macro->parameter_names.tail = NULL;
	state->shared.macro.is_short = is_short;
	state->shared.macro.uses_label = macro->uses_label;

	state->shared.macro.source_line_list.head = NULL;
	state->shared.macro.source_line_list.tail = NULL;
}

static void ProcessIf(SemanticState *state, Expression *expression)
{
	unsigned long value;

	++state->current_if_level;

	if (!ResolveExpression(state, expression, &value, cc_true))
	{
		SemanticError(state, "Condition must be evaluable on the first pass.");
		value = 1;
	}

	/* If this condition is false, then mark this as the false if-level. */
	if (value == 0)
		state->false_if_level = state->current_if_level;

	state->true_already_found = value != 0;
}

static void ProcessStatement(SemanticState *state, Statement *statement, const char *label)
{
	/* Update both copies of the program counter. */
	LookupSymbol(state, PROGRAM_COUNTER_OF_STATEMENT, sizeof(PROGRAM_COUNTER_OF_STATEMENT) - 1)->shared.unsigned_long = state->program_counter;
	LookupSymbol(state, PROGRAM_COUNTER_OF_EXPRESSION, sizeof(PROGRAM_COUNTER_OF_EXPRESSION) - 1)->shared.unsigned_long = state->program_counter;

	switch (statement->type)
	{
		case STATEMENT_TYPE_EMPTY:
		case STATEMENT_TYPE_INSTRUCTION:
		case STATEMENT_TYPE_DC:
		case STATEMENT_TYPE_DCB:
		case STATEMENT_TYPE_INCLUDE:
		case STATEMENT_TYPE_INCBIN:
		case STATEMENT_TYPE_REPT:
		case STATEMENT_TYPE_IF:
		case STATEMENT_TYPE_WHILE:
		case STATEMENT_TYPE_EVEN:
		case STATEMENT_TYPE_CNOP:
		case STATEMENT_TYPE_INFORM:
		case STATEMENT_TYPE_END:
			if (label != NULL && !state->doing_fix_up)
			{
				/* Handle the label here, instead of passing it onto a later function. */
				SetLastGlobalLabel(state, label);
				AddIdentifierToSymbolTable(state, label, state->program_counter, SYMBOL_LABEL);
			}

			break;

		case STATEMENT_TYPE_ENDR:
		case STATEMENT_TYPE_ENDM:
		case STATEMENT_TYPE_ELSEIF:
		case STATEMENT_TYPE_ELSE:
		case STATEMENT_TYPE_ENDC:
		case STATEMENT_TYPE_ENDW:
		case STATEMENT_TYPE_RSSET:
		case STATEMENT_TYPE_RSRESET:
		case STATEMENT_TYPE_OBJ:
		case STATEMENT_TYPE_OBJEND:
		case STATEMENT_TYPE_ORG:
			if (label != NULL)
				SemanticError(state, "There cannot be a label on this type of statement.");

			break;

		case STATEMENT_TYPE_MACRO:
		case STATEMENT_TYPE_MACROS:
		case STATEMENT_TYPE_EQU:
		case STATEMENT_TYPE_SET:
			if (label == NULL)
			{
				SemanticError(state, "This type of statement must have a label.");
				/* Bail, to avoid null pointer dereferences. */
				return;
			}

			break;

		case STATEMENT_TYPE_RS:
			/* Handled later. */
			break;
	}

	switch (statement->type)
	{
		case STATEMENT_TYPE_EMPTY:
			break;

		case STATEMENT_TYPE_INSTRUCTION:
			ProcessInstruction(state, &statement->shared.instruction);
			break;

		case STATEMENT_TYPE_DC:
			ProcessDc(state, &statement->shared.dc);
			break;

		case STATEMENT_TYPE_DCB:
			ProcessDcb(state, &statement->shared.dcb);
			break;

		case STATEMENT_TYPE_INCLUDE:
			ProcessInclude(state, &statement->shared.include);
			break;

		case STATEMENT_TYPE_INCBIN:
			ProcessIncbin(state, &statement->shared.incbin);
			break;

		case STATEMENT_TYPE_REPT:
			ProcessRept(state, &statement->shared.rept);
			break;

		case STATEMENT_TYPE_ENDR:
			/* Exit REPT mode. */
			if (state->mode != MODE_REPT)
				SemanticError(state, "This stray ENDR has no preceeding REPT.");
			else
				TerminateRept(state);

			break;

		case STATEMENT_TYPE_MACRO:
			ProcessMacro(state, &statement->shared.macro, label, cc_false);
			break;

		case STATEMENT_TYPE_MACROS:
			ProcessMacro(state, &statement->shared.macro, label, cc_true);
			break;

		case STATEMENT_TYPE_ENDM:
			/* Exit MACRO mode. */
			if (state->mode != MODE_MACRO)
				SemanticError(state, "This stray ENDM has no preceeding MACRO.");
			else
				TerminateMacro(state);

			break;

		case STATEMENT_TYPE_EQU:
		{
			unsigned long value;

			if (state->equ_set_descope_local_labels)
				SetLastGlobalLabel(state, label);

			if (ResolveExpression(state, &statement->shared.expression, &value, cc_true))
				AddIdentifierToSymbolTable(state, label, value, SYMBOL_CONSTANT);

			break;
		}

		case STATEMENT_TYPE_SET:
		{
			unsigned long value;

			if (state->equ_set_descope_local_labels)
				SetLastGlobalLabel(state, label);

			if (ResolveExpression(state, &statement->shared.expression, &value, cc_true))
				AddIdentifierToSymbolTable(state, label, value, SYMBOL_VARIABLE);

			break;
		}

		case STATEMENT_TYPE_IF:
			ProcessIf(state, &statement->shared.expression);
			break;

		case STATEMENT_TYPE_ELSEIF:
			if (state->current_if_level == 0)
			{
				SemanticError(state, "This stray ELSEIF has no preceeding IF.");
			}
			else
			{
				if (state->false_if_level == state->current_if_level || state->false_if_level == 0)
				{
					unsigned long value;

					if (!ResolveExpression(state, &statement->shared.expression, &value, cc_true))
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

			break;

		case STATEMENT_TYPE_ELSE:
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

			break;

		case STATEMENT_TYPE_ENDC:
			if (state->current_if_level == 0)
			{
				SemanticError(state, "This stray ENDC/ENDIF has no preceeding IF.");
			}
			else
			{
				if (state->false_if_level == state->current_if_level || state->false_if_level == 0)
				{
					state->true_already_found = cc_true;
					state->false_if_level = 0;
				}

				--state->current_if_level;
			}

			break;

		case STATEMENT_TYPE_WHILE:
			/* Enter WHILE mode. */
			state->mode = MODE_WHILE;

			state->shared.while_statement.expression = statement->shared.expression;
			state->shared.while_statement.source_line = DuplicateString(state, state->source_line);

			state->shared.while_statement.line_number = state->location->line_number;

			state->shared.while_statement.source_line_list.head = NULL;
			state->shared.while_statement.source_line_list.tail = NULL;

			/* Dummy-out the statement's expression, since the state now owns it, and we don't
			   want its sub-expressions to be automatically freed when the statement is freed. */
			statement->shared.expression.type = EXPRESSION_NUMBER;

			break;

		case STATEMENT_TYPE_ENDW:
			/* Exit WHILE mode. */
			if (state->mode != MODE_WHILE)
				SemanticError(state, "This stray ENDW has no preceeding WHILE.");
			else
				TerminateWhile(state);

			break;

		case STATEMENT_TYPE_EVEN:
			/* Pad to the nearest even address. */
			if (state->program_counter & 1)
			{
				++state->program_counter;
				OutputByte(state, 0);
			}

			break;

		case STATEMENT_TYPE_CNOP:
		{
			unsigned long offset;

			if (!ResolveExpression(state, &statement->shared.cnop.offset, &offset, cc_true))
			{
				SemanticError(state, "Offset must be evaluable on the first pass.");
			}
			else
			{
				unsigned long size_boundary;

				if (!ResolveExpression(state, &statement->shared.cnop.size_boundary, &size_boundary, cc_true))
				{
					SemanticError(state, "Size boundary must be evaluable on the first pass.");
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
						++state->program_counter;
					}
				}
			}

			break;
		}

		case STATEMENT_TYPE_INFORM:
		{
			unsigned long severity;

			if (statement->shared.inform.message == NULL)
			{
				/* Actually a 'FAIL'. */
				/* TODO: Halt assembly. */
				SemanticError(state, "Assembly Failed");
			}
			else
			{
				/* Definitely an 'INFORM'... */
				if (!ResolveExpression(state, &statement->shared.inform.severity, &severity, cc_true))
				{
					SemanticError(state, "Severity must be evaluable on the first pass.");
					severity = 0;
				}

				/* TODO - Everything... */
				switch (severity)
				{
					case 0:
						TextOutput_fprintf(state->error_callbacks, "INFORM: '%s'\n", statement->shared.inform.message);
						break;

					case 1:
						SemanticWarning(state, "INFORM: '%s'", statement->shared.inform.message);
						break;

					case 2:
					case 3: /* TODO: Halt assembly. */
						SemanticError(state, "INFORM: '%s'", statement->shared.inform.message);
						break;
				}
			}

			break;
		}

		case STATEMENT_TYPE_END:
			state->end = cc_true;
			break;

		case STATEMENT_TYPE_RS:
		{
			unsigned long length;

			if (!ResolveExpression(state, &statement->shared.rs.length, &length, cc_true))
			{
				SemanticError(state, "Length must be evaluable on the first pass.");
			}
			else
			{
				Dictionary_Entry* const rs = LookupSymbol(state, "__rs", sizeof("__rs") - 1);

				/* Add label to symbol table. */
				if (label != NULL)
					AddIdentifierToSymbolTable(state, label, rs->shared.unsigned_long, SYMBOL_CONSTANT);

				/* Advance '__rs' by the specified amount. */
				switch (statement->shared.rs.size)
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

				rs->shared.unsigned_long += length;
			}

			break;
		}

		case STATEMENT_TYPE_RSSET:
		{
			/* Set '__rs' to value. */
			unsigned long value;

			if (!ResolveExpression(state, &statement->shared.expression, &value, cc_true))
				SemanticError(state, "Value must be evaluable on the first pass.");
			else
				LookupSymbol(state, "__rs", sizeof("__rs") - 1)->shared.unsigned_long = value;

			break;
		}

		case STATEMENT_TYPE_RSRESET:
			/* Set '__rs' to 0. */
			LookupSymbol(state, "__rs", sizeof("__rs") - 1)->shared.unsigned_long = 0;
			break;

		case STATEMENT_TYPE_OBJ:
			if (state->obj_active)
			{
				SemanticError(state, "OBJ statements cannot be nested.");
			}
			else
			{
				unsigned long value;

				if (!ResolveExpression(state, &statement->shared.expression, &value, cc_true))
					SemanticError(state, "Value must be evaluable on the first pass.");
				else
					state->obj_delta = value - state->program_counter;

				state->obj_active = cc_true;
				state->program_counter += state->obj_delta;
			}

			break;

		case STATEMENT_TYPE_OBJEND:
			if (!state->obj_active)
			{
				SemanticError(state, "This stray OBJEND has no preceeding OBJ.");
			}
			else
			{
				state->program_counter -= state->obj_delta;
				state->obj_active = cc_false;
			}

			break;

		case STATEMENT_TYPE_ORG:
		{
			unsigned long value;

			if (!ResolveExpression(state, &statement->shared.expression, &value, cc_true))
			{
				SemanticError(state, "Value must be evaluable on the first pass.");
			}
			else
			{
				if (value < state->start_position)
				{
					SemanticError(state, "ORG cannot be used to seek before the start of the output file.");
				}
				else
				{
					/* If we're at the start of the file, then don't cause the org to insert padding. */
					/* This is a hidden feature of S.N. 68k (asm68k). */
					if (!state->output_written_to)
						state->start_position = value;
					else
						BinaryOutput_fseek(state, state->output_callbacks, value - state->start_position);

					state->program_counter = value;
				}
			}

			break;
		}
	}
}

static void ParseLine(SemanticState *state, const char *source_line, const char *label, const char *directive_and_operands)
{
	/* This is a normal assembly line. */
	YY_BUFFER_STATE buffer;
	int parse_result;
	Statement statement;

	/* Back these up, before they get a chance to be modified by ProcessStatement. */
	const unsigned long starting_program_counter = state->program_counter;
	const size_t starting_output_position = state->output_position;

	/* Parse the source line with Flex and Bison (Lex and Yacc). */
	buffer = m68kasm__scan_string(directive_and_operands, state->flex_state);
	parse_result = m68kasm_parse(state->flex_state, &statement);
	m68kasm__delete_buffer(buffer, state->flex_state);

	switch (parse_result)
	{
		case 2: /* Out of memory. */
			OutOfMemoryError(state);
			break;

		case 1: /* Parsing error. */
			/* An error message will have already been printed, so we don't need to do one here. */
			state->success = cc_false;
			break;

		case 0: /* No error. */
			state->fix_up_needed = cc_false;

			ProcessStatement(state, &statement, label);

			if (!state->fix_up_needed)
			{
				/* We're done with this statement: delete it. */
				DestroyStatement(&statement);
			}
			else
			{
				/* If the statement cannot currently be processed because of undefined symbols,
				   add it to the fix-up list so we can try again later. */
				FixUp *fix_up = (FixUp*)MallocAndHandleError(state, sizeof(FixUp));

				if (fix_up == NULL)
				{
					/* Might as well delete this, since there's no fix-up to take ownership of it. */
					DestroyStatement(&statement);
				}
				else
				{
					const Location *source_location = state->location;
					Location *destination_location = &fix_up->location;

					/* Backup the statement. */
					fix_up->statement = statement;

					/* Backup some state. */
					fix_up->program_counter = starting_program_counter;
					fix_up->output_position = starting_output_position;
					fix_up->last_global_label = state->last_global_label == NULL ? NULL : DuplicateString(state, state->last_global_label);
					fix_up->source_line = DuplicateString(state, source_line);
					fix_up->label = label == NULL ? NULL : DuplicateString(state, label);

					/* Clone the location. */
					*destination_location = *source_location;
					destination_location->file_path = DuplicateString(state, source_location->file_path);

					for (source_location = source_location->previous; source_location != NULL; source_location = source_location->previous)
					{
						destination_location->previous = (Location*)MallocAndHandleError(state, sizeof(Location));

						if (destination_location->previous == NULL)
						{
							break;
						}
						else
						{
							destination_location = destination_location->previous;
							*destination_location = *source_location;
							destination_location->file_path = DuplicateString(state, source_location->file_path);
						}
					}

					/* Add the new fix-up to the list. */
					if (state->fix_up_list_head == NULL)
						state->fix_up_list_head = fix_up;
					else
						state->fix_up_list_tail->next = fix_up;

					state->fix_up_list_tail = fix_up;

					fix_up->next = NULL;
				}

				state->fix_up_needed = cc_false;
			}

			break;
	}

}

static void AssembleLine(SemanticState *state, const char *source_line)
{
	size_t label_length;
	const char *source_line_pointer;
	char *label;
	size_t directive_length;

	++state->location->line_number;

	if (source_line[0] == '*')
	{
		/* This whole line is a comment. */
		return;
	}

	state->source_line = source_line;
	source_line_pointer = source_line;

	/* Despite the fact that we're using Flex and Bison to parse the
	   language for us, we unfortunately have to do quite a bit of
	   parsing manually because of inconsistent whitespace sensitivity
	   (label recognision) and manipulation of the input string
	   (macro expansion). */

#define DIRECTIVE_OR_MACRO_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?_"
#define LABEL_CHARS DIRECTIVE_OR_MACRO_CHARS ".@"

	/* Let's begin by parsing the label (if there is one). */

	/* Determine the length of the label. */
	label_length = strspn(source_line_pointer, LABEL_CHARS);

	if (label_length != 0)
	{
		/* We've found a label! */
	}
	else
	{
		/* Maybe the label has some whitespace before it: check again. */

		/* Skip whitespace. */
		source_line_pointer += strspn(source_line_pointer, " \t");

		/* Determine the length of the label again. */
		label_length = strspn(source_line_pointer, LABEL_CHARS);

		if (label_length != 0)
		{
			/* Since we've encountered whitespace, it's ambiguous whether this is a label or a statement.
			   To figure it out, we have to rely on the label having a ':' after it. */
			if (source_line_pointer[label_length] == ':')
			{
				/* We've found the label. */
			}
			else
			{
				/* Not a label. Set 'label_length' to 0 so that later code knows that no label was found. */
				label_length = 0;
			}
		}
	}

	if (label_length == 0)
	{
		/* "My disappointment is immeasurable, and my day is ruined." */
		label = NULL;
	}
	else
	{
		/* This line has a label; duplicate it for later. */
		label = DuplicateStringWithLength(state, source_line_pointer, label_length);

		/* Advance past the label in the source string. */
		source_line_pointer += label_length;

		/* Skip past the colon at the end, if one exists. */
		if (source_line_pointer[0] == ':')
			++source_line_pointer;
	}

	/* Skip the whitespace between the label and the directive. */
	source_line_pointer += strspn(source_line_pointer, " \t");

	/* Next let's process the directive. If it's an if-statement or a macro
	   invocation, then we'll need to handle it manually. If not, then we
	   can pass it to Flex and Bison. */

	/* Determine the length of the directive. */
	directive_length = strspn(source_line_pointer, DIRECTIVE_OR_MACRO_CHARS);

	switch (state->mode)
	{
		case MODE_NORMAL:
		{
			/* If we are in the false part of an if-statement, then manually parse the
			   source code until we encounter an IF, ELSEIF, ELSE, ENDC, or ENDIF.
			   The reason for this is that the false part of an if-statement may contain
			   invalid code and we do not want it to cause errors.
			   This can get pretty complicated because we need to account for nesting. */
			if (state->false_if_level != 0)
			{
				if (directive_length != 0)
				{
					/* TODO - Detect code after the keyword and error if any is found. */
					if (strncmpci(source_line_pointer, "if", directive_length) == 0)
					{
						/* If-statements that are nested within the false part of another if-statement
						   are themselves false, so create a false if-statement here and process it. */
						++state->current_if_level;
					}
					else if (strncmpci(source_line_pointer, "elseif", directive_length) == 0
					      || strncmpci(source_line_pointer, "else"  , directive_length) == 0
					      || strncmpci(source_line_pointer, "endc"  , directive_length) == 0
					      || strncmpci(source_line_pointer, "endif" , directive_length) == 0)
					{
						/* These can be processed normally too. */
						ParseLine(state, source_line, label, source_line_pointer);
					}
					else
					{
						/* Drop the line completely, since it's inside the false half of an if statement and should be ignored. */
					}
				}
			}
			else
			{
				/* This is either a directive, or a macro. */

				/* Look up the directive in the dictionary to see if it's actually a macro. */
				const Dictionary_Entry* const macro_dictionary_entry = LookupSymbol(state, source_line_pointer, directive_length);

				if (macro_dictionary_entry == NULL || macro_dictionary_entry->type != SYMBOL_MACRO)
				{
					/* This is not a macro invocation: it's just a regular line that can be assembled as-is. */
					ParseLine(state, source_line, label, source_line_pointer);
				}
				else
				{
					/* This is a macro invocation. */
					char **parameters;
					unsigned int total_parameters;
					char *narg_string, *symbol_value_string;

					Macro *macro = (Macro*)macro_dictionary_entry->shared.pointer;
					char* const unique_suffix = ComputeUniqueMacroSuffix(state, macro);

					source_line_pointer += strspn(source_line_pointer, DIRECTIVE_OR_MACRO_CHARS);
					parameters = (char**)MallocAndHandleError(state, sizeof(char*));
					total_parameters = 1;
					symbol_value_string = NULL;

					if (label != NULL)
					{
						SetLastGlobalLabel(state, label);

						if (!macro->uses_label)
							AddIdentifierToSymbolTable(state, label, state->program_counter, SYMBOL_LABEL);
					}

					/* Extract and store the macro size specifier, if one exists. */
					if (source_line_pointer[0] == '.')
					{
						size_t size_length;

						/* Skip the '.' character. */
						++source_line_pointer;

						size_length = strspn(source_line_pointer, DIRECTIVE_OR_MACRO_CHARS);
						parameters[0] = DuplicateStringWithLength(state, source_line_pointer, size_length);

						/* Advance past the size specifier. */
						source_line_pointer += size_length;
					}
					else
					{
						parameters[0] = NULL;
					}

					/* Extract and store the macro parameters, if they exist. */
					{
						const char* const parameter_string = source_line_pointer + strspn(source_line_pointer, " \t");

						char character;

						do
						{
							const char* const parameter_start = source_line_pointer += strspn(source_line_pointer, " \t");

							do
							{
								character = *source_line_pointer++;

								/* If commas appear between parentheses, then we shouldn't separate on them. */
								/* This is because of the possibility of code such as '(a0,d0.w)' being passed as a parameter. */
								/* To do this, manually skip characters until we find a closing parenthesis. */
								if (character == '(')
								{
									unsigned int parameter_depth = 1;

									while (parameter_depth != 0)
									{
										character = *source_line_pointer++;

										if (character == '(')
											++parameter_depth;
										else if (character == ')')
											--parameter_depth;
										else if (character == ';' || character == '\0')
											break;
									}
								}

								/* If we encounter a comma, a comment, or the end of the line, then split this off as a macro parameter. */
								if (character == ',' || character == ';' || character == '\0')
								{
									const size_t parameter_string_length = source_line_pointer - parameter_start - 1;

									if (parameter_string_length != 0)
									{
										char* const parameter_string = DuplicateStringWithLength(state, parameter_start, parameter_string_length);

										if (parameter_string != NULL)
										{
											char** const new_parameters = (char**)realloc(parameters, sizeof(char*) * (total_parameters + 1));

											if (new_parameters == NULL)
											{
												free(parameter_string);
												OutOfMemoryError(state);
											}
											else
											{
												parameters = new_parameters;
												parameters[total_parameters] = parameter_string;
												++total_parameters;
											}
										}
									}

									break;
								}
							} while (character != ';' && character != '\0');
						} while (character != ';' && character != '\0');

						/* Stringify the number of parameters for the 'narg' placeholder. */
						narg_string = DecimalIntegerToString(state, total_parameters - 1);

						/* Finally, invoke the macro. */
						{
							const SourceLineListNode *source_line_list_node;

							/* Push a new location (this macro).*/
							Location location;

							location.file_path = macro->name;
							location.line_number = 0;

							location.previous = state->location;
							state->location = &location;

							/* Iterate over each line of the macro, performing parameter substitution and then sending it to be processed. */
							for (source_line_list_node = macro->source_line_list_head; source_line_list_node != NULL; source_line_list_node = source_line_list_node->next)
							{
								const char *remaining_line;
								char *modified_line;

								/* Update the source line for the error printers. */
								state->source_line = source_line_list_node->source_line;

								/* A bit of a cheat so that errors that occur before the call to AssembleLine still show the correct line number. */
								++state->location->line_number;

								/* Replace the parameter placeholders with their proper contents. */
								remaining_line = modified_line = DuplicateString(state, source_line_list_node->source_line);

								if (modified_line != NULL)
								{
									for (;;)
									{
										const char *earliest_parameter_start;
										const char *earliest_parameter_end;
										const IdentifierListNode *parameter_name;
										unsigned long i;
										const char *found_parameter_start;
										const char *found_parameter_end;
										const char *substitute;

										/* Search for the earliest macro parameter placeholder in the line, storing its location in 'earliest_parameter_start'. */

										/* Silence bogus(?) 'variable may be used uninitialised' compiler warnings. */
										/* TODO: Are these really bogus? */
										substitute = NULL;
										earliest_parameter_end = NULL;

										/* Find numerical macro parameter placeholder. */
										earliest_parameter_start = strchr(remaining_line, '\\');

										if (earliest_parameter_start != NULL)
										{
											earliest_parameter_end = earliest_parameter_start + 2;

											if (earliest_parameter_start[1] == '*')
											{
												substitute = label;
											}
											else if (earliest_parameter_start[1] == '@')
											{
												substitute = unique_suffix;
											}
											else if (earliest_parameter_start[1] == '_')
											{
												substitute = parameter_string;
											}
											else if (earliest_parameter_start[1] == '#' || earliest_parameter_start[1] == '$')
											{
												unsigned long value;

												const char* const identifier = earliest_parameter_start + 2;
												const size_t identifier_length = strspn(identifier, DIRECTIVE_OR_MACRO_CHARS);

												earliest_parameter_end = identifier + identifier_length;
												/* Absorb trailing backslash. */
												if (identifier[identifier_length] == '\\')
													++earliest_parameter_end;

												free(symbol_value_string);

												if (!GetSymbolInteger(state, identifier, identifier_length, cc_true, &value))
													value = 0;

												if (earliest_parameter_start[1] == '#')
													symbol_value_string = DecimalIntegerToString(state, value);
												else /*if (earliest_parameter_start[1] == '$')*/
													symbol_value_string = HexadecimalIntegerToString(state, value);

												substitute = symbol_value_string;
											}
											else
											{
												char *end;

												/* Obtain numerical index of the parameter. */
												const unsigned long parameter_index = strtoul(earliest_parameter_start + 1, &end, 10);
												earliest_parameter_end = end;

												/* Check if conversion failed. */
												if (end == earliest_parameter_start + 1)
													earliest_parameter_start = NULL;
												else
													substitute = (parameter_index < total_parameters && parameters[parameter_index] != NULL) ? parameters[parameter_index] : "";
											}
										}

										/* Now find the identifier-based macro parameter placeholders. */
										for (parameter_name = macro->parameter_names, i = 1; parameter_name != NULL; parameter_name = parameter_name->next, ++i)
										{
											if (FindMacroParameter(remaining_line, parameter_name->identifier, &found_parameter_start, &found_parameter_end))
											{
												if (earliest_parameter_start == NULL || found_parameter_start < earliest_parameter_start)
												{
													substitute = (i < total_parameters && parameters[i] != NULL) ? parameters[i] : "";
													earliest_parameter_start = found_parameter_start;
													earliest_parameter_end = found_parameter_end;
												}
											}
										}

										/* Find the 'narg' placeholder, which represents how many parameters (arguments) have been passed to the macro. */
										if (FindMacroParameter(remaining_line, "narg", &found_parameter_start, &found_parameter_end))
										{
											if (earliest_parameter_start == NULL || found_parameter_start < earliest_parameter_start)
											{
												substitute = narg_string;
												earliest_parameter_start = found_parameter_start;
												earliest_parameter_end = found_parameter_end;
											}
										}

										/* If no placeholders can be found, then we are done here. */
										if (earliest_parameter_start == NULL)
											break;

										/* Split the line in two, and insert the parameter between them. */
										{
											char *new_modified_line;

											const char* const parameter = substitute == NULL ? "" : substitute;
											const size_t first_half_length = earliest_parameter_start - modified_line;
											const size_t parameter_length = strlen(parameter);
											const size_t second_half_length = strlen(earliest_parameter_end);

											new_modified_line = (char*)MallocAndHandleError(state, first_half_length + parameter_length + second_half_length + 1);

											if (new_modified_line != NULL)
											{
												memcpy(new_modified_line, modified_line, first_half_length);
												memcpy(new_modified_line + first_half_length, parameter, parameter_length);
												memcpy(new_modified_line + first_half_length + parameter_length, earliest_parameter_end, second_half_length);
												new_modified_line[first_half_length + parameter_length + second_half_length] = '\0';

												/* Continue our search from after the inserted parameter. */
												remaining_line = &new_modified_line[first_half_length + parameter_length];

												/* We don't need the old copy of the line anymore: free it, and replace it with the new copy. */
												free(modified_line);
												modified_line = new_modified_line;
											}
										}
									}
								}

								/* Undo our hack from before. */
								--state->location->line_number;

								/* Send our expanded macro line to be assembled. */
								AssembleLine(state, modified_line != NULL ? modified_line : source_line_list_node->source_line);

								/* The expanded line is done, so we can free it now. */
								free(modified_line);
							}

							/* Pop location. */
							state->location = state->location->previous;
						}

						/* Free the parameter strings. */
						{
							size_t i;

							for (i = 0; i < total_parameters; ++i)
								free(parameters[i]);

							free(parameters);
						}

						/* Free other stuff. */
						free(narg_string);
						free(unique_suffix);
						free(symbol_value_string);
					}
				}
			}

			break;
		}

		case MODE_REPT:
			/* If this line is an 'ENDR' directive, then exit REPT mode. Otherwise, add the line to the REPT. */
			if (directive_length != 0 && (strncmpci(source_line_pointer, "rept", directive_length) == 0 || strncmpci(source_line_pointer, "endr", directive_length) == 0))
			{
				/* TODO - Detect code after the keyword and error if any is found. */
				ParseLine(state, source_line, label, source_line_pointer);
			}
			else
			{
				AddToSourceLineList(state, &state->shared.rept.source_line_list, source_line);
			}

			break;

		case MODE_MACRO:
			/* If this line is an 'ENDM' directive, then exit macro mode. Otherwise, add the line to the macro. */
			if (directive_length != 0 && strncmpci(source_line_pointer, "endm", directive_length) == 0)
			{
				/* TODO - Detect code after the keyword and error if any is found. */
				ParseLine(state, source_line, label, source_line_pointer);

				if (state->shared.macro.is_short)
					SemanticError(state, "Short macros shouldn't use ENDM.");
			}
			else
			{
				if (state->shared.macro.is_short)
				{
					/* Short macros automatically terminate after one statement. */
					const char first_nonwhitespace_character = source_line[strspn(source_line, " \t")];

					if (label != NULL)
						SemanticError(state, "Short macros shouldn't create labels.");

					if (first_nonwhitespace_character == '\0' || first_nonwhitespace_character == ';')
					{
						/* This line is an empty statement: ignore it. */
					}
					else
					{
						AddToSourceLineList(state, &state->shared.macro.source_line_list, source_line);
						TerminateMacro(state);
					}
				}
				else
				{
					AddToSourceLineList(state, &state->shared.macro.source_line_list, source_line);
				}
			}

			break;

		case MODE_WHILE:
			/* If this line is an 'ENDW' directive, then exit 'WHILE' mode. Otherwise, add the line to the 'WHILE'. */
			if (directive_length != 0 && strncmpci(source_line_pointer, "endw", directive_length) == 0)
				/* TODO - Detect code after the keyword and error if any is found. */
				ParseLine(state, source_line, label, source_line_pointer);
			else
				AddToSourceLineList(state, &state->shared.while_statement.source_line_list, source_line);

			break;
	}

	free(label);
}

static void AssembleFile(SemanticState *state)
{
	while (!state->end && ReadSourceLine(state))
		AssembleAndListLine(state);

	/* If we're not in normal mode when a file ends, then something is wrong. */
	switch (state->mode)
	{
		case MODE_NORMAL:
			/* All okay. */
			break;

		case MODE_REPT:
			/* Terminate the REPT to hopefully avoid future complications. */
			TerminateRept(state);

			SemanticError(state, "REPT statement beginning at line %lu is missing its ENDR.", state->shared.rept.line_number);
			break;

		case MODE_MACRO:
			/* Terminate the macro to hopefully avoid future complications. */
			TerminateMacro(state);

			SemanticError(state, "MACRO statement beginning at line %lu is missing its ENDM.", state->shared.macro.line_number);
			break;

		case MODE_WHILE:
			/* Terminate the while-statement to hopefully avoid future complications. */
			TerminateWhile(state);

			SemanticError(state, "WHILE statement beginning at line %lu is missing its ENDW.", state->shared.while_statement.line_number);
			break;
	}
}

static cc_bool DictionaryFilterDeleteVariables(Dictionary_Entry *entry, const char *identifier, size_t identifier_length, void *user_data)
{
	(void)user_data;

	return (entry->type != SYMBOL_VARIABLE
	    || (identifier_length == sizeof(PROGRAM_COUNTER_OF_STATEMENT) - 1 && memcmp(identifier, PROGRAM_COUNTER_OF_STATEMENT, identifier_length) == 0)
	    || (identifier_length == sizeof(PROGRAM_COUNTER_OF_EXPRESSION) - 1 && memcmp(identifier, PROGRAM_COUNTER_OF_EXPRESSION, identifier_length) == 0)
	    || (identifier_length == sizeof("__rs") - 1 && memcmp(identifier, "__rs", identifier_length) == 0));
}

static cc_bool DictionaryFilterProduceSymbolFile(Dictionary_Entry *entry, const char *identifier, size_t identifier_length, void *user_data)
{
	/* The symbol file only contains labels. */
	/* Labels longer than 0xFF characters are silently ignored. */
	/* Local labels are also silently ignored (unless 'v+' is specified). */
	void** const parameters = (void**)user_data;
	const cc_bool output_local_labels_to_sym_file = *(cc_bool*)parameters[1];

	if (entry->type == SYMBOL_LABEL)
	{
		const char *label;
		size_t label_length;
		cc_bool is_local_label;

		for (label = &identifier[identifier_length], label_length = 0; label_length != identifier_length; --label, ++label_length)
			if (*label == '@' || *label == '.')
				break;

		is_local_label = label_length != identifier_length;

		if (label_length < 0x100 && (!is_local_label || output_local_labels_to_sym_file))
		{
			unsigned int i;

			const BinaryOutput* const symbol_callbacks = (const BinaryOutput*)parameters[0];

			/* Output the address of the label. */
			for (i = 0; i < 4; ++i)
				BinaryOutput_fputc((entry->shared.unsigned_long >> (i * 8)) & 0xFF, symbol_callbacks);

			/* I have no idea what this means. */
			BinaryOutput_fputc(is_local_label ? 6 : 2, symbol_callbacks);

			/* Output the length of the label. */
			BinaryOutput_fputc(label_length, symbol_callbacks);

			/* Output the label itself. */
			BinaryOutput_fwrite(label, 1, label_length, symbol_callbacks);
		}
	}

	return cc_true;
}

static cc_bool InitialiseBuiltInVariable(SemanticState* const state, const char* const identifier, const size_t identifier_length)
{
	Dictionary_Entry* const symbol = CreateSymbol(state, identifier, identifier_length);

	if (symbol != NULL)
		symbol->type = SYMBOL_VARIABLE;

	return symbol != NULL;
}

static void AddDefinition(void* const internal, const char* const identifier, const size_t identifier_length, const unsigned long value)
{
	SemanticState* const state = (SemanticState*)internal;
	Dictionary_Entry* const symbol = CreateSymbol(state, identifier, identifier_length);

	if (symbol != NULL)
	{
		/* TODO: Does asm68k create a constant or a variable? */
		symbol->type = SYMBOL_CONSTANT;
		symbol->shared.unsigned_long = value;
	}
}

cc_bool ClownAssembler_Assemble(
	const TextInput* const input_callbacks,
	const BinaryOutput* const output_callbacks,
	const TextOutput* const error_callbacks,
	const TextOutput* const listing_callbacks,
	const BinaryOutput* const symbol_callbacks,
	const char* const input_file_path,
	const cc_bool debug,
	const cc_bool case_insensitive,
	const cc_bool equ_set_descope_local_labels,
	const cc_bool output_local_labels_to_sym_file,
	const cc_bool warnings_enabled,
	void (* const definition_callback)(void *internal, void *user_data, ClownAssembler_AddDefinition add_definition),
	const void* const user_data)
{
	Location location;
	SemanticState state = {0}; /* Default-initialise everything. */

	/* For error messages, we want to maintain a list of filenames and line
	   numbers that describe the location and nesting of the erroneous
	   line. This is the first entry in that list. */
	location.previous = NULL;
	location.file_path = NULL;
	location.line_number = 0;

	/* Initialise the state's non-default values. */
	state.success = cc_true;
	state.input_callbacks = input_callbacks;
	state.output_callbacks = output_callbacks;
	state.error_callbacks = error_callbacks;
	state.listing_callbacks = listing_callbacks;
	state.equ_set_descope_local_labels = equ_set_descope_local_labels;
	state.warnings_enabled = warnings_enabled;
	state.location = &location;
	state.mode = MODE_NORMAL;

	/* Set the location path (note that we're taking care to not do this
	   before the state is fully initialised, as the error handler requires
	   valid state). */
	location.file_path = DuplicateString(&state, input_file_path);

	/* Create the symbol table dictionary. */
	if (!Dictionary_Init(&state.dictionary, case_insensitive))
	{
		OutOfMemoryError(&state);
	}
	else
	{
#define INITIALISE_BUILT_IN_VARIABLE(identifier) InitialiseBuiltInVariable(&state, identifier, sizeof(identifier) - 1)
		/* Create the dictionary entry for the program counter ahead of time.
		   Bizarrely, depending on the context, the program counter can be one
		   of two values, so we need to maintain two separate copies of it. */
		if (INITIALISE_BUILT_IN_VARIABLE(PROGRAM_COUNTER_OF_STATEMENT)
		 && INITIALISE_BUILT_IN_VARIABLE(PROGRAM_COUNTER_OF_EXPRESSION)
		 /* Create the secret '__rs' symbol too. */
		 && INITIALISE_BUILT_IN_VARIABLE("__rs"))
		{
			if (definition_callback != NULL)
				definition_callback(&state, (void*)user_data, AddDefinition);

			/* Initialise the lexer. */
			if (m68kasm_lex_init_extra(&state, &state.flex_state) != 0)
			{
				InternalError(&state, "m68kasm_lex_init failed.");
			}
			else
			{
				/* Enable Bison debugging if available and requested by the user. */
			#if M68KASM_DEBUG
				if (debug)
					m68kasm_debug = 1;
			#else
				(void)debug;
			#endif

				/* Perform first pass of assembly, creating a list of fix-ups. */
				AssembleFile(&state);

				/* Destroy the lexer, as we no longer need it. */
				if (m68kasm_lex_destroy(state.flex_state) != 0)
					InternalError(&state, "m68kasm_lex_destroy failed.");

				/* Perform some sanity checks to make sure we're not somehow in an invalid state. */
				if (state.current_if_level != 0)
					SemanticError(&state, "An IF statement somewhere is missing its ENDC/ENDIF.");
				if (state.obj_active)
					SemanticError(&state, "An OBJ statement somewhere is missing its OBJEND.");

				/* We're about to process the fix-ups, but first we need to remove all variables
				   from the symbol table so that they're not able to be used before they are declared. */
				Dictionary_Filter(&state.dictionary, DictionaryFilterDeleteVariables, NULL);

				/* Process the fix-ups: reassemble instructions and reprocess
				   directives that could not be completed in the first pass. */
				state.doing_fix_up = cc_true;

				for (;;)
				{
					cc_bool a_fix_up_has_been_fixed;
					FixUp **fix_up_pointer;

					a_fix_up_has_been_fixed = cc_false;

					fix_up_pointer = &state.fix_up_list_head;

					while (*fix_up_pointer != NULL)
					{
						FixUp *fix_up = *fix_up_pointer;

						/* Reset some state to how it was at the time the statement was first processed. */
						state.program_counter = fix_up->program_counter;
						BinaryOutput_fseek(&state, state.output_callbacks, fix_up->output_position);
						state.last_global_label = fix_up->last_global_label == NULL ? NULL : DuplicateString(&state, fix_up->last_global_label);
						state.source_line = fix_up->source_line;
						state.location = &fix_up->location;

						state.fix_up_needed = cc_false;

						/* Re-process statement. */
						ProcessStatement(&state, &fix_up->statement, fix_up->label);

						/* If this fix-up has been fixed, then we are done with it and free to delete it.
						   Alternatively, if this is the final pass, then just delete the fix-ups anyway
						   since they won't be needed anymore. */
						if (!state.fix_up_needed || state.doing_final_pass)
						{
							Location *location;

							*fix_up_pointer = fix_up->next;

							/* We're done with this statement: delete it. */
							DestroyStatement(&fix_up->statement);
							free(fix_up->last_global_label);
							free(fix_up->source_line);
							free(fix_up->label);

							/* Pop one location from the list. */
							location = fix_up->location.previous;

							while (location != NULL)
							{
								Location *previous_location = location->previous;
								free(location->file_path);
								free(location);
								location = previous_location;
							}

							free(fix_up);

							a_fix_up_has_been_fixed = cc_true;
						}
						else
						{
							fix_up_pointer = &fix_up->next;
						}
					}

					/* Once the final pass has ended, we can exit this infinite loop. */
					if (state.doing_final_pass)
						break;

					/* If no more fix-ups can be fixed, then something has gone wrong
					   and we should do one final pass to print error messages. */
					if (!a_fix_up_has_been_fixed)
						state.doing_final_pass = cc_true;
				}

				free(state.last_global_label);

				/* Produce asm68k symbol file, if requested. */
				if (BinaryOutput_exists(symbol_callbacks))
				{
					const void *parameters[2];

					parameters[0] = symbol_callbacks;
					parameters[1] = &output_local_labels_to_sym_file;

					/* Some kind of header. */
					BinaryOutput_fputc('M', symbol_callbacks);
					BinaryOutput_fputc('N', symbol_callbacks);
					BinaryOutput_fputc('D', symbol_callbacks);
					BinaryOutput_fputc(1, symbol_callbacks);
					BinaryOutput_fputc(0, symbol_callbacks);
					BinaryOutput_fputc(0, symbol_callbacks);
					BinaryOutput_fputc(0, symbol_callbacks);
					BinaryOutput_fputc(0, symbol_callbacks);

					Dictionary_Filter(&state.dictionary, DictionaryFilterProduceSymbolFile, parameters);
				}
			}
		}

		Dictionary_Deinit(&state.dictionary);
	}

	free(location.file_path);

	return state.success;
}

cc_bool ClownAssembler_AssembleFile(
	FILE* const input_file,
	FILE* const output_file,
	FILE* const error_file,
	FILE* const listing_file,
	FILE* const symbol_file,
	const char* const input_file_path,
	const cc_bool debug,
	const cc_bool case_insensitive,
	const cc_bool equ_set_descope_local_labels,
	const cc_bool output_local_labels_to_sym_file,
	const cc_bool warnings_enabled,
	void (* const definition_callback)(void *internal, void *user_data, ClownAssembler_AddDefinition add_definition),
	const void* const user_data)
{
	ClownAssembler_TextInput input_callbacks;
	ClownAssembler_BinaryOutput output_callbacks;
	ClownAssembler_TextOutput error_callbacks;
	ClownAssembler_TextOutput listing_callbacks;
	ClownAssembler_BinaryOutput symbol_callbacks;

	input_callbacks.user_data = input_file;
	input_callbacks.read_character = ReadCharacter;
	input_callbacks.read_line = ReadLine;

	output_callbacks.user_data = output_file;
	output_callbacks.write_character = WriteCharacter;
	output_callbacks.write_characters = WriteCharacters;
	output_callbacks.seek = Seek;

	error_callbacks.user_data = error_file;
	error_callbacks.print_formatted = PrintFormatted;
	error_callbacks.write_character = WriteCharacter;
	error_callbacks.write_string = WriteString;

	listing_callbacks.user_data = listing_file;
	listing_callbacks.print_formatted = PrintFormatted;
	listing_callbacks.write_character = WriteCharacter;
	listing_callbacks.write_string = WriteString;

	symbol_callbacks.user_data = symbol_file;
	symbol_callbacks.write_character = WriteCharacter;
	symbol_callbacks.write_characters = WriteCharacters;
	symbol_callbacks.seek = Seek;

	return ClownAssembler_Assemble(&input_callbacks, &output_callbacks, &error_callbacks, &listing_callbacks, &symbol_callbacks, input_file_path, debug, case_insensitive, equ_set_descope_local_labels, output_local_labels_to_sym_file, warnings_enabled, definition_callback, user_data);
}
