/*
 * Copyright (C) 2022-2025 Clownacy
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

/* TODO: Optimise EQUS by having the substitution logic search for the '\' rather than the string itself. */

#include "semantic.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clowncommon/clowncommon.h"

#include "dictionary.h"
#include "p2bin.h"
#include "shared-memory.h"
#include "string.h"
#include "string-stack.h"
#include "substitute.h"
#include "syntactic.h"
#define YY_NO_UNISTD_H
#include "lexical.h"

typedef ClownAssembler_TextInput TextInput;
typedef ClownAssembler_BinaryOutput BinaryOutput;
typedef ClownAssembler_TextOutput TextOutput;

typedef enum SymbolType
{
	SYMBOL_CONSTANT,
	SYMBOL_VARIABLE,
	SYMBOL_MACRO,
	SYMBOL_LABEL,
	SYMBOL_SPECIAL,
	SYMBOL_STRING_CONSTANT
} SymbolType;

typedef struct Location
{
	struct Location *previous;

	String file_path; /* TODO: This should really be a `StringView`, but fix-ups need deep copies... */
	unsigned long line_number;
} Location;

typedef struct FixUp
{
	struct FixUp *next;

	Statement statement;
	Dictionary_State *macro_dictionary;
	unsigned long program_counter;
	size_t output_position;
	size_t segment_length;
	String last_global_label;
	String source_line;
	Location location;
	String label;
	Options options;
} FixUp;

typedef struct SourceLineListNode
{
	struct SourceLineListNode *next;

	String source_line_buffer;
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

typedef struct Macro
{
	cc_bool is_short, uses_label;
	String name;
	IdentifierListNode *parameter_names;
	SourceLineListNode *source_line_list_head;
} Macro;

typedef struct SemanticState_Macro
{
	Macro *metadata;
	Dictionary_State *dictionary;
	Substitute_State substitutions;
	struct MacroCustomSubstituteSearch_Closure *closure;
	String *argument_list;
	size_t total_arguments, total_arguments_at_start;
	unsigned int starting_if_level;
	cc_bool active;
} SemanticState_Macro;

typedef struct SemanticState
{
	cc_bool success;

	/* Callbacks. */
	const TextInput *input_callbacks;
	const BinaryOutput *output_callbacks;
	const TextOutput *listing_callbacks;
	const TextOutput *error_callbacks;

	/* Global stuff. */
	Dictionary_State dictionary;
	String last_global_label;
	Location *location;
	yyscan_t flex_state;
	String line_buffer;
	const String *source_line;
	Substitute_State substitutions;
	StringStack_State string_stack;
	Options_State options;
	SemanticState_Macro macro;
	size_t output_position, previous_segment_length_output_position;
	size_t segment_position, segment_length;
	unsigned long program_counter, program_counter_of_statement, program_counter_of_expression;
	unsigned long current_macro_invocation;
	unsigned long rs;
	cc_bool end;

	/* Fix-ups. */
	FixUp *fix_up_list_head;
	FixUp *fix_up_list_tail;
	cc_bool fix_up_needed;
	cc_bool doing_fix_up;
	cc_bool doing_final_pass;

	/* If-statements. */
	unsigned int current_if_level;
	unsigned int false_if_level;
	cc_bool true_already_found;
	cc_bool evaluating_if_condition; /* Flag to suppress errors for undefined symbols in if conditions */

	/* OBJ directive. */
	cc_bool obj_active;
	unsigned long obj_delta;

	/* Listing. */
	unsigned int listing_counter;
	cc_bool line_listed;
	cc_bool suppress_listing;
	cc_bool listing_macro;

	/* Modes. */
	Mode mode;
	union
	{
		struct
		{
			unsigned long repetitions;
			unsigned long line_number;
			unsigned int nesting;
			SourceLineList source_line_list;
		} rept;
		struct
		{
			String name;
			unsigned long line_number;
			IdentifierList parameter_names;
			SourceLineList source_line_list;
			cc_bool is_short, uses_label;
		} macro;
		struct
		{
			Expression expression;
			String source_line;
			unsigned long line_number;
			unsigned int nesting;
			SourceLineList source_line_list;
		} while_statement;
	} shared;
} SemanticState;

/* Some forward declarations that are needed because some functions recurse into each other. */
static void AssembleFile(SemanticState *state);
static void AssembleLine(SemanticState *state, const String *source_line, const cc_bool write_line_to_listing_file);

static const StringView string_rs = STRING_VIEW_INITIALISER("__rs");
static const StringView string_narg = STRING_VIEW_INITIALISER("narg");
static const StringView string_assembler_identifier = STRING_VIEW_INITIALISER(CLOWNASSEMBLER_VERSION_STRING);

static void OutputWriteRawByte(SemanticState* const state, const unsigned char byte)
{
	++state->output_position;
	BinaryOutput_fputc(byte, state->output_callbacks);
}

static void OutputSeek(SemanticState* const state, const size_t position)
{
	state->output_position = position;
	BinaryOutput_fseek(state->output_callbacks, position);
}

static void TerminatePreviousRecordSegment(SemanticState *state)
{
	if (state->previous_segment_length_output_position != 0)
	{
		const size_t previous_output_position = state->output_position;

		/* Write length of previous record. */
		OutputSeek(state, state->previous_segment_length_output_position);
		OutputWriteRawByte(state, state->segment_length >> (8 * 0) & 0xFF);
		OutputWriteRawByte(state, state->segment_length >> (8 * 1) & 0xFF);
		OutputSeek(state, previous_output_position);
	}
}

/* TODO: Don't write empty segments! */
static void WriteRecordSegment(SemanticState *state)
{
	TerminatePreviousRecordSegment(state);

	/* CPU architecture (68000). */
	OutputWriteRawByte(state, 0x01);
	/* Start address. */
	OutputWriteRawByte(state, state->segment_position >> (8 * 0) & 0xFF);
	OutputWriteRawByte(state, state->segment_position >> (8 * 1) & 0xFF);
	OutputWriteRawByte(state, state->segment_position >> (8 * 2) & 0xFF);
	OutputWriteRawByte(state, state->segment_position >> (8 * 3) & 0xFF);
	/* Length (placeholder). */
	state->previous_segment_length_output_position = state->output_position;
	OutputWriteRawByte(state, 0);
	OutputWriteRawByte(state, 0);
}

static void OutputWriteSegmentByte(SemanticState* const state, const unsigned char byte)
{
	if (state->segment_length == 0xFFFF)
	{
		state->segment_position += state->segment_length;

		/* Avoid rewriting the record header when doing fix-ups,
		   as the length field will be left blank. */
		if (state->doing_fix_up)
			OutputSeek(state, state->output_position + 1 + 4 + 2);
		else
			WriteRecordSegment(state);

		state->segment_length = 0;
	}

	++state->segment_length;

	OutputWriteRawByte(state, byte);
}

/* Default FILE-based IO callbacks */

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
		TextOutput_fprintf(state->error_callbacks, "\nOn line %lu of '%s'...", location->line_number, String_CStr(&location->file_path));

	TextOutput_fprintf(state->error_callbacks, "\n%s\n\n", state->source_line == NULL ? "[No source line]" : String_CStr(state->source_line));
}

CC_ATTRIBUTE_PRINTF(2, 3) static void SemanticWarning(SemanticState *state, const char *fmt, ...)
{
	if (Options_Get(&state->options)->warnings_enabled)
	{
		va_list args;

		TextOutput_fputs("Warning: ", state->error_callbacks);

		va_start(args, fmt);
		TextOutput_vfprintf(state->error_callbacks, fmt, args);
		va_end(args);

		ErrorMessageCommon(state);
	}
}

CC_ATTRIBUTE_PRINTF(2, 3) static void SemanticError(SemanticState *state, const char *fmt, ...)
{
	va_list args;

	TextOutput_fputs("Error: ", state->error_callbacks);

	va_start(args, fmt);
	TextOutput_vfprintf(state->error_callbacks, fmt, args);
	va_end(args);

	ErrorMessageCommon(state);

	state->success = cc_false;
}

CC_ATTRIBUTE_PRINTF(2, 3) static void InternalError(SemanticState *state, const char *fmt, ...)
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

	(void)statement;

	if (Options_Get(&state->options)->warnings_enabled)
	{
		TextOutput_fprintf(state->error_callbacks, "Warning: %s", message);

		ErrorMessageCommon(state);
	}
}

