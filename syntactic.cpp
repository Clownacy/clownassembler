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
  YYSYMBOL_DIRECTIVE_ORI = 3,              /* DIRECTIVE_ORI  */
  YYSYMBOL_DIRECTIVE_ANDI = 4,             /* DIRECTIVE_ANDI  */
  YYSYMBOL_DIRECTIVE_SUBI = 5,             /* DIRECTIVE_SUBI  */
  YYSYMBOL_DIRECTIVE_ADDI = 6,             /* DIRECTIVE_ADDI  */
  YYSYMBOL_DIRECTIVE_EORI = 7,             /* DIRECTIVE_EORI  */
  YYSYMBOL_DIRECTIVE_CMPI = 8,             /* DIRECTIVE_CMPI  */
  YYSYMBOL_DIRECTIVE_BTST = 9,             /* DIRECTIVE_BTST  */
  YYSYMBOL_DIRECTIVE_BCHG = 10,            /* DIRECTIVE_BCHG  */
  YYSYMBOL_DIRECTIVE_BCLR = 11,            /* DIRECTIVE_BCLR  */
  YYSYMBOL_DIRECTIVE_BSET = 12,            /* DIRECTIVE_BSET  */
  YYSYMBOL_DIRECTIVE_MOVEP = 13,           /* DIRECTIVE_MOVEP  */
  YYSYMBOL_DIRECTIVE_MOVEA = 14,           /* DIRECTIVE_MOVEA  */
  YYSYMBOL_DIRECTIVE_MOVE = 15,            /* DIRECTIVE_MOVE  */
  YYSYMBOL_DIRECTIVE_NEGX = 16,            /* DIRECTIVE_NEGX  */
  YYSYMBOL_DIRECTIVE_CLR = 17,             /* DIRECTIVE_CLR  */
  YYSYMBOL_DIRECTIVE_NEG = 18,             /* DIRECTIVE_NEG  */
  YYSYMBOL_DIRECTIVE_NOT = 19,             /* DIRECTIVE_NOT  */
  YYSYMBOL_DIRECTIVE_EXT = 20,             /* DIRECTIVE_EXT  */
  YYSYMBOL_DIRECTIVE_NBCD = 21,            /* DIRECTIVE_NBCD  */
  YYSYMBOL_DIRECTIVE_SWAP = 22,            /* DIRECTIVE_SWAP  */
  YYSYMBOL_DIRECTIVE_PEA = 23,             /* DIRECTIVE_PEA  */
  YYSYMBOL_DIRECTIVE_ILLEGAL = 24,         /* DIRECTIVE_ILLEGAL  */
  YYSYMBOL_DIRECTIVE_TAS = 25,             /* DIRECTIVE_TAS  */
  YYSYMBOL_DIRECTIVE_TST = 26,             /* DIRECTIVE_TST  */
  YYSYMBOL_DIRECTIVE_TRAP = 27,            /* DIRECTIVE_TRAP  */
  YYSYMBOL_DIRECTIVE_LINK = 28,            /* DIRECTIVE_LINK  */
  YYSYMBOL_DIRECTIVE_UNLK = 29,            /* DIRECTIVE_UNLK  */
  YYSYMBOL_DIRECTIVE_RESET = 30,           /* DIRECTIVE_RESET  */
  YYSYMBOL_DIRECTIVE_NOP = 31,             /* DIRECTIVE_NOP  */
  YYSYMBOL_DIRECTIVE_STOP = 32,            /* DIRECTIVE_STOP  */
  YYSYMBOL_DIRECTIVE_RTE = 33,             /* DIRECTIVE_RTE  */
  YYSYMBOL_DIRECTIVE_RTS = 34,             /* DIRECTIVE_RTS  */
  YYSYMBOL_DIRECTIVE_TRAPV = 35,           /* DIRECTIVE_TRAPV  */
  YYSYMBOL_DIRECTIVE_RTR = 36,             /* DIRECTIVE_RTR  */
  YYSYMBOL_DIRECTIVE_JSR = 37,             /* DIRECTIVE_JSR  */
  YYSYMBOL_DIRECTIVE_JMP = 38,             /* DIRECTIVE_JMP  */
  YYSYMBOL_DIRECTIVE_MOVEM = 39,           /* DIRECTIVE_MOVEM  */
  YYSYMBOL_DIRECTIVE_LEA = 40,             /* DIRECTIVE_LEA  */
  YYSYMBOL_DIRECTIVE_CHK = 41,             /* DIRECTIVE_CHK  */
  YYSYMBOL_DIRECTIVE_ADDQ = 42,            /* DIRECTIVE_ADDQ  */
  YYSYMBOL_DIRECTIVE_SUBQ = 43,            /* DIRECTIVE_SUBQ  */
  YYSYMBOL_DIRECTIVE_ST = 44,              /* DIRECTIVE_ST  */
  YYSYMBOL_DIRECTIVE_SF = 45,              /* DIRECTIVE_SF  */
  YYSYMBOL_DIRECTIVE_SHI = 46,             /* DIRECTIVE_SHI  */
  YYSYMBOL_DIRECTIVE_SLS = 47,             /* DIRECTIVE_SLS  */
  YYSYMBOL_DIRECTIVE_SCC = 48,             /* DIRECTIVE_SCC  */
  YYSYMBOL_DIRECTIVE_SCS = 49,             /* DIRECTIVE_SCS  */
  YYSYMBOL_DIRECTIVE_SNE = 50,             /* DIRECTIVE_SNE  */
  YYSYMBOL_DIRECTIVE_SEQ = 51,             /* DIRECTIVE_SEQ  */
  YYSYMBOL_DIRECTIVE_SVC = 52,             /* DIRECTIVE_SVC  */
  YYSYMBOL_DIRECTIVE_SVS = 53,             /* DIRECTIVE_SVS  */
  YYSYMBOL_DIRECTIVE_SPL = 54,             /* DIRECTIVE_SPL  */
  YYSYMBOL_DIRECTIVE_SMI = 55,             /* DIRECTIVE_SMI  */
  YYSYMBOL_DIRECTIVE_SGE = 56,             /* DIRECTIVE_SGE  */
  YYSYMBOL_DIRECTIVE_SLT = 57,             /* DIRECTIVE_SLT  */
  YYSYMBOL_DIRECTIVE_SGT = 58,             /* DIRECTIVE_SGT  */
  YYSYMBOL_DIRECTIVE_SLE = 59,             /* DIRECTIVE_SLE  */
  YYSYMBOL_DIRECTIVE_DBT = 60,             /* DIRECTIVE_DBT  */
  YYSYMBOL_DIRECTIVE_DBF = 61,             /* DIRECTIVE_DBF  */
  YYSYMBOL_DIRECTIVE_DBHI = 62,            /* DIRECTIVE_DBHI  */
  YYSYMBOL_DIRECTIVE_DBLS = 63,            /* DIRECTIVE_DBLS  */
  YYSYMBOL_DIRECTIVE_DBCC = 64,            /* DIRECTIVE_DBCC  */
  YYSYMBOL_DIRECTIVE_DBCS = 65,            /* DIRECTIVE_DBCS  */
  YYSYMBOL_DIRECTIVE_DBNE = 66,            /* DIRECTIVE_DBNE  */
  YYSYMBOL_DIRECTIVE_DBEQ = 67,            /* DIRECTIVE_DBEQ  */
  YYSYMBOL_DIRECTIVE_DBVC = 68,            /* DIRECTIVE_DBVC  */
  YYSYMBOL_DIRECTIVE_DBVS = 69,            /* DIRECTIVE_DBVS  */
  YYSYMBOL_DIRECTIVE_DBPL = 70,            /* DIRECTIVE_DBPL  */
  YYSYMBOL_DIRECTIVE_DBMI = 71,            /* DIRECTIVE_DBMI  */
  YYSYMBOL_DIRECTIVE_DBGE = 72,            /* DIRECTIVE_DBGE  */
  YYSYMBOL_DIRECTIVE_DBLT = 73,            /* DIRECTIVE_DBLT  */
  YYSYMBOL_DIRECTIVE_DBGT = 74,            /* DIRECTIVE_DBGT  */
  YYSYMBOL_DIRECTIVE_DBLE = 75,            /* DIRECTIVE_DBLE  */
  YYSYMBOL_DIRECTIVE_BRA = 76,             /* DIRECTIVE_BRA  */
  YYSYMBOL_DIRECTIVE_BSR = 77,             /* DIRECTIVE_BSR  */
  YYSYMBOL_DIRECTIVE_BHI = 78,             /* DIRECTIVE_BHI  */
  YYSYMBOL_DIRECTIVE_BLS = 79,             /* DIRECTIVE_BLS  */
  YYSYMBOL_DIRECTIVE_BCC = 80,             /* DIRECTIVE_BCC  */
  YYSYMBOL_DIRECTIVE_BCS = 81,             /* DIRECTIVE_BCS  */
  YYSYMBOL_DIRECTIVE_BNE = 82,             /* DIRECTIVE_BNE  */
  YYSYMBOL_DIRECTIVE_BEQ = 83,             /* DIRECTIVE_BEQ  */
  YYSYMBOL_DIRECTIVE_BVC = 84,             /* DIRECTIVE_BVC  */
  YYSYMBOL_DIRECTIVE_BVS = 85,             /* DIRECTIVE_BVS  */
  YYSYMBOL_DIRECTIVE_BPL = 86,             /* DIRECTIVE_BPL  */
  YYSYMBOL_DIRECTIVE_BMI = 87,             /* DIRECTIVE_BMI  */
  YYSYMBOL_DIRECTIVE_BGE = 88,             /* DIRECTIVE_BGE  */
  YYSYMBOL_DIRECTIVE_BLT = 89,             /* DIRECTIVE_BLT  */
  YYSYMBOL_DIRECTIVE_BGT = 90,             /* DIRECTIVE_BGT  */
  YYSYMBOL_DIRECTIVE_BLE = 91,             /* DIRECTIVE_BLE  */
  YYSYMBOL_DIRECTIVE_MOVEQ = 92,           /* DIRECTIVE_MOVEQ  */
  YYSYMBOL_DIRECTIVE_DIVU = 93,            /* DIRECTIVE_DIVU  */
  YYSYMBOL_DIRECTIVE_DIVS = 94,            /* DIRECTIVE_DIVS  */
  YYSYMBOL_DIRECTIVE_SBCD = 95,            /* DIRECTIVE_SBCD  */
  YYSYMBOL_DIRECTIVE_OR = 96,              /* DIRECTIVE_OR  */
  YYSYMBOL_DIRECTIVE_SUB = 97,             /* DIRECTIVE_SUB  */
  YYSYMBOL_DIRECTIVE_SUBX = 98,            /* DIRECTIVE_SUBX  */
  YYSYMBOL_DIRECTIVE_SUBA = 99,            /* DIRECTIVE_SUBA  */
  YYSYMBOL_DIRECTIVE_EOR = 100,            /* DIRECTIVE_EOR  */
  YYSYMBOL_DIRECTIVE_CMPM = 101,           /* DIRECTIVE_CMPM  */
  YYSYMBOL_DIRECTIVE_CMP = 102,            /* DIRECTIVE_CMP  */
  YYSYMBOL_DIRECTIVE_CMPA = 103,           /* DIRECTIVE_CMPA  */
  YYSYMBOL_DIRECTIVE_MULU = 104,           /* DIRECTIVE_MULU  */
  YYSYMBOL_DIRECTIVE_MULS = 105,           /* DIRECTIVE_MULS  */
  YYSYMBOL_DIRECTIVE_ABCD = 106,           /* DIRECTIVE_ABCD  */
  YYSYMBOL_DIRECTIVE_EXG = 107,            /* DIRECTIVE_EXG  */
  YYSYMBOL_DIRECTIVE_AND = 108,            /* DIRECTIVE_AND  */
  YYSYMBOL_DIRECTIVE_ADD = 109,            /* DIRECTIVE_ADD  */
  YYSYMBOL_DIRECTIVE_ADDX = 110,           /* DIRECTIVE_ADDX  */
  YYSYMBOL_DIRECTIVE_ADDA = 111,           /* DIRECTIVE_ADDA  */
  YYSYMBOL_DIRECTIVE_ASL = 112,            /* DIRECTIVE_ASL  */
  YYSYMBOL_DIRECTIVE_ASR = 113,            /* DIRECTIVE_ASR  */
  YYSYMBOL_DIRECTIVE_LSL = 114,            /* DIRECTIVE_LSL  */
  YYSYMBOL_DIRECTIVE_LSR = 115,            /* DIRECTIVE_LSR  */
  YYSYMBOL_DIRECTIVE_ROXL = 116,           /* DIRECTIVE_ROXL  */
  YYSYMBOL_DIRECTIVE_ROXR = 117,           /* DIRECTIVE_ROXR  */
  YYSYMBOL_DIRECTIVE_ROL = 118,            /* DIRECTIVE_ROL  */
  YYSYMBOL_DIRECTIVE_ROR = 119,            /* DIRECTIVE_ROR  */
  YYSYMBOL_DIRECTIVE_DC = 120,             /* DIRECTIVE_DC  */
  YYSYMBOL_DIRECTIVE_DCB = 121,            /* DIRECTIVE_DCB  */
  YYSYMBOL_DIRECTIVE_DS = 122,             /* DIRECTIVE_DS  */
  YYSYMBOL_DIRECTIVE_REPT = 123,           /* DIRECTIVE_REPT  */
  YYSYMBOL_DIRECTIVE_ENDR = 124,           /* DIRECTIVE_ENDR  */
  YYSYMBOL_DIRECTIVE_MACRO = 125,          /* DIRECTIVE_MACRO  */
  YYSYMBOL_DIRECTIVE_MACROS = 126,         /* DIRECTIVE_MACROS  */
  YYSYMBOL_DIRECTIVE_ENDM = 127,           /* DIRECTIVE_ENDM  */
  YYSYMBOL_DIRECTIVE_INCLUDE = 128,        /* DIRECTIVE_INCLUDE  */
  YYSYMBOL_DIRECTIVE_INCBIN = 129,         /* DIRECTIVE_INCBIN  */
  YYSYMBOL_DIRECTIVE_EQU = 130,            /* DIRECTIVE_EQU  */
  YYSYMBOL_DIRECTIVE_EQUS = 131,           /* DIRECTIVE_EQUS  */
  YYSYMBOL_DIRECTIVE_SUBSTR = 132,         /* DIRECTIVE_SUBSTR  */
  YYSYMBOL_DIRECTIVE_SET = 133,            /* DIRECTIVE_SET  */
  YYSYMBOL_DIRECTIVE_IF = 134,             /* DIRECTIVE_IF  */
  YYSYMBOL_DIRECTIVE_ELSEIF = 135,         /* DIRECTIVE_ELSEIF  */
  YYSYMBOL_DIRECTIVE_ELSE = 136,           /* DIRECTIVE_ELSE  */
  YYSYMBOL_DIRECTIVE_ENDC = 137,           /* DIRECTIVE_ENDC  */
  YYSYMBOL_DIRECTIVE_WHILE = 138,          /* DIRECTIVE_WHILE  */
  YYSYMBOL_DIRECTIVE_ENDW = 139,           /* DIRECTIVE_ENDW  */
  YYSYMBOL_DIRECTIVE_EVEN = 140,           /* DIRECTIVE_EVEN  */
  YYSYMBOL_DIRECTIVE_CNOP = 141,           /* DIRECTIVE_CNOP  */
  YYSYMBOL_DIRECTIVE_INFORM = 142,         /* DIRECTIVE_INFORM  */
  YYSYMBOL_DIRECTIVE_FAIL = 143,           /* DIRECTIVE_FAIL  */
  YYSYMBOL_DIRECTIVE_END = 144,            /* DIRECTIVE_END  */
  YYSYMBOL_DIRECTIVE_RS = 145,             /* DIRECTIVE_RS  */
  YYSYMBOL_DIRECTIVE_RSSET = 146,          /* DIRECTIVE_RSSET  */
  YYSYMBOL_DIRECTIVE_RSRESET = 147,        /* DIRECTIVE_RSRESET  */
  YYSYMBOL_DIRECTIVE_OBJ = 148,            /* DIRECTIVE_OBJ  */
  YYSYMBOL_DIRECTIVE_OBJEND = 149,         /* DIRECTIVE_OBJEND  */
  YYSYMBOL_DIRECTIVE_ORG = 150,            /* DIRECTIVE_ORG  */
  YYSYMBOL_DIRECTIVE_PUSHO = 151,          /* DIRECTIVE_PUSHO  */
  YYSYMBOL_DIRECTIVE_POPO = 152,           /* DIRECTIVE_POPO  */
  YYSYMBOL_DIRECTIVE_OPT = 153,            /* DIRECTIVE_OPT  */
  YYSYMBOL_DIRECTIVE_PUSHP = 154,          /* DIRECTIVE_PUSHP  */
  YYSYMBOL_DIRECTIVE_POPP = 155,           /* DIRECTIVE_POPP  */
  YYSYMBOL_DIRECTIVE_SHIFT = 156,          /* DIRECTIVE_SHIFT  */
  YYSYMBOL_DIRECTIVE_MEXIT = 157,          /* DIRECTIVE_MEXIT  */
  YYSYMBOL_DIRECTIVE_LOCAL = 158,          /* DIRECTIVE_LOCAL  */
  YYSYMBOL_DIRECTIVE_LIST = 159,           /* DIRECTIVE_LIST  */
  YYSYMBOL_DIRECTIVE_NOLIST = 160,         /* DIRECTIVE_NOLIST  */
  YYSYMBOL_DIRECTIVE_PURGE = 161,          /* DIRECTIVE_PURGE  */
  YYSYMBOL_SIZE_BYTE = 162,                /* SIZE_BYTE  */
  YYSYMBOL_SIZE_SHORT = 163,               /* SIZE_SHORT  */
  YYSYMBOL_SIZE_WORD = 164,                /* SIZE_WORD  */
  YYSYMBOL_SIZE_LONGWORD = 165,            /* SIZE_LONGWORD  */
  YYSYMBOL_DATA_REGISTER = 166,            /* DATA_REGISTER  */
  YYSYMBOL_ADDRESS_REGISTER = 167,         /* ADDRESS_REGISTER  */
  YYSYMBOL_NUMBER = 168,                   /* NUMBER  */
  YYSYMBOL_IDENTIFIER = 169,               /* IDENTIFIER  */
  YYSYMBOL_LOCAL_IDENTIFIER = 170,         /* LOCAL_IDENTIFIER  */
  YYSYMBOL_STRING = 171,                   /* STRING  */
  YYSYMBOL_OPTION = 172,                   /* OPTION  */
  YYSYMBOL_STATUS_REGISTER = 173,          /* STATUS_REGISTER  */
  YYSYMBOL_CONDITION_CODE_REGISTER = 174,  /* CONDITION_CODE_REGISTER  */
  YYSYMBOL_USER_STACK_POINTER_REGISTER = 175, /* USER_STACK_POINTER_REGISTER  */
  YYSYMBOL_PROGRAM_COUNTER = 176,          /* PROGRAM_COUNTER  */
  YYSYMBOL_LOGICAL_AND = 177,              /* LOGICAL_AND  */
  YYSYMBOL_LOGICAL_OR = 178,               /* LOGICAL_OR  */
  YYSYMBOL_EQUALITY = 179,                 /* EQUALITY  */
  YYSYMBOL_INEQUALITY = 180,               /* INEQUALITY  */
  YYSYMBOL_LESS_OR_EQUAL = 181,            /* LESS_OR_EQUAL  */
  YYSYMBOL_MORE_OR_EQUAL = 182,            /* MORE_OR_EQUAL  */
  YYSYMBOL_LEFT_SHIFT = 183,               /* LEFT_SHIFT  */
  YYSYMBOL_RIGHT_SHIFT = 184,              /* RIGHT_SHIFT  */
  YYSYMBOL_STRLEN = 185,                   /* STRLEN  */
  YYSYMBOL_STRCMP = 186,                   /* STRCMP  */
  YYSYMBOL_INSTR = 187,                    /* INSTR  */
  YYSYMBOL_DEF = 188,                      /* DEF  */
  YYSYMBOL_TYPE = 189,                     /* TYPE  */
  YYSYMBOL_FILESIZE = 190,                 /* FILESIZE  */
  YYSYMBOL_OFFSET = 191,                   /* OFFSET  */
  YYSYMBOL_LOWER_PRECEDENCE_THAN_SIZE = 192, /* LOWER_PRECEDENCE_THAN_SIZE  */
  YYSYMBOL_HIGHER_PRECEDENCE_THAN_SIZE = 193, /* HIGHER_PRECEDENCE_THAN_SIZE  */
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


