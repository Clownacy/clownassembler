#include "semantic.h"

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clowncommon.h"

#include "dictionary.h"
#include "syntactic.h"
#define YY_NO_UNISTD_H
#include "lexical.h"

#define PROGRAM_COUNTER ",,PROGRAM_COUNTER,,"

typedef enum SymbolType
{
	SYMBOL_CONSTANT,
	SYMBOL_VARIABLE,
	SYMBOL_MACRO
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
	long output_position;
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

typedef struct SemanticState
{
	cc_bool success;
	FILE *output_file;
	FILE *listing_file;
	unsigned long program_counter;
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
	cc_bool end;
	enum
	{
		MODE_NORMAL,
		MODE_REPT,
		MODE_MACRO
	} mode;
	struct
	{
		unsigned long total_repeats;
		unsigned long line_number;
		SourceLineList source_line_list;
	} rept;
	struct
	{
		char *name;
		unsigned long line_number;
		IdentifierListNode *parameter_names;
		SourceLineList source_line_list;
	} macro;
} SemanticState;

typedef struct Macro
{
	char *name;
	IdentifierListNode *parameter_names;
	SourceLineListNode *source_line_list_head;
} Macro;

/* Some forward declarations that are needed because some functions recurse into each other. */
static void AssembleFile(SemanticState *state, FILE *input_file);
static void AssembleLine(SemanticState *state, const char *source_line);

/* Prevent errors when __attribute__ is not supported. */
#ifndef __GNUC__
#define  __attribute__(x)
#endif

static void ErrorMessageCommon(SemanticState *state)
{
	const Location *location;

	for (location = state->location; location != NULL; location = location->previous)
		fprintf(stderr, "\nOn line %lu of '%s'...", location->line_number, location->file_path);

	fprintf(stderr, "\n%s\n\n", state->source_line);
}

__attribute__((format(printf, 2, 3))) static void SemanticWarning(SemanticState *state, const char *fmt, ...)
{
	va_list args;

	fputs("Warning: ", stderr);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	ErrorMessageCommon(state);
}

__attribute__((format(printf, 2, 3))) static void SemanticError(SemanticState *state, const char *fmt, ...)
{
	va_list args;

	fputs("Error: ", stderr);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	ErrorMessageCommon(state);

	state->success = cc_false;
}

__attribute__((format(printf, 2, 3))) static void InternalError(SemanticState *state, const char *fmt, ...)
{
	va_list args;

	fputs("Internal error: ", stderr);

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	ErrorMessageCommon(state);

	state->success = cc_false;
}

static void OutOfMemoryError(SemanticState *state)
{
	fputs("Out-of-memory error.", stderr);

	ErrorMessageCommon(state);

	state->success = cc_false;
}

void m68kasm_warning(void *scanner, Statement *statement, const char *message)
{
	SemanticState *state = m68kasm_get_extra(scanner);

	(void)statement;

	fprintf(stderr, "Warning: %s", message);

	ErrorMessageCommon(state);
}

void m68kasm_error(void *scanner, Statement *statement, const char *message)
{
	SemanticState *state = m68kasm_get_extra(scanner);

	(void)statement;

	fprintf(stderr, "Error: %s", message);

	ErrorMessageCommon(state);
}

static void* MallocAndHandleError(SemanticState *state, size_t size)
{
	void *memory = malloc(size);

	if (memory == NULL)
		OutOfMemoryError(state);

	return memory;
}

static int strncasecmp(const char *lhs, const char *rhs, size_t count)
{
	int delta;
	size_t i;

	delta = 0;

	for (i = 0; i < count; ++i)
	{
		const int lhs_character = tolower((unsigned char)*lhs++);
		const int rhs_character = tolower((unsigned char)*rhs++);

		delta = lhs_character - rhs_character;

		if (delta != 0 || (lhs_character == '\0' /*&& rhs_character == '\0'*/))
			break;
	}

	return delta;
}

static char* DuplicateString(const char *string)
{
	char *duplicated_string;

	const size_t string_size = strlen(string) + 1;

	duplicated_string = malloc(string_size);

	if (duplicated_string != NULL)
		memcpy(duplicated_string, string, string_size);

	return duplicated_string;
}

static char* DuplicateStringAndHandleError(SemanticState *state, const char *string)
{
	char *duplicated_string = DuplicateString(string);

	if (duplicated_string == NULL)
		OutOfMemoryError(state);

	return duplicated_string;
}

static Dictionary_Entry* CreateSymbol(SemanticState *state, const char *identifier)
{
	Dictionary_Entry *dictionary_entry;

	if (!Dictionary_LookUpAndCreateIfNotExist(&state->dictionary, identifier, strlen(identifier), &dictionary_entry))
	{
		OutOfMemoryError(state);
		dictionary_entry = NULL;
	}
	else if (dictionary_entry->type != -1)
	{
		SemanticError(state, "Symbol '%s' cannot be redefined.", identifier);
		dictionary_entry = NULL;
	}

	return dictionary_entry;
}

static void TerminateRept(SemanticState *state)
{
	unsigned long countdown;
	SourceLineListNode *source_line_list_node;

	/* Exit ENDR mode before we recurse into the REPT's nested statements. */
	state->mode = MODE_NORMAL;

	/* Repeat the statements as many times as requested. */
	countdown = state->rept.total_repeats;

	while (countdown-- != 0)
	{
		/* Rewind back to line number of the start of the REPT. */
		state->location->line_number = state->rept.line_number;

		/* Process the REPT's nested statements. */
		for (source_line_list_node = state->rept.source_line_list.head; source_line_list_node != NULL; source_line_list_node = source_line_list_node->next)
			AssembleLine(state, source_line_list_node->source_line);
	}

	/* Increment past the ENDR line number. */
	++state->location->line_number;

	/* Free the source line list. */
	source_line_list_node = state->rept.source_line_list.head;

	while (source_line_list_node != NULL)
	{
		SourceLineListNode *next_source_line_list_node = source_line_list_node->next;

		free(source_line_list_node->source_line);
		free(source_line_list_node);

		source_line_list_node = next_source_line_list_node;
	}
}

static void TerminateMacro(SemanticState *state)
{
	Dictionary_Entry *symbol;

	state->mode = MODE_NORMAL;

	symbol = CreateSymbol(state, state->macro.name);

	if (symbol != NULL)
	{
		Macro *macro = MallocAndHandleError(state, sizeof(Macro));

		if (macro != NULL)
		{
			macro->name = state->macro.name;
			macro->parameter_names = state->macro.parameter_names;
			macro->source_line_list_head = state->macro.source_line_list.head;

			symbol->type = SYMBOL_MACRO;
			symbol->shared.pointer = macro;
		}
	}
}

static void AddToSourceLineList(SemanticState *state, SourceLineList *source_line_list, const char *source_line)
{
	SourceLineListNode *source_line_list_node = MallocAndHandleError(state, sizeof(SourceLineListNode));

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
		source_line_list_node->source_line = DuplicateStringAndHandleError(state, source_line);
	}
}

