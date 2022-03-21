/*%define parse.trace*/

%define api.pure

%define api.prefix {m68kasm_}

%param {void *scanner}

%parse-param {Statement *statement}

%define parse.error verbose

%code requires {

#include "clowncommon.h"

#define YYSTYPE M68KASM_STYPE

typedef enum Size
{
	SIZE_BYTE      = 1 << 0,
	SIZE_SHORT     = 1 << 1,
	SIZE_WORD      = 1 << 2,
	SIZE_LONGWORD  = 1 << 3,
	SIZE_UNDEFINED = 1 << 4
} Size;

typedef enum Condition
{
	CONDITION_TRUE,
	CONDITION_FALSE,
	CONDITION_HIGHER,
	CONDITION_LOWER_OR_SAME,
	CONDITION_CARRY_CLEAR,
	CONDITION_CARRY_SET,
	CONDITION_NOT_EQUAL,
	CONDITION_EQUAL,
	CONDITION_OVERFLOW_CLEAR,
	CONDITION_OVERFLOW_SET,
	CONDITION_PLUS,
	CONDITION_MINUS,
	CONDITION_GREATER_OR_EQUAL,
	CONDITION_LESS_THAN,
	CONDITION_GREATER_THAN,
	CONDITION_LESS_OR_EQUAL
} Condition;

typedef enum OpCodeType
{
	OPCODE_ORI_TO_CCR,
	OPCODE_ORI_TO_SR,
	OPCODE_ORI,
	OPCODE_ANDI_TO_CCR,
	OPCODE_ANDI_TO_SR,
	OPCODE_ANDI,
	OPCODE_SUBI,
	OPCODE_ADDI,
	OPCODE_EORI_TO_CCR,
	OPCODE_EORI_TO_SR,
	OPCODE_EORI,
	OPCODE_CMPI,
	OPCODE_BTST_STATIC,
	OPCODE_BCHG_STATIC,
	OPCODE_BCLR_STATIC,
	OPCODE_BSET_STATIC,
	OPCODE_BTST_DYNAMIC,
	OPCODE_BCHG_DYNAMIC,
	OPCODE_BCLR_DYNAMIC,
	OPCODE_BSET_DYNAMIC,
	OPCODE_MOVEP_TO_REG,
	OPCODE_MOVEP_FROM_REG,
	OPCODE_MOVEA,
	OPCODE_MOVE,
	OPCODE_MOVE_FROM_SR,
	OPCODE_MOVE_TO_CCR,
	OPCODE_MOVE_TO_SR,
	OPCODE_NEGX,
	OPCODE_CLR,
	OPCODE_NEG,
	OPCODE_NOT,
	OPCODE_EXT,
	OPCODE_NBCD,
	OPCODE_SWAP,
	OPCODE_PEA,
	OPCODE_ILLEGAL,
	OPCODE_TAS,
	OPCODE_TST,
	OPCODE_TRAP,
	OPCODE_LINK,
	OPCODE_UNLK,
	OPCODE_MOVE_TO_USP,
	OPCODE_MOVE_FROM_USP,
	OPCODE_RESET,
	OPCODE_NOP,
	OPCODE_STOP,
	OPCODE_RTE,
	OPCODE_RTS,
	OPCODE_TRAPV,
	OPCODE_RTR,
	OPCODE_JSR,
	OPCODE_JMP,
	OPCODE_MOVEM_TO_REGS,
	OPCODE_MOVEM_FROM_REGS,
	OPCODE_LEA,
	OPCODE_CHK,
	OPCODE_ADDQ,
	OPCODE_SUBQ,
	OPCODE_Scc,
	OPCODE_DBcc,
	OPCODE_BRA,
	OPCODE_BSR,
	OPCODE_Bcc,
	OPCODE_MOVEQ,
	OPCODE_DIVU,
	OPCODE_DIVS,
	OPCODE_SBCD_DATA_REGS,
	OPCODE_SBCD_ADDRESS_REGS,
	OPCODE_OR_TO_REG,
	OPCODE_OR_FROM_REG,
	OPCODE_SUB_TO_REG,
	OPCODE_SUB_FROM_REG,
	OPCODE_SUBX_DATA_REGS,
	OPCODE_SUBX_ADDRESS_REGS,
	OPCODE_SUBA,
	OPCODE_EOR,
	OPCODE_CMPM,
	OPCODE_CMP,
	OPCODE_CMPA,
	OPCODE_MULU,
	OPCODE_MULS,
	OPCODE_ABCD_DATA_REGS,
	OPCODE_ABCD_ADDRESS_REGS,
	OPCODE_EXG,
	OPCODE_AND_TO_REG,
	OPCODE_AND_FROM_REG,
	OPCODE_ADD_TO_REG,
	OPCODE_ADD_FROM_REG,
	OPCODE_ADDX_DATA_REGS,
	OPCODE_ADDX_ADDRESS_REGS,
	OPCODE_ADDA,
	OPCODE_ASL_STATIC,
	OPCODE_ASR_STATIC,
	OPCODE_LSL_STATIC,
	OPCODE_LSR_STATIC,
	OPCODE_ROXL_STATIC,
	OPCODE_ROXR_STATIC,
	OPCODE_ROL_STATIC,
	OPCODE_ROR_STATIC,
	OPCODE_ASL_DYNAMIC,
	OPCODE_ASR_DYNAMIC,
	OPCODE_LSL_DYNAMIC,
	OPCODE_LSR_DYNAMIC,
	OPCODE_ROXL_DYNAMIC,
	OPCODE_ROXR_DYNAMIC,
	OPCODE_ROL_DYNAMIC,
	OPCODE_ROR_DYNAMIC,
	OPCODE_ASL_SINGLE,
	OPCODE_ASR_SINGLE,
	OPCODE_LSL_SINGLE,
	OPCODE_LSR_SINGLE,
	OPCODE_ROXL_SINGLE,
	OPCODE_ROXR_SINGLE,
	OPCODE_ROL_SINGLE,
	OPCODE_ROR_SINGLE
} OpcodeType;

typedef enum ValueType
{
	VALUE_SUBTRACT,
	VALUE_ADD,
	VALUE_MULTIPLY,
	VALUE_DIVIDE,
	VALUE_MODULO,
	VALUE_NEGATE,
	VALUE_BITWISE_NOT,
	VALUE_LOGICAL_NOT,
	VALUE_LOGICAL_OR,
	VALUE_LOGICAL_AND,
	VALUE_ARITHMETIC_OR,
	VALUE_ARITHMETIC_XOR,
	VALUE_ARITHMETIC_AND,
	VALUE_EQUALITY,
	VALUE_INEQUALITY,
	VALUE_LESS_THAN,
	VALUE_LESS_OR_EQUAL,
	VALUE_MORE_THAN,
	VALUE_MORE_OR_EQUAL,
	VALUE_LEFT_SHIFT,
	VALUE_RIGHT_SHIFT,
	VALUE_NUMBER,
	VALUE_IDENTIFIER
} ValueType;

typedef struct Value
{
	ValueType type;
	union
	{
		unsigned long integer;
		char *identifier;
		struct Value *values;
	} data;
} Value;

typedef struct ValueListNode
{
	struct ValueListNode *next;

	enum
	{
		VALUE_LIST_NODE_TYPE_VALUE,
		VALUE_LIST_NODE_TYPE_STRING
	} type;

	union
	{
		Value value;
		char *string;
	} shared;
} ValueListNode;

typedef struct IdentifierListNode
{
	char *identifier;

	struct IdentifierListNode *next;
} IdentifierListNode;

typedef struct Opcode
{
	OpcodeType type;
	Condition condition;
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
	OPERAND_ADDRESS_ABSOLUTE                                               = 1 << 8,
	OPERAND_LITERAL                                                        = 1 << 9,
	OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT                              = 1 << 10,
	OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER           = 1 << 11,
	OPERAND_STATUS_REGISTER                                                = 1 << 12,
	OPERAND_CONDITION_CODE_REGISTER                                        = 1 << 13,
	OPERAND_USER_STACK_POINTER_REGISTER                                    = 1 << 14,
	OPERAND_REGISTER_LIST                                                  = 1 << 15
} OperandType;

typedef struct Operand
{
	OperandType type;

	unsigned int main_register;
	unsigned int index_register;
	Size size;
	Value literal;
	cc_bool index_register_is_address_register;
} Operand;

typedef struct Instruction
{
	Opcode opcode;
	Operand operands[2];
} Instruction;

typedef struct Dc
{
	Size size;
	ValueListNode *values;
} Dc;

typedef struct Dcb
{
	Size size;
	Value repetitions;
	Value value;
} Dcb;

typedef struct Include
{
	char *path;
} Include;

typedef struct StatementIncbin
{
	char *path;
	Value start;
} StatementIncbin;

typedef struct Rept
{
	Value total_repeats;
} Rept;

typedef struct StatementMacro
{
	IdentifierListNode *parameter_names;
} StatementMacro;

typedef struct Statement
{
	enum
	{
		STATEMENT_TYPE_EMPTY,
		STATEMENT_TYPE_INSTRUCTION,
		STATEMENT_TYPE_DC,
		STATEMENT_TYPE_DCB,
		STATEMENT_TYPE_INCLUDE,
		STATEMENT_TYPE_INCBIN,
		STATEMENT_TYPE_REPT,
		STATEMENT_TYPE_ENDR,
		STATEMENT_TYPE_MACRO,
		STATEMENT_TYPE_ENDM,
		STATEMENT_TYPE_EQU,
		STATEMENT_TYPE_IF,
		STATEMENT_TYPE_ELSE,
		STATEMENT_TYPE_ENDC,
		STATEMENT_TYPE_EVEN
	} type;
	union
	{
		Instruction instruction;
		Dc dc;
		Dcb dcb;
		Include include;
		StatementIncbin incbin;
		Rept rept;
		StatementMacro macro;
		Value value;
	} data;
} Statement;

typedef struct ListMetadata
{
	void *head;
	void *tail;
} ListMetadata;

}

