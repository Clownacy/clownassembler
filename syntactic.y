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

/*%define parse.trace*/

%define api.pure

%define api.prefix {m68kasm_}

%param {void *scanner}

%parse-param {Statement *statement}

%define parse.error verbose

%code requires {

#include "clowncommon/clowncommon.h"
#include "string.h"

/* A hack for older versions of Bison. */
/* Should probably be removed when they go out of circulation. */
#if defined(YYBISON) && YYBISON < 30802 && !defined(YYNOMEM)
#define YYNOMEM goto yyexhaustedlab;
#endif

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
	EXPRESSION_STRLEN,
	EXPRESSION_STRCMP,
	EXPRESSION_INSTR,
	EXPRESSION_DEF,
	EXPRESSION_TYPE_WITH_IDENTIFIER,
	EXPRESSION_TYPE_WITH_NUMBER,
	EXPRESSION_FILESIZE
} ExpressionType;

typedef struct Expression
{
	ExpressionType type;
	union
	{
		unsigned long unsigned_long;
		String string;
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

	String identifier;
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

typedef struct StatementDs
{
	Size size;
	Expression length;
} StatementDs;

typedef struct StatementInclude
{
	String path;
} StatementInclude;

typedef struct StatementIncbin
{
	String path;
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
	cc_bool uses_label;
	IdentifierList parameter_names;
} StatementMacro;

typedef struct StatementInform
{
	Expression severity;
	String message;
} StatementInform;

typedef struct StatementSubstr
{
	Expression start, end;
	String string;
} StatementSubstr;

typedef struct StatementOpt
{
	IdentifierList options;
} StatementOpt;

typedef struct StatementLocal
{
	IdentifierList identifiers;
} StatementLocal;

typedef enum StatementType
{
	STATEMENT_TYPE_EMPTY,
	STATEMENT_TYPE_INSTRUCTION,
	STATEMENT_TYPE_DC,
	STATEMENT_TYPE_DCB,
	STATEMENT_TYPE_DS,
	STATEMENT_TYPE_INCLUDE,
	STATEMENT_TYPE_INCBIN,
	STATEMENT_TYPE_REPT,
	STATEMENT_TYPE_ENDR,
	STATEMENT_TYPE_MACRO,
	STATEMENT_TYPE_MACROS,
	STATEMENT_TYPE_ENDM,
	STATEMENT_TYPE_EQU,
	STATEMENT_TYPE_EQUS_STRING,
	STATEMENT_TYPE_EQUS_IDENTIFIER,
	STATEMENT_TYPE_SUBSTR,
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
	STATEMENT_TYPE_FAIL,
	STATEMENT_TYPE_END,
	STATEMENT_TYPE_RS,
	STATEMENT_TYPE_RSSET,
	STATEMENT_TYPE_RSRESET,
	STATEMENT_TYPE_OBJ,
	STATEMENT_TYPE_OBJEND,
	STATEMENT_TYPE_ORG,
	STATEMENT_TYPE_PUSHO,
	STATEMENT_TYPE_POPO,
	STATEMENT_TYPE_OPT,
	STATEMENT_TYPE_PUSHP,
	STATEMENT_TYPE_POPP,
	STATEMENT_TYPE_SHIFT,
	STATEMENT_TYPE_MEXIT,
	STATEMENT_TYPE_LOCAL,
	STATEMENT_TYPE_PURGE
} StatementType;

typedef struct Statement
{
	StatementType type;
	union
	{
		StatementInstruction instruction;
		StatementDc dc;
		StatementDcb dcb;
		StatementDs ds;
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
		StatementSubstr substr;
		StatementOpt opt;
		StatementLocal local;
		Expression expression;
		String string;
	} shared;
} Statement;

}

%code provides {

void DestroyExpression(Expression *expression);
void DestroyStatement(Statement *statement);
void DestroyIdentifierList(IdentifierList *list);

}

%code {

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int m68kasm_lex(M68KASM_STYPE *yylval_param, void *yyscanner);
void m68kasm_warning(void *scanner, Statement *statement, const char *message);
void m68kasm_warning_pedantic(void *scanner, Statement *statement, const char *message);
void m68kasm_error(void *scanner, Statement *statement, const char *message);

static cc_bool DoExpressionTriple(Expression *expression, ExpressionType type, Expression *left_expression, Expression *middle_expression, Expression *right_expression);
static cc_bool DoExpression(Expression *expression, ExpressionType type, Expression *left_expression, Expression *right_expression);
static void DestroyExpressionList(ExpressionList *list);
static void DestroyOperand(Operand *operand);
static void DestroyStatementInstruction(StatementInstruction *instruction);

}

%union {
	unsigned long unsigned_long;
	String string;
	Opcode opcode;
	Size size;
	Operand operand;
	StatementInstruction instruction;
	Statement statement;
	ExpressionList expression_list;
	IdentifierList identifier_list;
	Expression expression;
}

%define api.token.prefix {TOKEN_}

%token DIRECTIVE_ORI
%token DIRECTIVE_ANDI
%token DIRECTIVE_SUBI
%token DIRECTIVE_ADDI
%token DIRECTIVE_EORI
%token DIRECTIVE_CMPI
%token DIRECTIVE_BTST
%token DIRECTIVE_BCHG
%token DIRECTIVE_BCLR
%token DIRECTIVE_BSET
%token DIRECTIVE_MOVEP
%token DIRECTIVE_MOVEA
%token DIRECTIVE_MOVE
%token DIRECTIVE_NEGX
%token DIRECTIVE_CLR
%token DIRECTIVE_NEG
%token DIRECTIVE_NOT
%token DIRECTIVE_EXT
%token DIRECTIVE_NBCD
%token DIRECTIVE_SWAP
%token DIRECTIVE_PEA
%token DIRECTIVE_ILLEGAL
%token DIRECTIVE_TAS
%token DIRECTIVE_TST
%token DIRECTIVE_TRAP
%token DIRECTIVE_LINK
%token DIRECTIVE_UNLK
%token DIRECTIVE_RESET
%token DIRECTIVE_NOP
%token DIRECTIVE_STOP
%token DIRECTIVE_RTE
%token DIRECTIVE_RTS
%token DIRECTIVE_TRAPV
%token DIRECTIVE_RTR
%token DIRECTIVE_JSR
%token DIRECTIVE_JMP
%token DIRECTIVE_MOVEM
%token DIRECTIVE_LEA
%token DIRECTIVE_CHK
%token DIRECTIVE_ADDQ
%token DIRECTIVE_SUBQ
%token DIRECTIVE_ST
%token DIRECTIVE_SF
%token DIRECTIVE_SHI
%token DIRECTIVE_SLS
%token DIRECTIVE_SCC
%token DIRECTIVE_SCS
%token DIRECTIVE_SNE
%token DIRECTIVE_SEQ
%token DIRECTIVE_SVC
%token DIRECTIVE_SVS
%token DIRECTIVE_SPL
%token DIRECTIVE_SMI
%token DIRECTIVE_SGE
%token DIRECTIVE_SLT
%token DIRECTIVE_SGT
%token DIRECTIVE_SLE
%token DIRECTIVE_DBT
%token DIRECTIVE_DBF
%token DIRECTIVE_DBHI
%token DIRECTIVE_DBLS
%token DIRECTIVE_DBCC
%token DIRECTIVE_DBCS
%token DIRECTIVE_DBNE
%token DIRECTIVE_DBEQ
%token DIRECTIVE_DBVC
%token DIRECTIVE_DBVS
%token DIRECTIVE_DBPL
%token DIRECTIVE_DBMI
%token DIRECTIVE_DBGE
%token DIRECTIVE_DBLT
%token DIRECTIVE_DBGT
%token DIRECTIVE_DBLE
%token DIRECTIVE_BRA
%token DIRECTIVE_BSR
%token DIRECTIVE_BHI
%token DIRECTIVE_BLS
%token DIRECTIVE_BCC
%token DIRECTIVE_BCS
%token DIRECTIVE_BNE
%token DIRECTIVE_BEQ
%token DIRECTIVE_BVC
%token DIRECTIVE_BVS
%token DIRECTIVE_BPL
%token DIRECTIVE_BMI
%token DIRECTIVE_BGE
%token DIRECTIVE_BLT
%token DIRECTIVE_BGT
%token DIRECTIVE_BLE
%token DIRECTIVE_MOVEQ
%token DIRECTIVE_DIVU
%token DIRECTIVE_DIVS
%token DIRECTIVE_SBCD
%token DIRECTIVE_OR
%token DIRECTIVE_SUB
%token DIRECTIVE_SUBX
%token DIRECTIVE_SUBA
%token DIRECTIVE_EOR
%token DIRECTIVE_CMPM
%token DIRECTIVE_CMP
%token DIRECTIVE_CMPA
%token DIRECTIVE_MULU
%token DIRECTIVE_MULS
%token DIRECTIVE_ABCD
%token DIRECTIVE_EXG
%token DIRECTIVE_AND
%token DIRECTIVE_ADD
%token DIRECTIVE_ADDX
%token DIRECTIVE_ADDA
%token DIRECTIVE_ASL
%token DIRECTIVE_ASR
%token DIRECTIVE_LSL
%token DIRECTIVE_LSR
%token DIRECTIVE_ROXL
%token DIRECTIVE_ROXR
%token DIRECTIVE_ROL
%token DIRECTIVE_ROR
%token DIRECTIVE_DC
%token DIRECTIVE_DCB
%token DIRECTIVE_DS
%token DIRECTIVE_REPT
%token DIRECTIVE_ENDR
%token DIRECTIVE_MACRO
%token DIRECTIVE_MACROS
%token DIRECTIVE_ENDM
%token DIRECTIVE_INCLUDE
%token DIRECTIVE_INCBIN
%token DIRECTIVE_EQU
%token DIRECTIVE_EQUS
%token DIRECTIVE_SUBSTR
%token DIRECTIVE_SET
%token DIRECTIVE_IF
%token DIRECTIVE_ELSEIF
%token DIRECTIVE_ELSE
%token DIRECTIVE_ENDC
%token DIRECTIVE_WHILE
%token DIRECTIVE_ENDW
%token DIRECTIVE_EVEN
%token DIRECTIVE_CNOP
%token DIRECTIVE_INFORM
%token DIRECTIVE_FAIL
%token DIRECTIVE_END
%token DIRECTIVE_RS
%token DIRECTIVE_RSSET
%token DIRECTIVE_RSRESET
%token DIRECTIVE_OBJ
%token DIRECTIVE_OBJEND
%token DIRECTIVE_ORG
%token DIRECTIVE_PUSHO
%token DIRECTIVE_POPO
%token DIRECTIVE_OPT
%token DIRECTIVE_PUSHP
%token DIRECTIVE_POPP
%token DIRECTIVE_SHIFT
%token DIRECTIVE_MEXIT
%token DIRECTIVE_LOCAL
%token DIRECTIVE_LIST
%token DIRECTIVE_NOLIST
%token DIRECTIVE_PURGE
%token SIZE_BYTE
%token SIZE_SHORT
%token SIZE_WORD
%token SIZE_LONGWORD
%token<unsigned_long> DATA_REGISTER
%token<unsigned_long> ADDRESS_REGISTER
%token<unsigned_long> NUMBER
%token<string> IDENTIFIER
%token<string> LOCAL_IDENTIFIER
%token<string> STRING
%token<string> OPTION
%token STATUS_REGISTER
%token CONDITION_CODE_REGISTER
%token USER_STACK_POINTER_REGISTER
%token PROGRAM_COUNTER
%token LOGICAL_AND
%token LOGICAL_OR
%token EQUALITY
%token INEQUALITY
%token LESS_OR_EQUAL
%token MORE_OR_EQUAL
%token LEFT_SHIFT
%token RIGHT_SHIFT
%token STRLEN
%token STRCMP
%token INSTR
%token DEF
%token TYPE
%token FILESIZE
%token OFFSET

%type<instruction> instruction
%type<opcode> opcode
%type<size> size
%type<opcode> full_opcode
%type<operand> operand
%type<unsigned_long> register_list
%type<unsigned_long> register_span
%type<unsigned_long> data_or_address_register
%type<expression_list> expression_list
%type<identifier_list> identifier_list option_list
%type<expression> expression expression1 expression2 expression3 expression4 expression5 expression6 expression7 expression8 string
%type<string> local_identifier identifier

%destructor { String_Destroy(&$$); } IDENTIFIER LOCAL_IDENTIFIER STRING OPTION local_identifier identifier
%destructor { DestroyOperand(&$$); } operand
%destructor { DestroyExpressionList(&$$); } expression_list
%destructor { DestroyIdentifierList(&$$); } identifier_list option_list
%destructor { DestroyExpression(&$$); } expression expression1 expression2 expression3 expression4 expression5 expression6 expression7 expression8 string

/* Hack to resolve ambiguity around size tokens. */
%precedence LOWER_PRECEDENCE_THAN_SIZE
%precedence SIZE_BYTE SIZE_SHORT SIZE_WORD SIZE_LONGWORD
%precedence HIGHER_PRECEDENCE_THAN_SIZE IDENTIFIER

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
	| DIRECTIVE_DC size expression_list
	{
		statement->type = STATEMENT_TYPE_DC;
		statement->shared.dc.size = $2;
		statement->shared.dc.values = $3;
	}
	| DIRECTIVE_DCB size expression ',' expression
	{
		statement->type = STATEMENT_TYPE_DCB;
		statement->shared.dcb.size = $2;
		statement->shared.dcb.repetitions = $3;
		statement->shared.dcb.value = $5;
	}
	| DIRECTIVE_DS size expression
	{
		statement->type = STATEMENT_TYPE_DS;
		statement->shared.ds.size = $2;
		statement->shared.ds.length = $3;
	}
	| DIRECTIVE_INCLUDE STRING
	{
		statement->type = STATEMENT_TYPE_INCLUDE;
		statement->shared.include.path = $2;
	}
	| DIRECTIVE_INCBIN STRING
	{
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = $2;
		statement->shared.incbin.start.type = EXPRESSION_NUMBER;
		statement->shared.incbin.start.shared.unsigned_long = 0;
		statement->shared.incbin.has_length = cc_false;
	}
	| DIRECTIVE_INCBIN STRING ',' expression
	{
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = $2;
		statement->shared.incbin.start = $4;
		statement->shared.incbin.has_length = cc_false;
	}
	| DIRECTIVE_INCBIN STRING ',' ',' expression
	{
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = $2;
		statement->shared.incbin.start.type = EXPRESSION_NUMBER;
		statement->shared.incbin.start.shared.unsigned_long = 0;
		statement->shared.incbin.has_length = cc_true;
		statement->shared.incbin.length = $5;
	}
	| DIRECTIVE_INCBIN STRING ',' expression ',' expression
	{
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = $2;
		statement->shared.incbin.start = $4;
		statement->shared.incbin.has_length = cc_true;
		statement->shared.incbin.length = $6;
	}
	| DIRECTIVE_REPT expression
	{
		statement->type = STATEMENT_TYPE_REPT;
		statement->shared.rept.repetitions = $2;
	}
	| DIRECTIVE_ENDR
	{
		statement->type = STATEMENT_TYPE_ENDR;
	}
	| DIRECTIVE_MACRO
	{
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
	| DIRECTIVE_MACRO '*'
	{
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_true;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
	| DIRECTIVE_MACRO identifier_list
	{
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names = $2;
	}
	| DIRECTIVE_MACRO '*' ',' identifier_list
	{
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_true;
		statement->shared.macro.parameter_names = $4;
	}
	| DIRECTIVE_MACROS
	{
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
	| DIRECTIVE_MACROS identifier_list
	{
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names = $2;
	}
	| DIRECTIVE_ENDM
	{
		statement->type = STATEMENT_TYPE_ENDM;
	}
	| DIRECTIVE_EQU expression
	{
		statement->type = STATEMENT_TYPE_EQU;
		statement->shared.expression = $2;
	}
	| DIRECTIVE_EQUS STRING
	{
		statement->type = STATEMENT_TYPE_EQUS_STRING;
		statement->shared.string = $2;
	}
	| DIRECTIVE_EQUS IDENTIFIER
	{
		statement->type = STATEMENT_TYPE_EQUS_IDENTIFIER;
		statement->shared.string = $2;
	}
	| DIRECTIVE_SUBSTR ',' ',' STRING
	{
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start.type = EXPRESSION_NUMBER;
		statement->shared.substr.start.shared.unsigned_long = 1;
		statement->shared.substr.end.type = EXPRESSION_NUMBER;
		statement->shared.substr.end.shared.unsigned_long = String_Length(&$4);
		statement->shared.substr.string = $4;
	}
	| DIRECTIVE_SUBSTR expression ',' ',' STRING
	{
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start = $2;
		statement->shared.substr.end.type = EXPRESSION_NUMBER;
		statement->shared.substr.end.shared.unsigned_long = String_Length(&$5);
		statement->shared.substr.string = $5;
	}
	| DIRECTIVE_SUBSTR ',' expression ',' STRING
	{
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start.type = EXPRESSION_NUMBER;
		statement->shared.substr.start.shared.unsigned_long = 1;
		statement->shared.substr.end = $3;
		statement->shared.substr.string = $5;
	}
	| DIRECTIVE_SUBSTR expression ',' expression ',' STRING
	{
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start = $2;
		statement->shared.substr.end = $4;
		statement->shared.substr.string = $6;
	}
	| DIRECTIVE_SET expression
	{
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.expression = $2;
	}
	| '=' expression
	{
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.expression = $2;
	}
	| DIRECTIVE_IF expression
	{
		statement->type = STATEMENT_TYPE_IF;
		statement->shared.expression = $2;
	}
	| DIRECTIVE_ELSEIF expression
	{
		statement->type = STATEMENT_TYPE_ELSEIF;
		statement->shared.expression = $2;
	}
	| DIRECTIVE_ELSEIF
	{
		statement->type = STATEMENT_TYPE_ELSE;
	}
	| DIRECTIVE_ELSE
	{
		statement->type = STATEMENT_TYPE_ELSE;
	}
	| DIRECTIVE_ENDC
	{
		statement->type = STATEMENT_TYPE_ENDC;
	}
	| DIRECTIVE_WHILE expression
	{
		statement->type = STATEMENT_TYPE_WHILE;
		statement->shared.expression = $2;
	}
	| DIRECTIVE_ENDW
	{
		statement->type = STATEMENT_TYPE_ENDW;
	}
	| DIRECTIVE_EVEN
	{
		statement->type = STATEMENT_TYPE_EVEN;
	}
	| DIRECTIVE_CNOP expression ',' expression
	{
		statement->type = STATEMENT_TYPE_CNOP;
		statement->shared.cnop.offset = $2;
		statement->shared.cnop.size_boundary = $4;
	}
	| DIRECTIVE_INFORM expression ',' STRING
	{
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.severity = $2;
		statement->shared.inform.message = $4;
	}
	| DIRECTIVE_INFORM expression ',' STRING ',' expression_list
	{
		(void)$6;

		/* TODO - parameters */
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.severity = $2;
		statement->shared.inform.message = $4;
		DestroyExpressionList(&$6);
	}
	| DIRECTIVE_FAIL
	{
		statement->type = STATEMENT_TYPE_FAIL;
	}
	| DIRECTIVE_END
	{
		statement->type = STATEMENT_TYPE_END;
	}
	| DIRECTIVE_RS size expression
	{
		statement->type = STATEMENT_TYPE_RS;
		statement->shared.rs.size = $2;
		statement->shared.rs.length = $3;
	}
	| DIRECTIVE_RSSET expression
	{
		statement->type = STATEMENT_TYPE_RSSET;
		statement->shared.expression = $2;
	}
	| DIRECTIVE_RSRESET expression
	{
		/* As mentioned in S.N. 68k's manual, this odd hack
		   is necessary for compatibility with other assemblers. */
		statement->type = STATEMENT_TYPE_RSSET;
		statement->shared.expression = $2;
	}
	| DIRECTIVE_RSRESET
	{
		statement->type = STATEMENT_TYPE_RSRESET;
	}
	| DIRECTIVE_OBJ expression
	{
		statement->type = STATEMENT_TYPE_OBJ;
		statement->shared.expression = $2;
	}
	| DIRECTIVE_OBJEND
	{
		statement->type = STATEMENT_TYPE_OBJEND;
	}
	| DIRECTIVE_ORG expression
	{
		statement->type = STATEMENT_TYPE_ORG;
		statement->shared.expression = $2;
	}
	| DIRECTIVE_PUSHO
	{
		statement->type = STATEMENT_TYPE_PUSHO;
	}
	| DIRECTIVE_POPO
	{
		statement->type = STATEMENT_TYPE_POPO;
	}
	| DIRECTIVE_OPT option_list
	{
		statement->type = STATEMENT_TYPE_OPT;
		statement->shared.opt.options = $2;
	}
	| DIRECTIVE_PUSHP STRING
	{
		statement->type = STATEMENT_TYPE_PUSHP;
		statement->shared.string = $2;
	}
	| DIRECTIVE_POPP IDENTIFIER
	{
		statement->type = STATEMENT_TYPE_POPP;
		statement->shared.string = $2;
	}
	| DIRECTIVE_SHIFT
	{
		statement->type = STATEMENT_TYPE_SHIFT;
	}
	| DIRECTIVE_MEXIT
	{
		statement->type = STATEMENT_TYPE_MEXIT;
	}
	| DIRECTIVE_LOCAL identifier_list
	{
		statement->type = STATEMENT_TYPE_LOCAL;
		statement->shared.local.identifiers = $2;
	}
	| DIRECTIVE_LIST
	{
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
	| DIRECTIVE_LIST '+'
	{
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
	| DIRECTIVE_LIST '-'
	{
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
	| DIRECTIVE_NOLIST
	{
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
	| DIRECTIVE_PURGE IDENTIFIER
	{
		statement->type = STATEMENT_TYPE_PURGE;
		statement->shared.string = $2;
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
	: IDENTIFIER
	{
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		if (node == NULL)
		{
			String_Destroy(&$1);
			YYNOMEM;
		}
		else
		{
			node->identifier = $1;
			node->next = NULL;
		}

		$$.head = $$.tail = node;
	}
	| identifier_list ',' IDENTIFIER
	{
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		$$ = $1;

		if (node == NULL)
		{
			DestroyIdentifierList(&$1);
			String_Destroy(&$3);
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

option_list
	: OPTION
	{
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		if (node == NULL)
		{
			String_Destroy(&$1);
			YYNOMEM;
		}
		else
		{
			node->identifier = $1;
			node->next = NULL;
		}

		$$.head = $$.tail = node;
	}
	| option_list ',' OPTION
	{
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		$$ = $1;

		if (node == NULL)
		{
			DestroyIdentifierList(&$1);
			String_Destroy(&$3);
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
	: opcode %prec LOWER_PRECEDENCE_THAN_SIZE
	{
		$$ = $1;
		$$.size = SIZE_UNDEFINED;
	}
	| opcode '.'
	{
		$$ = $1;
		$$.size = SIZE_UNDEFINED;
		m68kasm_warning_pedantic(scanner, statement, "Opcode has a dot but no size; either remove the dot or add an explicit size.");
	}
	| opcode size
	{
		$$ = $1;
		$$.size = $2;
	}
	;

opcode
	: DIRECTIVE_ORI
	{
		$$.type = OPCODE_ORI;
	}
	| DIRECTIVE_ANDI
	{
		$$.type = OPCODE_ANDI;
	}
	| DIRECTIVE_SUBI
	{
		$$.type = OPCODE_SUBI;
	}
	| DIRECTIVE_ADDI
	{
		$$.type = OPCODE_ADDI;
	}
	| DIRECTIVE_EORI
	{
		$$.type = OPCODE_EORI;
	}
	| DIRECTIVE_CMPI
	{
		$$.type = OPCODE_CMPI;
	}
	| DIRECTIVE_BTST
	{
		$$.type = OPCODE_BTST_STATIC;
	}
	| DIRECTIVE_BCHG
	{
		$$.type = OPCODE_BCHG_STATIC;
	}
	| DIRECTIVE_BCLR
	{
		$$.type = OPCODE_BCLR_STATIC;
	}
	| DIRECTIVE_BSET
	{
		$$.type = OPCODE_BSET_STATIC;
	}
	| DIRECTIVE_MOVEP
	{
		$$.type = OPCODE_MOVEP_TO_REG;
	}
	| DIRECTIVE_MOVEA
	{
		$$.type = OPCODE_MOVEA;
	}
	| DIRECTIVE_MOVE
	{
		$$.type = OPCODE_MOVE;
	}
	| DIRECTIVE_NEGX
	{
		$$.type = OPCODE_NEGX;
	}
	| DIRECTIVE_CLR
	{
		$$.type = OPCODE_CLR;
	}
	| DIRECTIVE_NEG
	{
		$$.type = OPCODE_NEG;
	}
	| DIRECTIVE_NOT
	{
		$$.type = OPCODE_NOT;
	}
	| DIRECTIVE_EXT
	{
		$$.type = OPCODE_EXT;
	}
	| DIRECTIVE_NBCD
	{
		$$.type = OPCODE_NBCD;
	}
	| DIRECTIVE_SWAP
	{
		$$.type = OPCODE_SWAP;
	}
	| DIRECTIVE_PEA
	{
		$$.type = OPCODE_PEA;
	}
	| DIRECTIVE_ILLEGAL
	{
		$$.type = OPCODE_ILLEGAL;
	}
	| DIRECTIVE_TAS
	{
		$$.type = OPCODE_TAS;
	}
	| DIRECTIVE_TST
	{
		$$.type = OPCODE_TST;
	}
	| DIRECTIVE_TRAP
	{
		$$.type = OPCODE_TRAP;
	}
	| DIRECTIVE_LINK
	{
		$$.type = OPCODE_LINK;
	}
	| DIRECTIVE_UNLK
	{
		$$.type = OPCODE_UNLK;
	}
	| DIRECTIVE_RESET
	{
		$$.type = OPCODE_RESET;
	}
	| DIRECTIVE_NOP
	{
		$$.type = OPCODE_NOP;
	}
	| DIRECTIVE_STOP
	{
		$$.type = OPCODE_STOP;
	}
	| DIRECTIVE_RTE
	{
		$$.type = OPCODE_RTE;
	}
	| DIRECTIVE_RTS
	{
		$$.type = OPCODE_RTS;
	}
	| DIRECTIVE_TRAPV
	{
		$$.type = OPCODE_TRAPV;
	}
	| DIRECTIVE_RTR
	{
		$$.type = OPCODE_RTR;
	}
	| DIRECTIVE_JSR
	{
		$$.type = OPCODE_JSR;
	}
	| DIRECTIVE_JMP
	{
		$$.type = OPCODE_JMP;
	}
	| DIRECTIVE_MOVEM
	{
		$$.type = OPCODE_MOVEM_TO_REGS;
	}
	| DIRECTIVE_LEA
	{
		$$.type = OPCODE_LEA;
	}
	| DIRECTIVE_CHK
	{
		$$.type = OPCODE_CHK;
	}
	| DIRECTIVE_ADDQ
	{
		$$.type = OPCODE_ADDQ;
	}
	| DIRECTIVE_SUBQ
	{
		$$.type = OPCODE_SUBQ;
	}
	| DIRECTIVE_ST
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_TRUE;
	}
	| DIRECTIVE_SF
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_FALSE;
	}
	| DIRECTIVE_SHI
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_HIGHER;
	}
	| DIRECTIVE_SLS
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_LOWER_OR_SAME;
	}
	| DIRECTIVE_SCC
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_CARRY_CLEAR;
	}
	| DIRECTIVE_SCS
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_CARRY_SET;
	}
	| DIRECTIVE_SNE
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_NOT_EQUAL;
	}
	| DIRECTIVE_SEQ
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_EQUAL;
	}
	| DIRECTIVE_SVC
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_OVERFLOW_CLEAR;
	}
	| DIRECTIVE_SVS
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_OVERFLOW_SET;
	}
	| DIRECTIVE_SPL
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_PLUS;
	}
	| DIRECTIVE_SMI
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_MINUS;
	}
	| DIRECTIVE_SGE
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_GREATER_OR_EQUAL;
	}
	| DIRECTIVE_SLT
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_LESS_THAN;
	}
	| DIRECTIVE_SGT
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_GREATER_THAN;
	}
	| DIRECTIVE_SLE
	{
		$$.type = OPCODE_Scc;
		$$.condition = CONDITION_LESS_OR_EQUAL;
	}
	| DIRECTIVE_DBT
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_TRUE;
	}
	| DIRECTIVE_DBF
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_FALSE;
	}
	| DIRECTIVE_DBHI
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_HIGHER;
	}
	| DIRECTIVE_DBLS
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_LOWER_OR_SAME;
	}
	| DIRECTIVE_DBCC
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_CARRY_CLEAR;
	}
	| DIRECTIVE_DBCS
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_CARRY_SET;
	}
	| DIRECTIVE_DBNE
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_NOT_EQUAL;
	}
	| DIRECTIVE_DBEQ
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_EQUAL;
	}
	| DIRECTIVE_DBVC
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_OVERFLOW_CLEAR;
	}
	| DIRECTIVE_DBVS
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_OVERFLOW_SET;
	}
	| DIRECTIVE_DBPL
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_PLUS;
	}
	| DIRECTIVE_DBMI
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_MINUS;
	}
	| DIRECTIVE_DBGE
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_GREATER_OR_EQUAL;
	}
	| DIRECTIVE_DBLT
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_LESS_THAN;
	}
	| DIRECTIVE_DBGT
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_GREATER_THAN;
	}
	| DIRECTIVE_DBLE
	{
		$$.type = OPCODE_DBcc;
		$$.condition = CONDITION_LESS_OR_EQUAL;
	}
	| DIRECTIVE_BRA
	{
		$$.type = OPCODE_BRA;
	}
	| DIRECTIVE_BSR
	{
		$$.type = OPCODE_BSR;
	}
	| DIRECTIVE_BHI
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_HIGHER;
	}
	| DIRECTIVE_BLS
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_LOWER_OR_SAME;
	}
	| DIRECTIVE_BCC
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_CARRY_CLEAR;
	}
	| DIRECTIVE_BCS
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_CARRY_SET;
	}
	| DIRECTIVE_BNE
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_NOT_EQUAL;
	}
	| DIRECTIVE_BEQ
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_EQUAL;
	}
	| DIRECTIVE_BVC
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_OVERFLOW_CLEAR;
	}
	| DIRECTIVE_BVS
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_OVERFLOW_SET;
	}
	| DIRECTIVE_BPL
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_PLUS;
	}
	| DIRECTIVE_BMI
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_MINUS;
	}
	| DIRECTIVE_BGE
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_GREATER_OR_EQUAL;
	}
	| DIRECTIVE_BLT
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_LESS_THAN;
	}
	| DIRECTIVE_BGT
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_GREATER_THAN;
	}
	| DIRECTIVE_BLE
	{
		$$.type = OPCODE_Bcc;
		$$.condition = CONDITION_LESS_OR_EQUAL;
	}
	| DIRECTIVE_MOVEQ
	{
		$$.type = OPCODE_MOVEQ;
	}
	| DIRECTIVE_DIVU
	{
		$$.type = OPCODE_DIVU;
	}
	| DIRECTIVE_DIVS
	{
		$$.type = OPCODE_DIVS;
	}
	| DIRECTIVE_SBCD
	{
		$$.type = OPCODE_SBCD_DATA_REGS;
	}
	| DIRECTIVE_OR
	{
		$$.type = OPCODE_OR_TO_REG;
	}
	| DIRECTIVE_SUB
	{
		$$.type = OPCODE_SUB_TO_REG;
	}
	| DIRECTIVE_SUBX
	{
		$$.type = OPCODE_SUBX_DATA_REGS;
	}
	| DIRECTIVE_SUBA
	{
		$$.type = OPCODE_SUBA;
	}
	| DIRECTIVE_EOR
	{
		$$.type = OPCODE_EOR;
	}
	| DIRECTIVE_CMPM
	{
		$$.type = OPCODE_CMPM;
	}
	| DIRECTIVE_CMP
	{
		$$.type = OPCODE_CMP;
	}
	| DIRECTIVE_CMPA
	{
		$$.type = OPCODE_CMPA;
	}
	| DIRECTIVE_MULU
	{
		$$.type = OPCODE_MULU;
	}
	| DIRECTIVE_MULS
	{
		$$.type = OPCODE_MULS;
	}
	| DIRECTIVE_ABCD
	{
		$$.type = OPCODE_ABCD_DATA_REGS;
	}
	| DIRECTIVE_EXG
	{
		$$.type = OPCODE_EXG;
	}
	| DIRECTIVE_AND
	{
		$$.type = OPCODE_AND_TO_REG;
	}
	| DIRECTIVE_ADD
	{
		$$.type = OPCODE_ADD_TO_REG;
	}
	| DIRECTIVE_ADDX
	{
		$$.type = OPCODE_ADDX_DATA_REGS;
	}
	| DIRECTIVE_ADDA
	{
		$$.type = OPCODE_ADDA;
	}
	| DIRECTIVE_ASL
	{
		$$.type = OPCODE_ASL_STATIC;
	}
	| DIRECTIVE_ASR
	{
		$$.type = OPCODE_ASR_STATIC;
	}
	| DIRECTIVE_LSL
	{
		$$.type = OPCODE_LSL_STATIC;
	}
	| DIRECTIVE_LSR
	{
		$$.type = OPCODE_LSR_STATIC;
	}
	| DIRECTIVE_ROXL
	{
		$$.type = OPCODE_ROXL_STATIC;
	}
	| DIRECTIVE_ROXR
	{
		$$.type = OPCODE_ROXR_STATIC;
	}
	| DIRECTIVE_ROL
	{
		$$.type = OPCODE_ROL_STATIC;
	}
	| DIRECTIVE_ROR
	{
		$$.type = OPCODE_ROR_STATIC;
	}
	;

