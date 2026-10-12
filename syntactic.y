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

%language "c++"

%require "3.2"

%define api.prefix {m68kasm_}
%define api.namespace {m68kasm}

%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define api.value.automove 

%param {void *scanner}
%param {ErrorManager &error_manager}

%parse-param {Statement &statement}

%define parse.error verbose

%code requires {

#include <array>
#include <deque>
#include <memory>
#include <string>
#include <string_view>
#include <variant>

class ErrorManager;

enum Size
{
	SIZE_BYTE      = 1 << 0,
	SIZE_SHORT     = 1 << 1,
	SIZE_WORD      = 1 << 2,
	SIZE_LONGWORD  = 1 << 3,
	SIZE_UNDEFINED = 1 << 4
};

enum Condition
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
};

enum OpcodeType
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
};

struct ExpressionArithmetic1Operand;
struct ExpressionArithmetic2Operand;
struct ExpressionInstr;
struct ExpressionStrcmp;
struct ExpressionIdentifier;
struct ExpressionString;
struct ExpressionStrlen;
struct ExpressionDef;
struct ExpressionTypeWithIdentifier;
struct ExpressionFilesize;
struct ExpressionNumber;
struct ExpressionProgramCounterOfStatement;
struct ExpressionProgramCounterOfExpression;
struct ExpressionTypeWithNumber;

using Expression = std::variant<
	ExpressionArithmetic1Operand,
	ExpressionArithmetic2Operand,
	ExpressionInstr,
	ExpressionStrcmp,
	ExpressionIdentifier,
	ExpressionString,
	ExpressionStrlen,
	ExpressionDef,
	ExpressionTypeWithIdentifier,
	ExpressionFilesize,
	ExpressionNumber,
	ExpressionProgramCounterOfStatement,
	ExpressionProgramCounterOfExpression,
	ExpressionTypeWithNumber
>;

struct ExpressionArithmetic1Operand
{
	enum class Type
	{
		NEGATE,
		BITWISE_NOT,
		LOGICAL_NOT,

	} type;
	std::unique_ptr<Expression> subexpressions;
};

struct ExpressionArithmetic2Operand
{
	enum class Type
	{
		SUBTRACT,
		ADD,
		MULTIPLY,
		DIVIDE,
		MODULO,
		LOGICAL_OR,
		LOGICAL_AND,
		BITWISE_OR,
		BITWISE_XOR,
		BITWISE_AND,
		EQUALITY,
		INEQUALITY,
		LESS_THAN,
		LESS_OR_EQUAL,
		MORE_THAN,
		MORE_OR_EQUAL,
		LEFT_SHIFT,
		RIGHT_SHIFT,
	} type;
	std::unique_ptr<Expression[]> subexpressions;
};

struct ExpressionInstr
{
	std::unique_ptr<Expression[]> subexpressions;
};

struct ExpressionStrcmp
{
	std::string string_a, string_b;
};

struct ExpressionIdentifier
{
	std::string string;
};

struct ExpressionString
{
	std::string string;
};

struct ExpressionStrlen
{
	std::string string;
};

struct ExpressionDef
{
	std::string string;
};

struct ExpressionTypeWithIdentifier
{
	std::string string;
};

struct ExpressionFilesize
{
	std::string string;
};

struct ExpressionNumber
{
	unsigned long value;
};

struct ExpressionProgramCounterOfStatement
{
	unsigned long value;
};

struct ExpressionProgramCounterOfExpression
{
	unsigned long value;
};

struct ExpressionTypeWithNumber
{
	unsigned long value;
};

using ExpressionList = std::deque<Expression>;
using IdentifierList = std::deque<std::string>;

struct Opcode
{
	OpcodeType type;
	Condition condition;
	Size size;
};

enum OperandType
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
};

struct Operand
{
	OperandType type;

	unsigned int main_register;
	unsigned int index_register;
	Size size;
	Expression literal;
	bool index_register_is_address_register;
};

struct StatementEmpty
{
};

struct StatementInstruction
{
	Opcode opcode;
	std::array<Operand, 2> operands;
};

struct StatementDc
{
	Size size;
	ExpressionList values;
};