#line 367 "syntactic.cpp"

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
       0,   718,   718,   721,   726,   732,   739,   745,   750,   758,
     765,   774,   782,   787,   791,   798,   805,   811,   817,   824,
     830,   834,   839,   844,   849,   858,   866,   874,   881,   886,
     891,   896,   901,   905,   909,   913,   918,   922,   926,   932,
     938,   948,   952,   956,   962,   967,   974,   978,   983,   987,
     992,   996,  1000,  1005,  1010,  1015,  1019,  1023,  1028,  1033,
    1038,  1043,  1048,  1056,  1073,  1101,  1118,  1146,  1163,  1191,
    1197,  1203,  1212,  1217,  1223,  1231,  1235,  1239,  1243,  1247,
    1251,  1255,  1259,  1263,  1267,  1271,  1275,  1279,  1283,  1287,
    1291,  1295,  1299,  1303,  1307,  1311,  1315,  1319,  1323,  1327,
    1331,  1335,  1339,  1343,  1347,  1351,  1355,  1359,  1363,  1367,
    1371,  1375,  1379,  1383,  1387,  1391,  1395,  1400,  1405,  1410,
    1415,  1420,  1425,  1430,  1435,  1440,  1445,  1450,  1455,  1460,
    1465,  1470,  1475,  1480,  1485,  1490,  1495,  1500,  1505,  1510,
    1515,  1520,  1525,  1530,  1535,  1540,  1545,  1550,  1555,  1559,
    1563,  1568,  1573,  1578,  1583,  1588,  1593,  1598,  1603,  1608,
    1613,  1618,  1623,  1628,  1633,  1637,  1641,  1645,  1649,  1653,
    1657,  1661,  1665,  1669,  1673,  1677,  1681,  1685,  1689,  1693,
    1697,  1701,  1705,  1709,  1713,  1717,  1721,  1725,  1729,  1733,
    1737,  1741,  1748,  1752,  1756,  1760,  1768,  1773,  1778,  1783,
    1789,  1795,  1801,  1811,  1820,  1829,  1840,  1850,  1860,  1865,
    1870,  1875,  1884,  1892,  1900,  1910,  1919,  1929,  1945,  1949,
    1953,  1958,  1964,  1971,  2002,  2006,  2013,  2017,  2042,  2046,
    2059,  2064,  2070,  2078,  2082,  2087,  2092,  2100,  2104,  2109,
    2114,  2119,  2127,  2131,  2136,  2144,  2148,  2153,  2158,  2166,
    2170,  2175,  2180,  2185,  2193,  2197,  2202,  2210,  2214,  2218,
    2223,  2229,  2237,  2238,  2244,  2250,  2256,  2265,  2266,  2267,
    2279,  2284,  2289,  2293,  2297,  2301,  2305,  2310,  2315,  2325,
    2330,  2335,  2340,  2345,  2350,  2358
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
  "\"end of file\"", "error", "\"invalid token\"", "DIRECTIVE_ORI",
  "DIRECTIVE_ANDI", "DIRECTIVE_SUBI", "DIRECTIVE_ADDI", "DIRECTIVE_EORI",
  "DIRECTIVE_CMPI", "DIRECTIVE_BTST", "DIRECTIVE_BCHG", "DIRECTIVE_BCLR",
  "DIRECTIVE_BSET", "DIRECTIVE_MOVEP", "DIRECTIVE_MOVEA", "DIRECTIVE_MOVE",
  "DIRECTIVE_NEGX", "DIRECTIVE_CLR", "DIRECTIVE_NEG", "DIRECTIVE_NOT",
  "DIRECTIVE_EXT", "DIRECTIVE_NBCD", "DIRECTIVE_SWAP", "DIRECTIVE_PEA",
  "DIRECTIVE_ILLEGAL", "DIRECTIVE_TAS", "DIRECTIVE_TST", "DIRECTIVE_TRAP",
  "DIRECTIVE_LINK", "DIRECTIVE_UNLK", "DIRECTIVE_RESET", "DIRECTIVE_NOP",
  "DIRECTIVE_STOP", "DIRECTIVE_RTE", "DIRECTIVE_RTS", "DIRECTIVE_TRAPV",
  "DIRECTIVE_RTR", "DIRECTIVE_JSR", "DIRECTIVE_JMP", "DIRECTIVE_MOVEM",
  "DIRECTIVE_LEA", "DIRECTIVE_CHK", "DIRECTIVE_ADDQ", "DIRECTIVE_SUBQ",
  "DIRECTIVE_ST", "DIRECTIVE_SF", "DIRECTIVE_SHI", "DIRECTIVE_SLS",
  "DIRECTIVE_SCC", "DIRECTIVE_SCS", "DIRECTIVE_SNE", "DIRECTIVE_SEQ",
  "DIRECTIVE_SVC", "DIRECTIVE_SVS", "DIRECTIVE_SPL", "DIRECTIVE_SMI",
  "DIRECTIVE_SGE", "DIRECTIVE_SLT", "DIRECTIVE_SGT", "DIRECTIVE_SLE",
  "DIRECTIVE_DBT", "DIRECTIVE_DBF", "DIRECTIVE_DBHI", "DIRECTIVE_DBLS",
  "DIRECTIVE_DBCC", "DIRECTIVE_DBCS", "DIRECTIVE_DBNE", "DIRECTIVE_DBEQ",
  "DIRECTIVE_DBVC", "DIRECTIVE_DBVS", "DIRECTIVE_DBPL", "DIRECTIVE_DBMI",
  "DIRECTIVE_DBGE", "DIRECTIVE_DBLT", "DIRECTIVE_DBGT", "DIRECTIVE_DBLE",
  "DIRECTIVE_BRA", "DIRECTIVE_BSR", "DIRECTIVE_BHI", "DIRECTIVE_BLS",
  "DIRECTIVE_BCC", "DIRECTIVE_BCS", "DIRECTIVE_BNE", "DIRECTIVE_BEQ",
  "DIRECTIVE_BVC", "DIRECTIVE_BVS", "DIRECTIVE_BPL", "DIRECTIVE_BMI",
  "DIRECTIVE_BGE", "DIRECTIVE_BLT", "DIRECTIVE_BGT", "DIRECTIVE_BLE",
  "DIRECTIVE_MOVEQ", "DIRECTIVE_DIVU", "DIRECTIVE_DIVS", "DIRECTIVE_SBCD",
  "DIRECTIVE_OR", "DIRECTIVE_SUB", "DIRECTIVE_SUBX", "DIRECTIVE_SUBA",
  "DIRECTIVE_EOR", "DIRECTIVE_CMPM", "DIRECTIVE_CMP", "DIRECTIVE_CMPA",
  "DIRECTIVE_MULU", "DIRECTIVE_MULS", "DIRECTIVE_ABCD", "DIRECTIVE_EXG",
  "DIRECTIVE_AND", "DIRECTIVE_ADD", "DIRECTIVE_ADDX", "DIRECTIVE_ADDA",
  "DIRECTIVE_ASL", "DIRECTIVE_ASR", "DIRECTIVE_LSL", "DIRECTIVE_LSR",
  "DIRECTIVE_ROXL", "DIRECTIVE_ROXR", "DIRECTIVE_ROL", "DIRECTIVE_ROR",
  "DIRECTIVE_DC", "DIRECTIVE_DCB", "DIRECTIVE_DS", "DIRECTIVE_REPT",
  "DIRECTIVE_ENDR", "DIRECTIVE_MACRO", "DIRECTIVE_MACROS",
  "DIRECTIVE_ENDM", "DIRECTIVE_INCLUDE", "DIRECTIVE_INCBIN",
  "DIRECTIVE_EQU", "DIRECTIVE_EQUS", "DIRECTIVE_SUBSTR", "DIRECTIVE_SET",
  "DIRECTIVE_IF", "DIRECTIVE_ELSEIF", "DIRECTIVE_ELSE", "DIRECTIVE_ENDC",
  "DIRECTIVE_WHILE", "DIRECTIVE_ENDW", "DIRECTIVE_EVEN", "DIRECTIVE_CNOP",
  "DIRECTIVE_INFORM", "DIRECTIVE_FAIL", "DIRECTIVE_END", "DIRECTIVE_RS",
  "DIRECTIVE_RSSET", "DIRECTIVE_RSRESET", "DIRECTIVE_OBJ",
  "DIRECTIVE_OBJEND", "DIRECTIVE_ORG", "DIRECTIVE_PUSHO", "DIRECTIVE_POPO",
  "DIRECTIVE_OPT", "DIRECTIVE_PUSHP", "DIRECTIVE_POPP", "DIRECTIVE_SHIFT",
  "DIRECTIVE_MEXIT", "DIRECTIVE_LOCAL", "DIRECTIVE_LIST",
  "DIRECTIVE_NOLIST", "DIRECTIVE_PURGE", "SIZE_BYTE", "SIZE_SHORT",
  "SIZE_WORD", "SIZE_LONGWORD", "DATA_REGISTER", "ADDRESS_REGISTER",
  "NUMBER", "IDENTIFIER", "LOCAL_IDENTIFIER", "STRING", "OPTION",
  "STATUS_REGISTER", "CONDITION_CODE_REGISTER",
  "USER_STACK_POINTER_REGISTER", "PROGRAM_COUNTER", "LOGICAL_AND",
  "LOGICAL_OR", "EQUALITY", "INEQUALITY", "LESS_OR_EQUAL", "MORE_OR_EQUAL",
  "LEFT_SHIFT", "RIGHT_SHIFT", "STRLEN", "STRCMP", "INSTR", "DEF", "TYPE",
  "FILESIZE", "OFFSET", "LOWER_PRECEDENCE_THAN_SIZE",
  "HIGHER_PRECEDENCE_THAN_SIZE", "','", "'*'", "'='", "'+'", "'-'", "'.'",
  "'('", "')'", "'#'", "'/'", "'<'", "'>'", "'%'", "'&'", "'!'", "'|'",
  "'^'", "'~'", "'@'", "$accept", "statement", "expression_list",
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
#define yyclearin       (yychar = TOKEN_M68KASM_EMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == TOKEN_M68KASM_EMPTY)                                        \
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
   Use TOKEN_M68KASM_error or TOKEN_M68KASM_UNDEF. */
