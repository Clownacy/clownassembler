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

/*%define parse.trace*/

%define api.pure

%define api.prefix {m68kasm_}

%param {void *scanner}

%parse-param {Statement *statement}

%define parse.error verbose

%code requires {

#include "clowncommon.h"

#define YYSTYPE M68KASM_STYPE

#define CREATE_LIST_TYPE(TYPE)\
typedef struct TYPE\
{\
	TYPE##Node *head;\
	TYPE##Node *tail;\
} TYPE

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

typedef enum ExpressionType
{
	EXPRESSION_SUBTRACT,
	EXPRESSION_ADD,
	EXPRESSION_MULTIPLY,
	EXPRESSION_DIVIDE,
	EXPRESSION_MODULO,
	EXPRESSION_NEGATE,
	EXPRESSION_LOGICAL_NOT,
	EXPRESSION_LOGICAL_OR,
	EXPRESSION_LOGICAL_AND,
	EXPRESSION_BITWISE_NOT,
	EXPRESSION_BITWISE_OR,
	EXPRESSION_BITWISE_XOR,
	EXPRESSION_BITWISE_AND,
	EXPRESSION_EQUALITY,
	EXPRESSION_INEQUALITY,
	EXPRESSION_LESS_THAN,
	EXPRESSION_LESS_OR_EQUAL,
	EXPRESSION_MORE_THAN,
	EXPRESSION_MORE_OR_EQUAL,
	EXPRESSION_LEFT_SHIFT,
	EXPRESSION_RIGHT_SHIFT,
	EXPRESSION_NUMBER,
	EXPRESSION_IDENTIFIER,
	EXPRESSION_STRING,
	EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT,
	EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION,
	EXPRESSION_STRLEN
} ExpressionType;

typedef struct Expression
{
	ExpressionType type;
	union
	{
		unsigned long unsigned_long;
		char *string;
		struct Expression *subexpressions;
	} shared;
} Expression;

typedef struct ExpressionListNode
{
	struct ExpressionListNode *next;

	Expression expression;
} ExpressionListNode;

CREATE_LIST_TYPE(ExpressionList);

typedef struct IdentifierListNode
{
	struct IdentifierListNode *next;

	char *identifier;
} IdentifierListNode;

CREATE_LIST_TYPE(IdentifierList);

typedef struct Opcode
{
	OpcodeType type;
	Condition condition;
	Size size;
} Opcode;

typedef enum OperandType
{
	OPERAND_NONE                                                           = 0,
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
	Expression literal;
	cc_bool index_register_is_address_register;
} Operand;

typedef struct StatementInstruction
{
	Opcode opcode;
	Operand operands[2];
} StatementInstruction;

typedef struct StatementDc
{
	Size size;
	ExpressionList values;
} StatementDc;

typedef struct StatementDcb
{
	Size size;
	Expression repetitions;
	Expression value;
} StatementDcb;

typedef struct StatementInclude
{
	char *path;
} StatementInclude;

typedef struct StatementIncbin
{
	char *path;
	Expression start;
	cc_bool has_length;
	Expression length;
} StatementIncbin;

typedef struct StatementRept
{
	Expression repetitions;
} StatementRept;

typedef struct StatementMacro
{
	IdentifierList parameter_names;
} StatementMacro;

typedef struct StatementInform
{
	char *message;
} StatementInform;

typedef enum StatementType
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
	STATEMENT_TYPE_MACROS,
	STATEMENT_TYPE_ENDM,
	STATEMENT_TYPE_EQU,
	STATEMENT_TYPE_SET,
	STATEMENT_TYPE_IF,
	STATEMENT_TYPE_ELSEIF,
	STATEMENT_TYPE_ELSE,
	STATEMENT_TYPE_ENDC,
	STATEMENT_TYPE_WHILE,
	STATEMENT_TYPE_ENDW,
	STATEMENT_TYPE_EVEN,
	STATEMENT_TYPE_CNOP,
	STATEMENT_TYPE_INFORM,
	STATEMENT_TYPE_END,
	STATEMENT_TYPE_RS,
	STATEMENT_TYPE_RSSET,
	STATEMENT_TYPE_RSRESET
} StatementType;

typedef struct Statement
{
	StatementType type;
	union
	{
		StatementInstruction instruction;
		StatementDc dc;
		StatementDcb dcb;
		StatementInclude include;
		StatementIncbin incbin;
		StatementRept rept;
		StatementMacro macro;
		struct
		{
			Expression offset;
			Expression size_boundary;
		} cnop;
		StatementInform inform;
		struct
		{
			Size size;
			Expression length;
		} rs;
		Expression expression;
	} shared;
} Statement;

}

%code provides {

void DestroyExpression(Expression *expression);
void DestroyStatement(Statement *statement);

}

%code {

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int m68kasm_lex(M68KASM_STYPE *yylval_param, void *yyscanner);
void m68kasm_warning(void *scanner, Statement *statement, const char *message);
void m68kasm_error(void *scanner, Statement *statement, const char *message);

static cc_bool DoExpression(Expression *expression, ExpressionType type, Expression *left_expression, Expression *right_expression);
static void DestroyIdentifierList(IdentifierList *list);
static void DestroyExpressionList(ExpressionList *list);
static void DestroyOperand(Operand *operand);
static void DestroyStatementInstruction(StatementInstruction *instruction);

}

%union {
	unsigned long unsigned_long;
	char *string;
	Opcode opcode;
	Size size;
	Operand operand;
	StatementInstruction instruction;
	Statement statement;
	ExpressionList expression_list;
	IdentifierList identifier_list;
	Expression expression;
}

