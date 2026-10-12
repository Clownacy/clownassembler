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
#line 38 "syntactic.y"


#include <array>
#include <deque>
#include <memory>
#include <string>
#include <string_view>
#include <variant>

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

struct StatementEndR
{
};

struct StatementMacro
{
	bool uses_label;
	IdentifierList parameter_names;
};

struct StatementMacroS
{
	StatementMacro macro;
};

struct StatementEndM
{
};

struct StatementEqu
{
	Expression expression;
};

struct StatementEquSString
{
	std::string string;
};

struct StatementEquSIdentifier
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

struct StatementElseIf
{
	Expression expression;
};

struct StatementElse
{
};

struct StatementEndC
{
};

struct StatementWhile
{
	Expression expression;
};

struct StatementEndW
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

struct StatementRS
{
	Size size;
	Expression length;
};

struct StatementRSSet
{
	Expression expression;
};

struct StatementRSReset
{
};

struct StatementObj
{
	Expression expression;
};

struct StatementObjEnd
{
};

struct StatementOrg
{
	Expression expression;
};

struct StatementPushO
{
};

struct StatementPopO
{
};

struct StatementOpt
{
	IdentifierList options;
};

struct StatementPushP
{
	std::string string;
};

struct StatementPopP
{
	std::string string;
};

struct StatementShift
{
};

struct StatementMExit
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
	StatementEndR,
	StatementMacro,
	StatementMacroS,
	StatementEndM,
	StatementEqu,
	StatementEquSString,
	StatementEquSIdentifier,
	StatementSubstr,
	StatementSet,
	StatementIf,
	StatementElseIf,
	StatementElse,
	StatementEndC,
	StatementWhile,
	StatementEndW,
	StatementEven,
	StatementCnop,
	StatementInform,
	StatementFail,
	StatementEnd,
	StatementRS,
	StatementRSSet,
	StatementRSReset,
	StatementObj,
	StatementObjEnd,
	StatementOrg,
	StatementPushO,
	StatementPopO,
	StatementOpt,
	StatementPushP,
	StatementPopP,
	StatementShift,
	StatementMExit,
	StatementLocal,
	StatementPurge
>;