#define YYERRCODE TOKEN_M68KASM_UNDEF


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
    case YYSYMBOL_IDENTIFIER: /* IDENTIFIER  */
#line 701 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1778 "syntactic.cpp"
        break;

    case YYSYMBOL_LOCAL_IDENTIFIER: /* LOCAL_IDENTIFIER  */
#line 701 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1784 "syntactic.cpp"
        break;

    case YYSYMBOL_STRING: /* STRING  */
#line 701 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1790 "syntactic.cpp"
        break;

    case YYSYMBOL_OPTION: /* OPTION  */
#line 701 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1796 "syntactic.cpp"
        break;

    case YYSYMBOL_expression_list: /* expression_list  */
#line 703 "syntactic.y"
            { DestroyExpressionList(&((*yyvaluep).expression_list)); }
#line 1802 "syntactic.cpp"
        break;

    case YYSYMBOL_identifier_list: /* identifier_list  */
#line 704 "syntactic.y"
            { DestroyIdentifierList(&((*yyvaluep).identifier_list)); }
#line 1808 "syntactic.cpp"
        break;

    case YYSYMBOL_option_list: /* option_list  */
#line 704 "syntactic.y"
            { DestroyIdentifierList(&((*yyvaluep).identifier_list)); }
#line 1814 "syntactic.cpp"
        break;

    case YYSYMBOL_operand: /* operand  */
#line 702 "syntactic.y"
            { DestroyOperand(&((*yyvaluep).operand)); }
#line 1820 "syntactic.cpp"
        break;

    case YYSYMBOL_expression: /* expression  */
#line 705 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1826 "syntactic.cpp"
        break;

    case YYSYMBOL_expression1: /* expression1  */
#line 705 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1832 "syntactic.cpp"
        break;

    case YYSYMBOL_expression2: /* expression2  */
#line 705 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1838 "syntactic.cpp"
        break;

    case YYSYMBOL_expression3: /* expression3  */
#line 705 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1844 "syntactic.cpp"
        break;

    case YYSYMBOL_expression4: /* expression4  */
#line 705 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1850 "syntactic.cpp"
        break;

    case YYSYMBOL_expression5: /* expression5  */
#line 705 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1856 "syntactic.cpp"
        break;

    case YYSYMBOL_expression6: /* expression6  */
#line 705 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1862 "syntactic.cpp"
        break;

    case YYSYMBOL_expression7: /* expression7  */
#line 705 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1868 "syntactic.cpp"
        break;

    case YYSYMBOL_local_identifier: /* local_identifier  */
#line 701 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1874 "syntactic.cpp"
        break;

    case YYSYMBOL_identifier: /* identifier  */
#line 701 "syntactic.y"
            { String_Destroy(&((*yyvaluep).string)); }
#line 1880 "syntactic.cpp"
        break;

    case YYSYMBOL_expression8: /* expression8  */
#line 705 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1886 "syntactic.cpp"
        break;

    case YYSYMBOL_string: /* string  */
#line 705 "syntactic.y"
            { DestroyExpression(&((*yyvaluep).expression)); }