struct StatementDcb
{
	Size size;
	Expression repetitions;
	Expression value;
};

struct StatementDs
{
	Size size;
	Expression length;
};

struct StatementInclude
{
	std::string path;
};

struct StatementIncbin
{
	std::string path;
	Expression start;
	bool has_length;
	Expression length;
};

struct StatementRept
{
	Expression repetitions;
};

struct StatementEndr
{
};

struct StatementMacro
{
	bool uses_label;
	IdentifierList parameter_names;
};

struct StatementMacros
{
	StatementMacro macro;
};

struct StatementEndm
{
};

struct StatementEqu
{
	Expression expression;
};

struct StatementEqusString
{
	std::string string;
};

struct StatementEqusIdentifier
{
	std::string string;
};

struct StatementSubstr
{
	Expression start, end;
	std::string string;
};

struct StatementSet
{
	Expression expression;
};

struct StatementIf
{
	Expression expression;
};

struct StatementElseif
{
	Expression expression;
};

struct StatementElse
{
};

struct StatementEndc
{
};

struct StatementWhile
{
	Expression expression;
};

struct StatementEndw
{
};

struct StatementEven
{
};

struct StatementCnop
{
	Expression offset;
	Expression size_boundary;
};

struct StatementInform
{
	Expression severity;
	std::string message;
};

struct StatementFail
{
};

struct StatementEnd
{
};

struct StatementRs
{
	Size size;
	Expression length;
};

struct StatementRsset
{
	Expression expression;
};

struct StatementRsreset
{
};

struct StatementObj
{
	Expression expression;
};

struct StatementObjend
{
};

struct StatementOrg
{
	Expression expression;
};

struct StatementPusho
{
};

struct StatementPopo
{
};

struct StatementOpt
{
	IdentifierList options;
};

struct StatementPushp
{
	std::string string;
};

struct StatementPopp
{
	std::string string;
};

struct StatementShift
{
};

struct StatementMexit
{
};

struct StatementLocal
{
	IdentifierList identifiers;
};

struct StatementPurge
{
	std::string string;
};

using Statement = std::variant<
	StatementEmpty,
	StatementInstruction,
	StatementDc,
	StatementDcb,
	StatementDs,
	StatementInclude,
	StatementIncbin,
	StatementRept,
	StatementEndr,
	StatementMacro,
	StatementMacros,
	StatementEndm,
	StatementEqu,
	StatementEqusString,
	StatementEqusIdentifier,
	StatementSubstr,
	StatementSet,
	StatementIf,
	StatementElseif,
	StatementElse,
	StatementEndc,
	StatementWhile,
	StatementEndw,
	StatementEven,
	StatementCnop,
	StatementInform,
	StatementFail,
	StatementEnd,
	StatementRs,
	StatementRsset,
	StatementRsreset,
	StatementObj,
	StatementObjend,
	StatementOrg,
	StatementPusho,
	StatementPopo,
	StatementOpt,
	StatementPushp,
	StatementPopp,
	StatementShift,
	StatementMexit,
	StatementLocal,
	StatementPurge
>;

}

%code provides {

#define YY_DECL m68kasm::parser::symbol_type m68kasm_lex(void *yyscanner, ErrorManager &error_manager)

}

