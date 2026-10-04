// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


/**
 ** \file syntactic.h
 ** Define the m68kasm::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.

#ifndef YY_M68KASM_SYNTACTIC_H_INCLUDED
# define YY_M68KASM_SYNTACTIC_H_INCLUDED
// "%code requires" blocks.
#line 33 "syntactic.y"


#include "clowncommon/clowncommon.h"
#include "string.h"

/* Temporary junk! */
#define YYNOMEM YYERROR

#define YYSTYPE m68kasm::parser::semantic_type

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


#line 471 "syntactic.h"


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif



#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef M68KASM_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define M68KASM_DEBUG 1
#  else
#   define M68KASM_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define M68KASM_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined M68KASM_DEBUG */

#line 25 "syntactic.y"
namespace m68kasm {
#line 615 "syntactic.h"




  /// A Bison parser.
  class parser
  {
  public:
#ifdef M68KASM_STYPE
# ifdef __GNUC__
#  pragma GCC message "bison: do not #define M68KASM_STYPE in C++, use %define api.value.type"
# endif
    typedef M68KASM_STYPE value_type;
#else
    /// Symbol semantic values.
    union value_type
    {
#line 488 "syntactic.y"

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

#line 646 "syntactic.h"

    };
#endif
    /// Backward compatibility (Bison 3.8).
    typedef value_type semantic_type;


    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const std::string& m)
        : std::runtime_error (m)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;
    };

    /// Token kinds.
    struct token
    {
      enum token_kind_type
      {
        TOKEN_M68KASM_EMPTY = -2,
    TOKEN_YYEOF = 0,               // "end of file"
    TOKEN_M68KASM_error = 256,     // error
    TOKEN_M68KASM_UNDEF = 257,     // "invalid token"
    TOKEN_DIRECTIVE_ORI = 258,     // DIRECTIVE_ORI
    TOKEN_DIRECTIVE_ANDI = 259,    // DIRECTIVE_ANDI
    TOKEN_DIRECTIVE_SUBI = 260,    // DIRECTIVE_SUBI
    TOKEN_DIRECTIVE_ADDI = 261,    // DIRECTIVE_ADDI
    TOKEN_DIRECTIVE_EORI = 262,    // DIRECTIVE_EORI
    TOKEN_DIRECTIVE_CMPI = 263,    // DIRECTIVE_CMPI
    TOKEN_DIRECTIVE_BTST = 264,    // DIRECTIVE_BTST
    TOKEN_DIRECTIVE_BCHG = 265,    // DIRECTIVE_BCHG
    TOKEN_DIRECTIVE_BCLR = 266,    // DIRECTIVE_BCLR
    TOKEN_DIRECTIVE_BSET = 267,    // DIRECTIVE_BSET
    TOKEN_DIRECTIVE_MOVEP = 268,   // DIRECTIVE_MOVEP
    TOKEN_DIRECTIVE_MOVEA = 269,   // DIRECTIVE_MOVEA
    TOKEN_DIRECTIVE_MOVE = 270,    // DIRECTIVE_MOVE
    TOKEN_DIRECTIVE_NEGX = 271,    // DIRECTIVE_NEGX
    TOKEN_DIRECTIVE_CLR = 272,     // DIRECTIVE_CLR
    TOKEN_DIRECTIVE_NEG = 273,     // DIRECTIVE_NEG
    TOKEN_DIRECTIVE_NOT = 274,     // DIRECTIVE_NOT
    TOKEN_DIRECTIVE_EXT = 275,     // DIRECTIVE_EXT
    TOKEN_DIRECTIVE_NBCD = 276,    // DIRECTIVE_NBCD
    TOKEN_DIRECTIVE_SWAP = 277,    // DIRECTIVE_SWAP
    TOKEN_DIRECTIVE_PEA = 278,     // DIRECTIVE_PEA
    TOKEN_DIRECTIVE_ILLEGAL = 279, // DIRECTIVE_ILLEGAL
    TOKEN_DIRECTIVE_TAS = 280,     // DIRECTIVE_TAS
    TOKEN_DIRECTIVE_TST = 281,     // DIRECTIVE_TST
    TOKEN_DIRECTIVE_TRAP = 282,    // DIRECTIVE_TRAP
    TOKEN_DIRECTIVE_LINK = 283,    // DIRECTIVE_LINK
    TOKEN_DIRECTIVE_UNLK = 284,    // DIRECTIVE_UNLK
    TOKEN_DIRECTIVE_RESET = 285,   // DIRECTIVE_RESET
    TOKEN_DIRECTIVE_NOP = 286,     // DIRECTIVE_NOP
    TOKEN_DIRECTIVE_STOP = 287,    // DIRECTIVE_STOP
    TOKEN_DIRECTIVE_RTE = 288,     // DIRECTIVE_RTE
    TOKEN_DIRECTIVE_RTS = 289,     // DIRECTIVE_RTS
    TOKEN_DIRECTIVE_TRAPV = 290,   // DIRECTIVE_TRAPV
    TOKEN_DIRECTIVE_RTR = 291,     // DIRECTIVE_RTR
    TOKEN_DIRECTIVE_JSR = 292,     // DIRECTIVE_JSR
    TOKEN_DIRECTIVE_JMP = 293,     // DIRECTIVE_JMP
    TOKEN_DIRECTIVE_MOVEM = 294,   // DIRECTIVE_MOVEM
    TOKEN_DIRECTIVE_LEA = 295,     // DIRECTIVE_LEA
    TOKEN_DIRECTIVE_CHK = 296,     // DIRECTIVE_CHK
    TOKEN_DIRECTIVE_ADDQ = 297,    // DIRECTIVE_ADDQ
    TOKEN_DIRECTIVE_SUBQ = 298,    // DIRECTIVE_SUBQ
    TOKEN_DIRECTIVE_ST = 299,      // DIRECTIVE_ST
    TOKEN_DIRECTIVE_SF = 300,      // DIRECTIVE_SF
    TOKEN_DIRECTIVE_SHI = 301,     // DIRECTIVE_SHI
    TOKEN_DIRECTIVE_SLS = 302,     // DIRECTIVE_SLS
    TOKEN_DIRECTIVE_SCC = 303,     // DIRECTIVE_SCC
    TOKEN_DIRECTIVE_SCS = 304,     // DIRECTIVE_SCS
    TOKEN_DIRECTIVE_SNE = 305,     // DIRECTIVE_SNE
    TOKEN_DIRECTIVE_SEQ = 306,     // DIRECTIVE_SEQ
    TOKEN_DIRECTIVE_SVC = 307,     // DIRECTIVE_SVC
    TOKEN_DIRECTIVE_SVS = 308,     // DIRECTIVE_SVS
    TOKEN_DIRECTIVE_SPL = 309,     // DIRECTIVE_SPL
    TOKEN_DIRECTIVE_SMI = 310,     // DIRECTIVE_SMI
    TOKEN_DIRECTIVE_SGE = 311,     // DIRECTIVE_SGE
    TOKEN_DIRECTIVE_SLT = 312,     // DIRECTIVE_SLT
    TOKEN_DIRECTIVE_SGT = 313,     // DIRECTIVE_SGT
    TOKEN_DIRECTIVE_SLE = 314,     // DIRECTIVE_SLE
    TOKEN_DIRECTIVE_DBT = 315,     // DIRECTIVE_DBT
    TOKEN_DIRECTIVE_DBF = 316,     // DIRECTIVE_DBF
    TOKEN_DIRECTIVE_DBHI = 317,    // DIRECTIVE_DBHI
    TOKEN_DIRECTIVE_DBLS = 318,    // DIRECTIVE_DBLS
    TOKEN_DIRECTIVE_DBCC = 319,    // DIRECTIVE_DBCC
    TOKEN_DIRECTIVE_DBCS = 320,    // DIRECTIVE_DBCS
    TOKEN_DIRECTIVE_DBNE = 321,    // DIRECTIVE_DBNE
    TOKEN_DIRECTIVE_DBEQ = 322,    // DIRECTIVE_DBEQ
    TOKEN_DIRECTIVE_DBVC = 323,    // DIRECTIVE_DBVC
    TOKEN_DIRECTIVE_DBVS = 324,    // DIRECTIVE_DBVS
    TOKEN_DIRECTIVE_DBPL = 325,    // DIRECTIVE_DBPL
    TOKEN_DIRECTIVE_DBMI = 326,    // DIRECTIVE_DBMI
    TOKEN_DIRECTIVE_DBGE = 327,    // DIRECTIVE_DBGE
    TOKEN_DIRECTIVE_DBLT = 328,    // DIRECTIVE_DBLT
    TOKEN_DIRECTIVE_DBGT = 329,    // DIRECTIVE_DBGT
    TOKEN_DIRECTIVE_DBLE = 330,    // DIRECTIVE_DBLE
    TOKEN_DIRECTIVE_BRA = 331,     // DIRECTIVE_BRA
    TOKEN_DIRECTIVE_BSR = 332,     // DIRECTIVE_BSR
    TOKEN_DIRECTIVE_BHI = 333,     // DIRECTIVE_BHI
    TOKEN_DIRECTIVE_BLS = 334,     // DIRECTIVE_BLS
    TOKEN_DIRECTIVE_BCC = 335,     // DIRECTIVE_BCC
    TOKEN_DIRECTIVE_BCS = 336,     // DIRECTIVE_BCS
    TOKEN_DIRECTIVE_BNE = 337,     // DIRECTIVE_BNE
    TOKEN_DIRECTIVE_BEQ = 338,     // DIRECTIVE_BEQ
    TOKEN_DIRECTIVE_BVC = 339,     // DIRECTIVE_BVC
    TOKEN_DIRECTIVE_BVS = 340,     // DIRECTIVE_BVS
    TOKEN_DIRECTIVE_BPL = 341,     // DIRECTIVE_BPL
    TOKEN_DIRECTIVE_BMI = 342,     // DIRECTIVE_BMI
    TOKEN_DIRECTIVE_BGE = 343,     // DIRECTIVE_BGE
    TOKEN_DIRECTIVE_BLT = 344,     // DIRECTIVE_BLT
    TOKEN_DIRECTIVE_BGT = 345,     // DIRECTIVE_BGT
    TOKEN_DIRECTIVE_BLE = 346,     // DIRECTIVE_BLE
    TOKEN_DIRECTIVE_MOVEQ = 347,   // DIRECTIVE_MOVEQ
    TOKEN_DIRECTIVE_DIVU = 348,    // DIRECTIVE_DIVU
    TOKEN_DIRECTIVE_DIVS = 349,    // DIRECTIVE_DIVS
    TOKEN_DIRECTIVE_SBCD = 350,    // DIRECTIVE_SBCD
    TOKEN_DIRECTIVE_OR = 351,      // DIRECTIVE_OR
    TOKEN_DIRECTIVE_SUB = 352,     // DIRECTIVE_SUB
    TOKEN_DIRECTIVE_SUBX = 353,    // DIRECTIVE_SUBX
    TOKEN_DIRECTIVE_SUBA = 354,    // DIRECTIVE_SUBA
    TOKEN_DIRECTIVE_EOR = 355,     // DIRECTIVE_EOR
    TOKEN_DIRECTIVE_CMPM = 356,    // DIRECTIVE_CMPM
    TOKEN_DIRECTIVE_CMP = 357,     // DIRECTIVE_CMP
    TOKEN_DIRECTIVE_CMPA = 358,    // DIRECTIVE_CMPA
    TOKEN_DIRECTIVE_MULU = 359,    // DIRECTIVE_MULU
    TOKEN_DIRECTIVE_MULS = 360,    // DIRECTIVE_MULS
    TOKEN_DIRECTIVE_ABCD = 361,    // DIRECTIVE_ABCD
    TOKEN_DIRECTIVE_EXG = 362,     // DIRECTIVE_EXG
    TOKEN_DIRECTIVE_AND = 363,     // DIRECTIVE_AND
    TOKEN_DIRECTIVE_ADD = 364,     // DIRECTIVE_ADD
    TOKEN_DIRECTIVE_ADDX = 365,    // DIRECTIVE_ADDX
    TOKEN_DIRECTIVE_ADDA = 366,    // DIRECTIVE_ADDA
    TOKEN_DIRECTIVE_ASL = 367,     // DIRECTIVE_ASL
    TOKEN_DIRECTIVE_ASR = 368,     // DIRECTIVE_ASR
    TOKEN_DIRECTIVE_LSL = 369,     // DIRECTIVE_LSL
    TOKEN_DIRECTIVE_LSR = 370,     // DIRECTIVE_LSR
    TOKEN_DIRECTIVE_ROXL = 371,    // DIRECTIVE_ROXL
    TOKEN_DIRECTIVE_ROXR = 372,    // DIRECTIVE_ROXR
    TOKEN_DIRECTIVE_ROL = 373,     // DIRECTIVE_ROL
    TOKEN_DIRECTIVE_ROR = 374,     // DIRECTIVE_ROR
    TOKEN_DIRECTIVE_DC = 375,      // DIRECTIVE_DC
    TOKEN_DIRECTIVE_DCB = 376,     // DIRECTIVE_DCB
    TOKEN_DIRECTIVE_DS = 377,      // DIRECTIVE_DS
    TOKEN_DIRECTIVE_REPT = 378,    // DIRECTIVE_REPT
    TOKEN_DIRECTIVE_ENDR = 379,    // DIRECTIVE_ENDR
    TOKEN_DIRECTIVE_MACRO = 380,   // DIRECTIVE_MACRO
    TOKEN_DIRECTIVE_MACROS = 381,  // DIRECTIVE_MACROS
    TOKEN_DIRECTIVE_ENDM = 382,    // DIRECTIVE_ENDM
    TOKEN_DIRECTIVE_INCLUDE = 383, // DIRECTIVE_INCLUDE
    TOKEN_DIRECTIVE_INCBIN = 384,  // DIRECTIVE_INCBIN
    TOKEN_DIRECTIVE_EQU = 385,     // DIRECTIVE_EQU
    TOKEN_DIRECTIVE_EQUS = 386,    // DIRECTIVE_EQUS
    TOKEN_DIRECTIVE_SUBSTR = 387,  // DIRECTIVE_SUBSTR
    TOKEN_DIRECTIVE_SET = 388,     // DIRECTIVE_SET
    TOKEN_DIRECTIVE_IF = 389,      // DIRECTIVE_IF
    TOKEN_DIRECTIVE_ELSEIF = 390,  // DIRECTIVE_ELSEIF
    TOKEN_DIRECTIVE_ELSE = 391,    // DIRECTIVE_ELSE
    TOKEN_DIRECTIVE_ENDC = 392,    // DIRECTIVE_ENDC
    TOKEN_DIRECTIVE_WHILE = 393,   // DIRECTIVE_WHILE
    TOKEN_DIRECTIVE_ENDW = 394,    // DIRECTIVE_ENDW
    TOKEN_DIRECTIVE_EVEN = 395,    // DIRECTIVE_EVEN
    TOKEN_DIRECTIVE_CNOP = 396,    // DIRECTIVE_CNOP
    TOKEN_DIRECTIVE_INFORM = 397,  // DIRECTIVE_INFORM
    TOKEN_DIRECTIVE_FAIL = 398,    // DIRECTIVE_FAIL
    TOKEN_DIRECTIVE_END = 399,     // DIRECTIVE_END
    TOKEN_DIRECTIVE_RS = 400,      // DIRECTIVE_RS
    TOKEN_DIRECTIVE_RSSET = 401,   // DIRECTIVE_RSSET
    TOKEN_DIRECTIVE_RSRESET = 402, // DIRECTIVE_RSRESET
    TOKEN_DIRECTIVE_OBJ = 403,     // DIRECTIVE_OBJ
    TOKEN_DIRECTIVE_OBJEND = 404,  // DIRECTIVE_OBJEND
    TOKEN_DIRECTIVE_ORG = 405,     // DIRECTIVE_ORG
    TOKEN_DIRECTIVE_PUSHO = 406,   // DIRECTIVE_PUSHO
    TOKEN_DIRECTIVE_POPO = 407,    // DIRECTIVE_POPO
    TOKEN_DIRECTIVE_OPT = 408,     // DIRECTIVE_OPT
    TOKEN_DIRECTIVE_PUSHP = 409,   // DIRECTIVE_PUSHP
    TOKEN_DIRECTIVE_POPP = 410,    // DIRECTIVE_POPP
    TOKEN_DIRECTIVE_SHIFT = 411,   // DIRECTIVE_SHIFT
    TOKEN_DIRECTIVE_MEXIT = 412,   // DIRECTIVE_MEXIT
    TOKEN_DIRECTIVE_LOCAL = 413,   // DIRECTIVE_LOCAL
    TOKEN_DIRECTIVE_LIST = 414,    // DIRECTIVE_LIST
    TOKEN_DIRECTIVE_NOLIST = 415,  // DIRECTIVE_NOLIST
    TOKEN_DIRECTIVE_PURGE = 416,   // DIRECTIVE_PURGE
    TOKEN_SIZE_BYTE = 417,         // SIZE_BYTE
    TOKEN_SIZE_SHORT = 418,        // SIZE_SHORT
    TOKEN_SIZE_WORD = 419,         // SIZE_WORD
    TOKEN_SIZE_LONGWORD = 420,     // SIZE_LONGWORD
    TOKEN_DATA_REGISTER = 421,     // DATA_REGISTER
    TOKEN_ADDRESS_REGISTER = 422,  // ADDRESS_REGISTER
    TOKEN_NUMBER = 423,            // NUMBER
    TOKEN_IDENTIFIER = 424,        // IDENTIFIER
    TOKEN_LOCAL_IDENTIFIER = 425,  // LOCAL_IDENTIFIER
    TOKEN_STRING = 426,            // STRING
    TOKEN_OPTION = 427,            // OPTION
    TOKEN_STATUS_REGISTER = 428,   // STATUS_REGISTER
    TOKEN_CONDITION_CODE_REGISTER = 429, // CONDITION_CODE_REGISTER
    TOKEN_USER_STACK_POINTER_REGISTER = 430, // USER_STACK_POINTER_REGISTER
    TOKEN_PROGRAM_COUNTER = 431,   // PROGRAM_COUNTER
    TOKEN_LOGICAL_AND = 432,       // LOGICAL_AND
    TOKEN_LOGICAL_OR = 433,        // LOGICAL_OR
    TOKEN_EQUALITY = 434,          // EQUALITY
    TOKEN_INEQUALITY = 435,        // INEQUALITY
    TOKEN_LESS_OR_EQUAL = 436,     // LESS_OR_EQUAL
    TOKEN_MORE_OR_EQUAL = 437,     // MORE_OR_EQUAL
    TOKEN_LEFT_SHIFT = 438,        // LEFT_SHIFT
    TOKEN_RIGHT_SHIFT = 439,       // RIGHT_SHIFT
    TOKEN_STRLEN = 440,            // STRLEN
    TOKEN_STRCMP = 441,            // STRCMP
    TOKEN_INSTR = 442,             // INSTR
    TOKEN_DEF = 443,               // DEF
    TOKEN_TYPE = 444,              // TYPE
    TOKEN_FILESIZE = 445,          // FILESIZE
    TOKEN_OFFSET = 446,            // OFFSET
    TOKEN_LOWER_PRECEDENCE_THAN_SIZE = 447, // LOWER_PRECEDENCE_THAN_SIZE
    TOKEN_HIGHER_PRECEDENCE_THAN_SIZE = 448 // HIGHER_PRECEDENCE_THAN_SIZE
      };
      /// Backward compatibility alias (Bison 3.6).
      typedef token_kind_type yytokentype;
    };

    /// Token kind, as returned by yylex.
    typedef token::token_kind_type token_kind_type;

    /// Backward compatibility alias (Bison 3.6).
    typedef token_kind_type token_type;

    /// Symbol kinds.
    struct symbol_kind
    {
      enum symbol_kind_type
      {
        YYNTOKENS = 213, ///< Number of tokens.
        S_YYEMPTY = -2,
        S_YYEOF = 0,                             // "end of file"
        S_YYerror = 1,                           // error
        S_YYUNDEF = 2,                           // "invalid token"
        S_DIRECTIVE_ORI = 3,                     // DIRECTIVE_ORI
        S_DIRECTIVE_ANDI = 4,                    // DIRECTIVE_ANDI
        S_DIRECTIVE_SUBI = 5,                    // DIRECTIVE_SUBI
        S_DIRECTIVE_ADDI = 6,                    // DIRECTIVE_ADDI
        S_DIRECTIVE_EORI = 7,                    // DIRECTIVE_EORI
        S_DIRECTIVE_CMPI = 8,                    // DIRECTIVE_CMPI
        S_DIRECTIVE_BTST = 9,                    // DIRECTIVE_BTST
        S_DIRECTIVE_BCHG = 10,                   // DIRECTIVE_BCHG
        S_DIRECTIVE_BCLR = 11,                   // DIRECTIVE_BCLR
        S_DIRECTIVE_BSET = 12,                   // DIRECTIVE_BSET
        S_DIRECTIVE_MOVEP = 13,                  // DIRECTIVE_MOVEP
        S_DIRECTIVE_MOVEA = 14,                  // DIRECTIVE_MOVEA
        S_DIRECTIVE_MOVE = 15,                   // DIRECTIVE_MOVE
        S_DIRECTIVE_NEGX = 16,                   // DIRECTIVE_NEGX
        S_DIRECTIVE_CLR = 17,                    // DIRECTIVE_CLR
        S_DIRECTIVE_NEG = 18,                    // DIRECTIVE_NEG
        S_DIRECTIVE_NOT = 19,                    // DIRECTIVE_NOT
        S_DIRECTIVE_EXT = 20,                    // DIRECTIVE_EXT
        S_DIRECTIVE_NBCD = 21,                   // DIRECTIVE_NBCD
        S_DIRECTIVE_SWAP = 22,                   // DIRECTIVE_SWAP
        S_DIRECTIVE_PEA = 23,                    // DIRECTIVE_PEA
        S_DIRECTIVE_ILLEGAL = 24,                // DIRECTIVE_ILLEGAL
        S_DIRECTIVE_TAS = 25,                    // DIRECTIVE_TAS
        S_DIRECTIVE_TST = 26,                    // DIRECTIVE_TST
        S_DIRECTIVE_TRAP = 27,                   // DIRECTIVE_TRAP
        S_DIRECTIVE_LINK = 28,                   // DIRECTIVE_LINK
        S_DIRECTIVE_UNLK = 29,                   // DIRECTIVE_UNLK
        S_DIRECTIVE_RESET = 30,                  // DIRECTIVE_RESET
        S_DIRECTIVE_NOP = 31,                    // DIRECTIVE_NOP
        S_DIRECTIVE_STOP = 32,                   // DIRECTIVE_STOP
        S_DIRECTIVE_RTE = 33,                    // DIRECTIVE_RTE
        S_DIRECTIVE_RTS = 34,                    // DIRECTIVE_RTS
        S_DIRECTIVE_TRAPV = 35,                  // DIRECTIVE_TRAPV
        S_DIRECTIVE_RTR = 36,                    // DIRECTIVE_RTR
        S_DIRECTIVE_JSR = 37,                    // DIRECTIVE_JSR
        S_DIRECTIVE_JMP = 38,                    // DIRECTIVE_JMP
        S_DIRECTIVE_MOVEM = 39,                  // DIRECTIVE_MOVEM
        S_DIRECTIVE_LEA = 40,                    // DIRECTIVE_LEA
        S_DIRECTIVE_CHK = 41,                    // DIRECTIVE_CHK
        S_DIRECTIVE_ADDQ = 42,                   // DIRECTIVE_ADDQ
        S_DIRECTIVE_SUBQ = 43,                   // DIRECTIVE_SUBQ
        S_DIRECTIVE_ST = 44,                     // DIRECTIVE_ST
        S_DIRECTIVE_SF = 45,                     // DIRECTIVE_SF
        S_DIRECTIVE_SHI = 46,                    // DIRECTIVE_SHI
        S_DIRECTIVE_SLS = 47,                    // DIRECTIVE_SLS
        S_DIRECTIVE_SCC = 48,                    // DIRECTIVE_SCC
        S_DIRECTIVE_SCS = 49,                    // DIRECTIVE_SCS
        S_DIRECTIVE_SNE = 50,                    // DIRECTIVE_SNE
        S_DIRECTIVE_SEQ = 51,                    // DIRECTIVE_SEQ
        S_DIRECTIVE_SVC = 52,                    // DIRECTIVE_SVC
        S_DIRECTIVE_SVS = 53,                    // DIRECTIVE_SVS
        S_DIRECTIVE_SPL = 54,                    // DIRECTIVE_SPL
        S_DIRECTIVE_SMI = 55,                    // DIRECTIVE_SMI
        S_DIRECTIVE_SGE = 56,                    // DIRECTIVE_SGE
        S_DIRECTIVE_SLT = 57,                    // DIRECTIVE_SLT
        S_DIRECTIVE_SGT = 58,                    // DIRECTIVE_SGT
        S_DIRECTIVE_SLE = 59,                    // DIRECTIVE_SLE
        S_DIRECTIVE_DBT = 60,                    // DIRECTIVE_DBT
        S_DIRECTIVE_DBF = 61,                    // DIRECTIVE_DBF
        S_DIRECTIVE_DBHI = 62,                   // DIRECTIVE_DBHI
        S_DIRECTIVE_DBLS = 63,                   // DIRECTIVE_DBLS
        S_DIRECTIVE_DBCC = 64,                   // DIRECTIVE_DBCC
        S_DIRECTIVE_DBCS = 65,                   // DIRECTIVE_DBCS
        S_DIRECTIVE_DBNE = 66,                   // DIRECTIVE_DBNE
        S_DIRECTIVE_DBEQ = 67,                   // DIRECTIVE_DBEQ
        S_DIRECTIVE_DBVC = 68,                   // DIRECTIVE_DBVC
        S_DIRECTIVE_DBVS = 69,                   // DIRECTIVE_DBVS
        S_DIRECTIVE_DBPL = 70,                   // DIRECTIVE_DBPL
        S_DIRECTIVE_DBMI = 71,                   // DIRECTIVE_DBMI
        S_DIRECTIVE_DBGE = 72,                   // DIRECTIVE_DBGE
        S_DIRECTIVE_DBLT = 73,                   // DIRECTIVE_DBLT
        S_DIRECTIVE_DBGT = 74,                   // DIRECTIVE_DBGT
        S_DIRECTIVE_DBLE = 75,                   // DIRECTIVE_DBLE
        S_DIRECTIVE_BRA = 76,                    // DIRECTIVE_BRA
        S_DIRECTIVE_BSR = 77,                    // DIRECTIVE_BSR
        S_DIRECTIVE_BHI = 78,                    // DIRECTIVE_BHI
        S_DIRECTIVE_BLS = 79,                    // DIRECTIVE_BLS
        S_DIRECTIVE_BCC = 80,                    // DIRECTIVE_BCC
        S_DIRECTIVE_BCS = 81,                    // DIRECTIVE_BCS
        S_DIRECTIVE_BNE = 82,                    // DIRECTIVE_BNE
        S_DIRECTIVE_BEQ = 83,                    // DIRECTIVE_BEQ
        S_DIRECTIVE_BVC = 84,                    // DIRECTIVE_BVC
        S_DIRECTIVE_BVS = 85,                    // DIRECTIVE_BVS
        S_DIRECTIVE_BPL = 86,                    // DIRECTIVE_BPL
        S_DIRECTIVE_BMI = 87,                    // DIRECTIVE_BMI
        S_DIRECTIVE_BGE = 88,                    // DIRECTIVE_BGE
        S_DIRECTIVE_BLT = 89,                    // DIRECTIVE_BLT
        S_DIRECTIVE_BGT = 90,                    // DIRECTIVE_BGT
        S_DIRECTIVE_BLE = 91,                    // DIRECTIVE_BLE
        S_DIRECTIVE_MOVEQ = 92,                  // DIRECTIVE_MOVEQ
        S_DIRECTIVE_DIVU = 93,                   // DIRECTIVE_DIVU
        S_DIRECTIVE_DIVS = 94,                   // DIRECTIVE_DIVS
        S_DIRECTIVE_SBCD = 95,                   // DIRECTIVE_SBCD
        S_DIRECTIVE_OR = 96,                     // DIRECTIVE_OR
        S_DIRECTIVE_SUB = 97,                    // DIRECTIVE_SUB
        S_DIRECTIVE_SUBX = 98,                   // DIRECTIVE_SUBX
        S_DIRECTIVE_SUBA = 99,                   // DIRECTIVE_SUBA
        S_DIRECTIVE_EOR = 100,                   // DIRECTIVE_EOR
        S_DIRECTIVE_CMPM = 101,                  // DIRECTIVE_CMPM
        S_DIRECTIVE_CMP = 102,                   // DIRECTIVE_CMP
        S_DIRECTIVE_CMPA = 103,                  // DIRECTIVE_CMPA
        S_DIRECTIVE_MULU = 104,                  // DIRECTIVE_MULU
        S_DIRECTIVE_MULS = 105,                  // DIRECTIVE_MULS
        S_DIRECTIVE_ABCD = 106,                  // DIRECTIVE_ABCD
        S_DIRECTIVE_EXG = 107,                   // DIRECTIVE_EXG
        S_DIRECTIVE_AND = 108,                   // DIRECTIVE_AND
        S_DIRECTIVE_ADD = 109,                   // DIRECTIVE_ADD
        S_DIRECTIVE_ADDX = 110,                  // DIRECTIVE_ADDX
        S_DIRECTIVE_ADDA = 111,                  // DIRECTIVE_ADDA
        S_DIRECTIVE_ASL = 112,                   // DIRECTIVE_ASL
        S_DIRECTIVE_ASR = 113,                   // DIRECTIVE_ASR
        S_DIRECTIVE_LSL = 114,                   // DIRECTIVE_LSL
        S_DIRECTIVE_LSR = 115,                   // DIRECTIVE_LSR
        S_DIRECTIVE_ROXL = 116,                  // DIRECTIVE_ROXL
        S_DIRECTIVE_ROXR = 117,                  // DIRECTIVE_ROXR
        S_DIRECTIVE_ROL = 118,                   // DIRECTIVE_ROL
        S_DIRECTIVE_ROR = 119,                   // DIRECTIVE_ROR
        S_DIRECTIVE_DC = 120,                    // DIRECTIVE_DC
        S_DIRECTIVE_DCB = 121,                   // DIRECTIVE_DCB
        S_DIRECTIVE_DS = 122,                    // DIRECTIVE_DS
        S_DIRECTIVE_REPT = 123,                  // DIRECTIVE_REPT
        S_DIRECTIVE_ENDR = 124,                  // DIRECTIVE_ENDR
        S_DIRECTIVE_MACRO = 125,                 // DIRECTIVE_MACRO
        S_DIRECTIVE_MACROS = 126,                // DIRECTIVE_MACROS
        S_DIRECTIVE_ENDM = 127,                  // DIRECTIVE_ENDM
        S_DIRECTIVE_INCLUDE = 128,               // DIRECTIVE_INCLUDE
        S_DIRECTIVE_INCBIN = 129,                // DIRECTIVE_INCBIN
        S_DIRECTIVE_EQU = 130,                   // DIRECTIVE_EQU
        S_DIRECTIVE_EQUS = 131,                  // DIRECTIVE_EQUS
        S_DIRECTIVE_SUBSTR = 132,                // DIRECTIVE_SUBSTR
        S_DIRECTIVE_SET = 133,                   // DIRECTIVE_SET
        S_DIRECTIVE_IF = 134,                    // DIRECTIVE_IF
        S_DIRECTIVE_ELSEIF = 135,                // DIRECTIVE_ELSEIF
        S_DIRECTIVE_ELSE = 136,                  // DIRECTIVE_ELSE
        S_DIRECTIVE_ENDC = 137,                  // DIRECTIVE_ENDC
        S_DIRECTIVE_WHILE = 138,                 // DIRECTIVE_WHILE
        S_DIRECTIVE_ENDW = 139,                  // DIRECTIVE_ENDW
        S_DIRECTIVE_EVEN = 140,                  // DIRECTIVE_EVEN
        S_DIRECTIVE_CNOP = 141,                  // DIRECTIVE_CNOP
        S_DIRECTIVE_INFORM = 142,                // DIRECTIVE_INFORM
        S_DIRECTIVE_FAIL = 143,                  // DIRECTIVE_FAIL
        S_DIRECTIVE_END = 144,                   // DIRECTIVE_END
        S_DIRECTIVE_RS = 145,                    // DIRECTIVE_RS
        S_DIRECTIVE_RSSET = 146,                 // DIRECTIVE_RSSET
        S_DIRECTIVE_RSRESET = 147,               // DIRECTIVE_RSRESET
        S_DIRECTIVE_OBJ = 148,                   // DIRECTIVE_OBJ
        S_DIRECTIVE_OBJEND = 149,                // DIRECTIVE_OBJEND
        S_DIRECTIVE_ORG = 150,                   // DIRECTIVE_ORG
        S_DIRECTIVE_PUSHO = 151,                 // DIRECTIVE_PUSHO
        S_DIRECTIVE_POPO = 152,                  // DIRECTIVE_POPO
        S_DIRECTIVE_OPT = 153,                   // DIRECTIVE_OPT
        S_DIRECTIVE_PUSHP = 154,                 // DIRECTIVE_PUSHP
        S_DIRECTIVE_POPP = 155,                  // DIRECTIVE_POPP
        S_DIRECTIVE_SHIFT = 156,                 // DIRECTIVE_SHIFT
        S_DIRECTIVE_MEXIT = 157,                 // DIRECTIVE_MEXIT
        S_DIRECTIVE_LOCAL = 158,                 // DIRECTIVE_LOCAL
        S_DIRECTIVE_LIST = 159,                  // DIRECTIVE_LIST
        S_DIRECTIVE_NOLIST = 160,                // DIRECTIVE_NOLIST
        S_DIRECTIVE_PURGE = 161,                 // DIRECTIVE_PURGE
        S_SIZE_BYTE = 162,                       // SIZE_BYTE
        S_SIZE_SHORT = 163,                      // SIZE_SHORT
        S_SIZE_WORD = 164,                       // SIZE_WORD
        S_SIZE_LONGWORD = 165,                   // SIZE_LONGWORD
        S_DATA_REGISTER = 166,                   // DATA_REGISTER
        S_ADDRESS_REGISTER = 167,                // ADDRESS_REGISTER
        S_NUMBER = 168,                          // NUMBER
        S_IDENTIFIER = 169,                      // IDENTIFIER
        S_LOCAL_IDENTIFIER = 170,                // LOCAL_IDENTIFIER
        S_STRING = 171,                          // STRING
        S_OPTION = 172,                          // OPTION
        S_STATUS_REGISTER = 173,                 // STATUS_REGISTER
        S_CONDITION_CODE_REGISTER = 174,         // CONDITION_CODE_REGISTER
        S_USER_STACK_POINTER_REGISTER = 175,     // USER_STACK_POINTER_REGISTER
        S_PROGRAM_COUNTER = 176,                 // PROGRAM_COUNTER
        S_LOGICAL_AND = 177,                     // LOGICAL_AND
        S_LOGICAL_OR = 178,                      // LOGICAL_OR
        S_EQUALITY = 179,                        // EQUALITY
        S_INEQUALITY = 180,                      // INEQUALITY
        S_LESS_OR_EQUAL = 181,                   // LESS_OR_EQUAL
        S_MORE_OR_EQUAL = 182,                   // MORE_OR_EQUAL
        S_LEFT_SHIFT = 183,                      // LEFT_SHIFT
        S_RIGHT_SHIFT = 184,                     // RIGHT_SHIFT
        S_STRLEN = 185,                          // STRLEN
        S_STRCMP = 186,                          // STRCMP
        S_INSTR = 187,                           // INSTR
        S_DEF = 188,                             // DEF
        S_TYPE = 189,                            // TYPE
        S_FILESIZE = 190,                        // FILESIZE
        S_OFFSET = 191,                          // OFFSET
        S_LOWER_PRECEDENCE_THAN_SIZE = 192,      // LOWER_PRECEDENCE_THAN_SIZE
        S_HIGHER_PRECEDENCE_THAN_SIZE = 193,     // HIGHER_PRECEDENCE_THAN_SIZE
        S_194_ = 194,                            // ','
        S_195_ = 195,                            // '*'
        S_196_ = 196,                            // '='
        S_197_ = 197,                            // '+'
        S_198_ = 198,                            // '-'
        S_199_ = 199,                            // '.'
        S_200_ = 200,                            // '('
        S_201_ = 201,                            // ')'
        S_202_ = 202,                            // '#'
        S_203_ = 203,                            // '/'
        S_204_ = 204,                            // '<'
        S_205_ = 205,                            // '>'
        S_206_ = 206,                            // '%'
        S_207_ = 207,                            // '&'
        S_208_ = 208,                            // '!'
        S_209_ = 209,                            // '|'
        S_210_ = 210,                            // '^'
        S_211_ = 211,                            // '~'
        S_212_ = 212,                            // '@'
        S_YYACCEPT = 213,                        // $accept
        S_statement = 214,                       // statement
        S_expression_list = 215,                 // expression_list
        S_identifier_list = 216,                 // identifier_list
        S_option_list = 217,                     // option_list
        S_instruction = 218,                     // instruction
        S_full_opcode = 219,                     // full_opcode
        S_opcode = 220,                          // opcode
        S_size = 221,                            // size
        S_operand = 222,                         // operand
        S_register_list = 223,                   // register_list
        S_register_span = 224,                   // register_span
        S_data_or_address_register = 225,        // data_or_address_register
        S_expression = 226,                      // expression
        S_expression1 = 227,                     // expression1
        S_expression2 = 228,                     // expression2
        S_expression3 = 229,                     // expression3
        S_expression4 = 230,                     // expression4
        S_expression5 = 231,                     // expression5
        S_expression6 = 232,                     // expression6
        S_expression7 = 233,                     // expression7
        S_local_identifier = 234,                // local_identifier
        S_identifier = 235,                      // identifier
        S_expression8 = 236,                     // expression8
        S_string = 237                           // string
      };
    };

    /// (Internal) symbol kind.
    typedef symbol_kind::symbol_kind_type symbol_kind_type;

    /// The number of tokens.
    static const symbol_kind_type YYNTOKENS = symbol_kind::YYNTOKENS;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol kind
    /// via kind ().
    ///
    /// Provide access to semantic value.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol () YY_NOEXCEPT
        : value ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that)
        : Base (std::move (that))
        , value (std::move (that.value))
      {}
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);
      /// Constructor for valueless symbols.
      basic_symbol (typename Base::kind_type t);

      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    YY_RVREF (value_type) v);

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        Base::clear ();
      }

      /// The user-facing name of this symbol.
      std::string name () const YY_NOEXCEPT
      {
        return parser::symbol_name (this->kind ());
      }

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      value_type value;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_kind
    {
      /// The symbol kind as needed by the constructor.
      typedef token_kind_type kind_type;

      /// Default constructor.
      by_kind () YY_NOEXCEPT;

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_kind (by_kind&& that) YY_NOEXCEPT;
#endif

      /// Copy constructor.
      by_kind (const by_kind& that) YY_NOEXCEPT;

      /// Constructor from (external) token numbers.
      by_kind (kind_type t) YY_NOEXCEPT;



      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_kind& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// Backward compatibility (Bison 3.6).
      symbol_kind_type type_get () const YY_NOEXCEPT;

      /// The symbol kind.
      /// \a S_YYEMPTY when empty.
      symbol_kind_type kind_;
    };

    /// Backward compatibility for a private implementation detail (Bison 3.6).
    typedef by_kind by_type;

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_kind>
    {};

    /// Build a parser object.
    parser (void *scanner_yyarg, Statement *statement_yyarg);
    virtual ~parser ();

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    parser (const parser&) = delete;
    /// Non copyable.
    parser& operator= (const parser&) = delete;
