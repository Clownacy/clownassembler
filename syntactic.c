/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Substitute the type names.  */
#define YYSTYPE         M68KASM_STYPE
/* Substitute the variable and function names.  */
#define yyparse         m68kasm_parse
#define yylex           m68kasm_lex
#define yyerror         m68kasm_error
#define yydebug         m68kasm_debug
#define yynerrs         m68kasm_nerrs


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

#include "syntactic.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOKEN_DIRECTIVE_ORI = 3,        /* TOKEN_DIRECTIVE_ORI  */
  YYSYMBOL_TOKEN_DIRECTIVE_ANDI = 4,       /* TOKEN_DIRECTIVE_ANDI  */
  YYSYMBOL_TOKEN_DIRECTIVE_SUBI = 5,       /* TOKEN_DIRECTIVE_SUBI  */
  YYSYMBOL_TOKEN_DIRECTIVE_ADDI = 6,       /* TOKEN_DIRECTIVE_ADDI  */
  YYSYMBOL_TOKEN_DIRECTIVE_EORI = 7,       /* TOKEN_DIRECTIVE_EORI  */
  YYSYMBOL_TOKEN_DIRECTIVE_CMPI = 8,       /* TOKEN_DIRECTIVE_CMPI  */
  YYSYMBOL_TOKEN_DIRECTIVE_BTST = 9,       /* TOKEN_DIRECTIVE_BTST  */
  YYSYMBOL_TOKEN_DIRECTIVE_BCHG = 10,      /* TOKEN_DIRECTIVE_BCHG  */
  YYSYMBOL_TOKEN_DIRECTIVE_BCLR = 11,      /* TOKEN_DIRECTIVE_BCLR  */
  YYSYMBOL_TOKEN_DIRECTIVE_BSET = 12,      /* TOKEN_DIRECTIVE_BSET  */
  YYSYMBOL_TOKEN_DIRECTIVE_MOVEP = 13,     /* TOKEN_DIRECTIVE_MOVEP  */
  YYSYMBOL_TOKEN_DIRECTIVE_MOVEA = 14,     /* TOKEN_DIRECTIVE_MOVEA  */
  YYSYMBOL_TOKEN_DIRECTIVE_MOVE = 15,      /* TOKEN_DIRECTIVE_MOVE  */
  YYSYMBOL_TOKEN_DIRECTIVE_NEGX = 16,      /* TOKEN_DIRECTIVE_NEGX  */
  YYSYMBOL_TOKEN_DIRECTIVE_CLR = 17,       /* TOKEN_DIRECTIVE_CLR  */
  YYSYMBOL_TOKEN_DIRECTIVE_NEG = 18,       /* TOKEN_DIRECTIVE_NEG  */
  YYSYMBOL_TOKEN_DIRECTIVE_NOT = 19,       /* TOKEN_DIRECTIVE_NOT  */
  YYSYMBOL_TOKEN_DIRECTIVE_EXT = 20,       /* TOKEN_DIRECTIVE_EXT  */
  YYSYMBOL_TOKEN_DIRECTIVE_NBCD = 21,      /* TOKEN_DIRECTIVE_NBCD  */
  YYSYMBOL_TOKEN_DIRECTIVE_SWAP = 22,      /* TOKEN_DIRECTIVE_SWAP  */
  YYSYMBOL_TOKEN_DIRECTIVE_PEA = 23,       /* TOKEN_DIRECTIVE_PEA  */
  YYSYMBOL_TOKEN_DIRECTIVE_ILLEGAL = 24,   /* TOKEN_DIRECTIVE_ILLEGAL  */
  YYSYMBOL_TOKEN_DIRECTIVE_TAS = 25,       /* TOKEN_DIRECTIVE_TAS  */
  YYSYMBOL_TOKEN_DIRECTIVE_TST = 26,       /* TOKEN_DIRECTIVE_TST  */
  YYSYMBOL_TOKEN_DIRECTIVE_TRAP = 27,      /* TOKEN_DIRECTIVE_TRAP  */
  YYSYMBOL_TOKEN_DIRECTIVE_LINK = 28,      /* TOKEN_DIRECTIVE_LINK  */
  YYSYMBOL_TOKEN_DIRECTIVE_UNLK = 29,      /* TOKEN_DIRECTIVE_UNLK  */
  YYSYMBOL_TOKEN_DIRECTIVE_RESET = 30,     /* TOKEN_DIRECTIVE_RESET  */
  YYSYMBOL_TOKEN_DIRECTIVE_NOP = 31,       /* TOKEN_DIRECTIVE_NOP  */
  YYSYMBOL_TOKEN_DIRECTIVE_STOP = 32,      /* TOKEN_DIRECTIVE_STOP  */
  YYSYMBOL_TOKEN_DIRECTIVE_RTE = 33,       /* TOKEN_DIRECTIVE_RTE  */
  YYSYMBOL_TOKEN_DIRECTIVE_RTS = 34,       /* TOKEN_DIRECTIVE_RTS  */
  YYSYMBOL_TOKEN_DIRECTIVE_TRAPV = 35,     /* TOKEN_DIRECTIVE_TRAPV  */
  YYSYMBOL_TOKEN_DIRECTIVE_RTR = 36,       /* TOKEN_DIRECTIVE_RTR  */
  YYSYMBOL_TOKEN_DIRECTIVE_JSR = 37,       /* TOKEN_DIRECTIVE_JSR  */
  YYSYMBOL_TOKEN_DIRECTIVE_JMP = 38,       /* TOKEN_DIRECTIVE_JMP  */
  YYSYMBOL_TOKEN_DIRECTIVE_MOVEM = 39,     /* TOKEN_DIRECTIVE_MOVEM  */
  YYSYMBOL_TOKEN_DIRECTIVE_LEA = 40,       /* TOKEN_DIRECTIVE_LEA  */
  YYSYMBOL_TOKEN_DIRECTIVE_CHK = 41,       /* TOKEN_DIRECTIVE_CHK  */
  YYSYMBOL_TOKEN_DIRECTIVE_ADDQ = 42,      /* TOKEN_DIRECTIVE_ADDQ  */
  YYSYMBOL_TOKEN_DIRECTIVE_SUBQ = 43,      /* TOKEN_DIRECTIVE_SUBQ  */
  YYSYMBOL_TOKEN_DIRECTIVE_ST = 44,        /* TOKEN_DIRECTIVE_ST  */
  YYSYMBOL_TOKEN_DIRECTIVE_SF = 45,        /* TOKEN_DIRECTIVE_SF  */
  YYSYMBOL_TOKEN_DIRECTIVE_SHI = 46,       /* TOKEN_DIRECTIVE_SHI  */
  YYSYMBOL_TOKEN_DIRECTIVE_SLS = 47,       /* TOKEN_DIRECTIVE_SLS  */
  YYSYMBOL_TOKEN_DIRECTIVE_SCC = 48,       /* TOKEN_DIRECTIVE_SCC  */
  YYSYMBOL_TOKEN_DIRECTIVE_SCS = 49,       /* TOKEN_DIRECTIVE_SCS  */
  YYSYMBOL_TOKEN_DIRECTIVE_SNE = 50,       /* TOKEN_DIRECTIVE_SNE  */
  YYSYMBOL_TOKEN_DIRECTIVE_SEQ = 51,       /* TOKEN_DIRECTIVE_SEQ  */
  YYSYMBOL_TOKEN_DIRECTIVE_SVC = 52,       /* TOKEN_DIRECTIVE_SVC  */
  YYSYMBOL_TOKEN_DIRECTIVE_SVS = 53,       /* TOKEN_DIRECTIVE_SVS  */
  YYSYMBOL_TOKEN_DIRECTIVE_SPL = 54,       /* TOKEN_DIRECTIVE_SPL  */
  YYSYMBOL_TOKEN_DIRECTIVE_SMI = 55,       /* TOKEN_DIRECTIVE_SMI  */
  YYSYMBOL_TOKEN_DIRECTIVE_SGE = 56,       /* TOKEN_DIRECTIVE_SGE  */
  YYSYMBOL_TOKEN_DIRECTIVE_SLT = 57,       /* TOKEN_DIRECTIVE_SLT  */
  YYSYMBOL_TOKEN_DIRECTIVE_SGT = 58,       /* TOKEN_DIRECTIVE_SGT  */
  YYSYMBOL_TOKEN_DIRECTIVE_SLE = 59,       /* TOKEN_DIRECTIVE_SLE  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBT = 60,       /* TOKEN_DIRECTIVE_DBT  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBF = 61,       /* TOKEN_DIRECTIVE_DBF  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBHI = 62,      /* TOKEN_DIRECTIVE_DBHI  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBLS = 63,      /* TOKEN_DIRECTIVE_DBLS  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBCC = 64,      /* TOKEN_DIRECTIVE_DBCC  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBCS = 65,      /* TOKEN_DIRECTIVE_DBCS  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBNE = 66,      /* TOKEN_DIRECTIVE_DBNE  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBEQ = 67,      /* TOKEN_DIRECTIVE_DBEQ  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBVC = 68,      /* TOKEN_DIRECTIVE_DBVC  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBVS = 69,      /* TOKEN_DIRECTIVE_DBVS  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBPL = 70,      /* TOKEN_DIRECTIVE_DBPL  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBMI = 71,      /* TOKEN_DIRECTIVE_DBMI  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBGE = 72,      /* TOKEN_DIRECTIVE_DBGE  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBLT = 73,      /* TOKEN_DIRECTIVE_DBLT  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBGT = 74,      /* TOKEN_DIRECTIVE_DBGT  */
  YYSYMBOL_TOKEN_DIRECTIVE_DBLE = 75,      /* TOKEN_DIRECTIVE_DBLE  */
  YYSYMBOL_TOKEN_DIRECTIVE_BRA = 76,       /* TOKEN_DIRECTIVE_BRA  */
  YYSYMBOL_TOKEN_DIRECTIVE_BSR = 77,       /* TOKEN_DIRECTIVE_BSR  */
  YYSYMBOL_TOKEN_DIRECTIVE_BHI = 78,       /* TOKEN_DIRECTIVE_BHI  */
  YYSYMBOL_TOKEN_DIRECTIVE_BLS = 79,       /* TOKEN_DIRECTIVE_BLS  */
  YYSYMBOL_TOKEN_DIRECTIVE_BCC = 80,       /* TOKEN_DIRECTIVE_BCC  */
  YYSYMBOL_TOKEN_DIRECTIVE_BCS = 81,       /* TOKEN_DIRECTIVE_BCS  */
  YYSYMBOL_TOKEN_DIRECTIVE_BNE = 82,       /* TOKEN_DIRECTIVE_BNE  */
  YYSYMBOL_TOKEN_DIRECTIVE_BEQ = 83,       /* TOKEN_DIRECTIVE_BEQ  */
  YYSYMBOL_TOKEN_DIRECTIVE_BVC = 84,       /* TOKEN_DIRECTIVE_BVC  */
  YYSYMBOL_TOKEN_DIRECTIVE_BVS = 85,       /* TOKEN_DIRECTIVE_BVS  */
  YYSYMBOL_TOKEN_DIRECTIVE_BPL = 86,       /* TOKEN_DIRECTIVE_BPL  */
  YYSYMBOL_TOKEN_DIRECTIVE_BMI = 87,       /* TOKEN_DIRECTIVE_BMI  */
  YYSYMBOL_TOKEN_DIRECTIVE_BGE = 88,       /* TOKEN_DIRECTIVE_BGE  */
  YYSYMBOL_TOKEN_DIRECTIVE_BLT = 89,       /* TOKEN_DIRECTIVE_BLT  */
  YYSYMBOL_TOKEN_DIRECTIVE_BGT = 90,       /* TOKEN_DIRECTIVE_BGT  */
  YYSYMBOL_TOKEN_DIRECTIVE_BLE = 91,       /* TOKEN_DIRECTIVE_BLE  */
  YYSYMBOL_TOKEN_DIRECTIVE_MOVEQ = 92,     /* TOKEN_DIRECTIVE_MOVEQ  */
  YYSYMBOL_TOKEN_DIRECTIVE_DIVU = 93,      /* TOKEN_DIRECTIVE_DIVU  */
  YYSYMBOL_TOKEN_DIRECTIVE_DIVS = 94,      /* TOKEN_DIRECTIVE_DIVS  */
  YYSYMBOL_TOKEN_DIRECTIVE_SBCD = 95,      /* TOKEN_DIRECTIVE_SBCD  */
  YYSYMBOL_TOKEN_DIRECTIVE_OR = 96,        /* TOKEN_DIRECTIVE_OR  */
  YYSYMBOL_TOKEN_DIRECTIVE_SUB = 97,       /* TOKEN_DIRECTIVE_SUB  */
  YYSYMBOL_TOKEN_DIRECTIVE_SUBX = 98,      /* TOKEN_DIRECTIVE_SUBX  */
  YYSYMBOL_TOKEN_DIRECTIVE_SUBA = 99,      /* TOKEN_DIRECTIVE_SUBA  */
  YYSYMBOL_TOKEN_DIRECTIVE_EOR = 100,      /* TOKEN_DIRECTIVE_EOR  */
  YYSYMBOL_TOKEN_DIRECTIVE_CMPM = 101,     /* TOKEN_DIRECTIVE_CMPM  */
  YYSYMBOL_TOKEN_DIRECTIVE_CMP = 102,      /* TOKEN_DIRECTIVE_CMP  */
  YYSYMBOL_TOKEN_DIRECTIVE_CMPA = 103,     /* TOKEN_DIRECTIVE_CMPA  */
  YYSYMBOL_TOKEN_DIRECTIVE_MULU = 104,     /* TOKEN_DIRECTIVE_MULU  */
  YYSYMBOL_TOKEN_DIRECTIVE_MULS = 105,     /* TOKEN_DIRECTIVE_MULS  */
  YYSYMBOL_TOKEN_DIRECTIVE_ABCD = 106,     /* TOKEN_DIRECTIVE_ABCD  */
  YYSYMBOL_TOKEN_DIRECTIVE_EXG = 107,      /* TOKEN_DIRECTIVE_EXG  */
  YYSYMBOL_TOKEN_DIRECTIVE_AND = 108,      /* TOKEN_DIRECTIVE_AND  */
  YYSYMBOL_TOKEN_DIRECTIVE_ADD = 109,      /* TOKEN_DIRECTIVE_ADD  */
  YYSYMBOL_TOKEN_DIRECTIVE_ADDX = 110,     /* TOKEN_DIRECTIVE_ADDX  */
  YYSYMBOL_TOKEN_DIRECTIVE_ADDA = 111,     /* TOKEN_DIRECTIVE_ADDA  */
  YYSYMBOL_TOKEN_DIRECTIVE_ASL = 112,      /* TOKEN_DIRECTIVE_ASL  */
  YYSYMBOL_TOKEN_DIRECTIVE_ASR = 113,      /* TOKEN_DIRECTIVE_ASR  */
  YYSYMBOL_TOKEN_DIRECTIVE_LSL = 114,      /* TOKEN_DIRECTIVE_LSL  */
  YYSYMBOL_TOKEN_DIRECTIVE_LSR = 115,      /* TOKEN_DIRECTIVE_LSR  */
  YYSYMBOL_TOKEN_DIRECTIVE_ROXL = 116,     /* TOKEN_DIRECTIVE_ROXL  */
  YYSYMBOL_TOKEN_DIRECTIVE_ROXR = 117,     /* TOKEN_DIRECTIVE_ROXR  */
  YYSYMBOL_TOKEN_DIRECTIVE_ROL = 118,      /* TOKEN_DIRECTIVE_ROL  */
  YYSYMBOL_TOKEN_DIRECTIVE_ROR = 119,      /* TOKEN_DIRECTIVE_ROR  */
  YYSYMBOL_TOKEN_DIRECTIVE_DC = 120,       /* TOKEN_DIRECTIVE_DC  */
  YYSYMBOL_TOKEN_DIRECTIVE_DCB = 121,      /* TOKEN_DIRECTIVE_DCB  */
  YYSYMBOL_TOKEN_DIRECTIVE_DS = 122,       /* TOKEN_DIRECTIVE_DS  */
  YYSYMBOL_TOKEN_DIRECTIVE_REPT = 123,     /* TOKEN_DIRECTIVE_REPT  */
  YYSYMBOL_TOKEN_DIRECTIVE_ENDR = 124,     /* TOKEN_DIRECTIVE_ENDR  */
  YYSYMBOL_TOKEN_DIRECTIVE_MACRO = 125,    /* TOKEN_DIRECTIVE_MACRO  */
  YYSYMBOL_TOKEN_DIRECTIVE_MACROS = 126,   /* TOKEN_DIRECTIVE_MACROS  */
  YYSYMBOL_TOKEN_DIRECTIVE_ENDM = 127,     /* TOKEN_DIRECTIVE_ENDM  */
  YYSYMBOL_TOKEN_DIRECTIVE_INCLUDE = 128,  /* TOKEN_DIRECTIVE_INCLUDE  */
  YYSYMBOL_TOKEN_DIRECTIVE_INCBIN = 129,   /* TOKEN_DIRECTIVE_INCBIN  */
  YYSYMBOL_TOKEN_DIRECTIVE_EQU = 130,      /* TOKEN_DIRECTIVE_EQU  */
  YYSYMBOL_TOKEN_DIRECTIVE_EQUS = 131,     /* TOKEN_DIRECTIVE_EQUS  */
  YYSYMBOL_TOKEN_DIRECTIVE_SUBSTR = 132,   /* TOKEN_DIRECTIVE_SUBSTR  */
  YYSYMBOL_TOKEN_DIRECTIVE_SET = 133,      /* TOKEN_DIRECTIVE_SET  */
  YYSYMBOL_TOKEN_DIRECTIVE_IF = 134,       /* TOKEN_DIRECTIVE_IF  */
  YYSYMBOL_TOKEN_DIRECTIVE_ELSEIF = 135,   /* TOKEN_DIRECTIVE_ELSEIF  */
  YYSYMBOL_TOKEN_DIRECTIVE_ELSE = 136,     /* TOKEN_DIRECTIVE_ELSE  */
  YYSYMBOL_TOKEN_DIRECTIVE_ENDC = 137,     /* TOKEN_DIRECTIVE_ENDC  */
  YYSYMBOL_TOKEN_DIRECTIVE_WHILE = 138,    /* TOKEN_DIRECTIVE_WHILE  */
  YYSYMBOL_TOKEN_DIRECTIVE_ENDW = 139,     /* TOKEN_DIRECTIVE_ENDW  */
  YYSYMBOL_TOKEN_DIRECTIVE_EVEN = 140,     /* TOKEN_DIRECTIVE_EVEN  */
  YYSYMBOL_TOKEN_DIRECTIVE_CNOP = 141,     /* TOKEN_DIRECTIVE_CNOP  */
  YYSYMBOL_TOKEN_DIRECTIVE_INFORM = 142,   /* TOKEN_DIRECTIVE_INFORM  */
  YYSYMBOL_TOKEN_DIRECTIVE_FAIL = 143,     /* TOKEN_DIRECTIVE_FAIL  */
  YYSYMBOL_TOKEN_DIRECTIVE_END = 144,      /* TOKEN_DIRECTIVE_END  */
  YYSYMBOL_TOKEN_DIRECTIVE_RS = 145,       /* TOKEN_DIRECTIVE_RS  */
  YYSYMBOL_TOKEN_DIRECTIVE_RSSET = 146,    /* TOKEN_DIRECTIVE_RSSET  */
  YYSYMBOL_TOKEN_DIRECTIVE_RSRESET = 147,  /* TOKEN_DIRECTIVE_RSRESET  */
  YYSYMBOL_TOKEN_DIRECTIVE_OBJ = 148,      /* TOKEN_DIRECTIVE_OBJ  */
  YYSYMBOL_TOKEN_DIRECTIVE_OBJEND = 149,   /* TOKEN_DIRECTIVE_OBJEND  */
  YYSYMBOL_TOKEN_DIRECTIVE_ORG = 150,      /* TOKEN_DIRECTIVE_ORG  */
  YYSYMBOL_TOKEN_DIRECTIVE_PUSHO = 151,    /* TOKEN_DIRECTIVE_PUSHO  */
  YYSYMBOL_TOKEN_DIRECTIVE_POPO = 152,     /* TOKEN_DIRECTIVE_POPO  */
  YYSYMBOL_TOKEN_DIRECTIVE_OPT = 153,      /* TOKEN_DIRECTIVE_OPT  */
  YYSYMBOL_TOKEN_DIRECTIVE_PUSHP = 154,    /* TOKEN_DIRECTIVE_PUSHP  */
  YYSYMBOL_TOKEN_DIRECTIVE_POPP = 155,     /* TOKEN_DIRECTIVE_POPP  */
  YYSYMBOL_TOKEN_DIRECTIVE_SHIFT = 156,    /* TOKEN_DIRECTIVE_SHIFT  */
  YYSYMBOL_TOKEN_DIRECTIVE_MEXIT = 157,    /* TOKEN_DIRECTIVE_MEXIT  */
  YYSYMBOL_TOKEN_DIRECTIVE_LOCAL = 158,    /* TOKEN_DIRECTIVE_LOCAL  */
  YYSYMBOL_TOKEN_DIRECTIVE_LIST = 159,     /* TOKEN_DIRECTIVE_LIST  */
  YYSYMBOL_TOKEN_DIRECTIVE_NOLIST = 160,   /* TOKEN_DIRECTIVE_NOLIST  */
  YYSYMBOL_TOKEN_DIRECTIVE_PURGE = 161,    /* TOKEN_DIRECTIVE_PURGE  */
  YYSYMBOL_TOKEN_SIZE_BYTE = 162,          /* TOKEN_SIZE_BYTE  */
  YYSYMBOL_TOKEN_SIZE_SHORT = 163,         /* TOKEN_SIZE_SHORT  */
  YYSYMBOL_TOKEN_SIZE_WORD = 164,          /* TOKEN_SIZE_WORD  */
  YYSYMBOL_TOKEN_SIZE_LONGWORD = 165,      /* TOKEN_SIZE_LONGWORD  */
  YYSYMBOL_TOKEN_DATA_REGISTER = 166,      /* TOKEN_DATA_REGISTER  */
  YYSYMBOL_TOKEN_ADDRESS_REGISTER = 167,   /* TOKEN_ADDRESS_REGISTER  */
  YYSYMBOL_TOKEN_NUMBER = 168,             /* TOKEN_NUMBER  */
  YYSYMBOL_TOKEN_IDENTIFIER = 169,         /* TOKEN_IDENTIFIER  */
  YYSYMBOL_TOKEN_LOCAL_IDENTIFIER = 170,   /* TOKEN_LOCAL_IDENTIFIER  */
  YYSYMBOL_TOKEN_STRING = 171,             /* TOKEN_STRING  */
  YYSYMBOL_TOKEN_OPTION = 172,             /* TOKEN_OPTION  */
  YYSYMBOL_TOKEN_STATUS_REGISTER = 173,    /* TOKEN_STATUS_REGISTER  */
  YYSYMBOL_TOKEN_CONDITION_CODE_REGISTER = 174, /* TOKEN_CONDITION_CODE_REGISTER  */
  YYSYMBOL_TOKEN_USER_STACK_POINTER_REGISTER = 175, /* TOKEN_USER_STACK_POINTER_REGISTER  */
  YYSYMBOL_TOKEN_PROGRAM_COUNTER = 176,    /* TOKEN_PROGRAM_COUNTER  */
  YYSYMBOL_TOKEN_LOGICAL_AND = 177,        /* TOKEN_LOGICAL_AND  */
  YYSYMBOL_TOKEN_LOGICAL_OR = 178,         /* TOKEN_LOGICAL_OR  */
  YYSYMBOL_TOKEN_EQUALITY = 179,           /* TOKEN_EQUALITY  */
  YYSYMBOL_TOKEN_INEQUALITY = 180,         /* TOKEN_INEQUALITY  */
  YYSYMBOL_TOKEN_LESS_OR_EQUAL = 181,      /* TOKEN_LESS_OR_EQUAL  */
  YYSYMBOL_TOKEN_MORE_OR_EQUAL = 182,      /* TOKEN_MORE_OR_EQUAL  */
  YYSYMBOL_TOKEN_LEFT_SHIFT = 183,         /* TOKEN_LEFT_SHIFT  */
  YYSYMBOL_TOKEN_RIGHT_SHIFT = 184,        /* TOKEN_RIGHT_SHIFT  */
  YYSYMBOL_TOKEN_STRLEN = 185,             /* TOKEN_STRLEN  */
  YYSYMBOL_TOKEN_STRCMP = 186,             /* TOKEN_STRCMP  */
  YYSYMBOL_TOKEN_INSTR = 187,              /* TOKEN_INSTR  */
  YYSYMBOL_TOKEN_DEF = 188,                /* TOKEN_DEF  */
  YYSYMBOL_TOKEN_TYPE = 189,               /* TOKEN_TYPE  */
  YYSYMBOL_TOKEN_FILESIZE = 190,           /* TOKEN_FILESIZE  */
  YYSYMBOL_TOKEN_OFFSET = 191,             /* TOKEN_OFFSET  */
  YYSYMBOL_TOKEN_LOWER_PRECEDENCE_THAN_SIZE = 192, /* TOKEN_LOWER_PRECEDENCE_THAN_SIZE  */
  YYSYMBOL_TOKEN_HIGHER_PRECEDENCE_THAN_SIZE = 193, /* TOKEN_HIGHER_PRECEDENCE_THAN_SIZE  */
  YYSYMBOL_194_ = 194,                     /* ','  */
  YYSYMBOL_195_ = 195,                     /* '*'  */
  YYSYMBOL_196_ = 196,                     /* '='  */
  YYSYMBOL_197_ = 197,                     /* '+'  */
  YYSYMBOL_198_ = 198,                     /* '-'  */
  YYSYMBOL_199_ = 199,                     /* '.'  */
  YYSYMBOL_200_ = 200,                     /* '('  */
  YYSYMBOL_201_ = 201,                     /* ')'  */
  YYSYMBOL_202_ = 202,                     /* '#'  */
  YYSYMBOL_203_ = 203,                     /* '/'  */
  YYSYMBOL_204_ = 204,                     /* '<'  */
  YYSYMBOL_205_ = 205,                     /* '>'  */
  YYSYMBOL_206_ = 206,                     /* '%'  */
  YYSYMBOL_207_ = 207,                     /* '&'  */
  YYSYMBOL_208_ = 208,                     /* '!'  */
  YYSYMBOL_209_ = 209,                     /* '|'  */
  YYSYMBOL_210_ = 210,                     /* '^'  */
  YYSYMBOL_211_ = 211,                     /* '~'  */
  YYSYMBOL_212_ = 212,                     /* '@'  */
  YYSYMBOL_YYACCEPT = 213,                 /* $accept  */
  YYSYMBOL_statement = 214,                /* statement  */
  YYSYMBOL_expression_list = 215,          /* expression_list  */
  YYSYMBOL_identifier_list = 216,          /* identifier_list  */
  YYSYMBOL_option_list = 217,              /* option_list  */
  YYSYMBOL_instruction = 218,              /* instruction  */
  YYSYMBOL_full_opcode = 219,              /* full_opcode  */
  YYSYMBOL_opcode = 220,                   /* opcode  */
  YYSYMBOL_size = 221,                     /* size  */
  YYSYMBOL_operand = 222,                  /* operand  */
  YYSYMBOL_register_list = 223,            /* register_list  */
  YYSYMBOL_register_span = 224,            /* register_span  */
  YYSYMBOL_data_or_address_register = 225, /* data_or_address_register  */
  YYSYMBOL_expression = 226,               /* expression  */
  YYSYMBOL_expression1 = 227,              /* expression1  */
  YYSYMBOL_expression2 = 228,              /* expression2  */
  YYSYMBOL_expression3 = 229,              /* expression3  */
  YYSYMBOL_expression4 = 230,              /* expression4  */
  YYSYMBOL_expression5 = 231,              /* expression5  */
  YYSYMBOL_expression6 = 232,              /* expression6  */
  YYSYMBOL_expression7 = 233,              /* expression7  */
  YYSYMBOL_local_identifier = 234,         /* local_identifier  */
  YYSYMBOL_identifier = 235,               /* identifier  */
  YYSYMBOL_expression8 = 236,              /* expression8  */
  YYSYMBOL_string = 237                    /* string  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;



/* Unqualified %code blocks.  */
#line 463 "syntactic.y"


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


