/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_M68KASM_SYNTACTIC_H_INCLUDED
# define YY_M68KASM_SYNTACTIC_H_INCLUDED
/* Debug traces.  */
#ifndef M68KASM_DEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define M68KASM_DEBUG 1
#  else
#   define M68KASM_DEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define M68KASM_DEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined M68KASM_DEBUG */
#if M68KASM_DEBUG
extern int m68kasm_debug;
#endif
/* "%code requires" blocks.  */
#line 30 "syntactic.y"


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


#line 423 "syntactic.h"

/* Token kinds.  */
#ifndef M68KASM_TOKENTYPE
# define M68KASM_TOKENTYPE
  enum m68kasm_tokentype
  {
    M68KASM_EMPTY = -2,
    M68KASM_EOF = 0,               /* "end of file"  */
    M68KASM_error = 256,           /* error  */
    M68KASM_UNDEF = 257,           /* "invalid token"  */
    TOKEN_DIRECTIVE_ORI = 258,     /* TOKEN_DIRECTIVE_ORI  */
    TOKEN_DIRECTIVE_ANDI = 259,    /* TOKEN_DIRECTIVE_ANDI  */
    TOKEN_DIRECTIVE_SUBI = 260,    /* TOKEN_DIRECTIVE_SUBI  */
    TOKEN_DIRECTIVE_ADDI = 261,    /* TOKEN_DIRECTIVE_ADDI  */
    TOKEN_DIRECTIVE_EORI = 262,    /* TOKEN_DIRECTIVE_EORI  */
    TOKEN_DIRECTIVE_CMPI = 263,    /* TOKEN_DIRECTIVE_CMPI  */
    TOKEN_DIRECTIVE_BTST = 264,    /* TOKEN_DIRECTIVE_BTST  */
    TOKEN_DIRECTIVE_BCHG = 265,    /* TOKEN_DIRECTIVE_BCHG  */
    TOKEN_DIRECTIVE_BCLR = 266,    /* TOKEN_DIRECTIVE_BCLR  */
    TOKEN_DIRECTIVE_BSET = 267,    /* TOKEN_DIRECTIVE_BSET  */
    TOKEN_DIRECTIVE_MOVEP = 268,   /* TOKEN_DIRECTIVE_MOVEP  */
    TOKEN_DIRECTIVE_MOVEA = 269,   /* TOKEN_DIRECTIVE_MOVEA  */
    TOKEN_DIRECTIVE_MOVE = 270,    /* TOKEN_DIRECTIVE_MOVE  */
    TOKEN_DIRECTIVE_NEGX = 271,    /* TOKEN_DIRECTIVE_NEGX  */
    TOKEN_DIRECTIVE_CLR = 272,     /* TOKEN_DIRECTIVE_CLR  */
    TOKEN_DIRECTIVE_NEG = 273,     /* TOKEN_DIRECTIVE_NEG  */
    TOKEN_DIRECTIVE_NOT = 274,     /* TOKEN_DIRECTIVE_NOT  */
    TOKEN_DIRECTIVE_EXT = 275,     /* TOKEN_DIRECTIVE_EXT  */
    TOKEN_DIRECTIVE_NBCD = 276,    /* TOKEN_DIRECTIVE_NBCD  */
    TOKEN_DIRECTIVE_SWAP = 277,    /* TOKEN_DIRECTIVE_SWAP  */
    TOKEN_DIRECTIVE_PEA = 278,     /* TOKEN_DIRECTIVE_PEA  */
    TOKEN_DIRECTIVE_ILLEGAL = 279, /* TOKEN_DIRECTIVE_ILLEGAL  */
    TOKEN_DIRECTIVE_TAS = 280,     /* TOKEN_DIRECTIVE_TAS  */
    TOKEN_DIRECTIVE_TST = 281,     /* TOKEN_DIRECTIVE_TST  */
    TOKEN_DIRECTIVE_TRAP = 282,    /* TOKEN_DIRECTIVE_TRAP  */
    TOKEN_DIRECTIVE_LINK = 283,    /* TOKEN_DIRECTIVE_LINK  */
    TOKEN_DIRECTIVE_UNLK = 284,    /* TOKEN_DIRECTIVE_UNLK  */
    TOKEN_DIRECTIVE_RESET = 285,   /* TOKEN_DIRECTIVE_RESET  */
    TOKEN_DIRECTIVE_NOP = 286,     /* TOKEN_DIRECTIVE_NOP  */
    TOKEN_DIRECTIVE_STOP = 287,    /* TOKEN_DIRECTIVE_STOP  */
    TOKEN_DIRECTIVE_RTE = 288,     /* TOKEN_DIRECTIVE_RTE  */
    TOKEN_DIRECTIVE_RTS = 289,     /* TOKEN_DIRECTIVE_RTS  */
    TOKEN_DIRECTIVE_TRAPV = 290,   /* TOKEN_DIRECTIVE_TRAPV  */
    TOKEN_DIRECTIVE_RTR = 291,     /* TOKEN_DIRECTIVE_RTR  */
    TOKEN_DIRECTIVE_JSR = 292,     /* TOKEN_DIRECTIVE_JSR  */
    TOKEN_DIRECTIVE_JMP = 293,     /* TOKEN_DIRECTIVE_JMP  */
    TOKEN_DIRECTIVE_MOVEM = 294,   /* TOKEN_DIRECTIVE_MOVEM  */
    TOKEN_DIRECTIVE_LEA = 295,     /* TOKEN_DIRECTIVE_LEA  */
    TOKEN_DIRECTIVE_CHK = 296,     /* TOKEN_DIRECTIVE_CHK  */
    TOKEN_DIRECTIVE_ADDQ = 297,    /* TOKEN_DIRECTIVE_ADDQ  */
    TOKEN_DIRECTIVE_SUBQ = 298,    /* TOKEN_DIRECTIVE_SUBQ  */
    TOKEN_DIRECTIVE_ST = 299,      /* TOKEN_DIRECTIVE_ST  */
    TOKEN_DIRECTIVE_SF = 300,      /* TOKEN_DIRECTIVE_SF  */
    TOKEN_DIRECTIVE_SHI = 301,     /* TOKEN_DIRECTIVE_SHI  */
    TOKEN_DIRECTIVE_SLS = 302,     /* TOKEN_DIRECTIVE_SLS  */
    TOKEN_DIRECTIVE_SCC = 303,     /* TOKEN_DIRECTIVE_SCC  */
    TOKEN_DIRECTIVE_SCS = 304,     /* TOKEN_DIRECTIVE_SCS  */
    TOKEN_DIRECTIVE_SNE = 305,     /* TOKEN_DIRECTIVE_SNE  */
    TOKEN_DIRECTIVE_SEQ = 306,     /* TOKEN_DIRECTIVE_SEQ  */
    TOKEN_DIRECTIVE_SVC = 307,     /* TOKEN_DIRECTIVE_SVC  */
    TOKEN_DIRECTIVE_SVS = 308,     /* TOKEN_DIRECTIVE_SVS  */
    TOKEN_DIRECTIVE_SPL = 309,     /* TOKEN_DIRECTIVE_SPL  */
    TOKEN_DIRECTIVE_SMI = 310,     /* TOKEN_DIRECTIVE_SMI  */
    TOKEN_DIRECTIVE_SGE = 311,     /* TOKEN_DIRECTIVE_SGE  */
    TOKEN_DIRECTIVE_SLT = 312,     /* TOKEN_DIRECTIVE_SLT  */
    TOKEN_DIRECTIVE_SGT = 313,     /* TOKEN_DIRECTIVE_SGT  */
    TOKEN_DIRECTIVE_SLE = 314,     /* TOKEN_DIRECTIVE_SLE  */
    TOKEN_DIRECTIVE_DBT = 315,     /* TOKEN_DIRECTIVE_DBT  */
    TOKEN_DIRECTIVE_DBF = 316,     /* TOKEN_DIRECTIVE_DBF  */
    TOKEN_DIRECTIVE_DBHI = 317,    /* TOKEN_DIRECTIVE_DBHI  */
    TOKEN_DIRECTIVE_DBLS = 318,    /* TOKEN_DIRECTIVE_DBLS  */
    TOKEN_DIRECTIVE_DBCC = 319,    /* TOKEN_DIRECTIVE_DBCC  */
    TOKEN_DIRECTIVE_DBCS = 320,    /* TOKEN_DIRECTIVE_DBCS  */
    TOKEN_DIRECTIVE_DBNE = 321,    /* TOKEN_DIRECTIVE_DBNE  */
    TOKEN_DIRECTIVE_DBEQ = 322,    /* TOKEN_DIRECTIVE_DBEQ  */
    TOKEN_DIRECTIVE_DBVC = 323,    /* TOKEN_DIRECTIVE_DBVC  */
    TOKEN_DIRECTIVE_DBVS = 324,    /* TOKEN_DIRECTIVE_DBVS  */
    TOKEN_DIRECTIVE_DBPL = 325,    /* TOKEN_DIRECTIVE_DBPL  */
    TOKEN_DIRECTIVE_DBMI = 326,    /* TOKEN_DIRECTIVE_DBMI  */
    TOKEN_DIRECTIVE_DBGE = 327,    /* TOKEN_DIRECTIVE_DBGE  */
    TOKEN_DIRECTIVE_DBLT = 328,    /* TOKEN_DIRECTIVE_DBLT  */
    TOKEN_DIRECTIVE_DBGT = 329,    /* TOKEN_DIRECTIVE_DBGT  */
    TOKEN_DIRECTIVE_DBLE = 330,    /* TOKEN_DIRECTIVE_DBLE  */
    TOKEN_DIRECTIVE_BRA = 331,     /* TOKEN_DIRECTIVE_BRA  */
    TOKEN_DIRECTIVE_BSR = 332,     /* TOKEN_DIRECTIVE_BSR  */
    TOKEN_DIRECTIVE_BHI = 333,     /* TOKEN_DIRECTIVE_BHI  */
    TOKEN_DIRECTIVE_BLS = 334,     /* TOKEN_DIRECTIVE_BLS  */
    TOKEN_DIRECTIVE_BCC = 335,     /* TOKEN_DIRECTIVE_BCC  */
    TOKEN_DIRECTIVE_BCS = 336,     /* TOKEN_DIRECTIVE_BCS  */
    TOKEN_DIRECTIVE_BNE = 337,     /* TOKEN_DIRECTIVE_BNE  */
    TOKEN_DIRECTIVE_BEQ = 338,     /* TOKEN_DIRECTIVE_BEQ  */
    TOKEN_DIRECTIVE_BVC = 339,     /* TOKEN_DIRECTIVE_BVC  */
    TOKEN_DIRECTIVE_BVS = 340,     /* TOKEN_DIRECTIVE_BVS  */
    TOKEN_DIRECTIVE_BPL = 341,     /* TOKEN_DIRECTIVE_BPL  */
    TOKEN_DIRECTIVE_BMI = 342,     /* TOKEN_DIRECTIVE_BMI  */
    TOKEN_DIRECTIVE_BGE = 343,     /* TOKEN_DIRECTIVE_BGE  */
    TOKEN_DIRECTIVE_BLT = 344,     /* TOKEN_DIRECTIVE_BLT  */
    TOKEN_DIRECTIVE_BGT = 345,     /* TOKEN_DIRECTIVE_BGT  */
    TOKEN_DIRECTIVE_BLE = 346,     /* TOKEN_DIRECTIVE_BLE  */
    TOKEN_DIRECTIVE_MOVEQ = 347,   /* TOKEN_DIRECTIVE_MOVEQ  */
    TOKEN_DIRECTIVE_DIVU = 348,    /* TOKEN_DIRECTIVE_DIVU  */
    TOKEN_DIRECTIVE_DIVS = 349,    /* TOKEN_DIRECTIVE_DIVS  */
    TOKEN_DIRECTIVE_SBCD = 350,    /* TOKEN_DIRECTIVE_SBCD  */
    TOKEN_DIRECTIVE_OR = 351,      /* TOKEN_DIRECTIVE_OR  */
    TOKEN_DIRECTIVE_SUB = 352,     /* TOKEN_DIRECTIVE_SUB  */
    TOKEN_DIRECTIVE_SUBX = 353,    /* TOKEN_DIRECTIVE_SUBX  */
    TOKEN_DIRECTIVE_SUBA = 354,    /* TOKEN_DIRECTIVE_SUBA  */
    TOKEN_DIRECTIVE_EOR = 355,     /* TOKEN_DIRECTIVE_EOR  */
    TOKEN_DIRECTIVE_CMPM = 356,    /* TOKEN_DIRECTIVE_CMPM  */
    TOKEN_DIRECTIVE_CMP = 357,     /* TOKEN_DIRECTIVE_CMP  */
    TOKEN_DIRECTIVE_CMPA = 358,    /* TOKEN_DIRECTIVE_CMPA  */
    TOKEN_DIRECTIVE_MULU = 359,    /* TOKEN_DIRECTIVE_MULU  */
    TOKEN_DIRECTIVE_MULS = 360,    /* TOKEN_DIRECTIVE_MULS  */
    TOKEN_DIRECTIVE_ABCD = 361,    /* TOKEN_DIRECTIVE_ABCD  */
    TOKEN_DIRECTIVE_EXG = 362,     /* TOKEN_DIRECTIVE_EXG  */
    TOKEN_DIRECTIVE_AND = 363,     /* TOKEN_DIRECTIVE_AND  */
    TOKEN_DIRECTIVE_ADD = 364,     /* TOKEN_DIRECTIVE_ADD  */
    TOKEN_DIRECTIVE_ADDX = 365,    /* TOKEN_DIRECTIVE_ADDX  */
    TOKEN_DIRECTIVE_ADDA = 366,    /* TOKEN_DIRECTIVE_ADDA  */
    TOKEN_DIRECTIVE_ASL = 367,     /* TOKEN_DIRECTIVE_ASL  */
    TOKEN_DIRECTIVE_ASR = 368,     /* TOKEN_DIRECTIVE_ASR  */
    TOKEN_DIRECTIVE_LSL = 369,     /* TOKEN_DIRECTIVE_LSL  */
    TOKEN_DIRECTIVE_LSR = 370,     /* TOKEN_DIRECTIVE_LSR  */
    TOKEN_DIRECTIVE_ROXL = 371,    /* TOKEN_DIRECTIVE_ROXL  */
    TOKEN_DIRECTIVE_ROXR = 372,    /* TOKEN_DIRECTIVE_ROXR  */
    TOKEN_DIRECTIVE_ROL = 373,     /* TOKEN_DIRECTIVE_ROL  */
    TOKEN_DIRECTIVE_ROR = 374,     /* TOKEN_DIRECTIVE_ROR  */
    TOKEN_DIRECTIVE_DC = 375,      /* TOKEN_DIRECTIVE_DC  */
    TOKEN_DIRECTIVE_DCB = 376,     /* TOKEN_DIRECTIVE_DCB  */
    TOKEN_DIRECTIVE_REPT = 377,    /* TOKEN_DIRECTIVE_REPT  */
    TOKEN_DIRECTIVE_ENDR = 378,    /* TOKEN_DIRECTIVE_ENDR  */
    TOKEN_DIRECTIVE_MACRO = 379,   /* TOKEN_DIRECTIVE_MACRO  */
    TOKEN_DIRECTIVE_MACROS = 380,  /* TOKEN_DIRECTIVE_MACROS  */
    TOKEN_DIRECTIVE_ENDM = 381,    /* TOKEN_DIRECTIVE_ENDM  */
    TOKEN_DIRECTIVE_INCLUDE = 382, /* TOKEN_DIRECTIVE_INCLUDE  */
    TOKEN_DIRECTIVE_INCBIN = 383,  /* TOKEN_DIRECTIVE_INCBIN  */
    TOKEN_DIRECTIVE_EQU = 384,     /* TOKEN_DIRECTIVE_EQU  */
    TOKEN_DIRECTIVE_SET = 385,     /* TOKEN_DIRECTIVE_SET  */
    TOKEN_DIRECTIVE_IF = 386,      /* TOKEN_DIRECTIVE_IF  */
    TOKEN_DIRECTIVE_ELSEIF = 387,  /* TOKEN_DIRECTIVE_ELSEIF  */
    TOKEN_DIRECTIVE_ELSE = 388,    /* TOKEN_DIRECTIVE_ELSE  */
    TOKEN_DIRECTIVE_ENDC = 389,    /* TOKEN_DIRECTIVE_ENDC  */
    TOKEN_DIRECTIVE_WHILE = 390,   /* TOKEN_DIRECTIVE_WHILE  */
    TOKEN_DIRECTIVE_ENDW = 391,    /* TOKEN_DIRECTIVE_ENDW  */
    TOKEN_DIRECTIVE_EVEN = 392,    /* TOKEN_DIRECTIVE_EVEN  */
    TOKEN_DIRECTIVE_CNOP = 393,    /* TOKEN_DIRECTIVE_CNOP  */
    TOKEN_DIRECTIVE_INFORM = 394,  /* TOKEN_DIRECTIVE_INFORM  */
    TOKEN_DIRECTIVE_END = 395,     /* TOKEN_DIRECTIVE_END  */
    TOKEN_DIRECTIVE_RS = 396,      /* TOKEN_DIRECTIVE_RS  */
    TOKEN_DIRECTIVE_RSSET = 397,   /* TOKEN_DIRECTIVE_RSSET  */
    TOKEN_DIRECTIVE_RSRESET = 398, /* TOKEN_DIRECTIVE_RSRESET  */
    TOKEN_SIZE_BYTE = 399,         /* TOKEN_SIZE_BYTE  */
    TOKEN_SIZE_SHORT = 400,        /* TOKEN_SIZE_SHORT  */
    TOKEN_SIZE_WORD = 401,         /* TOKEN_SIZE_WORD  */
    TOKEN_SIZE_LONGWORD = 402,     /* TOKEN_SIZE_LONGWORD  */
    TOKEN_DATA_REGISTER = 403,     /* TOKEN_DATA_REGISTER  */
    TOKEN_ADDRESS_REGISTER = 404,  /* TOKEN_ADDRESS_REGISTER  */
    TOKEN_NUMBER = 405,            /* TOKEN_NUMBER  */
    TOKEN_IDENTIFIER = 406,        /* TOKEN_IDENTIFIER  */
    TOKEN_LOCAL_IDENTIFIER = 407,  /* TOKEN_LOCAL_IDENTIFIER  */
    TOKEN_STRING = 408,            /* TOKEN_STRING  */
    TOKEN_STATUS_REGISTER = 409,   /* TOKEN_STATUS_REGISTER  */
    TOKEN_CONDITION_CODE_REGISTER = 410, /* TOKEN_CONDITION_CODE_REGISTER  */
    TOKEN_USER_STACK_POINTER_REGISTER = 411, /* TOKEN_USER_STACK_POINTER_REGISTER  */
    TOKEN_PROGRAM_COUNTER = 412,   /* TOKEN_PROGRAM_COUNTER  */
    TOKEN_LOGICAL_AND = 413,       /* TOKEN_LOGICAL_AND  */
    TOKEN_LOGICAL_OR = 414,        /* TOKEN_LOGICAL_OR  */
    TOKEN_EQUALITY = 415,          /* TOKEN_EQUALITY  */
    TOKEN_INEQUALITY = 416,        /* TOKEN_INEQUALITY  */
    TOKEN_LESS_OR_EQUAL = 417,     /* TOKEN_LESS_OR_EQUAL  */
    TOKEN_MORE_OR_EQUAL = 418,     /* TOKEN_MORE_OR_EQUAL  */
    TOKEN_LEFT_SHIFT = 419,        /* TOKEN_LEFT_SHIFT  */
    TOKEN_RIGHT_SHIFT = 420,       /* TOKEN_RIGHT_SHIFT  */
    TOKEN_STRLEN = 421             /* TOKEN_STRLEN  */
  };
  typedef enum m68kasm_tokentype m68kasm_token_kind_t;
#endif

/* Value type.  */
#if ! defined M68KASM_STYPE && ! defined M68KASM_STYPE_IS_DECLARED
union M68KASM_STYPE
{
#line 422 "syntactic.y"

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

#line 619 "syntactic.h"

};
typedef union M68KASM_STYPE M68KASM_STYPE;
# define M68KASM_STYPE_IS_TRIVIAL 1
# define M68KASM_STYPE_IS_DECLARED 1
#endif




int m68kasm_parse (void *scanner, Statement *statement);

/* "%code provides" blocks.  */
#line 396 "syntactic.y"


void DestroyExpression(Expression *expression);
void DestroyStatement(Statement *statement);


#line 640 "syntactic.h"

#endif /* !YY_M68KASM_SYNTACTIC_H_INCLUDED  */