#endif

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if M68KASM_DEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param msg    a description of the syntax error.
    virtual void error (const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    /// The user-facing name of the symbol whose (internal) number is
    /// YYSYMBOL.  No bounds checking.
    static std::string symbol_name (symbol_kind_type yysymbol);



    class context
    {
    public:
      context (const parser& yyparser, const symbol_type& yyla);
      const symbol_type& lookahead () const YY_NOEXCEPT { return yyla_; }
      symbol_kind_type token () const YY_NOEXCEPT { return yyla_.kind (); }
      /// Put in YYARG at most YYARGN of the expected tokens, and return the
      /// number of tokens stored in YYARG.  If YYARG is null, return the
      /// number of expected tokens (guaranteed to be less than YYNTOKENS).
      int expected_tokens (symbol_kind_type yyarg[], int yyargn) const;

    private:
      const parser& yyparser_;
      const symbol_type& yyla_;
    };

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    parser (const parser&);
    /// Non copyable.
    parser& operator= (const parser&);
#endif


    /// Stored state numbers (used for stacks).
    typedef short state_type;

    /// The arguments of the error message.
    int yy_syntax_error_arguments_ (const context& yyctx,
                                    symbol_kind_type yyarg[], int yyargn) const;

    /// Generate an error message.
    /// \param yyctx     the context in which the error occurred.
    virtual std::string yysyntax_error_ (const context& yyctx) const;
    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT;

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT;

    static const short yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token kind \a t to a symbol kind.
    /// In theory \a t should be a token_kind_type, but character literals
    /// are valid, yet not members of the token_kind_type enum.
    static symbol_kind_type yytranslate_ (int t) YY_NOEXCEPT;

    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *yystr);

    /// For a symbol, its name in clear.
    static const char* const yytname_[];


    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const short yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const short yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const unsigned char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const short yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
    // state STATE-NUM.
    static const unsigned char yystos_[];

    // YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.
    static const unsigned char yyr1_[];

    // YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.
    static const signed char yyr2_[];