#line 367 "syntactic.c"

#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
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


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined M68KASM_STYPE_IS_TRIVIAL && M68KASM_STYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  237
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   863

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  213
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  285
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  428

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   448


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   208,     2,   202,     2,   206,   207,     2,
     200,   201,   195,   197,   194,   198,   199,   203,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     204,   196,   205,     2,   212,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   210,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   209,     2,   211,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193
};

#if M68KASM_DEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   716,   716,   719,   724,   730,   737,   743,   748,   756,
     763,   772,   780,   785,   789,   796,   803,   809,   815,   822,
     828,   832,   837,   842,   847,   856,   864,   872,   879,   884,
     889,   894,   899,   903,   907,   911,   916,   920,   924,   930,
     936,   946,   950,   954,   960,   965,   972,   976,   981,   985,
     990,   994,   998,  1003,  1008,  1013,  1017,  1021,  1026,  1031,
    1036,  1041,  1046,  1054,  1071,  1099,  1116,  1144,  1161,  1189,
    1195,  1201,  1210,  1215,  1221,  1229,  1233,  1237,  1241,  1245,
    1249,  1253,  1257,  1261,  1265,  1269,  1273,  1277,  1281,  1285,
    1289,  1293,  1297,  1301,  1305,  1309,  1313,  1317,  1321,  1325,
    1329,  1333,  1337,  1341,  1345,  1349,  1353,  1357,  1361,  1365,
    1369,  1373,  1377,  1381,  1385,  1389,  1393,  1398,  1403,  1408,
    1413,  1418,  1423,  1428,  1433,  1438,  1443,  1448,  1453,  1458,
    1463,  1468,  1473,  1478,  1483,  1488,  1493,  1498,  1503,  1508,
    1513,  1518,  1523,  1528,  1533,  1538,  1543,  1548,  1553,  1557,
    1561,  1566,  1571,  1576,  1581,  1586,  1591,  1596,  1601,  1606,
    1611,  1616,  1621,  1626,  1631,  1635,  1639,  1643,  1647,  1651,
    1655,  1659,  1663,  1667,  1671,  1675,  1679,  1683,  1687,  1691,
    1695,  1699,  1703,  1707,  1711,  1715,  1719,  1723,  1727,  1731,
    1735,  1739,  1746,  1750,  1754,  1758,  1766,  1771,  1776,  1781,
    1787,  1793,  1799,  1809,  1818,  1827,  1838,  1848,  1858,  1863,
    1868,  1873,  1882,  1890,  1898,  1908,  1917,  1927,  1943,  1947,
    1951,  1956,  1962,  1969,  2000,  2004,  2011,  2015,  2040,  2044,
    2057,  2062,  2068,  2076,  2080,  2085,  2090,  2098,  2102,  2107,
    2112,  2117,  2125,  2129,  2134,  2142,  2146,  2151,  2156,  2164,
    2168,  2173,  2178,  2183,  2191,  2195,  2200,  2208,  2212,  2216,
    2221,  2227,  2235,  2236,  2242,  2248,  2254,  2263,  2264,  2265,
    2277,  2282,  2287,  2291,  2295,  2299,  2303,  2308,  2313,  2323,
    2328,  2333,  2338,  2343,  2348,  2356
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "TOKEN_DIRECTIVE_ORI",
  "TOKEN_DIRECTIVE_ANDI", "TOKEN_DIRECTIVE_SUBI", "TOKEN_DIRECTIVE_ADDI",
  "TOKEN_DIRECTIVE_EORI", "TOKEN_DIRECTIVE_CMPI", "TOKEN_DIRECTIVE_BTST",
  "TOKEN_DIRECTIVE_BCHG", "TOKEN_DIRECTIVE_BCLR", "TOKEN_DIRECTIVE_BSET",
  "TOKEN_DIRECTIVE_MOVEP", "TOKEN_DIRECTIVE_MOVEA", "TOKEN_DIRECTIVE_MOVE",
  "TOKEN_DIRECTIVE_NEGX", "TOKEN_DIRECTIVE_CLR", "TOKEN_DIRECTIVE_NEG",
  "TOKEN_DIRECTIVE_NOT", "TOKEN_DIRECTIVE_EXT", "TOKEN_DIRECTIVE_NBCD",
  "TOKEN_DIRECTIVE_SWAP", "TOKEN_DIRECTIVE_PEA", "TOKEN_DIRECTIVE_ILLEGAL",
  "TOKEN_DIRECTIVE_TAS", "TOKEN_DIRECTIVE_TST", "TOKEN_DIRECTIVE_TRAP",
  "TOKEN_DIRECTIVE_LINK", "TOKEN_DIRECTIVE_UNLK", "TOKEN_DIRECTIVE_RESET",
  "TOKEN_DIRECTIVE_NOP", "TOKEN_DIRECTIVE_STOP", "TOKEN_DIRECTIVE_RTE",
  "TOKEN_DIRECTIVE_RTS", "TOKEN_DIRECTIVE_TRAPV", "TOKEN_DIRECTIVE_RTR",
  "TOKEN_DIRECTIVE_JSR", "TOKEN_DIRECTIVE_JMP", "TOKEN_DIRECTIVE_MOVEM",
  "TOKEN_DIRECTIVE_LEA", "TOKEN_DIRECTIVE_CHK", "TOKEN_DIRECTIVE_ADDQ",
  "TOKEN_DIRECTIVE_SUBQ", "TOKEN_DIRECTIVE_ST", "TOKEN_DIRECTIVE_SF",
  "TOKEN_DIRECTIVE_SHI", "TOKEN_DIRECTIVE_SLS", "TOKEN_DIRECTIVE_SCC",
  "TOKEN_DIRECTIVE_SCS", "TOKEN_DIRECTIVE_SNE", "TOKEN_DIRECTIVE_SEQ",
  "TOKEN_DIRECTIVE_SVC", "TOKEN_DIRECTIVE_SVS", "TOKEN_DIRECTIVE_SPL",
  "TOKEN_DIRECTIVE_SMI", "TOKEN_DIRECTIVE_SGE", "TOKEN_DIRECTIVE_SLT",
  "TOKEN_DIRECTIVE_SGT", "TOKEN_DIRECTIVE_SLE", "TOKEN_DIRECTIVE_DBT",
  "TOKEN_DIRECTIVE_DBF", "TOKEN_DIRECTIVE_DBHI", "TOKEN_DIRECTIVE_DBLS",
  "TOKEN_DIRECTIVE_DBCC", "TOKEN_DIRECTIVE_DBCS", "TOKEN_DIRECTIVE_DBNE",
  "TOKEN_DIRECTIVE_DBEQ", "TOKEN_DIRECTIVE_DBVC", "TOKEN_DIRECTIVE_DBVS",
  "TOKEN_DIRECTIVE_DBPL", "TOKEN_DIRECTIVE_DBMI", "TOKEN_DIRECTIVE_DBGE",
  "TOKEN_DIRECTIVE_DBLT", "TOKEN_DIRECTIVE_DBGT", "TOKEN_DIRECTIVE_DBLE",
  "TOKEN_DIRECTIVE_BRA", "TOKEN_DIRECTIVE_BSR", "TOKEN_DIRECTIVE_BHI",
  "TOKEN_DIRECTIVE_BLS", "TOKEN_DIRECTIVE_BCC", "TOKEN_DIRECTIVE_BCS",
  "TOKEN_DIRECTIVE_BNE", "TOKEN_DIRECTIVE_BEQ", "TOKEN_DIRECTIVE_BVC",
  "TOKEN_DIRECTIVE_BVS", "TOKEN_DIRECTIVE_BPL", "TOKEN_DIRECTIVE_BMI",
  "TOKEN_DIRECTIVE_BGE", "TOKEN_DIRECTIVE_BLT", "TOKEN_DIRECTIVE_BGT",
  "TOKEN_DIRECTIVE_BLE", "TOKEN_DIRECTIVE_MOVEQ", "TOKEN_DIRECTIVE_DIVU",
  "TOKEN_DIRECTIVE_DIVS", "TOKEN_DIRECTIVE_SBCD", "TOKEN_DIRECTIVE_OR",
  "TOKEN_DIRECTIVE_SUB", "TOKEN_DIRECTIVE_SUBX", "TOKEN_DIRECTIVE_SUBA",
  "TOKEN_DIRECTIVE_EOR", "TOKEN_DIRECTIVE_CMPM", "TOKEN_DIRECTIVE_CMP",
  "TOKEN_DIRECTIVE_CMPA", "TOKEN_DIRECTIVE_MULU", "TOKEN_DIRECTIVE_MULS",
  "TOKEN_DIRECTIVE_ABCD", "TOKEN_DIRECTIVE_EXG", "TOKEN_DIRECTIVE_AND",
  "TOKEN_DIRECTIVE_ADD", "TOKEN_DIRECTIVE_ADDX", "TOKEN_DIRECTIVE_ADDA",
  "TOKEN_DIRECTIVE_ASL", "TOKEN_DIRECTIVE_ASR", "TOKEN_DIRECTIVE_LSL",
  "TOKEN_DIRECTIVE_LSR", "TOKEN_DIRECTIVE_ROXL", "TOKEN_DIRECTIVE_ROXR",
  "TOKEN_DIRECTIVE_ROL", "TOKEN_DIRECTIVE_ROR", "TOKEN_DIRECTIVE_DC",
  "TOKEN_DIRECTIVE_DCB", "TOKEN_DIRECTIVE_DS", "TOKEN_DIRECTIVE_REPT",
  "TOKEN_DIRECTIVE_ENDR", "TOKEN_DIRECTIVE_MACRO",
  "TOKEN_DIRECTIVE_MACROS", "TOKEN_DIRECTIVE_ENDM",
  "TOKEN_DIRECTIVE_INCLUDE", "TOKEN_DIRECTIVE_INCBIN",
  "TOKEN_DIRECTIVE_EQU", "TOKEN_DIRECTIVE_EQUS", "TOKEN_DIRECTIVE_SUBSTR",
  "TOKEN_DIRECTIVE_SET", "TOKEN_DIRECTIVE_IF", "TOKEN_DIRECTIVE_ELSEIF",
  "TOKEN_DIRECTIVE_ELSE", "TOKEN_DIRECTIVE_ENDC", "TOKEN_DIRECTIVE_WHILE",
  "TOKEN_DIRECTIVE_ENDW", "TOKEN_DIRECTIVE_EVEN", "TOKEN_DIRECTIVE_CNOP",
  "TOKEN_DIRECTIVE_INFORM", "TOKEN_DIRECTIVE_FAIL", "TOKEN_DIRECTIVE_END",
  "TOKEN_DIRECTIVE_RS", "TOKEN_DIRECTIVE_RSSET", "TOKEN_DIRECTIVE_RSRESET",
  "TOKEN_DIRECTIVE_OBJ", "TOKEN_DIRECTIVE_OBJEND", "TOKEN_DIRECTIVE_ORG",
  "TOKEN_DIRECTIVE_PUSHO", "TOKEN_DIRECTIVE_POPO", "TOKEN_DIRECTIVE_OPT",
  "TOKEN_DIRECTIVE_PUSHP", "TOKEN_DIRECTIVE_POPP", "TOKEN_DIRECTIVE_SHIFT",
  "TOKEN_DIRECTIVE_MEXIT", "TOKEN_DIRECTIVE_LOCAL", "TOKEN_DIRECTIVE_LIST",
  "TOKEN_DIRECTIVE_NOLIST", "TOKEN_DIRECTIVE_PURGE", "TOKEN_SIZE_BYTE",
  "TOKEN_SIZE_SHORT", "TOKEN_SIZE_WORD", "TOKEN_SIZE_LONGWORD",
  "TOKEN_DATA_REGISTER", "TOKEN_ADDRESS_REGISTER", "TOKEN_NUMBER",
  "TOKEN_IDENTIFIER", "TOKEN_LOCAL_IDENTIFIER", "TOKEN_STRING",
  "TOKEN_OPTION", "TOKEN_STATUS_REGISTER", "TOKEN_CONDITION_CODE_REGISTER",
  "TOKEN_USER_STACK_POINTER_REGISTER", "TOKEN_PROGRAM_COUNTER",
  "TOKEN_LOGICAL_AND", "TOKEN_LOGICAL_OR", "TOKEN_EQUALITY",
  "TOKEN_INEQUALITY", "TOKEN_LESS_OR_EQUAL", "TOKEN_MORE_OR_EQUAL",
  "TOKEN_LEFT_SHIFT", "TOKEN_RIGHT_SHIFT", "TOKEN_STRLEN", "TOKEN_STRCMP",
  "TOKEN_INSTR", "TOKEN_DEF", "TOKEN_TYPE", "TOKEN_FILESIZE",
  "TOKEN_OFFSET", "TOKEN_LOWER_PRECEDENCE_THAN_SIZE",
  "TOKEN_HIGHER_PRECEDENCE_THAN_SIZE", "','", "'*'", "'='", "'+'", "'-'",
  "'.'", "'('", "')'", "'#'", "'/'", "'<'", "'>'", "'%'", "'&'", "'!'",
  "'|'", "'^'", "'~'", "'@'", "$accept", "statement", "expression_list",
  "identifier_list", "option_list", "instruction", "full_opcode", "opcode",
  "size", "operand", "register_list", "register_span",
  "data_or_address_register", "expression", "expression1", "expression2",
  "expression3", "expression4", "expression5", "expression6",
  "expression7", "local_identifier", "identifier", "expression8", "string", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-300)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     292,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,   105,   105,
     105,   600,  -300,  -164,  -140,  -300,  -132,  -116,   600,  -160,
     345,   600,   600,   600,  -300,  -300,   600,  -300,  -300,   600,
     600,  -300,  -300,   105,   600,   600,   600,  -300,   600,  -300,
    -300,  -104,   -80,   -75,  -300,  -300,  -140,   -43,  -300,   -73,
     600,   110,  -300,    32,   -12,  -300,  -300,  -300,  -300,   600,
     600,   600,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
     -72,   -70,   -62,   -56,   -25,   -15,    -9,  -300,   600,   600,
     600,   600,   600,  -300,    61,  -173,  -135,   -18,    58,    65,
      73,  -300,  -300,    38,  -300,  -300,  -300,    10,    16,    16,
    -300,    30,    61,  -300,  -300,   396,   -69,    61,    61,    61,
      61,    -8,    -5,   600,    61,    61,    61,    61,  -300,    39,
    -300,  -300,    16,  -300,  -300,  -300,    61,  -300,  -300,  -300,
    -300,  -300,  -300,   651,   294,   600,    42,  -300,    23,    49,
     -16,  -300,  -300,    60,    61,    34,    61,    94,   100,   600,
      44,    83,   283,   600,  -300,  -300,  -134,  -300,  -300,   600,
     600,   600,   600,   600,   600,   600,   600,   600,   600,   600,
     600,   600,   600,   600,   600,   600,   600,   600,   600,  -300,
    -140,   286,   447,   289,    37,   498,   600,   298,    61,   302,
     549,  -169,   281,   -65,    61,    32,   118,   118,   -88,  -300,
     600,   600,   275,   284,    64,   276,   285,   295,   296,  -128,
    -300,  -173,  -173,  -135,  -135,  -135,   -18,   -18,   -18,   -18,
      58,    58,    65,    65,    65,    73,    73,    73,    73,  -300,
    -300,    16,  -300,   600,    68,  -300,   316,   319,    72,    61,
     299,  -300,   297,  -105,   303,  -105,     0,  -300,  -300,  -300,
    -161,  -153,    61,    61,  -300,   100,   600,  -300,  -300,  -300,
    -300,  -300,    61,   600,  -300,  -300,   324,   600,  -300,  -127,
     -38,  -300,   -87,   -20,   -11,    15,   118,  -300,   118,  -300,
     300,   -35,    61,  -300,    60,  -300,   310,  -300,  -300,   311,
    -300,   118,  -300,   118,  -300,   -64,   -45,  -300,   600,  -300,
    -300,  -300,   -41,   -30,  -300,   317,  -300,   318,   -13,  -300,
     320,  -300,   321,  -300,  -300,  -300,  -300,  -300
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       2,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,     0,     0,
       0,     0,    13,    14,    18,    20,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     0,    36,    37,     0,
       0,    41,    42,     0,     0,    46,     0,    48,     0,    50,
      51,     0,     0,     0,    55,    56,     0,    58,    61,     0,
       0,     0,     3,    69,    72,   192,   193,   194,   195,     0,
       0,     0,   263,   264,   265,   266,   270,   267,   262,   285,
       0,     0,     0,     0,     0,     0,     0,   273,     0,     0,
       0,     0,     0,   274,    12,   230,   233,   237,   242,   245,
     249,   254,   268,   271,   257,   272,    65,    15,    16,    19,
       7,     8,    21,    23,    22,     0,     0,    28,    30,    31,
      35,     0,     0,     0,    44,    45,    47,    49,    67,    52,
      53,    54,    57,    59,    60,    62,    29,     1,   228,   229,
     218,   219,   220,     0,     0,     0,    70,   223,   224,   226,
     221,    73,    74,     4,    63,     0,     6,     0,     0,     0,
       0,     0,     0,     0,   258,   259,     0,   261,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   269,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,   217,     0,     0,     0,     0,   222,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     275,   231,   232,   235,   236,   234,   239,   241,   238,   240,
     243,   244,   246,   247,   248,   250,   251,   252,   253,   255,
     256,    17,    66,     0,     9,    24,     0,     0,     0,    38,
      39,    68,     0,     0,   196,     0,     0,    71,   225,   227,
       0,     0,    64,     5,   276,     0,     0,   280,   282,   281,
     283,   284,    10,     0,    26,    25,     0,     0,   198,     0,
       0,   197,     0,     0,     0,     0,     0,   199,     0,   208,
       0,     0,    11,    27,    40,   205,     0,   201,   214,     0,
     210,     0,   200,     0,   209,     0,     0,   277,     0,   278,
     202,   211,     0,     0,   206,     0,   215,     0,     0,   207,
       0,   216,     0,   203,   212,   279,   204,   213
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -300,  -300,   122,  -122,  -300,  -300,  -300,  -300,  -119,   198,
     211,  -300,  -299,  -118,    19,   195,     2,    13,   191,    -3,
    -172,   301,  -300,  -300,  -254
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,   161,   253,   208,   229,   162,   163,   164,   169,   246,
     247,   248,   249,   250,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     170,   171,   209,   194,   313,   206,   271,   272,   359,   213,
     212,   214,   216,   217,   218,   219,   264,   265,   220,   267,
     268,   221,   222,   273,   223,   353,   224,   225,   226,   206,
     227,   207,   354,   386,   232,   165,   166,   167,   168,   210,
     387,   388,   236,   269,   270,   252,   274,   275,   389,   269,
     270,   254,   255,   256,   379,   211,   382,   172,   173,   174,
     175,   238,   239,   176,   177,   178,   179,   320,   228,   276,
     277,   265,   266,   371,   395,   165,   166,   167,   168,   360,
     180,   181,   182,   183,   184,   185,   186,   405,   361,   406,
     187,   230,   188,   189,   231,   190,   235,   294,   165,   166,
     167,   168,   412,   191,   413,   298,   192,   193,   269,   270,
     237,   390,   269,   270,   398,   339,   340,   165,   166,   167,
     168,   165,   166,   167,   168,   295,   303,   304,   257,   356,
     258,   309,   165,   166,   167,   168,   320,   414,   259,   269,
     270,   314,   269,   270,   260,   319,   165,   166,   167,   168,
     165,   166,   167,   168,   233,   234,   416,   269,   270,   408,
     419,   269,   270,   397,   269,   270,   409,   384,   341,   269,
     270,   421,   269,   270,   344,   261,   385,   348,   349,   278,
     279,   400,   266,   401,   308,   262,   296,   251,   425,   297,
     402,   263,   362,   363,   172,   173,   174,   175,   238,   239,
     176,   177,   178,   179,   290,   240,   241,   242,   178,   403,
     291,   269,   270,   315,   269,   270,   404,   180,   181,   182,
     183,   184,   185,   186,   292,   372,   306,   187,   311,   188,
     243,   346,   244,   299,   245,   380,   305,   383,   269,   270,
     191,   269,   270,   192,   193,   269,   270,   307,   391,   269,
     270,   316,   317,   280,   310,   392,   287,   288,   366,   254,
     396,   281,   373,   399,   282,   312,   376,   165,   166,   167,
     168,   179,   283,   284,   285,   286,   326,   327,   328,   329,
     335,   336,   337,   338,   238,   239,   415,   417,   321,   322,
     418,   330,   331,   420,   422,     1,     2,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   318,   342,   172,   173,   174,   175,
     345,   301,   176,   177,   178,   179,   323,   324,   325,   350,
     302,   332,   333,   334,   351,   355,   364,   367,   365,   180,
     181,   182,   183,   184,   185,   186,   368,   374,   160,   187,
     375,   188,   189,   377,   190,   393,   369,   370,   378,   394,
     381,   407,   191,   357,   289,   192,   193,   172,   173,   174,
     175,   410,   411,   176,   177,   178,   179,   358,   423,   424,
       0,   426,   427,     0,     0,     0,     0,     0,     0,     0,
     180,   181,   182,   183,   184,   185,   186,     0,     0,   215,
     187,     0,   188,   189,     0,   190,     0,     0,     0,     0,
       0,     0,     0,   191,     0,     0,   192,   193,   172,   173,
     174,   175,     0,     0,   176,   177,   178,   179,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,   183,   184,   185,   186,     0,     0,
     293,   187,     0,   188,   189,     0,   190,     0,     0,     0,
       0,     0,     0,     0,   191,     0,     0,   192,   193,   172,
     173,   174,   175,     0,     0,   176,   177,   178,   179,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,   183,   184,   185,   186,     0,
       0,   343,   187,     0,   188,   189,     0,   190,     0,     0,
       0,     0,     0,     0,     0,   191,     0,     0,   192,   193,
     172,   173,   174,   175,     0,     0,   176,   177,   178,   179,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,   183,   184,   185,   186,
       0,     0,   347,   187,     0,   188,   189,     0,   190,     0,
       0,     0,     0,     0,     0,     0,   191,     0,     0,   192,
     193,   172,   173,   174,   175,     0,   352,   176,   177,   178,
     179,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   180,   181,   182,   183,   184,   185,
     186,     0,     0,     0,   187,     0,   188,   189,     0,   190,
       0,     0,     0,     0,     0,     0,     0,   191,     0,     0,
     192,   193,   172,   173,   174,   175,     0,     0,   176,   177,
     178,   179,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   180,   181,   182,   183,   184,
     185,   186,     0,     0,     0,   187,     0,   188,   189,     0,
     190,     0,     0,     0,     0,     0,     0,     0,   191,     0,
       0,   192,   193,   172,   173,   174,   175,     0,     0,   176,
     177,   178,   179,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   180,   181,   182,   183,
     184,   185,   186,     0,     0,     0,   187,     0,   188,   189,
       0,   300,     0,     0,     0,     0,     0,     0,     0,   191,
       0,     0,   192,   193
};