#line 639 "syntactic.h"


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
#line 783 "syntactic.h"




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
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class value_type
  {
  public:
    /// Type of *this.
    typedef value_type self_type;

    /// Empty construction.
    value_type () YY_NOEXCEPT
      : yyraw_ ()
    {}

    /// Construct and fill.
    template <typename T>
    value_type (YY_RVREF (T) t)
    {
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

#if 201103L <= YY_CPLUSPLUS
    /// Non copyable.
    value_type (const self_type&) = delete;
    /// Non copyable.
    self_type& operator= (const self_type&) = delete;
#endif

    /// Destruction, allowed only if empty.
    ~value_type () YY_NOEXCEPT
    {}

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
#if YY_CPLUSPLUS < 201103L
    /// Non copyable.
    value_type (const self_type&);
    /// Non copyable.
    self_type& operator= (const self_type&);
#endif

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yyraw_;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yyraw_;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // expression
      // expression1
      // expression2
      // expression3
      // expression4
      // expression5
      // expression6
      // expression7
      // expression8
      // string
      char dummy1[sizeof (Expression)];

      // expression_list
      char dummy2[sizeof (ExpressionList)];

      // identifier_list
      // option_list
      char dummy3[sizeof (IdentifierList)];

      // full_opcode
      // opcode
      char dummy4[sizeof (Opcode)];

      // operand
      char dummy5[sizeof (Operand)];

      // size
      char dummy6[sizeof (Size)];

      // instruction
      char dummy7[sizeof (StatementInstruction)];

      // IDENTIFIER
      // LOCAL_IDENTIFIER
      // STRING
      // OPTION
      // local_identifier
      // identifier
      char dummy8[sizeof (std::string)];

      // DATA_REGISTER
      // ADDRESS_REGISTER
      // NUMBER
      // register_list
      // register_span
      // data_or_address_register
      char dummy9[sizeof (unsigned long)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me_;
      /// A buffer large enough to store any of the semantic values.
      char yyraw_[size];
    };
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
    TOKEN_M68KASM_error = 1,       // error
    TOKEN_M68KASM_UNDEF = 2,       // "invalid token"
    TOKEN_DIRECTIVE_ORI = 3,       // DIRECTIVE_ORI
    TOKEN_DIRECTIVE_ANDI = 4,      // DIRECTIVE_ANDI
    TOKEN_DIRECTIVE_SUBI = 5,      // DIRECTIVE_SUBI
    TOKEN_DIRECTIVE_ADDI = 6,      // DIRECTIVE_ADDI
    TOKEN_DIRECTIVE_EORI = 7,      // DIRECTIVE_EORI
    TOKEN_DIRECTIVE_CMPI = 8,      // DIRECTIVE_CMPI
    TOKEN_DIRECTIVE_BTST = 9,      // DIRECTIVE_BTST
    TOKEN_DIRECTIVE_BCHG = 10,     // DIRECTIVE_BCHG
    TOKEN_DIRECTIVE_BCLR = 11,     // DIRECTIVE_BCLR
    TOKEN_DIRECTIVE_BSET = 12,     // DIRECTIVE_BSET
    TOKEN_DIRECTIVE_MOVEP = 13,    // DIRECTIVE_MOVEP
    TOKEN_DIRECTIVE_MOVEA = 14,    // DIRECTIVE_MOVEA
    TOKEN_DIRECTIVE_MOVE = 15,     // DIRECTIVE_MOVE
    TOKEN_DIRECTIVE_NEGX = 16,     // DIRECTIVE_NEGX
    TOKEN_DIRECTIVE_CLR = 17,      // DIRECTIVE_CLR
    TOKEN_DIRECTIVE_NEG = 18,      // DIRECTIVE_NEG
    TOKEN_DIRECTIVE_NOT = 19,      // DIRECTIVE_NOT
    TOKEN_DIRECTIVE_EXT = 20,      // DIRECTIVE_EXT
    TOKEN_DIRECTIVE_NBCD = 21,     // DIRECTIVE_NBCD
    TOKEN_DIRECTIVE_SWAP = 22,     // DIRECTIVE_SWAP
    TOKEN_DIRECTIVE_PEA = 23,      // DIRECTIVE_PEA
    TOKEN_DIRECTIVE_ILLEGAL = 24,  // DIRECTIVE_ILLEGAL
    TOKEN_DIRECTIVE_TAS = 25,      // DIRECTIVE_TAS
    TOKEN_DIRECTIVE_TST = 26,      // DIRECTIVE_TST
    TOKEN_DIRECTIVE_TRAP = 27,     // DIRECTIVE_TRAP
    TOKEN_DIRECTIVE_LINK = 28,     // DIRECTIVE_LINK
    TOKEN_DIRECTIVE_UNLK = 29,     // DIRECTIVE_UNLK
    TOKEN_DIRECTIVE_RESET = 30,    // DIRECTIVE_RESET
    TOKEN_DIRECTIVE_NOP = 31,      // DIRECTIVE_NOP
    TOKEN_DIRECTIVE_STOP = 32,     // DIRECTIVE_STOP
    TOKEN_DIRECTIVE_RTE = 33,      // DIRECTIVE_RTE
    TOKEN_DIRECTIVE_RTS = 34,      // DIRECTIVE_RTS
    TOKEN_DIRECTIVE_TRAPV = 35,    // DIRECTIVE_TRAPV
    TOKEN_DIRECTIVE_RTR = 36,      // DIRECTIVE_RTR
    TOKEN_DIRECTIVE_JSR = 37,      // DIRECTIVE_JSR
    TOKEN_DIRECTIVE_JMP = 38,      // DIRECTIVE_JMP
    TOKEN_DIRECTIVE_MOVEM = 39,    // DIRECTIVE_MOVEM
    TOKEN_DIRECTIVE_LEA = 40,      // DIRECTIVE_LEA
    TOKEN_DIRECTIVE_CHK = 41,      // DIRECTIVE_CHK
    TOKEN_DIRECTIVE_ADDQ = 42,     // DIRECTIVE_ADDQ
    TOKEN_DIRECTIVE_SUBQ = 43,     // DIRECTIVE_SUBQ
    TOKEN_DIRECTIVE_ST = 44,       // DIRECTIVE_ST
    TOKEN_DIRECTIVE_SF = 45,       // DIRECTIVE_SF
    TOKEN_DIRECTIVE_SHI = 46,      // DIRECTIVE_SHI
    TOKEN_DIRECTIVE_SLS = 47,      // DIRECTIVE_SLS
    TOKEN_DIRECTIVE_SCC = 48,      // DIRECTIVE_SCC
    TOKEN_DIRECTIVE_SCS = 49,      // DIRECTIVE_SCS
    TOKEN_DIRECTIVE_SNE = 50,      // DIRECTIVE_SNE
    TOKEN_DIRECTIVE_SEQ = 51,      // DIRECTIVE_SEQ
    TOKEN_DIRECTIVE_SVC = 52,      // DIRECTIVE_SVC
    TOKEN_DIRECTIVE_SVS = 53,      // DIRECTIVE_SVS
    TOKEN_DIRECTIVE_SPL = 54,      // DIRECTIVE_SPL
    TOKEN_DIRECTIVE_SMI = 55,      // DIRECTIVE_SMI
    TOKEN_DIRECTIVE_SGE = 56,      // DIRECTIVE_SGE
    TOKEN_DIRECTIVE_SLT = 57,      // DIRECTIVE_SLT
    TOKEN_DIRECTIVE_SGT = 58,      // DIRECTIVE_SGT
    TOKEN_DIRECTIVE_SLE = 59,      // DIRECTIVE_SLE
    TOKEN_DIRECTIVE_DBT = 60,      // DIRECTIVE_DBT
    TOKEN_DIRECTIVE_DBF = 61,      // DIRECTIVE_DBF
    TOKEN_DIRECTIVE_DBHI = 62,     // DIRECTIVE_DBHI
    TOKEN_DIRECTIVE_DBLS = 63,     // DIRECTIVE_DBLS
    TOKEN_DIRECTIVE_DBCC = 64,     // DIRECTIVE_DBCC
    TOKEN_DIRECTIVE_DBCS = 65,     // DIRECTIVE_DBCS
    TOKEN_DIRECTIVE_DBNE = 66,     // DIRECTIVE_DBNE
    TOKEN_DIRECTIVE_DBEQ = 67,     // DIRECTIVE_DBEQ
    TOKEN_DIRECTIVE_DBVC = 68,     // DIRECTIVE_DBVC
    TOKEN_DIRECTIVE_DBVS = 69,     // DIRECTIVE_DBVS
    TOKEN_DIRECTIVE_DBPL = 70,     // DIRECTIVE_DBPL
    TOKEN_DIRECTIVE_DBMI = 71,     // DIRECTIVE_DBMI
    TOKEN_DIRECTIVE_DBGE = 72,     // DIRECTIVE_DBGE
    TOKEN_DIRECTIVE_DBLT = 73,     // DIRECTIVE_DBLT
    TOKEN_DIRECTIVE_DBGT = 74,     // DIRECTIVE_DBGT
    TOKEN_DIRECTIVE_DBLE = 75,     // DIRECTIVE_DBLE
    TOKEN_DIRECTIVE_BRA = 76,      // DIRECTIVE_BRA
    TOKEN_DIRECTIVE_BSR = 77,      // DIRECTIVE_BSR
    TOKEN_DIRECTIVE_BHI = 78,      // DIRECTIVE_BHI
    TOKEN_DIRECTIVE_BLS = 79,      // DIRECTIVE_BLS
    TOKEN_DIRECTIVE_BCC = 80,      // DIRECTIVE_BCC
    TOKEN_DIRECTIVE_BCS = 81,      // DIRECTIVE_BCS
    TOKEN_DIRECTIVE_BNE = 82,      // DIRECTIVE_BNE
    TOKEN_DIRECTIVE_BEQ = 83,      // DIRECTIVE_BEQ
    TOKEN_DIRECTIVE_BVC = 84,      // DIRECTIVE_BVC
    TOKEN_DIRECTIVE_BVS = 85,      // DIRECTIVE_BVS
    TOKEN_DIRECTIVE_BPL = 86,      // DIRECTIVE_BPL
    TOKEN_DIRECTIVE_BMI = 87,      // DIRECTIVE_BMI
    TOKEN_DIRECTIVE_BGE = 88,      // DIRECTIVE_BGE
    TOKEN_DIRECTIVE_BLT = 89,      // DIRECTIVE_BLT
    TOKEN_DIRECTIVE_BGT = 90,      // DIRECTIVE_BGT
    TOKEN_DIRECTIVE_BLE = 91,      // DIRECTIVE_BLE
    TOKEN_DIRECTIVE_MOVEQ = 92,    // DIRECTIVE_MOVEQ
    TOKEN_DIRECTIVE_DIVU = 93,     // DIRECTIVE_DIVU
    TOKEN_DIRECTIVE_DIVS = 94,     // DIRECTIVE_DIVS
    TOKEN_DIRECTIVE_SBCD = 95,     // DIRECTIVE_SBCD
    TOKEN_DIRECTIVE_OR = 96,       // DIRECTIVE_OR
    TOKEN_DIRECTIVE_SUB = 97,      // DIRECTIVE_SUB
    TOKEN_DIRECTIVE_SUBX = 98,     // DIRECTIVE_SUBX
    TOKEN_DIRECTIVE_SUBA = 99,     // DIRECTIVE_SUBA
    TOKEN_DIRECTIVE_EOR = 100,     // DIRECTIVE_EOR
    TOKEN_DIRECTIVE_CMPM = 101,    // DIRECTIVE_CMPM
    TOKEN_DIRECTIVE_CMP = 102,     // DIRECTIVE_CMP
    TOKEN_DIRECTIVE_CMPA = 103,    // DIRECTIVE_CMPA
    TOKEN_DIRECTIVE_MULU = 104,    // DIRECTIVE_MULU
    TOKEN_DIRECTIVE_MULS = 105,    // DIRECTIVE_MULS
    TOKEN_DIRECTIVE_ABCD = 106,    // DIRECTIVE_ABCD
    TOKEN_DIRECTIVE_EXG = 107,     // DIRECTIVE_EXG
    TOKEN_DIRECTIVE_AND = 108,     // DIRECTIVE_AND
    TOKEN_DIRECTIVE_ADD = 109,     // DIRECTIVE_ADD
    TOKEN_DIRECTIVE_ADDX = 110,    // DIRECTIVE_ADDX
    TOKEN_DIRECTIVE_ADDA = 111,    // DIRECTIVE_ADDA
    TOKEN_DIRECTIVE_ASL = 112,     // DIRECTIVE_ASL
    TOKEN_DIRECTIVE_ASR = 113,     // DIRECTIVE_ASR
    TOKEN_DIRECTIVE_LSL = 114,     // DIRECTIVE_LSL
    TOKEN_DIRECTIVE_LSR = 115,     // DIRECTIVE_LSR
    TOKEN_DIRECTIVE_ROXL = 116,    // DIRECTIVE_ROXL
    TOKEN_DIRECTIVE_ROXR = 117,    // DIRECTIVE_ROXR
    TOKEN_DIRECTIVE_ROL = 118,     // DIRECTIVE_ROL
    TOKEN_DIRECTIVE_ROR = 119,     // DIRECTIVE_ROR
    TOKEN_DIRECTIVE_DC = 120,      // DIRECTIVE_DC
    TOKEN_DIRECTIVE_DCB = 121,     // DIRECTIVE_DCB
    TOKEN_DIRECTIVE_DS = 122,      // DIRECTIVE_DS
    TOKEN_DIRECTIVE_REPT = 123,    // DIRECTIVE_REPT
    TOKEN_DIRECTIVE_ENDR = 124,    // DIRECTIVE_ENDR
    TOKEN_DIRECTIVE_MACRO = 125,   // DIRECTIVE_MACRO
    TOKEN_DIRECTIVE_MACROS = 126,  // DIRECTIVE_MACROS
    TOKEN_DIRECTIVE_ENDM = 127,    // DIRECTIVE_ENDM
    TOKEN_DIRECTIVE_INCLUDE = 128, // DIRECTIVE_INCLUDE
    TOKEN_DIRECTIVE_INCBIN = 129,  // DIRECTIVE_INCBIN
    TOKEN_DIRECTIVE_EQU = 130,     // DIRECTIVE_EQU
    TOKEN_DIRECTIVE_EQUS = 131,    // DIRECTIVE_EQUS
    TOKEN_DIRECTIVE_SUBSTR = 132,  // DIRECTIVE_SUBSTR
    TOKEN_DIRECTIVE_SET = 133,     // DIRECTIVE_SET
    TOKEN_DIRECTIVE_IF = 134,      // DIRECTIVE_IF
    TOKEN_DIRECTIVE_ELSEIF = 135,  // DIRECTIVE_ELSEIF
    TOKEN_DIRECTIVE_ELSE = 136,    // DIRECTIVE_ELSE
    TOKEN_DIRECTIVE_ENDC = 137,    // DIRECTIVE_ENDC
    TOKEN_DIRECTIVE_WHILE = 138,   // DIRECTIVE_WHILE
    TOKEN_DIRECTIVE_ENDW = 139,    // DIRECTIVE_ENDW
    TOKEN_DIRECTIVE_EVEN = 140,    // DIRECTIVE_EVEN
    TOKEN_DIRECTIVE_CNOP = 141,    // DIRECTIVE_CNOP
    TOKEN_DIRECTIVE_INFORM = 142,  // DIRECTIVE_INFORM
    TOKEN_DIRECTIVE_FAIL = 143,    // DIRECTIVE_FAIL
    TOKEN_DIRECTIVE_END = 144,     // DIRECTIVE_END
    TOKEN_DIRECTIVE_RS = 145,      // DIRECTIVE_RS
    TOKEN_DIRECTIVE_RSSET = 146,   // DIRECTIVE_RSSET
    TOKEN_DIRECTIVE_RSRESET = 147, // DIRECTIVE_RSRESET
    TOKEN_DIRECTIVE_OBJ = 148,     // DIRECTIVE_OBJ
    TOKEN_DIRECTIVE_OBJEND = 149,  // DIRECTIVE_OBJEND
    TOKEN_DIRECTIVE_ORG = 150,     // DIRECTIVE_ORG
    TOKEN_DIRECTIVE_PUSHO = 151,   // DIRECTIVE_PUSHO
    TOKEN_DIRECTIVE_POPO = 152,    // DIRECTIVE_POPO
    TOKEN_DIRECTIVE_OPT = 153,     // DIRECTIVE_OPT
    TOKEN_DIRECTIVE_PUSHP = 154,   // DIRECTIVE_PUSHP
    TOKEN_DIRECTIVE_POPP = 155,    // DIRECTIVE_POPP
    TOKEN_DIRECTIVE_SHIFT = 156,   // DIRECTIVE_SHIFT
    TOKEN_DIRECTIVE_MEXIT = 157,   // DIRECTIVE_MEXIT
    TOKEN_DIRECTIVE_LOCAL = 158,   // DIRECTIVE_LOCAL
    TOKEN_DIRECTIVE_LIST = 159,    // DIRECTIVE_LIST
    TOKEN_DIRECTIVE_NOLIST = 160,  // DIRECTIVE_NOLIST
    TOKEN_DIRECTIVE_PURGE = 161,   // DIRECTIVE_PURGE
    TOKEN_SIZE_BYTE = 162,         // SIZE_BYTE
    TOKEN_SIZE_SHORT = 163,        // SIZE_SHORT
    TOKEN_SIZE_WORD = 164,         // SIZE_WORD
    TOKEN_SIZE_LONGWORD = 165,     // SIZE_LONGWORD
    TOKEN_DATA_REGISTER = 166,     // DATA_REGISTER
    TOKEN_ADDRESS_REGISTER = 167,  // ADDRESS_REGISTER
    TOKEN_NUMBER = 168,            // NUMBER
    TOKEN_IDENTIFIER = 169,        // IDENTIFIER
    TOKEN_LOCAL_IDENTIFIER = 170,  // LOCAL_IDENTIFIER
    TOKEN_STRING = 171,            // STRING
    TOKEN_OPTION = 172,            // OPTION
    TOKEN_STATUS_REGISTER = 173,   // STATUS_REGISTER
    TOKEN_CONDITION_CODE_REGISTER = 174, // CONDITION_CODE_REGISTER
    TOKEN_USER_STACK_POINTER_REGISTER = 175, // USER_STACK_POINTER_REGISTER
    TOKEN_PROGRAM_COUNTER = 176,   // PROGRAM_COUNTER
    TOKEN_LOGICAL_AND = 177,       // LOGICAL_AND
    TOKEN_LOGICAL_OR = 178,        // LOGICAL_OR
    TOKEN_EQUALITY = 179,          // EQUALITY
    TOKEN_INEQUALITY = 180,        // INEQUALITY
    TOKEN_LESS_OR_EQUAL = 181,     // LESS_OR_EQUAL
    TOKEN_MORE_OR_EQUAL = 182,     // MORE_OR_EQUAL
    TOKEN_LEFT_SHIFT = 183,        // LEFT_SHIFT
    TOKEN_RIGHT_SHIFT = 184,       // RIGHT_SHIFT
    TOKEN_STRLEN = 185,            // STRLEN
    TOKEN_STRCMP = 186,            // STRCMP
    TOKEN_INSTR = 187,             // INSTR
    TOKEN_DEF = 188,               // DEF
    TOKEN_TYPE = 189,              // TYPE
    TOKEN_FILESIZE = 190,          // FILESIZE
    TOKEN_OFFSET = 191,            // OFFSET
    TOKEN_PERIOD = 192,            // "."
    TOKEN_COMMA = 193,             // ","
    TOKEN_PARENTHESIS_LEFT = 194,  // "("
    TOKEN_PARENTHESIS_RIGHT = 195, // ")"
    TOKEN_DOLLAR = 196,            // "$"
    TOKEN_PLUS = 197,              // "+"
    TOKEN_MINUS = 198,             // "-"
    TOKEN_ASTERIX = 199,           // "*"
    TOKEN_FORWARD_SLASH = 200,     // "/"
    TOKEN_EQUAL = 201,             // "="
    TOKEN_AT = 202,                // "@"
    TOKEN_HASH = 203,              // "#"
    TOKEN_AMPERSAND = 204,         // "&"
    TOKEN_CARET = 205,             // "^"
    TOKEN_EXCLAMATION = 206,       // "!"
    TOKEN_VERTICAL_BAR = 207,      // "|"
    TOKEN_ANGLE_LEFT = 208,        // "<"
    TOKEN_ANGLE_RIGHT = 209,       // ">"
    TOKEN_PERCENTAGE = 210,        // "%"
    TOKEN_TILDE = 211,             // "~"
    TOKEN_LOWER_PRECEDENCE_THAN_SIZE = 212, // LOWER_PRECEDENCE_THAN_SIZE
    TOKEN_HIGHER_PRECEDENCE_THAN_SIZE = 213 // HIGHER_PRECEDENCE_THAN_SIZE
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
        YYNTOKENS = 214, ///< Number of tokens.
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
        S_PERIOD = 192,                          // "."
        S_COMMA = 193,                           // ","
        S_PARENTHESIS_LEFT = 194,                // "("
        S_PARENTHESIS_RIGHT = 195,               // ")"
        S_DOLLAR = 196,                          // "$"
        S_PLUS = 197,                            // "+"
        S_MINUS = 198,                           // "-"
        S_ASTERIX = 199,                         // "*"
        S_FORWARD_SLASH = 200,                   // "/"
        S_EQUAL = 201,                           // "="
        S_AT = 202,                              // "@"
        S_HASH = 203,                            // "#"
        S_AMPERSAND = 204,                       // "&"
        S_CARET = 205,                           // "^"
        S_EXCLAMATION = 206,                     // "!"
        S_VERTICAL_BAR = 207,                    // "|"
        S_ANGLE_LEFT = 208,                      // "<"
        S_ANGLE_RIGHT = 209,                     // ">"
        S_PERCENTAGE = 210,                      // "%"
        S_TILDE = 211,                           // "~"
        S_LOWER_PRECEDENCE_THAN_SIZE = 212,      // LOWER_PRECEDENCE_THAN_SIZE
        S_HIGHER_PRECEDENCE_THAN_SIZE = 213,     // HIGHER_PRECEDENCE_THAN_SIZE
        S_YYACCEPT = 214,                        // $accept
        S_statement = 215,                       // statement
        S_expression_list = 216,                 // expression_list
        S_identifier_list = 217,                 // identifier_list
        S_option_list = 218,                     // option_list
        S_instruction = 219,                     // instruction
        S_full_opcode = 220,                     // full_opcode
        S_opcode = 221,                          // opcode
        S_size = 222,                            // size
        S_operand = 223,                         // operand
        S_register_list = 224,                   // register_list
        S_register_span = 225,                   // register_span
        S_data_or_address_register = 226,        // data_or_address_register
        S_expression = 227,                      // expression
        S_expression1 = 228,                     // expression1
        S_expression2 = 229,                     // expression2
        S_expression3 = 230,                     // expression3
        S_expression4 = 231,                     // expression4
        S_expression5 = 232,                     // expression5
        S_expression6 = 233,                     // expression6
        S_expression7 = 234,                     // expression7
        S_local_identifier = 235,                // local_identifier
        S_identifier = 236,                      // identifier
        S_expression8 = 237,                     // expression8
        S_string = 238                           // string
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
        , value ()
      {
        switch (this->kind ())
    {
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression1: // expression1
      case symbol_kind::S_expression2: // expression2
      case symbol_kind::S_expression3: // expression3
      case symbol_kind::S_expression4: // expression4
      case symbol_kind::S_expression5: // expression5
      case symbol_kind::S_expression6: // expression6
      case symbol_kind::S_expression7: // expression7
      case symbol_kind::S_expression8: // expression8
      case symbol_kind::S_string: // string
        value.move< Expression > (std::move (that.value));
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.move< ExpressionList > (std::move (that.value));
        break;

      case symbol_kind::S_identifier_list: // identifier_list
      case symbol_kind::S_option_list: // option_list
        value.move< IdentifierList > (std::move (that.value));
        break;

      case symbol_kind::S_full_opcode: // full_opcode
      case symbol_kind::S_opcode: // opcode
        value.move< Opcode > (std::move (that.value));
        break;

      case symbol_kind::S_operand: // operand
        value.move< Operand > (std::move (that.value));
        break;

      case symbol_kind::S_size: // size
        value.move< Size > (std::move (that.value));
        break;

      case symbol_kind::S_instruction: // instruction
        value.move< StatementInstruction > (std::move (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_LOCAL_IDENTIFIER: // LOCAL_IDENTIFIER
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPTION: // OPTION
      case symbol_kind::S_local_identifier: // local_identifier
      case symbol_kind::S_identifier: // identifier
        value.move< std::string > (std::move (that.value));
        break;

      case symbol_kind::S_DATA_REGISTER: // DATA_REGISTER
      case symbol_kind::S_ADDRESS_REGISTER: // ADDRESS_REGISTER
      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_register_list: // register_list
      case symbol_kind::S_register_span: // register_span
      case symbol_kind::S_data_or_address_register: // data_or_address_register
        value.move< unsigned long > (std::move (that.value));
        break;

      default:
        break;
    }

      }
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructors for typed symbols.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t)
        : Base (t)
      {}
#else
      basic_symbol (typename Base::kind_type t)
        : Base (t)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Expression&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Expression& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ExpressionList&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ExpressionList& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, IdentifierList&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const IdentifierList& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Opcode&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Opcode& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Operand&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Operand& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Size&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Size& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, StatementInstruction&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const StatementInstruction& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v)
        : Base (t)
        , value (v)
      {}
#endif

#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, unsigned long&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const unsigned long& v)
        : Base (t)
        , value (v)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }



      /// Destroy contents, and record that is empty.
      void clear () YY_NOEXCEPT
      {
        // User destructor.
        symbol_kind_type yykind = this->kind ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yykind)
        {
       default:
          break;
        }

        // Value type destructor.
switch (yykind)
    {
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression1: // expression1
      case symbol_kind::S_expression2: // expression2
      case symbol_kind::S_expression3: // expression3
      case symbol_kind::S_expression4: // expression4
      case symbol_kind::S_expression5: // expression5
      case symbol_kind::S_expression6: // expression6
      case symbol_kind::S_expression7: // expression7
      case symbol_kind::S_expression8: // expression8
      case symbol_kind::S_string: // string
        value.template destroy< Expression > ();
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.template destroy< ExpressionList > ();
        break;

      case symbol_kind::S_identifier_list: // identifier_list
      case symbol_kind::S_option_list: // option_list
        value.template destroy< IdentifierList > ();
        break;

      case symbol_kind::S_full_opcode: // full_opcode
      case symbol_kind::S_opcode: // opcode
        value.template destroy< Opcode > ();
        break;

      case symbol_kind::S_operand: // operand
        value.template destroy< Operand > ();
        break;

      case symbol_kind::S_size: // size
        value.template destroy< Size > ();
        break;

      case symbol_kind::S_instruction: // instruction
        value.template destroy< StatementInstruction > ();
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_LOCAL_IDENTIFIER: // LOCAL_IDENTIFIER
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPTION: // OPTION
      case symbol_kind::S_local_identifier: // local_identifier
      case symbol_kind::S_identifier: // identifier
        value.template destroy< std::string > ();
        break;

      case symbol_kind::S_DATA_REGISTER: // DATA_REGISTER
      case symbol_kind::S_ADDRESS_REGISTER: // ADDRESS_REGISTER
      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_register_list: // register_list
      case symbol_kind::S_register_span: // register_span
      case symbol_kind::S_data_or_address_register: // data_or_address_register
        value.template destroy< unsigned long > ();
        break;

      default:
        break;
    }

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
    {
      /// Superclass.
      typedef basic_symbol<by_kind> super_type;

      /// Empty symbol.
      symbol_type () YY_NOEXCEPT {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok)
        : super_type (token_kind_type (tok))
#else
      symbol_type (int tok)
        : super_type (token_kind_type (tok))
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v)
        : super_type (token_kind_type (tok), std::move (v))
#else
      symbol_type (int tok, const std::string& v)
        : super_type (token_kind_type (tok), v)
#endif
      {}
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, unsigned long v)
        : super_type (token_kind_type (tok), std::move (v))
