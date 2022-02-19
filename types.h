#ifndef TYPES_H
#define TYPES_H

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
	int size;
} Opcode;

typedef struct Operand
{
	enum
	{
		OPERAND_TYPE_DATA_REGISTER,
		OPERAND_TYPE_ADDRESS_REGISTER,
		OPERAND_TYPE_STATUS_REGISTER,
		OPERAND_TYPE_CONDITION_CODE_REGISTER,
		OPERAND_TYPE_ADDRESS,
		OPERAND_TYPE_LITERAL
	} type;

	struct Operand *next;

	union
	{
		unsigned int data_register;
		unsigned int address_register;
		struct
		{
			Value value;
			int size;
		} address;
    Value literal;
	} data;
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