%token TOKEN_DIRECTIVE_ORI
%token TOKEN_DIRECTIVE_ANDI
%token TOKEN_DIRECTIVE_SUBI
%token TOKEN_DIRECTIVE_ADDI
%token TOKEN_DIRECTIVE_EORI
%token TOKEN_DIRECTIVE_CMPI
%token TOKEN_DIRECTIVE_BTST
%token TOKEN_DIRECTIVE_BCHG
%token TOKEN_DIRECTIVE_BCLR
%token TOKEN_DIRECTIVE_BSET
%token TOKEN_DIRECTIVE_MOVEP
%token TOKEN_DIRECTIVE_MOVEA
%token TOKEN_DIRECTIVE_MOVE
%token TOKEN_DIRECTIVE_NEGX
%token TOKEN_DIRECTIVE_CLR
%token TOKEN_DIRECTIVE_NEG
%token TOKEN_DIRECTIVE_NOT
%token TOKEN_DIRECTIVE_EXT
%token TOKEN_DIRECTIVE_NBCD
%token TOKEN_DIRECTIVE_SWAP
%token TOKEN_DIRECTIVE_PEA
%token TOKEN_DIRECTIVE_ILLEGAL
%token TOKEN_DIRECTIVE_TAS
%token TOKEN_DIRECTIVE_TST
%token TOKEN_DIRECTIVE_TRAP
%token TOKEN_DIRECTIVE_LINK
%token TOKEN_DIRECTIVE_UNLK
%token TOKEN_DIRECTIVE_RESET
%token TOKEN_DIRECTIVE_NOP
%token TOKEN_DIRECTIVE_STOP
%token TOKEN_DIRECTIVE_RTE
%token TOKEN_DIRECTIVE_RTS
%token TOKEN_DIRECTIVE_TRAPV
%token TOKEN_DIRECTIVE_RTR
%token TOKEN_DIRECTIVE_JSR
%token TOKEN_DIRECTIVE_JMP
%token TOKEN_DIRECTIVE_MOVEM
%token TOKEN_DIRECTIVE_LEA
%token TOKEN_DIRECTIVE_CHK
%token TOKEN_DIRECTIVE_ADDQ
%token TOKEN_DIRECTIVE_SUBQ
%token TOKEN_DIRECTIVE_ST
%token TOKEN_DIRECTIVE_SF
%token TOKEN_DIRECTIVE_SHI
%token TOKEN_DIRECTIVE_SLS
%token TOKEN_DIRECTIVE_SCC
%token TOKEN_DIRECTIVE_SCS
%token TOKEN_DIRECTIVE_SNE
%token TOKEN_DIRECTIVE_SEQ
%token TOKEN_DIRECTIVE_SVC
%token TOKEN_DIRECTIVE_SVS
%token TOKEN_DIRECTIVE_SPL
%token TOKEN_DIRECTIVE_SMI
%token TOKEN_DIRECTIVE_SGE
%token TOKEN_DIRECTIVE_SLT
%token TOKEN_DIRECTIVE_SGT
%token TOKEN_DIRECTIVE_SLE
%token TOKEN_DIRECTIVE_DBT
%token TOKEN_DIRECTIVE_DBF
%token TOKEN_DIRECTIVE_DBHI
%token TOKEN_DIRECTIVE_DBLS
%token TOKEN_DIRECTIVE_DBCC
%token TOKEN_DIRECTIVE_DBCS
%token TOKEN_DIRECTIVE_DBNE
%token TOKEN_DIRECTIVE_DBEQ
%token TOKEN_DIRECTIVE_DBVC
%token TOKEN_DIRECTIVE_DBVS
%token TOKEN_DIRECTIVE_DBPL
%token TOKEN_DIRECTIVE_DBMI
%token TOKEN_DIRECTIVE_DBGE
%token TOKEN_DIRECTIVE_DBLT
%token TOKEN_DIRECTIVE_DBGT
%token TOKEN_DIRECTIVE_DBLE
%token TOKEN_DIRECTIVE_BRA
%token TOKEN_DIRECTIVE_BSR
%token TOKEN_DIRECTIVE_BHI
%token TOKEN_DIRECTIVE_BLS
%token TOKEN_DIRECTIVE_BCC
%token TOKEN_DIRECTIVE_BCS
%token TOKEN_DIRECTIVE_BNE
%token TOKEN_DIRECTIVE_BEQ
%token TOKEN_DIRECTIVE_BVC
%token TOKEN_DIRECTIVE_BVS
%token TOKEN_DIRECTIVE_BPL
%token TOKEN_DIRECTIVE_BMI
%token TOKEN_DIRECTIVE_BGE
%token TOKEN_DIRECTIVE_BLT
%token TOKEN_DIRECTIVE_BGT
%token TOKEN_DIRECTIVE_BLE
%token TOKEN_DIRECTIVE_MOVEQ
%token TOKEN_DIRECTIVE_DIVU
%token TOKEN_DIRECTIVE_DIVS
%token TOKEN_DIRECTIVE_SBCD
%token TOKEN_DIRECTIVE_OR
%token TOKEN_DIRECTIVE_SUB
%token TOKEN_DIRECTIVE_SUBX
%token TOKEN_DIRECTIVE_SUBA
%token TOKEN_DIRECTIVE_EOR
%token TOKEN_DIRECTIVE_CMPM
%token TOKEN_DIRECTIVE_CMP
%token TOKEN_DIRECTIVE_CMPA
%token TOKEN_DIRECTIVE_MULU
%token TOKEN_DIRECTIVE_MULS
%token TOKEN_DIRECTIVE_ABCD
%token TOKEN_DIRECTIVE_EXG
%token TOKEN_DIRECTIVE_AND
%token TOKEN_DIRECTIVE_ADD
%token TOKEN_DIRECTIVE_ADDX
%token TOKEN_DIRECTIVE_ADDA
%token TOKEN_DIRECTIVE_ASL
%token TOKEN_DIRECTIVE_ASR
%token TOKEN_DIRECTIVE_LSL
%token TOKEN_DIRECTIVE_LSR
%token TOKEN_DIRECTIVE_ROXL
%token TOKEN_DIRECTIVE_ROXR
%token TOKEN_DIRECTIVE_ROL
%token TOKEN_DIRECTIVE_ROR
%token TOKEN_DIRECTIVE_DC
%token TOKEN_DIRECTIVE_DCB
%token TOKEN_DIRECTIVE_REPT
%token TOKEN_DIRECTIVE_ENDR
%token TOKEN_DIRECTIVE_MACRO
%token TOKEN_DIRECTIVE_MACROS
%token TOKEN_DIRECTIVE_ENDM
%token TOKEN_DIRECTIVE_INCLUDE
%token TOKEN_DIRECTIVE_INCBIN
%token TOKEN_DIRECTIVE_EQU
%token TOKEN_DIRECTIVE_SET
%token TOKEN_DIRECTIVE_IF
%token TOKEN_DIRECTIVE_ELSEIF
%token TOKEN_DIRECTIVE_ELSE
%token TOKEN_DIRECTIVE_ENDC
%token TOKEN_DIRECTIVE_WHILE
%token TOKEN_DIRECTIVE_ENDW
%token TOKEN_DIRECTIVE_EVEN
%token TOKEN_DIRECTIVE_CNOP
%token TOKEN_DIRECTIVE_INFORM
%token TOKEN_DIRECTIVE_END
%token TOKEN_DIRECTIVE_RS
%token TOKEN_DIRECTIVE_RSSET
%token TOKEN_DIRECTIVE_RSRESET
%token TOKEN_SIZE_BYTE
%token TOKEN_SIZE_SHORT
%token TOKEN_SIZE_WORD
%token TOKEN_SIZE_LONGWORD
%token<unsigned_long> TOKEN_DATA_REGISTER
%token<unsigned_long> TOKEN_ADDRESS_REGISTER
%token<unsigned_long> TOKEN_NUMBER
%token<string> TOKEN_IDENTIFIER
%token<string> TOKEN_LOCAL_IDENTIFIER
%token<string> TOKEN_STRING
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
%token TOKEN_STRLEN