%code {

#include <stddef.h>
#include <stdlib.h>

int m68kasm_lex(M68KASM_STYPE *yylval_param, void *yyscanner);
void m68kasm_warning(void *scanner, Statement *statement, const char *message);
void m68kasm_error(void *scanner, Statement *statement, const char *message);

static cc_bool DoValue(Value *value, ValueType type, Value *left_value, Value *right_value);

}

%union {
	struct
	{
		unsigned long integer;
		char *string;
	} generic;
	Opcode opcode;
	Operand operand;
	Instruction instruction;
	Statement statement;
	ListMetadata list_metadata;
	Value value;
}

%token TOKEN_OPCODE_ORI
%token TOKEN_OPCODE_ANDI
%token TOKEN_OPCODE_SUBI
%token TOKEN_OPCODE_ADDI
%token TOKEN_OPCODE_EORI
%token TOKEN_OPCODE_CMPI
%token TOKEN_OPCODE_BTST
%token TOKEN_OPCODE_BCHG
%token TOKEN_OPCODE_BCLR
%token TOKEN_OPCODE_BSET
%token TOKEN_OPCODE_MOVEP
%token TOKEN_OPCODE_MOVEA
%token TOKEN_OPCODE_MOVE
%token TOKEN_OPCODE_NEGX
%token TOKEN_OPCODE_CLR
%token TOKEN_OPCODE_NEG
%token TOKEN_OPCODE_NOT
%token TOKEN_OPCODE_EXT
%token TOKEN_OPCODE_NBCD
%token TOKEN_OPCODE_SWAP
%token TOKEN_OPCODE_PEA
%token TOKEN_OPCODE_ILLEGAL
%token TOKEN_OPCODE_TAS
%token TOKEN_OPCODE_TST
%token TOKEN_OPCODE_TRAP
%token TOKEN_OPCODE_LINK
%token TOKEN_OPCODE_UNLK
%token TOKEN_OPCODE_RESET
%token TOKEN_OPCODE_NOP
%token TOKEN_OPCODE_STOP
%token TOKEN_OPCODE_RTE
%token TOKEN_OPCODE_RTS
%token TOKEN_OPCODE_TRAPV
%token TOKEN_OPCODE_RTR
%token TOKEN_OPCODE_JSR
%token TOKEN_OPCODE_JMP
%token TOKEN_OPCODE_MOVEM
%token TOKEN_OPCODE_LEA
%token TOKEN_OPCODE_CHK
%token TOKEN_OPCODE_ADDQ
%token TOKEN_OPCODE_SUBQ
%token TOKEN_OPCODE_ST
%token TOKEN_OPCODE_SF
%token TOKEN_OPCODE_SHI
%token TOKEN_OPCODE_SLS
%token TOKEN_OPCODE_SCC
%token TOKEN_OPCODE_SCS
%token TOKEN_OPCODE_SNE
%token TOKEN_OPCODE_SEQ
%token TOKEN_OPCODE_SVC
%token TOKEN_OPCODE_SVS
%token TOKEN_OPCODE_SPL
%token TOKEN_OPCODE_SMI
%token TOKEN_OPCODE_SGE
%token TOKEN_OPCODE_SLT
%token TOKEN_OPCODE_SGT
%token TOKEN_OPCODE_SLE
%token TOKEN_OPCODE_DBT
%token TOKEN_OPCODE_DBF
%token TOKEN_OPCODE_DBHI
%token TOKEN_OPCODE_DBLS
%token TOKEN_OPCODE_DBCC
%token TOKEN_OPCODE_DBCS
%token TOKEN_OPCODE_DBNE
%token TOKEN_OPCODE_DBEQ
%token TOKEN_OPCODE_DBVC
%token TOKEN_OPCODE_DBVS
%token TOKEN_OPCODE_DBPL
%token TOKEN_OPCODE_DBMI
%token TOKEN_OPCODE_DBGE
%token TOKEN_OPCODE_DBLT
%token TOKEN_OPCODE_DBGT
%token TOKEN_OPCODE_DBLE
%token TOKEN_OPCODE_BRA
%token TOKEN_OPCODE_BSR
%token TOKEN_OPCODE_BHI
%token TOKEN_OPCODE_BLS
%token TOKEN_OPCODE_BCC
%token TOKEN_OPCODE_BCS
%token TOKEN_OPCODE_BNE
%token TOKEN_OPCODE_BEQ
%token TOKEN_OPCODE_BVC
%token TOKEN_OPCODE_BVS
%token TOKEN_OPCODE_BPL
%token TOKEN_OPCODE_BMI
%token TOKEN_OPCODE_BGE
%token TOKEN_OPCODE_BLT
%token TOKEN_OPCODE_BGT
%token TOKEN_OPCODE_BLE
%token TOKEN_OPCODE_MOVEQ
%token TOKEN_OPCODE_DIVU
%token TOKEN_OPCODE_DIVS
%token TOKEN_OPCODE_SBCD
%token TOKEN_OPCODE_OR
%token TOKEN_OPCODE_SUB
%token TOKEN_OPCODE_SUBX
%token TOKEN_OPCODE_SUBA
%token TOKEN_OPCODE_EOR
%token TOKEN_OPCODE_CMPM
%token TOKEN_OPCODE_CMP
%token TOKEN_OPCODE_CMPA
%token TOKEN_OPCODE_MULU
%token TOKEN_OPCODE_MULS
%token TOKEN_OPCODE_ABCD
%token TOKEN_OPCODE_EXG
%token TOKEN_OPCODE_AND
%token TOKEN_OPCODE_ADD
%token TOKEN_OPCODE_ADDX
%token TOKEN_OPCODE_ADDA
%token TOKEN_OPCODE_ASL
%token TOKEN_OPCODE_ASR
%token TOKEN_OPCODE_LSL
%token TOKEN_OPCODE_LSR
%token TOKEN_OPCODE_ROXL
%token TOKEN_OPCODE_ROXR
%token TOKEN_OPCODE_ROL
%token TOKEN_OPCODE_ROR
%token TOKEN_DIRECTIVE_DC
%token TOKEN_DIRECTIVE_DCB
%token TOKEN_DIRECTIVE_REPT
%token TOKEN_DIRECTIVE_ENDR
%token TOKEN_DIRECTIVE_MACRO
%token TOKEN_DIRECTIVE_ENDM
%token TOKEN_DIRECTIVE_INCLUDE
%token TOKEN_DIRECTIVE_INCBIN
%token TOKEN_DIRECTIVE_EQU
%token TOKEN_DIRECTIVE_IF
%token TOKEN_DIRECTIVE_ELSE
%token TOKEN_DIRECTIVE_ENDC
%token TOKEN_DIRECTIVE_EVEN
%token TOKEN_SIZE_BYTE
%token TOKEN_SIZE_SHORT
%token TOKEN_SIZE_WORD
%token TOKEN_SIZE_LONGWORD
%token<generic.integer> TOKEN_DATA_REGISTER
%token<generic.integer> TOKEN_ADDRESS_REGISTER
%token<generic.integer> TOKEN_NUMBER
%token<generic.string> TOKEN_IDENTIFIER
%token<generic.string> TOKEN_STRING
%token TOKEN_STATUS_REGISTER
%token TOKEN_CONDITION_CODE_REGISTER
%token TOKEN_USER_STACK_POINTER_REGISTER
%token TOKEN_PROGRAM_COUNTER
%token TOKEN_LOGICAL_AND
%token TOKEN_LOGICAL_OR
%token TOKEN_EQUALITY
%token TOKEN_INEQUALITY
%token TOKEN_LESS_OR_EQUAL
%token TOKEN_MORE_OR_EQUAL
%token TOKEN_LEFT_SHIFT
%token TOKEN_RIGHT_SHIFT

