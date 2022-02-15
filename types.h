#ifndef TYPES_H
#define TYPES_H

typedef struct Opcode
{
	int type;
	int size;
} Opcode;

typedef struct Operand
{
	int type;
	union
	{
		unsigned int data_register;
		unsigned int address_register;
		unsigned long address;
	} data;
} Operand;

typedef struct Instruction
{
	Opcode opcode;
	Operand operands[2];
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