void m68kasm_warning_pedantic(void *scanner, Statement *statement, const char *message)
{
	SemanticState *state = (SemanticState*)m68kasm_get_extra(scanner);

	if (Options_Get(&state->options)->pedantic_warnings_enabled)
		m68kasm_warning(scanner, statement, message);
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

static unsigned int GetDecimalIntegerStringLength(unsigned long integer)
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

static unsigned int GetHexadecimalIntegerStringLength(unsigned long integer)
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

static String DecimalIntegerToString(const unsigned long integer)
{
	String string;

	const size_t length = GetDecimalIntegerStringLength(integer);

	String_CreateBlank(&string);
	if (String_ResizeNoFill(&string, length))
		sprintf(String_Data(&string), "%lu", integer);

	return string;
}

static String HexadecimalIntegerToString(const unsigned long integer)
{
	String string;

	const size_t length = GetHexadecimalIntegerStringLength(integer);

	String_CreateBlank(&string);
	if (String_ResizeNoFill(&string, length))
		sprintf(String_Data(&string), "%lX", integer);

	return string;
}

static String ComputeUniqueMacroSuffix(const SemanticState* const state)
{
	String string;
	const unsigned long integer = state->current_macro_invocation;

	const size_t length = GetDecimalIntegerStringLength(integer);

	String_CreateBlank(&string);
	if (String_ResizeNoFill(&string, 1 + length))
		sprintf(String_Data(&string), "_%lu", integer);

	return string;
}

static FILE* fopen_backslash(const StringView *path, const char *mode)
{
	FILE *file;
	String path_copy;

	if (!String_CreateCopyView(&path_copy, path))
	{
		file = NULL;
	}
	else
	{
		size_t i;

		for (i = 0; i < StringView_Length(path); ++i)
			if (String_At(&path_copy, i) == '\\')
				String_At(&path_copy, i) = '/';

		file = fopen(String_CStr(&path_copy), mode);

		String_Destroy(&path_copy);
	}

	return file;
}

static void ListSourceLine(SemanticState *state)
{
	/* Output line to listing file. */
	if (TextOutput_exists(state->listing_callbacks))
	{
		if (!state->line_listed && state->source_line != NULL)
		{
			unsigned int i;

			state->line_listed = cc_true;

			for (i = state->listing_counter; i < 26; ++i)
				TextOutput_fputc(' ', state->listing_callbacks);

			TextOutput_fputc(state->listing_macro ? 'M' : ' ', state->listing_callbacks);

			TextOutput_fprintf(state->listing_callbacks, " %s\n", String_CStr(state->source_line));
		}
	}
}

static void ListIdentifierValue(SemanticState *state, unsigned long value)
{
	/* Output line to listing file. */
	if (TextOutput_exists(state->listing_callbacks) && !state->doing_fix_up && !state->suppress_listing)
	{
		TextOutput_fprintf(state->listing_callbacks, " =%08lX", value);

		state->listing_counter += 2 + 8;
	}
}

/* TODO: Duplicate this and use function pointers. */
static void OutputByte(SemanticState *state, unsigned int byte)
{
	/* Write to listing file. */
	if (TextOutput_exists(state->listing_callbacks) && !state->doing_fix_up && !state->suppress_listing)
	{
		/* We can only write up to 10 bytes. */
		if (state->listing_counter <= 25)
		{
			if (state->listing_counter == 25)
			{
				/* After the last byte, we output a '+' to signal there's more. */
				TextOutput_fputc('+', state->listing_callbacks);
				++state->listing_counter;
			}
			else
			{
				if (state->listing_counter % 5 == 0)
				{
					TextOutput_fputc(' ', state->listing_callbacks);
					++state->listing_counter;
				}

				TextOutput_fprintf(state->listing_callbacks, "%02X", byte);
				state->listing_counter += 2;
			}
		}
	}

	/* Write to output file. */
	OutputWriteSegmentByte(state, byte);
}

static cc_bool CurrentlyExpandingMacro(const SemanticState* const state)
{
	return state->macro.active;
}

static void ExpandIdentifier(SemanticState *state, String* const expanded_identifier, const StringView* const identifier)
{
	if (StringView_Empty(identifier) || (StringView_At(identifier, 0) != Options_Get(&state->options)->local_signifier))
		String_CreateBlank(expanded_identifier);
	else if (String_Empty(&state->last_global_label))
		String_CreateInternal(expanded_identifier, StringView_Data(identifier) + 1, StringView_Length(identifier) - 1, NULL, 0);
	else
		String_CreateAppendView(expanded_identifier, String_View(&state->last_global_label), identifier);
}

static Dictionary_Entry* LookupSymbol(SemanticState *state, const StringView *identifier, Dictionary_State **dictionary)
{
	Dictionary_Entry *dictionary_entry = NULL;
	String expanded_identifier;

	/* TODO: Avoid this allocation entirely by hashing each half of the identifier separately. */
	ExpandIdentifier(state, &expanded_identifier, identifier);

	if (!String_Empty(&expanded_identifier))
		identifier = String_View(&expanded_identifier);

	if (dictionary != NULL)
		*dictionary = state->macro.dictionary;

	if (state->macro.dictionary != NULL)
		dictionary_entry = Dictionary_LookUp(state->macro.dictionary, identifier);

	if (dictionary_entry == NULL)
	{
		if (dictionary != NULL)
			*dictionary = &state->dictionary;

		dictionary_entry = Dictionary_LookUp(&state->dictionary, identifier);

		if (dictionary_entry == NULL)
			if (dictionary != NULL)
				*dictionary = NULL;
	}

	String_Destroy(&expanded_identifier);

	return dictionary_entry;
}

static Dictionary_Entry* LookupSymbolAndCreateIfNotExist(SemanticState *state, const StringView *identifier, Dictionary_State **dictionary)
{
	Dictionary_Entry *dictionary_entry = NULL;
	String expanded_identifier;

	/* TODO: Avoid this allocation entirely by hashing each half of the identifier separately. */
	ExpandIdentifier(state, &expanded_identifier, identifier);

	if (!String_Empty(&expanded_identifier))
		identifier = String_View(&expanded_identifier);

	if (dictionary != NULL)
		*dictionary = state->macro.dictionary;

	if (state->macro.dictionary != NULL)
		dictionary_entry = Dictionary_LookUp(state->macro.dictionary, identifier);

	if (dictionary_entry == NULL)
	{
		if (dictionary != NULL)
			*dictionary = &state->dictionary;

		if (!Dictionary_LookUpAndCreateIfNotExist(&state->dictionary, identifier, &dictionary_entry))
		{
			OutOfMemoryError(state);
			dictionary_entry = NULL;
			if (dictionary != NULL)
				*dictionary = NULL;
		}
	}

	String_Destroy(&expanded_identifier);

	return dictionary_entry;
}

static Dictionary_Entry* CreateSymbol(SemanticState *state, const StringView *identifier)
{
	Dictionary_Entry *dictionary_entry;

	dictionary_entry = LookupSymbolAndCreateIfNotExist(state, identifier, NULL);

	if (dictionary_entry != NULL && dictionary_entry->type != -1)
	{
		SemanticError(state, "Symbol '%.*s' cannot be redefined.", (int)StringView_Length(identifier), StringView_Data(identifier));
		dictionary_entry = NULL;
	}

	return dictionary_entry;
}

static cc_bool GetSymbolInteger(SemanticState *state, const StringView *identifier, const cc_bool must_evaluate_on_first_pass, unsigned long* const value)
{
	cc_bool success = cc_true;

	Dictionary_Entry *dictionary_entry;

	/* Initialize to 0 as default */
	*value = 0;

	dictionary_entry = LookupSymbol(state, identifier, NULL);

	if (dictionary_entry == NULL || dictionary_entry->type == -1)
	{
		/* For 'if' statements, undefined symbols should be treated as 0 without error (ASM68K behavior) */
		if (state->evaluating_if_condition)
		{
			/* Silently treat undefined as 0 in if conditions */
			success = cc_true;
		}
		else if (must_evaluate_on_first_pass)
		{
			SemanticError(state, "Symbol '%.*s' must be evaluable on first pass.", (int)StringView_Length(identifier), StringView_Data(identifier));
			success = cc_false;
		}
		else if (state->doing_final_pass)
		{
			SemanticError(state, "Symbol '%.*s' does not exist.", (int)StringView_Length(identifier), StringView_Data(identifier));
			success = cc_false;
		}
		else
		{
			state->fix_up_needed = cc_true;
			success = cc_false;
		}
	}
	else
	{
		switch (dictionary_entry->type)
		{
			case SYMBOL_SPECIAL:
				/* Don't worry; I know what I'm doing. */
				if (dictionary_entry->shared.pointer == &string_rs)
				{
					*value = state->rs;
				}
				else if (dictionary_entry->shared.pointer == &string_narg)
				{
					if (!CurrentlyExpandingMacro(state))
					{
						SemanticWarning(state, "Symbol '%.*s' cannot be used outside of a macro.", (int)StringView_Length(identifier), StringView_Data(identifier));
						success = cc_false;
					}
					else
					{
						*value = state->macro.total_arguments_at_start;
					}
				}
				else
				{
					assert(cc_false);
					success = cc_false;
				}

				break;

			case SYMBOL_LABEL:
			case SYMBOL_CONSTANT:
			case SYMBOL_VARIABLE:
				if (state->doing_fix_up && dictionary_entry->type == SYMBOL_VARIABLE)
					SemanticWarning(state, "Symbol '%.*s' forward-referenced despite being a redefineable variable.", (int)StringView_Length(identifier), StringView_Data(identifier));

				*value = dictionary_entry->shared.unsigned_long;
				break;

			default:
				SemanticError(state, "Symbol '%.*s' is not a label, constant, or variable.", (int)StringView_Length(identifier), StringView_Data(identifier));
				success = cc_false;
				break;
		}
	}

	return success;
}

static void AddToSourceLineList(SemanticState *state, SourceLineList *source_line_list, const String *source_line)
{
	SourceLineListNode* const source_line_list_node = (SourceLineListNode*)MallocAndHandleError(state, sizeof(SourceLineListNode));

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
		String_CreateCopy(&source_line_list_node->source_line_buffer, source_line);
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
					case EXPRESSION_INSTR:
					case EXPRESSION_DEF:
					case EXPRESSION_TYPE_WITH_IDENTIFIER:
					case EXPRESSION_TYPE_WITH_NUMBER:
					case EXPRESSION_FILESIZE:
					case EXPRESSION_NEGATE:
					case EXPRESSION_BITWISE_NOT:
					case EXPRESSION_LOGICAL_NOT:
						/* This code should never be ran. */
						assert(cc_false);
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
					case EXPRESSION_INSTR:
					case EXPRESSION_DEF:
					case EXPRESSION_TYPE_WITH_IDENTIFIER:
					case EXPRESSION_TYPE_WITH_NUMBER:
					case EXPRESSION_FILESIZE:
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
						assert(cc_false);
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
			if (!GetSymbolInteger(state, String_View(&expression->shared.string), cc_false, value))
				success = cc_false;

			break;

		case EXPRESSION_STRING:
		{
			const size_t length = String_Length(&expression->shared.string);

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
					*value |= String_At(&expression->shared.string, i);
				}
			}

			break;
		}

		case EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT:
			*value = state->program_counter_of_statement;
			break;

		case EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION:
			*value = state->program_counter_of_expression;
			break;

		case EXPRESSION_STRLEN:
			*value = String_Length(&expression->shared.string);
			break;

		case EXPRESSION_STRCMP:
			*value = String_Compare(&expression->shared.subexpressions[0].shared.string, &expression->shared.subexpressions[1].shared.string) ? -1 : 0;
			break;

		case EXPRESSION_INSTR:
		{
			unsigned long position;

			if (!ResolveExpression(state, &expression->shared.subexpressions[0], &position, fold))
			{
				success = cc_false;
			}
			else if (expression->shared.subexpressions[1].type != EXPRESSION_STRING || expression->shared.subexpressions[2].type != EXPRESSION_STRING)
			{
				success = cc_false;
				SemanticError(state, "Last two arguments to `INSTR` must be strings.");
			}
			else
			{
				/* TODO: What if 'position' is 0? */
				const size_t found_position = String_Find(&expression->shared.subexpressions[1].shared.string, String_View(&expression->shared.subexpressions[2].shared.string), position - 1, Options_Get(&state->options)->case_insensitive);
				*value = found_position == STRING_POSITION_INVALID ? 0 : found_position + 1;
			}
			break;
		}

		case EXPRESSION_DEF:
			*value = LookupSymbol(state, String_View(&expression->shared.string), NULL) != NULL ? -1 : 0;
			break;

		case EXPRESSION_TYPE_WITH_IDENTIFIER:
			*value = 0; /* TODO: This is a placeholder! */
			break;

		case EXPRESSION_TYPE_WITH_NUMBER:
			*value = 1; /* TODO: This is a placeholder! */
			break;

		case EXPRESSION_FILESIZE:
		{
			const char* const file_path = String_CStr(&expression->shared.string);
			FILE* const file = fopen(file_path, "rb");

			if (file == NULL)
			{
				success = cc_false;
				SemanticError(state, "File '%s' could not be opened.", file_path);
			}
			else
			{
				fseek(file, 0, SEEK_END);
				*value = ftell(file);

				fclose(file);
			}

			break;
		}
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
			case EXPRESSION_INSTR:
				free(expression->shared.subexpressions);
				break;

			case EXPRESSION_IDENTIFIER:
			case EXPRESSION_STRING:
			case EXPRESSION_STRLEN:
			case EXPRESSION_DEF:
			case EXPRESSION_TYPE_WITH_IDENTIFIER:
			case EXPRESSION_FILESIZE:
				String_Destroy(&expression->shared.string);
				break;

			case EXPRESSION_NUMBER:
			case EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT:
			case EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION:
			case EXPRESSION_TYPE_WITH_NUMBER:
				break;
		}

		expression->type = EXPRESSION_NUMBER;
		expression->shared.unsigned_long = *value;
	}

	return success;
}

static void TerminateIf(SemanticState *state)
{
	if (state->false_if_level == state->current_if_level || state->false_if_level == 0)
	{
		state->true_already_found = cc_true;
		state->false_if_level = 0;
	}

	--state->current_if_level;
}

static void TerminateRept(SemanticState *state)
{
	/* Back-up some state into local variables, in case a nested REPT statement clobbers it. */
	unsigned long repetitions = state->shared.rept.repetitions;

	const unsigned long line_number = state->shared.rept.line_number;
	const SourceLineList source_line_list = state->shared.rept.source_line_list;

	/* Exit REPT mode. */
	state->mode = MODE_NORMAL;

	while (repetitions-- != 0)
	{
		SourceLineListNode *source_line_list_node;

		/* Rewind back to the line number of the start of the REPT. */
		state->location->line_number = line_number;

		/* Process the REPT's nested statements. */
		for (source_line_list_node = source_line_list.head; source_line_list_node != NULL; source_line_list_node = source_line_list_node->next)
			AssembleLine(state, &source_line_list_node->source_line_buffer, Options_Get(&state->options)->expand_all_macros);
	}

	/* Increment past the ENDR line number. */
	++state->location->line_number;

	FreeSourceLineList(source_line_list.head);

}

static void PurgeMacro(SemanticState* const state, const StringView* const identifier, const cc_bool allow_undefined)
{
	Dictionary_State *dictionary;
	Dictionary_Entry* const entry = LookupSymbol(state, identifier, &dictionary);

	if (entry == NULL)
	{
		if (!allow_undefined)
			SemanticError(state, "Cannot purge '%.*s' as it does not exist.\n", (int)StringView_Length(identifier), StringView_Data(identifier));
	}
	else if (entry->type != SYMBOL_MACRO)
	{
		SemanticError(state, "Cannot purge '%.*s' as it is not a macro.\n", (int)StringView_Length(identifier), StringView_Data(identifier));
	}
	else
	{
		Dictionary_Remove(dictionary, identifier);
	}
}