static const yytype_int16 yycheck[] =
{
     119,   120,   124,   121,   258,   169,   179,   180,   307,   169,
     128,   171,   130,   131,   132,   133,   188,   189,   136,   191,
     192,   139,   140,   196,   143,   194,   144,   145,   146,   169,
     148,   195,   201,   194,   156,   162,   163,   164,   165,   171,
     201,   194,   160,   177,   178,   164,   181,   182,   201,   177,
     178,   169,   170,   171,   353,   171,   355,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   201,   172,   204,
     205,   243,   190,   201,   201,   162,   163,   164,   165,   167,
     185,   186,   187,   188,   189,   190,   191,   386,   176,   388,
     195,   171,   197,   198,   169,   200,   169,   215,   162,   163,
     164,   165,   401,   208,   403,   223,   211,   212,   177,   178,
       0,   365,   177,   178,   201,   287,   288,   162,   163,   164,
     165,   162,   163,   164,   165,   194,   244,   245,   200,   194,
     200,   250,   162,   163,   164,   165,   201,   201,   200,   177,
     178,   259,   177,   178,   200,   263,   162,   163,   164,   165,
     162,   163,   164,   165,   197,   198,   201,   177,   178,   194,
     201,   177,   178,   201,   177,   178,   201,   167,   290,   177,
     178,   201,   177,   178,   292,   200,   176,   295,   296,   197,
     198,   201,   300,   194,   200,   200,   194,   199,   201,   194,
     201,   200,   310,   311,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   194,   173,   174,   175,   170,   194,
     194,   177,   178,   169,   177,   178,   201,   185,   186,   187,
     188,   189,   190,   191,   194,   343,   203,   195,   194,   197,
     198,   194,   200,   194,   202,   353,   194,   355,   177,   178,
     208,   177,   178,   211,   212,   177,   178,   198,   366,   177,
     178,   168,   169,   195,   194,   373,   183,   184,   194,   377,
     379,   203,   194,   382,   206,   171,   194,   162,   163,   164,
     165,   171,   207,   208,   209,   210,   274,   275,   276,   277,
     283,   284,   285,   286,   166,   167,   405,   406,   269,   270,
     408,   278,   279,   412,   413,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   171,   169,   162,   163,   164,   165,
     171,   167,   168,   169,   170,   171,   271,   272,   273,   171,
     176,   280,   281,   282,   172,   194,   201,   201,   194,   185,
     186,   187,   188,   189,   190,   191,   201,   171,   196,   195,
     171,   197,   198,   194,   200,   171,   201,   201,   201,   377,
     197,   201,   208,   305,   203,   211,   212,   162,   163,   164,
     165,   201,   201,   168,   169,   170,   171,   306,   201,   201,
      -1,   201,   201,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     185,   186,   187,   188,   189,   190,   191,    -1,    -1,   194,
     195,    -1,   197,   198,    -1,   200,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   208,    -1,    -1,   211,   212,   162,   163,
     164,   165,    -1,    -1,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,   187,   188,   189,   190,   191,    -1,    -1,
     194,   195,    -1,   197,   198,    -1,   200,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   208,    -1,    -1,   211,   212,   162,
     163,   164,   165,    -1,    -1,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,   187,   188,   189,   190,   191,    -1,
      -1,   194,   195,    -1,   197,   198,    -1,   200,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,   211,   212,
     162,   163,   164,   165,    -1,    -1,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,   194,   195,    -1,   197,   198,    -1,   200,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,   211,
     212,   162,   163,   164,   165,    -1,   167,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,   187,   188,   189,   190,
     191,    -1,    -1,    -1,   195,    -1,   197,   198,    -1,   200,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,
     211,   212,   162,   163,   164,   165,    -1,    -1,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,   187,   188,   189,
     190,   191,    -1,    -1,    -1,   195,    -1,   197,   198,    -1,
     200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,    -1,
      -1,   211,   212,   162,   163,   164,   165,    -1,    -1,   168,
     169,   170,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   185,   186,   187,   188,
     189,   190,   191,    -1,    -1,    -1,   195,    -1,   197,   198,
      -1,   200,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   208,
      -1,    -1,   211,   212
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     196,   214,   218,   219,   220,   162,   163,   164,   165,   221,
     221,   221,   162,   163,   164,   165,   168,   169,   170,   171,
     185,   186,   187,   188,   189,   190,   191,   195,   197,   198,
     200,   208,   211,   212,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   235,   236,   237,   169,   195,   216,   216,
     171,   171,   226,   169,   171,   194,   226,   226,   226,   226,
     226,   226,   226,   221,   226,   226,   226,   226,   172,   217,
     171,   169,   216,   197,   198,   169,   226,     0,   166,   167,
     173,   174,   175,   198,   200,   202,   222,   223,   224,   225,
     226,   199,   221,   215,   226,   226,   226,   200,   200,   200,
     200,   200,   200,   200,   233,   233,   226,   233,   233,   177,
     178,   179,   180,   196,   181,   182,   204,   205,   197,   198,
     195,   203,   206,   207,   208,   209,   210,   183,   184,   234,
     194,   194,   194,   194,   226,   194,   194,   194,   226,   194,
     200,   167,   176,   226,   226,   194,   203,   198,   200,   221,
     194,   194,   171,   237,   226,   169,   168,   169,   171,   226,
     201,   227,   227,   228,   228,   228,   229,   229,   229,   229,
     230,   230,   231,   231,   231,   232,   232,   232,   232,   233,
     233,   216,   169,   194,   226,   171,   194,   194,   226,   226,
     171,   172,   167,   194,   201,   194,   194,   222,   223,   225,
     167,   176,   226,   226,   201,   194,   194,   201,   201,   201,
     201,   201,   226,   194,   171,   171,   194,   194,   201,   225,
     226,   197,   225,   226,   167,   176,   194,   201,   194,   201,
     237,   226,   226,   171,   215,   201,   221,   201,   201,   221,
     201,   194,   201,   194,   201,   225,   225,   201,   194,   201,
     201,   201,   225,   225,   201,   221,   201,   221,   226,   201,
     221,   201,   221,   201,   201,   201,   201,   201
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   213,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     214,   214,   214,   215,   215,   216,   216,   217,   217,   218,
     218,   218,   219,   219,   219,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   220,   220,
     220,   220,   221,   221,   221,   221,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   222,   222,   222,   222,   222,   222,
     222,   222,   222,   222,   223,   223,   224,   224,   225,   225,
     226,   226,   226,   227,   227,   227,   227,   228,   228,   228,
     228,   228,   229,   229,   229,   230,   230,   230,   230,   231,
     231,   231,   231,   231,   232,   232,   232,   233,   233,   233,
     233,   233,   234,   234,   234,   234,   234,   235,   235,   235,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   237
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     1,     3,     5,     3,     2,     2,     4,
       5,     6,     2,     1,     1,     2,     2,     4,     1,     2,
       1,     2,     2,     2,     4,     5,     5,     6,     2,     2,
       2,     2,     1,     1,     1,     2,     1,     1,     4,     4,
       6,     1,     1,     3,     2,     2,     1,     2,     1,     2,
       1,     1,     2,     2,     2,     1,     1,     2,     1,     2,
       2,     1,     2,     1,     3,     1,     3,     1,     3,     1,
       2,     4,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     4,     4,     4,
       5,     5,     6,     7,     8,     5,     6,     7,     4,     5,
       5,     6,     7,     8,     5,     6,     7,     2,     1,     1,
       1,     1,     2,     1,     1,     3,     1,     3,     1,     1,
       1,     3,     3,     1,     3,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       3,     3,     3,     3,     1,     3,     3,     1,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     3,     4,     6,     6,     8,
       4,     4,     4,     4,     4,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = M68KASM_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == M68KASM_EMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (scanner, statement, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use M68KASM_error or M68KASM_UNDEF. */
#define YYERRCODE M68KASM_UNDEF


/* Enable debugging if requested.  */
#if M68KASM_DEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, scanner, statement); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *scanner, Statement *statement)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (scanner);
  YY_USE (statement);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *scanner, Statement *statement)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, scanner, statement);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, void *scanner, Statement *statement)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], scanner, statement);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, scanner, statement); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !M68KASM_DEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !M68KASM_DEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, void *scanner, Statement *statement)
{
  YY_USE (yyvaluep);
  YY_USE (scanner);
  YY_USE (statement);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_TOKEN_IDENTIFIER: /* TOKEN_IDENTIFIER  */
#line 699 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1794 "syntactic.c"
        break;

    case YYSYMBOL_TOKEN_LOCAL_IDENTIFIER: /* TOKEN_LOCAL_IDENTIFIER  */
#line 699 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1800 "syntactic.c"
        break;

    case YYSYMBOL_TOKEN_STRING: /* TOKEN_STRING  */
#line 699 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1806 "syntactic.c"
        break;

    case YYSYMBOL_TOKEN_OPTION: /* TOKEN_OPTION  */
#line 699 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1812 "syntactic.c"
        break;

    case YYSYMBOL_expression_list: /* expression_list  */
#line 701 "syntactic.y"
            { DestroyExpressionList(&((*yyvaluep).expression_list)); }
#line 1818 "syntactic.c"
        break;

    case YYSYMBOL_identifier_list: /* identifier_list  */
#line 702 "syntactic.y"
            { DestroyIdentifierList(&((*yyvaluep).identifier_list)); }
#line 1824 "syntactic.c"
        break;

    case YYSYMBOL_option_list: /* option_list  */
#line 702 "syntactic.y"
            { DestroyIdentifierList(&((*yyvaluep).identifier_list)); }
#line 1830 "syntactic.c"
        break;

    case YYSYMBOL_operand: /* operand  */
#line 700 "syntactic.y"
            { DestroyOperand(&((*yyvaluep).operand)); }
#line 1836 "syntactic.c"
        break;

    case YYSYMBOL_expression: /* expression  */
#line 703 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1842 "syntactic.c"
        break;

    case YYSYMBOL_expression1: /* expression1  */
#line 703 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1848 "syntactic.c"
        break;

    case YYSYMBOL_expression2: /* expression2  */
#line 703 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1854 "syntactic.c"
        break;

    case YYSYMBOL_expression3: /* expression3  */
#line 703 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1860 "syntactic.c"
        break;

    case YYSYMBOL_expression4: /* expression4  */
#line 703 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1866 "syntactic.c"
        break;

    case YYSYMBOL_expression5: /* expression5  */
#line 703 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1872 "syntactic.c"
        break;

    case YYSYMBOL_expression6: /* expression6  */
#line 703 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1878 "syntactic.c"
        break;

    case YYSYMBOL_expression7: /* expression7  */
#line 703 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1884 "syntactic.c"
        break;

    case YYSYMBOL_local_identifier: /* local_identifier  */
#line 699 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1890 "syntactic.c"
        break;

    case YYSYMBOL_identifier: /* identifier  */
#line 699 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1896 "syntactic.c"
        break;

    case YYSYMBOL_expression8: /* expression8  */
#line 703 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1902 "syntactic.c"
        break;

    case YYSYMBOL_string: /* string  */
#line 703 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1908 "syntactic.c"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *scanner, Statement *statement)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = M68KASM_EMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == M68KASM_EMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, scanner);
    }

  if (yychar <= M68KASM_EOF)
    {
      yychar = M68KASM_EOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == M68KASM_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = M68KASM_UNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = M68KASM_EMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* statement: %empty  */
#line 716 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 2189 "syntactic.c"
    break;

  case 3: /* statement: instruction  */
#line 720 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INSTRUCTION;
		statement->shared.instruction = (yyvsp[0].instruction);
	}