%type<instruction> instruction
%type<opcode> opcode
%type<size> size
%type<opcode> full_opcode
%type<operand> operand
%type<unsigned_long> register_list
%type<unsigned_long> register_span
%type<unsigned_long> data_or_address_register
%type<expression_list> expression_list
%type<identifier_list> identifier_list
%type<expression> expression expression1 expression2 expression3 expression4 expression5 expression6 expression7 expression8

%destructor { free($$); } TOKEN_IDENTIFIER TOKEN_LOCAL_IDENTIFIER TOKEN_STRING
%destructor { DestroyOperand(&$$); } operand
%destructor { DestroyExpressionList(&$$); } expression_list
%destructor { DestroyIdentifierList(&$$); } identifier_list
%destructor { DestroyExpression(&$$); } expression expression1 expression2 expression3 expression4 expression5 expression6 expression7 expression8

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
		statement->shared.instruction = $1;
	}
	| TOKEN_DIRECTIVE_DC size expression_list
	{
		statement->type = STATEMENT_TYPE_DC;
		statement->shared.dc.size = $2;
		statement->shared.dc.values = $3;
	}
	| TOKEN_DIRECTIVE_DCB size expression ',' expression
	{
		statement->type = STATEMENT_TYPE_DCB;
		statement->shared.dcb.size = $2;
		statement->shared.dcb.repetitions = $3;
		statement->shared.dcb.value = $5;
	}
	| TOKEN_DIRECTIVE_INCLUDE TOKEN_STRING
	{
		statement->type = STATEMENT_TYPE_INCLUDE;
		statement->shared.include.path = $2;
	}
	| TOKEN_DIRECTIVE_INCBIN TOKEN_STRING
	{
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = $2;
		statement->shared.incbin.start.type = EXPRESSION_NUMBER;
		statement->shared.incbin.start.shared.unsigned_long = 0;
		statement->shared.incbin.has_length = cc_false;
	}
	| TOKEN_DIRECTIVE_INCBIN TOKEN_STRING ',' expression
	{
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = $2;
		statement->shared.incbin.start = $4;
		statement->shared.incbin.has_length = cc_false;
	}
	| TOKEN_DIRECTIVE_INCBIN TOKEN_STRING ',' expression ',' expression
	{
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = $2;
		statement->shared.incbin.start = $4;
		statement->shared.incbin.has_length = cc_true;
		statement->shared.incbin.length = $6;
	}
	| TOKEN_DIRECTIVE_REPT expression
	{
		statement->type = STATEMENT_TYPE_REPT;
		statement->shared.rept.repetitions = $2;
	}
	| TOKEN_DIRECTIVE_ENDR
	{
		statement->type = STATEMENT_TYPE_ENDR;
	}
	| TOKEN_DIRECTIVE_MACRO
	{
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
	| TOKEN_DIRECTIVE_MACRO identifier_list
	{
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.parameter_names = $2;
	}
	| TOKEN_DIRECTIVE_MACROS
	{
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
	| TOKEN_DIRECTIVE_MACROS identifier_list
	{
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.parameter_names = $2;
	}
	| TOKEN_DIRECTIVE_ENDM
	{
		statement->type = STATEMENT_TYPE_ENDM;
	}
	| TOKEN_DIRECTIVE_EQU expression
	{
		statement->type = STATEMENT_TYPE_EQU;
		statement->shared.expression = $2;
	}
	| TOKEN_DIRECTIVE_SET expression
	{
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.expression = $2;
	}
	| '=' expression
	{
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.expression = $2;
	}
	| TOKEN_DIRECTIVE_IF expression
	{
		statement->type = STATEMENT_TYPE_IF;
		statement->shared.expression = $2;
	}
	| TOKEN_DIRECTIVE_ELSEIF expression
	{
		statement->type = STATEMENT_TYPE_ELSEIF;
		statement->shared.expression = $2;
	}
	| TOKEN_DIRECTIVE_ELSEIF
	{
		statement->type = STATEMENT_TYPE_ELSE;
	}
	| TOKEN_DIRECTIVE_ELSE
	{
		statement->type = STATEMENT_TYPE_ELSE;
	}
	| TOKEN_DIRECTIVE_ENDC
	{
		statement->type = STATEMENT_TYPE_ENDC;
	}
	| TOKEN_DIRECTIVE_WHILE expression
	{
		statement->type = STATEMENT_TYPE_WHILE;
		statement->shared.expression = $2;
	}
	| TOKEN_DIRECTIVE_ENDW
	{
		statement->type = STATEMENT_TYPE_ENDW;
	}
	| TOKEN_DIRECTIVE_EVEN
	{
		statement->type = STATEMENT_TYPE_EVEN;
	}
	| TOKEN_DIRECTIVE_CNOP expression ',' expression
	{
		statement->type = STATEMENT_TYPE_CNOP;
		statement->shared.cnop.offset = $2;
		statement->shared.cnop.size_boundary = $4;
	}
	| TOKEN_DIRECTIVE_INFORM expression ',' TOKEN_STRING
	{
		(void)$2;

		/* TODO - Severity level(?) */
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.message = $4;
	}
	| TOKEN_DIRECTIVE_INFORM expression ',' TOKEN_STRING ',' expression_list
	{
		(void)$2;
		(void)$6;

		/* TODO - Severity level(?) and parameters */
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.message = $4;
	}
	| TOKEN_DIRECTIVE_END
	{
		statement->type = STATEMENT_TYPE_END;
	}
	| TOKEN_DIRECTIVE_RS size expression
	{
		statement->type = STATEMENT_TYPE_RS;
		statement->shared.rs.size = $2;
		statement->shared.rs.length = $3;
	}
	| TOKEN_DIRECTIVE_RSSET expression
	{
		statement->type = STATEMENT_TYPE_RSSET;
		statement->shared.expression = $2;
	}
	| TOKEN_DIRECTIVE_RSRESET
	{
		statement->type = STATEMENT_TYPE_RSRESET;
	}
	;

expression_list
	: expression
	{
		ExpressionListNode *node = (ExpressionListNode*)malloc(sizeof(ExpressionListNode));

		if (node == NULL)
		{
			DestroyExpression(&$1);
			YYNOMEM;
		}
		else
		{
			node->expression = $1;
			node->next = NULL;
		}

		$$.head = $$.tail = node;
	}
	| expression_list ',' expression
	{
		ExpressionListNode *node = (ExpressionListNode*)malloc(sizeof(ExpressionListNode));

		$$ = $1;

		if (node == NULL)
		{
			DestroyExpressionList(&$1);
			DestroyExpression(&$3);
			YYNOMEM;
		}
		else
		{
			node->expression = $3;
			node->next = NULL;

			if ($$.head == NULL)
				$$.head = node;
			else
				((ExpressionListNode*)$$.tail)->next = node;

			$$.tail = node;
		}
	}
	;

identifier_list
	: TOKEN_IDENTIFIER
	{
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		if (node == NULL)
		{
			free($1);
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
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		$$ = $1;

		if (node == NULL)
		{
			DestroyIdentifierList(&$1);
			free(&$3);
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
		$$.operands[0].type = OPERAND_NONE;
		$$.operands[1].type = OPERAND_NONE;
	}
	| full_opcode operand
	{
		$$.opcode = $1;
		$$.operands[0] = $2;
		$$.operands[1].type = OPERAND_NONE;
	}
	| full_opcode operand ',' operand
	{
		$$.opcode = $1;
		$$.operands[0] = $2;
		$$.operands[1] = $4;
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
		m68kasm_warning(scanner, statement, "Opcode has a dot but no size; either remove the dot or add an explicit size.");
	}
	| opcode size
	{
		$$ = $1;
		$$.size = $2;
	}
	;

opcode
	: TOKEN_DIRECTIVE_ORI
	{
		$$.type = OPCODE_ORI;
	}
	| TOKEN_DIRECTIVE_ANDI
	{
		$$.type = OPCODE_ANDI;
	}
	| TOKEN_DIRECTIVE_SUBI
	{
		$$.type = OPCODE_SUBI;
	}
	| TOKEN_DIRECTIVE_ADDI
	{
		$$.type = OPCODE_ADDI;
	}
	| TOKEN_DIRECTIVE_EORI
	{
		$$.type = OPCODE_EORI;
	}
	| TOKEN_DIRECTIVE_CMPI
	{
		$$.type = OPCODE_CMPI;
	}
	| TOKEN_DIRECTIVE_BTST
	{
		$$.type = OPCODE_BTST_STATIC;
	}
	| TOKEN_DIRECTIVE_BCHG
	{
		$$.type = OPCODE_BCHG_STATIC;
	}
	| TOKEN_DIRECTIVE_BCLR
	{
		$$.type = OPCODE_BCLR_STATIC;
	}
	| TOKEN_DIRECTIVE_BSET
	{
		$$.type = OPCODE_BSET_STATIC;
	}
	| TOKEN_DIRECTIVE_MOVEP
	{
		$$.type = OPCODE_MOVEP_TO_REG;
	}
	| TOKEN_DIRECTIVE_MOVEA
	{
		$$.type = OPCODE_MOVEA;
	}
	| TOKEN_DIRECTIVE_MOVE
	{
		$$.type = OPCODE_MOVE;
	}
	| TOKEN_DIRECTIVE_NEGX
	{
		$$.type = OPCODE_NEGX;
	}
	| TOKEN_DIRECTIVE_CLR
	{
		$$.type = OPCODE_CLR;
	}
	| TOKEN_DIRECTIVE_NEG
	{
		$$.type = OPCODE_NEG;
	}
	| TOKEN_DIRECTIVE_NOT
	{
		$$.type = OPCODE_NOT;
	}
	| TOKEN_DIRECTIVE_EXT
	{
		$$.type = OPCODE_EXT;
	}
	| TOKEN_DIRECTIVE_NBCD
	{
		$$.type = OPCODE_NBCD;
	}
	| TOKEN_DIRECTIVE_SWAP
	{
		$$.type = OPCODE_SWAP;
	}
	| TOKEN_DIRECTIVE_PEA
	{
		$$.type = OPCODE_PEA;
	}
	| TOKEN_DIRECTIVE_ILLEGAL
	{
		$$.type = OPCODE_ILLEGAL;
	}
	| TOKEN_DIRECTIVE_TAS
	{
		$$.type = OPCODE_TAS;
	}
	| TOKEN_DIRECTIVE_TST
	{
		$$.type = OPCODE_TST;
	}
	| TOKEN_DIRECTIVE_TRAP
	{
		$$.type = OPCODE_TRAP;
	}
	| TOKEN_DIRECTIVE_LINK
	{
		$$.type = OPCODE_LINK;
	}
	| TOKEN_DIRECTIVE_UNLK
	{
		$$.type = OPCODE_UNLK;
	}
	| TOKEN_DIRECTIVE_RESET
	{
		$$.type = OPCODE_RESET;
	}
	| TOKEN_DIRECTIVE_NOP
	{
		$$.type = OPCODE_NOP;
	}
	| TOKEN_DIRECTIVE_STOP
	{
		$$.type = OPCODE_STOP;
	}
	| TOKEN_DIRECTIVE_RTE
	{
		$$.type = OPCODE_RTE;
	}
	| TOKEN_DIRECTIVE_RTS
	{
		$$.type = OPCODE_RTS;
	}
	| TOKEN_DIRECTIVE_TRAPV
	{
		$$.type = OPCODE_TRAPV;
	}
	| TOKEN_DIRECTIVE_RTR
	{
		$$.type = OPCODE_RTR;
	}
	| TOKEN_DIRECTIVE_JSR
	{
		$$.type = OPCODE_JSR;
	}
	| TOKEN_DIRECTIVE_JMP
	{
		$$.type = OPCODE_JMP;
	}
	| TOKEN_DIRECTIVE_MOVEM
	{
		$$.type = OPCODE_MOVEM_TO_REGS;
	}
	| TOKEN_DIRECTIVE_LEA
	{
		$$.type = OPCODE_LEA;
	}
	| TOKEN_DIRECTIVE_CHK
	{
		$$.type = OPCODE_CHK;
	}
	| TOKEN_DIRECTIVE_ADDQ
	{
		$$.type = OPCODE_ADDQ;
	}
	| TOKEN_DIRECTIVE_SUBQ
	{
		$$.type = OPCODE_SUBQ;
	}
	| TOKEN_DIRECTIVE_ST
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_TRUE;
	}
	| TOKEN_DIRECTIVE_SF
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_FALSE;
	}
	| TOKEN_DIRECTIVE_SHI
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_HIGHER;
	}
	| TOKEN_DIRECTIVE_SLS
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_LOWER_OR_SAME;
	}
	| TOKEN_DIRECTIVE_SCC
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_CARRY_CLEAR;
	}
	| TOKEN_DIRECTIVE_SCS
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_CARRY_SET;
	}
	| TOKEN_DIRECTIVE_SNE
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_NOT_EQUAL;
	}
	| TOKEN_DIRECTIVE_SEQ
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_EQUAL;
	}
	| TOKEN_DIRECTIVE_SVC
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_OVERFLOW_CLEAR;
	}
	| TOKEN_DIRECTIVE_SVS
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_OVERFLOW_SET;
	}
	| TOKEN_DIRECTIVE_SPL
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_PLUS;
	}
	| TOKEN_DIRECTIVE_SMI
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_MINUS;
	}
	| TOKEN_DIRECTIVE_SGE
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_GREATER_OR_EQUAL;
	}
	| TOKEN_DIRECTIVE_SLT
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_LESS_THAN;
	}
	| TOKEN_DIRECTIVE_SGT
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_GREATER_THAN;
	}
	| TOKEN_DIRECTIVE_SLE
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_LESS_OR_EQUAL;
	}
	| TOKEN_DIRECTIVE_DBT
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_TRUE;
	}
	| TOKEN_DIRECTIVE_DBF
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_FALSE;
	}
	| TOKEN_DIRECTIVE_DBHI
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_HIGHER;
	}
	| TOKEN_DIRECTIVE_DBLS
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_LOWER_OR_SAME;
	}
	| TOKEN_DIRECTIVE_DBCC
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_CARRY_CLEAR;
	}
	| TOKEN_DIRECTIVE_DBCS
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_CARRY_SET;
	}
	| TOKEN_DIRECTIVE_DBNE
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_NOT_EQUAL;
	}
	| TOKEN_DIRECTIVE_DBEQ
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_EQUAL;
	}
	| TOKEN_DIRECTIVE_DBVC
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_OVERFLOW_CLEAR;
	}
	| TOKEN_DIRECTIVE_DBVS
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_OVERFLOW_SET;
	}
	| TOKEN_DIRECTIVE_DBPL
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_PLUS;
	}
	| TOKEN_DIRECTIVE_DBMI
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_MINUS;
	}
	| TOKEN_DIRECTIVE_DBGE
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_GREATER_OR_EQUAL;
	}
	| TOKEN_DIRECTIVE_DBLT
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_LESS_THAN;
	}
	| TOKEN_DIRECTIVE_DBGT
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_GREATER_THAN;
	}
	| TOKEN_DIRECTIVE_DBLE
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_LESS_OR_EQUAL;
	}
	| TOKEN_DIRECTIVE_BRA
	{
		$$.type = OPCODE_BRA;
	}
	| TOKEN_DIRECTIVE_BSR
	{
		$$.type = OPCODE_BSR;
	}
	| TOKEN_DIRECTIVE_BHI
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_HIGHER;
	}
	| TOKEN_DIRECTIVE_BLS
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_LOWER_OR_SAME;
	}
	| TOKEN_DIRECTIVE_BCC
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_CARRY_CLEAR;
	}
	| TOKEN_DIRECTIVE_BCS
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_CARRY_SET;
	}
	| TOKEN_DIRECTIVE_BNE
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_NOT_EQUAL;
	}
	| TOKEN_DIRECTIVE_BEQ
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_EQUAL;
	}
	| TOKEN_DIRECTIVE_BVC
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_OVERFLOW_CLEAR;
	}
	| TOKEN_DIRECTIVE_BVS
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_OVERFLOW_SET;
	}
	| TOKEN_DIRECTIVE_BPL
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_PLUS;
	}
	| TOKEN_DIRECTIVE_BMI
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_MINUS;
	}
	| TOKEN_DIRECTIVE_BGE
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_GREATER_OR_EQUAL;
	}
	| TOKEN_DIRECTIVE_BLT
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_LESS_THAN;
	}
	| TOKEN_DIRECTIVE_BGT
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_GREATER_THAN;
	}
	| TOKEN_DIRECTIVE_BLE
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_LESS_OR_EQUAL;
	}
	| TOKEN_DIRECTIVE_MOVEQ
	{
		$$.type = OPCODE_MOVEQ;
	}
	| TOKEN_DIRECTIVE_DIVU
	{
		$$.type = OPCODE_DIVU;
	}
	| TOKEN_DIRECTIVE_DIVS
	{
		$$.type = OPCODE_DIVS;
	}
	| TOKEN_DIRECTIVE_SBCD
	{
		$$.type = OPCODE_SBCD_DATA_REGS;
	}
	| TOKEN_DIRECTIVE_OR
	{
		$$.type = OPCODE_OR_TO_REG;
	}
	| TOKEN_DIRECTIVE_SUB
	{
		$$.type = OPCODE_SUB_TO_REG;
	}
	| TOKEN_DIRECTIVE_SUBX
	{
		$$.type = OPCODE_SUBX_DATA_REGS;
	}
	| TOKEN_DIRECTIVE_SUBA
	{
		$$.type = OPCODE_SUBA;
	}
	| TOKEN_DIRECTIVE_EOR
	{
		$$.type = OPCODE_EOR;
	}
	| TOKEN_DIRECTIVE_CMPM
	{
		$$.type = OPCODE_CMPM;
	}
	| TOKEN_DIRECTIVE_CMP
	{
		$$.type = OPCODE_CMP;
	}
	| TOKEN_DIRECTIVE_CMPA
	{
		$$.type = OPCODE_CMPA;
	}
	| TOKEN_DIRECTIVE_MULU
	{
		$$.type = OPCODE_MULU;
	}
	| TOKEN_DIRECTIVE_MULS
	{
		$$.type = OPCODE_MULS;
	}
	| TOKEN_DIRECTIVE_ABCD
	{
		$$.type = OPCODE_ABCD_DATA_REGS;
	}
	| TOKEN_DIRECTIVE_EXG
	{
		$$.type = OPCODE_EXG;
	}
	| TOKEN_DIRECTIVE_AND
	{
		$$.type = OPCODE_AND_TO_REG;
	}
	| TOKEN_DIRECTIVE_ADD
	{
		$$.type = OPCODE_ADD_TO_REG;
	}
	| TOKEN_DIRECTIVE_ADDX
	{
		$$.type = OPCODE_ADDX_DATA_REGS;
	}
	| TOKEN_DIRECTIVE_ADDA
	{
		$$.type = OPCODE_ADDA;
	}
	| TOKEN_DIRECTIVE_ASL
	{
		$$.type = OPCODE_ASL_STATIC;
	}
	| TOKEN_DIRECTIVE_ASR
	{
		$$.type = OPCODE_ASR_STATIC;
	}
	| TOKEN_DIRECTIVE_LSL
	{
		$$.type = OPCODE_LSL_STATIC;
	}
	| TOKEN_DIRECTIVE_LSR
	{
		$$.type = OPCODE_LSR_STATIC;
	}
	| TOKEN_DIRECTIVE_ROXL
	{
		$$.type = OPCODE_ROXL_STATIC;
	}
	| TOKEN_DIRECTIVE_ROXR
	{
		$$.type = OPCODE_ROXR_STATIC;
	}
	| TOKEN_DIRECTIVE_ROL
	{
		$$.type = OPCODE_ROL_STATIC;
	}
	| TOKEN_DIRECTIVE_ROR
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
	| expression '(' TOKEN_ADDRESS_REGISTER ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		$$.literal = $1;
		$$.main_register = $3;
	}
	| '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register size ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = EXPRESSION_NUMBER;
		$$.literal.shared.unsigned_long = 0;
		$$.main_register = $2;
		$$.index_register = $4 % 8;
		$$.size = $5;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| expression '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register size ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.main_register = $3;
		$$.index_register = $5 % 8;
		$$.size = $6;
		$$.index_register_is_address_register = $5 / 8 !=0;
	}
	| '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register ')'
	{
		m68kasm_warning(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = EXPRESSION_NUMBER;
		$$.literal.shared.unsigned_long = 0;
		$$.main_register = $2;
		$$.index_register = $4 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| expression '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register ')'
	{
		m68kasm_warning(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.main_register = $3;
		$$.index_register = $5 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $5 / 8 !=0;
	}
	| expression '(' TOKEN_PROGRAM_COUNTER ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		$$.literal = $1;
	}
	| '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register size ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = EXPRESSION_NUMBER;
		$$.literal.shared.unsigned_long = 0;
		$$.index_register = $4 % 8;
		$$.size = $5;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| expression '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register size ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.index_register = $5 % 8;
		$$.size = $6;
		$$.index_register_is_address_register = $5 / 8 != 0;
	}
	| '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register ')'
	{
		m68kasm_warning(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = EXPRESSION_NUMBER;
		$$.literal.shared.unsigned_long = 0;
		$$.index_register = $4 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| expression '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register ')'
	{
		m68kasm_warning(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.index_register = $5 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $5 / 8 != 0;
	}
	/* Literal */
	| '#' expression
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
	| expression
	{
		$$.type = OPERAND_ADDRESS;
		$$.literal = $1;
		$$.size = SIZE_UNDEFINED;
	}
	| '(' expression ')' size
	{
		$$.type = OPERAND_ADDRESS_ABSOLUTE;
		$$.literal = $2;
		$$.size = $4;
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
expression
	: expression1
	{
		$$ = $1;
	}
	/* This is an assembler extension: asm68k doesn't support this. */
	| expression TOKEN_LOGICAL_AND expression1
	{
		if (!DoExpression(&$$, EXPRESSION_LOGICAL_AND, &$1, &$3))
			YYNOMEM;
	}
	/* This is an assembler extension: asm68k doesn't support this. */
	| expression TOKEN_LOGICAL_OR expression1
	{
		if (!DoExpression(&$$, EXPRESSION_LOGICAL_OR, &$1, &$3))
			YYNOMEM;
	}
	;

expression1
	: expression2
	{
		$$ = $1;
	}
	| expression1 '=' expression2
	{
		if (!DoExpression(&$$, EXPRESSION_EQUALITY, &$1, &$3))
			YYNOMEM;
	}
	| expression1 TOKEN_EQUALITY expression2
	{
		if (!DoExpression(&$$, EXPRESSION_EQUALITY, &$1, &$3))
			YYNOMEM;
	}
	| expression1 TOKEN_INEQUALITY expression2
	{
		if (!DoExpression(&$$, EXPRESSION_INEQUALITY, &$1, &$3))
			YYNOMEM;
	}
	;

expression2
	: expression3
	{
		$$ = $1;
	}
	| expression2 '<' expression3
	{
		if (!DoExpression(&$$, EXPRESSION_LESS_THAN, &$1, &$3))
			YYNOMEM;
	}
	| expression2 TOKEN_LESS_OR_EQUAL expression3
	{
		if (!DoExpression(&$$, EXPRESSION_LESS_OR_EQUAL, &$1, &$3))
			YYNOMEM;
	}
	| expression2 '>' expression3
	{
		if (!DoExpression(&$$, EXPRESSION_MORE_THAN, &$1, &$3))
			YYNOMEM;
	}
	| expression2 TOKEN_MORE_OR_EQUAL expression3
	{
		if (!DoExpression(&$$, EXPRESSION_MORE_OR_EQUAL, &$1, &$3))
			YYNOMEM;
	}
	;

expression3
	: expression4
	{
		$$ = $1;
	}
	| expression3 '+' expression4
	{
		if (!DoExpression(&$$, EXPRESSION_ADD, &$1, &$3))
			YYNOMEM;
	}
	| expression3 '-' expression4
	{
		if (!DoExpression(&$$, EXPRESSION_SUBTRACT, &$1, &$3))
			YYNOMEM;
	}
	;

expression4
	: expression5
	{
		$$ = $1;
	}
	| expression4 '*' expression5
	{
		if (!DoExpression(&$$, EXPRESSION_MULTIPLY, &$1, &$3))
			YYNOMEM;
	}
	| expression4 '/' expression5
	{
		if (!DoExpression(&$$, EXPRESSION_DIVIDE, &$1, &$3))
			YYNOMEM;
	}
	| expression4 '%' expression5
	{
		if (!DoExpression(&$$, EXPRESSION_MODULO, &$1, &$3))
			YYNOMEM;
	}
	;

expression5
	: expression6
	{
		$$ = $1;
	}
	| expression5 '&' expression6
	{
		if (!DoExpression(&$$, EXPRESSION_BITWISE_AND, &$1, &$3))
			YYNOMEM;
	}
	| expression5 '!' expression6
	{
		if (!DoExpression(&$$, EXPRESSION_BITWISE_OR, &$1, &$3))
			YYNOMEM;
	}
	| expression5 '|' expression6
	{
		if (!DoExpression(&$$, EXPRESSION_BITWISE_OR, &$1, &$3))
			YYNOMEM;
	}
	| expression5 '^' expression6
	{
		if (!DoExpression(&$$, EXPRESSION_BITWISE_XOR, &$1, &$3))
			YYNOMEM;
	}
	;

expression6
	: expression7
	{
		$$ = $1;
	}
	| expression6 TOKEN_LEFT_SHIFT expression7
	{
		if (!DoExpression(&$$, EXPRESSION_LEFT_SHIFT, &$1, &$3))
			YYNOMEM;
	}
	| expression6 TOKEN_RIGHT_SHIFT expression7
	{
		if (!DoExpression(&$$, EXPRESSION_RIGHT_SHIFT, &$1, &$3))
			YYNOMEM;
	}
	;

expression7
	: expression8
	{
		$$ = $1;
	}
	| '+' expression7
	{
		$$ = $2;
	}
	| '-' expression7
	{
		if (!DoExpression(&$$, EXPRESSION_NEGATE, &$2, NULL))
			YYNOMEM;
	}
	| '~' expression7
	{
		if (!DoExpression(&$$, EXPRESSION_BITWISE_NOT, &$2, NULL))
			YYNOMEM;
	}
	/* This is an assembler extension: asm68k doesn't support this. */
	| '!' expression7
	{
		if (!DoExpression(&$$, EXPRESSION_LOGICAL_NOT, &$2, NULL))
			YYNOMEM;
	}
	;

expression8
	: TOKEN_NUMBER
	{
		$$.type = EXPRESSION_NUMBER;
		$$.shared.unsigned_long = $1;
	}
	| TOKEN_IDENTIFIER
	{
		$$.type = EXPRESSION_IDENTIFIER;
		$$.shared.string = $1;
	}
	| TOKEN_LOCAL_IDENTIFIER
	{
		$$.type = EXPRESSION_IDENTIFIER;
		$$.shared.string = $1;
	}
	| TOKEN_IDENTIFIER TOKEN_LOCAL_IDENTIFIER
	{
		const size_t identifier_length = strlen($1);
		const size_t local_identifier_length = strlen($2 + 1);

		$$.shared.string = (char*)malloc(identifier_length + 1 + local_identifier_length + 1);

		if ($$.shared.string == NULL)
		{
			free($1);
			free($2);
			YYNOMEM;
		}
		else
		{
			$$.type = EXPRESSION_IDENTIFIER;
			memcpy(&$$.shared.string[0], $1, identifier_length);
			free($1);
			$$.shared.string[identifier_length] = '@';
			memcpy(&$$.shared.string[identifier_length + 1], $2 + 1, local_identifier_length + 1);
			free($2);
		}
	}
	| TOKEN_STRING
	{
		$$.type = EXPRESSION_STRING;
		$$.shared.string = $1;
	}
	| '*'
	{
		$$.type = EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT;
	}
	| '@'
	{
		$$.type = EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION;
	}
	| '(' expression ')'
	{
		$$ = $2;
	}
	| TOKEN_STRLEN '(' TOKEN_STRING ')'
	{
		$$.type = EXPRESSION_STRLEN;
		$$.shared.string = $3;
	}
	;

%%

static cc_bool DoExpression(Expression *expression, ExpressionType type, Expression *left_expression, Expression *right_expression)
{
	cc_bool success = cc_true;

	expression->type = type;

	expression->shared.subexpressions = (Expression*)malloc(sizeof(Expression) * (right_expression != NULL ? 2 : 1));

	if (expression->shared.subexpressions == NULL)
	{
		DestroyExpression(left_expression);

		if (right_expression != NULL)
			DestroyExpression(right_expression);

		success = cc_false;
	}
	else
	{
		expression->shared.subexpressions[0] = *left_expression;

		if (right_expression != NULL)
			expression->shared.subexpressions[1] = *right_expression;
	}

	return success;
}

void DestroyExpression(Expression *expression)
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
			DestroyExpression(&expression->shared.subexpressions[0]);
			DestroyExpression(&expression->shared.subexpressions[1]);
			free(expression->shared.subexpressions);
			break;

		case EXPRESSION_NEGATE:
		case EXPRESSION_BITWISE_NOT:
		case EXPRESSION_LOGICAL_NOT:
			DestroyExpression(&expression->shared.subexpressions[0]);
			free(expression->shared.subexpressions);
			break;

		case EXPRESSION_IDENTIFIER:
		case EXPRESSION_STRING:
		case EXPRESSION_STRLEN:
			free(expression->shared.string);
			break;

		case EXPRESSION_NUMBER:
		case EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT:
		case EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION:
			break;
	}
}

static void DestroyIdentifierList(IdentifierList *list)
{
	IdentifierListNode *node = list->head;

	while (node != NULL)
	{
		IdentifierListNode* const next_node = node->next;

		free(node->identifier);

		free(node);

		node = next_node;
	}
}

static void DestroyExpressionList(ExpressionList *list)
{
	ExpressionListNode *node = list->head;

	while (node != NULL)
	{
		ExpressionListNode* const next_node = node->next;

		DestroyExpression(&node->expression);

		free(node);

		node = next_node;
	}
}

static void DestroyOperand(Operand *operand)
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
		case OPERAND_REGISTER_LIST:
			break;

		case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT:
		case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
		case OPERAND_ADDRESS:
		case OPERAND_ADDRESS_ABSOLUTE:
		case OPERAND_LITERAL:
		case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT:
		case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
			DestroyExpression(&operand->literal);
			break;
	}
}