static void TerminateMacro(SemanticState *state)
{
	const StringView* const identifier = String_View(&state->shared.macro.name);

	/* Exit macro mode. */
	state->mode = MODE_NORMAL;

	PurgeMacro(state, identifier, cc_true);

	if (!StringView_Empty(identifier))
	{
		Dictionary_Entry* const symbol = CreateSymbol(state, identifier);

		/* Add the macro to the symbol table. */
		if (symbol != NULL)
		{
			Macro* const macro = (Macro*)MallocAndHandleError(state, sizeof(Macro));

			if (macro != NULL)
			{
				macro->is_short = state->shared.macro.is_short;
				macro->uses_label = state->shared.macro.uses_label;
				String_CreateMove(&macro->name, &state->shared.macro.name);
				macro->parameter_names = state->shared.macro.parameter_names.head;
				macro->source_line_list_head = state->shared.macro.source_line_list.head;

				symbol->type = SYMBOL_MACRO;
				symbol->shared.pointer = macro;
			}
		}
	}
}

static void TerminateWhile(SemanticState *state)
{
	const String* const old_source_line = state->source_line;

	/* Back-up some state into local variables, in case a nested WHILE statement clobbers it. */
	Expression expression = state->shared.while_statement.expression;
	String source_line = state->shared.while_statement.source_line;
	const unsigned long starting_line_number = state->shared.while_statement.line_number;
	SourceLineListNode* const source_line_list_head = state->shared.while_statement.source_line_list.head;

	/* Exit WHILE mode. */
	state->mode = MODE_NORMAL;

	state->suppress_listing = cc_true;

	for (;;)
	{
		unsigned long value;
		SourceLineListNode *source_line_list_node;

		state->source_line = &source_line;

		if (!ResolveExpression(state, &expression, &value, cc_false))
		{
			SemanticError(state, "Expression must be evaluable on the first pass.");
			break;
		}

		if (value == 0)
			break;

		/* Rewind back to the line number of the start of the WHILE. */
		state->location->line_number = starting_line_number;

		/* Process the WHILE's nested statements. */
		for (source_line_list_node = source_line_list_head; source_line_list_node != NULL; source_line_list_node = source_line_list_node->next)
			AssembleLine(state, &source_line_list_node->source_line_buffer, cc_false);
	}

	/* Increment past the ENDW line number. */
	++state->location->line_number;

	FreeSourceLineList(source_line_list_head);
	String_Destroy(&source_line);
	DestroyExpression(&expression);

	state->source_line = old_source_line;
}