%code {

#include "error.h"

YY_DECL;

static void DoExpression(Expression &expression, ExpressionArithmetic1Operand::Type type, Expression subexpression)
{
	auto subexpression_copy = std::make_unique<Expression>(std::move(subexpression));

	expression.emplace<ExpressionArithmetic1Operand>(type, std::move(subexpression_copy));
}

static void DoExpression(Expression &expression, ExpressionArithmetic2Operand::Type type, Expression left_expression, Expression right_expression)
{
	auto subexpressions = std::make_unique<Expression[]>(2);

	subexpressions[0] = std::move(left_expression);
	subexpressions[1] = std::move(right_expression);

	expression.emplace<ExpressionArithmetic2Operand>(type, std::move(subexpressions));
}

void m68kasm::parser::error(const std::string &message)
{
	error_manager.Error(message);
}

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
%token<unsigned long> DATA_REGISTER
%token<unsigned long> ADDRESS_REGISTER
%token<unsigned long> NUMBER
%token<std::string> IDENTIFIER
%token<std::string> LOCAL_IDENTIFIER
%token<std::string> STRING
%token<std::string> OPTION
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
%token PERIOD "."
%token COMMA ","
%token PARENTHESIS_LEFT "("
%token PARENTHESIS_RIGHT ")"
%token DOLLAR "$"
%token PLUS "+"
%token MINUS "-"
%token ASTERIX "*"
%token FORWARD_SLASH "/"
%token EQUAL "="
%token AT "@"
%token HASH "#"
%token AMPERSAND "&"
%token CARET "^"
%token EXCLAMATION "!"
%token VERTICAL_BAR "|"
%token ANGLE_LEFT "<"
%token ANGLE_RIGHT ">"
%token PERCENTAGE "%"
%token TILDE "~"

%type<StatementInstruction> instruction
%type<Opcode> opcode
%type<Size> size
%type<Opcode> full_opcode
%type<Operand> operand
%type<unsigned long> register_list
%type<unsigned long> register_span
%type<unsigned long> data_or_address_register
%type<ExpressionList> expression_list
%type<IdentifierList> identifier_list option_list
%type<Expression> expression expression1 expression2 expression3 expression4 expression5 expression6 expression7 expression8 string
%type<std::string> local_identifier identifier

/* Hack to resolve ambiguity around size tokens. */
%precedence LOWER_PRECEDENCE_THAN_SIZE
%precedence SIZE_BYTE SIZE_SHORT SIZE_WORD SIZE_LONGWORD
%precedence HIGHER_PRECEDENCE_THAN_SIZE IDENTIFIER

%start statement

%%

statement
	:
	{
		statement.emplace<StatementEmpty>();
	}
	| instruction
	{
		statement.emplace<StatementInstruction>($1);
	}
	| DIRECTIVE_DC size expression_list
	{
		statement.emplace<StatementDc>($2, $3);
	}
	| DIRECTIVE_DCB size expression "," expression
	{
		statement.emplace<StatementDcb>($2, $3, $5);
	}
	| DIRECTIVE_DS size expression
	{
		statement.emplace<StatementDs>($2, $3);
	}
	| DIRECTIVE_INCLUDE STRING
	{
		statement.emplace<StatementInclude>($2);
	}
	| DIRECTIVE_INCBIN STRING
	{		
		statement.emplace<StatementIncbin>($2, ExpressionNumber{0}, false);
	}
	| DIRECTIVE_INCBIN STRING "," expression
	{
		statement.emplace<StatementIncbin>($2, $4, false);
	}
	| DIRECTIVE_INCBIN STRING "," "," expression
	{
		statement.emplace<StatementIncbin>($2, ExpressionNumber{0}, true, $5);
	}
	| DIRECTIVE_INCBIN STRING "," expression "," expression
	{
		statement.emplace<StatementIncbin>($2, $4, true, $6);
	}
	| DIRECTIVE_REPT expression
	{
		statement.emplace<StatementRept>($2);
	}
	| DIRECTIVE_ENDR
	{
		statement.emplace<StatementEndr>();
	}
	| DIRECTIVE_MACRO
	{
		statement.emplace<StatementMacro>(false);
	}
	| DIRECTIVE_MACRO "*"
	{
		statement.emplace<StatementMacro>(true);
	}
	| DIRECTIVE_MACRO identifier_list
	{
		statement.emplace<StatementMacro>(false, $2);
	}
	| DIRECTIVE_MACRO "*" "," identifier_list
	{
		statement.emplace<StatementMacro>(true, $4);
	}
	| DIRECTIVE_MACROS
	{
		statement.emplace<StatementMacros>(StatementMacro{false, {}});
	}
	| DIRECTIVE_MACROS identifier_list
	{
		statement.emplace<StatementMacros>(StatementMacro{false, $2});
	}
	| DIRECTIVE_ENDM
	{
		statement.emplace<StatementEndm>();
	}
	| DIRECTIVE_EQU expression
	{
		statement.emplace<StatementEqu>($2);
	}
	| DIRECTIVE_EQUS STRING
	{
		statement.emplace<StatementEqusString>($2);
	}
	| DIRECTIVE_EQUS IDENTIFIER
	{
		statement.emplace<StatementEqusIdentifier>($2);
	}
	| DIRECTIVE_SUBSTR "," "," STRING
	{
		std::string string = $4;
		const auto string_length = string.length();
		statement.emplace<StatementSubstr>(ExpressionNumber{1}, ExpressionNumber{string_length}, std::move(string));
	}
	| DIRECTIVE_SUBSTR expression "," "," STRING
	{
		std::string string = $5;
		const auto string_length = string.length();
		statement.emplace<StatementSubstr>($2, ExpressionNumber{string_length}, std::move(string));
	}
	| DIRECTIVE_SUBSTR "," expression "," STRING
	{
		statement.emplace<StatementSubstr>(ExpressionNumber{1}, $3, $5);
	}
	| DIRECTIVE_SUBSTR expression "," expression "," STRING
	{
		statement.emplace<StatementSubstr>($2, $4, $6);
	}
	| DIRECTIVE_SET expression
	{
		statement.emplace<StatementSet>($2);
	}
	| "=" expression
	{
		statement.emplace<StatementSet>($2);
	}
	| DIRECTIVE_IF expression
	{
		statement.emplace<StatementIf>($2);
	}
	| DIRECTIVE_ELSEIF expression
	{
		statement.emplace<StatementElseif>($2);
	}
	| DIRECTIVE_ELSEIF
	{
		statement.emplace<StatementElse>();
	}
	| DIRECTIVE_ELSE
	{
		statement.emplace<StatementElse>();
	}
	| DIRECTIVE_ENDC
	{
		statement.emplace<StatementEndc>();
	}
	| DIRECTIVE_WHILE expression
	{
		statement.emplace<StatementWhile>($2);
	}
	| DIRECTIVE_ENDW
	{
		statement.emplace<StatementEndw>();
	}
	| DIRECTIVE_EVEN
	{
		statement.emplace<StatementEven>();
	}
	| DIRECTIVE_CNOP expression "," expression
	{
		statement.emplace<StatementCnop>($2, $4);
	}
	| DIRECTIVE_INFORM expression "," STRING
	{
		statement.emplace<StatementInform>($2, $4);
	}
	| DIRECTIVE_INFORM expression "," STRING "," expression_list
	{
		/* TODO - parameters */
		statement.emplace<StatementInform>($2, $4);
	}
	| DIRECTIVE_FAIL
	{
		statement.emplace<StatementFail>();
	}
	| DIRECTIVE_END
	{
		statement.emplace<StatementEnd>();
	}
	| DIRECTIVE_RS size expression
	{
		statement.emplace<StatementRs>($2, $3);
	}
	| DIRECTIVE_RSSET expression
	{
		statement.emplace<StatementRsset>($2);
	}
	| DIRECTIVE_RSRESET expression
	{
		/* As mentioned in S.N. 68k's manual, this odd hack
		   is necessary for compatibility with other assemblers. */
		statement.emplace<StatementRsset>($2);
	}
	| DIRECTIVE_RSRESET
	{
		statement.emplace<StatementRsreset>();
	}
	| DIRECTIVE_OBJ expression
	{
		statement.emplace<StatementObj>($2);
	}
	| DIRECTIVE_OBJEND
	{
		statement.emplace<StatementObjend>();
	}
	| DIRECTIVE_ORG expression
	{
		statement.emplace<StatementOrg>($2);
	}
	| DIRECTIVE_PUSHO
	{
		statement.emplace<StatementPusho>();
	}
	| DIRECTIVE_POPO
	{
		statement.emplace<StatementPopo>();
	}
	| DIRECTIVE_OPT option_list
	{
		statement.emplace<StatementOpt>($2);
	}
	| DIRECTIVE_PUSHP STRING
	{
		statement.emplace<StatementPushp>($2);
	}
	| DIRECTIVE_POPP IDENTIFIER
	{
		statement.emplace<StatementPopp>($2);
	}
	| DIRECTIVE_SHIFT
	{
		statement.emplace<StatementShift>();
	}
	| DIRECTIVE_MEXIT
	{
		statement.emplace<StatementMexit>();
	}
	| DIRECTIVE_LOCAL identifier_list
	{
		statement.emplace<StatementLocal>($2);
	}
	| DIRECTIVE_LIST
	{
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
	| DIRECTIVE_LIST "+"
	{
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
	| DIRECTIVE_LIST "-"
	{
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
	| DIRECTIVE_NOLIST
	{
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
	| DIRECTIVE_PURGE IDENTIFIER
	{
		statement.emplace<StatementPurge>($2);
	}
	;

expression_list
	: expression
	{
		$$.emplace_back($1);
	}
	| expression_list "," expression
	{
		$$ = $1;

		$$.emplace_back($3);
	}
	;

identifier_list
	: IDENTIFIER
	{
		$$.emplace_back($1);
	}
	| identifier_list "," IDENTIFIER
	{
		$$ = $1;

		$$.emplace_back($3);
	}
	;

option_list
	: OPTION
	{
		$$.emplace_back($1);
	}
	| option_list "," OPTION
	{
		$$ = $1;

		$$.emplace_back($3);
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
	| full_opcode operand "," operand
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
	| opcode "."
	{
		$$ = $1;
		$$.size = SIZE_UNDEFINED;
		error_manager.WarningPedantic("Opcode has a dot but no size; either remove the dot or add an explicit size.");
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
	: "(" ADDRESS_REGISTER ")"
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT;
		$$.main_register = $2;
	}
	| "(" ADDRESS_REGISTER ")" "+"
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
		$$.main_register = $2;
	}
	| "-" "(" ADDRESS_REGISTER ")"
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
		$$.main_register = $3;
	}
	| expression "(" ADDRESS_REGISTER ")"
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		$$.literal = $1;
		$$.main_register = $3;
	}
	| "(" expression "," ADDRESS_REGISTER ")"
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		$$.literal = $2;
		$$.main_register = $4;
	}
	| "(" ADDRESS_REGISTER "," expression ")"
	{
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		$$.literal = $4;
		$$.main_register = $2;
	}
	| "(" ADDRESS_REGISTER "," data_or_address_register size ")"
	{
		const auto data_or_address_register = $4;

		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.emplace<ExpressionNumber>(0);
		$$.main_register = $2;
		$$.index_register = data_or_address_register % 8;
		$$.size = $5;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	| expression "(" ADDRESS_REGISTER "," data_or_address_register size ")"
	{
		const auto data_or_address_register = $5;

		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.main_register = $3;
		$$.index_register = data_or_address_register % 8;
		$$.size = $6;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	| "(" expression "," ADDRESS_REGISTER "," data_or_address_register size ")"
	{
		const auto data_or_address_register = $6;

		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $2;
		$$.main_register = $4;
		$$.index_register = data_or_address_register % 8;
		$$.size = $7;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	| "(" ADDRESS_REGISTER "," data_or_address_register ")"
	{
		const auto data_or_address_register = $4;

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.emplace<ExpressionNumber>(0);
		$$.main_register = $2;
		$$.index_register = data_or_address_register % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	| expression "(" ADDRESS_REGISTER "," data_or_address_register ")"
	{
		const auto data_or_address_register = $5;

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.main_register = $3;
		$$.index_register = data_or_address_register % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	| "(" expression "," ADDRESS_REGISTER "," data_or_address_register ")"
	{
		const auto data_or_address_register = $6;

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $2;
		$$.main_register = $4;
		$$.index_register = data_or_address_register % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	| expression "(" PROGRAM_COUNTER ")"
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		$$.literal = $1;
	}
	| "(" expression "," PROGRAM_COUNTER ")"
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		$$.literal = $2;
	}
	| "(" PROGRAM_COUNTER "," expression ")"
	{
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		$$.literal = $4;
	}
	| "(" PROGRAM_COUNTER "," data_or_address_register size ")"
	{
		const auto data_or_address_register = $4;

		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.emplace<ExpressionNumber>(0);
		$$.index_register = data_or_address_register % 8;
		$$.size = $5;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	| expression "(" PROGRAM_COUNTER "," data_or_address_register size ")"
	{
		const auto data_or_address_register = $5;

		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.index_register = data_or_address_register % 8;
		$$.size = $6;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	| "(" expression "," PROGRAM_COUNTER "," data_or_address_register size ")"
	{
		const auto data_or_address_register = $6;

		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $2;
		$$.index_register = data_or_address_register % 8;
		$$.size = $7;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	| "(" PROGRAM_COUNTER "," data_or_address_register ")"
	{
		const auto data_or_address_register = $4;

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal.emplace<ExpressionNumber>(0);
		$$.index_register = data_or_address_register % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	| expression "(" PROGRAM_COUNTER "," data_or_address_register ")"
	{
		const auto data_or_address_register = $5;

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $1;
		$$.index_register = data_or_address_register % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	| "(" expression "," PROGRAM_COUNTER "," data_or_address_register ")"
	{
		const auto data_or_address_register = $6;

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		$$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		$$.literal = $2;
		$$.index_register = data_or_address_register % 8;
		$$.size = SIZE_WORD;
		$$.index_register_is_address_register = data_or_address_register / 8 != 0;
	}
	/* Literal */
	| "#" expression
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
		const auto register_list = $1;

		/* Neat trick to check if only one bit is set (yes, I know that it evaluates to true for 0, but that doesn't matter here). */
		if ((register_list & (register_list - 1)) == 0)
		{
			/* This is a single register. */
			$$.main_register = 0;

			while ((register_list & (1 << $$.main_register)) == 0)
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
			$$.main_register = register_list; /* Such a hack... */
		}
	}
	;

register_list
	: register_span
	{
		$$ = $1;
	}
	| register_span "/" register_list
	{
		$$ = $1 | $3;
	}
	;

register_span
	: data_or_address_register
	{
		$$ = 1 << $1;
	}
	| data_or_address_register "-" data_or_address_register
	{
		const auto first_data_or_address_register = $1;
		const auto second_data_or_address_register = $3;

		unsigned long start;
		unsigned long end;

		if (first_data_or_address_register < second_data_or_address_register)
		{
			start = first_data_or_address_register;
			end = second_data_or_address_register;
		}
		else
		{
			start = second_data_or_address_register;
			end = first_data_or_address_register;
		}

		$$ = 0;

		for (unsigned long i = start; i <= end; ++i)
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
	| "$" NUMBER
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
		DoExpression($$, ExpressionArithmetic2Operand::Type::LOGICAL_AND, $1, $3);
	}
	/* This is an assembler extension: asm68k doesn't support this. */
	| expression LOGICAL_OR expression1
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::LOGICAL_OR, $1, $3);
	}
	;

expression1
	: expression2
	{
		$$ = $1;
	}
	| expression1 "=" expression2
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::EQUALITY, $1, $3);
	}
	| expression1 EQUALITY expression2
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::EQUALITY, $1, $3);
	}
	| expression1 INEQUALITY expression2
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::INEQUALITY, $1, $3);
	}
	;

expression2
	: expression3
	{
		$$ = $1;
	}
	| expression2 "<" expression3
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::LESS_THAN, $1, $3);
	}
	| expression2 LESS_OR_EQUAL expression3
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::LESS_OR_EQUAL, $1, $3);
	}
	| expression2 ">" expression3
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::MORE_THAN, $1, $3);
	}
	| expression2 MORE_OR_EQUAL expression3
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::MORE_OR_EQUAL, $1, $3);
	}
	;