#line 1892 "syntactic.cpp"
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

  yychar = TOKEN_M68KASM_EMPTY; /* Cause a token to be read.  */

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
  if (yychar == TOKEN_M68KASM_EMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, scanner);
    }

  if (yychar <= TOKEN_YYEOF)
    {
      yychar = TOKEN_YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == TOKEN_M68KASM_error)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = TOKEN_M68KASM_UNDEF;
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
  yychar = TOKEN_M68KASM_EMPTY;
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
#line 718 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 2173 "syntactic.cpp"
    break;

  case 3: /* statement: instruction  */
#line 722 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INSTRUCTION;
		statement->shared.instruction = (yyvsp[0].instruction);
	}
#line 2182 "syntactic.cpp"
    break;

  case 4: /* statement: DIRECTIVE_DC size expression_list  */
#line 727 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DC;
		statement->shared.dc.size = (yyvsp[-1].size);
		statement->shared.dc.values = (yyvsp[0].expression_list);
	}
#line 2192 "syntactic.cpp"
    break;

  case 5: /* statement: DIRECTIVE_DCB size expression ',' expression  */
#line 733 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DCB;
		statement->shared.dcb.size = (yyvsp[-3].size);
		statement->shared.dcb.repetitions = (yyvsp[-2].expression);
		statement->shared.dcb.value = (yyvsp[0].expression);
	}
#line 2203 "syntactic.cpp"
    break;

  case 6: /* statement: DIRECTIVE_DS size expression  */
#line 740 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DS;
		statement->shared.ds.size = (yyvsp[-1].size);
		statement->shared.ds.length = (yyvsp[0].expression);
	}
#line 2213 "syntactic.cpp"
    break;

  case 7: /* statement: DIRECTIVE_INCLUDE STRING  */
#line 746 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCLUDE;
		statement->shared.include.path = (yyvsp[0].string);
	}
#line 2222 "syntactic.cpp"
    break;

  case 8: /* statement: DIRECTIVE_INCBIN STRING  */
#line 751 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yyvsp[0].string);
		statement->shared.incbin.start.type = EXPRESSION_NUMBER;
		statement->shared.incbin.start.shared.unsigned_long = 0;
		statement->shared.incbin.has_length = cc_false;
	}
#line 2234 "syntactic.cpp"
    break;

  case 9: /* statement: DIRECTIVE_INCBIN STRING ',' expression  */
#line 759 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yyvsp[-2].string);
		statement->shared.incbin.start = (yyvsp[0].expression);
		statement->shared.incbin.has_length = cc_false;
	}
#line 2245 "syntactic.cpp"
    break;

  case 10: /* statement: DIRECTIVE_INCBIN STRING ',' ',' expression  */
#line 766 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yyvsp[-3].string);
		statement->shared.incbin.start.type = EXPRESSION_NUMBER;
		statement->shared.incbin.start.shared.unsigned_long = 0;
		statement->shared.incbin.has_length = cc_true;
		statement->shared.incbin.length = (yyvsp[0].expression);
	}
#line 2258 "syntactic.cpp"
    break;

  case 11: /* statement: DIRECTIVE_INCBIN STRING ',' expression ',' expression  */
#line 775 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yyvsp[-4].string);
		statement->shared.incbin.start = (yyvsp[-2].expression);
		statement->shared.incbin.has_length = cc_true;
		statement->shared.incbin.length = (yyvsp[0].expression);
	}
#line 2270 "syntactic.cpp"
    break;

  case 12: /* statement: DIRECTIVE_REPT expression  */
#line 783 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_REPT;
		statement->shared.rept.repetitions = (yyvsp[0].expression);
	}
#line 2279 "syntactic.cpp"
    break;

  case 13: /* statement: DIRECTIVE_ENDR  */
#line 788 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDR;
	}
#line 2287 "syntactic.cpp"
    break;

  case 14: /* statement: DIRECTIVE_MACRO  */
#line 792 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 2298 "syntactic.cpp"
    break;

  case 15: /* statement: DIRECTIVE_MACRO '*'  */
#line 799 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_true;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 2309 "syntactic.cpp"
    break;

  case 16: /* statement: DIRECTIVE_MACRO identifier_list  */
#line 806 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names = (yyvsp[0].identifier_list);
	}
#line 2319 "syntactic.cpp"
    break;

  case 17: /* statement: DIRECTIVE_MACRO '*' ',' identifier_list  */
#line 812 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_true;
		statement->shared.macro.parameter_names = (yyvsp[0].identifier_list);
	}
#line 2329 "syntactic.cpp"
    break;

  case 18: /* statement: DIRECTIVE_MACROS  */
#line 818 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 2340 "syntactic.cpp"
    break;

  case 19: /* statement: DIRECTIVE_MACROS identifier_list  */
#line 825 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names = (yyvsp[0].identifier_list);
	}
#line 2350 "syntactic.cpp"
    break;

  case 20: /* statement: DIRECTIVE_ENDM  */
#line 831 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDM;
	}
#line 2358 "syntactic.cpp"
    break;

  case 21: /* statement: DIRECTIVE_EQU expression  */
#line 835 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQU;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2367 "syntactic.cpp"
    break;

  case 22: /* statement: DIRECTIVE_EQUS STRING  */
#line 840 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQUS_STRING;
		statement->shared.string = (yyvsp[0].string);
	}
#line 2376 "syntactic.cpp"
    break;

  case 23: /* statement: DIRECTIVE_EQUS IDENTIFIER  */
#line 845 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQUS_IDENTIFIER;
		statement->shared.string = (yyvsp[0].string);
	}
#line 2385 "syntactic.cpp"
    break;

  case 24: /* statement: DIRECTIVE_SUBSTR ',' ',' STRING  */
#line 850 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start.type = EXPRESSION_NUMBER;
		statement->shared.substr.start.shared.unsigned_long = 1;
		statement->shared.substr.end.type = EXPRESSION_NUMBER;
		statement->shared.substr.end.shared.unsigned_long = String_Length(&(yyvsp[0].string));
		statement->shared.substr.string = (yyvsp[0].string);
	}
#line 2398 "syntactic.cpp"
    break;

  case 25: /* statement: DIRECTIVE_SUBSTR expression ',' ',' STRING  */
#line 859 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start = (yyvsp[-3].expression);
		statement->shared.substr.end.type = EXPRESSION_NUMBER;
		statement->shared.substr.end.shared.unsigned_long = String_Length(&(yyvsp[0].string));
		statement->shared.substr.string = (yyvsp[0].string);
	}
#line 2410 "syntactic.cpp"
    break;

  case 26: /* statement: DIRECTIVE_SUBSTR ',' expression ',' STRING  */
#line 867 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start.type = EXPRESSION_NUMBER;
		statement->shared.substr.start.shared.unsigned_long = 1;
		statement->shared.substr.end = (yyvsp[-2].expression);
		statement->shared.substr.string = (yyvsp[0].string);
	}
#line 2422 "syntactic.cpp"
    break;

  case 27: /* statement: DIRECTIVE_SUBSTR expression ',' expression ',' STRING  */
#line 875 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start = (yyvsp[-4].expression);
		statement->shared.substr.end = (yyvsp[-2].expression);
		statement->shared.substr.string = (yyvsp[0].string);
	}
#line 2433 "syntactic.cpp"
    break;

  case 28: /* statement: DIRECTIVE_SET expression  */
#line 882 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2442 "syntactic.cpp"
    break;

  case 29: /* statement: '=' expression  */
#line 887 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2451 "syntactic.cpp"
    break;

  case 30: /* statement: DIRECTIVE_IF expression  */
#line 892 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_IF;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2460 "syntactic.cpp"
    break;

  case 31: /* statement: DIRECTIVE_ELSEIF expression  */
#line 897 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSEIF;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2469 "syntactic.cpp"
    break;

  case 32: /* statement: DIRECTIVE_ELSEIF  */
#line 902 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSE;
	}
#line 2477 "syntactic.cpp"
    break;

  case 33: /* statement: DIRECTIVE_ELSE  */
#line 906 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSE;
	}
#line 2485 "syntactic.cpp"
    break;

  case 34: /* statement: DIRECTIVE_ENDC  */
#line 910 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDC;
	}
#line 2493 "syntactic.cpp"
    break;

  case 35: /* statement: DIRECTIVE_WHILE expression  */
#line 914 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_WHILE;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2502 "syntactic.cpp"
    break;

  case 36: /* statement: DIRECTIVE_ENDW  */
#line 919 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDW;
	}
#line 2510 "syntactic.cpp"
    break;

  case 37: /* statement: DIRECTIVE_EVEN  */
#line 923 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EVEN;
	}
#line 2518 "syntactic.cpp"
    break;

  case 38: /* statement: DIRECTIVE_CNOP expression ',' expression  */
#line 927 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_CNOP;
		statement->shared.cnop.offset = (yyvsp[-2].expression);
		statement->shared.cnop.size_boundary = (yyvsp[0].expression);
	}
#line 2528 "syntactic.cpp"
    break;

  case 39: /* statement: DIRECTIVE_INFORM expression ',' STRING  */
#line 933 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.severity = (yyvsp[-2].expression);
		statement->shared.inform.message = (yyvsp[0].string);
	}
#line 2538 "syntactic.cpp"
    break;

  case 40: /* statement: DIRECTIVE_INFORM expression ',' STRING ',' expression_list  */
#line 939 "syntactic.y"
        {
		(void)(yyvsp[0].expression_list);

		/* TODO - parameters */
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.severity = (yyvsp[-4].expression);
		statement->shared.inform.message = (yyvsp[-2].string);
		DestroyExpressionList(&(yyvsp[0].expression_list));
	}
#line 2552 "syntactic.cpp"
    break;

  case 41: /* statement: DIRECTIVE_FAIL  */
#line 949 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_FAIL;
	}
#line 2560 "syntactic.cpp"
    break;

  case 42: /* statement: DIRECTIVE_END  */
#line 953 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_END;
	}
#line 2568 "syntactic.cpp"
    break;

  case 43: /* statement: DIRECTIVE_RS size expression  */
#line 957 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RS;
		statement->shared.rs.size = (yyvsp[-1].size);
		statement->shared.rs.length = (yyvsp[0].expression);
	}
#line 2578 "syntactic.cpp"
    break;

  case 44: /* statement: DIRECTIVE_RSSET expression  */
#line 963 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RSSET;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2587 "syntactic.cpp"
    break;

  case 45: /* statement: DIRECTIVE_RSRESET expression  */
#line 968 "syntactic.y"
        {
		/* As mentioned in S.N. 68k's manual, this odd hack
		   is necessary for compatibility with other assemblers. */
		statement->type = STATEMENT_TYPE_RSSET;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2598 "syntactic.cpp"
    break;

  case 46: /* statement: DIRECTIVE_RSRESET  */
#line 975 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RSRESET;
	}
#line 2606 "syntactic.cpp"
    break;

  case 47: /* statement: DIRECTIVE_OBJ expression  */
#line 979 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OBJ;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2615 "syntactic.cpp"
    break;

  case 48: /* statement: DIRECTIVE_OBJEND  */
#line 984 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OBJEND;
	}
#line 2623 "syntactic.cpp"
    break;

  case 49: /* statement: DIRECTIVE_ORG expression  */
#line 988 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ORG;
		statement->shared.expression = (yyvsp[0].expression);
	}
#line 2632 "syntactic.cpp"
    break;

  case 50: /* statement: DIRECTIVE_PUSHO  */