static unsigned int ConstructSizeBits(Size size)
{
	switch (size)
	{
		case SIZE_BYTE:
		case SIZE_SHORT:
			return 0x0000;

		case SIZE_UNDEFINED:
		case SIZE_WORD:
			return 0x0040;

		case SIZE_LONGWORD:
			return 0x0080;
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

static void SetLastGlobalLabel(SemanticState *state, const StringView *label)
{
	const char first_character = StringView_At(label, 0);

	if (first_character != Options_Get(&state->options)->local_signifier)
	{
		/* This is a global label - cache it for later. */
		String_Destroy(&state->last_global_label);
		String_CreateCopyView(&state->last_global_label, label);
	}
}

static void AddIdentifierToSymbolTable(SemanticState *state, const StringView *label, unsigned long value, SymbolType type)
{
	Dictionary_Entry *symbol;

	symbol = NULL;

	/* Now add it to the symbol table. */
	switch (type)
	{
		case SYMBOL_VARIABLE:
		case SYMBOL_CONSTANT:
			symbol = LookupSymbolAndCreateIfNotExist(state, label, NULL);

			if (symbol != NULL)
			{
				if (symbol->type != -1 && (SymbolType)symbol->type != type)
					SemanticError(state, "Symbol redefined as a different type.");
				else if (type == SYMBOL_CONSTANT && symbol->type == SYMBOL_CONSTANT && symbol->shared.unsigned_long != value)
					SemanticError(state, "Constant cannot be redefined to a different value.");
			}

			break;

		case SYMBOL_LABEL:
			symbol = CreateSymbol(state, label);
			break;

		case SYMBOL_MACRO:
		case SYMBOL_SPECIAL:
		case SYMBOL_STRING_CONSTANT:
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
			#define ALIAS_AND_LOOP(ALIAS) {instruction->opcode.type = ALIAS; continue;}
			#define WARN_ALIAS_AND_LOOP(ALIAS, MESSAGE) {if (Options_Get(&state->options)->pedantic_warnings_enabled) SemanticWarning(state, MESSAGE); ALIAS_AND_LOOP(ALIAS)}

			case OPCODE_ORI:
				if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ORI_TO_CCR)
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ORI_TO_SR)

				break;

			case OPCODE_ANDI:
				if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ANDI_TO_CCR)
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ANDI_TO_SR)

				break;

			case OPCODE_EORI:
				if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_EORI_TO_CCR)
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_EORI_TO_SR)

				break;

			case OPCODE_BTST_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BTST_DYNAMIC)

				break;

			case OPCODE_BCHG_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BCHG_DYNAMIC)

				break;

			case OPCODE_BCLR_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BCLR_DYNAMIC)

				break;

			case OPCODE_BSET_STATIC:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_BSET_DYNAMIC)

				break;

			case OPCODE_MOVEP_TO_REG:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVEP_FROM_REG)

				break;

			case OPCODE_MOVE:
				if (instruction->operands[0].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_FROM_SR)
				else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_TO_SR)
				else if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_TO_CCR)
				else if (instruction->operands[0].type == OPERAND_USER_STACK_POINTER_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_FROM_USP)
				else if (instruction->operands[1].type == OPERAND_USER_STACK_POINTER_REGISTER)
					ALIAS_AND_LOOP(OPCODE_MOVE_TO_USP)
				else if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_MOVEA, "MOVE should be MOVEA.")
				else if (instruction->operands[0].type == OPERAND_REGISTER_LIST)
					WARN_ALIAS_AND_LOOP(OPCODE_MOVEM_FROM_REGS, "MOVE should be MOVEM.")
				else if (instruction->operands[1].type == OPERAND_REGISTER_LIST)
					WARN_ALIAS_AND_LOOP(OPCODE_MOVEM_TO_REGS, "MOVE should be MOVEM.")

				break;

			case OPCODE_MOVEM_TO_REGS:
				if (instruction->operands[0].type == OPERAND_DATA_REGISTER || instruction->operands[0].type == OPERAND_ADDRESS_REGISTER || instruction->operands[0].type == OPERAND_REGISTER_LIST)
					ALIAS_AND_LOOP(OPCODE_MOVEM_FROM_REGS)

				break;

			case OPCODE_SBCD_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_SBCD_ADDRESS_REGS)

				break;

			case OPCODE_OR_TO_REG:
				if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_ORI, "OR should be ORI.")
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_OR_FROM_REG)

				break;

			case OPCODE_SUB_TO_REG:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_SUBA, "SUB should be SUBA.")
				else if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_SUBI, "SUB should be SUBI.")
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_SUB_FROM_REG)

				break;

			case OPCODE_SUBI:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_SUBA, "SUBI should be SUBA.")

				break;

			case OPCODE_SUBX_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_SUBX_ADDRESS_REGS)

				break;

			case OPCODE_ABCD_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_ABCD_ADDRESS_REGS)

				break;

			case OPCODE_AND_TO_REG:
				if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_ANDI, "AND should be ANDI.")
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_AND_FROM_REG)

				break;

			case OPCODE_ADD_TO_REG:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_ADDA, "ADD should be ADDA.")
				else if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_ADDI, "ADD should be ADDI.")
				else if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
					ALIAS_AND_LOOP(OPCODE_ADD_FROM_REG)

				break;

			case OPCODE_ADDI:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_ADDA, "ADDI should be ADDA.")

				break;

			case OPCODE_ADDX_DATA_REGS:
				if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					ALIAS_AND_LOOP(OPCODE_ADDX_ADDRESS_REGS)

				break;

			case OPCODE_CMP:
				if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					WARN_ALIAS_AND_LOOP(OPCODE_CMPA, "CMP should be CMPA.")
				else if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_CMPI, "CMP should be CMPI.")
				else if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT && instruction->operands[1].type == OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT)
					WARN_ALIAS_AND_LOOP(OPCODE_CMPM, "CMP should be CMPM.")

				break;

			case OPCODE_EOR:
				if (instruction->operands[0].type == OPERAND_LITERAL)
					WARN_ALIAS_AND_LOOP(OPCODE_EORI, "EOR should be EORI.")

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
								ALIAS_AND_LOOP(OPCODE_ASL_DYNAMIC)
								break;

							case OPCODE_ASR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ASR_DYNAMIC)
								break;

							case OPCODE_LSL_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSL_DYNAMIC)
								break;

							case OPCODE_LSR_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSR_DYNAMIC)
								break;

							case OPCODE_ROXL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXL_DYNAMIC)
								break;

							case OPCODE_ROXR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXR_DYNAMIC)
								break;

							case OPCODE_ROL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROL_DYNAMIC)
								break;

							case OPCODE_ROR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROR_DYNAMIC)
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
								ALIAS_AND_LOOP(OPCODE_ASL_SINGLE)
								break;

							case OPCODE_ASR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ASR_SINGLE)
								break;

							case OPCODE_LSL_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSL_SINGLE)
								break;

							case OPCODE_LSR_STATIC:
								ALIAS_AND_LOOP(OPCODE_LSR_SINGLE)
								break;

							case OPCODE_ROXL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXL_SINGLE)
								break;

							case OPCODE_ROXR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROXR_SINGLE)
								break;

							case OPCODE_ROL_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROL_SINGLE)
								break;

							case OPCODE_ROR_STATIC:
								ALIAS_AND_LOOP(OPCODE_ROR_SINGLE)
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUBI */
		"SUBI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_ADDI */
		"ADDI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_CMPI */
		"CMPI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_MOVEP_FROM_REG */
		"MOVEP",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT,
		}
	},
	{	/* OPCODE_MOVEA */
		"MOVEA",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_CLR */
		"CLR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_NEG */
		"NEG",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_NOT */
		"NOT",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
			0,
		}
	},
	{	/* OPCODE_EXT */
		"EXT",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUBQ */
		"SUBQ",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUB_TO_REG */
		"SUB",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_SUBX_DATA_REGS */
		"SUBX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_SUBX_ADDRESS_REGS */
		"SUBX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
		}
	},
	{	/* OPCODE_SUBA */
		"SUBA",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_CMPM */
		"CMPM",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT,
			OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT,
		}
	},
	{	/* OPCODE_CMP */
		"CMP",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_ADD_TO_REG */
		"ADD",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_ADDRESS_ABSOLUTE,
		}
	},
	{	/* OPCODE_ADDX_DATA_REGS */
		"ADDX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ADDX_ADDRESS_REGS */
		"ADDX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
			OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT,
		}
	},
	{	/* OPCODE_ADDA */
		"ADDA",
		SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ASR_STATIC */
		"ASR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSL_STATIC */
		"LSL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSR_STATIC */
		"LSR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXL_STATIC */
		"ROXL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXR_STATIC */
		"ROXR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROL_STATIC */
		"ROL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROR_STATIC */
		"ROR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ASL_DYNAMIC */
		"ASL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ASR_DYNAMIC */
		"ASR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSL_DYNAMIC */
		"LSL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_LSR_DYNAMIC */
		"LSR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXL_DYNAMIC */
		"ROXL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROXR_DYNAMIC */
		"ROXR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROL_DYNAMIC */
		"ROL",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
		}
	},
	{	/* OPCODE_ROR_DYNAMIC */
		"ROR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD | SIZE_UNDEFINED,
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

							case SIZE_UNDEFINED:
							case SIZE_WORD:
								machine_code = 0x3000;
								break;

							case SIZE_LONGWORD:
								machine_code = 0x2000;
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

						if ((value & 1) != 0)
							SemanticWarning(state, "Branch leads to odd address 0x%lX.", value);

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

						if ((value & 1) != 0)
							SemanticWarning(state, "Branch leads to odd address 0x%lX.", value);

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

static void PushSubstitute(SemanticState* const state, const StringView* const identifier, const StringView* const value)
{
	Dictionary_State *dictionary;
	Dictionary_Entry* const dictionary_entry = LookupSymbolAndCreateIfNotExist(state, identifier, &dictionary);

	if (dictionary_entry != NULL)
	{
		switch (dictionary_entry->type)
		{
			case -1:
				dictionary_entry->type = SYMBOL_STRING_CONSTANT;
				Substitute_PushSubstitute(dictionary == &state->dictionary ? &state->substitutions : &state->macro.substitutions, identifier, String_View(&dictionary_entry->shared.string));
				break;

			case SYMBOL_STRING_CONSTANT:
				String_Destroy(&dictionary_entry->shared.string);
				break;

			default:
				SemanticError(state, "Attempted to redefined symbol '%.*s' as a string constant.", (int)StringView_Length(identifier), StringView_Data(identifier));
				return;
		}

		String_CreateCopyView(&dictionary_entry->shared.string, value);
	}
}

static void PushMacroArgumentSubstitutions(SemanticState* const state)
{
	const IdentifierListNode *parameter_name;
	size_t argument_index;

	const StringView blank_argument = STRING_VIEW_INITIALISER_BLANK;

	for (parameter_name = state->macro.metadata->parameter_names, argument_index = 0; parameter_name != NULL; parameter_name = parameter_name->next, ++argument_index)
	{
		Dictionary_Entry *dictionary_entry;

		const StringView* const parameter = String_View(&parameter_name->identifier);
		const StringView* const argument = argument_index >= state->macro.total_arguments ? &blank_argument : String_View(&state->macro.argument_list[argument_index]);

		if (!Dictionary_LookUpAndCreateIfNotExist(state->macro.dictionary, parameter, &dictionary_entry))
		{
			OutOfMemoryError(state);
		}
		else
		{
			/* TODO: De-duplicate this with the 'PushSubstitute' function. */
			if (dictionary_entry->type == -1)
			{
				dictionary_entry->type = SYMBOL_STRING_CONSTANT;
				Substitute_PushSubstitute(&state->macro.substitutions, parameter, String_View(&dictionary_entry->shared.string));
			}
			else
			{
				String_Destroy(&dictionary_entry->shared.string);
			}

			String_CreateCopyView(&dictionary_entry->shared.string, argument);
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
				SemanticError(state, "Value cannot be higher than $FF or lower than -$100, but was $%lX.", value);

			bytes_to_write = 1;

			break;

		case SIZE_WORD:
			if (value > 0xFFFF && value < 0xFFFF0000)
				SemanticError(state, "Value cannot be higher than $FFFF or lower than -$10000, but was $%lX.", value);

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

static void PerformEven(SemanticState* const state)
{
	/* Pad to the nearest even address. */
	if (state->program_counter & 1)
	{
		++state->program_counter;
		OutputByte(state, 0);
	}
}

static cc_bool ShouldPerformAutomaticEven(SemanticState* const state, const Size size)
{
	return Options_Get(&state->options)->automatic_even && size != SIZE_BYTE;
}

static void ProcessDc(SemanticState *state, StatementDc *dc)
{
	ExpressionListNode *expression_list_node;

	if (ShouldPerformAutomaticEven(state, dc->size))
		PerformEven(state);

	for (expression_list_node = dc->values.head; expression_list_node != NULL; expression_list_node = expression_list_node->next)
	{
		if (expression_list_node->expression.type == EXPRESSION_STRING && (dc->size == SIZE_BYTE || dc->size == SIZE_SHORT))
		{
			/* If this is a 'dc.b', and the value is a raw string, then output each character as a single byte. */
			/* This allows 'dc.b' to be used to embed strings into the output. */
			size_t i;

			for (i = 0; i < String_Length(&expression_list_node->expression.shared.string); ++i)
				OutputDcValue(state, dc->size, String_At(&expression_list_node->expression.shared.string, i));
		}
		else
		{
			unsigned long value;

			/* Update the program counter symbol in between values, to keep it up to date. */
			state->program_counter_of_expression = state->program_counter;

			if (!ResolveExpression(state, &expression_list_node->expression, &value, cc_true))
				value = 0;

			OutputDcValue(state, dc->size, value);
		}
	}
}

static void ProcessDcb(SemanticState *state, StatementDcb *dcb)
{
	unsigned long repetitions;

	if (ShouldPerformAutomaticEven(state, dcb->size))
		PerformEven(state);

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

static void ProcessDs(SemanticState *state, StatementDs *ds)
{
	unsigned long length;

	if (ShouldPerformAutomaticEven(state, ds->size))
		PerformEven(state);

	if (!ResolveExpression(state, &ds->length, &length, cc_true))
	{
		SemanticError(state, "Repetition value must be evaluable on first pass.");
	}
	else
	{
		unsigned long i;

		for (i = 0; i < length; ++i)
			OutputDcValue(state, ds->size, 0);
	}
}

static void ProcessInclude(SemanticState *state, const StatementInclude *include)
{
	FILE* const input_file = fopen_backslash(String_View(&include->path), "r");

	if (input_file == NULL)
	{
		SemanticError(state, "File '%s' could not be opened.", String_CStr(&include->path));
	}
	else
	{
		TextInput input_callbacks;
		Location location;

		const TextInput* const previous_input_callbacks = state->input_callbacks;

		input_callbacks.user_data = input_file;
		input_callbacks.read_line = ReadLine;

		/* Add file path and line number to the location list. */
		String_CreateCopy(&location.file_path, &include->path);
		location.line_number = 0;

		location.previous = state->location;
		state->location = &location;

		state->input_callbacks = &input_callbacks;

		AssembleFile(state);

		state->input_callbacks = previous_input_callbacks;

		state->location = state->location->previous;

		String_Destroy(&location.file_path);

		fclose(input_file);
	}
}

static void ProcessIncbin(SemanticState *state, StatementIncbin *incbin)
{
	FILE* const input_file = fopen_backslash(String_View(&incbin->path), "rb");

	if (input_file == NULL)
	{
		SemanticError(state, "File '%s' could not be opened.", String_CStr(&incbin->path));
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
				OutputWriteSegmentByte(state, character);
			}
		}
		else
		{
			int character;

			/* TODO: Batch bytes to make this faster? */
			while ((character = fgetc(input_file)) != EOF)
			{
				++state->program_counter;
				OutputWriteSegmentByte(state, character);
			}
		}

		fclose(input_file);
	}
}

static cc_bool ReadSourceLineRaw(SemanticState *state, size_t line_buffer_write_position)
{
	cc_bool data_read = cc_false;

	/* Repeatedly read into the string until we've fit a whole line (or an error occurs). */
	while (TextInput_fgets(&String_At(&state->line_buffer, line_buffer_write_position), String_Capacity(&state->line_buffer) + 1 - line_buffer_write_position, state->input_callbacks) != NULL)
	{
		line_buffer_write_position += strlen(&String_At(&state->line_buffer, line_buffer_write_position));

		if (line_buffer_write_position != 0)
		{
			const char character_before_end = String_At(&state->line_buffer, line_buffer_write_position - 1);

			data_read = cc_true;

			/* Finding a newline means that a whole line has been read! */
			if (character_before_end == '\r' || character_before_end == '\n')
			{
				/* Discard the newline. */
				--line_buffer_write_position;

				/* Discard the other newline, if one exists. */
				if (character_before_end == '\n' && line_buffer_write_position != 0)
				{
					const char character_before_newline = String_At(&state->line_buffer, line_buffer_write_position - 1);

					if (character_before_newline == '\r')
						--line_buffer_write_position;
				}

				break;
			}
		}

		/* If the buffer is not full but there is no newline, then we must have reached the end of the file, so we can exit now. */
		if (line_buffer_write_position != String_Capacity(&state->line_buffer))
			break;

		/* If there the buffer is full and there is no newline, then the line must be longer than the buffer, so make the buffer bigger! */
		String_Reserve(&state->line_buffer, ((String_Capacity(&state->line_buffer) + 1) * 2) - 1);
		/* Now loop back around and read more of the line! */
	}

	/* Correct line length. */
	String_ResizeNoFill(&state->line_buffer, line_buffer_write_position);

	return data_read;
}

static cc_bool ReadSourceLine(SemanticState *state)
{
	cc_bool data_read = cc_false;
	size_t line_buffer_write_position = 0;

	while (ReadSourceLineRaw(state, line_buffer_write_position))
	{
		char quote_character;

		data_read = cc_true;

		/* Find where the code ends and a comment begins.
		   Note that terminating at ';' prevents a trailing '&' from being recognised
		   and causing a line to be continued on the next line.
		   This is needed for compatibility with S.N. 68k (asm68k). */
		/* TODO: Does S.N. 68k actually concatenate when a '&' occurs before a comment? */
		quote_character = '\0';

		for (; line_buffer_write_position < String_Length(&state->line_buffer); ++line_buffer_write_position)
		{
			const char character = String_At(&state->line_buffer, line_buffer_write_position);

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

		/* An '&' at the end of a line is like a '\' at the end of a line in C:
		   it signals that the current line is continued on the next line. */
		if (line_buffer_write_position == 0 || String_At(&state->line_buffer, line_buffer_write_position - 1) != '&')
			break;

		/* Discard the '&'. */
		--line_buffer_write_position;

		/* Go back and get another line. */
	}

	return data_read;
}

static void ProcessRept(SemanticState *state, StatementRept *rept)
{
	/* Enter REPT mode. */
	state->mode = MODE_REPT;

	if (!ResolveExpression(state, &rept->repetitions, &state->shared.rept.repetitions, cc_true))
	{
		SemanticError(state, "Repetition value must be evaluable on the first pass.");
		state->shared.rept.repetitions = 1;
	}

	state->shared.rept.line_number = state->location->line_number;
	state->shared.rept.nesting = 0;

	state->shared.rept.source_line_list.head = NULL;
	state->shared.rept.source_line_list.tail = NULL;
}

static void ProcessMacro(SemanticState *state, StatementMacro *macro, const StringView *label, cc_bool is_short)
{
	/* Enter MACRO mode. */
	state->mode = MODE_MACRO;

	String_CreateCopyView(&state->shared.macro.name, label);
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

	/* Set flag to allow undefined symbols in if conditions */
	state->evaluating_if_condition = cc_true;
	
	/* Undefined symbols will be silently treated as 0 */
	if (!ResolveExpression(state, expression, &value, cc_true))
	{
		/* Value is already initialized to 0 by GetSymbolInteger for undefined symbols */
		value = 0;
	}
	
	state->evaluating_if_condition = cc_false;

	/* If this condition is false, then mark this as the false if-level. */
	if (value == 0)
		state->false_if_level = state->current_if_level;

	state->true_already_found = value != 0;
}

static void ProcessIfEq(SemanticState *state, Expression *expression)
{
	unsigned long value;

	++state->current_if_level;

	/* Set flag to allow undefined symbols in if conditions */
	state->evaluating_if_condition = cc_true;
	
	if (!ResolveExpression(state, expression, &value, cc_true))
		value = 0;
	
	state->evaluating_if_condition = cc_false;

	/* ifeq: true if value == 0 */
	if (value != 0)
		state->false_if_level = state->current_if_level;

	state->true_already_found = (value == 0);
}

static void ProcessIfNe(SemanticState *state, Expression *expression)
{
	unsigned long value;

	++state->current_if_level;

	state->evaluating_if_condition = cc_true;
	
	if (!ResolveExpression(state, expression, &value, cc_true))
		value = 0;
	
	state->evaluating_if_condition = cc_false;

	/* ifne: true if value != 0 */
	if (value == 0)
		state->false_if_level = state->current_if_level;

	state->true_already_found = (value != 0);
}

static void ProcessIfGt(SemanticState *state, Expression *expression)
{
	unsigned long value;

	++state->current_if_level;

	state->evaluating_if_condition = cc_true;
	
	if (!ResolveExpression(state, expression, &value, cc_true))
		value = 0;
	
	state->evaluating_if_condition = cc_false;

	/* ifgt: true if value > 0 */
	if (value <= 0)
		state->false_if_level = state->current_if_level;

	state->true_already_found = (value > 0);
}

static void ProcessIfLt(SemanticState *state, Expression *expression)
{
	unsigned long value;

	++state->current_if_level;

	state->evaluating_if_condition = cc_true;
	
	if (!ResolveExpression(state, expression, &value, cc_true))
		value = 0;
	
	state->evaluating_if_condition = cc_false;

	/* iflt: true if value < 0 (signed comparison) */
	/* Treat as signed: if value > 0x7FFFFFFF, it's negative */
	long signed_value = (long)(value & 0xFFFFFFFF);
	if (signed_value >= 0)
		state->false_if_level = state->current_if_level;

	state->true_already_found = (signed_value < 0);
}

static void ProcessIfGe(SemanticState *state, Expression *expression)
{
	unsigned long value;

	++state->current_if_level;

	state->evaluating_if_condition = cc_true;
	
	if (!ResolveExpression(state, expression, &value, cc_true))
		value = 0;
	
	state->evaluating_if_condition = cc_false;

	/* ifge: true if value >= 0 (signed comparison) */
	long signed_value = (long)(value & 0xFFFFFFFF);
	if (signed_value < 0)
		state->false_if_level = state->current_if_level;

	state->true_already_found = (signed_value >= 0);
}

static void ProcessIfLe(SemanticState *state, Expression *expression)
{
	unsigned long value;

	++state->current_if_level;

	state->evaluating_if_condition = cc_true;
	
	if (!ResolveExpression(state, expression, &value, cc_true))
		value = 0;
	
	state->evaluating_if_condition = cc_false;

	/* ifle: true if value <= 0 (signed comparison) */
	long signed_value = (long)(value & 0xFFFFFFFF);
	if (signed_value > 0)
		state->false_if_level = state->current_if_level;

	state->true_already_found = (signed_value <= 0);
}

static void ProcessStatement(SemanticState *state, Statement *statement, const StringView *label)
{
	/* Bizarrely, depending on the context, the program counter can be one
	   of two values, so we need to maintain two separate copies of it. */
	state->program_counter_of_statement = state->program_counter;
	state->program_counter_of_expression = state->program_counter;

	switch (statement->type)
	{
		case STATEMENT_TYPE_EMPTY:
		case STATEMENT_TYPE_INSTRUCTION:
		case STATEMENT_TYPE_DC:
		case STATEMENT_TYPE_DCB:
		case STATEMENT_TYPE_DS:
		case STATEMENT_TYPE_INCLUDE:
		case STATEMENT_TYPE_INCBIN:
		case STATEMENT_TYPE_REPT:
		case STATEMENT_TYPE_IF:
		case STATEMENT_TYPE_IFEQ:
		case STATEMENT_TYPE_IFNE:
		case STATEMENT_TYPE_IFGT:
		case STATEMENT_TYPE_IFLT:
		case STATEMENT_TYPE_IFGE:
		case STATEMENT_TYPE_IFLE:
		case STATEMENT_TYPE_WHILE:
		case STATEMENT_TYPE_EVEN:
		case STATEMENT_TYPE_CNOP:
		case STATEMENT_TYPE_INFORM:
		case STATEMENT_TYPE_FAIL:
		case STATEMENT_TYPE_END:
		case STATEMENT_TYPE_PUSHO:
		case STATEMENT_TYPE_POPO:
		case STATEMENT_TYPE_OPT:
		case STATEMENT_TYPE_PUSHP:
		case STATEMENT_TYPE_POPP:
		case STATEMENT_TYPE_SHIFT:
		case STATEMENT_TYPE_MEXIT:
		case STATEMENT_TYPE_LOCAL:
		case STATEMENT_TYPE_PURGE:
			if (!StringView_Empty(label) && !state->doing_fix_up)
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
			if (!StringView_Empty(label))
				SemanticError(state, "There cannot be a label on this type of statement.");

			break;

		case STATEMENT_TYPE_MACRO:
		case STATEMENT_TYPE_MACROS:
		case STATEMENT_TYPE_EQU:
		case STATEMENT_TYPE_EQUR:
		case STATEMENT_TYPE_EQUS_STRING:
		case STATEMENT_TYPE_EQUS_IDENTIFIER:
		case STATEMENT_TYPE_SUBSTR:
		case STATEMENT_TYPE_SET:
			if (StringView_Empty(label))
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

		case STATEMENT_TYPE_DS:
			ProcessDs(state, &statement->shared.ds);
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

			if (Options_Get(&state->options)->equ_set_descope_local_labels)
				SetLastGlobalLabel(state, label);

			if (ResolveExpression(state, &statement->shared.expression, &value, cc_true))
			{
				AddIdentifierToSymbolTable(state, label, value, SYMBOL_CONSTANT);
				ListIdentifierValue(state, value);
			}

			break;
		}

		case STATEMENT_TYPE_EQUR:
		{
			char register_name[4];
			unsigned long register_value;
			String register_string;
			size_t register_name_length;

			if (Options_Get(&state->options)->equ_set_descope_local_labels)
				SetLastGlobalLabel(state, label);

			/* Register value is already encoded: 0-7 for d0-d7, 8-15 for a0-a7 */
			register_value = statement->shared.equr.register_value;

			/* Create a string substitution like "d0" or "a5" */
			if (register_value < 8)
			{
				/* Data register: d0-d7 */
				register_name_length = sprintf(register_name, "d%lu", register_value);
			}
			else
			{
				/* Address register: a0-a7 */
				register_name_length = sprintf(register_name, "a%lu", register_value - 8);
			}

			/* Create a String to hold the register name permanently */
			if (!String_Create(&register_string, register_name, register_name_length))
			{
				SemanticError(state, "Could not allocate memory for register name.");
				break;
			}

			/* Create a string substitution so the register name is replaced textually */
			PushSubstitute(state, label, String_View(&register_string));

			/* The string data has been copied by PushSubstitute, so we can destroy our local copy */
			String_Destroy(&register_string);

			break;
		}

		case STATEMENT_TYPE_EQUS_STRING:
			PushSubstitute(state, label, String_View(&statement->shared.string));
			break;

		case STATEMENT_TYPE_EQUS_IDENTIFIER:
		{
			const Dictionary_Entry* const dictionary_entry = LookupSymbol(state, String_View(&statement->shared.string), NULL);

			if (dictionary_entry == NULL || dictionary_entry->type != SYMBOL_STRING_CONSTANT)
				SemanticError(state, "String constant can only be defined as a string or another string constant.");
			else
				PushSubstitute(state, label, String_View(&dictionary_entry->shared.string));

			break;
		}

		case STATEMENT_TYPE_SUBSTR:
		{
			unsigned long start, end;
			StringView substring;

			if (!ResolveExpression(state, &statement->shared.substr.start, &start, cc_true))
			{
				SemanticError(state, "Start must be evaluable on the first pass.");
				break;
			}

			if (!ResolveExpression(state, &statement->shared.substr.end, &end, cc_true))
			{
				SemanticError(state, "End must be evaluable on the first pass.");
				break;
			}

			if (start == 0)
			{
				SemanticError(state, "Start cannot be zero.");
				break;
			}

			if (end < start)
				end = start - 1;

			StringView_SubStr(&substring, String_View(&statement->shared.substr.string), start - 1, end - start + 1);
			PushSubstitute(state, label, &substring);
			break;
		}

		case STATEMENT_TYPE_SET:
		{
			unsigned long value;

			if (Options_Get(&state->options)->equ_set_descope_local_labels)
				SetLastGlobalLabel(state, label);

			if (ResolveExpression(state, &statement->shared.expression, &value, cc_true))
			{
				AddIdentifierToSymbolTable(state, label, value, SYMBOL_VARIABLE);
				ListIdentifierValue(state, value);
			}

			break;
		}

		case STATEMENT_TYPE_IF:
			ProcessIf(state, &statement->shared.expression);
			break;

		case STATEMENT_TYPE_IFEQ:
			if (!Options_Get(&state->options)->allow_ifeq_directives)
				SemanticError(state, "Directive 'ifeq' requires the /k flag.");
			else
				ProcessIfEq(state, &statement->shared.expression);
			break;

		case STATEMENT_TYPE_IFNE:
			if (!Options_Get(&state->options)->allow_ifeq_directives)
				SemanticError(state, "Directive 'ifne' requires the /k flag.");
			else
				ProcessIfNe(state, &statement->shared.expression);
			break;

		case STATEMENT_TYPE_IFGT:
			if (!Options_Get(&state->options)->allow_ifeq_directives)
				SemanticError(state, "Directive 'ifgt' requires the /k flag.");
			else
				ProcessIfGt(state, &statement->shared.expression);
			break;

		case STATEMENT_TYPE_IFLT:
			if (!Options_Get(&state->options)->allow_ifeq_directives)
				SemanticError(state, "Directive 'iflt' requires the /k flag.");
			else
				ProcessIfLt(state, &statement->shared.expression);
			break;

		case STATEMENT_TYPE_IFGE:
			if (!Options_Get(&state->options)->allow_ifeq_directives)
				SemanticError(state, "Directive 'ifge' requires the /k flag.");
			else
				ProcessIfGe(state, &statement->shared.expression);
			break;

		case STATEMENT_TYPE_IFLE:
			if (!Options_Get(&state->options)->allow_ifeq_directives)
				SemanticError(state, "Directive 'ifle' requires the /k flag.");
			else
				ProcessIfLe(state, &statement->shared.expression);
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

					/* Set flag to allow undefined symbols in elseif conditions */
					state->evaluating_if_condition = cc_true;
					
					if (!ResolveExpression(state, &statement->shared.expression, &value, cc_true))
					{
						/* Undefined symbols default to 0 (ASM68K behavior) */
						value = 0;
					}
					
					state->evaluating_if_condition = cc_false;

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
				SemanticError(state, "This stray ENDC/ENDIF has no preceeding IF.");
			else
				TerminateIf(state);

			break;

		case STATEMENT_TYPE_WHILE:
			/* Enter WHILE mode. */
			state->mode = MODE_WHILE;

			state->shared.while_statement.expression = statement->shared.expression;
			String_CreateCopy(&state->shared.while_statement.source_line, state->source_line);

			state->shared.while_statement.line_number = state->location->line_number;
			state->shared.while_statement.nesting = 0;

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
			PerformEven(state);
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
					TextOutput_fprintf(state->error_callbacks, "INFORM: '%s'\n", String_CStr(&statement->shared.inform.message));
					break;

				case 1:
					SemanticWarning(state, "INFORM: '%s'", String_CStr(&statement->shared.inform.message));
					break;

				case 2:
				case 3: /* TODO: Halt assembly. */
					SemanticError(state, "INFORM: '%s'", String_CStr(&statement->shared.inform.message));
					break;
			}

			break;
		}

		case STATEMENT_TYPE_FAIL:
			/* TODO: Halt assembly. */
			SemanticError(state, "Assembly Failed");
			break;

		case STATEMENT_TYPE_END:
			state->end = cc_true;
			break;

		case STATEMENT_TYPE_RS:
		{
			unsigned long length;

			if (ShouldPerformAutomaticEven(state, statement->shared.rs.size))
				if ((state->rs & 1) != 0)
					++state->rs;

			if (!ResolveExpression(state, &statement->shared.rs.length, &length, cc_true))
			{
				SemanticError(state, "Length must be evaluable on the first pass.");
			}
			else
			{
				/* Add label to symbol table. */
				if (!StringView_Empty(label))
				{
					AddIdentifierToSymbolTable(state, label, state->rs, SYMBOL_CONSTANT);
					ListIdentifierValue(state, state->rs);
				}

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

				state->rs += length;
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
				state->rs = value;

			break;
		}

		case STATEMENT_TYPE_RSRESET:
			/* Set '__rs' to 0. */
			state->rs = 0;
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
				/* Begin a new segment! */
				state->segment_position = value;
				WriteRecordSegment(state);
				state->segment_length = 0;

				state->program_counter = value;
			}

			break;
		}

		case STATEMENT_TYPE_PUSHO:
			Options_Push(&state->options);
			break;

		case STATEMENT_TYPE_POPO:
			Options_Pop(&state->options);
			break;

		case STATEMENT_TYPE_OPT:
		{
			const IdentifierListNode *option;

			for (option = statement->shared.opt.options.head; option != NULL; option = option->next)
			{
				/* TODO: A lot of these are unimplemented... */
				if (String_CompareCStrCaseInsensitive(&option->identifier, "ae+"))
					Options_Get(&state->options)->automatic_even = cc_true;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "ae-"))
					Options_Get(&state->options)->automatic_even = cc_false;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "an+"))
					;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "an-"))
					;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "c+"))
					Options_Get(&state->options)->case_insensitive = cc_false;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "c-"))
					Options_Get(&state->options)->case_insensitive = cc_true;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "d+"))
					Options_Get(&state->options)->equ_set_descope_local_labels = cc_true;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "d-"))
					Options_Get(&state->options)->equ_set_descope_local_labels = cc_false;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "e+"))
					;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "e-"))
					;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "l+"))
					Options_Get(&state->options)->local_signifier = '.';
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "l-"))
					Options_Get(&state->options)->local_signifier = '@';
				else if (String_Length(&option->identifier) == 2 && (String_At(&option->identifier, 0) == 'l' || String_At(&option->identifier, 0) == 'L'))
					Options_Get(&state->options)->local_signifier = String_At(&option->identifier, 1);
				/*else if (String_CompareCStrCaseInsensitive(&option->identifier, "s+"))
					;*/
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "s-"))
					;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "m+"))
					Options_Get(&state->options)->expand_all_macros = cc_true;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "m-"))
					Options_Get(&state->options)->expand_all_macros = cc_false;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "v+"))
					Options_Get(&state->options)->output_local_labels_to_sym_file = cc_true;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "v-"))
					Options_Get(&state->options)->output_local_labels_to_sym_file = cc_false;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "w+"))
					Options_Get(&state->options)->warnings_enabled = cc_true;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "w-"))
					Options_Get(&state->options)->warnings_enabled = cc_false;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "wp+"))
					Options_Get(&state->options)->pedantic_warnings_enabled = cc_true;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "wp-"))
					Options_Get(&state->options)->pedantic_warnings_enabled = cc_false;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "ws+"))
					;
				else if (String_CompareCStrCaseInsensitive(&option->identifier, "ws-"))
					;
				else
					SemanticWarning(state, "Unrecognised option '%s'.", String_CStr(&option->identifier));
			}			

			break;
		}

		case STATEMENT_TYPE_PUSHP:
			if (!StringStack_Push(&state->string_stack, String_View(&statement->shared.string)))
				InternalError(state, "Failed to allocate memory for the string stack.");

			break;

		case STATEMENT_TYPE_POPP:
		{
			String string;

			if (!StringStack_Pop(&state->string_stack, &string))
				SemanticError(state, "POPO used but the string stack was empty.");
			else
				PushSubstitute(state, String_View(&statement->shared.string), String_View(&string));

			break;
		}

		case STATEMENT_TYPE_SHIFT:
			if (!CurrentlyExpandingMacro(state))
			{
				SemanticError(state, "SHIFT used outside of macro.");
			}
			else
			{
				/* TODO: What if there are already 0 arguments? */
				--state->macro.total_arguments;
				String_Destroy(&state->macro.argument_list[0]);
				memmove(state->macro.argument_list, state->macro.argument_list + 1, sizeof(*state->macro.argument_list) * state->macro.total_arguments);
				PushMacroArgumentSubstitutions(state);
			}

			break;

		case STATEMENT_TYPE_MEXIT:
			state->macro.active = cc_false;
			break;

		case STATEMENT_TYPE_LOCAL:
			if (!CurrentlyExpandingMacro(state))
			{
				SemanticError(state, "LOCAL used outside of macro.");
			}
			else
			{
				const IdentifierListNode *list_entry;

				for (list_entry = statement->shared.local.identifiers.head; list_entry != NULL; list_entry = list_entry->next)
					if (!Dictionary_LookUpAndCreateIfNotExist(state->macro.dictionary, String_View(&list_entry->identifier), NULL))
						OutOfMemoryError(state);
			}

			break;

		case STATEMENT_TYPE_PURGE:
			PurgeMacro(state, String_View(&statement->shared.string), cc_false);
			break;
	}

	/* Update both copies of the program counter again, so that things like WHILE statements don't use stale values in their expressions. */
	state->program_counter_of_statement = state->program_counter;
	state->program_counter_of_expression = state->program_counter;
}