static char* ExpandLocalIdentifier(SemanticState *state, const char *identifier)
{
	const char *last_global_label = state->last_global_label != NULL ? state->last_global_label : "";
	const size_t prefix_length = strlen(last_global_label);
	const size_t suffix_length = strlen(identifier);
	char *expanded_identifier = MallocAndHandleError(state, prefix_length + suffix_length + 1);

	if (expanded_identifier != NULL)
	{
		memcpy(expanded_identifier, last_global_label, prefix_length);
		memcpy(expanded_identifier + prefix_length, identifier, suffix_length + 1);
	}

	return expanded_identifier;
}

static cc_bool ResolveValue(SemanticState *state, Value *value, unsigned long *value_integer)
{
	cc_bool success = cc_true;

	switch (value->type)
	{
		case VALUE_SUBTRACT:
		case VALUE_ADD:
		case VALUE_MULTIPLY:
		case VALUE_DIVIDE:
		case VALUE_MODULO:
		case VALUE_LOGICAL_OR:
		case VALUE_LOGICAL_AND:
		case VALUE_ARITHMETIC_OR:
		case VALUE_ARITHMETIC_XOR:
		case VALUE_ARITHMETIC_AND:
		case VALUE_EQUALITY:
		case VALUE_INEQUALITY:
		case VALUE_LESS_THAN:
		case VALUE_LESS_OR_EQUAL:
		case VALUE_MORE_THAN:
		case VALUE_MORE_OR_EQUAL:
		case VALUE_LEFT_SHIFT:
		case VALUE_RIGHT_SHIFT:
		{
			unsigned long left_value;
			unsigned long right_value;

			if (!ResolveValue(state, &value->shared.values[0], &left_value) || !ResolveValue(state, &value->shared.values[1], &right_value))
			{
				success = cc_false;
			}
			else
			{
				/* We're done with these; delete them. */
				free(value->shared.values);

				switch (value->type)
				{
					case VALUE_NUMBER:
					case VALUE_IDENTIFIER:
					case VALUE_PROGRAM_COUNTER:
					case VALUE_NEGATE:
					case VALUE_BITWISE_NOT:
					case VALUE_LOGICAL_NOT:
						/* This code should never be ran. */
						break;

					case VALUE_SUBTRACT:
						*value_integer = left_value - right_value;
						break;

					case VALUE_ADD:
						*value_integer = left_value + right_value;
						break;

					case VALUE_MULTIPLY:
						*value_integer = left_value * right_value;
						break;

					case VALUE_DIVIDE:
						*value_integer = left_value / right_value;
						break;

					case VALUE_MODULO:
						*value_integer = left_value % right_value;
						break;

					case VALUE_LOGICAL_OR:
						*value_integer = left_value || right_value;
						break;

					case VALUE_LOGICAL_AND:
						*value_integer = left_value && right_value;
						break;

					case VALUE_ARITHMETIC_OR:
						*value_integer = left_value | right_value;
						break;

					case VALUE_ARITHMETIC_XOR:
						*value_integer = left_value ^ right_value;
						break;

					case VALUE_ARITHMETIC_AND:
						*value_integer = left_value & right_value;
						break;

					case VALUE_EQUALITY:
						*value_integer = left_value == right_value;
						break;

					case VALUE_INEQUALITY:
						*value_integer = left_value != right_value;
						break;

					case VALUE_LESS_THAN:
						*value_integer = left_value < right_value;
						break;

					case VALUE_LESS_OR_EQUAL:
						*value_integer = left_value <= right_value;
						break;

					case VALUE_MORE_THAN:
						*value_integer = left_value > right_value;
						break;

					case VALUE_MORE_OR_EQUAL:
						*value_integer = left_value >= right_value;
						break;

					case VALUE_LEFT_SHIFT:
						*value_integer = left_value << right_value;
						break;

					case VALUE_RIGHT_SHIFT:
						*value_integer = left_value >> right_value;
						break;
				}

				/* Prevent 'bleeding' out of the 68k's 32-bit range. */
				*value_integer &= 0xFFFFFFFF;
			}

			break;
		}

		case VALUE_NEGATE:
		case VALUE_BITWISE_NOT:
		case VALUE_LOGICAL_NOT:
			if (!ResolveValue(state, value->shared.values, value_integer))
			{
				success = cc_false;
			}
			else
			{
				/* We're done with this; delete it. */
				free(value->shared.values);

				switch (value->type)
				{
					case VALUE_NUMBER:
					case VALUE_IDENTIFIER:
					case VALUE_PROGRAM_COUNTER:
					case VALUE_SUBTRACT:
					case VALUE_ADD:
					case VALUE_MULTIPLY:
					case VALUE_DIVIDE:
					case VALUE_MODULO:
					case VALUE_LOGICAL_OR:
					case VALUE_LOGICAL_AND:
					case VALUE_ARITHMETIC_OR:
					case VALUE_ARITHMETIC_XOR:
					case VALUE_ARITHMETIC_AND:
					case VALUE_EQUALITY:
					case VALUE_INEQUALITY:
					case VALUE_LESS_THAN:
					case VALUE_LESS_OR_EQUAL:
					case VALUE_MORE_THAN:
					case VALUE_MORE_OR_EQUAL:
					case VALUE_LEFT_SHIFT:
					case VALUE_RIGHT_SHIFT:
						/* This code should never be ran. */
						break;

					case VALUE_NEGATE:
						*value_integer = 0 - *value_integer;
						break;

					case VALUE_BITWISE_NOT:
						*value_integer = ~*value_integer;
						break;

					case VALUE_LOGICAL_NOT:
						*value_integer = !*value_integer;
						break;
				}

				/* Prevent 'bleeding' out of the 68k's 32-bit range. */
				*value_integer &= 0xFFFFFFFF;
			}

			break;

		case VALUE_NUMBER:
			*value_integer = value->shared.integer;
			break;

		case VALUE_IDENTIFIER:
		{
			char *expanded_identifier = NULL;
			const char *identifier = value->shared.identifier;
			Dictionary_Entry *dictionary_entry;

			if (value->shared.identifier[0] == '@')
			{
				expanded_identifier = ExpandLocalIdentifier(state, value->shared.identifier);

				if (expanded_identifier != NULL)
					identifier = expanded_identifier;
			}

			dictionary_entry = Dictionary_LookUp(&state->dictionary, identifier, strlen(identifier));

			if (dictionary_entry == NULL || dictionary_entry->type == -1)
			{
				success = cc_false;

				if (state->doing_final_pass)
					SemanticError(state, "Symbol '%s' does not exist.", identifier);
				else
					state->fix_up_needed = cc_true;
			}
			else if (dictionary_entry->type != SYMBOL_CONSTANT && dictionary_entry->type != SYMBOL_VARIABLE)
			{
				success = cc_false;
				SemanticError(state, "Symbol '%s' is not a constant or a variable.", identifier);
			}
			else
			{
				/* We're done with the identifier: delete it. */
				free(value->shared.identifier);

				*value_integer = dictionary_entry->shared.unsigned_integer;
			}

			free(expanded_identifier);

			break;
		}

		case VALUE_PROGRAM_COUNTER:
			*value_integer = Dictionary_LookUp(&state->dictionary, PROGRAM_COUNTER, sizeof(PROGRAM_COUNTER) - 1)->shared.unsigned_integer;
			break;
	}

	/* Now that we have resolved the value, let's hardcode it here so that we don't ever have to calculate it again. */
	/* This is especially useful for fix-ups, which may otherwise depend on identifiers that no longer exist at the time is value is resolved again. */
	if (success)
	{
		value->type = VALUE_NUMBER;
		value->shared.integer = *value_integer;
	}

	return success;
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
			return 0x0000;
	}

	assert(cc_false);
	return 0x0000;
}