#if M68KASM_DEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r) const;
    /// Print the state stack on the debug stream.
    virtual void yy_stack_print_ () const;

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol kind, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol kind as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol kind from \a that.
      void move (by_state& that);

      /// The symbol kind (corresponding to \a state).
      /// \a symbol_kind::S_YYEMPTY when empty.
      symbol_kind_type kind () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::iterator iterator;
      typedef typename S::const_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200) YY_NOEXCEPT
        : seq_ (n)
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Non copyable.
      stack (const stack&) = delete;
      /// Non copyable.
      stack& operator= (const stack&) = delete;
#endif

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.begin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.end ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range) YY_NOEXCEPT
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
#if YY_CPLUSPLUS < 201103L
      /// Non copyable.
      stack (const stack&);
      /// Non copyable.
      stack& operator= (const stack&);
#endif
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1) YY_NOEXCEPT;

    /// Constants.
    enum
    {
      yylast_ = 863,     ///< Last index in yytable_.
      yynnts_ = 25,  ///< Number of nonterminal symbols.
      yyfinal_ = 237 ///< Termination state number.
    };


    // User arguments.
    void *scanner;
    Statement *statement;

  };


#line 25 "syntactic.y"
} // m68kasm
#line 1640 "syntactic.h"


// "%code provides" blocks.
#line 455 "syntactic.y"


void DestroyExpression(Expression *expression);
void DestroyStatement(Statement *statement);
void DestroyIdentifierList(IdentifierList *list);


#line 1652 "syntactic.h"


#endif // !YY_M68KASM_SYNTACTIC_H_INCLUDED