#line 2198 "syntactic.c"
    break;

  case 4: /* statement: TOKEN_DIRECTIVE_DC size expression_list  */
#line 725 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DC;
		statement->shared.dc.size = (yyvsp[-1].size);
		statement->shared.dc.values = (yyvsp[0].expression_list);
	}
#line 2208 "syntactic.c"
    break;

  case 5: /* statement: TOKEN_DIRECTIVE_DCB size expression ',' expression  */
#line 731 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DCB;
		statement->shared.dcb.size = (yyvsp[-3].size);
		statement->shared.dcb.repetitions = (yyvsp[-2].expression);
		statement->shared.dcb.value = (yyvsp[0].expression);
	}
#line 2219 "syntactic.c"
    break;

  case 6: /* statement: TOKEN_DIRECTIVE_DS size expression  */
#line 738 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DS;
		statement->shared.ds.size = (yyvsp[-1].size);
		statement->shared.ds.length = (yyvsp[0].expression);
	}
#line 2229 "syntactic.c"
    break;

  case 7: /* statement: TOKEN_DIRECTIVE_INCLUDE TOKEN_STRING  */
#line 744 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCLUDE;
		statement->shared.include.path = (yyvsp[0].string);
	}
#line 2238 "syntactic.c"
    break;

  case 8: /* statement: TOKEN_DIRECTIVE_INCBIN TOKEN_STRING  */