%type<instruction> instruction
%type<opcode> opcode
%type<generic.integer> size
%type<opcode> full_opcode
%type<instruction> operand_list
%type<operand> operand
%type<generic.integer> register_list
%type<generic.integer> register_span
%type<generic.integer> data_or_address_register
%type<list_metadata> value_list
%type<list_metadata> identifier_list
%type<value> value
%type<value> value1
%type<value> value2
%type<value> value3
%type<value> value4
%type<value> value5
%type<value> value6
%type<value> value7
%type<value> value8
%type<value> value9
%type<value> value10
%type<value> value11

%start statement

%%

statement
	:
	{
		statement->type = STATEMENT_TYPE_EMPTY;
	}
	| instruction
	{
		statement->type = STATEMENT_TYPE_INSTRUCTION;
		statement->data.instruction = $1;
	}
	| TOKEN_DIRECTIVE_DC '.' size value_list
	{
		statement->type = STATEMENT_TYPE_DC;
		statement->data.dc.size = $3;
		statement->data.dc.values = $4.head;
	}
	| TOKEN_DIRECTIVE_DCB '.' size value ',' value
	{
		statement->type = STATEMENT_TYPE_DCB;
		statement->data.dcb.size = $3;
		statement->data.dcb.repetitions = $4;
		statement->data.dcb.value = $6;
	}
	| TOKEN_DIRECTIVE_INCLUDE TOKEN_STRING
	{
		statement->type = STATEMENT_TYPE_INCLUDE;
		statement->data.include.path = $2;
	}
	| TOKEN_DIRECTIVE_INCBIN TOKEN_STRING
	{
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->data.incbin.path = $2;
		statement->data.incbin.start.type = VALUE_NUMBER;
		statement->data.incbin.start.data.integer = 0;
	}
	| TOKEN_DIRECTIVE_INCBIN TOKEN_STRING ',' value
	{
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->data.incbin.path = $2;
		statement->data.incbin.start = $4;
	}
	| TOKEN_DIRECTIVE_REPT value
	{
		statement->type = STATEMENT_TYPE_REPT;
		statement->data.rept.total_repeats = $2;
	}
	| TOKEN_DIRECTIVE_ENDR
	{
		statement->type = STATEMENT_TYPE_ENDR;
	}
	| TOKEN_DIRECTIVE_MACRO
	{
		statement->type = STATEMENT_TYPE_MACRO;
		statement->data.macro.parameter_names = NULL;
	}
	| TOKEN_DIRECTIVE_MACRO identifier_list
	{
		statement->type = STATEMENT_TYPE_MACRO;
		statement->data.macro.parameter_names = $2.head;
	}
	| TOKEN_DIRECTIVE_ENDM
	{
		statement->type = STATEMENT_TYPE_ENDM;
	}
	| TOKEN_DIRECTIVE_EQU value
	{
		statement->type = STATEMENT_TYPE_EQU;
		statement->data.value = $2;
	}
	| TOKEN_DIRECTIVE_IF value
	{
		statement->type = STATEMENT_TYPE_IF;
		statement->data.value = $2;
	}
	| TOKEN_DIRECTIVE_ELSE
	{
		statement->type = STATEMENT_TYPE_ELSE;
	}
	| TOKEN_DIRECTIVE_ENDC
	{
		statement->type = STATEMENT_TYPE_ENDC;
	}
	| TOKEN_DIRECTIVE_EVEN
	{
		statement->type = STATEMENT_TYPE_EVEN;
	}
	;