static void DestroyStatementInstruction(StatementInstruction *instruction)
{
	size_t i;

	for (i = 0; i < CC_COUNT_OF(instruction->operands); ++i)
		DestroyOperand(&instruction->operands[i]);
}

void DestroyStatement(Statement *statement)
{
	switch (statement->type)
	{
		case STATEMENT_TYPE_EMPTY:
		case STATEMENT_TYPE_ENDR:
		case STATEMENT_TYPE_ENDM:
		case STATEMENT_TYPE_ELSE:
		case STATEMENT_TYPE_ENDC:
		case STATEMENT_TYPE_ENDW:
		case STATEMENT_TYPE_EVEN:
		case STATEMENT_TYPE_END:
		case STATEMENT_TYPE_RSRESET:
			break;

		case STATEMENT_TYPE_INSTRUCTION:
			DestroyStatementInstruction(&statement->shared.instruction);
			break;

		case STATEMENT_TYPE_DC:
			DestroyExpressionList(&statement->shared.dc.values);
			break;

		case STATEMENT_TYPE_DCB:
			DestroyExpression(&statement->shared.dcb.repetitions);
			DestroyExpression(&statement->shared.dcb.value);
			break;

		case STATEMENT_TYPE_INCLUDE:
			free(statement->shared.include.path);
			break;

		case STATEMENT_TYPE_INCBIN:
			free(statement->shared.incbin.path);
			DestroyExpression(&statement->shared.incbin.start);

			if (statement->shared.incbin.has_length)
				DestroyExpression(&statement->shared.incbin.length);

			break;

		case STATEMENT_TYPE_REPT:
			DestroyExpression(&statement->shared.rept.repetitions);
			break;

		case STATEMENT_TYPE_MACRO:
		case STATEMENT_TYPE_MACROS:
			DestroyIdentifierList(&statement->shared.macro.parameter_names);
			break;

		case STATEMENT_TYPE_EQU:
		case STATEMENT_TYPE_SET:
		case STATEMENT_TYPE_IF:
		case STATEMENT_TYPE_ELSEIF:
		case STATEMENT_TYPE_WHILE:
		case STATEMENT_TYPE_RSSET:
			DestroyExpression(&statement->shared.expression);
			break;

		case STATEMENT_TYPE_CNOP:
			DestroyExpression(&statement->shared.cnop.offset);
			DestroyExpression(&statement->shared.cnop.size_boundary);
			break;

		case STATEMENT_TYPE_INFORM:
			free(statement->shared.inform.message);
			break;

		case STATEMENT_TYPE_RS:
			DestroyExpression(&statement->shared.rs.length);
			break;
	}
}
