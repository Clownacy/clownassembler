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
#  define M68KASM_DEBUG 0
# endif /* ! defined YYDEBUG */
#endif  /* ! defined M68KASM_DEBUG */
#if M68KASM_DEBUG
extern int m68kasm_debug;
#endif
/* "%code requires" blocks.  */
#line 30 "syntactic.y"


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


#line 482 "syntactic.h"

/* Token kinds.  */
#ifndef M68KASM_TOKENTYPE
# define M68KASM_TOKENTYPE
  enum m68kasm_tokentype
  {
    TOKEN_M68KASM_EMPTY = -2,
    TOKEN_YYEOF = 0,               /* "end of file"  */
    TOKEN_M68KASM_error = 256,     /* error  */
    TOKEN_M68KASM_UNDEF = 257,     /* "invalid token"  */
    TOKEN_DIRECTIVE_ORI = 258,     /* DIRECTIVE_ORI  */
    TOKEN_DIRECTIVE_ANDI = 259,    /* DIRECTIVE_ANDI  */
    TOKEN_DIRECTIVE_SUBI = 260,    /* DIRECTIVE_SUBI  */
    TOKEN_DIRECTIVE_ADDI = 261,    /* DIRECTIVE_ADDI  */
    TOKEN_DIRECTIVE_EORI = 262,    /* DIRECTIVE_EORI  */
    TOKEN_DIRECTIVE_CMPI = 263,    /* DIRECTIVE_CMPI  */
    TOKEN_DIRECTIVE_BTST = 264,    /* DIRECTIVE_BTST  */
    TOKEN_DIRECTIVE_BCHG = 265,    /* DIRECTIVE_BCHG  */
    TOKEN_DIRECTIVE_BCLR = 266,    /* DIRECTIVE_BCLR  */
    TOKEN_DIRECTIVE_BSET = 267,    /* DIRECTIVE_BSET  */
    TOKEN_DIRECTIVE_MOVEP = 268,   /* DIRECTIVE_MOVEP  */
    TOKEN_DIRECTIVE_MOVEA = 269,   /* DIRECTIVE_MOVEA  */
    TOKEN_DIRECTIVE_MOVE = 270,    /* DIRECTIVE_MOVE  */
    TOKEN_DIRECTIVE_NEGX = 271,    /* DIRECTIVE_NEGX  */
    TOKEN_DIRECTIVE_CLR = 272,     /* DIRECTIVE_CLR  */
    TOKEN_DIRECTIVE_NEG = 273,     /* DIRECTIVE_NEG  */
    TOKEN_DIRECTIVE_NOT = 274,     /* DIRECTIVE_NOT  */
    TOKEN_DIRECTIVE_EXT = 275,     /* DIRECTIVE_EXT  */
    TOKEN_DIRECTIVE_NBCD = 276,    /* DIRECTIVE_NBCD  */
    TOKEN_DIRECTIVE_SWAP = 277,    /* DIRECTIVE_SWAP  */
    TOKEN_DIRECTIVE_PEA = 278,     /* DIRECTIVE_PEA  */
    TOKEN_DIRECTIVE_ILLEGAL = 279, /* DIRECTIVE_ILLEGAL  */
    TOKEN_DIRECTIVE_TAS = 280,     /* DIRECTIVE_TAS  */
    TOKEN_DIRECTIVE_TST = 281,     /* DIRECTIVE_TST  */
    TOKEN_DIRECTIVE_TRAP = 282,    /* DIRECTIVE_TRAP  */
    TOKEN_DIRECTIVE_LINK = 283,    /* DIRECTIVE_LINK  */
    TOKEN_DIRECTIVE_UNLK = 284,    /* DIRECTIVE_UNLK  */
    TOKEN_DIRECTIVE_RESET = 285,   /* DIRECTIVE_RESET  */
    TOKEN_DIRECTIVE_NOP = 286,     /* DIRECTIVE_NOP  */
    TOKEN_DIRECTIVE_STOP = 287,    /* DIRECTIVE_STOP  */
    TOKEN_DIRECTIVE_RTE = 288,     /* DIRECTIVE_RTE  */
    TOKEN_DIRECTIVE_RTS = 289,     /* DIRECTIVE_RTS  */
    TOKEN_DIRECTIVE_TRAPV = 290,   /* DIRECTIVE_TRAPV  */
    TOKEN_DIRECTIVE_RTR = 291,     /* DIRECTIVE_RTR  */
    TOKEN_DIRECTIVE_JSR = 292,     /* DIRECTIVE_JSR  */
    TOKEN_DIRECTIVE_JMP = 293,     /* DIRECTIVE_JMP  */
    TOKEN_DIRECTIVE_MOVEM = 294,   /* DIRECTIVE_MOVEM  */
    TOKEN_DIRECTIVE_LEA = 295,     /* DIRECTIVE_LEA  */
    TOKEN_DIRECTIVE_CHK = 296,     /* DIRECTIVE_CHK  */
    TOKEN_DIRECTIVE_ADDQ = 297,    /* DIRECTIVE_ADDQ  */
    TOKEN_DIRECTIVE_SUBQ = 298,    /* DIRECTIVE_SUBQ  */
    TOKEN_DIRECTIVE_ST = 299,      /* DIRECTIVE_ST  */
    TOKEN_DIRECTIVE_SF = 300,      /* DIRECTIVE_SF  */
    TOKEN_DIRECTIVE_SHI = 301,     /* DIRECTIVE_SHI  */
    TOKEN_DIRECTIVE_SLS = 302,     /* DIRECTIVE_SLS  */
    TOKEN_DIRECTIVE_SCC = 303,     /* DIRECTIVE_SCC  */
    TOKEN_DIRECTIVE_SCS = 304,     /* DIRECTIVE_SCS  */
    TOKEN_DIRECTIVE_SNE = 305,     /* DIRECTIVE_SNE  */
    TOKEN_DIRECTIVE_SEQ = 306,     /* DIRECTIVE_SEQ  */
    TOKEN_DIRECTIVE_SVC = 307,     /* DIRECTIVE_SVC  */
    TOKEN_DIRECTIVE_SVS = 308,     /* DIRECTIVE_SVS  */
    TOKEN_DIRECTIVE_SPL = 309,     /* DIRECTIVE_SPL  */
    TOKEN_DIRECTIVE_SMI = 310,     /* DIRECTIVE_SMI  */
    TOKEN_DIRECTIVE_SGE = 311,     /* DIRECTIVE_SGE  */
    TOKEN_DIRECTIVE_SLT = 312,     /* DIRECTIVE_SLT  */
    TOKEN_DIRECTIVE_SGT = 313,     /* DIRECTIVE_SGT  */
    TOKEN_DIRECTIVE_SLE = 314,     /* DIRECTIVE_SLE  */
    TOKEN_DIRECTIVE_DBT = 315,     /* DIRECTIVE_DBT  */
    TOKEN_DIRECTIVE_DBF = 316,     /* DIRECTIVE_DBF  */
    TOKEN_DIRECTIVE_DBHI = 317,    /* DIRECTIVE_DBHI  */
    TOKEN_DIRECTIVE_DBLS = 318,    /* DIRECTIVE_DBLS  */
    TOKEN_DIRECTIVE_DBCC = 319,    /* DIRECTIVE_DBCC  */
    TOKEN_DIRECTIVE_DBCS = 320,    /* DIRECTIVE_DBCS  */
    TOKEN_DIRECTIVE_DBNE = 321,    /* DIRECTIVE_DBNE  */
    TOKEN_DIRECTIVE_DBEQ = 322,    /* DIRECTIVE_DBEQ  */
    TOKEN_DIRECTIVE_DBVC = 323,    /* DIRECTIVE_DBVC  */
    TOKEN_DIRECTIVE_DBVS = 324,    /* DIRECTIVE_DBVS  */
    TOKEN_DIRECTIVE_DBPL = 325,    /* DIRECTIVE_DBPL  */
    TOKEN_DIRECTIVE_DBMI = 326,    /* DIRECTIVE_DBMI  */
    TOKEN_DIRECTIVE_DBGE = 327,    /* DIRECTIVE_DBGE  */
    TOKEN_DIRECTIVE_DBLT = 328,    /* DIRECTIVE_DBLT  */
    TOKEN_DIRECTIVE_DBGT = 329,    /* DIRECTIVE_DBGT  */
    TOKEN_DIRECTIVE_DBLE = 330,    /* DIRECTIVE_DBLE  */
    TOKEN_DIRECTIVE_BRA = 331,     /* DIRECTIVE_BRA  */
    TOKEN_DIRECTIVE_BSR = 332,     /* DIRECTIVE_BSR  */
    TOKEN_DIRECTIVE_BHI = 333,     /* DIRECTIVE_BHI  */
    TOKEN_DIRECTIVE_BLS = 334,     /* DIRECTIVE_BLS  */
    TOKEN_DIRECTIVE_BCC = 335,     /* DIRECTIVE_BCC  */
    TOKEN_DIRECTIVE_BCS = 336,     /* DIRECTIVE_BCS  */
    TOKEN_DIRECTIVE_BNE = 337,     /* DIRECTIVE_BNE  */
    TOKEN_DIRECTIVE_BEQ = 338,     /* DIRECTIVE_BEQ  */
    TOKEN_DIRECTIVE_BVC = 339,     /* DIRECTIVE_BVC  */
    TOKEN_DIRECTIVE_BVS = 340,     /* DIRECTIVE_BVS  */
    TOKEN_DIRECTIVE_BPL = 341,     /* DIRECTIVE_BPL  */
    TOKEN_DIRECTIVE_BMI = 342,     /* DIRECTIVE_BMI  */
    TOKEN_DIRECTIVE_BGE = 343,     /* DIRECTIVE_BGE  */
    TOKEN_DIRECTIVE_BLT = 344,     /* DIRECTIVE_BLT  */
    TOKEN_DIRECTIVE_BGT = 345,     /* DIRECTIVE_BGT  */
    TOKEN_DIRECTIVE_BLE = 346,     /* DIRECTIVE_BLE  */
    TOKEN_DIRECTIVE_MOVEQ = 347,   /* DIRECTIVE_MOVEQ  */
    TOKEN_DIRECTIVE_DIVU = 348,    /* DIRECTIVE_DIVU  */
    TOKEN_DIRECTIVE_DIVS = 349,    /* DIRECTIVE_DIVS  */
    TOKEN_DIRECTIVE_SBCD = 350,    /* DIRECTIVE_SBCD  */
    TOKEN_DIRECTIVE_OR = 351,      /* DIRECTIVE_OR  */
    TOKEN_DIRECTIVE_SUB = 352,     /* DIRECTIVE_SUB  */
    TOKEN_DIRECTIVE_SUBX = 353,    /* DIRECTIVE_SUBX  */
    TOKEN_DIRECTIVE_SUBA = 354,    /* DIRECTIVE_SUBA  */
    TOKEN_DIRECTIVE_EOR = 355,     /* DIRECTIVE_EOR  */
    TOKEN_DIRECTIVE_CMPM = 356,    /* DIRECTIVE_CMPM  */
    TOKEN_DIRECTIVE_CMP = 357,     /* DIRECTIVE_CMP  */
    TOKEN_DIRECTIVE_CMPA = 358,    /* DIRECTIVE_CMPA  */
    TOKEN_DIRECTIVE_MULU = 359,    /* DIRECTIVE_MULU  */
    TOKEN_DIRECTIVE_MULS = 360,    /* DIRECTIVE_MULS  */
    TOKEN_DIRECTIVE_ABCD = 361,    /* DIRECTIVE_ABCD  */
    TOKEN_DIRECTIVE_EXG = 362,     /* DIRECTIVE_EXG  */
    TOKEN_DIRECTIVE_AND = 363,     /* DIRECTIVE_AND  */
    TOKEN_DIRECTIVE_ADD = 364,     /* DIRECTIVE_ADD  */
    TOKEN_DIRECTIVE_ADDX = 365,    /* DIRECTIVE_ADDX  */
    TOKEN_DIRECTIVE_ADDA = 366,    /* DIRECTIVE_ADDA  */
    TOKEN_DIRECTIVE_ASL = 367,     /* DIRECTIVE_ASL  */
    TOKEN_DIRECTIVE_ASR = 368,     /* DIRECTIVE_ASR  */
    TOKEN_DIRECTIVE_LSL = 369,     /* DIRECTIVE_LSL  */
    TOKEN_DIRECTIVE_LSR = 370,     /* DIRECTIVE_LSR  */
    TOKEN_DIRECTIVE_ROXL = 371,    /* DIRECTIVE_ROXL  */
    TOKEN_DIRECTIVE_ROXR = 372,    /* DIRECTIVE_ROXR  */
    TOKEN_DIRECTIVE_ROL = 373,     /* DIRECTIVE_ROL  */
    TOKEN_DIRECTIVE_ROR = 374,     /* DIRECTIVE_ROR  */
    TOKEN_DIRECTIVE_DC = 375,      /* DIRECTIVE_DC  */
    TOKEN_DIRECTIVE_DCB = 376,     /* DIRECTIVE_DCB  */
    TOKEN_DIRECTIVE_DS = 377,      /* DIRECTIVE_DS  */
    TOKEN_DIRECTIVE_REPT = 378,    /* DIRECTIVE_REPT  */
    TOKEN_DIRECTIVE_ENDR = 379,    /* DIRECTIVE_ENDR  */
    TOKEN_DIRECTIVE_MACRO = 380,   /* DIRECTIVE_MACRO  */
    TOKEN_DIRECTIVE_MACROS = 381,  /* DIRECTIVE_MACROS  */
    TOKEN_DIRECTIVE_ENDM = 382,    /* DIRECTIVE_ENDM  */
    TOKEN_DIRECTIVE_INCLUDE = 383, /* DIRECTIVE_INCLUDE  */
    TOKEN_DIRECTIVE_INCBIN = 384,  /* DIRECTIVE_INCBIN  */
    TOKEN_DIRECTIVE_EQU = 385,     /* DIRECTIVE_EQU  */
    TOKEN_DIRECTIVE_EQUS = 386,    /* DIRECTIVE_EQUS  */
    TOKEN_DIRECTIVE_SUBSTR = 387,  /* DIRECTIVE_SUBSTR  */
    TOKEN_DIRECTIVE_SET = 388,     /* DIRECTIVE_SET  */
    TOKEN_DIRECTIVE_IF = 389,      /* DIRECTIVE_IF  */
    TOKEN_DIRECTIVE_ELSEIF = 390,  /* DIRECTIVE_ELSEIF  */
    TOKEN_DIRECTIVE_ELSE = 391,    /* DIRECTIVE_ELSE  */
    TOKEN_DIRECTIVE_ENDC = 392,    /* DIRECTIVE_ENDC  */
    TOKEN_DIRECTIVE_WHILE = 393,   /* DIRECTIVE_WHILE  */
    TOKEN_DIRECTIVE_ENDW = 394,    /* DIRECTIVE_ENDW  */
    TOKEN_DIRECTIVE_EVEN = 395,    /* DIRECTIVE_EVEN  */
    TOKEN_DIRECTIVE_CNOP = 396,    /* DIRECTIVE_CNOP  */
    TOKEN_DIRECTIVE_INFORM = 397,  /* DIRECTIVE_INFORM  */
    TOKEN_DIRECTIVE_FAIL = 398,    /* DIRECTIVE_FAIL  */
    TOKEN_DIRECTIVE_END = 399,     /* DIRECTIVE_END  */
    TOKEN_DIRECTIVE_RS = 400,      /* DIRECTIVE_RS  */
    TOKEN_DIRECTIVE_RSSET = 401,   /* DIRECTIVE_RSSET  */
    TOKEN_DIRECTIVE_RSRESET = 402, /* DIRECTIVE_RSRESET  */
    TOKEN_DIRECTIVE_OBJ = 403,     /* DIRECTIVE_OBJ  */
    TOKEN_DIRECTIVE_OBJEND = 404,  /* DIRECTIVE_OBJEND  */
    TOKEN_DIRECTIVE_ORG = 405,     /* DIRECTIVE_ORG  */
    TOKEN_DIRECTIVE_PUSHO = 406,   /* DIRECTIVE_PUSHO  */
    TOKEN_DIRECTIVE_POPO = 407,    /* DIRECTIVE_POPO  */
    TOKEN_DIRECTIVE_OPT = 408,     /* DIRECTIVE_OPT  */
    TOKEN_DIRECTIVE_PUSHP = 409,   /* DIRECTIVE_PUSHP  */
    TOKEN_DIRECTIVE_POPP = 410,    /* DIRECTIVE_POPP  */
    TOKEN_DIRECTIVE_SHIFT = 411,   /* DIRECTIVE_SHIFT  */
    TOKEN_DIRECTIVE_MEXIT = 412,   /* DIRECTIVE_MEXIT  */
    TOKEN_DIRECTIVE_LOCAL = 413,   /* DIRECTIVE_LOCAL  */
    TOKEN_DIRECTIVE_LIST = 414,    /* DIRECTIVE_LIST  */
    TOKEN_DIRECTIVE_NOLIST = 415,  /* DIRECTIVE_NOLIST  */
    TOKEN_DIRECTIVE_PURGE = 416,   /* DIRECTIVE_PURGE  */
    TOKEN_SIZE_BYTE = 417,         /* SIZE_BYTE  */
    TOKEN_SIZE_SHORT = 418,        /* SIZE_SHORT  */
    TOKEN_SIZE_WORD = 419,         /* SIZE_WORD  */
    TOKEN_SIZE_LONGWORD = 420,     /* SIZE_LONGWORD  */
    TOKEN_DATA_REGISTER = 421,     /* DATA_REGISTER  */
    TOKEN_ADDRESS_REGISTER = 422,  /* ADDRESS_REGISTER  */
    TOKEN_NUMBER = 423,            /* NUMBER  */
    TOKEN_IDENTIFIER = 424,        /* IDENTIFIER  */
    TOKEN_LOCAL_IDENTIFIER = 425,  /* LOCAL_IDENTIFIER  */
    TOKEN_STRING = 426,            /* STRING  */
    TOKEN_OPTION = 427,            /* OPTION  */
    TOKEN_STATUS_REGISTER = 428,   /* STATUS_REGISTER  */
    TOKEN_CONDITION_CODE_REGISTER = 429, /* CONDITION_CODE_REGISTER  */
    TOKEN_USER_STACK_POINTER_REGISTER = 430, /* USER_STACK_POINTER_REGISTER  */
    TOKEN_PROGRAM_COUNTER = 431,   /* PROGRAM_COUNTER  */
    TOKEN_LOGICAL_AND = 432,       /* LOGICAL_AND  */
    TOKEN_LOGICAL_OR = 433,        /* LOGICAL_OR  */
    TOKEN_EQUALITY = 434,          /* EQUALITY  */
    TOKEN_INEQUALITY = 435,        /* INEQUALITY  */
    TOKEN_LESS_OR_EQUAL = 436,     /* LESS_OR_EQUAL  */
    TOKEN_MORE_OR_EQUAL = 437,     /* MORE_OR_EQUAL  */
    TOKEN_LEFT_SHIFT = 438,        /* LEFT_SHIFT  */
    TOKEN_RIGHT_SHIFT = 439,       /* RIGHT_SHIFT  */
    TOKEN_STRLEN = 440,            /* STRLEN  */
    TOKEN_STRCMP = 441,            /* STRCMP  */
    TOKEN_INSTR = 442,             /* INSTR  */
    TOKEN_DEF = 443,               /* DEF  */
    TOKEN_TYPE = 444,              /* TYPE  */
    TOKEN_FILESIZE = 445,          /* FILESIZE  */
    TOKEN_OFFSET = 446,            /* OFFSET  */
    TOKEN_LOWER_PRECEDENCE_THAN_SIZE = 447, /* LOWER_PRECEDENCE_THAN_SIZE  */
    TOKEN_HIGHER_PRECEDENCE_THAN_SIZE = 448 /* HIGHER_PRECEDENCE_THAN_SIZE  */
  };
  typedef enum m68kasm_tokentype m68kasm_token_kind_t;
#endif

/* Value type.  */
#if ! defined M68KASM_STYPE && ! defined M68KASM_STYPE_IS_DECLARED
union M68KASM_STYPE
{
#line 483 "syntactic.y"

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

#line 705 "syntactic.h"

};
typedef union M68KASM_STYPE M68KASM_STYPE;
# define M68KASM_STYPE_IS_TRIVIAL 1
# define M68KASM_STYPE_IS_DECLARED 1
#endif




int m68kasm_parse (void *scanner, Statement *statement);

/* "%code provides" blocks.  */
#line 455 "syntactic.y"


void DestroyExpression(Expression *expression);
void DestroyStatement(Statement *statement);
void DestroyIdentifierList(IdentifierList *list);


#line 727 "syntactic.h"

#endif /* !YY_M68KASM_SYNTACTIC_H_INCLUDED  */