static unsigned int ConstructEffectiveAddressBits(SemanticState *state, const Operand *operand)
{
	unsigned int m, xn;

	/* Shut up 'variable may be used uninitialised' compiler warnings. */
	m = 0;
	xn = 0;

	switch (operand->type)
	{
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

/*
static cc_bool OperandIsUnusual(const Operand *operand)
{
	switch (operand->type)
	{
		case OPERAND_DATA_REGISTER:
		case OPERAND_ADDRESS_REGISTER:
		case OPERAND_ADDRESS:
		case OPERAND_ADDRESS_ABSOLUTE:
		case OPERAND_LITERAL:
			return cc_false;

		case OPERAND_STATUS_REGISTER:
		case OPERAND_CONDITION_CODE_REGISTER:
		case OPERAND_USER_STACK_POINTER_REGISTER:
			return cc_true;
	}
}
*/
static unsigned int ToAlternateEffectiveAddressBits(unsigned int bits)
{
	const unsigned int m = (bits >> 3) & 7;
	const unsigned int dn = (bits >> 0) & 7;

	return (m << 6) | (dn << 9);
}

static void SetLastGlobalLabel(SemanticState *state, const char *label)
{
	if (label[0] != '@')
	{
		/* This is a global label - cache it for later. */
		free(state->last_global_label);
		state->last_global_label = DuplicateStringAndHandleError(state, label);
	}
}

static void AddLabelToSymbolTable(SemanticState *state, const char *label, unsigned long value)
{
	char *expanded_identifier;
	const char *identifier;
	Dictionary_Entry *symbol;

	expanded_identifier = NULL;
	identifier = label;

	if (label[0] == '@')
	{
		/* This is a local label - prefix it with the previous global label. */
		expanded_identifier = ExpandLocalIdentifier(state, label);

		if (expanded_identifier != NULL)
			identifier = expanded_identifier;
	}

	/* Now add it to the symbol table. */
	symbol = CreateSymbol(state, identifier);

	if (symbol != NULL)
	{
		symbol->type = SYMBOL_CONSTANT;
		symbol->shared.unsigned_integer = value;
	}

	free(expanded_identifier);
}

typedef struct InstructionMetadata
{
	const char *name;

	Size allowed_sizes;

	OperandType allowed_operands[2];
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
			OPERAND_REGISTER_LIST,
		}
	},
	{	/* OPCODE_MOVEM_FROM_REGS */
		"MOVEM",
		SIZE_WORD | SIZE_LONGWORD,
		{
			OPERAND_REGISTER_LIST,
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

	state->program_counter += 2;

	/* Some instructions are ambiguous, so figure them out fully here. */
	switch (instruction->opcode.type)
	{
		case OPCODE_ORI:
		case OPCODE_ANDI:
		case OPCODE_EORI:
		case OPCODE_BTST_STATIC:
		case OPCODE_BCHG_STATIC:
		case OPCODE_BCLR_STATIC:
		case OPCODE_BSET_STATIC:
		case OPCODE_MOVEP_TO_REG:
		case OPCODE_MOVE:
		case OPCODE_MOVEM_TO_REGS:
		case OPCODE_SBCD_DATA_REGS:
		case OPCODE_OR_TO_REG:
		case OPCODE_SUB_TO_REG:
		case OPCODE_SUBX_DATA_REGS:
		case OPCODE_ABCD_DATA_REGS:
		case OPCODE_AND_TO_REG:
		case OPCODE_ADD_TO_REG:
		case OPCODE_ADDX_DATA_REGS:
			switch (instruction->opcode.type)
			{
				case OPCODE_ORI:
					if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
						instruction->opcode.type = OPCODE_ORI_TO_CCR;
					else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
						instruction->opcode.type = OPCODE_ORI_TO_SR;

					break;

				case OPCODE_ANDI:
					if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
						instruction->opcode.type = OPCODE_ANDI_TO_CCR;
					else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
						instruction->opcode.type = OPCODE_ANDI_TO_SR;

					break;

				case OPCODE_EORI:
					if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
						instruction->opcode.type = OPCODE_EORI_TO_CCR;
					else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
						instruction->opcode.type = OPCODE_EORI_TO_SR;

					break;

				case OPCODE_BTST_STATIC:
					if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
						instruction->opcode.type = OPCODE_BTST_DYNAMIC;

					break;

				case OPCODE_BCHG_STATIC:
					if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
						instruction->opcode.type = OPCODE_BCHG_DYNAMIC;

					break;

				case OPCODE_BCLR_STATIC:
					if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
						instruction->opcode.type = OPCODE_BCLR_DYNAMIC;

					break;

				case OPCODE_BSET_STATIC:
					if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
						instruction->opcode.type = OPCODE_BSET_DYNAMIC;

					break;

				case OPCODE_MOVEP_TO_REG:
					if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
						instruction->opcode.type = OPCODE_MOVEP_FROM_REG;

					break;

				case OPCODE_MOVE:
					if (instruction->operands[0].type == OPERAND_STATUS_REGISTER)
						instruction->opcode.type = OPCODE_MOVE_FROM_SR;
					else if (instruction->operands[1].type == OPERAND_STATUS_REGISTER)
						instruction->opcode.type = OPCODE_MOVE_TO_SR;
					else if (instruction->operands[1].type == OPERAND_CONDITION_CODE_REGISTER)
						instruction->opcode.type = OPCODE_MOVE_TO_CCR;
					else if (instruction->operands[0].type == OPERAND_USER_STACK_POINTER_REGISTER)
						instruction->opcode.type = OPCODE_MOVE_FROM_USP;
					else if (instruction->operands[1].type == OPERAND_USER_STACK_POINTER_REGISTER)
						instruction->opcode.type = OPCODE_MOVE_TO_USP;
					else if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER)
					{
						instruction->opcode.type = OPCODE_MOVEA; /* MOVEA mistyped as MOVE */
						SemanticWarning(state, "MOVE should be MOVEA.");
					}

					break;

				case OPCODE_MOVEM_TO_REGS:
					if (instruction->operands[0].type == OPERAND_REGISTER_LIST)
						instruction->opcode.type = OPCODE_MOVEM_FROM_REGS;

					break;

				case OPCODE_SBCD_DATA_REGS:
					if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
						instruction->opcode.type = OPCODE_SBCD_ADDRESS_REGS;

					break;

				case OPCODE_OR_TO_REG:
					if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
						instruction->opcode.type = OPCODE_OR_FROM_REG;

					break;

				case OPCODE_SUB_TO_REG:
					if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
						instruction->opcode.type = OPCODE_SUB_FROM_REG;

					break;

				case OPCODE_SUBX_DATA_REGS:
					if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
						instruction->opcode.type = OPCODE_SUBX_ADDRESS_REGS;

					break;

				case OPCODE_ABCD_DATA_REGS:
					if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
						instruction->opcode.type = OPCODE_ABCD_ADDRESS_REGS;

					break;

				case OPCODE_AND_TO_REG:
					if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
						instruction->opcode.type = OPCODE_AND_FROM_REG;

					break;

				case OPCODE_ADD_TO_REG:
					if (instruction->operands[1].type != OPERAND_DATA_REGISTER)
						instruction->opcode.type = OPCODE_ADD_FROM_REG;

					break;

				case OPCODE_ADDX_DATA_REGS:
					if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
						instruction->opcode.type = OPCODE_ADDX_ADDRESS_REGS;

					break;

				default:
					break;
			}

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
							instruction->opcode.type = OPCODE_ASL_DYNAMIC;
							break;

						case OPCODE_ASR_STATIC:
							instruction->opcode.type = OPCODE_ASR_DYNAMIC;
							break;

						case OPCODE_LSL_STATIC:
							instruction->opcode.type = OPCODE_LSL_DYNAMIC;
							break;

						case OPCODE_LSR_STATIC:
							instruction->opcode.type = OPCODE_LSR_DYNAMIC;
							break;

						case OPCODE_ROXL_STATIC:
							instruction->opcode.type = OPCODE_ROXL_DYNAMIC;
							break;

						case OPCODE_ROXR_STATIC:
							instruction->opcode.type = OPCODE_ROXR_DYNAMIC;
							break;

						case OPCODE_ROL_STATIC:
							instruction->opcode.type = OPCODE_ROL_DYNAMIC;
							break;

						case OPCODE_ROR_STATIC:
							instruction->opcode.type = OPCODE_ROR_DYNAMIC;
							break;

						default:
							break;
					}
				}
			}
			else
			{
				switch (instruction->opcode.type)
				{
					case OPCODE_ASL_STATIC:
						instruction->opcode.type = OPCODE_ASL_SINGLE;
						break;

					case OPCODE_ASR_STATIC:
						instruction->opcode.type = OPCODE_ASR_SINGLE;
						break;

					case OPCODE_LSL_STATIC:
						instruction->opcode.type = OPCODE_LSL_SINGLE;
						break;

					case OPCODE_LSR_STATIC:
						instruction->opcode.type = OPCODE_LSR_SINGLE;
						break;

					case OPCODE_ROXL_STATIC:
						instruction->opcode.type = OPCODE_ROXL_SINGLE;
						break;

					case OPCODE_ROXR_STATIC:
						instruction->opcode.type = OPCODE_ROXR_SINGLE;
						break;

					case OPCODE_ROL_STATIC:
						instruction->opcode.type = OPCODE_ROL_SINGLE;
						break;

					case OPCODE_ROR_STATIC:
						instruction->opcode.type = OPCODE_ROR_SINGLE;
						break;

					default:
						break;
				}
			}

			break;

		default:
			break;
	}

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
			size_undefined = "";
		}

		SemanticError(state, "Instruction cannot be this size: allowed sizes are...%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", newline_byte, opcode_byte, size_byte, newline_short, opcode_short, size_short, newline_word, opcode_word, size_word, newline_longword, opcode_longword, size_longword, newline_undefined, opcode_undefined, size_undefined);
	}
	else
	{
		cc_bool good_operands = cc_true;

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
					instruction->opcode.size = instruction_metadata->allowed_sizes & ~SIZE_UNDEFINED;
					break;
			}
		}

		/* Count operands that we want. */
		total_operands_wanted = 0;

		while (instruction_metadata->allowed_operands[total_operands_wanted] != 0 && total_operands_wanted < CC_COUNT_OF(instruction_metadata->allowed_operands))
			++total_operands_wanted;

		/* Count operands that we have. */
		total_operands_have = 0;

		while (instruction->operands[total_operands_have].type != 0 && total_operands_have < CC_COUNT_OF(instruction->operands))
			++total_operands_have;

		if (total_operands_wanted != total_operands_have)
		{
			SemanticError(state, "Instruction has %u operands, but it should have %u.", total_operands_have, total_operands_wanted);
			good_operands = cc_false;
		}
		else
		{
			/* Check whether the operands are of the correct types. */
			for (i = 0; i < total_operands_have && instruction_metadata->allowed_operands[i] != 0; ++i)
			{
				if ((instruction->operands[i].type & ~instruction_metadata->allowed_operands[i]) != 0)
				{
					const char *operand_string = "[REDACTED]"; /* Dumb joke - this should never be seen. */

					switch (instruction->operands[i].type)
					{
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

					SemanticError(state, "Instruction operand %u cannot be %s.", i, operand_string);
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

								if (!ResolveValue(state, &instruction->operands[0].literal, &value))
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
						unsigned int data_register = 0;
						unsigned int address_register = 0;

						if (instruction->operands[0].type == OPERAND_DATA_REGISTER)
						{
							data_register = instruction->operands[0].main_register;
							address_register = instruction->operands[1].main_register;
						}
						else if (instruction->operands[1].type == OPERAND_DATA_REGISTER)
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

						if (!ResolveValue(state, &instruction->operands[0].literal, &value))
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
						/* Swap the operands, since the literal for the register list needs to come before the other operator. */
						operands_to_output[0] = &instruction->operands[1];
						operands_to_output[1] = &instruction->operands[0];
						/* Fallthrough */
					case OPCODE_MOVEM_FROM_REGS:
						machine_code = 0x4880;
						machine_code |= (instruction->opcode.size == SIZE_LONGWORD) << 6;

						if (instruction->opcode.type == OPCODE_MOVEM_TO_REGS)
							machine_code |= 1 << 10;

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

						switch (instruction->opcode.type)
						{
							case OPCODE_ADDQ:
								machine_code = 0x5000;
								break;

							case OPCODE_SUBQ:
								machine_code = 0x5100;
								break;

							default:
								break;
						}

						machine_code |= ConstructSizeBits(instruction->opcode.size);
						machine_code |= ConstructEffectiveAddressBits(state, &instruction->operands[1]);

						if (!ResolveValue(state, &instruction->operands[0].literal, &value))
							value = 1;

						if (value < 1 || value > 8)
							SemanticError(state, "The value to add/subtract cannot be lower than 1 or higher than 8.");
						else
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

						if (!ResolveValue(state, &instruction->operands[1].literal, &value))
							value = state->program_counter - 2;

						operands_to_output[0] = &custom_operands[0];
						operands_to_output[1] = NULL;
						custom_operands[0].type = OPERAND_LITERAL;
						custom_operands[0].literal.type = VALUE_NUMBER;

						if (value >= state->program_counter)
						{
							const unsigned long offset = value - state->program_counter;

							if (offset > 0x7FFF)
								SemanticError(state, "The destination is too far away: it must be less than $8000 bytes after the start of the instruction, but instead it was $%lX bytes away.", offset);

							custom_operands[0].literal.shared.integer = offset;
						}
						else
						{
							const unsigned long offset = state->program_counter - value;

							if (offset > 0x8000)
								SemanticError(state, "The destination is too far away: it must be less than $8001 bytes before the start of the instruction, but instead it was $%lX bytes away.", offset);

							custom_operands[0].literal.shared.integer = 0 - offset;
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
								break;
						}

						if (!ResolveValue(state, &instruction->operands[0].literal, &value))
							value = state->program_counter - 2;

						if (value >= state->program_counter)
						{
							offset = value - state->program_counter;

							if (instruction->opcode.size == SIZE_BYTE || instruction->opcode.size == SIZE_SHORT)
							{
								if (offset == 0)
									SemanticError(state, "The destination cannot be 0 bytes away when using a short-sized branch.");
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
							operands_to_output[0] = &custom_operands[0];
							custom_operands[0].type = OPERAND_LITERAL;
							custom_operands[0].literal.type = VALUE_NUMBER;
							custom_operands[0].literal.shared.integer = offset;
						}

						break;
					}

					case OPCODE_MOVEQ:
					{
						unsigned long value;

						machine_code = 0x7000;

						if (!ResolveValue(state, &instruction->operands[0].literal, &value))
							value = 0;

						if (value > 0x7F && value < 0xFFFFFF80)
							SemanticError(state, "Value is too large: it must be between -$80 and $7F, or $FFFFFF80 and $FFFFFFFF.");
						else
							machine_code |= value & 0xFF;

						machine_code |= instruction->operands[1].main_register << 9;

						/* MOVEQ's operands are embedded directly into the machine code, so we don't need to output them separately. */
						operands_to_output[0] = NULL;

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
						else if (instruction->operands[0].type == OPERAND_ADDRESS_REGISTER && instruction->operands[1].type == OPERAND_DATA_REGISTER)
						{
							machine_code |= 0x0088;
							machine_code |= instruction->operands[1].main_register << 9;
							machine_code |= instruction->operands[0].main_register << 0;
						}

						break;

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
						unsigned int identifier;

						switch (instruction->opcode.type)
						{
							default:
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

								if (!ResolveValue(state, &instruction->operands[0].literal, &value))
									value = 0;

								if (value > 8 || value < 1)
									SemanticError(state, "The shift value must not be greater than 8 or lower than 1.");
								else
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
		fputc((machine_code >> (8 * i)) & 0xFF, state->output_file);

	/* Output the data for the operands. */
	for (i = 0; i < CC_COUNT_OF(operands_to_output); ++i)
	{
		Operand *operand = operands_to_output[i];

		if (operand != NULL)
		{
			switch (operand->type)
			{
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

					if (!ResolveValue(state, &operand->literal, &value))
					{
						if (operand->type == OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT || operand->type == OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER)
							value = state->program_counter; /* Prevent out-of-range displacements later on. */
						else
							value = 0;
					}

					switch (operand->type)
					{
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
						fputc((value >> (8 * bytes_to_write)) & 0xFF, state->output_file);

					break;
				}

				case OPERAND_REGISTER_LIST:
				{
					unsigned int bytes_to_write;
					unsigned int register_list = operand->main_register;

					/* Ugly hack to reverse the register list when doing `movem.w/.l d0-a7,-(aN)` */
					if (instruction->operands[1].type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
					{
						static unsigned int reverse_nibble[0x10] = {0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE, 0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF};

						register_list = reverse_nibble[(register_list >> (4 * 0)) & 0xF] << (4 * 3)
							      | reverse_nibble[(register_list >> (4 * 1)) & 0xF] << (4 * 2)
							      | reverse_nibble[(register_list >> (4 * 2)) & 0xF] << (4 * 1)
							      | reverse_nibble[(register_list >> (4 * 3)) & 0xF] << (4 * 0);
					}

					for (bytes_to_write = 2; bytes_to_write-- > 0; )
						fputc((register_list >> (8 * bytes_to_write)) & 0xFF, state->output_file);

					state->program_counter += 2;

					break;
				}
			}
		}
	}
}

static void OutputDcValue(SemanticState *state, const Size size, unsigned long value)
{
	unsigned int bytes_to_write = 0;

	/* Update the program counter symbol in between values, to keep it up to date. */
	Dictionary_LookUp(&state->dictionary, PROGRAM_COUNTER, sizeof(PROGRAM_COUNTER) - 1)->shared.unsigned_integer = state->program_counter;

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
			break;
	}

	state->program_counter += bytes_to_write;

	while (bytes_to_write-- != 0)
		fputc((value >> (bytes_to_write * 8)) & 0xFF, state->output_file);
}

static void ProcessDc(SemanticState *state, StatementDc *dc)
{
	ValueListNode *value_list_node;

	for (value_list_node = dc->values; value_list_node != NULL; value_list_node = value_list_node->next)
	{
		switch (value_list_node->type)
		{
			case VALUE_LIST_NODE_TYPE_VALUE:
			{
				unsigned long value;

				if (!ResolveValue(state, &value_list_node->shared.value, &value))
					value = 0;

				OutputDcValue(state, dc->size, value);

				break;
			}

			case VALUE_LIST_NODE_TYPE_STRING:
			{
				const char *character;

				for (character = value_list_node->shared.string; *character != '\0'; ++character)
					OutputDcValue(state, dc->size, *character);

				break;
			}
		}
	}
}

static void ProcessDcb(SemanticState *state, StatementDcb *dcb)
{
	unsigned long repetitions;

	if (!ResolveValue(state, &dcb->repetitions, &repetitions))
	{
		SemanticError(state, "Repetition value must be evaluable on first pass.");
	}
	else
	{
		unsigned long value;
		unsigned long i;

		if (!ResolveValue(state, &dcb->value, &value))
			value = 0;

		for (i = 0; i < repetitions; ++i)
			OutputDcValue(state, dcb->size, value);	
	}
}

static void ProcessInclude(SemanticState *state, const StatementInclude *include)
{
	FILE *input_file = fopen(include->path, "r");

	if (input_file == NULL)
	{
		SemanticError(state, "File '%s' could not be opened.", include->path);
	}
	else
	{
		/* Add file path and line number to the location list. */
		Location location;

		location.file_path = include->path;
		location.line_number = 0;

		location.previous = state->location;
		state->location = &location;

		AssembleFile(state, input_file);

		state->location = state->location->previous;

		fclose(input_file);
	}
}

static void ProcessIncbin(SemanticState *state, StatementIncbin *incbin)
{
	FILE *input_file = fopen(incbin->path, "rb");

	if (input_file == NULL)
	{
		SemanticError(state, "File '%s' could not be opened.", incbin->path);
	}
	else
	{
		unsigned long value;

		if (!ResolveValue(state, &incbin->start, &value))
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

			if (!ResolveValue(state, &incbin->length, &length))
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
				fputc(character, state->output_file);
			}
		}
		else
		{
			int character;

			while ((character = fgetc(input_file)) != EOF)
			{
				++state->program_counter;
				fputc(character, state->output_file);
			}
		}

		fclose(input_file);
	}
}

static void ProcessRept(SemanticState *state, StatementRept *rept)
{
	state->mode = MODE_REPT;

	if (!ResolveValue(state, &rept->total_repeats, &state->rept.total_repeats))
	{
		SemanticError(state, "Repetition value must be evaluable on the first pass.");
		state->rept.total_repeats = 1;
	}

	state->rept.line_number = state->location->line_number;

	state->rept.source_line_list.head = NULL;
	state->rept.source_line_list.tail = NULL;
}

static void ProcessMacro(SemanticState *state, StatementMacro *macro, const char *label)
{
	state->mode = MODE_MACRO;

	state->macro.name = DuplicateStringAndHandleError(state, label);
	state->macro.line_number = state->location->line_number;
	state->macro.parameter_names = macro->parameter_names;
	macro->parameter_names = NULL;

	state->macro.source_line_list.head = NULL;
	state->macro.source_line_list.tail = NULL;
}

static void ProcessIf(SemanticState *state, Value *value)
{
	++state->current_if_level;

	/* If we are within a false conditional block, then this if statement is null and void. */
	if (state->false_if_level == 0)
	{
		unsigned long resolved_value;

		if (!ResolveValue(state, value, &resolved_value))
		{
			SemanticError(state, "Condition must be evaluable on the first pass.");
			resolved_value = 1;
		}

		/* If this condition is false, then mark this as the false if-level. */
		if (resolved_value == 0)
			state->false_if_level = state->current_if_level;
	}
}

static void ProcessStatement(SemanticState *state, Statement *statement, const char *label)
{
	Dictionary_LookUp(&state->dictionary, PROGRAM_COUNTER, sizeof(PROGRAM_COUNTER) - 1)->shared.unsigned_integer = state->program_counter;

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
		case STATEMENT_TYPE_EVEN:
		case STATEMENT_TYPE_INFORM:
		case STATEMENT_TYPE_END:
			if (label != NULL && !state->doing_fix_up && !state->doing_final_pass)
			{
				SetLastGlobalLabel(state, label);
				AddLabelToSymbolTable(state, label, state->program_counter);
			}

			break;

		case STATEMENT_TYPE_ENDR:
		case STATEMENT_TYPE_ENDM:
		case STATEMENT_TYPE_ELSE:
		case STATEMENT_TYPE_ENDC:
			if (label != NULL)
				SemanticError(state, "There cannot be a label on this type of statement.");

			break;

		case STATEMENT_TYPE_MACRO:
		case STATEMENT_TYPE_EQU:
			if (label == NULL)
			{
				SemanticError(state, "This type of statement must have a label.");
			}
			else if (statement->type == STATEMENT_TYPE_EQU)
			{
				unsigned long resolved_value;

				SetLastGlobalLabel(state, label);

				if (ResolveValue(state, &statement->shared.value, &resolved_value))
					AddLabelToSymbolTable(state, label, resolved_value);
			}

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
			if (state->mode != MODE_REPT)
				SemanticError(state, "This stray ENDR has no preceeding REPT.");
			else
				TerminateRept(state);

			break;

		case STATEMENT_TYPE_MACRO:
			ProcessMacro(state, &statement->shared.macro, label);
			break;

		case STATEMENT_TYPE_ENDM:
			if (state->mode != MODE_MACRO)
				SemanticError(state, "This stray ENDM has no preceeding MACRO.");
			else
				TerminateMacro(state);

			break;

		case STATEMENT_TYPE_EQU:
			/* Doesn't do anything here: see above. */
			break;

		case STATEMENT_TYPE_IF:
			ProcessIf(state, &statement->shared.value);
			break;

		case STATEMENT_TYPE_ELSE:
			if (state->current_if_level == 0)
			{
				SemanticError(state, "This stray ELSE has no preceeding IF.");
			}
			else
			{
				/* If this is the false if-level, then it isn't anymore. */
				/* Likewise, if there is no false if-level, then there is now. */
				if (state->false_if_level == state->current_if_level)
					state->false_if_level = 0;
				else if (state->false_if_level == 0)
					state->false_if_level = state->current_if_level;
			}

			break;

		case STATEMENT_TYPE_ENDC:
			if (state->current_if_level == 0)
			{
				SemanticError(state, "This stray ENDC/ENDIF has no preceeding IF.");
			}
			else
			{
				/* If this is the false if-level, then it isn't anymore. */
				if (state->false_if_level == state->current_if_level)
					state->false_if_level = 0;

				--state->current_if_level;
			}

			break;

		case STATEMENT_TYPE_EVEN:
			/* Pad to the nearest even address. */
			if (state->program_counter & 1)
			{
				++state->program_counter;
				fputc(0, state->output_file);
			}

			break;

		case STATEMENT_TYPE_INFORM:
			SemanticWarning(state, "INFORM: '%s'", statement->shared.inform.message);
			break;

		case STATEMENT_TYPE_END:
			state->end = cc_true;
			break;
	}
}

static void AssembleLine(SemanticState *state, const char *source_line)
{
	size_t label_length;
	const char *source_line_pointer;
	char *label;
	size_t keyword_length;
	Statement statement;

	if (state->listing_file != NULL)
		fprintf(state->listing_file, "%08lX : %s\n", state->program_counter, source_line);

	++state->location->line_number;

	state->source_line = source_line;
	source_line_pointer = source_line;

	label_length = strcspn(source_line_pointer, " \t:;");

	if (label_length != 0)
	{
		/* We've found a label! */
	}
	else
	{
		/* Maybe the label has some whitespace before it: check again. */
		source_line_pointer += strspn(source_line_pointer, " \t");

		label_length = strcspn(source_line_pointer, " \t:;");

		if (label_length != 0)
		{
			/* Since we've encountered whitespace, it's ambiguous whether this is a label or a statement.
			   To figure it out, we have to rely on the label having a ':' after it. */
			if (source_line_pointer[label_length] == ':')
			{
				/* We've found the label */
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
		/* This line has a label. */
		label = MallocAndHandleError(state, label_length + 1);

		if (label != NULL)
		{
			memcpy(label, source_line_pointer, label_length);
			label[label_length] = '\0';
		}

		source_line_pointer += label_length;

		if (source_line_pointer[0] == ':')
			++source_line_pointer;
	}

	source_line_pointer += strspn(source_line_pointer, " \t");
	keyword_length = strcspn(source_line_pointer, " \t.;");

	switch (state->mode)
	{
		case MODE_NORMAL:
		{
			if (state->false_if_level != 0)
			{
				if (keyword_length != 0)
				{
					if (strncasecmp(source_line_pointer, "if", keyword_length) == 0)
					{
						/* Create a false if statement. */
						statement.type = STATEMENT_TYPE_IF;
						statement.shared.value.type = VALUE_NUMBER;
						statement.shared.value.shared.integer = 0;
						ProcessStatement(state, &statement, label);
					}
					else if (strncasecmp(source_line_pointer, "else", keyword_length) == 0)
					{
						/* TODO - Detect code after the keyword and error if any is found. */
						statement.type = STATEMENT_TYPE_ELSE;
						ProcessStatement(state, &statement, label);
					}
					else if (strncasecmp(source_line_pointer, "endc", keyword_length) == 0 || strncasecmp(source_line_pointer, "endif", keyword_length) == 0)
					{
						/* TODO - Detect code after the keyword and error if any is found. */
						statement.type = STATEMENT_TYPE_ENDC;
						ProcessStatement(state, &statement, label);
					}
					else
					{
						/* Drop the line completely, since it's inside the false half of an if statement. */
					}
				}
			}
			else
			{
				/* Look up the keyword in the dictionary to see if it's a macro. */
				const Dictionary_Entry* const macro_dictionary_entry = Dictionary_LookUp(&state->dictionary, source_line_pointer, keyword_length);

				if (macro_dictionary_entry != NULL && macro_dictionary_entry->type == SYMBOL_MACRO)
				{
					/* Macro invocation. */
					char **parameters;
					size_t total_parameters;

					source_line_pointer += strcspn(source_line_pointer, " \t.;");
					parameters = MallocAndHandleError(state, sizeof(char*));
					total_parameters = 1;

					if (label != NULL)
					{
						SetLastGlobalLabel(state, label);
						AddLabelToSymbolTable(state, label, state->program_counter);
					}

					/* Extract and store the macro size specifier, if one exists. */
					if (source_line_pointer[0] == '.')
					{
						size_t size_length;

						++source_line_pointer;

						size_length = strcspn(source_line_pointer, " \t;");
						parameters[0] = MallocAndHandleError(state, size_length + 1);

						if (parameters[0] != NULL)
						{
							memcpy(parameters[0], source_line_pointer, size_length);
							parameters[0][size_length] = '\0';
						}

						source_line_pointer += size_length;
					}
					else
					{
						parameters[0] = NULL;
					}

					/* Extract and store the macro parameters, if they exist. */
					{
						char character;

						do
						{
							const char* const parameter_start = source_line_pointer += strspn(source_line_pointer, " \t");

							do
							{
								character = *source_line_pointer++;

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

								if (character == ',' || character == ';' || character == '\0')
								{
									const size_t parameter_string_length = source_line_pointer - parameter_start - 1;

									if (parameter_string_length != 0)
									{
										char *parameter_string;

										parameter_string = MallocAndHandleError(state, parameter_string_length + 1);

										if (parameter_string != NULL)
										{
											memcpy(parameter_string, parameter_start, parameter_string_length);
											parameter_string[parameter_string_length] = '\0';

											parameters = realloc(parameters, sizeof(char*) * (total_parameters + 1));

											if (parameters == NULL)
											{
												OutOfMemoryError(state);
											}
											else
											{
												parameters[total_parameters] = parameter_string;
												++total_parameters;
											}
										}
									}

									break;
								}
							} while (character != ';' && character != '\0');
						} while (character != ';' && character != '\0');
					}

					/* Define the 'narg' symbol, which represents how many parameters (arguments) have been passed to the macro. */
					{
						Dictionary_Entry *symbol;

						symbol = CreateSymbol(state, "narg");

						if (symbol != NULL)
						{
							symbol->type = SYMBOL_CONSTANT;
							symbol->shared.unsigned_integer = total_parameters - 1;
						}
					}

					/* Finally, invoke the macro. */
					{
						const Macro *macro = macro_dictionary_entry->shared.pointer;
						const SourceLineListNode *source_line_list_node;

						/* Push a new location (this macro).*/
						Location location;

						location.file_path = macro->name;
						location.line_number = 0;

						location.previous = state->location;
						state->location = &location;

						for (source_line_list_node = macro->source_line_list_head; source_line_list_node != NULL; source_line_list_node = source_line_list_node->next)
						{
							const char *remaining_line;
							char *modified_line;

							/* Update the source line for the error printers. */
							state->source_line = source_line_list_node->source_line;

							/* A bit of a cheat so that errors that occur before the call to AssembleLine still show the correct line number. */
							++state->location->line_number;

							/* Replace the parameter placeholders with their proper contents. */
							remaining_line = modified_line = DuplicateStringAndHandleError(state, source_line_list_node->source_line);

							if (modified_line != NULL)
							{
								for (;;)
								{
									const char *parameter_position;
									const char *line_after_parameter;
									unsigned long parameter_index;
									const IdentifierListNode *parameter_name;
									unsigned long i;

									/* Search for the earliest macro parameter placeholder in the line, storing its location in 'parameter_position'. */

									/* Silence bogus(?) 'variable may be used uninitialised' compiler warnings. */
									line_after_parameter = NULL;
									parameter_index = 0;

									/* Find numerical macro parameter placeholder. */
									parameter_position = strchr(remaining_line, '\\');

									if (parameter_position != NULL)
									{
										char *end;

										/* Obtain numerical index of the parameter. */
										parameter_index = strtoul(parameter_position + 1, &end, 10);
										line_after_parameter = end;

										/* Check if conversion failed. */
										if (end == parameter_position + 1)
											parameter_position = NULL;
									}

									/* Now find the identifier-based macro parameter placeholders. */
									for (parameter_name = macro->parameter_names, i = 1; parameter_name != NULL; parameter_name = parameter_name->next, ++i)
									{
										const char* const other_parameter_position = strstr(remaining_line, parameter_name->identifier);
										const char* const other_parameter_end = other_parameter_position + strlen(parameter_name->identifier);

										/* Obviously bail if the identifier wasn't found. */
										if (other_parameter_position != NULL)
										{
											/* If the identifier was in the middle of a larger block of letters/numbers, then don't replace it. */
											/* (This is what AS does, and the Sonic 1 disassembly relies on this). */
											if (other_parameter_position == source_line_list_node->source_line
											|| ((other_parameter_position[-1] < 'a' || other_parameter_position[-1] > 'z')
											 && (other_parameter_position[-1] < 'A' || other_parameter_position[-1] > 'Z')
											 && (other_parameter_position[-1] < '0' || other_parameter_position[-1] > '9')))
											{
												if ((other_parameter_end[0] < 'a' || other_parameter_end[0] > 'z')
												 && (other_parameter_end[0] < 'A' || other_parameter_end[0] > 'Z')
												 && (other_parameter_end[0] < '0' || other_parameter_end[0] > '9'))
												{
													if (parameter_position == NULL || other_parameter_position < parameter_position)
													{
														parameter_index = i;
														parameter_position = other_parameter_position;
														line_after_parameter = other_parameter_end;
													}
												}
											}
										}
									}

									/* If no placeholders can be found, then we are done here. */
									if (parameter_position == NULL)
										break;

									/* Split the line in two, and insert the parameter between them. */
									{
										char *new_modified_line;

										const char* const parameter = (parameter_index < total_parameters && parameters[parameter_index] != NULL) ? parameters[parameter_index] : "";
										const size_t first_half_length = parameter_position - modified_line;
										const size_t parameter_length = strlen(parameter);
										const size_t second_half_length = strlen(line_after_parameter);

										new_modified_line = MallocAndHandleError(state, first_half_length + parameter_length + second_half_length + 1);

										if (new_modified_line != NULL)
										{
											memcpy(new_modified_line, modified_line, first_half_length);
											memcpy(new_modified_line + first_half_length, parameter, parameter_length);
											memcpy(new_modified_line + first_half_length + parameter_length, line_after_parameter, second_half_length);
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
					}

					/* Undefine the 'narg' symbol. */
					if (!Dictionary_Remove(&state->dictionary, "narg", sizeof("narg") - 1))
						InternalError(state, "Could not remove symbol 'narg' from the dictionary.");
				}
				else
				{
					/* Normal assembly line. */
					YY_BUFFER_STATE buffer;
					int parse_result;

					/* Back these up, before they get a chance to be modified by ProcessStatement. */
					const unsigned long starting_program_counter = state->program_counter;
					const long starting_output_position = ftell(state->output_file);

					/* Parse the source line with Flex and Bison (Lex and Yacc). */
					buffer = m68kasm__scan_string(source_line_pointer, state->flex_state);
					parse_result = m68kasm_parse(state->flex_state, &statement);
					m68kasm__delete_buffer(buffer, state->flex_state);

					switch (parse_result)
					{
						case 2: /* Out of memory. */
							OutOfMemoryError(state);
							break;

						case 1: /* Parsing error. */
							/* An error message will have already been printed, so we don't need to do one here. */
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
								FixUp *fix_up = MallocAndHandleError(state, sizeof(FixUp));

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
									/* TODO - Free statement. */

									/* Backup some state. */
									fix_up->program_counter = starting_program_counter;
									fix_up->output_position = starting_output_position;
									fix_up->last_global_label = DuplicateStringAndHandleError(state, state->last_global_label);
									fix_up->source_line = DuplicateStringAndHandleError(state, source_line);
									fix_up->label = label != NULL ? DuplicateStringAndHandleError(state, label) : NULL;

									/* Clone the location. */
									*destination_location = *source_location;
									destination_location->file_path = DuplicateStringAndHandleError(state, source_location->file_path);

									for (source_location = source_location->previous; source_location != NULL; source_location = source_location->previous)
									{
										destination_location->previous = MallocAndHandleError(state, sizeof(Location));

										if (destination_location->previous == NULL)
										{
											break;
										}
										else
										{
											destination_location = destination_location->previous;
											*destination_location = *source_location;
											destination_location->file_path = DuplicateStringAndHandleError(state, source_location->file_path);
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
			}

			break;
		}

		case MODE_REPT:
			if (keyword_length != 0 && strncasecmp(source_line_pointer, "endr", keyword_length) == 0)
			{
				/* TODO - Detect code after the keyword and error if any is found. */
				statement.type = STATEMENT_TYPE_ENDR;
				ProcessStatement(state, &statement, label);
			}
			else
			{
				AddToSourceLineList(state, &state->rept.source_line_list, source_line);
			}

			break;

		case MODE_MACRO:
			if (keyword_length != 0 && strncasecmp(source_line_pointer, "endm", keyword_length) == 0)
			{
				/* TODO - Detect code after the keyword and error if any is found. */
				statement.type = STATEMENT_TYPE_ENDM;
				ProcessStatement(state, &statement, label);
			}
			else
			{
				AddToSourceLineList(state, &state->macro.source_line_list, source_line);
			}

			break;
	}

	free(label);
}

static void AssembleFile(SemanticState *state, FILE *input_file)
{
	while (!state->end && fgets(state->line_buffer, sizeof(state->line_buffer), input_file) != NULL)
	{
		const size_t newline_index = strcspn(state->line_buffer, "\r\n");
		const char newline_character = state->line_buffer[newline_index];

		/* Remove newlines from the string, so they don't appear in the error message. */
		state->line_buffer[newline_index] = '\0';

		/* If there is no newline, then we've either reached the end of the file,
		   or the source line was too long to fit in the buffer. */
		if (newline_character == '\0')
		{
			int character = fgetc(input_file);

			if (character != EOF)
			{
				InternalError(state, "The source line was too long to fit in the internal buffer.");

				/* Fast-forward through until the end of the line. */
				while (character != '\r' && character != '\n' && character != EOF)
					character = fgetc(input_file);
			}
		}

		AssembleLine(state, state->line_buffer);
	}

	switch (state->mode)
	{
		case MODE_NORMAL:
			/* All okay. */
			break;

		case MODE_REPT:
			TerminateRept(state);

			SemanticError(state, "REPT statement beginning at line %lu is missing its ENDR.", state->rept.line_number);
			break;

		case MODE_MACRO:
			TerminateMacro(state);

			SemanticError(state, "MACRO statement beginning at line %lu is missing its ENDM.", state->macro.line_number);
			break;
	}
}

static cc_bool DictionaryFilterDeleteVariables(Dictionary_Entry *entry, const char *identifier, size_t identifier_length, void *user_data)
{
	(void)user_data;

	return (entry->type != SYMBOL_VARIABLE || (identifier_length == sizeof(PROGRAM_COUNTER) - 1 && memcmp(identifier, PROGRAM_COUNTER, identifier_length) == 0));
}

cc_bool ClownAssembler_Assemble(FILE *input_file, FILE *output_file, FILE *listing_file, const char *input_file_path, cc_bool debug, cc_bool case_insensitive)
{
	Location location;
	SemanticState state;
	Dictionary_Entry *symbol;

	location.previous = NULL;
	location.file_path = DuplicateStringAndHandleError(&state, input_file_path != NULL ? input_file_path : "[No path given]");
	location.line_number = 0;

	state.success = cc_true;
	state.output_file = output_file;
	state.listing_file = listing_file;
	state.program_counter = 0;
	state.fix_up_list_head = NULL;
	state.fix_up_list_tail = NULL;
	state.doing_fix_up = cc_false;
	state.doing_final_pass = cc_false;
	state.last_global_label = NULL;
	state.location = &location;
	state.source_line = "[No source line]";
	state.current_if_level = 0;
	state.false_if_level = 0;
	state.mode = MODE_NORMAL;
	state.end = cc_false;

	Dictionary_Init(&state.dictionary, case_insensitive);

	symbol = CreateSymbol(&state, PROGRAM_COUNTER);

	/* Create the dictionary entry for the program counter ahead of time. */
	if (symbol != NULL)
	{
		symbol->type = SYMBOL_VARIABLE;

		if (m68kasm_lex_init_extra(&state, &state.flex_state) != 0)
		{
			InternalError(&state, "m68kasm_lex_init failed.");
		}
		else
		{
		#if M68KASM_DEBUG
			if (debug)
				m68kasm_debug = 1;
		#else
			(void)debug;
		#endif

			/* Perform first pass, and create a list of fix-ups if needed. */
			AssembleFile(&state, input_file);

			if (state.current_if_level != 0)
				SemanticError(&state, "An IF statement somewhere is missing its ENDC/ENDIF.");

			if (m68kasm_lex_destroy(state.flex_state) != 0)
				InternalError(&state, "m68kasm_lex_destroy failed.");

			/* Filter variables out from the symbol table, (variables cannot be used before they are declared, since their values are position-dependent). */
			Dictionary_Filter(&state.dictionary, DictionaryFilterDeleteVariables, NULL);

			/* Process the fix-ups, reassembling instructions and reprocessing directives that could not be done in the first pass. */
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
					fseek(state.output_file, fix_up->output_position , SEEK_SET);
					state.last_global_label = DuplicateStringAndHandleError(&state, fix_up->last_global_label);
					state.source_line = fix_up->source_line != NULL ? fix_up->source_line : "[No source line]";
					state.location = &fix_up->location;

					state.fix_up_needed = cc_false;

					/* Re-process statement. */
					ProcessStatement(&state, &fix_up->statement, fix_up->label != NULL ? fix_up->label : NULL);

					if (!state.fix_up_needed)
					{
						/* This fix-up has been fixed, so we're done with it; now delete it. */
						Location *location;

						*fix_up_pointer = fix_up->next;

						/* We're done with this statement: delete it. */
						DestroyStatement(&fix_up->statement);
						free(fix_up->last_global_label);
						free(fix_up->source_line);
						free(fix_up->label);

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

				/* If no more fix-ups can be fixed, then do one final pass to print errors. */
				if (!a_fix_up_has_been_fixed)
					state.doing_final_pass = cc_true;
			}

			free(state.last_global_label);
		}
	}

	Dictionary_Deinit(&state.dictionary);

	free(location.file_path);

	return state.success;
}