value_list
	: value
	{
		ValueListNode *node = malloc(sizeof(ValueListNode));

		if (node == NULL)
		{
			YYNOMEM;
		}
		else
		{
			node->type = VALUE_LIST_NODE_TYPE_VALUE;
			node->shared.value = $1;
			node->next = NULL;
		}

		$$.head = $$.tail = node;
	}
	| value_list ',' value
	{
		ValueListNode *node = malloc(sizeof(ValueListNode));

		$$ = $1;

		if (node == NULL)
		{
			YYNOMEM;
		}
		else
		{
			node->type = VALUE_LIST_NODE_TYPE_VALUE;
			node->shared.value = $3;
			node->next = NULL;

			if ($$.head == NULL)
				$$.head = node;
			else
				((ValueListNode*)$$.tail)->next = node;

			$$.tail = node;
		}
	}
	| TOKEN_STRING
	{
		ValueListNode *node = malloc(sizeof(ValueListNode));

		if (node == NULL)
		{
			YYNOMEM;
		}
		else
		{
			node->type = VALUE_LIST_NODE_TYPE_STRING;
			node->shared.string = $1;
			node->next = NULL;
		}

		$$.head = $$.tail = node;
	}
	| value_list ',' TOKEN_STRING
	{
		ValueListNode *node = malloc(sizeof(ValueListNode));

		$$ = $1;

		if (node == NULL)
		{
			YYNOMEM;
		}
		else
		{
			node->type = VALUE_LIST_NODE_TYPE_STRING;
			node->shared.string = $3;
			node->next = NULL;

			if ($$.head == NULL)
				$$.head = node;
			else
				((ValueListNode*)$$.tail)->next = node;

			$$.tail = node;
		}
	}
	;

identifier_list
	: TOKEN_IDENTIFIER
	{
		IdentifierListNode *node = malloc(sizeof(IdentifierListNode));

		if (node == NULL)
		{
			YYNOMEM;
		}
		else
		{
			node->identifier = $1;
			node->next = NULL;
		}

		$$.head = $$.tail = node;
	}
	| identifier_list ',' TOKEN_IDENTIFIER
	{
		IdentifierListNode *node = malloc(sizeof(IdentifierListNode));

		$$ = $1;

		if (node == NULL)
		{
			YYNOMEM;
		}
		else
		{
			node->identifier = $3;
			node->next = NULL;

			if ($$.head == NULL)
				$$.head = node;
			else
				((IdentifierListNode*)$$.tail)->next = node;

			$$.tail = node;
		}
	}
	;