#line 749 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yyvsp[0].string);
		statement->shared.incbin.start.type = EXPRESSION_NUMBER;
		statement->shared.incbin.start.shared.unsigned_long = 0;
		statement->shared.incbin.has_length = cc_false;
	}
#line 2250 "syntactic.c"
    break;

  case 9: /* statement: TOKEN_DIRECTIVE_INCBIN TOKEN_STRING ',' expression  */
#line 757 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yyvsp[-2].string);
		statement->shared.incbin.start = (yyvsp[0].expression);
		statement->shared.incbin.has_length = cc_false;
	}
#line 2261 "syntactic.c"
    break;

  case 10: /* statement: TOKEN_DIRECTIVE_INCBIN TOKEN_STRING ',' ',' expression  */
#line 764 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yyvsp[-3].string);
		statement->shared.incbin.start.type = EXPRESSION_NUMBER;
		statement->shared.incbin.start.shared.unsigned_long = 0;
		statement->shared.incbin.has_length = cc_true;
		statement->shared.incbin.length = (yyvsp[0].expression);
	}
#line 2274 "syntactic.c"
    break;

  case 11: /* statement: TOKEN_DIRECTIVE_INCBIN TOKEN_STRING ',' expression ',' expression  */
#line 773 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yyvsp[-4].string);
		statement->shared.incbin.start = (yyvsp[-2].expression);
		statement->shared.incbin.has_length = cc_true;
		statement->shared.incbin.length = (yyvsp[0].expression);
	}
#line 2286 "syntactic.c"
    break;

  case 12: /* statement: TOKEN_DIRECTIVE_REPT expression  */
#line 781 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_REPT;
		statement->shared.rept.repetitions = (yyvsp[0].expression);
	}
#line 2295 "syntactic.c"
    break;

  case 13: /* statement: TOKEN_DIRECTIVE_ENDR  */
#line 786 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDR;
	}
#line 2303 "syntactic.c"
    break;

  case 14: /* statement: TOKEN_DIRECTIVE_MACRO  */
#line 790 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 2314 "syntactic.c"
    break;

  case 15: /* statement: TOKEN_DIRECTIVE_MACRO '*'  */
#line 797 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_true;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 2325 "syntactic.c"
    break;

  case 16: /* statement: TOKEN_DIRECTIVE_MACRO identifier_list  */
#line 804 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names = (yyvsp[0].identifier_list);
	}
#line 2335 "syntactic.c"
    break;

  case 17: /* statement: TOKEN_DIRECTIVE_MACRO '*' ',' identifier_list  */
#line 810 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_true;
		statement->shared.macro.parameter_names = (yyvsp[0].identifier_list);
	}
#line 2345 "syntactic.c"
    break;

  case 18: /* statement: TOKEN_DIRECTIVE_MACROS  */
#line 816 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 2356 "syntactic.c"
    break;

  case 19: /* statement: TOKEN_DIRECTIVE_MACROS identifier_list  */
#line 823 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names = (yyvsp[0].identifier_list);
	}
#line 2366 "syntactic.c"
    break;

  case 20: /* statement: TOKEN_DIRECTIVE_ENDM  */
#line 829 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDM;
	}
#line 2374 "syntactic.c"
    break;

  case 21: /* statement: TOKEN_DIRECTIVE_EQU expression  */
#line 833 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQU;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2383 "syntactic.c"
    break;

  case 22: /* statement: TOKEN_DIRECTIVE_EQUS TOKEN_STRING  */
#line 838 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQUS_STRING;
		statement->shared.string = (yyvsp[0].string);
	}
#line 2392 "syntactic.c"
    break;

  case 23: /* statement: TOKEN_DIRECTIVE_EQUS TOKEN_IDENTIFIER  */
#line 843 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQUS_IDENTIFIER;
		statement->shared.string = (yyvsp[0].string);
	}
#line 2401 "syntactic.c"
    break;

  case 24: /* statement: TOKEN_DIRECTIVE_SUBSTR ',' ',' TOKEN_STRING  */
#line 848 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start.type = EXPRESSION_NUMBER;
		statement->shared.substr.start.shared.unsigned_long = 1;
		statement->shared.substr.end.type = EXPRESSION_NUMBER;
		statement->shared.substr.end.shared.unsigned_long = String_Length(&(yyvsp[0].string));
		statement->shared.substr.string = (yyvsp[0].string);
	}
#line 2414 "syntactic.c"
    break;

  case 25: /* statement: TOKEN_DIRECTIVE_SUBSTR expression ',' ',' TOKEN_STRING  */
#line 857 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start = (yyvsp[-3].expression);
		statement->shared.substr.end.type = EXPRESSION_NUMBER;
		statement->shared.substr.end.shared.unsigned_long = String_Length(&(yyvsp[0].string));
		statement->shared.substr.string = (yyvsp[0].string);
	}
#line 2426 "syntactic.c"
    break;

  case 26: /* statement: TOKEN_DIRECTIVE_SUBSTR ',' expression ',' TOKEN_STRING  */
#line 865 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start.type = EXPRESSION_NUMBER;
		statement->shared.substr.start.shared.unsigned_long = 1;
		statement->shared.substr.end = (yyvsp[-2].expression);
		statement->shared.substr.string = (yyvsp[0].string);
	}
#line 2438 "syntactic.c"
    break;

  case 27: /* statement: TOKEN_DIRECTIVE_SUBSTR expression ',' expression ',' TOKEN_STRING  */
#line 873 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start = (yyvsp[-4].expression);
		statement->shared.substr.end = (yyvsp[-2].expression);
		statement->shared.substr.string = (yyvsp[0].string);
	}
#line 2449 "syntactic.c"
    break;

  case 28: /* statement: TOKEN_DIRECTIVE_SET expression  */
#line 880 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2458 "syntactic.c"
    break;

  case 29: /* statement: '=' expression  */
#line 885 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2467 "syntactic.c"
    break;

  case 30: /* statement: TOKEN_DIRECTIVE_IF expression  */
#line 890 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_IF;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2476 "syntactic.c"
    break;

  case 31: /* statement: TOKEN_DIRECTIVE_ELSEIF expression  */
#line 895 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSEIF;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2485 "syntactic.c"
    break;

  case 32: /* statement: TOKEN_DIRECTIVE_ELSEIF  */
#line 900 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSE;
	}
#line 2493 "syntactic.c"
    break;

  case 33: /* statement: TOKEN_DIRECTIVE_ELSE  */
#line 904 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSE;
	}
#line 2501 "syntactic.c"
    break;

  case 34: /* statement: TOKEN_DIRECTIVE_ENDC  */
#line 908 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDC;
	}
#line 2509 "syntactic.c"
    break;

  case 35: /* statement: TOKEN_DIRECTIVE_WHILE expression  */
#line 912 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_WHILE;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2518 "syntactic.c"
    break;

  case 36: /* statement: TOKEN_DIRECTIVE_ENDW  */
#line 917 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDW;
	}
#line 2526 "syntactic.c"
    break;

  case 37: /* statement: TOKEN_DIRECTIVE_EVEN  */
#line 921 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EVEN;
	}
#line 2534 "syntactic.c"
    break;

  case 38: /* statement: TOKEN_DIRECTIVE_CNOP expression ',' expression  */
#line 925 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_CNOP;
		statement->shared.cnop.offset = (yyvsp[-2].expression);
		statement->shared.cnop.size_boundary = (yyvsp[0].expression);
	}
#line 2544 "syntactic.c"
    break;

  case 39: /* statement: TOKEN_DIRECTIVE_INFORM expression ',' TOKEN_STRING  */
#line 931 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.severity = (yyvsp[-2].expression);
		statement->shared.inform.message = (yyvsp[0].string);
	}
#line 2554 "syntactic.c"
    break;

  case 40: /* statement: TOKEN_DIRECTIVE_INFORM expression ',' TOKEN_STRING ',' expression_list  */
#line 937 "syntactic.y"
        {
		(void)(yyvsp[0].expression_list);

		/* TODO - parameters */
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.severity = (yyvsp[-4].expression);
		statement->shared.inform.message = (yyvsp[-2].string);
		DestroyExpressionList(&(yyvsp[0].expression_list));
	}
#line 2568 "syntactic.c"
    break;

  case 41: /* statement: TOKEN_DIRECTIVE_FAIL  */
#line 947 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_FAIL;
	}
#line 2576 "syntactic.c"
    break;

  case 42: /* statement: TOKEN_DIRECTIVE_END  */
#line 951 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_END;
	}
#line 2584 "syntactic.c"
    break;

  case 43: /* statement: TOKEN_DIRECTIVE_RS size expression  */
#line 955 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RS;
		statement->shared.rs.size = (yyvsp[-1].size);
		statement->shared.rs.length = (yyvsp[0].expression);
	}
#line 2594 "syntactic.c"
    break;

  case 44: /* statement: TOKEN_DIRECTIVE_RSSET expression  */
#line 961 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RSSET;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2603 "syntactic.c"
    break;

  case 45: /* statement: TOKEN_DIRECTIVE_RSRESET expression  */
#line 966 "syntactic.y"
        {
		/* As mentioned in S.N. 68k's manual, this odd hack
		   is necessary for compatibility with other assemblers. */
		statement->type = STATEMENT_TYPE_RSSET;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2614 "syntactic.c"
    break;

  case 46: /* statement: TOKEN_DIRECTIVE_RSRESET  */
#line 973 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RSRESET;
	}
#line 2622 "syntactic.c"
    break;

  case 47: /* statement: TOKEN_DIRECTIVE_OBJ expression  */
#line 977 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OBJ;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2631 "syntactic.c"
    break;

  case 48: /* statement: TOKEN_DIRECTIVE_OBJEND  */
#line 982 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OBJEND;
	}
#line 2639 "syntactic.c"
    break;

  case 49: /* statement: TOKEN_DIRECTIVE_ORG expression  */
#line 986 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ORG;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2648 "syntactic.c"
    break;

  case 50: /* statement: TOKEN_DIRECTIVE_PUSHO  */
#line 991 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PUSHO;
	}
#line 2656 "syntactic.c"
    break;

  case 51: /* statement: TOKEN_DIRECTIVE_POPO  */
#line 995 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_POPO;
	}
#line 2664 "syntactic.c"
    break;

  case 52: /* statement: TOKEN_DIRECTIVE_OPT option_list  */
#line 999 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OPT;
		statement->shared.opt.options = (yyvsp[0].identifier_list);
	}
#line 2673 "syntactic.c"
    break;

  case 53: /* statement: TOKEN_DIRECTIVE_PUSHP TOKEN_STRING  */
#line 1004 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PUSHP;
		statement->shared.string = (yyvsp[0].string);
	}
#line 2682 "syntactic.c"
    break;

  case 54: /* statement: TOKEN_DIRECTIVE_POPP TOKEN_IDENTIFIER  */
#line 1009 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_POPP;
		statement->shared.string = (yyvsp[0].string);
	}
#line 2691 "syntactic.c"
    break;

  case 55: /* statement: TOKEN_DIRECTIVE_SHIFT  */
#line 1014 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SHIFT;
	}
#line 2699 "syntactic.c"
    break;

  case 56: /* statement: TOKEN_DIRECTIVE_MEXIT  */
#line 1018 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MEXIT;
	}
#line 2707 "syntactic.c"
    break;

  case 57: /* statement: TOKEN_DIRECTIVE_LOCAL identifier_list  */
#line 1022 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_LOCAL;
		statement->shared.local.identifiers = (yyvsp[0].identifier_list);
	}
#line 2716 "syntactic.c"
    break;

  case 58: /* statement: TOKEN_DIRECTIVE_LIST  */
#line 1027 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 2725 "syntactic.c"
    break;

  case 59: /* statement: TOKEN_DIRECTIVE_LIST '+'  */
#line 1032 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 2734 "syntactic.c"
    break;

  case 60: /* statement: TOKEN_DIRECTIVE_LIST '-'  */
#line 1037 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 2743 "syntactic.c"
    break;

  case 61: /* statement: TOKEN_DIRECTIVE_NOLIST  */
#line 1042 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 2752 "syntactic.c"
    break;

  case 62: /* statement: TOKEN_DIRECTIVE_PURGE TOKEN_IDENTIFIER  */
#line 1047 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PURGE;
		statement->shared.string = (yyvsp[0].string);
	}
#line 2761 "syntactic.c"
    break;

  case 63: /* expression_list: expression  */
#line 1055 "syntactic.y"
        {
		ExpressionListNode *node = (ExpressionListNode*)malloc(sizeof(ExpressionListNode));

		if (node == NULL)
		{
			DestroyExpression(&(yyvsp[0].expression));
			YYNOMEM;
		}
		else
		{
			node->expression = (yyvsp[0].expression);
			node->next = NULL;
		}

		(yyval.expression_list).head = (yyval.expression_list).tail = node;
	}
#line 2782 "syntactic.c"
    break;

  case 64: /* expression_list: expression_list ',' expression  */
#line 1072 "syntactic.y"
        {
		ExpressionListNode *node = (ExpressionListNode*)malloc(sizeof(ExpressionListNode));

		(yyval.expression_list) = (yyvsp[-2].expression_list);

		if (node == NULL)
		{
			DestroyExpressionList(&(yyvsp[-2].expression_list));
			DestroyExpression(&(yyvsp[0].expression));
			YYNOMEM;
		}
		else
		{
			node->expression = (yyvsp[0].expression);
			node->next = NULL;

			if ((yyval.expression_list).head == NULL)
				(yyval.expression_list).head = node;
			else
				((ExpressionListNode*)(yyval.expression_list).tail)->next = node;

			(yyval.expression_list).tail = node;
		}
	}