#else
      symbol_type (int tok, const unsigned long& v)
        : super_type (token_kind_type (tok), v)
#endif
      {}
    };

    /// Build a parser object.
    parser (void *scanner_yyarg, Statement &statement_yyarg);
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

    // Implementation of make_symbol for each token kind.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_YYEOF ()
      {
        return symbol_type (token::TOKEN_YYEOF);
      }
#else
      static
      symbol_type
      make_YYEOF ()
      {
        return symbol_type (token::TOKEN_YYEOF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_M68KASM_error ()
      {
        return symbol_type (token::TOKEN_M68KASM_error);
      }
#else
      static
      symbol_type
      make_M68KASM_error ()
      {
        return symbol_type (token::TOKEN_M68KASM_error);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_M68KASM_UNDEF ()
      {
        return symbol_type (token::TOKEN_M68KASM_UNDEF);
      }
#else
      static
      symbol_type
      make_M68KASM_UNDEF ()
      {
        return symbol_type (token::TOKEN_M68KASM_UNDEF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ORI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ORI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ORI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ORI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ANDI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ANDI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ANDI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ANDI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SUBI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUBI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SUBI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUBI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ADDI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ADDI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ADDI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ADDI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_EORI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EORI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_EORI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EORI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_CMPI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CMPI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_CMPI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CMPI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BTST ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BTST);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BTST ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BTST);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BCHG ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BCHG);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BCHG ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BCHG);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BCLR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BCLR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BCLR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BCLR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BSET ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BSET);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BSET ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BSET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_MOVEP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MOVEP);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_MOVEP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MOVEP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_MOVEA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MOVEA);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_MOVEA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MOVEA);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_MOVE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MOVE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_MOVE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MOVE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_NEGX ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NEGX);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_NEGX ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NEGX);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_CLR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CLR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_CLR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CLR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_NEG ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NEG);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_NEG ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NEG);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_NOT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NOT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_NOT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NOT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_EXT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EXT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_EXT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EXT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_NBCD ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NBCD);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_NBCD ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NBCD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SWAP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SWAP);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SWAP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SWAP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_PEA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_PEA);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_PEA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_PEA);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ILLEGAL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ILLEGAL);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ILLEGAL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ILLEGAL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_TAS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_TAS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_TAS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_TAS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_TST ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_TST);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_TST ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_TST);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_TRAP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_TRAP);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_TRAP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_TRAP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_LINK ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LINK);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_LINK ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LINK);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_UNLK ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_UNLK);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_UNLK ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_UNLK);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_RESET ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RESET);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_RESET ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RESET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_NOP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NOP);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_NOP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NOP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_STOP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_STOP);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_STOP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_STOP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_RTE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RTE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_RTE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RTE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_RTS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RTS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_RTS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RTS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_TRAPV ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_TRAPV);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_TRAPV ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_TRAPV);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_RTR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RTR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_RTR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RTR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_JSR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_JSR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_JSR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_JSR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_JMP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_JMP);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_JMP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_JMP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_MOVEM ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MOVEM);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_MOVEM ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MOVEM);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_LEA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LEA);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_LEA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LEA);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_CHK ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CHK);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_CHK ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CHK);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ADDQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ADDQ);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ADDQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ADDQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SUBQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUBQ);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SUBQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUBQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ST ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ST);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ST ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ST);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SF ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SF);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SF ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SHI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SHI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SHI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SHI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SLS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SLS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SLS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SLS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SCC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SCC);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SCC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SCC);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SCS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SCS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SCS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SCS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SNE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SNE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SNE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SNE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SEQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SEQ);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SEQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SEQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SVC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SVC);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SVC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SVC);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SVS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SVS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SVS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SVS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SPL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SPL);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SPL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SPL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SMI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SMI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SMI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SMI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SGE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SGE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SGE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SGE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SLT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SLT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SLT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SLT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SGT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SGT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SGT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SGT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SLE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SLE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SLE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SLE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBF ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBF);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBF ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBHI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBHI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBHI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBHI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBLS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBLS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBLS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBLS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBCC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBCC);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBCC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBCC);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBCS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBCS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBCS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBCS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBNE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBNE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBNE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBNE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBEQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBEQ);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBEQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBEQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBVC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBVC);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBVC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBVC);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBVS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBVS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBVS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBVS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBPL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBPL);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBPL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBPL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBMI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBMI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBMI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBMI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBGE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBGE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBGE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBGE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBLT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBLT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBLT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBLT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBGT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBGT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBGT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBGT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DBLE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBLE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DBLE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DBLE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BRA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BRA);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BRA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BRA);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BSR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BSR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BSR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BSR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BHI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BHI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BHI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BHI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BLS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BLS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BLS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BLS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BCC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BCC);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BCC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BCC);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BCS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BCS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BCS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BCS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BNE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BNE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BNE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BNE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BEQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BEQ);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BEQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BEQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BVC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BVC);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BVC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BVC);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BVS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BVS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BVS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BVS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BPL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BPL);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BPL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BPL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BMI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BMI);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BMI ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BMI);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BGE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BGE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BGE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BGE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BLT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BLT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BLT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BLT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BGT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BGT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BGT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BGT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_BLE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BLE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_BLE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_BLE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_MOVEQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MOVEQ);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_MOVEQ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MOVEQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DIVU ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DIVU);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DIVU ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DIVU);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DIVS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DIVS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DIVS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DIVS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SBCD ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SBCD);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SBCD ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SBCD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_OR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_OR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_OR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_OR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SUB ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUB);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SUB ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUB);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SUBX ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUBX);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SUBX ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUBX);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SUBA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUBA);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SUBA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUBA);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_EOR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EOR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_EOR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EOR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_CMPM ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CMPM);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_CMPM ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CMPM);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_CMP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CMP);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_CMP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CMP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_CMPA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CMPA);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_CMPA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CMPA);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_MULU ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MULU);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_MULU ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MULU);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_MULS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MULS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_MULS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MULS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ABCD ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ABCD);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ABCD ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ABCD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_EXG ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EXG);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_EXG ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EXG);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_AND ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_AND);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_AND ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_AND);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ADD ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ADD);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ADD ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ADD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ADDX ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ADDX);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ADDX ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ADDX);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ADDA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ADDA);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ADDA ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ADDA);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ASL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ASL);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ASL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ASL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ASR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ASR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ASR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ASR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_LSL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LSL);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_LSL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LSL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_LSR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LSR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_LSR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LSR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ROXL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ROXL);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ROXL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ROXL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ROXR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ROXR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ROXR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ROXR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ROL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ROL);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ROL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ROL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ROR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ROR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ROR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ROR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DC);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DC);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DCB ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DCB);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DCB ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DCB);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_DS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_DS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_DS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_REPT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_REPT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_REPT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_REPT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ENDR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ENDR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ENDR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ENDR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_MACRO ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MACRO);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_MACRO ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MACRO);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_MACROS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MACROS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_MACROS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MACROS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ENDM ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ENDM);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ENDM ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ENDM);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_INCLUDE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_INCLUDE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_INCLUDE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_INCLUDE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_INCBIN ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_INCBIN);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_INCBIN ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_INCBIN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_EQU ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EQU);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_EQU ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EQU);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_EQUS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EQUS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_EQUS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EQUS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SUBSTR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUBSTR);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SUBSTR ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SUBSTR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SET ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SET);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SET ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_IF ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_IF);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_IF ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_IF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ELSEIF ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ELSEIF);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ELSEIF ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ELSEIF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ELSE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ELSE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ELSE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ELSE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ENDC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ENDC);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ENDC ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ENDC);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_WHILE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_WHILE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_WHILE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_WHILE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ENDW ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ENDW);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ENDW ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ENDW);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_EVEN ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EVEN);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_EVEN ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_EVEN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_CNOP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CNOP);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_CNOP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_CNOP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_INFORM ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_INFORM);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_INFORM ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_INFORM);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_FAIL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_FAIL);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_FAIL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_FAIL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_END ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_END);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_END ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_END);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_RS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RS);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_RS ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_RSSET ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RSSET);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_RSSET ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RSSET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_RSRESET ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RSRESET);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_RSRESET ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_RSRESET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_OBJ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_OBJ);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_OBJ ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_OBJ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_OBJEND ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_OBJEND);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_OBJEND ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_OBJEND);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_ORG ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ORG);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_ORG ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_ORG);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_PUSHO ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_PUSHO);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_PUSHO ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_PUSHO);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_POPO ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_POPO);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_POPO ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_POPO);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_OPT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_OPT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_OPT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_OPT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_PUSHP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_PUSHP);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_PUSHP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_PUSHP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_POPP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_POPP);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_POPP ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_POPP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_SHIFT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SHIFT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_SHIFT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_SHIFT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_MEXIT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MEXIT);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_MEXIT ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_MEXIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_LOCAL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LOCAL);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_LOCAL ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LOCAL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_LIST ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LIST);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_LIST ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_LIST);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_NOLIST ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NOLIST);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_NOLIST ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_NOLIST);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIRECTIVE_PURGE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_PURGE);
      }