instruction
	: full_opcode
	{
		$$.opcode = $1;
		$$.operands[0].type = 0;
		$$.operands[1].type = 0;
	}
	| full_opcode operand_list
	{
		$$ = $2;
		$$.opcode = $1;
	}
	;

full_opcode
	: opcode
	{
		$$ = $1;
		$$.size = SIZE_UNDEFINED;
	}
	| opcode '.'
	{
		$$ = $1;
		$$.size = SIZE_UNDEFINED;
		m68kasm_warning(scanner, statement, "Opcode has a dot but no size; either remove the dot or add an explicit size");
	}
	| opcode '.' size
	{
		$$ = $1;
		$$.size = $3;
	}
	;

opcode
	: TOKEN_OPCODE_ORI
	{
		$$.type = OPCODE_ORI;
	}
	| TOKEN_OPCODE_ANDI
	{
		$$.type = OPCODE_ANDI;
	}
	| TOKEN_OPCODE_SUBI
	{
		$$.type = OPCODE_SUBI;
	}
	| TOKEN_OPCODE_ADDI
	{
		$$.type = OPCODE_ADDI;
	}
	| TOKEN_OPCODE_EORI
	{
		$$.type = OPCODE_EORI;
	}
	| TOKEN_OPCODE_CMPI
	{
		$$.type = OPCODE_CMPI;
	}
	| TOKEN_OPCODE_BTST
	{
		$$.type = OPCODE_BTST_STATIC;
	}
	| TOKEN_OPCODE_BCHG
	{
		$$.type = OPCODE_BCHG_STATIC;
	}
	| TOKEN_OPCODE_BCLR
	{
		$$.type = OPCODE_BCLR_STATIC;
	}
	| TOKEN_OPCODE_BSET
	{
		$$.type = OPCODE_BSET_STATIC;
	}
	| TOKEN_OPCODE_MOVEP
	{
		$$.type = OPCODE_MOVEP_TO_REG;
	}
	| TOKEN_OPCODE_MOVEA
	{
		$$.type = OPCODE_MOVEA;
	}
	| TOKEN_OPCODE_MOVE
	{
		$$.type = OPCODE_MOVE;
	}
	| TOKEN_OPCODE_NEGX
	{
		$$.type = OPCODE_NEGX;
	}
	| TOKEN_OPCODE_CLR
	{
		$$.type = OPCODE_CLR;
	}
	| TOKEN_OPCODE_NEG
	{
		$$.type = OPCODE_NEG;
	}
	| TOKEN_OPCODE_NOT
	{
		$$.type = OPCODE_NOT;
	}
	| TOKEN_OPCODE_EXT
	{
		$$.type = OPCODE_EXT;
	}
	| TOKEN_OPCODE_NBCD
	{
		$$.type = OPCODE_NBCD;
	}
	| TOKEN_OPCODE_SWAP
	{
		$$.type = OPCODE_SWAP;
	}
	| TOKEN_OPCODE_PEA
	{
		$$.type = OPCODE_PEA;
	}
	| TOKEN_OPCODE_ILLEGAL
	{
		$$.type = OPCODE_ILLEGAL;
	}
	| TOKEN_OPCODE_TAS
	{
		$$.type = OPCODE_TAS;
	}
	| TOKEN_OPCODE_TST
	{
		$$.type = OPCODE_TST;
	}
	| TOKEN_OPCODE_TRAP
	{
		$$.type = OPCODE_TRAP;
	}
	| TOKEN_OPCODE_LINK
	{
		$$.type = OPCODE_LINK;
	}
	| TOKEN_OPCODE_UNLK
	{
		$$.type = OPCODE_UNLK;
	}
	| TOKEN_OPCODE_RESET
	{
		$$.type = OPCODE_RESET;
	}
	| TOKEN_OPCODE_NOP
	{
		$$.type = OPCODE_NOP;
	}
	| TOKEN_OPCODE_STOP
	{
		$$.type = OPCODE_STOP;
	}
	| TOKEN_OPCODE_RTE
	{
		$$.type = OPCODE_RTE;
	}
	| TOKEN_OPCODE_RTS
	{
		$$.type = OPCODE_RTS;
	}
	| TOKEN_OPCODE_TRAPV
	{
		$$.type = OPCODE_TRAPV;
	}
	| TOKEN_OPCODE_RTR
	{
		$$.type = OPCODE_RTR;
	}
	| TOKEN_OPCODE_JSR
	{
		$$.type = OPCODE_JSR;
	}
	| TOKEN_OPCODE_JMP
	{
		$$.type = OPCODE_JMP;
	}
	| TOKEN_OPCODE_MOVEM
	{
		$$.type = OPCODE_MOVEM_TO_REGS;
	}
	| TOKEN_OPCODE_LEA
	{
		$$.type = OPCODE_LEA;
	}
	| TOKEN_OPCODE_CHK
	{
		$$.type = OPCODE_CHK;
	}
	| TOKEN_OPCODE_ADDQ
	{
		$$.type = OPCODE_ADDQ;
	}
	| TOKEN_OPCODE_SUBQ
	{
		$$.type = OPCODE_SUBQ;
	}
	| TOKEN_OPCODE_ST
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_TRUE;
	}
	| TOKEN_OPCODE_SF
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_FALSE;
	}
	| TOKEN_OPCODE_SHI
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_HIGHER;
	}
	| TOKEN_OPCODE_SLS
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_LOWER_OR_SAME;
	}
	| TOKEN_OPCODE_SCC
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_CARRY_CLEAR;
	}
	| TOKEN_OPCODE_SCS
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_CARRY_SET;
	}
	| TOKEN_OPCODE_SNE
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_NOT_EQUAL;
	}
	| TOKEN_OPCODE_SEQ
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_EQUAL;
	}
	| TOKEN_OPCODE_SVC
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_OVERFLOW_CLEAR;
	}
	| TOKEN_OPCODE_SVS
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_OVERFLOW_SET;
	}
	| TOKEN_OPCODE_SPL
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_PLUS;
	}
	| TOKEN_OPCODE_SMI
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_MINUS;
	}
	| TOKEN_OPCODE_SGE
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_GREATER_OR_EQUAL;
	}
	| TOKEN_OPCODE_SLT
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_LESS_THAN;
	}
	| TOKEN_OPCODE_SGT
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_GREATER_THAN;
	}
	| TOKEN_OPCODE_SLE
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_LESS_OR_EQUAL;
	}
	| TOKEN_OPCODE_DBT
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_TRUE;
	}
	| TOKEN_OPCODE_DBF
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_FALSE;
	}
	| TOKEN_OPCODE_DBHI
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_HIGHER;
	}
	| TOKEN_OPCODE_DBLS
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_LOWER_OR_SAME;
	}
	| TOKEN_OPCODE_DBCC
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_CARRY_CLEAR;
	}
	| TOKEN_OPCODE_DBCS
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_CARRY_SET;
	}
	| TOKEN_OPCODE_DBNE
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_NOT_EQUAL;
	}
	| TOKEN_OPCODE_DBEQ
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_EQUAL;
	}
	| TOKEN_OPCODE_DBVC
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_OVERFLOW_CLEAR;
	}
	| TOKEN_OPCODE_DBVS
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_OVERFLOW_SET;
	}
	| TOKEN_OPCODE_DBPL
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_PLUS;
	}
	| TOKEN_OPCODE_DBMI
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_MINUS;
	}
	| TOKEN_OPCODE_DBGE
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_GREATER_OR_EQUAL;
	}
	| TOKEN_OPCODE_DBLT
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_LESS_THAN;
	}
	| TOKEN_OPCODE_DBGT
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_GREATER_THAN;
	}
	| TOKEN_OPCODE_DBLE
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_LESS_OR_EQUAL;
	}
	| TOKEN_OPCODE_BRA
	{
		$$.type = OPCODE_BRA;
	}
	| TOKEN_OPCODE_BSR
	{
		$$.type = OPCODE_BSR;
	}
	| TOKEN_OPCODE_BHI
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_HIGHER;
	}
	| TOKEN_OPCODE_BLS
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_LOWER_OR_SAME;
	}
	| TOKEN_OPCODE_BCC
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_CARRY_CLEAR;
	}
	| TOKEN_OPCODE_BCS
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_CARRY_SET;
	}
	| TOKEN_OPCODE_BNE
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_NOT_EQUAL;
	}
	| TOKEN_OPCODE_BEQ
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_EQUAL;
	}
	| TOKEN_OPCODE_BVC
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_OVERFLOW_CLEAR;
	}
	| TOKEN_OPCODE_BVS
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_OVERFLOW_SET;
	}
	| TOKEN_OPCODE_BPL
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_PLUS;
	}
	| TOKEN_OPCODE_BMI
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_MINUS;
	}
	| TOKEN_OPCODE_BGE
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_GREATER_OR_EQUAL;
	}
	| TOKEN_OPCODE_BLT
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_LESS_THAN;
	}
	| TOKEN_OPCODE_BGT
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_GREATER_THAN;
	}
	| TOKEN_OPCODE_BLE
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_LESS_OR_EQUAL;
	}
	| TOKEN_OPCODE_MOVEQ
	{
		$$.type = OPCODE_MOVEQ;
	}
	| TOKEN_OPCODE_DIVU
	{
		$$.type = OPCODE_DIVU;
	}
	| TOKEN_OPCODE_DIVS
	{
		$$.type = OPCODE_DIVS;
	}
	| TOKEN_OPCODE_SBCD
	{
		$$.type = OPCODE_SBCD_DATA_REGS;
	}
	| TOKEN_OPCODE_OR
	{
		$$.type = OPCODE_OR_TO_REG;
	}
	| TOKEN_OPCODE_SUB
	{
		$$.type = OPCODE_SUB_TO_REG;
	}
	| TOKEN_OPCODE_SUBX
	{
		$$.type = OPCODE_SUBX_DATA_REGS;
	}
	| TOKEN_OPCODE_SUBA
	{
		$$.type = OPCODE_SUBA;
	}
	| TOKEN_OPCODE_EOR
	{
		$$.type = OPCODE_EOR;
	}
	| TOKEN_OPCODE_CMPM
	{
		$$.type = OPCODE_CMPM;
	}
	| TOKEN_OPCODE_CMP
	{
		$$.type = OPCODE_CMP;
	}
	| TOKEN_OPCODE_CMPA
	{
		$$.type = OPCODE_CMPA;
	}
	| TOKEN_OPCODE_MULU
	{
		$$.type = OPCODE_MULU;
	}
	| TOKEN_OPCODE_MULS
	{
		$$.type = OPCODE_MULS;
	}
	| TOKEN_OPCODE_ABCD
	{
		$$.type = OPCODE_ABCD_DATA_REGS;
	}
	| TOKEN_OPCODE_EXG
	{
		$$.type = OPCODE_EXG;
	}
	| TOKEN_OPCODE_AND
	{
		$$.type = OPCODE_AND_TO_REG;
	}
	| TOKEN_OPCODE_ADD
	{
		$$.type = OPCODE_ADD_TO_REG;
	}
	| TOKEN_OPCODE_ADDX
	{
		$$.type = OPCODE_ADDX_DATA_REGS;
	}
	| TOKEN_OPCODE_ADDA
	{
		$$.type = OPCODE_ADDA;
	}
	| TOKEN_OPCODE_ASL
	{
		$$.type = OPCODE_ASL_STATIC;
	}
	| TOKEN_OPCODE_ASR
	{
		$$.type = OPCODE_ASR_STATIC;
	}
	| TOKEN_OPCODE_LSL
	{
		$$.type = OPCODE_LSL_STATIC;
	}
	| TOKEN_OPCODE_LSR
	{
		$$.type = OPCODE_LSR_STATIC;
	}
	| TOKEN_OPCODE_ROXL
	{
		$$.type = OPCODE_ROXL_STATIC;
	}
	| TOKEN_OPCODE_ROXR
	{
		$$.type = OPCODE_ROXR_STATIC;
	}
	| TOKEN_OPCODE_ROL
	{
		$$.type = OPCODE_ROL_STATIC;
	}
	| TOKEN_OPCODE_ROR
	{
		$$.type = OPCODE_ROR_STATIC;
	}
	;