static cc_bool ParseStatement(SemanticState* const state, Statement* const statement, const StringView* const view)
{
	/* Parse the source line with Flex and Bison (Lex and Yacc). */
	const YY_BUFFER_STATE buffer = m68kasm__scan_bytes(StringView_Data(view), StringView_Length(view), state->flex_state);
	const int parse_result = m68kasm_parse(state->flex_state, statement);
	m68kasm__delete_buffer(buffer, state->flex_state);

	/* Out of memory. */
	if (parse_result == 2)
		OutOfMemoryError(state);

	return parse_result == 0;
}

static void ParseLine(SemanticState* const state, const StringView* const label, const StringView* const directive_and_operands)
{
	/* This is a normal assembly line. */
	Statement statement;

	/* Back these up, before they get a chance to be modified by ProcessStatement. */
	const unsigned long starting_program_counter = state->program_counter;
	const size_t starting_output_position = state->output_position;
	const size_t starting_segment_length = state->segment_length;

	if (!ParseStatement(state, &statement, directive_and_operands))
	{
		/* An error message will have already been printed, so we don't need to do one here. */
		state->success = cc_false;
	}
	else
	{
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
				fix_up->macro_dictionary = (Dictionary_State*)SharedMemory_Copy(state->macro.dictionary);
				fix_up->program_counter = starting_program_counter;
				fix_up->output_position = starting_output_position;
				fix_up->segment_length = starting_segment_length;
				String_CreateCopy(&fix_up->last_global_label, &state->last_global_label);
				String_CreateCopy(&fix_up->source_line, state->source_line);
				String_CreateCopyView(&fix_up->label, label);
				fix_up->options = *Options_Get(&state->options);

				/* Clone the location. */
				*destination_location = *source_location;
				String_CreateCopy(&destination_location->file_path, &source_location->file_path);

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
						String_CreateCopy(&destination_location->file_path, &source_location->file_path);
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
	}
}