expression3
	: expression4
	{
		$$ = $1;
	}
	| expression3 "+" expression4
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::ADD, $1, $3);
	}
	| expression3 "-" expression4
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::SUBTRACT, $1, $3);
	}
	;

expression4
	: expression5
	{
		$$ = $1;
	}
	| expression4 "*" expression5
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::MULTIPLY, $1, $3);
	}
	| expression4 "/" expression5
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::DIVIDE, $1, $3);
	}
	| expression4 "%" expression5
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::MODULO, $1, $3);
	}
	;

expression5
	: expression6
	{
		$$ = $1;
	}
	| expression5 "&" expression6
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::BITWISE_AND, $1, $3);
	}
	| expression5 "!" expression6
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::BITWISE_OR, $1, $3);
	}
	| expression5 "|" expression6
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::BITWISE_OR, $1, $3);
	}
	| expression5 "^" expression6
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::BITWISE_XOR, $1, $3);
	}
	;

expression6
	: expression7
	{
		$$ = $1;
	}
	| expression6 LEFT_SHIFT expression7
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::LEFT_SHIFT, $1, $3);
	}
	| expression6 RIGHT_SHIFT expression7
	{
		DoExpression($$, ExpressionArithmetic2Operand::Type::RIGHT_SHIFT, $1, $3);
	}
	;