#line 993 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PUSHO;
	}
#line 2640 "syntactic.cpp"
    break;

  case 51: /* statement: DIRECTIVE_POPO  */
#line 997 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_POPO;
	}
#line 2648 "syntactic.cpp"
    break;

  case 52: /* statement: DIRECTIVE_OPT option_list  */
#line 1001 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OPT;
		statement->shared.opt.options = (yyvsp[0].identifier_list);
	}
#line 2657 "syntactic.cpp"
    break;

  case 53: /* statement: DIRECTIVE_PUSHP STRING  */
#line 1006 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PUSHP;
		statement->shared.string = (yyvsp[0].string);
	}
#line 2666 "syntactic.cpp"
    break;

  case 54: /* statement: DIRECTIVE_POPP IDENTIFIER  */
#line 1011 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_POPP;
		statement->shared.string = (yyvsp[0].string);
	}
#line 2675 "syntactic.cpp"
    break;

  case 55: /* statement: DIRECTIVE_SHIFT  */
#line 1016 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SHIFT;
	}
#line 2683 "syntactic.cpp"
    break;

  case 56: /* statement: DIRECTIVE_MEXIT  */
#line 1020 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MEXIT;
	}
#line 2691 "syntactic.cpp"
    break;

  case 57: /* statement: DIRECTIVE_LOCAL identifier_list  */
#line 1024 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_LOCAL;
		statement->shared.local.identifiers = (yyvsp[0].identifier_list);
	}
#line 2700 "syntactic.cpp"
    break;

  case 58: /* statement: DIRECTIVE_LIST  */
#line 1029 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 2709 "syntactic.cpp"
    break;

  case 59: /* statement: DIRECTIVE_LIST '+'  */
#line 1034 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 2718 "syntactic.cpp"
    break;

  case 60: /* statement: DIRECTIVE_LIST '-'  */
#line 1039 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 2727 "syntactic.cpp"
    break;

  case 61: /* statement: DIRECTIVE_NOLIST  */
#line 1044 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 2736 "syntactic.cpp"
    break;

  case 62: /* statement: DIRECTIVE_PURGE IDENTIFIER  */
#line 1049 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PURGE;
		statement->shared.string = (yyvsp[0].string);
	}
#line 2745 "syntactic.cpp"
    break;

  case 63: /* expression_list: expression  */
#line 1057 "syntactic.y"
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
#line 2766 "syntactic.cpp"
    break;

  case 64: /* expression_list: expression_list ',' expression  */
#line 1074 "syntactic.y"
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
#line 2795 "syntactic.cpp"
    break;

  case 65: /* identifier_list: IDENTIFIER  */
#line 1102 "syntactic.y"
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
#line 2816 "syntactic.cpp"
    break;

  case 66: /* identifier_list: identifier_list ',' IDENTIFIER  */
#line 1119 "syntactic.y"
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
#line 2845 "syntactic.cpp"
    break;

  case 67: /* option_list: OPTION  */
#line 1147 "syntactic.y"
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
#line 2866 "syntactic.cpp"
    break;

  case 68: /* option_list: option_list ',' OPTION  */
#line 1164 "syntactic.y"
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
#line 2895 "syntactic.cpp"
    break;

  case 69: /* instruction: full_opcode  */
#line 1192 "syntactic.y"
        {
		(yyval.instruction).opcode = (yyvsp[0].opcode);
		(yyval.instruction).operands[0].type = OPERAND_NONE;
		(yyval.instruction).operands[1].type = OPERAND_NONE;
	}
#line 2905 "syntactic.cpp"
    break;

  case 70: /* instruction: full_opcode operand  */
#line 1198 "syntactic.y"
        {
		(yyval.instruction).opcode = (yyvsp[-1].opcode);
		(yyval.instruction).operands[0] = (yyvsp[0].operand);
		(yyval.instruction).operands[1].type = OPERAND_NONE;
	}
#line 2915 "syntactic.cpp"
    break;

  case 71: /* instruction: full_opcode operand ',' operand  */
#line 1204 "syntactic.y"
        {
		(yyval.instruction).opcode = (yyvsp[-3].opcode);
		(yyval.instruction).operands[0] = (yyvsp[-2].operand);
		(yyval.instruction).operands[1] = (yyvsp[0].operand);
	}
#line 2925 "syntactic.cpp"
    break;

  case 72: /* full_opcode: opcode  */
#line 1213 "syntactic.y"
        {
		(yyval.opcode) = (yyvsp[0].opcode);
		(yyval.opcode).size = SIZE_UNDEFINED;
	}
#line 2934 "syntactic.cpp"
    break;

  case 73: /* full_opcode: opcode '.'  */
#line 1218 "syntactic.y"
        {
		(yyval.opcode) = (yyvsp[-1].opcode);
		(yyval.opcode).size = SIZE_UNDEFINED;
		m68kasm_warning_pedantic(scanner, statement, "Opcode has a dot but no size; either remove the dot or add an explicit size.");
	}
#line 2944 "syntactic.cpp"
    break;

  case 74: /* full_opcode: opcode size  */
#line 1224 "syntactic.y"
        {
		(yyval.opcode) = (yyvsp[-1].opcode);
		(yyval.opcode).size = (yyvsp[0].size);
	}
#line 2953 "syntactic.cpp"
    break;

  case 75: /* opcode: DIRECTIVE_ORI  */
#line 1232 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ORI;
	}
#line 2961 "syntactic.cpp"
    break;

  case 76: /* opcode: DIRECTIVE_ANDI  */
#line 1236 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ANDI;
	}
#line 2969 "syntactic.cpp"
    break;

  case 77: /* opcode: DIRECTIVE_SUBI  */
#line 1240 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SUBI;
	}
#line 2977 "syntactic.cpp"
    break;

  case 78: /* opcode: DIRECTIVE_ADDI  */
#line 1244 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ADDI;
	}
#line 2985 "syntactic.cpp"
    break;

  case 79: /* opcode: DIRECTIVE_EORI  */
#line 1248 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_EORI;
	}
#line 2993 "syntactic.cpp"
    break;

  case 80: /* opcode: DIRECTIVE_CMPI  */
#line 1252 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CMPI;
	}
#line 3001 "syntactic.cpp"
    break;

  case 81: /* opcode: DIRECTIVE_BTST  */
#line 1256 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BTST_STATIC;
	}
#line 3009 "syntactic.cpp"
    break;

  case 82: /* opcode: DIRECTIVE_BCHG  */
#line 1260 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BCHG_STATIC;
	}
#line 3017 "syntactic.cpp"
    break;

  case 83: /* opcode: DIRECTIVE_BCLR  */
#line 1264 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BCLR_STATIC;
	}
#line 3025 "syntactic.cpp"
    break;

  case 84: /* opcode: DIRECTIVE_BSET  */
#line 1268 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BSET_STATIC;
	}
#line 3033 "syntactic.cpp"
    break;

  case 85: /* opcode: DIRECTIVE_MOVEP  */
#line 1272 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MOVEP_TO_REG;
	}
#line 3041 "syntactic.cpp"
    break;

  case 86: /* opcode: DIRECTIVE_MOVEA  */
#line 1276 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MOVEA;
	}
#line 3049 "syntactic.cpp"
    break;

  case 87: /* opcode: DIRECTIVE_MOVE  */
#line 1280 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MOVE;
	}
#line 3057 "syntactic.cpp"
    break;

  case 88: /* opcode: DIRECTIVE_NEGX  */
#line 1284 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_NEGX;
	}
#line 3065 "syntactic.cpp"
    break;

  case 89: /* opcode: DIRECTIVE_CLR  */
#line 1288 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CLR;
	}
#line 3073 "syntactic.cpp"
    break;

  case 90: /* opcode: DIRECTIVE_NEG  */
#line 1292 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_NEG;
	}
#line 3081 "syntactic.cpp"
    break;

  case 91: /* opcode: DIRECTIVE_NOT  */
#line 1296 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_NOT;
	}
#line 3089 "syntactic.cpp"
    break;

  case 92: /* opcode: DIRECTIVE_EXT  */
#line 1300 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_EXT;
	}
#line 3097 "syntactic.cpp"
    break;

  case 93: /* opcode: DIRECTIVE_NBCD  */
#line 1304 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_NBCD;
	}
#line 3105 "syntactic.cpp"
    break;

  case 94: /* opcode: DIRECTIVE_SWAP  */
#line 1308 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SWAP;
	}
#line 3113 "syntactic.cpp"
    break;

  case 95: /* opcode: DIRECTIVE_PEA  */
#line 1312 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_PEA;
	}
#line 3121 "syntactic.cpp"
    break;

  case 96: /* opcode: DIRECTIVE_ILLEGAL  */
#line 1316 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ILLEGAL;
	}
#line 3129 "syntactic.cpp"
    break;

  case 97: /* opcode: DIRECTIVE_TAS  */
#line 1320 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_TAS;
	}
#line 3137 "syntactic.cpp"
    break;

  case 98: /* opcode: DIRECTIVE_TST  */
#line 1324 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_TST;
	}
#line 3145 "syntactic.cpp"
    break;

  case 99: /* opcode: DIRECTIVE_TRAP  */
#line 1328 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_TRAP;
	}
#line 3153 "syntactic.cpp"
    break;

  case 100: /* opcode: DIRECTIVE_LINK  */
#line 1332 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_LINK;
	}
#line 3161 "syntactic.cpp"
    break;

  case 101: /* opcode: DIRECTIVE_UNLK  */
#line 1336 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_UNLK;
	}
#line 3169 "syntactic.cpp"
    break;

  case 102: /* opcode: DIRECTIVE_RESET  */
#line 1340 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_RESET;
	}
#line 3177 "syntactic.cpp"
    break;

  case 103: /* opcode: DIRECTIVE_NOP  */
#line 1344 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_NOP;
	}
#line 3185 "syntactic.cpp"
    break;

  case 104: /* opcode: DIRECTIVE_STOP  */
#line 1348 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_STOP;
	}
#line 3193 "syntactic.cpp"
    break;

  case 105: /* opcode: DIRECTIVE_RTE  */
#line 1352 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_RTE;
	}
#line 3201 "syntactic.cpp"
    break;

  case 106: /* opcode: DIRECTIVE_RTS  */
#line 1356 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_RTS;
	}
#line 3209 "syntactic.cpp"
    break;

  case 107: /* opcode: DIRECTIVE_TRAPV  */
#line 1360 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_TRAPV;
	}
#line 3217 "syntactic.cpp"
    break;

  case 108: /* opcode: DIRECTIVE_RTR  */
#line 1364 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_RTR;
	}
#line 3225 "syntactic.cpp"
    break;

  case 109: /* opcode: DIRECTIVE_JSR  */
#line 1368 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_JSR;
	}
#line 3233 "syntactic.cpp"
    break;

  case 110: /* opcode: DIRECTIVE_JMP  */
#line 1372 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_JMP;
	}
#line 3241 "syntactic.cpp"
    break;

  case 111: /* opcode: DIRECTIVE_MOVEM  */
#line 1376 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MOVEM_TO_REGS;
	}
#line 3249 "syntactic.cpp"
    break;

  case 112: /* opcode: DIRECTIVE_LEA  */
#line 1380 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_LEA;
	}
#line 3257 "syntactic.cpp"
    break;

  case 113: /* opcode: DIRECTIVE_CHK  */
#line 1384 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CHK;
	}