#define DIRECTIVE_OR_MACRO_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?_.@"
#define DIRECTIVE_OR_MACRO_CHARS_NO_DOT "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?_@"
#define LABEL_CHARS DIRECTIVE_OR_MACRO_CHARS

/* TODO: When I switch to C++, make this the first thing to go... */
typedef struct MacroCustomSubstituteSearch_Closure
{
	struct SemanticState *state;

	StringView size;
	StringView arguments;
	StringView label;
	String unique_suffix;
	String symbol_value_string;
} MacroCustomSubstituteSearch_Closure;

static const StringView* MacroCustomSubstituteSearch(void* const user_data, const StringView *view_to_search, size_t starting_position, cc_bool case_insensitive, size_t* const found_position, size_t* const found_length)
{
	/* `view_to_search` always points to a null-terminated string. */
	MacroCustomSubstituteSearch_Closure* const closure = (MacroCustomSubstituteSearch_Closure*)user_data;
	SemanticState* const state = closure->state;

	(void)case_insensitive;

	for (*found_position = starting_position; (*found_position = StringView_FindCharacter(view_to_search, '\\', *found_position)) != STRING_POSITION_INVALID; ++(*found_position))
	{
		const char symbol = StringView_At(view_to_search, *found_position + 1);

		*found_length = 2;

		switch (symbol)
		{
			case '0':
				return &closure->size;

			case '_':
				if (Substitute_IsSubstituteBlockingCharacter(StringView_At(view_to_search, *found_position + *found_length)))
					continue;

				return &closure->arguments;

			case '*':
				return &closure->label;

			case '@':
				return String_View(&closure->unique_suffix);

			case '#':
			case '$':
			{
				unsigned long value;
				const char* const identifier_start = &StringView_At(view_to_search, *found_position + 2);
				const size_t identifier_length = strspn(identifier_start, DIRECTIVE_OR_MACRO_CHARS);
				StringView identifier;

				StringView_Create(&identifier, identifier_start, identifier_length);

				*found_length += identifier_length;
				/* Absorb trailing backslash. */
				if (identifier_start[identifier_length] == '\\')
					++*found_length;

				String_Destroy(&closure->symbol_value_string);

				if (!GetSymbolInteger(state, &identifier, cc_true, &value))
					value = 0;

				if (symbol == '#')
					closure->symbol_value_string = DecimalIntegerToString(value);
				else /*if (symbol == '$')*/
					closure->symbol_value_string = HexadecimalIntegerToString(value);

				return String_View(&closure->symbol_value_string);
			}

			default:
			{
				char *end;

				/* Obtain numerical index of the parameter. */
				const char* const start = &StringView_At(view_to_search, *found_position + 1);
				const unsigned long parameter_index = strtoul(start, &end, 10) - 1;

				/* Check if conversion failed. */
				if (end == start)
					continue;

				*found_length = 1 + (end - start);

				if (parameter_index >= state->macro.total_arguments)
				{
					static const StringView blank_argument = STRING_VIEW_INITIALISER_BLANK;
					return &blank_argument;
				}

				return String_View(&state->macro.argument_list[parameter_index]);
			}
		}
	}

	return NULL;
}

static void PerformSubstitutionsOnSubString(SemanticState* const state, String* const string, StringView* const view_to_search, const cc_bool allow_implicit_matches)
{
	const cc_bool case_insensitive = Options_Get(&state->options)->case_insensitive;

	if (state->macro.closure != NULL)
		Substitute_ProcessSubString(&state->substitutions, &state->macro.substitutions, string, view_to_search, MacroCustomSubstituteSearch, state->macro.closure, allow_implicit_matches, case_insensitive);
	else
		Substitute_ProcessSubString(&state->substitutions, NULL, string, view_to_search, NULL, NULL, allow_implicit_matches, case_insensitive);
}

static void PerformSubstitutions(SemanticState* const state, String* const string, const cc_bool allow_implicit_matches)
{
	StringView view = *String_View(string);
	PerformSubstitutionsOnSubString(state, string, &view, allow_implicit_matches);
}

static cc_bool FindStringInSourceLine(const String* const string, const size_t starting_position, StringView* const view)
{
	char *start_pointer, *end_pointer;

	start_pointer = strpbrk(&String_At(string, starting_position), "'\"");

	if (start_pointer == NULL)
		return cc_false;

	end_pointer = strchr(start_pointer + 1, *start_pointer);

	if (end_pointer == NULL)
		return cc_false;

	StringView_Create(view, start_pointer, end_pointer - start_pointer + 1);
	return cc_true;
}

/* Performs string substitutions only in parts of the string that are not wrapped in quotations. */
/* This is needed to simulate how SN 68k does not expand string constants in strings, only expressions and operands. */
static void PerformSubstitutionsExcludingQuotedStrings(SemanticState* const state, String* const string, const size_t starting_position, const cc_bool allow_implicit_matches)
{
	cc_bool string_in_source_line_found;
	size_t position = starting_position;

	do
	{
		StringView string_in_source_line;

		string_in_source_line_found = FindStringInSourceLine(string, position, &string_in_source_line);

		/* Pretend there's a string at the end of the line, so that we can repurpose the code below. */
		if (!string_in_source_line_found)
			StringView_Create(&string_in_source_line, String_Data(string) + String_Length(string), 0);

		/* We've found a string! Perform substitutions on the part of the line before it. */
		{
			StringView view_to_search;
			/* Define the region that we will perform substitutions on. */
			const char* const search_start = &String_At(string, position);
			const size_t search_length = StringView_Data(&string_in_source_line) - search_start;

			/* Perform the substitutions. */
			StringView_Create(&view_to_search, search_start, search_length);
			PerformSubstitutionsOnSubString(state, string, &view_to_search, allow_implicit_matches);

			/* Move to after the string so that we can continue searching. */
			position += StringView_Length(&view_to_search) + StringView_Length(&string_in_source_line);
		}
	}
	while (string_in_source_line_found);
}

static void SubstituteAndParseLine(SemanticState *state, const StringView* const label, const StringView* const directive_and_operands, const StringView* const directive)
{
	String modified_directive_and_operands;

	/* Because of the below hack, we have to filter-out directives that do not use expressions or operands,
	   to prevent them from accidentally being substituted. This is needed for the Natsumi Z80 macros. */
	if (StringView_CompareCStrCaseInsensitive(directive, "binclude")
	 || StringView_CompareCStrCaseInsensitive(directive, "equs")
	 || StringView_CompareCStrCaseInsensitive(directive, "incbin")
	 || StringView_CompareCStrCaseInsensitive(directive, "include")
	 || StringView_CompareCStrCaseInsensitive(directive, "macro")
	 || StringView_CompareCStrCaseInsensitive(directive, "macros")
	 || StringView_CompareCStrCaseInsensitive(directive, "opt")
	 || StringView_CompareCStrCaseInsensitive(directive, "popp"))
	{
		ParseLine(state, label, directive_and_operands);
		return;
	}

	/* Perform implicit (backslash-less) string substitutions on the operands.
	   SN 68k achieves this by expanding string constants in its operand and expression evaluators,
	   which is absolutely insane, so this trick approximates it instead. */
	String_CreateCopyView(&modified_directive_and_operands, directive_and_operands);
	PerformSubstitutionsExcludingQuotedStrings(state, &modified_directive_and_operands, StringView_Length(directive), cc_true);

	ParseLine(state, label, String_View(&modified_directive_and_operands));

	String_Destroy(&modified_directive_and_operands);
}