#line 2811 "syntactic.c"
    break;

  case 65: /* identifier_list: TOKEN_IDENTIFIER  */
#line 1100 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		if (node == NULL)
		{
			String_Destroy(&(yyvsp[0].string));
			YYNOMEM;
		}
		else
		{
			node->identifier = (yyvsp[0].string);
			node->next = NULL;
		}

		(yyval.identifier_list).head = (yyval.identifier_list).tail = node;
	}
#line 2832 "syntactic.c"
    break;

  case 66: /* identifier_list: identifier_list ',' TOKEN_IDENTIFIER  */
#line 1117 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		(yyval.identifier_list) = (yyvsp[-2].identifier_list);

		if (node == NULL)
		{
			DestroyIdentifierList(&(yyvsp[-2].identifier_list));
			String_Destroy(&(yyvsp[0].string));
			YYNOMEM;
		}
		else
		{
			node->identifier = (yyvsp[0].string);
			node->next = NULL;

			if ((yyval.identifier_list).head == NULL)
				(yyval.identifier_list).head = node;
			else
				((IdentifierListNode*)(yyval.identifier_list).tail)->next = node;

			(yyval.identifier_list).tail = node;
		}
	}
#line 2861 "syntactic.c"
    break;

  case 67: /* option_list: TOKEN_OPTION  */
#line 1145 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		if (node == NULL)
		{
			String_Destroy(&(yyvsp[0].string));
			YYNOMEM;
		}
		else
		{
			node->identifier = (yyvsp[0].string);
			node->next = NULL;
		}

		(yyval.identifier_list).head = (yyval.identifier_list).tail = node;
	}
#line 2882 "syntactic.c"
    break;

  case 68: /* option_list: option_list ',' TOKEN_OPTION  */
#line 1162 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		(yyval.identifier_list) = (yyvsp[-2].identifier_list);

		if (node == NULL)
		{
			DestroyIdentifierList(&(yyvsp[-2].identifier_list));
			String_Destroy(&(yyvsp[0].string));
			YYNOMEM;
		}
		else
		{
			node->identifier = (yyvsp[0].string);
			node->next = NULL;

			if ((yyval.identifier_list).head == NULL)
				(yyval.identifier_list).head = node;
			else
				((IdentifierListNode*)(yyval.identifier_list).tail)->next = node;

			(yyval.identifier_list).tail = node;
		}
	}
#line 2911 "syntactic.c"
    break;

  case 69: /* instruction: full_opcode  */
#line 1190 "syntactic.y"
        {
		(yyval.instruction).opcode = (yyvsp[0].opcode);
		(yyval.instruction).operands[0].type = OPERAND_NONE;
		(yyval.instruction).operands[1].type = OPERAND_NONE;
	}
#line 2921 "syntactic.c"
    break;

  case 70: /* instruction: full_opcode operand  */
#line 1196 "syntactic.y"
        {
		(yyval.instruction).opcode = (yyvsp[-1].opcode);
		(yyval.instruction).operands[0] = (yyvsp[0].operand);
		(yyval.instruction).operands[1].type = OPERAND_NONE;
	}
#line 2931 "syntactic.c"
    break;

  case 71: /* instruction: full_opcode operand ',' operand  */
#line 1202 "syntactic.y"
        {
		(yyval.instruction).opcode = (yyvsp[-3].opcode);
		(yyval.instruction).operands[0] = (yyvsp[-2].operand);
		(yyval.instruction).operands[1] = (yyvsp[0].operand);
	}
#line 2941 "syntactic.c"
    break;

  case 72: /* full_opcode: opcode  */
#line 1211 "syntactic.y"
        {
		(yyval.opcode) = (yyvsp[0].opcode);
		(yyval.opcode).size = SIZE_UNDEFINED;
	}
#line 2950 "syntactic.c"
    break;

  case 73: /* full_opcode: opcode '.'  */
#line 1216 "syntactic.y"
        {
		(yyval.opcode) = (yyvsp[-1].opcode);
		(yyval.opcode).size = SIZE_UNDEFINED;
		m68kasm_warning_pedantic(scanner, statement, "Opcode has a dot but no size; either remove the dot or add an explicit size.");
	}
#line 2960 "syntactic.c"
    break;

  case 74: /* full_opcode: opcode size  */
#line 1222 "syntactic.y"
        {
		(yyval.opcode) = (yyvsp[-1].opcode);
		(yyval.opcode).size = (yyvsp[0].size);
	}
#line 2969 "syntactic.c"
    break;

  case 75: /* opcode: TOKEN_DIRECTIVE_ORI  */
#line 1230 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ORI;
	}
#line 2977 "syntactic.c"
    break;

  case 76: /* opcode: TOKEN_DIRECTIVE_ANDI  */
#line 1234 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ANDI;
	}
#line 2985 "syntactic.c"
    break;

  case 77: /* opcode: TOKEN_DIRECTIVE_SUBI  */
#line 1238 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SUBI;
	}
#line 2993 "syntactic.c"
    break;

  case 78: /* opcode: TOKEN_DIRECTIVE_ADDI  */
#line 1242 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ADDI;
	}
#line 3001 "syntactic.c"
    break;

  case 79: /* opcode: TOKEN_DIRECTIVE_EORI  */
#line 1246 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_EORI;
	}
#line 3009 "syntactic.c"
    break;

  case 80: /* opcode: TOKEN_DIRECTIVE_CMPI  */
#line 1250 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CMPI;
	}
#line 3017 "syntactic.c"
    break;

  case 81: /* opcode: TOKEN_DIRECTIVE_BTST  */
#line 1254 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BTST_STATIC;
	}
#line 3025 "syntactic.c"
    break;

  case 82: /* opcode: TOKEN_DIRECTIVE_BCHG  */
#line 1258 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BCHG_STATIC;
	}
#line 3033 "syntactic.c"
    break;

  case 83: /* opcode: TOKEN_DIRECTIVE_BCLR  */
#line 1262 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BCLR_STATIC;
	}
#line 3041 "syntactic.c"
    break;

  case 84: /* opcode: TOKEN_DIRECTIVE_BSET  */
#line 1266 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BSET_STATIC;
	}
#line 3049 "syntactic.c"
    break;

  case 85: /* opcode: TOKEN_DIRECTIVE_MOVEP  */
#line 1270 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MOVEP_TO_REG;
	}
#line 3057 "syntactic.c"
    break;

  case 86: /* opcode: TOKEN_DIRECTIVE_MOVEA  */
#line 1274 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MOVEA;
	}
#line 3065 "syntactic.c"
    break;

  case 87: /* opcode: TOKEN_DIRECTIVE_MOVE  */
#line 1278 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MOVE;
	}
#line 3073 "syntactic.c"
    break;

  case 88: /* opcode: TOKEN_DIRECTIVE_NEGX  */
#line 1282 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_NEGX;
	}
#line 3081 "syntactic.c"
    break;

  case 89: /* opcode: TOKEN_DIRECTIVE_CLR  */
#line 1286 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CLR;
	}
#line 3089 "syntactic.c"
    break;

  case 90: /* opcode: TOKEN_DIRECTIVE_NEG  */
#line 1290 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_NEG;
	}
#line 3097 "syntactic.c"
    break;

  case 91: /* opcode: TOKEN_DIRECTIVE_NOT  */
#line 1294 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_NOT;
	}
#line 3105 "syntactic.c"
    break;

  case 92: /* opcode: TOKEN_DIRECTIVE_EXT  */
#line 1298 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_EXT;
	}
#line 3113 "syntactic.c"
    break;

  case 93: /* opcode: TOKEN_DIRECTIVE_NBCD  */
#line 1302 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_NBCD;
	}
#line 3121 "syntactic.c"
    break;

  case 94: /* opcode: TOKEN_DIRECTIVE_SWAP  */
#line 1306 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SWAP;
	}
#line 3129 "syntactic.c"
    break;

  case 95: /* opcode: TOKEN_DIRECTIVE_PEA  */
#line 1310 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_PEA;
	}
#line 3137 "syntactic.c"
    break;

  case 96: /* opcode: TOKEN_DIRECTIVE_ILLEGAL  */
#line 1314 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ILLEGAL;
	}
#line 3145 "syntactic.c"
    break;

  case 97: /* opcode: TOKEN_DIRECTIVE_TAS  */
#line 1318 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_TAS;
	}
#line 3153 "syntactic.c"
    break;

  case 98: /* opcode: TOKEN_DIRECTIVE_TST  */
#line 1322 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_TST;
	}
#line 3161 "syntactic.c"
    break;

  case 99: /* opcode: TOKEN_DIRECTIVE_TRAP  */
#line 1326 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_TRAP;
	}
#line 3169 "syntactic.c"
    break;

  case 100: /* opcode: TOKEN_DIRECTIVE_LINK  */
#line 1330 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_LINK;
	}
#line 3177 "syntactic.c"
    break;

  case 101: /* opcode: TOKEN_DIRECTIVE_UNLK  */
#line 1334 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_UNLK;
	}
#line 3185 "syntactic.c"
    break;

  case 102: /* opcode: TOKEN_DIRECTIVE_RESET  */
#line 1338 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_RESET;
	}
#line 3193 "syntactic.c"
    break;

  case 103: /* opcode: TOKEN_DIRECTIVE_NOP  */
#line 1342 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_NOP;
	}
#line 3201 "syntactic.c"
    break;

  case 104: /* opcode: TOKEN_DIRECTIVE_STOP  */
#line 1346 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_STOP;
	}
#line 3209 "syntactic.c"
    break;

  case 105: /* opcode: TOKEN_DIRECTIVE_RTE  */
#line 1350 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_RTE;
	}
#line 3217 "syntactic.c"
    break;

  case 106: /* opcode: TOKEN_DIRECTIVE_RTS  */
#line 1354 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_RTS;
	}
#line 3225 "syntactic.c"
    break;

  case 107: /* opcode: TOKEN_DIRECTIVE_TRAPV  */
#line 1358 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_TRAPV;
	}
#line 3233 "syntactic.c"
    break;

  case 108: /* opcode: TOKEN_DIRECTIVE_RTR  */
#line 1362 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_RTR;
	}
#line 3241 "syntactic.c"
    break;

  case 109: /* opcode: TOKEN_DIRECTIVE_JSR  */
#line 1366 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_JSR;
	}
#line 3249 "syntactic.c"
    break;

  case 110: /* opcode: TOKEN_DIRECTIVE_JMP  */
#line 1370 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_JMP;
	}
#line 3257 "syntactic.c"
    break;

  case 111: /* opcode: TOKEN_DIRECTIVE_MOVEM  */
#line 1374 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MOVEM_TO_REGS;
	}
#line 3265 "syntactic.c"
    break;

  case 112: /* opcode: TOKEN_DIRECTIVE_LEA  */
#line 1378 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_LEA;
	}
#line 3273 "syntactic.c"
    break;

  case 113: /* opcode: TOKEN_DIRECTIVE_CHK  */
#line 1382 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CHK;
	}
#line 3281 "syntactic.c"
    break;

  case 114: /* opcode: TOKEN_DIRECTIVE_ADDQ  */
#line 1386 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ADDQ;
	}
#line 3289 "syntactic.c"
    break;

  case 115: /* opcode: TOKEN_DIRECTIVE_SUBQ  */
#line 1390 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SUBQ;
	}
#line 3297 "syntactic.c"
    break;

  case 116: /* opcode: TOKEN_DIRECTIVE_ST  */
#line 1394 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_TRUE;
	}
#line 3306 "syntactic.c"
    break;

  case 117: /* opcode: TOKEN_DIRECTIVE_SF  */
#line 1399 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_FALSE;
	}
#line 3315 "syntactic.c"
    break;

  case 118: /* opcode: TOKEN_DIRECTIVE_SHI  */
#line 1404 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_HIGHER;
	}
#line 3324 "syntactic.c"
    break;

  case 119: /* opcode: TOKEN_DIRECTIVE_SLS  */
#line 1409 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_LOWER_OR_SAME;
	}
#line 3333 "syntactic.c"
    break;

  case 120: /* opcode: TOKEN_DIRECTIVE_SCC  */
#line 1414 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_CARRY_CLEAR;
	}
#line 3342 "syntactic.c"
    break;

  case 121: /* opcode: TOKEN_DIRECTIVE_SCS  */
#line 1419 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_CARRY_SET;
	}
#line 3351 "syntactic.c"
    break;

  case 122: /* opcode: TOKEN_DIRECTIVE_SNE  */
#line 1424 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_NOT_EQUAL;
	}
#line 3360 "syntactic.c"
    break;

  case 123: /* opcode: TOKEN_DIRECTIVE_SEQ  */
#line 1429 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_EQUAL;
	}
#line 3369 "syntactic.c"
    break;

  case 124: /* opcode: TOKEN_DIRECTIVE_SVC  */
#line 1434 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 3378 "syntactic.c"
    break;

  case 125: /* opcode: TOKEN_DIRECTIVE_SVS  */
#line 1439 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_SET;
	}
#line 3387 "syntactic.c"
    break;

  case 126: /* opcode: TOKEN_DIRECTIVE_SPL  */
#line 1444 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_PLUS;
	}
#line 3396 "syntactic.c"
    break;

  case 127: /* opcode: TOKEN_DIRECTIVE_SMI  */
#line 1449 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_MINUS;
	}
#line 3405 "syntactic.c"
    break;

  case 128: /* opcode: TOKEN_DIRECTIVE_SGE  */
#line 1454 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 3414 "syntactic.c"
    break;

  case 129: /* opcode: TOKEN_DIRECTIVE_SLT  */
#line 1459 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_LESS_THAN;
	}
#line 3423 "syntactic.c"
    break;

  case 130: /* opcode: TOKEN_DIRECTIVE_SGT  */
#line 1464 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_GREATER_THAN;
	}
#line 3432 "syntactic.c"
    break;

  case 131: /* opcode: TOKEN_DIRECTIVE_SLE  */
#line 1469 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_LESS_OR_EQUAL;
	}
#line 3441 "syntactic.c"
    break;

  case 132: /* opcode: TOKEN_DIRECTIVE_DBT  */
#line 1474 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_TRUE;
	}
#line 3450 "syntactic.c"
    break;

  case 133: /* opcode: TOKEN_DIRECTIVE_DBF  */
#line 1479 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_FALSE;
	}
#line 3459 "syntactic.c"
    break;

  case 134: /* opcode: TOKEN_DIRECTIVE_DBHI  */
#line 1484 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_HIGHER;
	}
#line 3468 "syntactic.c"
    break;

  case 135: /* opcode: TOKEN_DIRECTIVE_DBLS  */
#line 1489 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_LOWER_OR_SAME;
	}
#line 3477 "syntactic.c"
    break;

  case 136: /* opcode: TOKEN_DIRECTIVE_DBCC  */
#line 1494 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_CARRY_CLEAR;
	}
#line 3486 "syntactic.c"
    break;

  case 137: /* opcode: TOKEN_DIRECTIVE_DBCS  */
#line 1499 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_CARRY_SET;
	}
#line 3495 "syntactic.c"
    break;

  case 138: /* opcode: TOKEN_DIRECTIVE_DBNE  */
#line 1504 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_NOT_EQUAL;
	}