size
	: TOKEN_SIZE_BYTE
	{
		$$ = SIZE_BYTE;
	}
	| TOKEN_SIZE_SHORT
	{
		$$ = SIZE_SHORT;
	}
	| TOKEN_SIZE_WORD
	{
		$$ = SIZE_WORD;
	}
	| TOKEN_SIZE_LONGWORD
	{
		$$ = SIZE_LONGWORD;
	}
	;

operand_list
	: operand
	{
		$$.operands[0] = $1;
		$$.operands[1].type = 0;
	}
	| operand_list ',' operand
	{
		$$ = $1;

		if ($$.operands[1].type != 0)
		{
			m68kasm_error(scanner, statement, "Instructions can never have more than two operands");
			YYABORT;
		}
		else
		{
			$$.operands[1] = $3;
		}
	}
	;

	/* Indirect address register */
operand
	: '(' TOKEN_ADDRESS_REGISTER ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT;
		$$.main_register = $2;
	}
	| '(' TOKEN_ADDRESS_REGISTER ')' '+'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
		$$.main_register = $2;
	}
	| '-' '(' TOKEN_ADDRESS_REGISTER ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
		$$.main_register = $3;
	}
	| value '(' TOKEN_ADDRESS_REGISTER ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		$$.literal = $1;
		$$.main_register = $3;
	}
	| '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register '.' size ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = VALUE_NUMBER;
		$$.literal.data.integer = 0;
		$$.main_register = $2;
		$$.index_register = $4 % 8;
		$$.size = $6;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| value '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register '.' size ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.main_register = $3;
		$$.index_register = $5 % 8;
		$$.size = $7;
		$$.index_register_is_address_register = $5 / 8 !=0;
	}
	| '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register ')'
	{
		m68kasm_warning(scanner, statement, "Index register lacks a size specifier - assuming word-size for now, but you should really add an explicit size");
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = VALUE_NUMBER;
		$$.literal.data.integer = 0;
		$$.main_register = $2;
		$$.index_register = $4 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| value '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register ')'
	{
		m68kasm_warning(scanner, statement, "Index register lacks a size specifier - assuming word-size for now, but you should really add an explicit size");
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.main_register = $3;
		$$.index_register = $5 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $5 / 8 !=0;
	}
	| value '(' TOKEN_PROGRAM_COUNTER ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		$$.literal = $1;
	}
	| '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register '.' size ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = VALUE_NUMBER;
		$$.literal.data.integer = 0;
		$$.index_register = $4 % 8;
		$$.size = $6;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| value '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register '.' size ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.index_register = $5 % 8;
		$$.size = $7;
		$$.index_register_is_address_register = $5 / 8 != 0;
	}
	| '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register ')'
	{
		m68kasm_warning(scanner, statement, "Index register lacks a size specifier - assuming word-size for now, but you should really add an explicit size");
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = VALUE_NUMBER;
		$$.literal.data.integer = 0;
		$$.index_register = $4 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| value '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register ')'
	{
		m68kasm_warning(scanner, statement, "Index register lacks a size specifier - assuming word-size for now, but you should really add an explicit size");
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.index_register = $5 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $5 / 8 != 0;
	}
	/* Literal */
	| '#' value
	{
		$$.type = OPERAND_LITERAL;
		$$.literal = $2;
	}
	/* Registers */