size
	: SIZE_BYTE
	{
		$$ = SIZE_BYTE;
	}
	| SIZE_SHORT
	{
		$$ = SIZE_SHORT;
	}
	| SIZE_WORD
	{
		$$ = SIZE_WORD;
	}
	| SIZE_LONGWORD
	{
		$$ = SIZE_LONGWORD;
	}
	;

	/* Indirect address register */
operand
	: '(' ADDRESS_REGISTER ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT;
		$$.main_register = $2;
	}
	| '(' ADDRESS_REGISTER ')' '+'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
		$$.main_register = $2;
	}
	| '-' '(' ADDRESS_REGISTER ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
		$$.main_register = $3;
	}
	| expression '(' ADDRESS_REGISTER ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		$$.literal = $1;
		$$.main_register = $3;
	}
	| '(' expression ',' ADDRESS_REGISTER ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		$$.literal = $2;
		$$.main_register = $4;
	}
	| '(' ADDRESS_REGISTER ',' expression ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		$$.literal = $4;
		$$.main_register = $2;
	}
	| '(' ADDRESS_REGISTER ',' data_or_address_register size ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = EXPRESSION_NUMBER;
		$$.literal.shared.unsigned_long = 0;
		$$.main_register = $2;
		$$.index_register = $4 % 8;
		$$.size = $5;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| expression '(' ADDRESS_REGISTER ',' data_or_address_register size ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.main_register = $3;
		$$.index_register = $5 % 8;
		$$.size = $6;
		$$.index_register_is_address_register = $5 / 8 !=0;
	}
	| '(' expression ',' ADDRESS_REGISTER ',' data_or_address_register size ')'
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $2;
		$$.main_register = $4;
		$$.index_register = $6 % 8;
		$$.size = $7;
		$$.index_register_is_address_register = $6 / 8 !=0;
	}
	| '(' ADDRESS_REGISTER ',' data_or_address_register ')'
	{
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = EXPRESSION_NUMBER;
		$$.literal.shared.unsigned_long = 0;
		$$.main_register = $2;
		$$.index_register = $4 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| expression '(' ADDRESS_REGISTER ',' data_or_address_register ')'
	{
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.main_register = $3;
		$$.index_register = $5 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $5 / 8 !=0;
	}
	| '(' expression ',' ADDRESS_REGISTER ',' data_or_address_register ')'
	{
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $2;
		$$.main_register = $4;
		$$.index_register = $6 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $6 / 8 !=0;
	}
	| expression '(' PROGRAM_COUNTER ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		$$.literal = $1;
	}
	| '(' expression ',' PROGRAM_COUNTER ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		$$.literal = $2;
	}
	| '(' PROGRAM_COUNTER ',' expression ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		$$.literal = $4;
	}
	| '(' PROGRAM_COUNTER ',' data_or_address_register size ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = EXPRESSION_NUMBER;
		$$.literal.shared.unsigned_long = 0;
		$$.index_register = $4 % 8;
		$$.size = $5;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| expression '(' PROGRAM_COUNTER ',' data_or_address_register size ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.index_register = $5 % 8;
		$$.size = $6;
		$$.index_register_is_address_register = $5 / 8 != 0;
	}
	| '(' expression ',' PROGRAM_COUNTER ',' data_or_address_register size ')'
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $2;
		$$.index_register = $6 % 8;
		$$.size = $7;
		$$.index_register_is_address_register = $6 / 8 != 0;
	}
	| '(' PROGRAM_COUNTER ',' data_or_address_register ')'
	{
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.type = EXPRESSION_NUMBER;
		$$.literal.shared.unsigned_long = 0;
		$$.index_register = $4 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $4 / 8 != 0;
	}
	| expression '(' PROGRAM_COUNTER ',' data_or_address_register ')'
	{
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.index_register = $5 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $5 / 8 != 0;
	}
	| '(' expression ',' PROGRAM_COUNTER ',' data_or_address_register ')'
	{
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $2;
		$$.index_register = $6 % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = $6 / 8 != 0;
	}
	/* Literal */
	| '#' expression
	{
		$$.type = OPERAND_LITERAL;
		$$.literal = $2;
	}
	/* Registers */