#line 3265 "syntactic.cpp"
    break;

  case 114: /* opcode: DIRECTIVE_ADDQ  */
#line 1388 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ADDQ;
	}
#line 3273 "syntactic.cpp"
    break;

  case 115: /* opcode: DIRECTIVE_SUBQ  */
#line 1392 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SUBQ;
	}
#line 3281 "syntactic.cpp"
    break;

  case 116: /* opcode: DIRECTIVE_ST  */
#line 1396 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_TRUE;
	}
#line 3290 "syntactic.cpp"
    break;

  case 117: /* opcode: DIRECTIVE_SF  */
#line 1401 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_FALSE;
	}
#line 3299 "syntactic.cpp"
    break;

  case 118: /* opcode: DIRECTIVE_SHI  */
#line 1406 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_HIGHER;
	}
#line 3308 "syntactic.cpp"
    break;

  case 119: /* opcode: DIRECTIVE_SLS  */
#line 1411 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_LOWER_OR_SAME;
	}
#line 3317 "syntactic.cpp"
    break;

  case 120: /* opcode: DIRECTIVE_SCC  */
#line 1416 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_CARRY_CLEAR;
	}
#line 3326 "syntactic.cpp"
    break;

  case 121: /* opcode: DIRECTIVE_SCS  */
#line 1421 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_CARRY_SET;
	}
#line 3335 "syntactic.cpp"
    break;

  case 122: /* opcode: DIRECTIVE_SNE  */
#line 1426 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_NOT_EQUAL;
	}
#line 3344 "syntactic.cpp"
    break;

  case 123: /* opcode: DIRECTIVE_SEQ  */
#line 1431 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_EQUAL;
	}
#line 3353 "syntactic.cpp"
    break;

  case 124: /* opcode: DIRECTIVE_SVC  */
#line 1436 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 3362 "syntactic.cpp"
    break;

  case 125: /* opcode: DIRECTIVE_SVS  */
#line 1441 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_SET;
	}
#line 3371 "syntactic.cpp"
    break;

  case 126: /* opcode: DIRECTIVE_SPL  */
#line 1446 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_PLUS;
	}
#line 3380 "syntactic.cpp"
    break;

  case 127: /* opcode: DIRECTIVE_SMI  */
#line 1451 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_MINUS;
	}
#line 3389 "syntactic.cpp"
    break;

  case 128: /* opcode: DIRECTIVE_SGE  */
#line 1456 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 3398 "syntactic.cpp"
    break;

  case 129: /* opcode: DIRECTIVE_SLT  */
#line 1461 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_LESS_THAN;
	}
#line 3407 "syntactic.cpp"
    break;

  case 130: /* opcode: DIRECTIVE_SGT  */
#line 1466 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_GREATER_THAN;
	}
#line 3416 "syntactic.cpp"
    break;

  case 131: /* opcode: DIRECTIVE_SLE  */
#line 1471 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Scc;
		(yyval.opcode).condition = CONDITION_LESS_OR_EQUAL;
	}
#line 3425 "syntactic.cpp"
    break;

  case 132: /* opcode: DIRECTIVE_DBT  */
#line 1476 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_TRUE;
	}
#line 3434 "syntactic.cpp"
    break;

  case 133: /* opcode: DIRECTIVE_DBF  */
#line 1481 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_FALSE;
	}
#line 3443 "syntactic.cpp"
    break;

  case 134: /* opcode: DIRECTIVE_DBHI  */
#line 1486 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_HIGHER;
	}
#line 3452 "syntactic.cpp"
    break;

  case 135: /* opcode: DIRECTIVE_DBLS  */
#line 1491 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_LOWER_OR_SAME;
	}
#line 3461 "syntactic.cpp"
    break;

  case 136: /* opcode: DIRECTIVE_DBCC  */
#line 1496 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_CARRY_CLEAR;
	}
#line 3470 "syntactic.cpp"
    break;

  case 137: /* opcode: DIRECTIVE_DBCS  */
#line 1501 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_CARRY_SET;
	}
#line 3479 "syntactic.cpp"
    break;

  case 138: /* opcode: DIRECTIVE_DBNE  */
#line 1506 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_NOT_EQUAL;
	}
#line 3488 "syntactic.cpp"
    break;

  case 139: /* opcode: DIRECTIVE_DBEQ  */
#line 1511 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_EQUAL;
	}
#line 3497 "syntactic.cpp"
    break;

  case 140: /* opcode: DIRECTIVE_DBVC  */
#line 1516 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 3506 "syntactic.cpp"
    break;

  case 141: /* opcode: DIRECTIVE_DBVS  */
#line 1521 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_SET;
	}
#line 3515 "syntactic.cpp"
    break;

  case 142: /* opcode: DIRECTIVE_DBPL  */
#line 1526 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_PLUS;
	}
#line 3524 "syntactic.cpp"
    break;

  case 143: /* opcode: DIRECTIVE_DBMI  */
#line 1531 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_MINUS;
	}
#line 3533 "syntactic.cpp"
    break;

  case 144: /* opcode: DIRECTIVE_DBGE  */
#line 1536 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 3542 "syntactic.cpp"
    break;

  case 145: /* opcode: DIRECTIVE_DBLT  */
#line 1541 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_LESS_THAN;
	}
#line 3551 "syntactic.cpp"
    break;

  case 146: /* opcode: DIRECTIVE_DBGT  */
#line 1546 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_GREATER_THAN;
	}
#line 3560 "syntactic.cpp"
    break;

  case 147: /* opcode: DIRECTIVE_DBLE  */
#line 1551 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DBcc;
		(yyval.opcode).condition = CONDITION_LESS_OR_EQUAL;
	}
#line 3569 "syntactic.cpp"
    break;

  case 148: /* opcode: DIRECTIVE_BRA  */
#line 1556 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BRA;
	}
#line 3577 "syntactic.cpp"
    break;

  case 149: /* opcode: DIRECTIVE_BSR  */
#line 1560 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_BSR;
	}
#line 3585 "syntactic.cpp"
    break;

  case 150: /* opcode: DIRECTIVE_BHI  */
#line 1564 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_HIGHER;
	}
#line 3594 "syntactic.cpp"
    break;

  case 151: /* opcode: DIRECTIVE_BLS  */
#line 1569 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_LOWER_OR_SAME;
	}
#line 3603 "syntactic.cpp"
    break;

  case 152: /* opcode: DIRECTIVE_BCC  */
#line 1574 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_CARRY_CLEAR;
	}
#line 3612 "syntactic.cpp"
    break;

  case 153: /* opcode: DIRECTIVE_BCS  */
#line 1579 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_CARRY_SET;
	}
#line 3621 "syntactic.cpp"
    break;

  case 154: /* opcode: DIRECTIVE_BNE  */
#line 1584 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_NOT_EQUAL;
	}
#line 3630 "syntactic.cpp"
    break;

  case 155: /* opcode: DIRECTIVE_BEQ  */
#line 1589 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_EQUAL;
	}
#line 3639 "syntactic.cpp"
    break;

  case 156: /* opcode: DIRECTIVE_BVC  */
#line 1594 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 3648 "syntactic.cpp"
    break;

  case 157: /* opcode: DIRECTIVE_BVS  */
#line 1599 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_OVERFLOW_SET;
	}
#line 3657 "syntactic.cpp"
    break;

  case 158: /* opcode: DIRECTIVE_BPL  */
#line 1604 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_PLUS;
	}
#line 3666 "syntactic.cpp"
    break;

  case 159: /* opcode: DIRECTIVE_BMI  */
#line 1609 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_MINUS;
	}
#line 3675 "syntactic.cpp"
    break;

  case 160: /* opcode: DIRECTIVE_BGE  */
#line 1614 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 3684 "syntactic.cpp"
    break;

  case 161: /* opcode: DIRECTIVE_BLT  */
#line 1619 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_LESS_THAN;
	}
#line 3693 "syntactic.cpp"
    break;

  case 162: /* opcode: DIRECTIVE_BGT  */
#line 1624 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_GREATER_THAN;
	}
#line 3702 "syntactic.cpp"
    break;

  case 163: /* opcode: DIRECTIVE_BLE  */
#line 1629 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_Bcc;
		(yyval.opcode).condition = CONDITION_LESS_OR_EQUAL;
	}
#line 3711 "syntactic.cpp"
    break;

  case 164: /* opcode: DIRECTIVE_MOVEQ  */
#line 1634 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MOVEQ;
	}
#line 3719 "syntactic.cpp"
    break;

  case 165: /* opcode: DIRECTIVE_DIVU  */
#line 1638 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DIVU;
	}
#line 3727 "syntactic.cpp"
    break;

  case 166: /* opcode: DIRECTIVE_DIVS  */
#line 1642 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_DIVS;
	}
#line 3735 "syntactic.cpp"
    break;

  case 167: /* opcode: DIRECTIVE_SBCD  */
#line 1646 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SBCD_DATA_REGS;
	}
#line 3743 "syntactic.cpp"
    break;

  case 168: /* opcode: DIRECTIVE_OR  */
#line 1650 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_OR_TO_REG;
	}
#line 3751 "syntactic.cpp"
    break;

  case 169: /* opcode: DIRECTIVE_SUB  */
#line 1654 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SUB_TO_REG;
	}
#line 3759 "syntactic.cpp"
    break;

  case 170: /* opcode: DIRECTIVE_SUBX  */
#line 1658 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SUBX_DATA_REGS;
	}
#line 3767 "syntactic.cpp"
    break;

  case 171: /* opcode: DIRECTIVE_SUBA  */
#line 1662 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_SUBA;
	}
#line 3775 "syntactic.cpp"
    break;

  case 172: /* opcode: DIRECTIVE_EOR  */
#line 1666 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_EOR;
	}
#line 3783 "syntactic.cpp"
    break;

  case 173: /* opcode: DIRECTIVE_CMPM  */
#line 1670 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CMPM;
	}
#line 3791 "syntactic.cpp"
    break;

  case 174: /* opcode: DIRECTIVE_CMP  */
#line 1674 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CMP;
	}
#line 3799 "syntactic.cpp"
    break;

  case 175: /* opcode: DIRECTIVE_CMPA  */
#line 1678 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_CMPA;
	}
#line 3807 "syntactic.cpp"
    break;

  case 176: /* opcode: DIRECTIVE_MULU  */
#line 1682 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MULU;
	}
#line 3815 "syntactic.cpp"
    break;

  case 177: /* opcode: DIRECTIVE_MULS  */
#line 1686 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_MULS;
	}
#line 3823 "syntactic.cpp"
    break;

  case 178: /* opcode: DIRECTIVE_ABCD  */
#line 1690 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ABCD_DATA_REGS;
	}
#line 3831 "syntactic.cpp"
    break;

  case 179: /* opcode: DIRECTIVE_EXG  */
#line 1694 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_EXG;
	}
#line 3839 "syntactic.cpp"
    break;

  case 180: /* opcode: DIRECTIVE_AND  */
#line 1698 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_AND_TO_REG;
	}
#line 3847 "syntactic.cpp"
    break;

  case 181: /* opcode: DIRECTIVE_ADD  */
#line 1702 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ADD_TO_REG;
	}
#line 3855 "syntactic.cpp"
    break;

  case 182: /* opcode: DIRECTIVE_ADDX  */
#line 1706 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ADDX_DATA_REGS;
	}