/*	| TOKEN_DATA_REGISTER
	{
		$$.type = OPERAND_DATA_REGISTER;
		$$.main_register = $1;
	}
	| TOKEN_ADDRESS_REGISTER
	{
		$$.type = OPERAND_ADDRESS_REGISTER;
		$$.main_register = $1;
	}
*/	| TOKEN_STATUS_REGISTER
	{
		$$.type = OPERAND_STATUS_REGISTER;
	}
	| TOKEN_CONDITION_CODE_REGISTER
	{
		$$.type = OPERAND_CONDITION_CODE_REGISTER;
	}
	| TOKEN_USER_STACK_POINTER_REGISTER
	{
		$$.type = OPERAND_USER_STACK_POINTER_REGISTER;
	}
	/* Addresses */
	| value
	{
		$$.type = OPERAND_ADDRESS;
		$$.literal = $1;
		$$.size = SIZE_UNDEFINED;
	}
	| '(' value ')' '.' size
	{
		$$.type = OPERAND_ADDRESS_ABSOLUTE;
		$$.literal = $2;
		$$.size = $5;
	}
	/* Register list */
	| register_list
	{
		/* Neat trick to check if only one bit is set (yes, I know that it evaluates to true for 0, but that doesn't matter here). */
		if (($1 & ($1 - 1)) == 0)
		{
			/* This is a single register. */
			$$.main_register = 0;

			while (($1 & (1 << $$.main_register)) == 0)
				++$$.main_register;

			if ($$.main_register >= 8)
			{
				$$.type = OPERAND_ADDRESS_REGISTER;
				$$.main_register -= 8;
			}
			else
			{
				$$.type = OPERAND_DATA_REGISTER;
			}
		}
		else
		{
			/* This is multiple registers. */
			$$.type = OPERAND_REGISTER_LIST;
			$$.main_register = $1; /* Such a hack... */
		}
	}
	;

register_list
	: register_span
	{
		$$ = $1;
	}
	| register_span '/' register_list
	{
		$$ = $1 | $3;
	}
	;