#line 3504 "syntactic.c"
    break;

  case 139: /* opcode: TOKEN_DIRECTIVE_DBEQ  */
#line 1509 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_EQUAL;
	}
#line 3513 "syntactic.c"
    break;

  case 140: /* opcode: TOKEN_DIRECTIVE_DBVC  */
#line 1514 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 3522 "syntactic.c"
    break;

  case 141: /* opcode: TOKEN_DIRECTIVE_DBVS  */
#line 1519 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_SET;
	}
#line 3531 "syntactic.c"
    break;

  case 142: /* opcode: TOKEN_DIRECTIVE_DBPL  */
#line 1524 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_PLUS;
	}
#line 3540 "syntactic.c"
    break;

  case 143: /* opcode: TOKEN_DIRECTIVE_DBMI  */
#line 1529 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_MINUS;
	}
#line 3549 "syntactic.c"
    break;

  case 144: /* opcode: TOKEN_DIRECTIVE_DBGE  */
#line 1534 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 3558 "syntactic.c"
    break;

  case 145: /* opcode: TOKEN_DIRECTIVE_DBLT  */
#line 1539 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_LESS_THAN;
	}
#line 3567 "syntactic.c"
    break;

  case 146: /* opcode: TOKEN_DIRECTIVE_DBGT  */
#line 1544 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_GREATER_THAN;
	}
#line 3576 "syntactic.c"
    break;

  case 147: /* opcode: TOKEN_DIRECTIVE_DBLE  */
#line 1549 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_LESS_OR_EQUAL;
	}
#line 3585 "syntactic.c"
    break;

  case 148: /* opcode: TOKEN_DIRECTIVE_BRA  */
#line 1554 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BRA;
	}
#line 3593 "syntactic.c"
    break;

  case 149: /* opcode: TOKEN_DIRECTIVE_BSR  */
#line 1558 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BSR;
	}
#line 3601 "syntactic.c"
    break;

  case 150: /* opcode: TOKEN_DIRECTIVE_BHI  */
#line 1562 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_HIGHER;
	}
#line 3610 "syntactic.c"
    break;

  case 151: /* opcode: TOKEN_DIRECTIVE_BLS  */
#line 1567 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_LOWER_OR_SAME;
	}
#line 3619 "syntactic.c"
    break;

  case 152: /* opcode: TOKEN_DIRECTIVE_BCC  */
#line 1572 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_CARRY_CLEAR;
	}
#line 3628 "syntactic.c"
    break;

  case 153: /* opcode: TOKEN_DIRECTIVE_BCS  */
#line 1577 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_CARRY_SET;
	}
#line 3637 "syntactic.c"
    break;

  case 154: /* opcode: TOKEN_DIRECTIVE_BNE  */
#line 1582 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_NOT_EQUAL;
	}
#line 3646 "syntactic.c"
    break;

  case 155: /* opcode: TOKEN_DIRECTIVE_BEQ  */
#line 1587 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_EQUAL;
	}
#line 3655 "syntactic.c"
    break;

  case 156: /* opcode: TOKEN_DIRECTIVE_BVC  */
#line 1592 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 3664 "syntactic.c"
    break;

  case 157: /* opcode: TOKEN_DIRECTIVE_BVS  */
#line 1597 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_SET;
	}
#line 3673 "syntactic.c"
    break;

  case 158: /* opcode: TOKEN_DIRECTIVE_BPL  */
#line 1602 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_PLUS;
	}
#line 3682 "syntactic.c"
    break;

  case 159: /* opcode: TOKEN_DIRECTIVE_BMI  */
#line 1607 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_MINUS;
	}
#line 3691 "syntactic.c"
    break;

  case 160: /* opcode: TOKEN_DIRECTIVE_BGE  */
#line 1612 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 3700 "syntactic.c"
    break;

  case 161: /* opcode: TOKEN_DIRECTIVE_BLT  */
#line 1617 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_LESS_THAN;
	}
#line 3709 "syntactic.c"
    break;

  case 162: /* opcode: TOKEN_DIRECTIVE_BGT  */
#line 1622 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_GREATER_THAN;
	}
#line 3718 "syntactic.c"
    break;

  case 163: /* opcode: TOKEN_DIRECTIVE_BLE  */
#line 1627 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_LESS_OR_EQUAL;
	}
#line 3727 "syntactic.c"
    break;

  case 164: /* opcode: TOKEN_DIRECTIVE_MOVEQ  */
#line 1632 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MOVEQ;
	}
#line 3735 "syntactic.c"
    break;

  case 165: /* opcode: TOKEN_DIRECTIVE_DIVU  */
#line 1636 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DIVU;
	}
#line 3743 "syntactic.c"
    break;

  case 166: /* opcode: TOKEN_DIRECTIVE_DIVS  */
#line 1640 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DIVS;
	}
#line 3751 "syntactic.c"
    break;

  case 167: /* opcode: TOKEN_DIRECTIVE_SBCD  */
#line 1644 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SBCD_DATA_REGS;
	}
#line 3759 "syntactic.c"
    break;

  case 168: /* opcode: TOKEN_DIRECTIVE_OR  */
#line 1648 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_OR_TO_REG;
	}
#line 3767 "syntactic.c"
    break;

  case 169: /* opcode: TOKEN_DIRECTIVE_SUB  */
#line 1652 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SUB_TO_REG;
	}
#line 3775 "syntactic.c"
    break;

  case 170: /* opcode: TOKEN_DIRECTIVE_SUBX  */
#line 1656 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SUBX_DATA_REGS;
	}
#line 3783 "syntactic.c"
    break;

  case 171: /* opcode: TOKEN_DIRECTIVE_SUBA  */
#line 1660 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SUBA;
	}
#line 3791 "syntactic.c"
    break;

  case 172: /* opcode: TOKEN_DIRECTIVE_EOR  */
#line 1664 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_EOR;
	}
#line 3799 "syntactic.c"
    break;

  case 173: /* opcode: TOKEN_DIRECTIVE_CMPM  */
#line 1668 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CMPM;
	}
#line 3807 "syntactic.c"
    break;

  case 174: /* opcode: TOKEN_DIRECTIVE_CMP  */
#line 1672 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CMP;
	}
#line 3815 "syntactic.c"
    break;

  case 175: /* opcode: TOKEN_DIRECTIVE_CMPA  */
#line 1676 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CMPA;
	}
#line 3823 "syntactic.c"
    break;

  case 176: /* opcode: TOKEN_DIRECTIVE_MULU  */
#line 1680 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MULU;
	}
#line 3831 "syntactic.c"
    break;

  case 177: /* opcode: TOKEN_DIRECTIVE_MULS  */
#line 1684 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MULS;
	}
#line 3839 "syntactic.c"
    break;

  case 178: /* opcode: TOKEN_DIRECTIVE_ABCD  */
#line 1688 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ABCD_DATA_REGS;
	}
#line 3847 "syntactic.c"
    break;

  case 179: /* opcode: TOKEN_DIRECTIVE_EXG  */
#line 1692 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_EXG;
	}
#line 3855 "syntactic.c"
    break;

  case 180: /* opcode: TOKEN_DIRECTIVE_AND  */
#line 1696 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_AND_TO_REG;
	}
#line 3863 "syntactic.c"
    break;

  case 181: /* opcode: TOKEN_DIRECTIVE_ADD  */
#line 1700 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ADD_TO_REG;
	}
#line 3871 "syntactic.c"
    break;

  case 182: /* opcode: TOKEN_DIRECTIVE_ADDX  */
#line 1704 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ADDX_DATA_REGS;
	}
#line 3879 "syntactic.c"
    break;

  case 183: /* opcode: TOKEN_DIRECTIVE_ADDA  */
#line 1708 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ADDA;
	}
#line 3887 "syntactic.c"
    break;

  case 184: /* opcode: TOKEN_DIRECTIVE_ASL  */
#line 1712 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ASL_STATIC;
	}
#line 3895 "syntactic.c"
    break;

  case 185: /* opcode: TOKEN_DIRECTIVE_ASR  */
#line 1716 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ASR_STATIC;
	}
#line 3903 "syntactic.c"
    break;

  case 186: /* opcode: TOKEN_DIRECTIVE_LSL  */
#line 1720 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_LSL_STATIC;
	}
#line 3911 "syntactic.c"
    break;

  case 187: /* opcode: TOKEN_DIRECTIVE_LSR  */
#line 1724 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_LSR_STATIC;
	}
#line 3919 "syntactic.c"
    break;

  case 188: /* opcode: TOKEN_DIRECTIVE_ROXL  */
#line 1728 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ROXL_STATIC;
	}
#line 3927 "syntactic.c"
    break;

  case 189: /* opcode: TOKEN_DIRECTIVE_ROXR  */
#line 1732 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ROXR_STATIC;
	}
#line 3935 "syntactic.c"
    break;

  case 190: /* opcode: TOKEN_DIRECTIVE_ROL  */
#line 1736 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ROL_STATIC;
	}
#line 3943 "syntactic.c"
    break;

  case 191: /* opcode: TOKEN_DIRECTIVE_ROR  */
#line 1740 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ROR_STATIC;
	}
#line 3951 "syntactic.c"
    break;

  case 192: /* size: TOKEN_SIZE_BYTE  */
#line 1747 "syntactic.y"
        {
		(yyval.size) = SIZE_BYTE;
	}
#line 3959 "syntactic.c"
    break;

  case 193: /* size: TOKEN_SIZE_SHORT  */
#line 1751 "syntactic.y"
        {
		(yyval.size) = SIZE_SHORT;
	}
#line 3967 "syntactic.c"
    break;

  case 194: /* size: TOKEN_SIZE_WORD  */
#line 1755 "syntactic.y"
        {
		(yyval.size) = SIZE_WORD;
	}
#line 3975 "syntactic.c"
    break;

  case 195: /* size: TOKEN_SIZE_LONGWORD  */
#line 1759 "syntactic.y"
        {
		(yyval.size) = SIZE_LONGWORD;
	}
#line 3983 "syntactic.c"
    break;

  case 196: /* operand: '(' TOKEN_ADDRESS_REGISTER ')'  */
#line 1767 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT;
		(yyval.operand).main_register = (yyvsp[-1].unsigned_long);
	}
#line 3992 "syntactic.c"
    break;

  case 197: /* operand: '(' TOKEN_ADDRESS_REGISTER ')' '+'  */
#line 1772 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
		(yyval.operand).main_register = (yyvsp[-2].unsigned_long);
	}
#line 4001 "syntactic.c"
    break;

  case 198: /* operand: '-' '(' TOKEN_ADDRESS_REGISTER ')'  */
#line 1777 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
		(yyval.operand).main_register = (yyvsp[-1].unsigned_long);
	}
#line 4010 "syntactic.c"
    break;

  case 199: /* operand: expression '(' TOKEN_ADDRESS_REGISTER ')'  */
#line 1782 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-3].expression);
		(yyval.operand).main_register = (yyvsp[-1].unsigned_long);
	}
#line 4020 "syntactic.c"
    break;

  case 200: /* operand: '(' expression ',' TOKEN_ADDRESS_REGISTER ')'  */
#line 1788 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-3].expression);
		(yyval.operand).main_register = (yyvsp[-1].unsigned_long);
	}
#line 4030 "syntactic.c"
    break;

  case 201: /* operand: '(' TOKEN_ADDRESS_REGISTER ',' expression ')'  */
#line 1794 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-1].expression);
		(yyval.operand).main_register = (yyvsp[-3].unsigned_long);
	}
#line 4040 "syntactic.c"
    break;

  case 202: /* operand: '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register size ')'  */
#line 1800 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal.type = EXPRESSION_NUMBER;
		(yyval.operand).literal.shared.unsigned_long = 0;
		(yyval.operand).main_register = (yyvsp[-4].unsigned_long);
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 != 0;
	}
#line 4054 "syntactic.c"
    break;

  case 203: /* operand: expression '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register size ')'  */
#line 1810 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-6].expression);
		(yyval.operand).main_register = (yyvsp[-4].unsigned_long);
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 !=0;
	}
#line 4067 "syntactic.c"
    break;

  case 204: /* operand: '(' expression ',' TOKEN_ADDRESS_REGISTER ',' data_or_address_register size ')'  */
#line 1819 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-6].expression);
		(yyval.operand).main_register = (yyvsp[-4].unsigned_long);
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 !=0;
	}
#line 4080 "syntactic.c"
    break;

  case 205: /* operand: '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register ')'  */
#line 1828 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal.type = EXPRESSION_NUMBER;
		(yyval.operand).literal.shared.unsigned_long = 0;
		(yyval.operand).main_register = (yyvsp[-3].unsigned_long);
		(yyval.operand).index_register = (yyvsp[-1].unsigned_long) % 8;
		(yyval.operand).size = SIZE_WORD;
		(yyval.operand).index_register_is_address_register = (yyvsp[-1].unsigned_long) / 8 != 0;
	}
#line 4095 "syntactic.c"
    break;

  case 206: /* operand: expression '(' TOKEN_ADDRESS_REGISTER ',' data_or_address_register ')'  */
#line 1839 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-5].expression);
		(yyval.operand).main_register = (yyvsp[-3].unsigned_long);
		(yyval.operand).index_register = (yyvsp[-1].unsigned_long) % 8;
		(yyval.operand).size = SIZE_WORD;
		(yyval.operand).index_register_is_address_register = (yyvsp[-1].unsigned_long) / 8 !=0;
	}
#line 4109 "syntactic.c"
    break;

  case 207: /* operand: '(' expression ',' TOKEN_ADDRESS_REGISTER ',' data_or_address_register ')'  */
#line 1849 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-5].expression);
		(yyval.operand).main_register = (yyvsp[-3].unsigned_long);
		(yyval.operand).index_register = (yyvsp[-1].unsigned_long) % 8;
		(yyval.operand).size = SIZE_WORD;
		(yyval.operand).index_register_is_address_register = (yyvsp[-1].unsigned_long) / 8 !=0;
	}
#line 4123 "syntactic.c"
    break;

  case 208: /* operand: expression '(' TOKEN_PROGRAM_COUNTER ')'  */
#line 1859 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-3].expression);
	}
#line 4132 "syntactic.c"
    break;

  case 209: /* operand: '(' expression ',' TOKEN_PROGRAM_COUNTER ')'  */
#line 1864 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-3].expression);
	}
#line 4141 "syntactic.c"
    break;

  case 210: /* operand: '(' TOKEN_PROGRAM_COUNTER ',' expression ')'  */
#line 1869 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-1].expression);
	}
#line 4150 "syntactic.c"
    break;

  case 211: /* operand: '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register size ')'  */
#line 1874 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal.type = EXPRESSION_NUMBER;
		(yyval.operand).literal.shared.unsigned_long = 0;
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 != 0;
	}
#line 4163 "syntactic.c"
    break;

  case 212: /* operand: expression '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register size ')'  */
#line 1883 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-6].expression);
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 != 0;
	}
#line 4175 "syntactic.c"
    break;

  case 213: /* operand: '(' expression ',' TOKEN_PROGRAM_COUNTER ',' data_or_address_register size ')'  */
#line 1891 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-6].expression);
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 != 0;
	}
#line 4187 "syntactic.c"
    break;

  case 214: /* operand: '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register ')'  */
#line 1899 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal.type = EXPRESSION_NUMBER;
		(yyval.operand).literal.shared.unsigned_long = 0;
		(yyval.operand).index_register = (yyvsp[-1].unsigned_long) % 8;
		(yyval.operand).size = SIZE_WORD;
		(yyval.operand).index_register_is_address_register = (yyvsp[-1].unsigned_long) / 8 != 0;
	}