static void InvokeMacro(SemanticState* const state, Macro* const macro, const StringView* const label, const char *source_line_pointer)
{
	/* This is a macro invocation. */
	MacroCustomSubstituteSearch_Closure closure;

	const SemanticState_Macro previous_macro_state = state->macro;

	state->macro.metadata = macro;
	state->macro.dictionary = (Dictionary_State*)SharedMemory_Allocate(sizeof(Dictionary_State));

	if (state->macro.dictionary == NULL)
	{
		OutOfMemoryError(state);
		return;
	}

	/* TODO: We should not be caching 'case_insensitive'; the OPT directive can change this at any time! */
	Dictionary_Init(state->macro.dictionary, Options_Get(&state->options)->case_insensitive);
	Substitute_Initialise(&state->macro.substitutions);
	state->macro.closure = &closure;
	state->macro.argument_list = NULL;
	state->macro.total_arguments = 0;
	state->macro.total_arguments_at_start = 0;
	state->macro.starting_if_level = state->current_if_level;
	state->macro.active = cc_true;

	++state->current_macro_invocation;

	closure.state = state;

	state->suppress_listing = cc_true;

	/* Skip past the macro name, but stop at '.' so we can extract the size specifier. */
	source_line_pointer += strspn(source_line_pointer, DIRECTIVE_OR_MACRO_CHARS_NO_DOT);

	if (!StringView_Empty(label))
	{
		SetLastGlobalLabel(state, label);

		if (!macro->uses_label)
			AddIdentifierToSymbolTable(state, label, state->program_counter, SYMBOL_LABEL);
	}

	/* Extract and store the macro size specifier, if one exists. */
	if (source_line_pointer[0] == '.')
	{
		/* Skip the '.' character. */
		++source_line_pointer;

		StringView_Create(&closure.size, source_line_pointer, strspn(source_line_pointer, DIRECTIVE_OR_MACRO_CHARS));

		/* Advance past the size specifier. */
		source_line_pointer += StringView_Length(&closure.size);
	}
	else
	{
		StringView_CreateBlank(&closure.size);
	}

	/* Extract combined arguments string. */
	{
		const char* const arguments_string_start = source_line_pointer + strspn(source_line_pointer, " \t");
		const char *arguments_string_end = strchr(arguments_string_start, ';');

		if (arguments_string_end == NULL)
			arguments_string_end = String_Data(state->source_line) + String_Length(state->source_line);

		/* Remove trailing whitespace. */
		while (arguments_string_end > arguments_string_start && (arguments_string_end[-1] == ' ' || arguments_string_end[-1] == '\t'))
			--arguments_string_end;

		StringView_Create(&closure.arguments, arguments_string_start, arguments_string_end - arguments_string_start);
	}

	closure.label = *label;
	closure.unique_suffix = ComputeUniqueMacroSuffix(state);
	String_CreateBlank(&closure.symbol_value_string);

	/* Extract and store the individual macro arguments, if they exist. */
	{
		char character;

		do
		{
			const char* const argument_string_start = source_line_pointer += strspn(source_line_pointer, " \t");

			do
			{
				character = *source_line_pointer++;

				/* If commas appear between parentheses, then we shouldn't separate on them. */
				/* This is because of the possibility of code such as '(a0,d0.w)' being passed as an argument. */
				/* To do this, manually skip characters until we find a closing parenthesis. */
				if (character == '(')
				{
					unsigned int argument_depth = 1;

					while (argument_depth != 0)
					{
						character = *source_line_pointer++;

						if (character == '(')
							++argument_depth;
						else if (character == ')')
							--argument_depth;
						else if (character == ';' || character == '\0')
							break;
					}
				}

				/* If we encounter a comma, a comment, or the end of the line, then split this off as a macro argument. */
				if (character == ',' || character == ';' || character == '\0')
				{
					/* Extract argument. */
					StringView argument;
					StringView_Create(&argument, argument_string_start, source_line_pointer - argument_string_start - 1);

					/* Remove trailing whitespace. */
					while (StringView_Length(&argument) != 0)
					{
						const char character = StringView_Back(&argument);

						if (character != ' ' && character != '\t')
							break;

						StringView_SubStr(&argument, &argument, 0, StringView_Length(&argument) - 1);
					}

					/* Do not bother pushing an empty first argument if it is the only argument, as that just means there are no arguments. */
					if (StringView_Length(&argument) == 0 && (character == ';' || character == '\0') && state->macro.total_arguments == 0)
						break;

					/* Add to argument list. */
					{
						String* const new_argument_list = (String*)realloc(state->macro.argument_list, sizeof(*state->macro.argument_list) * (state->macro.total_arguments + 1));

						if (new_argument_list != NULL)
						{
							state->macro.argument_list = new_argument_list;
							String_CreateCopyView(&state->macro.argument_list[state->macro.total_arguments], &argument);

							if (StringView_Length(&argument) != 0 && StringView_At(&argument, 0) != '"' && StringView_At(&argument, 0) != '\'' && Options_Get(&state->options)->case_insensitive)
								String_ToLower(&state->macro.argument_list[state->macro.total_arguments]);

							++state->macro.total_arguments;
							++state->macro.total_arguments_at_start;
						}
					}

					break;
				}
			} while (character != ';' && character != '\0');
		} while (character != ';' && character != '\0');

		/* Add identifier substitute. */
		PushMacroArgumentSubstitutions(state);
	}

	/* Finally, invoke the macro. */
	{
		Location location;
		const SourceLineListNode *source_line_list_node;

		const cc_bool previous_listing_macro = state->listing_macro;

		/* Flush line before changing this state, so that the macro line is not given the 'M' label. */
		ListSourceLine(state);
		state->listing_macro = cc_true;

		/* Push a new location (this macro).*/
		String_CreateCopy(&location.file_path, &macro->name);
		location.line_number = 0;

		location.previous = state->location;
		state->location = &location;

		/* Iterate over each line of the macro, sending it to be processed. */
		for (source_line_list_node = macro->source_line_list_head; source_line_list_node != NULL && CurrentlyExpandingMacro(state); source_line_list_node = source_line_list_node->next)
		{
			/* TODO: This is redundant and wrong: these substitutions are already done later, and this should only be doing the macro arguments! */
			if (macro->is_short)
			{
				String source_line;
				String_CreateCopy(&source_line, &source_line_list_node->source_line_buffer);
				PerformSubstitutions(state, &source_line, cc_false);
				AssembleLine(state, &source_line, Options_Get(&state->options)->expand_all_macros);
			}
			else
			{
				AssembleLine(state, &source_line_list_node->source_line_buffer, Options_Get(&state->options)->expand_all_macros);
			}
		}

		if (!macro->is_short)
		{
			/* 'MEXIT' may have ended us midway through an if-statement, so unwind to the original if-level here. */
			while (state->current_if_level != state->macro.starting_if_level)
				TerminateIf(state);
		}

		/* Pop location. */
		state->location = state->location->previous;

		String_Destroy(&location.file_path);

		state->listing_macro = previous_listing_macro;
	}

	String_Destroy(&closure.symbol_value_string);

	Substitute_Deinitialise(&state->macro.substitutions);
	if (SharedMemory_WillBeDestroyed(state->macro.dictionary))
		Dictionary_Deinit(state->macro.dictionary);
	SharedMemory_Free(state->macro.dictionary);

	/* Destroy argument list. */
	{
		size_t i;

		for (i = 0; i < state->macro.total_arguments; ++i)
			String_Destroy(&state->macro.argument_list[0]);

		free(state->macro.argument_list);
	}

	state->macro = previous_macro_state;
}

static void AssembleLine(SemanticState *state, const String *source_line_raw, const cc_bool write_line_to_listing_file)
{
	String source_line;
	size_t label_length;
	const char *source_line_pointer;
	StringView label;
	size_t directive_length;
	StringView directive_and_operands, directive;

	const String* const old_source_line = state->source_line;

	if (write_line_to_listing_file)
	{
		ListSourceLine(state);

		/* Output program counter to listing file. */
		if (TextOutput_exists(state->listing_callbacks))
		{
			state->line_listed = cc_false;

			state->listing_counter = 0;
			state->suppress_listing = cc_false;
			TextOutput_fprintf(state->listing_callbacks, "%08lX", state->program_counter);
		}
	}

	/* Expand string constants and macro parameters only if we're actually parsing code;
	   it is important that the input of REPT and WHILE is unaltered, so that each iteration
	   can have unique expansions! */
	if (state->mode == MODE_NORMAL)
	{
		String_CreateCopy(&source_line, source_line_raw);
		PerformSubstitutions(state, &source_line, cc_false);
		state->source_line = &source_line;
	}
	else
	{
		state->source_line = source_line_raw;
	}

	++state->location->line_number;

	if (String_At(state->source_line, 0) == '*')
	{
		/* This whole line is a comment. */
		return;
	}

	source_line_pointer = String_CStr(state->source_line);

	/* Despite the fact that we're using Flex and Bison to parse the
	   language for us, we unfortunately have to do quite a bit of
	   parsing manually because of inconsistent whitespace sensitivity
	   (label recognision) and manipulation of the input string
	   (macro expansion). */

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

	/* Duplicate the label (if any) for later. */
	StringView_Create(&label, source_line_pointer, label_length);

	if (label_length != 0)
	{
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

	StringView_Create(&directive_and_operands, source_line_pointer, (String_Data(state->source_line) + String_Length(state->source_line)) - source_line_pointer);
	StringView_SubStr(&directive, &directive_and_operands, 0, directive_length);

	/* This is either a directive, or a macro. */
	{
		/* For macro lookup, we need to strip any size suffix (.b, .w, .l, .s) from the directive name.
		   This allows macros to be invoked with size specifiers, e.g., "MyMacro.w arg1,arg2". */
		StringView directive_for_lookup;
		size_t directive_for_lookup_length = directive_length;

		/* Check if directive ends with a size suffix (.b, .w, .l, or .s) */
		if (directive_length >= 2 && source_line_pointer[directive_for_lookup_length - 2] == '.')
		{
			const char size_char = source_line_pointer[directive_for_lookup_length - 1] | 0x20; /* Convert to lowercase */
			if (size_char == 'b' || size_char == 'w' || size_char == 'l' || size_char == 's')
				directive_for_lookup_length -= 2;
		}

		StringView_SubStr(&directive_for_lookup, &directive_and_operands, 0, directive_for_lookup_length);

		/* Look up the directive in the dictionary to see if it's actually a macro. */
		const Dictionary_Entry* const macro_dictionary_entry = LookupSymbol(state, &directive_for_lookup, NULL);
		Macro* const macro = macro_dictionary_entry != NULL && macro_dictionary_entry->type == SYMBOL_MACRO ? (Macro*)macro_dictionary_entry->shared.pointer : NULL;

		if (macro != NULL && macro->is_short)
		{
			/* Short macros are always expanded, in case they're hiding an 'if' or an 'endr' or some other special directive; */
			/* the alternate modes explicitly search for these directives, so they must be visible. */
			InvokeMacro(state, macro, &label, source_line_pointer);
		}
		else
		{
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
						/* TODO - Detect code after the keyword and error if any is found. */
						if (StringView_CompareCStrCaseInsensitive(&directive, "if"))
						{
							/* If-statements that are nested within the false part of another if-statement
							   are themselves false, so create a false if-statement here and process it. */
							++state->current_if_level;
						}
						else if (StringView_CompareCStrCaseInsensitive(&directive, "elseif")
						      || StringView_CompareCStrCaseInsensitive(&directive, "else"  )
						      || StringView_CompareCStrCaseInsensitive(&directive, "endc"  )
						      || StringView_CompareCStrCaseInsensitive(&directive, "endif" ))
						{
							/* These can be processed normally too. */
							SubstituteAndParseLine(state, &label, &directive_and_operands, &directive);
						}
						else
						{
							/* Drop the line completely, since it's inside the false half of an if statement and should be ignored. */
						}
					}
					else
					{
						if (macro != NULL /*&& !macro->is_short*/)
						{
							/* Regular macros are only expanded when needed. */
							InvokeMacro(state, macro, &label, source_line_pointer);
						}
						else
						{
							/* This is not a macro invocation: it's just a regular line that can be assembled as-is. */
							SubstituteAndParseLine(state, &label, &directive_and_operands, &directive);
						}
					}
				}

				break;

			case MODE_REPT:
				/* If this line is an 'ENDR' directive, then exit REPT mode. Otherwise, add the line to the REPT. */
				if (StringView_CompareCStrCaseInsensitive(&directive, "endr") && state->shared.rept.nesting-- == 0)
				{
					/* TODO - Detect code after the keyword and error if any is found. */
					ParseLine(state, &label, &directive_and_operands);
				}
				else
				{
					if (StringView_CompareCStrCaseInsensitive(&directive, "rept"))
						++state->shared.rept.nesting;

					AddToSourceLineList(state, &state->shared.rept.source_line_list, state->source_line);
				}

				break;

			case MODE_MACRO:
				/* If this line is an 'ENDM' directive, then exit macro mode. Otherwise, add the line to the macro. */
				if (StringView_CompareCStrCaseInsensitive(&directive, "endm"))
				{
					/* TODO - Detect code after the keyword and error if any is found. */
					ParseLine(state, &label, &directive_and_operands);

					if (state->shared.macro.is_short)
						SemanticError(state, "Short macros shouldn't use ENDM.");
				}
				else
				{
					if (state->shared.macro.is_short)
					{
						/* Short macros automatically terminate after one statement. */
						const char first_nonwhitespace_character = String_At(state->source_line, strspn(String_CStr(state->source_line), " \t"));

						if (!StringView_Empty(&label))
							SemanticError(state, "Short macros shouldn't create labels.");

						if (first_nonwhitespace_character == '\0' || first_nonwhitespace_character == ';')
						{
							/* This line is an empty statement: ignore it. */
						}
						else
						{
							AddToSourceLineList(state, &state->shared.macro.source_line_list, state->source_line);
							TerminateMacro(state);
						}
					}
					else
					{
						AddToSourceLineList(state, &state->shared.macro.source_line_list, state->source_line);
					}
				}

				break;

			case MODE_WHILE:
				/* If this line is an 'ENDW' directive, then exit 'WHILE' mode. Otherwise, add the line to the 'WHILE'. */
				if (StringView_CompareCStrCaseInsensitive(&directive, "endw") && state->shared.while_statement.nesting-- == 0)
				{
					/* TODO - Detect code after the keyword and error if any is found. */
					ParseLine(state, &label, &directive_and_operands);
				}
				else
				{
					if (StringView_CompareCStrCaseInsensitive(&directive, "while"))
						++state->shared.while_statement.nesting;

					AddToSourceLineList(state, &state->shared.while_statement.source_line_list, state->source_line);
				}

				break;
			}
		}
	}

	if (write_line_to_listing_file)
		ListSourceLine(state);

	if (state->source_line == &source_line)
		String_Destroy(&source_line);

	state->source_line = old_source_line;
}