/*	| DATA_REGISTER
	{
		$$.type = OPERAND_DATA_REGISTER;
		$$.main_register = $1;
	}
	| ADDRESS_REGISTER
	{
		$$.type = OPERAND_ADDRESS_REGISTER;
		$$.main_register = $1;
	}
*/	| STATUS_REGISTER
	{
		$$.type = OPERAND_STATUS_REGISTER;
	}
	| CONDITION_CODE_REGISTER
	{
		$$.type = OPERAND_CONDITION_CODE_REGISTER;
	}
	| USER_STACK_POINTER_REGISTER
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
	| expression size
	{
		$$.type = OPERAND_ADDRESS_ABSOLUTE;
		$$.literal = $1;
		$$.size = $2;
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
	: DATA_REGISTER
	{
		$$ = 0 + $1;
	}
	| ADDRESS_REGISTER
	{
		$$ = 8 + $1;
	}
	;

/*
number
	: NUMBER
	| '$' NUMBER
	;
*/
expression
	: expression1
	{
		$$ = $1;
	}
	/* This is an assembler extension: asm68k doesn't support this. */
	| expression LOGICAL_AND expression1
	{
		if (!DoExpression(&$$, EXPRESSION_LOGICAL_AND, &$1, &$3))
			YYNOMEM;
	}
	/* This is an assembler extension: asm68k doesn't support this. */
	| expression LOGICAL_OR expression1
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
	| expression1 EQUALITY expression2
	{
		if (!DoExpression(&$$, EXPRESSION_EQUALITY, &$1, &$3))
			YYNOMEM;
	}
	| expression1 INEQUALITY expression2
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
	| expression2 LESS_OR_EQUAL expression3
	{
		if (!DoExpression(&$$, EXPRESSION_LESS_OR_EQUAL, &$1, &$3))
			YYNOMEM;
	}
	| expression2 '>' expression3
	{
		if (!DoExpression(&$$, EXPRESSION_MORE_THAN, &$1, &$3))
			YYNOMEM;
	}
	| expression2 MORE_OR_EQUAL expression3
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
	| expression6 LEFT_SHIFT expression7
	{
		if (!DoExpression(&$$, EXPRESSION_LEFT_SHIFT, &$1, &$3))
			YYNOMEM;
	}
	| expression6 RIGHT_SHIFT expression7
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

local_identifier
	: LOCAL_IDENTIFIER
	| SIZE_BYTE
	{
		const StringView literal = STRING_VIEW_INITIALISER(".b");
		if (!String_CreateCopyView(&$$, &literal))
			YYNOMEM;
	}
	| SIZE_SHORT
	{
		const StringView literal = STRING_VIEW_INITIALISER(".s");
		if (!String_CreateCopyView(&$$, &literal))
			YYNOMEM;
	}
	| SIZE_WORD
	{
		const StringView literal = STRING_VIEW_INITIALISER(".w");
		if (!String_CreateCopyView(&$$, &literal))
			YYNOMEM;
	}
	| SIZE_LONGWORD
	{
		const StringView literal = STRING_VIEW_INITIALISER(".l");
		if (!String_CreateCopyView(&$$, &literal))
			YYNOMEM;
	}
	;

identifier
	: IDENTIFIER
	| local_identifier
	| identifier local_identifier
	{
		const cc_bool success = String_CreateAppend(&$$, &$1, &$2);
		String_Destroy(&$1);
		String_Destroy(&$2);

		if (!success)
			YYNOMEM;
	}
	;

expression8
	: NUMBER
	{
		$$.type = EXPRESSION_NUMBER;
		$$.shared.unsigned_long = $1;
	}
	| identifier %prec HIGHER_PRECEDENCE_THAN_SIZE
	{
		$$.type = EXPRESSION_IDENTIFIER;
		$$.shared.string = $1;
	}
	| string
	{
		$$ = $1;
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
	| STRLEN '(' STRING ')'
	{
		$$.type = EXPRESSION_STRLEN;
		$$.shared.string = $3;
	}
	| STRCMP '(' string ',' string ')'
	{
		if (!DoExpression(&$$, EXPRESSION_STRCMP, &$3, &$5))
			YYNOMEM;
	}
	| INSTR '(' expression ',' expression ')'
	{
		Expression expression;

		expression.type = EXPRESSION_NUMBER;
		expression.shared.unsigned_long = 1;

		if (!DoExpressionTriple(&$$, EXPRESSION_INSTR, &expression, &$3, &$5))
			YYNOMEM;
	}
	| INSTR '(' expression ',' expression ',' expression ')'
	{
		if (!DoExpressionTriple(&$$, EXPRESSION_INSTR, &$3, &$5, &$7))
			YYNOMEM;
	}
	| DEF '(' IDENTIFIER ')'
	{
		$$.type = EXPRESSION_DEF;
		$$.shared.string = $3;
	}
	| TYPE '(' IDENTIFIER ')'
	{
		$$.type = EXPRESSION_TYPE_WITH_IDENTIFIER;
		$$.shared.string = $3;
	}
	| TYPE '(' NUMBER ')'
	{
		$$.type = EXPRESSION_TYPE_WITH_NUMBER;
		$$.shared.unsigned_long = $3;
	}
	| FILESIZE '(' STRING ')'
	{
		$$.type = EXPRESSION_FILESIZE;
		$$.shared.string = $3;
	}
	| OFFSET '(' expression ')'
	{
		/* TODO: Make this actually do something besides pass the expression through. */
		$$ = $3;
	}
	;

string
	: STRING
	{
		$$.type = EXPRESSION_STRING;
		$$.shared.string = $1;
	}

%%

static cc_bool DoExpressionTriple(Expression *expression, ExpressionType type, Expression *left_expression, Expression *middle_expression, Expression *right_expression)
{
	cc_bool success = cc_true;

	expression->type = type;

	expression->shared.subexpressions = (Expression*)malloc(sizeof(Expression) * (right_expression != NULL ? 3 : middle_expression != NULL ? 2 : 1));

	if (expression->shared.subexpressions == NULL)
	{
		DestroyExpression(left_expression);

		if (middle_expression != NULL)
			DestroyExpression(middle_expression);

		if (right_expression != NULL)
			DestroyExpression(right_expression);

		success = cc_false;
	}
	else
	{
		expression->shared.subexpressions[0] = *left_expression;

		if (middle_expression != NULL)
			expression->shared.subexpressions[1] = *middle_expression;

		if (right_expression != NULL)
			expression->shared.subexpressions[2] = *right_expression;
	}

	return success;
}

static cc_bool DoExpression(Expression *expression, ExpressionType type, Expression *left_expression, Expression *right_expression)
{
	return DoExpressionTriple(expression, type, left_expression, right_expression, NULL);
}

void DestroyExpression(Expression *expression)
{
	switch (expression->type)
	{
		case EXPRESSION_INSTR:
			DestroyExpression(&expression->shared.subexpressions[0]);
			DestroyExpression(&expression->shared.subexpressions[1]);
			DestroyExpression(&expression->shared.subexpressions[2]);
			free(expression->shared.subexpressions);
			break;

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
		case EXPRESSION_STRCMP:
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
}

void DestroyIdentifierList(IdentifierList *list)
{
	IdentifierListNode *node = list->head;

	while (node != NULL)
	{
		IdentifierListNode* const next_node = node->next;

		String_Destroy(&node->identifier);

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
		case STATEMENT_TYPE_FAIL:
		case STATEMENT_TYPE_RSRESET:
		case STATEMENT_TYPE_OBJEND:
		case STATEMENT_TYPE_PUSHO:
		case STATEMENT_TYPE_POPO:
		case STATEMENT_TYPE_SHIFT:
		case STATEMENT_TYPE_MEXIT:
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

		case STATEMENT_TYPE_DS:
			DestroyExpression(&statement->shared.ds.length);
			break;

		case STATEMENT_TYPE_INCLUDE:
			String_Destroy(&statement->shared.include.path);
			break;

		case STATEMENT_TYPE_INCBIN:
			String_Destroy(&statement->shared.incbin.path);
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
		case STATEMENT_TYPE_OBJ:
		case STATEMENT_TYPE_ORG:
			DestroyExpression(&statement->shared.expression);
			break;

		case STATEMENT_TYPE_EQUS_STRING:
		case STATEMENT_TYPE_EQUS_IDENTIFIER:
		case STATEMENT_TYPE_PUSHP:
		case STATEMENT_TYPE_POPP:
		case STATEMENT_TYPE_PURGE:
			String_Destroy(&statement->shared.string);
			break;

		case STATEMENT_TYPE_SUBSTR:
			DestroyExpression(&statement->shared.substr.start);
			DestroyExpression(&statement->shared.substr.end);
			String_Destroy(&statement->shared.substr.string);
			break;

		case STATEMENT_TYPE_CNOP:
			DestroyExpression(&statement->shared.cnop.offset);
			DestroyExpression(&statement->shared.cnop.size_boundary);
			break;

		case STATEMENT_TYPE_INFORM:
			DestroyExpression(&statement->shared.inform.severity);
			String_Destroy(&statement->shared.inform.message);
			break;

		case STATEMENT_TYPE_RS:
			DestroyExpression(&statement->shared.rs.length);
			break;

		case STATEMENT_TYPE_OPT:
			DestroyIdentifierList(&statement->shared.opt.options);
			break;

		case STATEMENT_TYPE_LOCAL:
			DestroyIdentifierList(&statement->shared.local.identifiers);
			break;
	}
}