#line 3863 "syntactic.cpp"
    break;

  case 183: /* opcode: DIRECTIVE_ADDA  */
#line 1710 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ADDA;
	}
#line 3871 "syntactic.cpp"
    break;

  case 184: /* opcode: DIRECTIVE_ASL  */
#line 1714 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ASL_STATIC;
	}
#line 3879 "syntactic.cpp"
    break;

  case 185: /* opcode: DIRECTIVE_ASR  */
#line 1718 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ASR_STATIC;
	}
#line 3887 "syntactic.cpp"
    break;

  case 186: /* opcode: DIRECTIVE_LSL  */
#line 1722 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_LSL_STATIC;
	}
#line 3895 "syntactic.cpp"
    break;

  case 187: /* opcode: DIRECTIVE_LSR  */
#line 1726 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_LSR_STATIC;
	}
#line 3903 "syntactic.cpp"
    break;

  case 188: /* opcode: DIRECTIVE_ROXL  */
#line 1730 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ROXL_STATIC;
	}
#line 3911 "syntactic.cpp"
    break;

  case 189: /* opcode: DIRECTIVE_ROXR  */
#line 1734 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ROXR_STATIC;
	}
#line 3919 "syntactic.cpp"
    break;

  case 190: /* opcode: DIRECTIVE_ROL  */
#line 1738 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ROL_STATIC;
	}
#line 3927 "syntactic.cpp"
    break;

  case 191: /* opcode: DIRECTIVE_ROR  */
#line 1742 "syntactic.y"
        {
		(yyval.opcode).type = OPCODE_ROR_STATIC;
	}
#line 3935 "syntactic.cpp"
    break;

  case 192: /* size: SIZE_BYTE  */
#line 1749 "syntactic.y"
        {
		(yyval.size) = SIZE_BYTE;
	}
#line 3943 "syntactic.cpp"
    break;

  case 193: /* size: SIZE_SHORT  */
#line 1753 "syntactic.y"
        {
		(yyval.size) = SIZE_SHORT;
	}
#line 3951 "syntactic.cpp"
    break;

  case 194: /* size: SIZE_WORD  */
#line 1757 "syntactic.y"
        {
		(yyval.size) = SIZE_WORD;
	}
#line 3959 "syntactic.cpp"
    break;

  case 195: /* size: SIZE_LONGWORD  */
#line 1761 "syntactic.y"
        {
		(yyval.size) = SIZE_LONGWORD;
	}
#line 3967 "syntactic.cpp"
    break;

  case 196: /* operand: '(' ADDRESS_REGISTER ')'  */
#line 1769 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT;
		(yyval.operand).main_register = (yyvsp[-1].unsigned_long);
	}
#line 3976 "syntactic.cpp"
    break;

  case 197: /* operand: '(' ADDRESS_REGISTER ')' '+'  */
#line 1774 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
		(yyval.operand).main_register = (yyvsp[-2].unsigned_long);
	}
#line 3985 "syntactic.cpp"
    break;

  case 198: /* operand: '-' '(' ADDRESS_REGISTER ')'  */
#line 1779 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
		(yyval.operand).main_register = (yyvsp[-1].unsigned_long);
	}
#line 3994 "syntactic.cpp"
    break;

  case 199: /* operand: expression '(' ADDRESS_REGISTER ')'  */
#line 1784 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-3].expression);
		(yyval.operand).main_register = (yyvsp[-1].unsigned_long);
	}
#line 4004 "syntactic.cpp"
    break;

  case 200: /* operand: '(' expression ',' ADDRESS_REGISTER ')'  */
#line 1790 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-3].expression);
		(yyval.operand).main_register = (yyvsp[-1].unsigned_long);
	}
#line 4014 "syntactic.cpp"
    break;

  case 201: /* operand: '(' ADDRESS_REGISTER ',' expression ')'  */
#line 1796 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-1].expression);
		(yyval.operand).main_register = (yyvsp[-3].unsigned_long);
	}
#line 4024 "syntactic.cpp"
    break;

  case 202: /* operand: '(' ADDRESS_REGISTER ',' data_or_address_register size ')'  */
#line 1802 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal.type = EXPRESSION_NUMBER;
		(yyval.operand).literal.shared.unsigned_long = 0;
		(yyval.operand).main_register = (yyvsp[-4].unsigned_long);
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 != 0;
	}
#line 4038 "syntactic.cpp"
    break;

  case 203: /* operand: expression '(' ADDRESS_REGISTER ',' data_or_address_register size ')'  */
#line 1812 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-6].expression);
		(yyval.operand).main_register = (yyvsp[-4].unsigned_long);
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 !=0;
	}
#line 4051 "syntactic.cpp"
    break;

  case 204: /* operand: '(' expression ',' ADDRESS_REGISTER ',' data_or_address_register size ')'  */
#line 1821 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-6].expression);
		(yyval.operand).main_register = (yyvsp[-4].unsigned_long);
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 !=0;
	}
#line 4064 "syntactic.cpp"
    break;

  case 205: /* operand: '(' ADDRESS_REGISTER ',' data_or_address_register ')'  */
#line 1830 "syntactic.y"
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
#line 4079 "syntactic.cpp"
    break;

  case 206: /* operand: expression '(' ADDRESS_REGISTER ',' data_or_address_register ')'  */
#line 1841 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-5].expression);
		(yyval.operand).main_register = (yyvsp[-3].unsigned_long);
		(yyval.operand).index_register = (yyvsp[-1].unsigned_long) % 8;
		(yyval.operand).size = SIZE_WORD;
		(yyval.operand).index_register_is_address_register = (yyvsp[-1].unsigned_long) / 8 !=0;
	}
#line 4093 "syntactic.cpp"
    break;

  case 207: /* operand: '(' expression ',' ADDRESS_REGISTER ',' data_or_address_register ')'  */
#line 1851 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yyval.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-5].expression);
		(yyval.operand).main_register = (yyvsp[-3].unsigned_long);
		(yyval.operand).index_register = (yyvsp[-1].unsigned_long) % 8;
		(yyval.operand).size = SIZE_WORD;
		(yyval.operand).index_register_is_address_register = (yyvsp[-1].unsigned_long) / 8 !=0;
	}
#line 4107 "syntactic.cpp"
    break;

  case 208: /* operand: expression '(' PROGRAM_COUNTER ')'  */
#line 1861 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-3].expression);
	}
#line 4116 "syntactic.cpp"
    break;

  case 209: /* operand: '(' expression ',' PROGRAM_COUNTER ')'  */
#line 1866 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-3].expression);
	}
#line 4125 "syntactic.cpp"
    break;

  case 210: /* operand: '(' PROGRAM_COUNTER ',' expression ')'  */
#line 1871 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		(yyval.operand).literal = (yyvsp[-1].expression);
	}
#line 4134 "syntactic.cpp"
    break;

  case 211: /* operand: '(' PROGRAM_COUNTER ',' data_or_address_register size ')'  */
#line 1876 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal.type = EXPRESSION_NUMBER;
		(yyval.operand).literal.shared.unsigned_long = 0;
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 != 0;
	}
#line 4147 "syntactic.cpp"
    break;

  case 212: /* operand: expression '(' PROGRAM_COUNTER ',' data_or_address_register size ')'  */
#line 1885 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-6].expression);
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 != 0;
	}
#line 4159 "syntactic.cpp"
    break;

  case 213: /* operand: '(' expression ',' PROGRAM_COUNTER ',' data_or_address_register size ')'  */
#line 1893 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-6].expression);
		(yyval.operand).index_register = (yyvsp[-2].unsigned_long) % 8;
		(yyval.operand).size = (yyvsp[-1].size);
		(yyval.operand).index_register_is_address_register = (yyvsp[-2].unsigned_long) / 8 != 0;
	}
#line 4171 "syntactic.cpp"
    break;

  case 214: /* operand: '(' PROGRAM_COUNTER ',' data_or_address_register ')'  */
#line 1901 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal.type = EXPRESSION_NUMBER;
		(yyval.operand).literal.shared.unsigned_long = 0;
		(yyval.operand).index_register = (yyvsp[-1].unsigned_long) % 8;
		(yyval.operand).size = SIZE_WORD;
		(yyval.operand).index_register_is_address_register = (yyvsp[-1].unsigned_long) / 8 != 0;
	}
#line 4185 "syntactic.cpp"
    break;

  case 215: /* operand: expression '(' PROGRAM_COUNTER ',' data_or_address_register ')'  */
#line 1911 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-5].expression);
		(yyval.operand).index_register = (yyvsp[-1].unsigned_long) % 8;
		(yyval.operand).size = SIZE_WORD;
		(yyval.operand).index_register_is_address_register = (yyvsp[-1].unsigned_long) / 8 != 0;
	}
#line 4198 "syntactic.cpp"
    break;

  case 216: /* operand: '(' expression ',' PROGRAM_COUNTER ',' data_or_address_register ')'  */
#line 1920 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yyval.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yyval.operand).literal = (yyvsp[-5].expression);
		(yyval.operand).index_register = (yyvsp[-1].unsigned_long) % 8;
		(yyval.operand).size = SIZE_WORD;
		(yyval.operand).index_register_is_address_register = (yyvsp[-1].unsigned_long) / 8 != 0;
	}
#line 4211 "syntactic.cpp"
    break;

  case 217: /* operand: '#' expression  */
#line 1930 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_LITERAL;
		(yyval.operand).literal = (yyvsp[0].expression);
	}
#line 4220 "syntactic.cpp"
    break;

  case 218: /* operand: STATUS_REGISTER  */
#line 1946 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_STATUS_REGISTER;
	}
#line 4228 "syntactic.cpp"
    break;

  case 219: /* operand: CONDITION_CODE_REGISTER  */
#line 1950 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_CONDITION_CODE_REGISTER;
	}
#line 4236 "syntactic.cpp"
    break;

  case 220: /* operand: USER_STACK_POINTER_REGISTER  */
#line 1954 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_USER_STACK_POINTER_REGISTER;
	}
#line 4244 "syntactic.cpp"
    break;

  case 221: /* operand: expression  */
#line 1959 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS;
		(yyval.operand).literal = (yyvsp[0].expression);
		(yyval.operand).size = SIZE_UNDEFINED;
	}
#line 4254 "syntactic.cpp"
    break;

  case 222: /* operand: expression size  */
#line 1965 "syntactic.y"
        {
		(yyval.operand).type = OPERAND_ADDRESS_ABSOLUTE;
		(yyval.operand).literal = (yyvsp[-1].expression);
		(yyval.operand).size = (yyvsp[0].size);
	}
#line 4264 "syntactic.cpp"
    break;

  case 223: /* operand: register_list  */
#line 1972 "syntactic.y"
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
#line 4296 "syntactic.cpp"
    break;

  case 224: /* register_list: register_span  */
#line 2003 "syntactic.y"
        {
		(yyval.unsigned_long) = (yyvsp[0].unsigned_long);
	}
#line 4304 "syntactic.cpp"
    break;

  case 225: /* register_list: register_span '/' register_list  */
#line 2007 "syntactic.y"
        {
		(yyval.unsigned_long) = (yyvsp[-2].unsigned_long) | (yyvsp[0].unsigned_long);
	}
#line 4312 "syntactic.cpp"
    break;

  case 226: /* register_span: data_or_address_register  */
#line 2014 "syntactic.y"
        {
		(yyval.unsigned_long) = 1 << (yyvsp[0].unsigned_long);
	}