static void AssembleFile(SemanticState *state)
{
	/* Read lines one at a time, feeding them to the 'AssembleLine' function. */
	while (!state->end && ReadSourceLine(state))
		AssembleLine(state, &state->line_buffer, cc_true);

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

static cc_bool DictionaryFilterProduceSymbolFile(Dictionary_Entry *entry, const char *identifier, size_t identifier_length, void *user_data)
{
	/* The symbol file only contains labels. */
	/* Labels longer than 0xFF characters are silently ignored. */
	/* Local labels are also silently ignored (unless 'v+' is specified). */
	void** const parameters = (void**)user_data;
	const SemanticState* const state = (const SemanticState*)parameters[1];

	if (entry->type == SYMBOL_LABEL)
	{
		const char *label;
		size_t label_length;
		cc_bool is_local_label;

		for (label = &identifier[identifier_length], label_length = 0; label_length != identifier_length; --label, ++label_length)
			if (*label == Options_Get(&state->options)->local_signifier)
				break;

		is_local_label = label_length != identifier_length;

		if (label_length < 0x100 && (!is_local_label || Options_Get(&state->options)->output_local_labels_to_sym_file))
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

static cc_bool InitialiseBuiltInVariable(SemanticState* const state, const StringView* const identifier)
{
	Dictionary_Entry* const symbol = CreateSymbol(state, identifier);

	if (symbol == NULL)
		return cc_false;

	symbol->type = SYMBOL_SPECIAL;
	symbol->shared.pointer = (void*)identifier;
	return cc_true;
}

static void AddDefinition(void* const internal, const char* const identifier_buffer, const size_t identifier_length, const unsigned long value)
{
	SemanticState* const state = (SemanticState*)internal;

	StringView identifier;
	StringView_Create(&identifier, identifier_buffer, identifier_length);

	{
		Dictionary_Entry* const symbol = CreateSymbol(state, &identifier);

		if (symbol != NULL)
		{
			/* TODO: Does asm68k create a constant or a variable? */
			symbol->type = SYMBOL_CONSTANT;
			symbol->shared.unsigned_long = value;
		}
	}
}

static cc_bool ClownAssembler_AssembleToObjectFile(
	const TextInput* const input_callbacks,
	const BinaryOutput* const output_callbacks,
	const TextOutput* const error_callbacks,
	const TextOutput* const listing_callbacks,
	const BinaryOutput* const symbol_callbacks,
	const char* const input_file_path,
	const ClownAssembler_Settings* const initial_options,
	const ClownAssembler_DefinitionCallback definition_callback,
	const void* const user_data)
{
	Location location;
	SemanticState state = {0}; /* Default-initialise everything. */

	/* For error messages, we want to maintain a list of filenames and line
	   numbers that describe the location and nesting of the erroneous
	   line. This is the first entry in that list. */
	location.previous = NULL;
	String_Create(&location.file_path, input_file_path, strlen(input_file_path));
	location.line_number = 0;

	/* Initialise the state's non-default values. */
	state.success = cc_true;

	state.input_callbacks = input_callbacks;
	state.output_callbacks = output_callbacks;
	state.error_callbacks = error_callbacks;
	state.listing_callbacks = listing_callbacks;

	Options_Initialise(&state.options, initial_options);

	String_CreateBlank(&state.last_global_label);
	String_CreateBlank(&state.line_buffer);
	state.location = &location;
	Substitute_Initialise(&state.substitutions);
	StringStack_Initialise(&state.string_stack);
	state.mode = MODE_NORMAL;

	/* Create the symbol table dictionary. */
	if (!Dictionary_Init(&state.dictionary, Options_Get(&state.options)->case_insensitive))
	{
		OutOfMemoryError(&state);
	}
	else
	{
		if (InitialiseBuiltInVariable(&state, &string_rs)
		 && InitialiseBuiltInVariable(&state, &string_narg))
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
				if (Options_Get(&state.options)->debug)
					m68kasm_debug = 1;
			#endif

				/* Object file magic number. */
				OutputWriteRawByte(&state, 0x89);
				OutputWriteRawByte(&state, 0x14);

				/* Begin first segment. */
				WriteRecordSegment(&state);

				/* Perform first pass of assembly, creating a list of fix-ups. */
				AssembleFile(&state);

				/* Finalise the segment record data. */
				TerminatePreviousRecordSegment(&state);

				/* Write terminating record. */
				OutputWriteRawByte(&state, 0x00);
				BinaryOutput_fwrite(StringView_Data(&string_assembler_identifier), 1, StringView_Length(&string_assembler_identifier), output_callbacks);

				/* Destroy the lexer, as we no longer need it. */
				if (m68kasm_lex_destroy(state.flex_state) != 0)
					InternalError(&state, "m68kasm_lex_destroy failed.");

				/* Perform some sanity checks to make sure we're not somehow in an invalid state. */
				if (state.current_if_level != 0)
					SemanticError(&state, "An IF statement somewhere is missing its ENDC/ENDIF.");
				if (state.obj_active)
					SemanticError(&state, "An OBJ statement somewhere is missing its OBJEND.");

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
						state.macro.dictionary = fix_up->macro_dictionary;
						state.program_counter = fix_up->program_counter;
						OutputSeek(&state, fix_up->output_position);
						state.segment_length = fix_up->segment_length;
						String_Destroy(&state.last_global_label);
						String_CreateCopy(&state.last_global_label, &fix_up->last_global_label);
						state.source_line = &fix_up->source_line;
						state.location = &fix_up->location;
						*Options_Get(&state.options) = fix_up->options;

						state.fix_up_needed = cc_false;

						/* Re-process statement. */
						ProcessStatement(&state, &fix_up->statement, String_View(&fix_up->label));

						/* If this fix-up has been fixed, then we are done with it and free to delete it.
						   Alternatively, if this is the final pass, then just delete the fix-ups anyway
						   since they won't be needed anymore. */
						if (!state.fix_up_needed || state.doing_final_pass)
						{
							Location *location;

							*fix_up_pointer = fix_up->next;

							/* We're done with this statement: delete it. */
							DestroyStatement(&fix_up->statement);
							if (SharedMemory_WillBeDestroyed(fix_up->macro_dictionary))
								Dictionary_Deinit(fix_up->macro_dictionary);
							SharedMemory_Free(fix_up->macro_dictionary);
							String_Destroy(&fix_up->last_global_label);
							String_Destroy(&fix_up->source_line);
							String_Destroy(&fix_up->label);

							/* Pop one location from the list. */
							location = fix_up->location.previous;

							while (location != NULL)
							{
								Location *previous_location = location->previous;
								String_Destroy(&location->file_path);
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

				/* Produce asm68k symbol file, if requested. */
				if (BinaryOutput_exists(symbol_callbacks))
				{
					const void *parameters[2];

					parameters[0] = symbol_callbacks;
					parameters[1] = &state;

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

	StringStack_Deinitialise(&state.string_stack);
	Substitute_Deinitialise(&state.substitutions);
	String_Destroy(&state.line_buffer);
	String_Destroy(&state.last_global_label);

	Options_Deinitialise(&state.options);

	return state.success;
}

cc_bool ClownAssembler_Assemble(
	const TextInput* const input_callbacks,
	const BinaryOutput* const output_callbacks,
	const TextOutput* const error_callbacks,
	const TextOutput* const listing_callbacks,
	const BinaryOutput* const symbol_callbacks,
	const char* const input_file_path,
	const ClownAssembler_Settings* const initial_options,
	const ClownAssembler_DefinitionCallback definition_callback,
	const void* const user_data)
{
	cc_bool success = cc_false;

	ClownAssembler_BinaryOutput temporary_callbacks;

	FILE* const temporary_file = tmpfile();

	if (temporary_file == NULL)
	{
		TextOutput_fputs("Internal error: Could not open temporary file.\n", error_callbacks);
	}
	else
	{
		temporary_callbacks.user_data = temporary_file;
		temporary_callbacks.write_character = WriteCharacter;
		temporary_callbacks.write_characters = WriteCharacters;
		temporary_callbacks.seek = Seek;

		if (ClownAssembler_AssembleToObjectFile(input_callbacks, &temporary_callbacks, error_callbacks, listing_callbacks, symbol_callbacks, input_file_path, initial_options, definition_callback, user_data))
		{
			rewind(temporary_file);

			if (ConvertObjectFileToFlatBinary(temporary_file, output_callbacks, error_callbacks))
				success = cc_true;
		}

		fclose(temporary_file);
	}

	return success;
}

cc_bool ClownAssembler_AssembleFile(
	FILE* const input_file,
	FILE* const output_file,
	FILE* const error_file,
	FILE* const listing_file,
	FILE* const symbol_file,
	const char* const input_file_path,
	const ClownAssembler_Settings* const settings,
	const ClownAssembler_DefinitionCallback definition_callback,
	const void* const user_data)
{
	ClownAssembler_TextInput input_callbacks;
	ClownAssembler_BinaryOutput output_callbacks;
	ClownAssembler_TextOutput error_callbacks;
	ClownAssembler_TextOutput listing_callbacks;
	ClownAssembler_BinaryOutput symbol_callbacks;

	input_callbacks.user_data = input_file;
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

	return ClownAssembler_Assemble(&input_callbacks, &output_callbacks, &error_callbacks, &listing_callbacks, &symbol_callbacks, input_file_path, settings, definition_callback, user_data);
}