#line 4201 "syntactic.c"
    break;

  case 215: /* operand: expression '(' TOKEN_PROGRAM_COUNTER ',' data_or_address_register ')'  */
#line 1909 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-5].expression);
		(yyval.operand).index_register = (yyvsp[-1].unsigned_long) % 8;
		(yyval.operand).size = SIZE_WORD;
		(yyval.operand).index_register_is_address_register = (yyvsp[-1].unsigned_long) / 8 != 0;
	}
#line 4214 "syntactic.c"
    break;

  case 216: /* operand: '(' expression ',' TOKEN_PROGRAM_COUNTER ',' data_or_address_register ')'  */
#line 1918 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-5].expression);
		(yyval.operand).index_register = (yyvsp[-1].unsigned_long) % 8;
		(yyval.operand).size = SIZE_WORD;
		(yyval.operand).index_register_is_address_register = (yyvsp[-1].unsigned_long) / 8 != 0;
	}
#line 4227 "syntactic.c"
    break;

  case 217: /* operand: '#' expression  */
#line 1928 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_LITERAL;
		(yyval.operand).literal = (yyvsp[0].expression);
	}
#line 4236 "syntactic.c"
    break;

  case 218: /* operand: TOKEN_STATUS_REGISTER  */
#line 1944 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_STATUS_REGISTER;
	}
#line 4244 "syntactic.c"
    break;

  case 219: /* operand: TOKEN_CONDITION_CODE_REGISTER  */
#line 1948 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_CONDITION_CODE_REGISTER;
	}
#line 4252 "syntactic.c"
    break;

  case 220: /* operand: TOKEN_USER_STACK_POINTER_REGISTER  */
#line 1952 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_USER_STACK_POINTER_REGISTER;
	}
#line 4260 "syntactic.c"
    break;

  case 221: /* operand: expression  */
#line 1957 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS;
		(yyval.operand).literal = (yyvsp[0].expression);
		(yyval.operand).size = SIZE_UNDEFINED;
	}
#line 4270 "syntactic.c"
    break;

  case 222: /* operand: expression size  */
#line 1963 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_ABSOLUTE;
		(yyval.operand).literal = (yyvsp[-1].expression);
		(yyval.operand).size = (yyvsp[0].size);
	}
#line 4280 "syntactic.c"
    break;

  case 223: /* operand: register_list  */
#line 1970 "syntactic.y"
        {
		/* Neat trick to check if only one bit is set (yes, I know that it evaluates to true for 0, but that doesn't matter here). */
		if (((yyvsp[0].unsigned_long) & ((yyvsp[0].unsigned_long) - 1)) == 0)
		{
			/* This is a single register. */
			(yyval.operand).main_register = 0;

			while (((yyvsp[0].unsigned_long) & (1 << (yyval.operand).main_register)) == 0)
				++(yyval.operand).main_register;

			if ((yyval.operand).main_register >= 8)
			{
				(yyval.operand).type = OPERAND_ADDRESS_REGISTER;
				(yyval.operand).main_register -= 8;
			}
			else
			{
				(yyval.operand).type = OPERAND_DATA_REGISTER;
			}
		}
		else
		{
			/* This is multiple registers. */
			(yyval.operand).type = OPERAND_REGISTER_LIST;
			(yyval.operand).main_register = (yyvsp[0].unsigned_long); /* Such a hack... */
		}
	}
#line 4312 "syntactic.c"
    break;

  case 224: /* register_list: register_span  */
#line 2001 "syntactic.y"
        {
		(yyval.unsigned_long) = (yyvsp[0].unsigned_long);
	}
#line 4320 "syntactic.c"
    break;

  case 225: /* register_list: register_span '/' register_list  */
#line 2005 "syntactic.y"
        {
		(yyval.unsigned_long) = (yyvsp[-2].unsigned_long) | (yyvsp[0].unsigned_long);
	}
#line 4328 "syntactic.c"
    break;

  case 226: /* register_span: data_or_address_register  */
#line 2012 "syntactic.y"
        {
		(yyval.unsigned_long) = 1 << (yyvsp[0].unsigned_long);
	}
#line 4336 "syntactic.c"
    break;

  case 227: /* register_span: data_or_address_register '-' data_or_address_register  */
#line 2016 "syntactic.y"
        {
		unsigned long start;
		unsigned long end;
		unsigned long i;

		if ((yyvsp[-2].unsigned_long) < (yyvsp[0].unsigned_long))
		{
			start = (yyvsp[-2].unsigned_long);
			end = (yyvsp[0].unsigned_long);
		}
		else
		{
			start = (yyvsp[0].unsigned_long);
			end = (yyvsp[-2].unsigned_long);
		}

		(yyval.unsigned_long) = 0;

		for (i = start; i <= end; ++i)
			(yyval.unsigned_long) |= 1 << i;
	}
#line 4362 "syntactic.c"
    break;

  case 228: /* data_or_address_register: TOKEN_DATA_REGISTER  */
#line 2041 "syntactic.y"
        {
		(yyval.unsigned_long) = 0 + (yyvsp[0].unsigned_long);
	}
#line 4370 "syntactic.c"
    break;

  case 229: /* data_or_address_register: TOKEN_ADDRESS_REGISTER  */
#line 2045 "syntactic.y"
        {
		(yyval.unsigned_long) = 8 + (yyvsp[0].unsigned_long);
	}
#line 4378 "syntactic.c"
    break;

  case 230: /* expression: expression1  */
#line 2058 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4386 "syntactic.c"
    break;

  case 231: /* expression: expression TOKEN_LOGICAL_AND expression1  */
#line 2063 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LOGICAL_AND, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4395 "syntactic.c"
    break;

  case 232: /* expression: expression TOKEN_LOGICAL_OR expression1  */
#line 2069 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LOGICAL_OR, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4404 "syntactic.c"
    break;

  case 233: /* expression1: expression2  */
#line 2077 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4412 "syntactic.c"
    break;

  case 234: /* expression1: expression1 '=' expression2  */
#line 2081 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_EQUALITY, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4421 "syntactic.c"
    break;

  case 235: /* expression1: expression1 TOKEN_EQUALITY expression2  */
#line 2086 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_EQUALITY, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4430 "syntactic.c"
    break;

  case 236: /* expression1: expression1 TOKEN_INEQUALITY expression2  */
#line 2091 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_INEQUALITY, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4439 "syntactic.c"
    break;

  case 237: /* expression2: expression3  */
#line 2099 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4447 "syntactic.c"
    break;

  case 238: /* expression2: expression2 '<' expression3  */
#line 2103 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LESS_THAN, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4456 "syntactic.c"
    break;

  case 239: /* expression2: expression2 TOKEN_LESS_OR_EQUAL expression3  */
#line 2108 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LESS_OR_EQUAL, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4465 "syntactic.c"
    break;

  case 240: /* expression2: expression2 '>' expression3  */
#line 2113 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_MORE_THAN, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4474 "syntactic.c"
    break;

  case 241: /* expression2: expression2 TOKEN_MORE_OR_EQUAL expression3  */
#line 2118 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_MORE_OR_EQUAL, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4483 "syntactic.c"
    break;

  case 242: /* expression3: expression4  */
#line 2126 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4491 "syntactic.c"
    break;

  case 243: /* expression3: expression3 '+' expression4  */
#line 2130 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_ADD, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4500 "syntactic.c"
    break;

  case 244: /* expression3: expression3 '-' expression4  */
#line 2135 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_SUBTRACT, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4509 "syntactic.c"
    break;

  case 245: /* expression4: expression5  */
#line 2143 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4517 "syntactic.c"
    break;

  case 246: /* expression4: expression4 '*' expression5  */
#line 2147 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_MULTIPLY, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4526 "syntactic.c"
    break;

  case 247: /* expression4: expression4 '/' expression5  */
#line 2152 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_DIVIDE, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4535 "syntactic.c"
    break;

  case 248: /* expression4: expression4 '%' expression5  */
#line 2157 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_MODULO, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4544 "syntactic.c"
    break;

  case 249: /* expression5: expression6  */
#line 2165 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4552 "syntactic.c"
    break;

  case 250: /* expression5: expression5 '&' expression6  */
#line 2169 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_BITWISE_AND, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4561 "syntactic.c"
    break;

  case 251: /* expression5: expression5 '!' expression6  */
#line 2174 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_BITWISE_OR, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4570 "syntactic.c"
    break;

  case 252: /* expression5: expression5 '|' expression6  */
#line 2179 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_BITWISE_OR, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4579 "syntactic.c"
    break;

  case 253: /* expression5: expression5 '^' expression6  */
#line 2184 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_BITWISE_XOR, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4588 "syntactic.c"
    break;

  case 254: /* expression6: expression7  */
#line 2192 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4596 "syntactic.c"
    break;

  case 255: /* expression6: expression6 TOKEN_LEFT_SHIFT expression7  */
#line 2196 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LEFT_SHIFT, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4605 "syntactic.c"
    break;

  case 256: /* expression6: expression6 TOKEN_RIGHT_SHIFT expression7  */
#line 2201 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_RIGHT_SHIFT, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4614 "syntactic.c"
    break;

  case 257: /* expression7: expression8  */
#line 2209 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4622 "syntactic.c"
    break;

  case 258: /* expression7: '+' expression7  */
#line 2213 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4630 "syntactic.c"
    break;

  case 259: /* expression7: '-' expression7  */
#line 2217 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_NEGATE, &(yyvsp[0].expression), NULL))
			YYNOMEM;
	}
#line 4639 "syntactic.c"
    break;

  case 260: /* expression7: '~' expression7  */
#line 2222 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_BITWISE_NOT, &(yyvsp[0].expression), NULL))
			YYNOMEM;
	}
#line 4648 "syntactic.c"
    break;

  case 261: /* expression7: '!' expression7  */
#line 2228 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LOGICAL_NOT, &(yyvsp[0].expression), NULL))
			YYNOMEM;
	}
#line 4657 "syntactic.c"
    break;

  case 263: /* local_identifier: TOKEN_SIZE_BYTE  */
#line 2237 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".b");
		if (!String_CreateCopyView(&(yyval.string), &literal))
			YYNOMEM;
	}
#line 4667 "syntactic.c"
    break;

  case 264: /* local_identifier: TOKEN_SIZE_SHORT  */
#line 2243 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".s");
		if (!String_CreateCopyView(&(yyval.string), &literal))
			YYNOMEM;
	}
#line 4677 "syntactic.c"
    break;

  case 265: /* local_identifier: TOKEN_SIZE_WORD  */
#line 2249 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".w");
		if (!String_CreateCopyView(&(yyval.string), &literal))
			YYNOMEM;
	}
#line 4687 "syntactic.c"
    break;

  case 266: /* local_identifier: TOKEN_SIZE_LONGWORD  */
#line 2255 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".l");
		if (!String_CreateCopyView(&(yyval.string), &literal))
			YYNOMEM;
	}
#line 4697 "syntactic.c"
    break;

  case 269: /* identifier: identifier local_identifier  */
#line 2266 "syntactic.y"
        {
		const cc_bool success = String_CreateAppend(&(yyval.string), &(yyvsp[-1].string), &(yyvsp[0].string));
		String_Destroy(&(yyvsp[-1].string));
		String_Destroy(&(yyvsp[0].string));

		if (!success)
			YYNOMEM;
	}
#line 4710 "syntactic.c"
    break;

  case 270: /* expression8: TOKEN_NUMBER  */
#line 2278 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_NUMBER;
		(yyval.expression).shared.unsigned_long = (yyvsp[0].unsigned_long);
	}
#line 4719 "syntactic.c"
    break;

  case 271: /* expression8: identifier  */
#line 2283 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_IDENTIFIER;
		(yyval.expression).shared.string = (yyvsp[0].string);
	}
#line 4728 "syntactic.c"
    break;

  case 272: /* expression8: string  */
#line 2288 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4736 "syntactic.c"
    break;

  case 273: /* expression8: '*'  */
#line 2292 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT;
	}
#line 4744 "syntactic.c"
    break;

  case 274: /* expression8: '@'  */
#line 2296 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION;
	}
#line 4752 "syntactic.c"
    break;

  case 275: /* expression8: '(' expression ')'  */
#line 2300 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[-1].expression);
	}
#line 4760 "syntactic.c"
    break;

  case 276: /* expression8: TOKEN_STRLEN '(' TOKEN_STRING ')'  */
#line 2304 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_STRLEN;
		(yyval.expression).shared.string = (yyvsp[-1].string);
	}
#line 4769 "syntactic.c"
    break;

  case 277: /* expression8: TOKEN_STRCMP '(' string ',' string ')'  */
#line 2309 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_STRCMP, &(yyvsp[-3].expression), &(yyvsp[-1].expression)))
			YYNOMEM;
	}
#line 4778 "syntactic.c"
    break;

  case 278: /* expression8: TOKEN_INSTR '(' expression ',' expression ')'  */
#line 2314 "syntactic.y"
        {
		Expression expression;

		expression.type = EXPRESSION_NUMBER;
		expression.shared.unsigned_long = 1;

		if (!DoExpressionTriple(&(yyval.expression), EXPRESSION_INSTR, &expression, &(yyvsp[-3].expression), &(yyvsp[-1].expression)))
			YYNOMEM;
	}
#line 4792 "syntactic.c"
    break;

  case 279: /* expression8: TOKEN_INSTR '(' expression ',' expression ',' expression ')'  */
#line 2324 "syntactic.y"
        {
		if (!DoExpressionTriple(&(yyval.expression), EXPRESSION_INSTR, &(yyvsp[-5].expression), &(yyvsp[-3].expression), &(yyvsp[-1].expression)))
			YYNOMEM;
	}
#line 4801 "syntactic.c"
    break;

  case 280: /* expression8: TOKEN_DEF '(' TOKEN_IDENTIFIER ')'  */
#line 2329 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_DEF;
		(yyval.expression).shared.string = (yyvsp[-1].string);
	}
#line 4810 "syntactic.c"
    break;

  case 281: /* expression8: TOKEN_TYPE '(' TOKEN_IDENTIFIER ')'  */
#line 2334 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_TYPE_WITH_IDENTIFIER;
		(yyval.expression).shared.string = (yyvsp[-1].string);
	}
#line 4819 "syntactic.c"
    break;

  case 282: /* expression8: TOKEN_TYPE '(' TOKEN_NUMBER ')'  */
#line 2339 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_TYPE_WITH_NUMBER;
		(yyval.expression).shared.unsigned_long = (yyvsp[-1].unsigned_long);
	}
#line 4828 "syntactic.c"
    break;

  case 283: /* expression8: TOKEN_FILESIZE '(' TOKEN_STRING ')'  */
#line 2344 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_FILESIZE;
		(yyval.expression).shared.string = (yyvsp[-1].string);
	}
#line 4837 "syntactic.c"
    break;

  case 284: /* expression8: TOKEN_OFFSET '(' expression ')'  */
#line 2349 "syntactic.y"
        {
		/* TODO: Make this actually do something besides pass the expression through. */
		(yyval.expression) = (yyvsp[-1].expression);
	}
#line 4846 "syntactic.c"
    break;

  case 285: /* string: TOKEN_STRING  */
#line 2357 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_STRING;
		(yyval.expression).shared.string = (yyvsp[0].string);
	}
#line 4855 "syntactic.c"
    break;


#line 4859 "syntactic.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == M68KASM_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (scanner, statement, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= M68KASM_EOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == M68KASM_EOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, scanner, statement);
          yychar = M68KASM_EMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, scanner, statement);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, statement, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != M68KASM_EMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, scanner, statement);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, scanner, statement);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 2362 "syntactic.y"


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