expression7
	: expression8
	{
		$$ = $1;
	}
	| "+" expression7
	{
		$$ = $2;
	}
	| "-" expression7
	{
		DoExpression($$, ExpressionArithmetic1Operand::Type::NEGATE, $2);
	}
	| "~" expression7
	{
		DoExpression($$, ExpressionArithmetic1Operand::Type::BITWISE_NOT, $2);
	}
	/* This is an assembler extension: asm68k doesn't support this. */
	| "!" expression7
	{
		DoExpression($$, ExpressionArithmetic1Operand::Type::LOGICAL_NOT, $2);
	}
	;

local_identifier
	: LOCAL_IDENTIFIER
	| SIZE_BYTE
	{
		$$ = ".b";
	}
	| SIZE_SHORT
	{
		$$ = ".s";
	}
	| SIZE_WORD
	{
		$$ = ".w";
	}
	| SIZE_LONGWORD
	{
		$$ = ".l";
	}
	;

identifier
	: IDENTIFIER
	| local_identifier
	| identifier local_identifier
	{
		$$ = $1 + $2;
	}
	;

expression8
	: NUMBER
	{
		$$.emplace<ExpressionNumber>($1);
	}
	| identifier %prec HIGHER_PRECEDENCE_THAN_SIZE
	{
		$$.emplace<ExpressionIdentifier>($1);
	}
	| string
	{
		$$ = $1;
	}
	| "*"
	{
		$$.emplace<ExpressionProgramCounterOfStatement>();
	}
	| "@"
	{
		$$.emplace<ExpressionProgramCounterOfExpression>();
	}
	| "(" expression ")"
	{
		$$ = $2;
	}
	| STRLEN "(" STRING ")"
	{
		$$.emplace<ExpressionStrlen>($3);
	}
	| STRCMP "(" STRING "," STRING ")"
	{
		$$.emplace<ExpressionStrcmp>($3, $5);
	}
	| INSTR "(" expression "," expression ")"
	{
		auto subexpressions = std::make_unique<Expression[]>(3);

		subexpressions[0] = ExpressionNumber{1};
		subexpressions[1] = $3;
		subexpressions[2] = $5;

		$$.emplace<ExpressionInstr>(std::move(subexpressions));
	}
	| INSTR "(" expression "," expression "," expression ")"
	{
		auto subexpressions = std::make_unique<Expression[]>(3);

		subexpressions[0] = $3;
		subexpressions[1] = $5;
		subexpressions[2] = $7;

		$$.emplace<ExpressionInstr>(std::move(subexpressions));
	}
	| DEF "(" IDENTIFIER ")"
	{
		$$.emplace<ExpressionDef>($3);
	}
	| TYPE "(" IDENTIFIER ")"
	{
		$$.emplace<ExpressionTypeWithIdentifier>($3);
	}
	| TYPE "(" NUMBER ")"
	{
		$$.emplace<ExpressionTypeWithNumber>($3);
	}
	| FILESIZE "(" STRING ")"
	{
		$$.emplace<ExpressionFilesize>($3);
	}
	| OFFSET "(" expression ")"
	{
		/* TODO: Make this actually do something besides pass the expression through. */
		$$ = $3;
	}
	;

string
	: STRING
	{
		$$.emplace<ExpressionString>($1);
	}
