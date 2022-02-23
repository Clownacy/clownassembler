#ifndef TYPES_H
#define TYPES_H

#include "clowncommon.h"

typedef enum Size
{
	SIZE_BYTE      = 1 << 0,
	SIZE_WORD      = 1 << 1,
	SIZE_LONGWORD  = 1 << 2,
	SIZE_UNDEFINED = 1 << 3
} Size;

typedef enum OpCodeType
{
	OPCODE_MOVE,
	OPCODE_MOVE_TO_USP,
	OPCODE_MOVE_FROM_USP,
	OPCODE_MOVE_TO_CCR,
	OPCODE_MOVE_TO_SR,
	OPCODE_MOVE_FROM_SR,
	OPCODE_ADD,
	OPCODE_ORI_TO_CCR,
	OPCODE_ORI_TO_SR,
	OPCODE_ORI
} OpcodeType;

typedef struct Value
{
	int type;
	union
	{
		unsigned long integer;
		char *identifier;
	} data;
} Value;

typedef struct Opcode
{
	int type;
	Size size;
} Opcode;

typedef enum OperandType
{
	OPERAND_DATA_REGISTER                                                  = 1 << 0,
	OPERAND_ADDRESS_REGISTER                                               = 1 << 1,
	OPERAND_ADDRESS_REGISTER_INDIRECT                                      = 1 << 2,
	OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT                        = 1 << 3,
	OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT                         = 1 << 4,
	OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT                    = 1 << 5,
	OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER = 1 << 6,
	OPERAND_ADDRESS                                                        = 1 << 7,
	OPERAND_LITERAL                                                        = 1 << 8,
	OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT                              = 1 << 9,
	OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER           = 1 << 10,
	OPERAND_STATUS_REGISTER                                                = 1 << 11,
	OPERAND_CONDITION_CODE_REGISTER                                        = 1 << 12,
	OPERAND_USER_STACK_POINTER_REGISTER                                    = 1 << 13
} OperandType;

typedef struct Operand
{
	OperandType type;

	struct Operand *next;

	unsigned int main_register;
	unsigned int index_register;
	Size size;
	Value literal;
	cc_bool index_register_is_address_register;
} Operand;

typedef struct Instruction
{
	Opcode opcode;
	Operand *operands;
} Instruction;

typedef struct Statement
{
	char *label;

	enum
	{
		STATEMENT_TYPE_EMPTY,
		STATEMENT_TYPE_INSTRUCTION,
		STATEMENT_TYPE_MACRO
	} type;
	union
	{
		Instruction instruction;
	} data;
} Statement;

typedef struct StatementListNode
{
	Statement statement;

	struct StatementListNode *next;
} StatementListNode;

#endif /* TYPES_H */