#line 4320 "syntactic.cpp"
    break;

  case 227: /* register_span: data_or_address_register '-' data_or_address_register  */
#line 2018 "syntactic.y"
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
#line 4346 "syntactic.cpp"
    break;

  case 228: /* data_or_address_register: DATA_REGISTER  */
#line 2043 "syntactic.y"
        {
		(yyval.unsigned_long) = 0 + (yyvsp[0].unsigned_long);
	}
#line 4354 "syntactic.cpp"
    break;

  case 229: /* data_or_address_register: ADDRESS_REGISTER  */
#line 2047 "syntactic.y"
        {
		(yyval.unsigned_long) = 8 + (yyvsp[0].unsigned_long);
	}
#line 4362 "syntactic.cpp"
    break;

  case 230: /* expression: expression1  */
#line 2060 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4370 "syntactic.cpp"
    break;

  case 231: /* expression: expression LOGICAL_AND expression1  */
#line 2065 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LOGICAL_AND, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4379 "syntactic.cpp"
    break;

  case 232: /* expression: expression LOGICAL_OR expression1  */
#line 2071 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LOGICAL_OR, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4388 "syntactic.cpp"
    break;

  case 233: /* expression1: expression2  */
#line 2079 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4396 "syntactic.cpp"
    break;

  case 234: /* expression1: expression1 '=' expression2  */
#line 2083 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_EQUALITY, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4405 "syntactic.cpp"
    break;

  case 235: /* expression1: expression1 EQUALITY expression2  */
#line 2088 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_EQUALITY, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4414 "syntactic.cpp"
    break;

  case 236: /* expression1: expression1 INEQUALITY expression2  */
#line 2093 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_INEQUALITY, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4423 "syntactic.cpp"
    break;

  case 237: /* expression2: expression3  */
#line 2101 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4431 "syntactic.cpp"
    break;

  case 238: /* expression2: expression2 '<' expression3  */
#line 2105 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LESS_THAN, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4440 "syntactic.cpp"
    break;

  case 239: /* expression2: expression2 LESS_OR_EQUAL expression3  */
#line 2110 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LESS_OR_EQUAL, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4449 "syntactic.cpp"
    break;

  case 240: /* expression2: expression2 '>' expression3  */
#line 2115 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_MORE_THAN, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4458 "syntactic.cpp"
    break;

  case 241: /* expression2: expression2 MORE_OR_EQUAL expression3  */
#line 2120 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_MORE_OR_EQUAL, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4467 "syntactic.cpp"
    break;

  case 242: /* expression3: expression4  */
#line 2128 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4475 "syntactic.cpp"
    break;

  case 243: /* expression3: expression3 '+' expression4  */
#line 2132 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_ADD, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4484 "syntactic.cpp"
    break;

  case 244: /* expression3: expression3 '-' expression4  */
#line 2137 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_SUBTRACT, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4493 "syntactic.cpp"
    break;

  case 245: /* expression4: expression5  */
#line 2145 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4501 "syntactic.cpp"
    break;

  case 246: /* expression4: expression4 '*' expression5  */
#line 2149 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_MULTIPLY, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4510 "syntactic.cpp"
    break;

  case 247: /* expression4: expression4 '/' expression5  */
#line 2154 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_DIVIDE, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4519 "syntactic.cpp"
    break;

  case 248: /* expression4: expression4 '%' expression5  */
#line 2159 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_MODULO, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4528 "syntactic.cpp"
    break;

  case 249: /* expression5: expression6  */
#line 2167 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4536 "syntactic.cpp"
    break;

  case 250: /* expression5: expression5 '&' expression6  */
#line 2171 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_BITWISE_AND, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4545 "syntactic.cpp"
    break;

  case 251: /* expression5: expression5 '!' expression6  */
#line 2176 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_BITWISE_OR, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4554 "syntactic.cpp"
    break;

  case 252: /* expression5: expression5 '|' expression6  */
#line 2181 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_BITWISE_OR, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4563 "syntactic.cpp"
    break;

  case 253: /* expression5: expression5 '^' expression6  */
#line 2186 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_BITWISE_XOR, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4572 "syntactic.cpp"
    break;

  case 254: /* expression6: expression7  */
#line 2194 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4580 "syntactic.cpp"
    break;

  case 255: /* expression6: expression6 LEFT_SHIFT expression7  */
#line 2198 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LEFT_SHIFT, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4589 "syntactic.cpp"
    break;

  case 256: /* expression6: expression6 RIGHT_SHIFT expression7  */
#line 2203 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_RIGHT_SHIFT, &(yyvsp[-2].expression), &(yyvsp[0].expression)))
			YYNOMEM;
	}
#line 4598 "syntactic.cpp"
    break;

  case 257: /* expression7: expression8  */
#line 2211 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4606 "syntactic.cpp"
    break;

  case 258: /* expression7: '+' expression7  */
#line 2215 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4614 "syntactic.cpp"
    break;

  case 259: /* expression7: '-' expression7  */
#line 2219 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_NEGATE, &(yyvsp[0].expression), NULL))
			YYNOMEM;
	}
#line 4623 "syntactic.cpp"
    break;

  case 260: /* expression7: '~' expression7  */
#line 2224 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_BITWISE_NOT, &(yyvsp[0].expression), NULL))
			YYNOMEM;
	}
#line 4632 "syntactic.cpp"
    break;

  case 261: /* expression7: '!' expression7  */
#line 2230 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_LOGICAL_NOT, &(yyvsp[0].expression), NULL))
			YYNOMEM;
	}
#line 4641 "syntactic.cpp"
    break;

  case 263: /* local_identifier: SIZE_BYTE  */
#line 2239 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".b");
		if (!String_CreateCopyView(&(yyval.string), &literal))
			YYNOMEM;
	}
#line 4651 "syntactic.cpp"
    break;

  case 264: /* local_identifier: SIZE_SHORT  */
#line 2245 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".s");
		if (!String_CreateCopyView(&(yyval.string), &literal))
			YYNOMEM;
	}
#line 4661 "syntactic.cpp"
    break;

  case 265: /* local_identifier: SIZE_WORD  */
#line 2251 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".w");
		if (!String_CreateCopyView(&(yyval.string), &literal))
			YYNOMEM;
	}
#line 4671 "syntactic.cpp"
    break;

  case 266: /* local_identifier: SIZE_LONGWORD  */
#line 2257 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".l");
		if (!String_CreateCopyView(&(yyval.string), &literal))
			YYNOMEM;
	}
#line 4681 "syntactic.cpp"
    break;

  case 269: /* identifier: identifier local_identifier  */
#line 2268 "syntactic.y"
        {
		const cc_bool success = String_CreateAppend(&(yyval.string), &(yyvsp[-1].string), &(yyvsp[0].string));
		String_Destroy(&(yyvsp[-1].string));
		String_Destroy(&(yyvsp[0].string));

		if (!success)
			YYNOMEM;
	}
#line 4694 "syntactic.cpp"
    break;

  case 270: /* expression8: NUMBER  */
#line 2280 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_NUMBER;
		(yyval.expression).shared.unsigned_long = (yyvsp[0].unsigned_long);
	}
#line 4703 "syntactic.cpp"
    break;

  case 271: /* expression8: identifier  */
#line 2285 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_IDENTIFIER;
		(yyval.expression).shared.string = (yyvsp[0].string);
	}
#line 4712 "syntactic.cpp"
    break;

  case 272: /* expression8: string  */
#line 2290 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[0].expression);
	}
#line 4720 "syntactic.cpp"
    break;

  case 273: /* expression8: '*'  */
#line 2294 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT;
	}
#line 4728 "syntactic.cpp"
    break;

  case 274: /* expression8: '@'  */
#line 2298 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION;
	}
#line 4736 "syntactic.cpp"
    break;

  case 275: /* expression8: '(' expression ')'  */
#line 2302 "syntactic.y"
        {
		(yyval.expression) = (yyvsp[-1].expression);
	}
#line 4744 "syntactic.cpp"
    break;

  case 276: /* expression8: STRLEN '(' STRING ')'  */
#line 2306 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_STRLEN;
		(yyval.expression).shared.string = (yyvsp[-1].string);
	}
#line 4753 "syntactic.cpp"
    break;

  case 277: /* expression8: STRCMP '(' string ',' string ')'  */
#line 2311 "syntactic.y"
        {
		if (!DoExpression(&(yyval.expression), EXPRESSION_STRCMP, &(yyvsp[-3].expression), &(yyvsp[-1].expression)))
			YYNOMEM;
	}
#line 4762 "syntactic.cpp"
    break;

  case 278: /* expression8: INSTR '(' expression ',' expression ')'  */
#line 2316 "syntactic.y"
        {
		Expression expression;

		expression.type = EXPRESSION_NUMBER;
		expression.shared.unsigned_long = 1;

		if (!DoExpressionTriple(&(yyval.expression), EXPRESSION_INSTR, &expression, &(yyvsp[-3].expression), &(yyvsp[-1].expression)))
			YYNOMEM;
	}
#line 4776 "syntactic.cpp"
    break;

  case 279: /* expression8: INSTR '(' expression ',' expression ',' expression ')'  */
#line 2326 "syntactic.y"
        {
		if (!DoExpressionTriple(&(yyval.expression), EXPRESSION_INSTR, &(yyvsp[-5].expression), &(yyvsp[-3].expression), &(yyvsp[-1].expression)))
			YYNOMEM;
	}
#line 4785 "syntactic.cpp"
    break;

  case 280: /* expression8: DEF '(' IDENTIFIER ')'  */
#line 2331 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_DEF;
		(yyval.expression).shared.string = (yyvsp[-1].string);
	}
#line 4794 "syntactic.cpp"
    break;

  case 281: /* expression8: TYPE '(' IDENTIFIER ')'  */
#line 2336 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_TYPE_WITH_IDENTIFIER;
		(yyval.expression).shared.string = (yyvsp[-1].string);
	}
#line 4803 "syntactic.cpp"
    break;

  case 282: /* expression8: TYPE '(' NUMBER ')'  */
#line 2341 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_TYPE_WITH_NUMBER;
		(yyval.expression).shared.unsigned_long = (yyvsp[-1].unsigned_long);
	}
#line 4812 "syntactic.cpp"
    break;

  case 283: /* expression8: FILESIZE '(' STRING ')'  */
#line 2346 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_FILESIZE;
		(yyval.expression).shared.string = (yyvsp[-1].string);
	}
#line 4821 "syntactic.cpp"
    break;

  case 284: /* expression8: OFFSET '(' expression ')'  */
#line 2351 "syntactic.y"
        {
		/* TODO: Make this actually do something besides pass the expression through. */
		(yyval.expression) = (yyvsp[-1].expression);
	}
#line 4830 "syntactic.cpp"
    break;

  case 285: /* string: STRING  */
#line 2359 "syntactic.y"
        {
		(yyval.expression).type = EXPRESSION_STRING;
		(yyval.expression).shared.string = (yyvsp[0].string);
	}
#line 4839 "syntactic.cpp"
    break;


#line 4843 "syntactic.cpp"

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
  yytoken = yychar == TOKEN_M68KASM_EMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
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

      if (yychar <= TOKEN_YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == TOKEN_YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, scanner, statement);
          yychar = TOKEN_M68KASM_EMPTY;
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
  if (yychar != TOKEN_M68KASM_EMPTY)
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

#line 2364 "syntactic.y"


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