#else
      static
      symbol_type
      make_DIRECTIVE_PURGE ()
      {
        return symbol_type (token::TOKEN_DIRECTIVE_PURGE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SIZE_BYTE ()
      {
        return symbol_type (token::TOKEN_SIZE_BYTE);
      }
#else
      static
      symbol_type
      make_SIZE_BYTE ()
      {
        return symbol_type (token::TOKEN_SIZE_BYTE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SIZE_SHORT ()
      {
        return symbol_type (token::TOKEN_SIZE_SHORT);
      }
#else
      static
      symbol_type
      make_SIZE_SHORT ()
      {
        return symbol_type (token::TOKEN_SIZE_SHORT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SIZE_WORD ()
      {
        return symbol_type (token::TOKEN_SIZE_WORD);
      }
#else
      static
      symbol_type
      make_SIZE_WORD ()
      {
        return symbol_type (token::TOKEN_SIZE_WORD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SIZE_LONGWORD ()
      {
        return symbol_type (token::TOKEN_SIZE_LONGWORD);
      }
#else
      static
      symbol_type
      make_SIZE_LONGWORD ()
      {
        return symbol_type (token::TOKEN_SIZE_LONGWORD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DATA_REGISTER (unsigned long v)
      {
        return symbol_type (token::TOKEN_DATA_REGISTER, std::move (v));
      }
#else
      static
      symbol_type
      make_DATA_REGISTER (const unsigned long& v)
      {
        return symbol_type (token::TOKEN_DATA_REGISTER, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ADDRESS_REGISTER (unsigned long v)
      {
        return symbol_type (token::TOKEN_ADDRESS_REGISTER, std::move (v));
      }
#else
      static
      symbol_type
      make_ADDRESS_REGISTER (const unsigned long& v)
      {
        return symbol_type (token::TOKEN_ADDRESS_REGISTER, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NUMBER (unsigned long v)
      {
        return symbol_type (token::TOKEN_NUMBER, std::move (v));
      }
#else
      static
      symbol_type
      make_NUMBER (const unsigned long& v)
      {
        return symbol_type (token::TOKEN_NUMBER, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IDENTIFIER (std::string v)
      {
        return symbol_type (token::TOKEN_IDENTIFIER, std::move (v));
      }
#else
      static
      symbol_type
      make_IDENTIFIER (const std::string& v)
      {
        return symbol_type (token::TOKEN_IDENTIFIER, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOCAL_IDENTIFIER (std::string v)
      {
        return symbol_type (token::TOKEN_LOCAL_IDENTIFIER, std::move (v));
      }
#else
      static
      symbol_type
      make_LOCAL_IDENTIFIER (const std::string& v)
      {
        return symbol_type (token::TOKEN_LOCAL_IDENTIFIER, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRING (std::string v)
      {
        return symbol_type (token::TOKEN_STRING, std::move (v));
      }
#else
      static
      symbol_type
      make_STRING (const std::string& v)
      {
        return symbol_type (token::TOKEN_STRING, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OPTION (std::string v)
      {
        return symbol_type (token::TOKEN_OPTION, std::move (v));
      }
#else
      static
      symbol_type
      make_OPTION (const std::string& v)
      {
        return symbol_type (token::TOKEN_OPTION, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STATUS_REGISTER ()
      {
        return symbol_type (token::TOKEN_STATUS_REGISTER);
      }
#else
      static
      symbol_type
      make_STATUS_REGISTER ()
      {
        return symbol_type (token::TOKEN_STATUS_REGISTER);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CONDITION_CODE_REGISTER ()
      {
        return symbol_type (token::TOKEN_CONDITION_CODE_REGISTER);
      }
#else
      static
      symbol_type
      make_CONDITION_CODE_REGISTER ()
      {
        return symbol_type (token::TOKEN_CONDITION_CODE_REGISTER);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_USER_STACK_POINTER_REGISTER ()
      {
        return symbol_type (token::TOKEN_USER_STACK_POINTER_REGISTER);
      }
#else
      static
      symbol_type
      make_USER_STACK_POINTER_REGISTER ()
      {
        return symbol_type (token::TOKEN_USER_STACK_POINTER_REGISTER);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PROGRAM_COUNTER ()
      {
        return symbol_type (token::TOKEN_PROGRAM_COUNTER);
      }
#else
      static
      symbol_type
      make_PROGRAM_COUNTER ()
      {
        return symbol_type (token::TOKEN_PROGRAM_COUNTER);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOGICAL_AND ()
      {
        return symbol_type (token::TOKEN_LOGICAL_AND);
      }
#else
      static
      symbol_type
      make_LOGICAL_AND ()
      {
        return symbol_type (token::TOKEN_LOGICAL_AND);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOGICAL_OR ()
      {
        return symbol_type (token::TOKEN_LOGICAL_OR);
      }
#else
      static
      symbol_type
      make_LOGICAL_OR ()
      {
        return symbol_type (token::TOKEN_LOGICAL_OR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQUALITY ()
      {
        return symbol_type (token::TOKEN_EQUALITY);
      }
#else
      static
      symbol_type
      make_EQUALITY ()
      {
        return symbol_type (token::TOKEN_EQUALITY);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INEQUALITY ()
      {
        return symbol_type (token::TOKEN_INEQUALITY);
      }
#else
      static
      symbol_type
      make_INEQUALITY ()
      {
        return symbol_type (token::TOKEN_INEQUALITY);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LESS_OR_EQUAL ()
      {
        return symbol_type (token::TOKEN_LESS_OR_EQUAL);
      }
#else
      static
      symbol_type
      make_LESS_OR_EQUAL ()
      {
        return symbol_type (token::TOKEN_LESS_OR_EQUAL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MORE_OR_EQUAL ()
      {
        return symbol_type (token::TOKEN_MORE_OR_EQUAL);
      }
#else
      static
      symbol_type
      make_MORE_OR_EQUAL ()
      {
        return symbol_type (token::TOKEN_MORE_OR_EQUAL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LEFT_SHIFT ()
      {
        return symbol_type (token::TOKEN_LEFT_SHIFT);
      }
#else
      static
      symbol_type
      make_LEFT_SHIFT ()
      {
        return symbol_type (token::TOKEN_LEFT_SHIFT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RIGHT_SHIFT ()
      {
        return symbol_type (token::TOKEN_RIGHT_SHIFT);
      }
#else
      static
      symbol_type
      make_RIGHT_SHIFT ()
      {
        return symbol_type (token::TOKEN_RIGHT_SHIFT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRLEN ()
      {
        return symbol_type (token::TOKEN_STRLEN);
      }
#else
      static
      symbol_type
      make_STRLEN ()
      {
        return symbol_type (token::TOKEN_STRLEN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRCMP ()
      {
        return symbol_type (token::TOKEN_STRCMP);
      }
#else
      static
      symbol_type
      make_STRCMP ()
      {
        return symbol_type (token::TOKEN_STRCMP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INSTR ()
      {
        return symbol_type (token::TOKEN_INSTR);
      }
#else
      static
      symbol_type
      make_INSTR ()
      {
        return symbol_type (token::TOKEN_INSTR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DEF ()
      {
        return symbol_type (token::TOKEN_DEF);
      }
#else
      static
      symbol_type
      make_DEF ()
      {
        return symbol_type (token::TOKEN_DEF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TYPE ()
      {
        return symbol_type (token::TOKEN_TYPE);
      }
#else
      static
      symbol_type
      make_TYPE ()
      {
        return symbol_type (token::TOKEN_TYPE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FILESIZE ()
      {
        return symbol_type (token::TOKEN_FILESIZE);
      }
#else
      static
      symbol_type
      make_FILESIZE ()
      {
        return symbol_type (token::TOKEN_FILESIZE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_OFFSET ()
      {
        return symbol_type (token::TOKEN_OFFSET);
      }
#else
      static
      symbol_type
      make_OFFSET ()
      {
        return symbol_type (token::TOKEN_OFFSET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PERIOD ()
      {
        return symbol_type (token::TOKEN_PERIOD);
      }
#else
      static
      symbol_type
      make_PERIOD ()
      {
        return symbol_type (token::TOKEN_PERIOD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA ()
      {
        return symbol_type (token::TOKEN_COMMA);
      }
#else
      static
      symbol_type
      make_COMMA ()
      {
        return symbol_type (token::TOKEN_COMMA);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PARENTHESIS_LEFT ()
      {
        return symbol_type (token::TOKEN_PARENTHESIS_LEFT);
      }
#else
      static
      symbol_type
      make_PARENTHESIS_LEFT ()
      {
        return symbol_type (token::TOKEN_PARENTHESIS_LEFT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PARENTHESIS_RIGHT ()
      {
        return symbol_type (token::TOKEN_PARENTHESIS_RIGHT);
      }
#else
      static
      symbol_type
      make_PARENTHESIS_RIGHT ()
      {
        return symbol_type (token::TOKEN_PARENTHESIS_RIGHT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DOLLAR ()
      {
        return symbol_type (token::TOKEN_DOLLAR);
      }
#else
      static
      symbol_type
      make_DOLLAR ()
      {
        return symbol_type (token::TOKEN_DOLLAR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS ()
      {
        return symbol_type (token::TOKEN_PLUS);
      }
#else
      static
      symbol_type
      make_PLUS ()
      {
        return symbol_type (token::TOKEN_PLUS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS ()
      {
        return symbol_type (token::TOKEN_MINUS);
      }
#else
      static
      symbol_type
      make_MINUS ()
      {
        return symbol_type (token::TOKEN_MINUS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ASTERIX ()
      {
        return symbol_type (token::TOKEN_ASTERIX);
      }
#else
      static
      symbol_type
      make_ASTERIX ()
      {
        return symbol_type (token::TOKEN_ASTERIX);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FORWARD_SLASH ()
      {
        return symbol_type (token::TOKEN_FORWARD_SLASH);
      }
#else
      static
      symbol_type
      make_FORWARD_SLASH ()
      {
        return symbol_type (token::TOKEN_FORWARD_SLASH);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQUAL ()
      {
        return symbol_type (token::TOKEN_EQUAL);
      }
#else
      static
      symbol_type
      make_EQUAL ()
      {
        return symbol_type (token::TOKEN_EQUAL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AT ()
      {
        return symbol_type (token::TOKEN_AT);
      }
#else
      static
      symbol_type
      make_AT ()
      {
        return symbol_type (token::TOKEN_AT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_HASH ()
      {
        return symbol_type (token::TOKEN_HASH);
      }
#else
      static
      symbol_type
      make_HASH ()
      {
        return symbol_type (token::TOKEN_HASH);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_AMPERSAND ()
      {
        return symbol_type (token::TOKEN_AMPERSAND);
      }
#else
      static
      symbol_type
      make_AMPERSAND ()
      {
        return symbol_type (token::TOKEN_AMPERSAND);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_CARET ()
      {
        return symbol_type (token::TOKEN_CARET);
      }
#else
      static
      symbol_type
      make_CARET ()
      {
        return symbol_type (token::TOKEN_CARET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EXCLAMATION ()
      {
        return symbol_type (token::TOKEN_EXCLAMATION);
      }
#else
      static
      symbol_type
      make_EXCLAMATION ()
      {
        return symbol_type (token::TOKEN_EXCLAMATION);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_VERTICAL_BAR ()
      {
        return symbol_type (token::TOKEN_VERTICAL_BAR);
      }
#else
      static
      symbol_type
      make_VERTICAL_BAR ()
      {
        return symbol_type (token::TOKEN_VERTICAL_BAR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ANGLE_LEFT ()
      {
        return symbol_type (token::TOKEN_ANGLE_LEFT);
      }
#else
      static
      symbol_type
      make_ANGLE_LEFT ()
      {
        return symbol_type (token::TOKEN_ANGLE_LEFT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ANGLE_RIGHT ()
      {
        return symbol_type (token::TOKEN_ANGLE_RIGHT);
      }
#else
      static
      symbol_type
      make_ANGLE_RIGHT ()
      {
        return symbol_type (token::TOKEN_ANGLE_RIGHT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PERCENTAGE ()
      {
        return symbol_type (token::TOKEN_PERCENTAGE);
      }
#else
      static
      symbol_type
      make_PERCENTAGE ()
      {
        return symbol_type (token::TOKEN_PERCENTAGE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TILDE ()
      {
        return symbol_type (token::TOKEN_TILDE);
      }
#else
      static
      symbol_type
      make_TILDE ()
      {
        return symbol_type (token::TOKEN_TILDE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOWER_PRECEDENCE_THAN_SIZE ()
      {
        return symbol_type (token::TOKEN_LOWER_PRECEDENCE_THAN_SIZE);
      }
#else
      static
      symbol_type
      make_LOWER_PRECEDENCE_THAN_SIZE ()
      {
        return symbol_type (token::TOKEN_LOWER_PRECEDENCE_THAN_SIZE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_HIGHER_PRECEDENCE_THAN_SIZE ()
      {
        return symbol_type (token::TOKEN_HIGHER_PRECEDENCE_THAN_SIZE);
      }
#else
      static
      symbol_type
      make_HIGHER_PRECEDENCE_THAN_SIZE ()
      {
        return symbol_type (token::TOKEN_HIGHER_PRECEDENCE_THAN_SIZE);
      }
#endif


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
      yylast_ = 819,     ///< Last index in yytable_.
      yynnts_ = 25,  ///< Number of nonterminal symbols.
      yyfinal_ = 237 ///< Termination state number.
    };


    // User arguments.
    void *scanner;
    Statement &statement;

  };

  inline
  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    return static_cast<symbol_kind_type> (t);
  }

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
  {
    switch (this->kind ())
    {
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression1: // expression1
      case symbol_kind::S_expression2: // expression2
      case symbol_kind::S_expression3: // expression3
      case symbol_kind::S_expression4: // expression4
      case symbol_kind::S_expression5: // expression5
      case symbol_kind::S_expression6: // expression6
      case symbol_kind::S_expression7: // expression7
      case symbol_kind::S_expression8: // expression8
      case symbol_kind::S_string: // string
        value.copy< Expression > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.copy< ExpressionList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier_list: // identifier_list
      case symbol_kind::S_option_list: // option_list
        value.copy< IdentifierList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_full_opcode: // full_opcode
      case symbol_kind::S_opcode: // opcode
        value.copy< Opcode > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_operand: // operand
        value.copy< Operand > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_size: // size
        value.copy< Size > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_instruction: // instruction
        value.copy< StatementInstruction > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_LOCAL_IDENTIFIER: // LOCAL_IDENTIFIER
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPTION: // OPTION
      case symbol_kind::S_local_identifier: // local_identifier
      case symbol_kind::S_identifier: // identifier
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DATA_REGISTER: // DATA_REGISTER
      case symbol_kind::S_ADDRESS_REGISTER: // ADDRESS_REGISTER
      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_register_list: // register_list
      case symbol_kind::S_register_span: // register_span
      case symbol_kind::S_data_or_address_register: // data_or_address_register
        value.copy< unsigned long > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_expression: // expression
      case symbol_kind::S_expression1: // expression1
      case symbol_kind::S_expression2: // expression2
      case symbol_kind::S_expression3: // expression3
      case symbol_kind::S_expression4: // expression4
      case symbol_kind::S_expression5: // expression5
      case symbol_kind::S_expression6: // expression6
      case symbol_kind::S_expression7: // expression7
      case symbol_kind::S_expression8: // expression8
      case symbol_kind::S_string: // string
        value.move< Expression > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.move< ExpressionList > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_identifier_list: // identifier_list
      case symbol_kind::S_option_list: // option_list
        value.move< IdentifierList > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_full_opcode: // full_opcode
      case symbol_kind::S_opcode: // opcode
        value.move< Opcode > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_operand: // operand
        value.move< Operand > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_size: // size
        value.move< Size > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_instruction: // instruction
        value.move< StatementInstruction > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_LOCAL_IDENTIFIER: // LOCAL_IDENTIFIER
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPTION: // OPTION
      case symbol_kind::S_local_identifier: // local_identifier
      case symbol_kind::S_identifier: // identifier
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_DATA_REGISTER: // DATA_REGISTER
      case symbol_kind::S_ADDRESS_REGISTER: // ADDRESS_REGISTER
      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_register_list: // register_list
      case symbol_kind::S_register_span: // register_span
      case symbol_kind::S_data_or_address_register: // data_or_address_register
        value.move< unsigned long > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

  }

  // by_kind.
  inline
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  inline
  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  inline
  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  inline
  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  inline
  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  inline
  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  inline
  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


#line 25 "syntactic.y"
} // m68kasm
#line 5770 "syntactic.h"


// "%code provides" blocks.
#line 628 "syntactic.y"


#define YY_DECL m68kasm::parser::symbol_type m68kasm_lex(void *yyscanner)


#line 5780 "syntactic.h"


#endif // !YY_M68KASM_SYNTACTIC_H_INCLUDED