register_span
	: data_or_address_register
	{
		$$ = 1 << $1;
	}
	| data_or_address_register '-' data_or_address_register
	{
		unsigned long start;
		unsigned long end;
		unsigned long i;

		if ($1 < $3)
		{
			start = $1;
			end = $3;
		}
		else
		{
			start = $3;
			end = $1;
		}

		$$ = 0;

		for (i = start; i <= end; ++i)
			$$ |= 1 << i;
	}
	;

data_or_address_register
	: TOKEN_DATA_REGISTER
	{
		$$ = 0 + $1;
	}
	| TOKEN_ADDRESS_REGISTER
	{
		$$ = 8 + $1;
	}
	;

/*
number
	: TOKEN_NUMBER
	| '$' TOKEN_NUMBER
	;
*/

value
	: value1
	{
		$$ = $1;
	}
	| value1 TOKEN_LOGICAL_OR value
	{
		if (!DoValue(&$$, VALUE_LOGICAL_OR, &$1, &$3))
			YYNOMEM;
	}
	;

value1
	: value2
	{
		$$ = $1;
	}
	| value2 TOKEN_LOGICAL_AND value1
	{
		if (!DoValue(&$$, VALUE_LOGICAL_AND, &$1, &$3))
			YYNOMEM;
	}
	;

value2
	: value3
	{
		$$ = $1;
	}
	| value3 '|' value2
	{
		if (!DoValue(&$$, VALUE_ARITHMETIC_OR, &$1, &$3))
			YYNOMEM;
	}
	;

value3
	: value4
	{
		$$ = $1;
	}
	| value4 '^' value3
	{
		if (!DoValue(&$$, VALUE_ARITHMETIC_XOR, &$1, &$3))
			YYNOMEM;
	}
	;

value4
	: value5
	{
		$$ = $1;
	}
	| value5 '&' value4
	{
		if (!DoValue(&$$, VALUE_ARITHMETIC_AND, &$1, &$3))
			YYNOMEM;
	}
	;

value5
	: value6
	{
		$$ = $1;
	}
	| value6 '=' value5
	{
		if (!DoValue(&$$, VALUE_EQUALITY, &$1, &$3))
			YYNOMEM;
	}
	| value6 TOKEN_EQUALITY value5
	{
		if (!DoValue(&$$, VALUE_EQUALITY, &$1, &$3))
			YYNOMEM;
	}
	| value6 TOKEN_INEQUALITY value5
	{
		if (!DoValue(&$$, VALUE_INEQUALITY, &$1, &$3))
			YYNOMEM;
	}
	;

value6
	: value7
	{
		$$ = $1;
	}
	| value7 '<' value6
	{
		if (!DoValue(&$$, VALUE_LESS_THAN, &$1, &$3))
			YYNOMEM;
	}
	| value7 TOKEN_LESS_OR_EQUAL value6
	{
		if (!DoValue(&$$, VALUE_LESS_OR_EQUAL, &$1, &$3))
			YYNOMEM;
	}
	| value7 '>' value6
	{
		if (!DoValue(&$$, VALUE_MORE_THAN, &$1, &$3))
			YYNOMEM;
	}
	| value7 TOKEN_MORE_OR_EQUAL value6
	{
		if (!DoValue(&$$, VALUE_MORE_OR_EQUAL, &$1, &$3))
			YYNOMEM;
	}
	;

value7
	: value8
	{
		$$ = $1;
	}
	| value8 TOKEN_LEFT_SHIFT value7
	{
		if (!DoValue(&$$, VALUE_LEFT_SHIFT, &$1, &$3))
			YYNOMEM;
	}
	| value8 TOKEN_RIGHT_SHIFT value7
	{
		if (!DoValue(&$$, VALUE_RIGHT_SHIFT, &$1, &$3))
			YYNOMEM;
	}
	;

value8
	: value9
	{
		$$ = $1;
	}
	| value9 '+' value8
	{
		if (!DoValue(&$$, VALUE_ADD, &$1, &$3))
			YYNOMEM;
	}
	| value9 '-' value8
	{
		if (!DoValue(&$$, VALUE_SUBTRACT, &$1, &$3))
			YYNOMEM;
	}
	;

value9
	: value10
	{
		$$ = $1;
	}
	| value10 '*' value9
	{
		if (!DoValue(&$$, VALUE_MULTIPLY, &$1, &$3))
			YYNOMEM;
	}
	| value10 '/' value9
	{
		if (!DoValue(&$$, VALUE_DIVIDE, &$1, &$3))
			YYNOMEM;
	}
	| value10 '%' value9
	{
		if (!DoValue(&$$, VALUE_MODULO, &$1, &$3))
			YYNOMEM;
	}
	;

value10
	: value11
	{
		$$ = $1;
	}
	| '-' value10
	{
		if (!DoValue(&$$, VALUE_NEGATE, &$2, NULL))
			YYNOMEM;
	}
	| '~' value10
	{
		if (!DoValue(&$$, VALUE_BITWISE_NOT, &$2, NULL))
			YYNOMEM;
	}
	| '!' value10
	{
		if (!DoValue(&$$, VALUE_LOGICAL_NOT, &$2, NULL))
			YYNOMEM;
	}
	;

value11
	: TOKEN_NUMBER
	{
		$$.type = VALUE_NUMBER;
		$$.data.integer = $1;
	}
	| TOKEN_IDENTIFIER
	{
		$$.type = VALUE_IDENTIFIER;
		$$.data.identifier = $1;
	}
	| '*'
	{
		$$.type = VALUE_IDENTIFIER;
		$$.data.identifier = ",,PROGRAM_COUNTER,,";
	}
	| '(' value ')'
	{
		$$ = $2;
	}
	;

%%

static cc_bool DoValue(Value *value, ValueType type, Value *left_value, Value *right_value)
{
	cc_bool success = cc_true;

	value->type = type;

	value->data.values = malloc(sizeof(Value) * (right_value != NULL ? 2 : 1));

	if (value->data.values == NULL)
	{
		success = cc_false;
	}
	else
	{
		value->data.values[0] = *left_value;

		if (right_value != NULL)
			value->data.values[1] = *right_value;
	}

	return success;
}
