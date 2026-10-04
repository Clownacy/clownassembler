// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

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

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.


// Take the name prefix into account.
#define yylex   m68kasm_lex



#include "syntactic.h"


// Unqualified %code blocks.
#line 619 "syntactic.y"


#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

YY_DECL;
void m68kasm_warning(void *scanner, Statement *statement, const char *message);
void m68kasm_warning_pedantic(void *scanner, Statement *statement, const char *message);
void m68kasm_error(void *scanner, Statement *statement, const char *message);

static cc_bool DoExpressionTriple(Expression *expression, ExpressionType type, Expression *left_expression, Expression *middle_expression, Expression *right_expression);
static cc_bool DoExpression(Expression *expression, ExpressionType type, Expression *left_expression, Expression *right_expression);
static void DestroyExpressionList(ExpressionList *list);
static void DestroyOperand(Operand *operand);
static void DestroyStatementInstruction(StatementInstruction *instruction);

void m68kasm::parser::error(const std::string &message)
{
	m68kasm_error(scanner, statement, message.c_str());
}


#line 73 "syntactic.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if M68KASM_DEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !M68KASM_DEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !M68KASM_DEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 25 "syntactic.y"
namespace m68kasm {
#line 147 "syntactic.cpp"

  /// Build a parser object.
  parser::parser (void *scanner_yyarg, Statement *statement_yyarg)
#if M68KASM_DEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      statement (statement_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
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
        value.YY_MOVE_OR_COPY< Expression > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.YY_MOVE_OR_COPY< ExpressionList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier_list: // identifier_list
      case symbol_kind::S_option_list: // option_list
        value.YY_MOVE_OR_COPY< IdentifierList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_full_opcode: // full_opcode
      case symbol_kind::S_opcode: // opcode
        value.YY_MOVE_OR_COPY< Opcode > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_operand: // operand
        value.YY_MOVE_OR_COPY< Operand > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_size: // size
        value.YY_MOVE_OR_COPY< Size > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_instruction: // instruction
        value.YY_MOVE_OR_COPY< StatementInstruction > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_LOCAL_IDENTIFIER: // LOCAL_IDENTIFIER
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPTION: // OPTION
      case symbol_kind::S_local_identifier: // local_identifier
      case symbol_kind::S_identifier: // identifier
        value.YY_MOVE_OR_COPY< String > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DATA_REGISTER: // DATA_REGISTER
      case symbol_kind::S_ADDRESS_REGISTER: // ADDRESS_REGISTER
      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_register_list: // register_list
      case symbol_kind::S_register_span: // register_span
      case symbol_kind::S_data_or_address_register: // data_or_address_register
        value.YY_MOVE_OR_COPY< unsigned long > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
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
        value.move< Expression > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.move< ExpressionList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_identifier_list: // identifier_list
      case symbol_kind::S_option_list: // option_list
        value.move< IdentifierList > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_full_opcode: // full_opcode
      case symbol_kind::S_opcode: // opcode
        value.move< Opcode > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_operand: // operand
        value.move< Operand > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_size: // size
        value.move< Size > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_instruction: // instruction
        value.move< StatementInstruction > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_LOCAL_IDENTIFIER: // LOCAL_IDENTIFIER
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPTION: // OPTION
      case symbol_kind::S_local_identifier: // local_identifier
      case symbol_kind::S_identifier: // identifier
        value.move< String > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_DATA_REGISTER: // DATA_REGISTER
      case symbol_kind::S_ADDRESS_REGISTER: // ADDRESS_REGISTER
      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_register_list: // register_list
      case symbol_kind::S_register_span: // register_span
      case symbol_kind::S_data_or_address_register: // data_or_address_register
        value.move< unsigned long > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
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
        value.copy< Expression > (that.value);
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.copy< ExpressionList > (that.value);
        break;

      case symbol_kind::S_identifier_list: // identifier_list
      case symbol_kind::S_option_list: // option_list
        value.copy< IdentifierList > (that.value);
        break;

      case symbol_kind::S_full_opcode: // full_opcode
      case symbol_kind::S_opcode: // opcode
        value.copy< Opcode > (that.value);
        break;

      case symbol_kind::S_operand: // operand
        value.copy< Operand > (that.value);
        break;

      case symbol_kind::S_size: // size
        value.copy< Size > (that.value);
        break;

      case symbol_kind::S_instruction: // instruction
        value.copy< StatementInstruction > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_LOCAL_IDENTIFIER: // LOCAL_IDENTIFIER
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPTION: // OPTION
      case symbol_kind::S_local_identifier: // local_identifier
      case symbol_kind::S_identifier: // identifier
        value.copy< String > (that.value);
        break;

      case symbol_kind::S_DATA_REGISTER: // DATA_REGISTER
      case symbol_kind::S_ADDRESS_REGISTER: // ADDRESS_REGISTER
      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_register_list: // register_list
      case symbol_kind::S_register_span: // register_span
      case symbol_kind::S_data_or_address_register: // data_or_address_register
        value.copy< unsigned long > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
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
        value.move< Expression > (that.value);
        break;

      case symbol_kind::S_expression_list: // expression_list
        value.move< ExpressionList > (that.value);
        break;

      case symbol_kind::S_identifier_list: // identifier_list
      case symbol_kind::S_option_list: // option_list
        value.move< IdentifierList > (that.value);
        break;

      case symbol_kind::S_full_opcode: // full_opcode
      case symbol_kind::S_opcode: // opcode
        value.move< Opcode > (that.value);
        break;

      case symbol_kind::S_operand: // operand
        value.move< Operand > (that.value);
        break;

      case symbol_kind::S_size: // size
        value.move< Size > (that.value);
        break;

      case symbol_kind::S_instruction: // instruction
        value.move< StatementInstruction > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_LOCAL_IDENTIFIER: // LOCAL_IDENTIFIER
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPTION: // OPTION
      case symbol_kind::S_local_identifier: // local_identifier
      case symbol_kind::S_identifier: // identifier
        value.move< String > (that.value);
        break;

      case symbol_kind::S_DATA_REGISTER: // DATA_REGISTER
      case symbol_kind::S_ADDRESS_REGISTER: // ADDRESS_REGISTER
      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_register_list: // register_list
      case symbol_kind::S_register_span: // register_span
      case symbol_kind::S_data_or_address_register: // data_or_address_register
        value.move< unsigned long > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if M68KASM_DEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if M68KASM_DEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // M68KASM_DEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (scanner));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
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
        yylhs.value.emplace< Expression > ();
        break;

      case symbol_kind::S_expression_list: // expression_list
        yylhs.value.emplace< ExpressionList > ();
        break;

      case symbol_kind::S_identifier_list: // identifier_list
      case symbol_kind::S_option_list: // option_list
        yylhs.value.emplace< IdentifierList > ();
        break;

      case symbol_kind::S_full_opcode: // full_opcode
      case symbol_kind::S_opcode: // opcode
        yylhs.value.emplace< Opcode > ();
        break;

      case symbol_kind::S_operand: // operand
        yylhs.value.emplace< Operand > ();
        break;

      case symbol_kind::S_size: // size
        yylhs.value.emplace< Size > ();
        break;

      case symbol_kind::S_instruction: // instruction
        yylhs.value.emplace< StatementInstruction > ();
        break;

      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
      case symbol_kind::S_LOCAL_IDENTIFIER: // LOCAL_IDENTIFIER
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_OPTION: // OPTION
      case symbol_kind::S_local_identifier: // local_identifier
      case symbol_kind::S_identifier: // identifier
        yylhs.value.emplace< String > ();
        break;

      case symbol_kind::S_DATA_REGISTER: // DATA_REGISTER
      case symbol_kind::S_ADDRESS_REGISTER: // ADDRESS_REGISTER
      case symbol_kind::S_NUMBER: // NUMBER
      case symbol_kind::S_register_list: // register_list
      case symbol_kind::S_register_span: // register_span
      case symbol_kind::S_data_or_address_register: // data_or_address_register
        yylhs.value.emplace< unsigned long > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // statement: %empty
#line 880 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 817 "syntactic.cpp"
    break;

  case 3: // statement: instruction
#line 884 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INSTRUCTION;
		statement->shared.instruction = yystack_[0].value.as < StatementInstruction > ();
	}
#line 826 "syntactic.cpp"
    break;

  case 4: // statement: DIRECTIVE_DC size expression_list
#line 889 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DC;
		statement->shared.dc.size = yystack_[1].value.as < Size > ();
		statement->shared.dc.values = yystack_[0].value.as < ExpressionList > ();
	}
#line 836 "syntactic.cpp"
    break;

  case 5: // statement: DIRECTIVE_DCB size expression "," expression
#line 895 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DCB;
		statement->shared.dcb.size = yystack_[3].value.as < Size > ();
		statement->shared.dcb.repetitions = yystack_[2].value.as < Expression > ();
		statement->shared.dcb.value = yystack_[0].value.as < Expression > ();
	}
#line 847 "syntactic.cpp"
    break;

  case 6: // statement: DIRECTIVE_DS size expression
#line 902 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DS;
		statement->shared.ds.size = yystack_[1].value.as < Size > ();
		statement->shared.ds.length = yystack_[0].value.as < Expression > ();
	}
#line 857 "syntactic.cpp"
    break;

  case 7: // statement: DIRECTIVE_INCLUDE STRING
#line 908 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCLUDE;
		statement->shared.include.path = yystack_[0].value.as < String > ();
	}
#line 866 "syntactic.cpp"
    break;

  case 8: // statement: DIRECTIVE_INCBIN STRING
#line 913 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = yystack_[0].value.as < String > ();
		statement->shared.incbin.start.type = EXPRESSION_NUMBER;
		statement->shared.incbin.start.shared.unsigned_long = 0;
		statement->shared.incbin.has_length = cc_false;
	}
#line 878 "syntactic.cpp"
    break;

  case 9: // statement: DIRECTIVE_INCBIN STRING "," expression
#line 921 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = yystack_[2].value.as < String > ();
		statement->shared.incbin.start = yystack_[0].value.as < Expression > ();
		statement->shared.incbin.has_length = cc_false;
	}
#line 889 "syntactic.cpp"
    break;

  case 10: // statement: DIRECTIVE_INCBIN STRING "," "," expression
#line 928 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = yystack_[3].value.as < String > ();
		statement->shared.incbin.start.type = EXPRESSION_NUMBER;
		statement->shared.incbin.start.shared.unsigned_long = 0;
		statement->shared.incbin.has_length = cc_true;
		statement->shared.incbin.length = yystack_[0].value.as < Expression > ();
	}
#line 902 "syntactic.cpp"
    break;

  case 11: // statement: DIRECTIVE_INCBIN STRING "," expression "," expression
#line 937 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = yystack_[4].value.as < String > ();
		statement->shared.incbin.start = yystack_[2].value.as < Expression > ();
		statement->shared.incbin.has_length = cc_true;
		statement->shared.incbin.length = yystack_[0].value.as < Expression > ();
	}
#line 914 "syntactic.cpp"
    break;

  case 12: // statement: DIRECTIVE_REPT expression
#line 945 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_REPT;
		statement->shared.rept.repetitions = yystack_[0].value.as < Expression > ();
	}
#line 923 "syntactic.cpp"
    break;

  case 13: // statement: DIRECTIVE_ENDR
#line 950 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDR;
	}
#line 931 "syntactic.cpp"
    break;

  case 14: // statement: DIRECTIVE_MACRO
#line 954 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 942 "syntactic.cpp"
    break;

  case 15: // statement: DIRECTIVE_MACRO "*"
#line 961 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_true;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 953 "syntactic.cpp"
    break;

  case 16: // statement: DIRECTIVE_MACRO identifier_list
#line 968 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names = yystack_[0].value.as < IdentifierList > ();
	}
#line 963 "syntactic.cpp"
    break;

  case 17: // statement: DIRECTIVE_MACRO "*" "," identifier_list
#line 974 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_true;
		statement->shared.macro.parameter_names = yystack_[0].value.as < IdentifierList > ();
	}
#line 973 "syntactic.cpp"
    break;

  case 18: // statement: DIRECTIVE_MACROS
#line 980 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 984 "syntactic.cpp"
    break;

  case 19: // statement: DIRECTIVE_MACROS identifier_list
#line 987 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names = yystack_[0].value.as < IdentifierList > ();
	}
#line 994 "syntactic.cpp"
    break;

  case 20: // statement: DIRECTIVE_ENDM
#line 993 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDM;
	}
#line 1002 "syntactic.cpp"
    break;

  case 21: // statement: DIRECTIVE_EQU expression
#line 997 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQU;
		statement->shared.equ.expression = yystack_[0].value.as < Expression > ();
	}
#line 1011 "syntactic.cpp"
    break;

  case 22: // statement: DIRECTIVE_EQUS STRING
#line 1002 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQUS_STRING;
		statement->shared.equs_string.string = yystack_[0].value.as < String > ();
	}
#line 1020 "syntactic.cpp"
    break;

  case 23: // statement: DIRECTIVE_EQUS IDENTIFIER
#line 1007 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQUS_IDENTIFIER;
		statement->shared.equs_identifier.string = yystack_[0].value.as < String > ();
	}
#line 1029 "syntactic.cpp"
    break;

  case 24: // statement: DIRECTIVE_SUBSTR "," "," STRING
#line 1012 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start.type = EXPRESSION_NUMBER;
		statement->shared.substr.start.shared.unsigned_long = 1;
		statement->shared.substr.end.type = EXPRESSION_NUMBER;
		statement->shared.substr.end.shared.unsigned_long = String_Length(&yystack_[0].value.as < String > ());
		statement->shared.substr.string = yystack_[0].value.as < String > ();
	}
#line 1042 "syntactic.cpp"
    break;

  case 25: // statement: DIRECTIVE_SUBSTR expression "," "," STRING
#line 1021 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start = yystack_[3].value.as < Expression > ();
		statement->shared.substr.end.type = EXPRESSION_NUMBER;
		statement->shared.substr.end.shared.unsigned_long = String_Length(&yystack_[0].value.as < String > ());
		statement->shared.substr.string = yystack_[0].value.as < String > ();
	}
#line 1054 "syntactic.cpp"
    break;

  case 26: // statement: DIRECTIVE_SUBSTR "," expression "," STRING
#line 1029 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start.type = EXPRESSION_NUMBER;
		statement->shared.substr.start.shared.unsigned_long = 1;
		statement->shared.substr.end = yystack_[2].value.as < Expression > ();
		statement->shared.substr.string = yystack_[0].value.as < String > ();
	}
#line 1066 "syntactic.cpp"
    break;

  case 27: // statement: DIRECTIVE_SUBSTR expression "," expression "," STRING
#line 1037 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start = yystack_[4].value.as < Expression > ();
		statement->shared.substr.end = yystack_[2].value.as < Expression > ();
		statement->shared.substr.string = yystack_[0].value.as < String > ();
	}
#line 1077 "syntactic.cpp"
    break;

  case 28: // statement: DIRECTIVE_SET expression
#line 1044 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.set.expression = yystack_[0].value.as < Expression > ();
	}
#line 1086 "syntactic.cpp"
    break;

  case 29: // statement: "=" expression
#line 1049 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.set.expression = yystack_[0].value.as < Expression > ();
	}
#line 1095 "syntactic.cpp"
    break;

  case 30: // statement: DIRECTIVE_IF expression
#line 1054 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_IF;
		statement->shared.if_statement.expression = yystack_[0].value.as < Expression > ();
	}
#line 1104 "syntactic.cpp"
    break;

  case 31: // statement: DIRECTIVE_ELSEIF expression
#line 1059 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSEIF;
		statement->shared.elseif.expression = yystack_[0].value.as < Expression > ();
	}
#line 1113 "syntactic.cpp"
    break;

  case 32: // statement: DIRECTIVE_ELSEIF
#line 1064 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSE;
	}
#line 1121 "syntactic.cpp"
    break;

  case 33: // statement: DIRECTIVE_ELSE
#line 1068 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSE;
	}
#line 1129 "syntactic.cpp"
    break;

  case 34: // statement: DIRECTIVE_ENDC
#line 1072 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDC;
	}
#line 1137 "syntactic.cpp"
    break;

  case 35: // statement: DIRECTIVE_WHILE expression
#line 1076 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_WHILE;
		statement->shared.while_statement.expression = yystack_[0].value.as < Expression > ();
	}
#line 1146 "syntactic.cpp"
    break;

  case 36: // statement: DIRECTIVE_ENDW
#line 1081 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDW;
	}
#line 1154 "syntactic.cpp"
    break;

  case 37: // statement: DIRECTIVE_EVEN
#line 1085 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EVEN;
	}
#line 1162 "syntactic.cpp"
    break;

  case 38: // statement: DIRECTIVE_CNOP expression "," expression
#line 1089 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_CNOP;
		statement->shared.cnop.offset = yystack_[2].value.as < Expression > ();
		statement->shared.cnop.size_boundary = yystack_[0].value.as < Expression > ();
	}
#line 1172 "syntactic.cpp"
    break;

  case 39: // statement: DIRECTIVE_INFORM expression "," STRING
#line 1095 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.severity = yystack_[2].value.as < Expression > ();
		statement->shared.inform.message = yystack_[0].value.as < String > ();
	}
#line 1182 "syntactic.cpp"
    break;

  case 40: // statement: DIRECTIVE_INFORM expression "," STRING "," expression_list
#line 1101 "syntactic.y"
        {
		(void)yystack_[0].value.as < ExpressionList > ();

		/* TODO - parameters */
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.severity = yystack_[4].value.as < Expression > ();
		statement->shared.inform.message = yystack_[2].value.as < String > ();
		DestroyExpressionList(&yystack_[0].value.as < ExpressionList > ());
	}
#line 1196 "syntactic.cpp"
    break;

  case 41: // statement: DIRECTIVE_FAIL
#line 1111 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_FAIL;
	}
#line 1204 "syntactic.cpp"
    break;

  case 42: // statement: DIRECTIVE_END
#line 1115 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_END;
	}
#line 1212 "syntactic.cpp"
    break;

  case 43: // statement: DIRECTIVE_RS size expression
#line 1119 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RS;
		statement->shared.rs.size = yystack_[1].value.as < Size > ();
		statement->shared.rs.length = yystack_[0].value.as < Expression > ();
	}
#line 1222 "syntactic.cpp"
    break;

  case 44: // statement: DIRECTIVE_RSSET expression
#line 1125 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RSSET;
		statement->shared.rsset.expression = yystack_[0].value.as < Expression > ();
	}
#line 1231 "syntactic.cpp"
    break;

  case 45: // statement: DIRECTIVE_RSRESET expression
#line 1130 "syntactic.y"
        {
		/* As mentioned in S.N. 68k's manual, this odd hack
		   is necessary for compatibility with other assemblers. */
		statement->type = STATEMENT_TYPE_RSSET;
		statement->shared.rsset.expression = yystack_[0].value.as < Expression > ();
	}
#line 1242 "syntactic.cpp"
    break;

  case 46: // statement: DIRECTIVE_RSRESET
#line 1137 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RSRESET;
	}
#line 1250 "syntactic.cpp"
    break;

  case 47: // statement: DIRECTIVE_OBJ expression
#line 1141 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OBJ;
		statement->shared.obj.expression = yystack_[0].value.as < Expression > ();
	}
#line 1259 "syntactic.cpp"
    break;

  case 48: // statement: DIRECTIVE_OBJEND
#line 1146 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OBJEND;
	}
#line 1267 "syntactic.cpp"
    break;

  case 49: // statement: DIRECTIVE_ORG expression
#line 1150 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ORG;
		statement->shared.org.expression = yystack_[0].value.as < Expression > ();
	}
#line 1276 "syntactic.cpp"
    break;

  case 50: // statement: DIRECTIVE_PUSHO
#line 1155 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PUSHO;
	}
#line 1284 "syntactic.cpp"
    break;

  case 51: // statement: DIRECTIVE_POPO
#line 1159 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_POPO;
	}
#line 1292 "syntactic.cpp"
    break;

  case 52: // statement: DIRECTIVE_OPT option_list
#line 1163 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OPT;
		statement->shared.opt.options = yystack_[0].value.as < IdentifierList > ();
	}
#line 1301 "syntactic.cpp"
    break;

  case 53: // statement: DIRECTIVE_PUSHP STRING
#line 1168 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PUSHP;
		statement->shared.pushp.string = yystack_[0].value.as < String > ();
	}
#line 1310 "syntactic.cpp"
    break;

  case 54: // statement: DIRECTIVE_POPP IDENTIFIER
#line 1173 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_POPP;
		statement->shared.popp.string = yystack_[0].value.as < String > ();
	}
#line 1319 "syntactic.cpp"
    break;

  case 55: // statement: DIRECTIVE_SHIFT
#line 1178 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SHIFT;
	}
#line 1327 "syntactic.cpp"
    break;

  case 56: // statement: DIRECTIVE_MEXIT
#line 1182 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MEXIT;
	}
#line 1335 "syntactic.cpp"
    break;

  case 57: // statement: DIRECTIVE_LOCAL identifier_list
#line 1186 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_LOCAL;
		statement->shared.local.identifiers = yystack_[0].value.as < IdentifierList > ();
	}
#line 1344 "syntactic.cpp"
    break;

  case 58: // statement: DIRECTIVE_LIST
#line 1191 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 1353 "syntactic.cpp"
    break;

  case 59: // statement: DIRECTIVE_LIST "+"
#line 1196 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 1362 "syntactic.cpp"
    break;

  case 60: // statement: DIRECTIVE_LIST "-"
#line 1201 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 1371 "syntactic.cpp"
    break;

  case 61: // statement: DIRECTIVE_NOLIST
#line 1206 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 1380 "syntactic.cpp"
    break;

  case 62: // statement: DIRECTIVE_PURGE IDENTIFIER
#line 1211 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PURGE;
		statement->shared.purge.string = yystack_[0].value.as < String > ();
	}
#line 1389 "syntactic.cpp"
    break;

  case 63: // expression_list: expression
#line 1219 "syntactic.y"
        {
		ExpressionListNode *node = (ExpressionListNode*)malloc(sizeof(ExpressionListNode));

		if (node == NULL)
		{
			DestroyExpression(&yystack_[0].value.as < Expression > ());
			YYNOMEM;
		}
		else
		{
			node->expression = yystack_[0].value.as < Expression > ();
			node->next = NULL;
		}

		yylhs.value.as < ExpressionList > ().head = yylhs.value.as < ExpressionList > ().tail = node;
	}
#line 1410 "syntactic.cpp"
    break;

  case 64: // expression_list: expression_list "," expression
#line 1236 "syntactic.y"
        {
		ExpressionListNode *node = (ExpressionListNode*)malloc(sizeof(ExpressionListNode));

		yylhs.value.as < ExpressionList > () = yystack_[2].value.as < ExpressionList > ();

		if (node == NULL)
		{
			DestroyExpressionList(&yystack_[2].value.as < ExpressionList > ());
			DestroyExpression(&yystack_[0].value.as < Expression > ());
			YYNOMEM;
		}
		else
		{
			node->expression = yystack_[0].value.as < Expression > ();
			node->next = NULL;

			if (yylhs.value.as < ExpressionList > ().head == NULL)
				yylhs.value.as < ExpressionList > ().head = node;
			else
				((ExpressionListNode*)yylhs.value.as < ExpressionList > ().tail)->next = node;

			yylhs.value.as < ExpressionList > ().tail = node;
		}
	}
#line 1439 "syntactic.cpp"
    break;

  case 65: // identifier_list: IDENTIFIER
#line 1264 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		if (node == NULL)
		{
			String_Destroy(&yystack_[0].value.as < String > ());
			YYNOMEM;
		}
		else
		{
			node->identifier = yystack_[0].value.as < String > ();
			node->next = NULL;
		}

		yylhs.value.as < IdentifierList > ().head = yylhs.value.as < IdentifierList > ().tail = node;
	}
#line 1460 "syntactic.cpp"
    break;

  case 66: // identifier_list: identifier_list "," IDENTIFIER
#line 1281 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		yylhs.value.as < IdentifierList > () = yystack_[2].value.as < IdentifierList > ();

		if (node == NULL)
		{
			DestroyIdentifierList(&yystack_[2].value.as < IdentifierList > ());
			String_Destroy(&yystack_[0].value.as < String > ());
			YYNOMEM;
		}
		else
		{
			node->identifier = yystack_[0].value.as < String > ();
			node->next = NULL;

			if (yylhs.value.as < IdentifierList > ().head == NULL)
				yylhs.value.as < IdentifierList > ().head = node;
			else
				((IdentifierListNode*)yylhs.value.as < IdentifierList > ().tail)->next = node;

			yylhs.value.as < IdentifierList > ().tail = node;
		}
	}
#line 1489 "syntactic.cpp"
    break;

  case 67: // option_list: OPTION
#line 1309 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		if (node == NULL)
		{
			String_Destroy(&yystack_[0].value.as < String > ());
			YYNOMEM;
		}
		else
		{
			node->identifier = yystack_[0].value.as < String > ();
			node->next = NULL;
		}

		yylhs.value.as < IdentifierList > ().head = yylhs.value.as < IdentifierList > ().tail = node;
	}
#line 1510 "syntactic.cpp"
    break;

  case 68: // option_list: option_list "," OPTION
#line 1326 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		yylhs.value.as < IdentifierList > () = yystack_[2].value.as < IdentifierList > ();

		if (node == NULL)
		{
			DestroyIdentifierList(&yystack_[2].value.as < IdentifierList > ());
			String_Destroy(&yystack_[0].value.as < String > ());
			YYNOMEM;
		}
		else
		{
			node->identifier = yystack_[0].value.as < String > ();
			node->next = NULL;

			if (yylhs.value.as < IdentifierList > ().head == NULL)
				yylhs.value.as < IdentifierList > ().head = node;
			else
				((IdentifierListNode*)yylhs.value.as < IdentifierList > ().tail)->next = node;

			yylhs.value.as < IdentifierList > ().tail = node;
		}
	}
#line 1539 "syntactic.cpp"
    break;

  case 69: // instruction: full_opcode
#line 1354 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = yystack_[0].value.as < Opcode > ();
		yylhs.value.as < StatementInstruction > ().operands[0].type = OPERAND_NONE;
		yylhs.value.as < StatementInstruction > ().operands[1].type = OPERAND_NONE;
	}
#line 1549 "syntactic.cpp"
    break;

  case 70: // instruction: full_opcode operand
#line 1360 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = yystack_[1].value.as < Opcode > ();
		yylhs.value.as < StatementInstruction > ().operands[0] = yystack_[0].value.as < Operand > ();
		yylhs.value.as < StatementInstruction > ().operands[1].type = OPERAND_NONE;
	}
#line 1559 "syntactic.cpp"
    break;

  case 71: // instruction: full_opcode operand "," operand
#line 1366 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = yystack_[3].value.as < Opcode > ();
		yylhs.value.as < StatementInstruction > ().operands[0] = yystack_[2].value.as < Operand > ();
		yylhs.value.as < StatementInstruction > ().operands[1] = yystack_[0].value.as < Operand > ();
	}
#line 1569 "syntactic.cpp"
    break;

  case 72: // full_opcode: opcode
#line 1375 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = yystack_[0].value.as < Opcode > ();
		yylhs.value.as < Opcode > ().size = SIZE_UNDEFINED;
	}
#line 1578 "syntactic.cpp"
    break;

  case 73: // full_opcode: opcode "."
#line 1380 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = yystack_[1].value.as < Opcode > ();
		yylhs.value.as < Opcode > ().size = SIZE_UNDEFINED;
		m68kasm_warning_pedantic(scanner, statement, "Opcode has a dot but no size; either remove the dot or add an explicit size.");
	}
#line 1588 "syntactic.cpp"
    break;

  case 74: // full_opcode: opcode size
#line 1386 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = yystack_[1].value.as < Opcode > ();
		yylhs.value.as < Opcode > ().size = yystack_[0].value.as < Size > ();
	}
#line 1597 "syntactic.cpp"
    break;

  case 75: // opcode: DIRECTIVE_ORI
#line 1394 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ORI;
	}
#line 1605 "syntactic.cpp"
    break;

  case 76: // opcode: DIRECTIVE_ANDI
#line 1398 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ANDI;
	}
#line 1613 "syntactic.cpp"
    break;

  case 77: // opcode: DIRECTIVE_SUBI
#line 1402 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBI;
	}
#line 1621 "syntactic.cpp"
    break;

  case 78: // opcode: DIRECTIVE_ADDI
#line 1406 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDI;
	}
#line 1629 "syntactic.cpp"
    break;

  case 79: // opcode: DIRECTIVE_EORI
#line 1410 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EORI;
	}
#line 1637 "syntactic.cpp"
    break;

  case 80: // opcode: DIRECTIVE_CMPI
#line 1414 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPI;
	}
#line 1645 "syntactic.cpp"
    break;

  case 81: // opcode: DIRECTIVE_BTST
#line 1418 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BTST_STATIC;
	}
#line 1653 "syntactic.cpp"
    break;

  case 82: // opcode: DIRECTIVE_BCHG
#line 1422 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BCHG_STATIC;
	}
#line 1661 "syntactic.cpp"
    break;

  case 83: // opcode: DIRECTIVE_BCLR
#line 1426 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BCLR_STATIC;
	}
#line 1669 "syntactic.cpp"
    break;

  case 84: // opcode: DIRECTIVE_BSET
#line 1430 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BSET_STATIC;
	}
#line 1677 "syntactic.cpp"
    break;

  case 85: // opcode: DIRECTIVE_MOVEP
#line 1434 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEP_TO_REG;
	}
#line 1685 "syntactic.cpp"
    break;

  case 86: // opcode: DIRECTIVE_MOVEA
#line 1438 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEA;
	}
#line 1693 "syntactic.cpp"
    break;

  case 87: // opcode: DIRECTIVE_MOVE
#line 1442 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVE;
	}
#line 1701 "syntactic.cpp"
    break;

  case 88: // opcode: DIRECTIVE_NEGX
#line 1446 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NEGX;
	}
#line 1709 "syntactic.cpp"
    break;

  case 89: // opcode: DIRECTIVE_CLR
#line 1450 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CLR;
	}
#line 1717 "syntactic.cpp"
    break;

  case 90: // opcode: DIRECTIVE_NEG
#line 1454 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NEG;
	}
#line 1725 "syntactic.cpp"
    break;

  case 91: // opcode: DIRECTIVE_NOT
#line 1458 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NOT;
	}
#line 1733 "syntactic.cpp"
    break;

  case 92: // opcode: DIRECTIVE_EXT
#line 1462 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EXT;
	}
#line 1741 "syntactic.cpp"
    break;

  case 93: // opcode: DIRECTIVE_NBCD
#line 1466 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NBCD;
	}
#line 1749 "syntactic.cpp"
    break;

  case 94: // opcode: DIRECTIVE_SWAP
#line 1470 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SWAP;
	}
#line 1757 "syntactic.cpp"
    break;

  case 95: // opcode: DIRECTIVE_PEA
#line 1474 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_PEA;
	}
#line 1765 "syntactic.cpp"
    break;

  case 96: // opcode: DIRECTIVE_ILLEGAL
#line 1478 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ILLEGAL;
	}
#line 1773 "syntactic.cpp"
    break;

  case 97: // opcode: DIRECTIVE_TAS
#line 1482 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TAS;
	}
#line 1781 "syntactic.cpp"
    break;

  case 98: // opcode: DIRECTIVE_TST
#line 1486 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TST;
	}
#line 1789 "syntactic.cpp"
    break;

  case 99: // opcode: DIRECTIVE_TRAP
#line 1490 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TRAP;
	}
#line 1797 "syntactic.cpp"
    break;

  case 100: // opcode: DIRECTIVE_LINK
#line 1494 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LINK;
	}
#line 1805 "syntactic.cpp"
    break;

  case 101: // opcode: DIRECTIVE_UNLK
#line 1498 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_UNLK;
	}
#line 1813 "syntactic.cpp"
    break;

  case 102: // opcode: DIRECTIVE_RESET
#line 1502 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RESET;
	}
#line 1821 "syntactic.cpp"
    break;

  case 103: // opcode: DIRECTIVE_NOP
#line 1506 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NOP;
	}
#line 1829 "syntactic.cpp"
    break;

  case 104: // opcode: DIRECTIVE_STOP
#line 1510 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_STOP;
	}
#line 1837 "syntactic.cpp"
    break;

  case 105: // opcode: DIRECTIVE_RTE
#line 1514 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTE;
	}
#line 1845 "syntactic.cpp"
    break;

  case 106: // opcode: DIRECTIVE_RTS
#line 1518 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTS;
	}
#line 1853 "syntactic.cpp"
    break;

  case 107: // opcode: DIRECTIVE_TRAPV
#line 1522 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TRAPV;
	}
#line 1861 "syntactic.cpp"
    break;

  case 108: // opcode: DIRECTIVE_RTR
#line 1526 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTR;
	}
#line 1869 "syntactic.cpp"
    break;

  case 109: // opcode: DIRECTIVE_JSR
#line 1530 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_JSR;
	}
#line 1877 "syntactic.cpp"
    break;

  case 110: // opcode: DIRECTIVE_JMP
#line 1534 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_JMP;
	}
#line 1885 "syntactic.cpp"
    break;

  case 111: // opcode: DIRECTIVE_MOVEM
#line 1538 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEM_TO_REGS;
	}
#line 1893 "syntactic.cpp"
    break;

  case 112: // opcode: DIRECTIVE_LEA
#line 1542 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LEA;
	}
#line 1901 "syntactic.cpp"
    break;

  case 113: // opcode: DIRECTIVE_CHK
#line 1546 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CHK;
	}
#line 1909 "syntactic.cpp"
    break;

  case 114: // opcode: DIRECTIVE_ADDQ
#line 1550 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDQ;
	}
#line 1917 "syntactic.cpp"
    break;

  case 115: // opcode: DIRECTIVE_SUBQ
#line 1554 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBQ;
	}
#line 1925 "syntactic.cpp"
    break;

  case 116: // opcode: DIRECTIVE_ST
#line 1558 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_TRUE;
	}
#line 1934 "syntactic.cpp"
    break;

  case 117: // opcode: DIRECTIVE_SF
#line 1563 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_FALSE;
	}
#line 1943 "syntactic.cpp"
    break;

  case 118: // opcode: DIRECTIVE_SHI
#line 1568 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 1952 "syntactic.cpp"
    break;

  case 119: // opcode: DIRECTIVE_SLS
#line 1573 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 1961 "syntactic.cpp"
    break;

  case 120: // opcode: DIRECTIVE_SCC
#line 1578 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 1970 "syntactic.cpp"
    break;

  case 121: // opcode: DIRECTIVE_SCS
#line 1583 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 1979 "syntactic.cpp"
    break;

  case 122: // opcode: DIRECTIVE_SNE
#line 1588 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 1988 "syntactic.cpp"
    break;

  case 123: // opcode: DIRECTIVE_SEQ
#line 1593 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 1997 "syntactic.cpp"
    break;

  case 124: // opcode: DIRECTIVE_SVC
#line 1598 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 2006 "syntactic.cpp"
    break;

  case 125: // opcode: DIRECTIVE_SVS
#line 1603 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 2015 "syntactic.cpp"
    break;

  case 126: // opcode: DIRECTIVE_SPL
#line 1608 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 2024 "syntactic.cpp"
    break;

  case 127: // opcode: DIRECTIVE_SMI
#line 1613 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 2033 "syntactic.cpp"
    break;

  case 128: // opcode: DIRECTIVE_SGE
#line 1618 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2042 "syntactic.cpp"
    break;

  case 129: // opcode: DIRECTIVE_SLT
#line 1623 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 2051 "syntactic.cpp"
    break;

  case 130: // opcode: DIRECTIVE_SGT
#line 1628 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 2060 "syntactic.cpp"
    break;

  case 131: // opcode: DIRECTIVE_SLE
#line 1633 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2069 "syntactic.cpp"
    break;

  case 132: // opcode: DIRECTIVE_DBT
#line 1638 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_TRUE;
	}
#line 2078 "syntactic.cpp"
    break;

  case 133: // opcode: DIRECTIVE_DBF
#line 1643 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_FALSE;
	}
#line 2087 "syntactic.cpp"
    break;

  case 134: // opcode: DIRECTIVE_DBHI
#line 1648 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 2096 "syntactic.cpp"
    break;

  case 135: // opcode: DIRECTIVE_DBLS
#line 1653 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 2105 "syntactic.cpp"
    break;

  case 136: // opcode: DIRECTIVE_DBCC
#line 1658 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 2114 "syntactic.cpp"
    break;

  case 137: // opcode: DIRECTIVE_DBCS
#line 1663 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 2123 "syntactic.cpp"
    break;

  case 138: // opcode: DIRECTIVE_DBNE
#line 1668 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 2132 "syntactic.cpp"
    break;

  case 139: // opcode: DIRECTIVE_DBEQ
#line 1673 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 2141 "syntactic.cpp"
    break;

  case 140: // opcode: DIRECTIVE_DBVC
#line 1678 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 2150 "syntactic.cpp"
    break;

  case 141: // opcode: DIRECTIVE_DBVS
#line 1683 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 2159 "syntactic.cpp"
    break;

  case 142: // opcode: DIRECTIVE_DBPL
#line 1688 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 2168 "syntactic.cpp"
    break;

  case 143: // opcode: DIRECTIVE_DBMI
#line 1693 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 2177 "syntactic.cpp"
    break;

  case 144: // opcode: DIRECTIVE_DBGE
#line 1698 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2186 "syntactic.cpp"
    break;

  case 145: // opcode: DIRECTIVE_DBLT
#line 1703 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 2195 "syntactic.cpp"
    break;

  case 146: // opcode: DIRECTIVE_DBGT
#line 1708 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 2204 "syntactic.cpp"
    break;

  case 147: // opcode: DIRECTIVE_DBLE
#line 1713 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2213 "syntactic.cpp"
    break;

  case 148: // opcode: DIRECTIVE_BRA
#line 1718 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BRA;
	}
#line 2221 "syntactic.cpp"
    break;

  case 149: // opcode: DIRECTIVE_BSR
#line 1722 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BSR;
	}
#line 2229 "syntactic.cpp"
    break;

  case 150: // opcode: DIRECTIVE_BHI
#line 1726 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 2238 "syntactic.cpp"
    break;

  case 151: // opcode: DIRECTIVE_BLS
#line 1731 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 2247 "syntactic.cpp"
    break;

  case 152: // opcode: DIRECTIVE_BCC
#line 1736 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 2256 "syntactic.cpp"
    break;

  case 153: // opcode: DIRECTIVE_BCS
#line 1741 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 2265 "syntactic.cpp"
    break;

  case 154: // opcode: DIRECTIVE_BNE
#line 1746 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 2274 "syntactic.cpp"
    break;

  case 155: // opcode: DIRECTIVE_BEQ
#line 1751 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 2283 "syntactic.cpp"
    break;

  case 156: // opcode: DIRECTIVE_BVC
#line 1756 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 2292 "syntactic.cpp"
    break;

  case 157: // opcode: DIRECTIVE_BVS
#line 1761 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 2301 "syntactic.cpp"
    break;

  case 158: // opcode: DIRECTIVE_BPL
#line 1766 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 2310 "syntactic.cpp"
    break;

  case 159: // opcode: DIRECTIVE_BMI
#line 1771 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 2319 "syntactic.cpp"
    break;

  case 160: // opcode: DIRECTIVE_BGE
#line 1776 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2328 "syntactic.cpp"
    break;

  case 161: // opcode: DIRECTIVE_BLT
#line 1781 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 2337 "syntactic.cpp"
    break;

  case 162: // opcode: DIRECTIVE_BGT
#line 1786 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 2346 "syntactic.cpp"
    break;

  case 163: // opcode: DIRECTIVE_BLE
#line 1791 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2355 "syntactic.cpp"
    break;

  case 164: // opcode: DIRECTIVE_MOVEQ
#line 1796 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEQ;
	}
#line 2363 "syntactic.cpp"
    break;

  case 165: // opcode: DIRECTIVE_DIVU
#line 1800 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DIVU;
	}
#line 2371 "syntactic.cpp"
    break;

  case 166: // opcode: DIRECTIVE_DIVS
#line 1804 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DIVS;
	}
#line 2379 "syntactic.cpp"
    break;

  case 167: // opcode: DIRECTIVE_SBCD
#line 1808 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SBCD_DATA_REGS;
	}
#line 2387 "syntactic.cpp"
    break;

  case 168: // opcode: DIRECTIVE_OR
#line 1812 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_OR_TO_REG;
	}
#line 2395 "syntactic.cpp"
    break;

  case 169: // opcode: DIRECTIVE_SUB
#line 1816 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUB_TO_REG;
	}
#line 2403 "syntactic.cpp"
    break;

  case 170: // opcode: DIRECTIVE_SUBX
#line 1820 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBX_DATA_REGS;
	}
#line 2411 "syntactic.cpp"
    break;

  case 171: // opcode: DIRECTIVE_SUBA
#line 1824 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBA;
	}
#line 2419 "syntactic.cpp"
    break;

  case 172: // opcode: DIRECTIVE_EOR
#line 1828 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EOR;
	}
#line 2427 "syntactic.cpp"
    break;

  case 173: // opcode: DIRECTIVE_CMPM
#line 1832 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPM;
	}
#line 2435 "syntactic.cpp"
    break;

  case 174: // opcode: DIRECTIVE_CMP
#line 1836 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMP;
	}
#line 2443 "syntactic.cpp"
    break;

  case 175: // opcode: DIRECTIVE_CMPA
#line 1840 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPA;
	}
#line 2451 "syntactic.cpp"
    break;

  case 176: // opcode: DIRECTIVE_MULU
#line 1844 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MULU;
	}
#line 2459 "syntactic.cpp"
    break;

  case 177: // opcode: DIRECTIVE_MULS
#line 1848 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MULS;
	}
#line 2467 "syntactic.cpp"
    break;

  case 178: // opcode: DIRECTIVE_ABCD
#line 1852 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ABCD_DATA_REGS;
	}
#line 2475 "syntactic.cpp"
    break;

  case 179: // opcode: DIRECTIVE_EXG
#line 1856 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EXG;
	}
#line 2483 "syntactic.cpp"
    break;

  case 180: // opcode: DIRECTIVE_AND
#line 1860 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_AND_TO_REG;
	}
#line 2491 "syntactic.cpp"
    break;

  case 181: // opcode: DIRECTIVE_ADD
#line 1864 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADD_TO_REG;
	}
#line 2499 "syntactic.cpp"
    break;

  case 182: // opcode: DIRECTIVE_ADDX
#line 1868 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDX_DATA_REGS;
	}
#line 2507 "syntactic.cpp"
    break;

  case 183: // opcode: DIRECTIVE_ADDA
#line 1872 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDA;
	}
#line 2515 "syntactic.cpp"
    break;

  case 184: // opcode: DIRECTIVE_ASL
#line 1876 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ASL_STATIC;
	}
#line 2523 "syntactic.cpp"
    break;

  case 185: // opcode: DIRECTIVE_ASR
#line 1880 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ASR_STATIC;
	}
#line 2531 "syntactic.cpp"
    break;

  case 186: // opcode: DIRECTIVE_LSL
#line 1884 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LSL_STATIC;
	}
#line 2539 "syntactic.cpp"
    break;

  case 187: // opcode: DIRECTIVE_LSR
#line 1888 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LSR_STATIC;
	}
#line 2547 "syntactic.cpp"
    break;

  case 188: // opcode: DIRECTIVE_ROXL
#line 1892 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROXL_STATIC;
	}
#line 2555 "syntactic.cpp"
    break;

  case 189: // opcode: DIRECTIVE_ROXR
#line 1896 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROXR_STATIC;
	}
#line 2563 "syntactic.cpp"
    break;

  case 190: // opcode: DIRECTIVE_ROL
#line 1900 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROL_STATIC;
	}
#line 2571 "syntactic.cpp"
    break;

  case 191: // opcode: DIRECTIVE_ROR
#line 1904 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROR_STATIC;
	}
#line 2579 "syntactic.cpp"
    break;

  case 192: // size: SIZE_BYTE
#line 1911 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_BYTE;
	}
#line 2587 "syntactic.cpp"
    break;

  case 193: // size: SIZE_SHORT
#line 1915 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_SHORT;
	}
#line 2595 "syntactic.cpp"
    break;

  case 194: // size: SIZE_WORD
#line 1919 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_WORD;
	}
#line 2603 "syntactic.cpp"
    break;

  case 195: // size: SIZE_LONGWORD
#line 1923 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_LONGWORD;
	}
#line 2611 "syntactic.cpp"
    break;

  case 196: // operand: "(" ADDRESS_REGISTER ")"
#line 1931 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT;
		yylhs.value.as < Operand > ().main_register = yystack_[1].value.as < unsigned long > ();
	}
#line 2620 "syntactic.cpp"
    break;

  case 197: // operand: "(" ADDRESS_REGISTER ")" "+"
#line 1936 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
		yylhs.value.as < Operand > ().main_register = yystack_[2].value.as < unsigned long > ();
	}
#line 2629 "syntactic.cpp"
    break;

  case 198: // operand: "-" "(" ADDRESS_REGISTER ")"
#line 1941 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
		yylhs.value.as < Operand > ().main_register = yystack_[1].value.as < unsigned long > ();
	}
#line 2638 "syntactic.cpp"
    break;

  case 199: // operand: expression "(" ADDRESS_REGISTER ")"
#line 1946 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[3].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[1].value.as < unsigned long > ();
	}
#line 2648 "syntactic.cpp"
    break;

  case 200: // operand: "(" expression "," ADDRESS_REGISTER ")"
#line 1952 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[3].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[1].value.as < unsigned long > ();
	}
#line 2658 "syntactic.cpp"
    break;

  case 201: // operand: "(" ADDRESS_REGISTER "," expression ")"
#line 1958 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[1].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[3].value.as < unsigned long > ();
	}
#line 2668 "syntactic.cpp"
    break;

  case 202: // operand: "(" ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1964 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.type = EXPRESSION_NUMBER;
		yylhs.value.as < Operand > ().literal.shared.unsigned_long = 0;
		yylhs.value.as < Operand > ().main_register = yystack_[4].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 != 0;
	}
#line 2682 "syntactic.cpp"
    break;

  case 203: // operand: expression "(" ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1974 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[6].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[4].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 !=0;
	}
#line 2695 "syntactic.cpp"
    break;

  case 204: // operand: "(" expression "," ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1983 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[6].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[4].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 !=0;
	}
#line 2708 "syntactic.cpp"
    break;

  case 205: // operand: "(" ADDRESS_REGISTER "," data_or_address_register ")"
#line 1992 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.type = EXPRESSION_NUMBER;
		yylhs.value.as < Operand > ().literal.shared.unsigned_long = 0;
		yylhs.value.as < Operand > ().main_register = yystack_[3].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 != 0;
	}
#line 2723 "syntactic.cpp"
    break;

  case 206: // operand: expression "(" ADDRESS_REGISTER "," data_or_address_register ")"
#line 2003 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[5].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[3].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 !=0;
	}
#line 2737 "syntactic.cpp"
    break;

  case 207: // operand: "(" expression "," ADDRESS_REGISTER "," data_or_address_register ")"
#line 2013 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[5].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[3].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 !=0;
	}
#line 2751 "syntactic.cpp"
    break;

  case 208: // operand: expression "(" PROGRAM_COUNTER ")"
#line 2023 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[3].value.as < Expression > ();
	}
#line 2760 "syntactic.cpp"
    break;

  case 209: // operand: "(" expression "," PROGRAM_COUNTER ")"
#line 2028 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[3].value.as < Expression > ();
	}
#line 2769 "syntactic.cpp"
    break;

  case 210: // operand: "(" PROGRAM_COUNTER "," expression ")"
#line 2033 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[1].value.as < Expression > ();
	}
#line 2778 "syntactic.cpp"
    break;

  case 211: // operand: "(" PROGRAM_COUNTER "," data_or_address_register size ")"
#line 2038 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.type = EXPRESSION_NUMBER;
		yylhs.value.as < Operand > ().literal.shared.unsigned_long = 0;
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 != 0;
	}
#line 2791 "syntactic.cpp"
    break;

  case 212: // operand: expression "(" PROGRAM_COUNTER "," data_or_address_register size ")"
#line 2047 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[6].value.as < Expression > ();
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 != 0;
	}
#line 2803 "syntactic.cpp"
    break;

  case 213: // operand: "(" expression "," PROGRAM_COUNTER "," data_or_address_register size ")"
#line 2055 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[6].value.as < Expression > ();
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 != 0;
	}
#line 2815 "syntactic.cpp"
    break;

  case 214: // operand: "(" PROGRAM_COUNTER "," data_or_address_register ")"
#line 2063 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.type = EXPRESSION_NUMBER;
		yylhs.value.as < Operand > ().literal.shared.unsigned_long = 0;
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 != 0;
	}
#line 2829 "syntactic.cpp"
    break;

  case 215: // operand: expression "(" PROGRAM_COUNTER "," data_or_address_register ")"
#line 2073 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[5].value.as < Expression > ();
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 != 0;
	}
#line 2842 "syntactic.cpp"
    break;

  case 216: // operand: "(" expression "," PROGRAM_COUNTER "," data_or_address_register ")"
#line 2082 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[5].value.as < Expression > ();
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 != 0;
	}
#line 2855 "syntactic.cpp"
    break;

  case 217: // operand: "#" expression
#line 2092 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_LITERAL;
		yylhs.value.as < Operand > ().literal = yystack_[0].value.as < Expression > ();
	}
#line 2864 "syntactic.cpp"
    break;

  case 218: // operand: STATUS_REGISTER
#line 2108 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_STATUS_REGISTER;
	}
#line 2872 "syntactic.cpp"
    break;

  case 219: // operand: CONDITION_CODE_REGISTER
#line 2112 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_CONDITION_CODE_REGISTER;
	}
#line 2880 "syntactic.cpp"
    break;

  case 220: // operand: USER_STACK_POINTER_REGISTER
#line 2116 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_USER_STACK_POINTER_REGISTER;
	}
#line 2888 "syntactic.cpp"
    break;

  case 221: // operand: expression
#line 2121 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS;
		yylhs.value.as < Operand > ().literal = yystack_[0].value.as < Expression > ();
		yylhs.value.as < Operand > ().size = SIZE_UNDEFINED;
	}
#line 2898 "syntactic.cpp"
    break;

  case 222: // operand: expression size
#line 2127 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_ABSOLUTE;
		yylhs.value.as < Operand > ().literal = yystack_[1].value.as < Expression > ();
		yylhs.value.as < Operand > ().size = yystack_[0].value.as < Size > ();
	}
#line 2908 "syntactic.cpp"
    break;

  case 223: // operand: register_list
#line 2134 "syntactic.y"
        {
		/* Neat trick to check if only one bit is set (yes, I know that it evaluates to true for 0, but that doesn't matter here). */
		if ((yystack_[0].value.as < unsigned long > () & (yystack_[0].value.as < unsigned long > () - 1)) == 0)
		{
			/* This is a single register. */
			yylhs.value.as < Operand > ().main_register = 0;

			while ((yystack_[0].value.as < unsigned long > () & (1 << yylhs.value.as < Operand > ().main_register)) == 0)
				++yylhs.value.as < Operand > ().main_register;

			if (yylhs.value.as < Operand > ().main_register >= 8)
			{
				yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER;
				yylhs.value.as < Operand > ().main_register -= 8;
			}
			else
			{
				yylhs.value.as < Operand > ().type = OPERAND_DATA_REGISTER;
			}
		}
		else
		{
			/* This is multiple registers. */
			yylhs.value.as < Operand > ().type = OPERAND_REGISTER_LIST;
			yylhs.value.as < Operand > ().main_register = yystack_[0].value.as < unsigned long > (); /* Such a hack... */
		}
	}
#line 2940 "syntactic.cpp"
    break;

  case 224: // register_list: register_span
#line 2165 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = yystack_[0].value.as < unsigned long > ();
	}
#line 2948 "syntactic.cpp"
    break;

  case 225: // register_list: register_span "/" register_list
#line 2169 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = yystack_[2].value.as < unsigned long > () | yystack_[0].value.as < unsigned long > ();
	}
#line 2956 "syntactic.cpp"
    break;

  case 226: // register_span: data_or_address_register
#line 2176 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 1 << yystack_[0].value.as < unsigned long > ();
	}
#line 2964 "syntactic.cpp"
    break;

  case 227: // register_span: data_or_address_register "-" data_or_address_register
#line 2180 "syntactic.y"
        {
		unsigned long start;
		unsigned long end;
		unsigned long i;

		if (yystack_[2].value.as < unsigned long > () < yystack_[0].value.as < unsigned long > ())
		{
			start = yystack_[2].value.as < unsigned long > ();
			end = yystack_[0].value.as < unsigned long > ();
		}
		else
		{
			start = yystack_[0].value.as < unsigned long > ();
			end = yystack_[2].value.as < unsigned long > ();
		}

		yylhs.value.as < unsigned long > () = 0;

		for (i = start; i <= end; ++i)
			yylhs.value.as < unsigned long > () |= 1 << i;
	}
#line 2990 "syntactic.cpp"
    break;

  case 228: // data_or_address_register: DATA_REGISTER
#line 2205 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 0 + yystack_[0].value.as < unsigned long > ();
	}
#line 2998 "syntactic.cpp"
    break;

  case 229: // data_or_address_register: ADDRESS_REGISTER
#line 2209 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 8 + yystack_[0].value.as < unsigned long > ();
	}
#line 3006 "syntactic.cpp"
    break;

  case 230: // expression: expression1
#line 2222 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3014 "syntactic.cpp"
    break;

  case 231: // expression: expression LOGICAL_AND expression1
#line 2227 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_LOGICAL_AND, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3023 "syntactic.cpp"
    break;

  case 232: // expression: expression LOGICAL_OR expression1
#line 2233 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_LOGICAL_OR, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3032 "syntactic.cpp"
    break;

  case 233: // expression1: expression2
#line 2241 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3040 "syntactic.cpp"
    break;

  case 234: // expression1: expression1 "=" expression2
#line 2245 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_EQUALITY, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3049 "syntactic.cpp"
    break;

  case 235: // expression1: expression1 EQUALITY expression2
#line 2250 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_EQUALITY, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3058 "syntactic.cpp"
    break;

  case 236: // expression1: expression1 INEQUALITY expression2
#line 2255 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_INEQUALITY, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3067 "syntactic.cpp"
    break;

  case 237: // expression2: expression3
#line 2263 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3075 "syntactic.cpp"
    break;

  case 238: // expression2: expression2 "<" expression3
#line 2267 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_LESS_THAN, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3084 "syntactic.cpp"
    break;

  case 239: // expression2: expression2 LESS_OR_EQUAL expression3
#line 2272 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_LESS_OR_EQUAL, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3093 "syntactic.cpp"
    break;

  case 240: // expression2: expression2 ">" expression3
#line 2277 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_MORE_THAN, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3102 "syntactic.cpp"
    break;

  case 241: // expression2: expression2 MORE_OR_EQUAL expression3
#line 2282 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_MORE_OR_EQUAL, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3111 "syntactic.cpp"
    break;

  case 242: // expression3: expression4
#line 2290 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3119 "syntactic.cpp"
    break;

  case 243: // expression3: expression3 "+" expression4
#line 2294 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_ADD, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3128 "syntactic.cpp"
    break;

  case 244: // expression3: expression3 "-" expression4
#line 2299 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_SUBTRACT, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3137 "syntactic.cpp"
    break;

  case 245: // expression4: expression5
#line 2307 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3145 "syntactic.cpp"
    break;

  case 246: // expression4: expression4 "*" expression5
#line 2311 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_MULTIPLY, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3154 "syntactic.cpp"
    break;

  case 247: // expression4: expression4 "/" expression5
#line 2316 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_DIVIDE, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3163 "syntactic.cpp"
    break;

  case 248: // expression4: expression4 "%" expression5
#line 2321 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_MODULO, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3172 "syntactic.cpp"
    break;

  case 249: // expression5: expression6
#line 2329 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3180 "syntactic.cpp"
    break;

  case 250: // expression5: expression5 "&" expression6
#line 2333 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_BITWISE_AND, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3189 "syntactic.cpp"
    break;

  case 251: // expression5: expression5 "!" expression6
#line 2338 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_BITWISE_OR, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3198 "syntactic.cpp"
    break;

  case 252: // expression5: expression5 "|" expression6
#line 2343 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_BITWISE_OR, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3207 "syntactic.cpp"
    break;

  case 253: // expression5: expression5 "^" expression6
#line 2348 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_BITWISE_XOR, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3216 "syntactic.cpp"
    break;

  case 254: // expression6: expression7
#line 2356 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3224 "syntactic.cpp"
    break;

  case 255: // expression6: expression6 LEFT_SHIFT expression7
#line 2360 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_LEFT_SHIFT, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3233 "syntactic.cpp"
    break;

  case 256: // expression6: expression6 RIGHT_SHIFT expression7
#line 2365 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_RIGHT_SHIFT, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3242 "syntactic.cpp"
    break;

  case 257: // expression7: expression8
#line 2373 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3250 "syntactic.cpp"
    break;

  case 258: // expression7: "+" expression7
#line 2377 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3258 "syntactic.cpp"
    break;

  case 259: // expression7: "-" expression7
#line 2381 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_NEGATE, &yystack_[0].value.as < Expression > (), NULL))
			YYNOMEM;
	}
#line 3267 "syntactic.cpp"
    break;

  case 260: // expression7: "~" expression7
#line 2386 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_BITWISE_NOT, &yystack_[0].value.as < Expression > (), NULL))
			YYNOMEM;
	}
#line 3276 "syntactic.cpp"
    break;

  case 261: // expression7: "!" expression7
#line 2392 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_LOGICAL_NOT, &yystack_[0].value.as < Expression > (), NULL))
			YYNOMEM;
	}
#line 3285 "syntactic.cpp"
    break;

  case 262: // local_identifier: LOCAL_IDENTIFIER
#line 2399 "syntactic.y"
          { yylhs.value.as < String > () = yystack_[0].value.as < String > (); }
#line 3291 "syntactic.cpp"
    break;

  case 263: // local_identifier: SIZE_BYTE
#line 2401 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".b");
		if (!String_CreateCopyView(&yylhs.value.as < String > (), &literal))
			YYNOMEM;
	}
#line 3301 "syntactic.cpp"
    break;

  case 264: // local_identifier: SIZE_SHORT
#line 2407 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".s");
		if (!String_CreateCopyView(&yylhs.value.as < String > (), &literal))
			YYNOMEM;
	}
#line 3311 "syntactic.cpp"
    break;

  case 265: // local_identifier: SIZE_WORD
#line 2413 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".w");
		if (!String_CreateCopyView(&yylhs.value.as < String > (), &literal))
			YYNOMEM;
	}
#line 3321 "syntactic.cpp"
    break;

  case 266: // local_identifier: SIZE_LONGWORD
#line 2419 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".l");
		if (!String_CreateCopyView(&yylhs.value.as < String > (), &literal))
			YYNOMEM;
	}
#line 3331 "syntactic.cpp"
    break;

  case 267: // identifier: IDENTIFIER
#line 2427 "syntactic.y"
          { yylhs.value.as < String > () = yystack_[0].value.as < String > (); }
#line 3337 "syntactic.cpp"
    break;

  case 268: // identifier: local_identifier
#line 2428 "syntactic.y"
          { yylhs.value.as < String > () = yystack_[0].value.as < String > (); }
#line 3343 "syntactic.cpp"
    break;

  case 269: // identifier: identifier local_identifier
#line 2430 "syntactic.y"
        {
		const cc_bool success = String_CreateAppend(&yylhs.value.as < String > (), &yystack_[1].value.as < String > (), &yystack_[0].value.as < String > ());
		String_Destroy(&yystack_[1].value.as < String > ());
		String_Destroy(&yystack_[0].value.as < String > ());

		if (!success)
			YYNOMEM;
	}
#line 3356 "syntactic.cpp"
    break;

  case 270: // expression8: NUMBER
#line 2442 "syntactic.y"
        {
		yylhs.value.as < Expression > ().type = EXPRESSION_NUMBER;
		yylhs.value.as < Expression > ().shared.unsigned_long = yystack_[0].value.as < unsigned long > ();
	}
#line 3365 "syntactic.cpp"
    break;

  case 271: // expression8: identifier
#line 2447 "syntactic.y"
        {
		yylhs.value.as < Expression > ().type = EXPRESSION_IDENTIFIER;
		yylhs.value.as < Expression > ().shared.string = yystack_[0].value.as < String > ();
	}
#line 3374 "syntactic.cpp"
    break;

  case 272: // expression8: string
#line 2452 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3382 "syntactic.cpp"
    break;

  case 273: // expression8: "*"
#line 2456 "syntactic.y"
        {
		yylhs.value.as < Expression > ().type = EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT;
	}
#line 3390 "syntactic.cpp"
    break;

  case 274: // expression8: "@"
#line 2460 "syntactic.y"
        {
		yylhs.value.as < Expression > ().type = EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION;
	}
#line 3398 "syntactic.cpp"
    break;

  case 275: // expression8: "(" expression ")"
#line 2464 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[1].value.as < Expression > ();
	}
#line 3406 "syntactic.cpp"
    break;

  case 276: // expression8: STRLEN "(" STRING ")"
#line 2468 "syntactic.y"
        {
		yylhs.value.as < Expression > ().type = EXPRESSION_STRLEN;
		yylhs.value.as < Expression > ().shared.string = yystack_[1].value.as < String > ();
	}
#line 3415 "syntactic.cpp"
    break;

  case 277: // expression8: STRCMP "(" string "," string ")"
#line 2473 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), EXPRESSION_STRCMP, &yystack_[3].value.as < Expression > (), &yystack_[1].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3424 "syntactic.cpp"
    break;

  case 278: // expression8: INSTR "(" expression "," expression ")"
#line 2478 "syntactic.y"
        {
		Expression expression;

		expression.type = EXPRESSION_NUMBER;
		expression.shared.unsigned_long = 1;

		if (!DoExpressionTriple(&yylhs.value.as < Expression > (), EXPRESSION_INSTR, &expression, &yystack_[3].value.as < Expression > (), &yystack_[1].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3438 "syntactic.cpp"
    break;

  case 279: // expression8: INSTR "(" expression "," expression "," expression ")"
#line 2488 "syntactic.y"
        {
		if (!DoExpressionTriple(&yylhs.value.as < Expression > (), EXPRESSION_INSTR, &yystack_[5].value.as < Expression > (), &yystack_[3].value.as < Expression > (), &yystack_[1].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3447 "syntactic.cpp"
    break;

  case 280: // expression8: DEF "(" IDENTIFIER ")"
#line 2493 "syntactic.y"
        {
		yylhs.value.as < Expression > ().type = EXPRESSION_DEF;
		yylhs.value.as < Expression > ().shared.string = yystack_[1].value.as < String > ();
	}
#line 3456 "syntactic.cpp"
    break;

  case 281: // expression8: TYPE "(" IDENTIFIER ")"
#line 2498 "syntactic.y"
        {
		yylhs.value.as < Expression > ().type = EXPRESSION_TYPE_WITH_IDENTIFIER;
		yylhs.value.as < Expression > ().shared.string = yystack_[1].value.as < String > ();
	}
#line 3465 "syntactic.cpp"
    break;

  case 282: // expression8: TYPE "(" NUMBER ")"
#line 2503 "syntactic.y"
        {
		yylhs.value.as < Expression > ().type = EXPRESSION_TYPE_WITH_NUMBER;
		yylhs.value.as < Expression > ().shared.unsigned_long = yystack_[1].value.as < unsigned long > ();
	}
#line 3474 "syntactic.cpp"
    break;

  case 283: // expression8: FILESIZE "(" STRING ")"
#line 2508 "syntactic.y"
        {
		yylhs.value.as < Expression > ().type = EXPRESSION_FILESIZE;
		yylhs.value.as < Expression > ().shared.string = yystack_[1].value.as < String > ();
	}
#line 3483 "syntactic.cpp"
    break;

  case 284: // expression8: OFFSET "(" expression ")"
#line 2513 "syntactic.y"
        {
		/* TODO: Make this actually do something besides pass the expression through. */
		yylhs.value.as < Expression > () = yystack_[1].value.as < Expression > ();
	}
#line 3492 "syntactic.cpp"
    break;

  case 285: // string: STRING
#line 2521 "syntactic.y"
        {
		yylhs.value.as < Expression > ().type = EXPRESSION_STRING;
		yylhs.value.as < Expression > ().shared.string = yystack_[0].value.as < String > ();
	}
#line 3501 "syntactic.cpp"
    break;


#line 3505 "syntactic.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
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
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short parser::yypact_ninf_ = -290;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     292,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
    -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,    25,    25,
      25,   613,  -290,  -149,  -136,  -290,  -120,  -112,   613,  -139,
     388,   613,   613,   613,  -290,  -290,   613,  -290,  -290,   613,
     613,  -290,  -290,    25,   613,   613,   613,  -290,   613,  -290,
    -290,  -128,   -98,  -105,  -290,  -290,  -136,   -13,  -290,   -91,
     613,    87,  -290,    42,   -73,  -290,  -290,  -290,  -290,   613,
     613,   613,  -290,  -290,  -290,  -290,  -290,  -290,  -290,  -290,
     -94,   -83,   -70,   -67,   -62,   -49,   -45,   613,   613,   613,
    -290,  -290,   613,   613,    65,   -64,   -79,    49,   -40,    -5,
      67,  -290,  -290,   -36,  -290,  -290,  -290,   -41,   -26,   -26,
    -290,    -2,    65,  -290,  -290,   433,   -92,    65,    65,    65,
      65,   -27,   -24,   613,    65,    65,    65,    65,  -290,    10,
    -290,  -290,   -26,  -290,  -290,  -290,    65,  -290,  -290,  -290,
    -290,  -290,  -290,   293,   658,   613,    21,  -290,    34,    40,
     -69,  -290,  -290,    59,    65,   -22,    65,    90,    99,   613,
     102,    86,   101,   613,  -170,  -290,  -290,  -290,  -290,   613,
     613,   613,   613,   613,   613,   613,   613,   613,   613,   613,
     613,   613,   613,   613,   613,   613,   613,   613,   613,  -290,
    -136,   104,   478,   103,   -20,   523,   613,   105,    65,   106,
    -133,    82,   -60,   568,    65,    42,    91,    91,  -155,  -290,
     613,   613,    84,    88,    -1,    85,    89,    93,    94,  -111,
    -290,   -64,   -64,   -79,   -79,   -79,    49,    49,    49,    49,
     -40,   -40,    -5,    -5,    -5,    67,    67,    67,    67,  -290,
    -290,   -26,  -290,   613,     3,  -290,   111,   112,     5,    65,
      92,  -290,   343,    80,   343,  -150,    95,  -290,  -290,  -290,
    -124,   -54,    65,    65,  -290,    99,   613,  -290,  -290,  -290,
    -290,  -290,    65,   613,  -290,  -290,   121,   613,  -127,   -55,
    -290,  -123,   -34,   -21,     2,  -290,    91,  -290,    91,  -290,
     259,   -57,    65,  -290,    59,  -290,   264,  -290,  -290,   270,
    -290,    91,  -290,    91,  -290,  -116,  -107,  -290,   613,  -290,
    -290,  -290,   -88,   -82,  -290,   271,  -290,   272,   -30,  -290,
     273,  -290,   275,  -290,  -290,  -290,  -290,  -290
  };

  const short
  parser::yydefact_[] =
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
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     273,   274,     0,     0,    12,   230,   233,   237,   242,   245,
     249,   254,   268,   271,   257,   272,    65,    15,    16,    19,
       7,     8,    21,    23,    22,     0,     0,    28,    30,    31,
      35,     0,     0,     0,    44,    45,    47,    49,    67,    52,
      53,    54,    57,    59,    60,    62,    29,     1,   228,   229,
     218,   219,   220,     0,     0,     0,    70,   223,   224,   226,
     221,    73,    74,     4,    63,     0,     6,     0,     0,     0,
       0,     0,     0,     0,     0,   258,   259,   261,   260,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   269,
       0,     0,     0,     0,     0,     0,     0,     0,    43,     0,
       0,     0,     0,     0,   217,     0,     0,     0,     0,   222,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     275,   231,   232,   235,   236,   234,   239,   241,   238,   240,
     243,   244,   246,   247,   248,   250,   253,   251,   252,   255,
     256,    17,    66,     0,     9,    24,     0,     0,     0,    38,
      39,    68,     0,   196,     0,     0,     0,    71,   225,   227,
       0,     0,    64,     5,   276,     0,     0,   280,   282,   281,
     283,   284,    10,     0,    26,    25,     0,     0,     0,     0,
     197,     0,     0,     0,     0,   198,     0,   199,     0,   208,
       0,     0,    11,    27,    40,   205,     0,   201,   214,     0,
     210,     0,   200,     0,   209,     0,     0,   277,     0,   278,
     202,   211,     0,     0,   206,     0,   215,     0,     0,   207,
       0,   216,     0,   203,   212,   279,   204,   213
  };

  const short
  parser::yypgoto_[] =
  {
    -290,  -290,    96,  -122,  -290,  -290,  -290,  -290,  -119,   166,
     168,  -290,  -289,  -117,    -3,  -109,   -56,   -10,   -17,   -61,
    -183,   269,  -290,  -290,  -255
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,   161,   253,   208,   229,   162,   163,   164,   169,   246,
     247,   248,   249,   250,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205
  };

  const short
  parser::yytable_[] =
  {
     170,   171,   209,   313,   194,   265,   266,   269,   270,   267,
     268,   212,   360,   216,   217,   218,   219,   383,   359,   220,
     206,   361,   221,   222,   223,   320,   384,   224,   225,   226,
     213,   227,   214,   206,   232,   165,   166,   167,   168,   165,
     166,   167,   168,   236,   228,   252,   165,   166,   167,   168,
     207,   210,   254,   255,   256,   165,   166,   167,   168,   211,
     352,   266,   353,   378,   231,   381,   269,   270,   395,   386,
     264,   387,   398,   230,   165,   166,   167,   168,   235,   414,
     165,   166,   167,   168,   371,   269,   270,   237,   416,   165,
     166,   167,   168,   165,   166,   167,   168,   405,   294,   406,
     257,   295,   274,   275,   339,   340,   298,   419,   269,   270,
     390,   258,   412,   421,   413,   271,   272,   269,   270,   251,
     269,   270,   269,   270,   259,   308,   302,   260,   304,   276,
     277,   309,   261,   355,   178,   320,   408,   273,   409,   388,
     397,   389,   314,   269,   270,   262,   319,   269,   270,   263,
     269,   270,   290,   269,   270,   269,   270,   269,   270,   280,
     281,   400,   323,   324,   325,   425,   296,   291,   341,   297,
     282,   311,   401,   346,   402,   344,   269,   270,   348,   349,
     269,   270,   269,   270,   233,   234,   264,   165,   166,   167,
     168,   292,   366,   362,   363,   403,   373,   404,   376,   283,
     284,   285,   286,   299,   172,   173,   174,   175,   238,   239,
     176,   177,   178,   179,   305,   240,   241,   242,   326,   327,
     328,   329,   335,   336,   337,   338,   372,   180,   181,   182,
     183,   184,   185,   186,   306,   379,   243,   382,   307,   188,
     244,   190,   269,   270,   191,   245,   278,   279,   192,   391,
     287,   288,   310,   193,   316,   317,   392,   238,   239,   396,
     254,   312,   399,   332,   333,   334,   321,   322,   330,   331,
     179,   315,   318,   342,   345,   354,   350,   380,   351,   364,
     367,   365,   374,   375,   368,   377,   415,   417,   369,   370,
     385,   418,   393,   420,   422,     1,     2,     3,     4,     5,
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
     156,   157,   158,   159,   407,   172,   173,   174,   175,   410,
     300,   176,   177,   178,   179,   411,   423,   424,   426,   301,
     427,   357,   289,   394,   358,     0,     0,     0,   180,   181,
     182,   183,   184,   185,   186,     0,     0,   187,     0,     0,
     188,   189,   190,   160,     0,   191,     0,     0,     0,   192,
       0,     0,     0,     0,   193,   172,   173,   174,   175,   238,
     239,   176,   177,   178,   179,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   180,   181,
     182,   183,   184,   185,   186,     0,     0,   187,     0,     0,
     188,   189,   190,     0,     0,   191,     0,     0,     0,   192,
     172,   173,   174,   175,   193,     0,   176,   177,   178,   179,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,   183,   184,   185,   186,
       0,   215,   187,     0,     0,   188,   189,   190,     0,     0,
     191,     0,     0,     0,   192,   172,   173,   174,   175,   193,
       0,   176,   177,   178,   179,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   180,   181,
     182,   183,   184,   185,   186,     0,   293,   187,     0,     0,
     188,   189,   190,     0,     0,   191,     0,     0,     0,   192,
     172,   173,   174,   175,   193,     0,   176,   177,   178,   179,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,   183,   184,   185,   186,
       0,   343,   187,     0,     0,   188,   189,   190,     0,     0,
     191,     0,     0,     0,   192,   172,   173,   174,   175,   193,
       0,   176,   177,   178,   179,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   180,   181,
     182,   183,   184,   185,   186,     0,   347,   187,     0,     0,
     188,   189,   190,     0,     0,   191,     0,     0,     0,   192,
     172,   173,   174,   175,   193,   356,   176,   177,   178,   179,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,   183,   184,   185,   186,
       0,     0,   187,     0,     0,   188,   189,   190,     0,     0,
     191,     0,     0,     0,   192,   172,   173,   174,   175,   193,
       0,   176,   177,   178,   179,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   180,   181,
     182,   183,   184,   185,   186,     0,     0,   187,     0,     0,
     188,   189,   190,     0,     0,   191,     0,     0,     0,   192,
     172,   173,   174,   175,   193,     0,   176,   177,   178,   179,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   180,   181,   182,   183,   184,   185,   186,
       0,     0,   303,     0,     0,   188,   189,   190,     0,     0,
     191,     0,     0,     0,   192,     0,     0,     0,     0,   193
  };

  const short
  parser::yycheck_[] =
  {
     119,   120,   124,   258,   121,   188,   189,   177,   178,   192,
     193,   128,   167,   130,   131,   132,   133,   167,   307,   136,
     169,   176,   139,   140,   143,   195,   176,   144,   145,   146,
     169,   148,   171,   169,   156,   162,   163,   164,   165,   162,
     163,   164,   165,   160,   172,   164,   162,   163,   164,   165,
     199,   171,   169,   170,   171,   162,   163,   164,   165,   171,
     193,   244,   195,   352,   169,   354,   177,   178,   195,   193,
     187,   195,   195,   171,   162,   163,   164,   165,   169,   195,
     162,   163,   164,   165,   195,   177,   178,     0,   195,   162,
     163,   164,   165,   162,   163,   164,   165,   386,   215,   388,
     194,   193,   181,   182,   287,   288,   223,   195,   177,   178,
     365,   194,   401,   195,   403,   179,   180,   177,   178,   192,
     177,   178,   177,   178,   194,   194,   243,   194,   245,   208,
     209,   250,   194,   193,   170,   195,   193,   201,   195,   193,
     195,   195,   259,   177,   178,   194,   263,   177,   178,   194,
     177,   178,   193,   177,   178,   177,   178,   177,   178,   199,
     200,   195,   271,   272,   273,   195,   193,   193,   290,   193,
     210,   193,   193,   193,   195,   292,   177,   178,   295,   296,
     177,   178,   177,   178,   197,   198,   303,   162,   163,   164,
     165,   193,   193,   310,   311,   193,   193,   195,   193,   204,
     205,   206,   207,   193,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   193,   173,   174,   175,   274,   275,
     276,   277,   283,   284,   285,   286,   343,   185,   186,   187,
     188,   189,   190,   191,   200,   352,   194,   354,   198,   197,
     198,   199,   177,   178,   202,   203,   197,   198,   206,   366,
     183,   184,   193,   211,   168,   169,   373,   166,   167,   378,
     377,   171,   381,   280,   281,   282,   269,   270,   278,   279,
     171,   169,   171,   169,   171,   193,   171,   197,   172,   195,
     195,   193,   171,   171,   195,   193,   405,   406,   195,   195,
     195,   408,   171,   412,   413,     3,     4,     5,     6,     7,
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
     158,   159,   160,   161,   195,   162,   163,   164,   165,   195,
     167,   168,   169,   170,   171,   195,   195,   195,   195,   176,
     195,   305,   203,   377,   306,    -1,    -1,    -1,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,   194,    -1,    -1,
     197,   198,   199,   201,    -1,   202,    -1,    -1,    -1,   206,
      -1,    -1,    -1,    -1,   211,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,   194,    -1,    -1,
     197,   198,   199,    -1,    -1,   202,    -1,    -1,    -1,   206,
     162,   163,   164,   165,   211,    -1,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
      -1,   193,   194,    -1,    -1,   197,   198,   199,    -1,    -1,
     202,    -1,    -1,    -1,   206,   162,   163,   164,   165,   211,
      -1,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
     187,   188,   189,   190,   191,    -1,   193,   194,    -1,    -1,
     197,   198,   199,    -1,    -1,   202,    -1,    -1,    -1,   206,
     162,   163,   164,   165,   211,    -1,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
      -1,   193,   194,    -1,    -1,   197,   198,   199,    -1,    -1,
     202,    -1,    -1,    -1,   206,   162,   163,   164,   165,   211,
      -1,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
     187,   188,   189,   190,   191,    -1,   193,   194,    -1,    -1,
     197,   198,   199,    -1,    -1,   202,    -1,    -1,    -1,   206,
     162,   163,   164,   165,   211,   167,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,   194,    -1,    -1,   197,   198,   199,    -1,    -1,
     202,    -1,    -1,    -1,   206,   162,   163,   164,   165,   211,
      -1,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
     187,   188,   189,   190,   191,    -1,    -1,   194,    -1,    -1,
     197,   198,   199,    -1,    -1,   202,    -1,    -1,    -1,   206,
     162,   163,   164,   165,   211,    -1,   168,   169,   170,   171,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
      -1,    -1,   194,    -1,    -1,   197,   198,   199,    -1,    -1,
     202,    -1,    -1,    -1,   206,    -1,    -1,    -1,    -1,   211
  };

  const unsigned char
  parser::yystos_[] =
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
     201,   215,   219,   220,   221,   162,   163,   164,   165,   222,
     222,   222,   162,   163,   164,   165,   168,   169,   170,   171,
     185,   186,   187,   188,   189,   190,   191,   194,   197,   198,
     199,   202,   206,   211,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   169,   199,   217,   217,
     171,   171,   227,   169,   171,   193,   227,   227,   227,   227,
     227,   227,   227,   222,   227,   227,   227,   227,   172,   218,
     171,   169,   217,   197,   198,   169,   227,     0,   166,   167,
     173,   174,   175,   194,   198,   203,   223,   224,   225,   226,
     227,   192,   222,   216,   227,   227,   227,   194,   194,   194,
     194,   194,   194,   194,   227,   234,   234,   234,   234,   177,
     178,   179,   180,   201,   181,   182,   208,   209,   197,   198,
     199,   200,   210,   204,   205,   206,   207,   183,   184,   235,
     193,   193,   193,   193,   227,   193,   193,   193,   227,   193,
     167,   176,   227,   194,   227,   193,   200,   198,   194,   222,
     193,   193,   171,   238,   227,   169,   168,   169,   171,   227,
     195,   228,   228,   229,   229,   229,   230,   230,   230,   230,
     231,   231,   232,   232,   232,   233,   233,   233,   233,   234,
     234,   217,   169,   193,   227,   171,   193,   193,   227,   227,
     171,   172,   193,   195,   193,   193,   167,   223,   224,   226,
     167,   176,   227,   227,   195,   193,   193,   195,   195,   195,
     195,   195,   227,   193,   171,   171,   193,   193,   226,   227,
     197,   226,   227,   167,   176,   195,   193,   195,   193,   195,
     238,   227,   227,   171,   216,   195,   222,   195,   195,   222,
     195,   193,   195,   193,   195,   226,   226,   195,   193,   195,
     195,   195,   226,   226,   195,   222,   195,   222,   227,   195,
     222,   195,   222,   195,   195,   195,   195,   195
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   214,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   215,   215,   215,   215,   215,   215,   215,
     215,   215,   215,   216,   216,   217,   217,   218,   218,   219,
     219,   219,   220,   220,   220,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   221,   221,   221,   221,   221,   221,   221,   221,
     221,   221,   222,   222,   222,   222,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   224,   224,   225,   225,   226,   226,
     227,   227,   227,   228,   228,   228,   228,   229,   229,   229,
     229,   229,   230,   230,   230,   231,   231,   231,   231,   232,
     232,   232,   232,   232,   233,   233,   233,   234,   234,   234,
     234,   234,   235,   235,   235,   235,   235,   236,   236,   236,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   237,   238
  };

  const signed char
  parser::yyr2_[] =
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


#if M68KASM_DEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
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
  "FILESIZE", "OFFSET", "\".\"", "\",\"", "\"(\"", "\")\"", "\"$\"",
  "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"=\"", "\"@\"", "\"#\"", "\"&\"",
  "\"^\"", "\"!\"", "\"|\"", "\"<\"", "\">\"", "\"%\"", "\"~\"",
  "LOWER_PRECEDENCE_THAN_SIZE", "HIGHER_PRECEDENCE_THAN_SIZE", "$accept",
  "statement", "expression_list", "identifier_list", "option_list",
  "instruction", "full_opcode", "opcode", "size", "operand",
  "register_list", "register_span", "data_or_address_register",
  "expression", "expression1", "expression2", "expression3", "expression4",
  "expression5", "expression6", "expression7", "local_identifier",
  "identifier", "expression8", "string", YY_NULLPTR
  };
#endif


#if M68KASM_DEBUG
  const short
  parser::yyrline_[] =
  {
       0,   880,   880,   883,   888,   894,   901,   907,   912,   920,
     927,   936,   944,   949,   953,   960,   967,   973,   979,   986,
     992,   996,  1001,  1006,  1011,  1020,  1028,  1036,  1043,  1048,
    1053,  1058,  1063,  1067,  1071,  1075,  1080,  1084,  1088,  1094,
    1100,  1110,  1114,  1118,  1124,  1129,  1136,  1140,  1145,  1149,
    1154,  1158,  1162,  1167,  1172,  1177,  1181,  1185,  1190,  1195,
    1200,  1205,  1210,  1218,  1235,  1263,  1280,  1308,  1325,  1353,
    1359,  1365,  1374,  1379,  1385,  1393,  1397,  1401,  1405,  1409,
    1413,  1417,  1421,  1425,  1429,  1433,  1437,  1441,  1445,  1449,
    1453,  1457,  1461,  1465,  1469,  1473,  1477,  1481,  1485,  1489,
    1493,  1497,  1501,  1505,  1509,  1513,  1517,  1521,  1525,  1529,
    1533,  1537,  1541,  1545,  1549,  1553,  1557,  1562,  1567,  1572,
    1577,  1582,  1587,  1592,  1597,  1602,  1607,  1612,  1617,  1622,
    1627,  1632,  1637,  1642,  1647,  1652,  1657,  1662,  1667,  1672,
    1677,  1682,  1687,  1692,  1697,  1702,  1707,  1712,  1717,  1721,
    1725,  1730,  1735,  1740,  1745,  1750,  1755,  1760,  1765,  1770,
    1775,  1780,  1785,  1790,  1795,  1799,  1803,  1807,  1811,  1815,
    1819,  1823,  1827,  1831,  1835,  1839,  1843,  1847,  1851,  1855,
    1859,  1863,  1867,  1871,  1875,  1879,  1883,  1887,  1891,  1895,
    1899,  1903,  1910,  1914,  1918,  1922,  1930,  1935,  1940,  1945,
    1951,  1957,  1963,  1973,  1982,  1991,  2002,  2012,  2022,  2027,
    2032,  2037,  2046,  2054,  2062,  2072,  2081,  2091,  2107,  2111,
    2115,  2120,  2126,  2133,  2164,  2168,  2175,  2179,  2204,  2208,
    2221,  2226,  2232,  2240,  2244,  2249,  2254,  2262,  2266,  2271,
    2276,  2281,  2289,  2293,  2298,  2306,  2310,  2315,  2320,  2328,
    2332,  2337,  2342,  2347,  2355,  2359,  2364,  2372,  2376,  2380,
    2385,  2391,  2399,  2400,  2406,  2412,  2418,  2427,  2428,  2429,
    2441,  2446,  2451,  2455,  2459,  2463,  2467,  2472,  2477,  2487,
    2492,  2497,  2502,  2507,  2512,  2520
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // M68KASM_DEBUG


#line 25 "syntactic.y"
} // m68kasm
#line 4403 "syntactic.cpp"

#line 2526 "syntactic.y"


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
			DestroyIdentifierList(&statement->shared.macro.parameter_names);
			break;

		case STATEMENT_TYPE_MACROS:
			DestroyIdentifierList(&statement->shared.macros.parameter_names);
			break;

		case STATEMENT_TYPE_EQU:
			DestroyExpression(&statement->shared.equ.expression);
			break;

		case STATEMENT_TYPE_SET:
			DestroyExpression(&statement->shared.set.expression);
			break;

		case STATEMENT_TYPE_IF:
			DestroyExpression(&statement->shared.if_statement.expression);
			break;

		case STATEMENT_TYPE_ELSEIF:
			DestroyExpression(&statement->shared.elseif.expression);
			break;

		case STATEMENT_TYPE_WHILE:
			DestroyExpression(&statement->shared.while_statement.expression);
			break;

		case STATEMENT_TYPE_RSSET:
			DestroyExpression(&statement->shared.rsset.expression);
			break;

		case STATEMENT_TYPE_OBJ:
			DestroyExpression(&statement->shared.obj.expression);
			break;

		case STATEMENT_TYPE_ORG:
			DestroyExpression(&statement->shared.org.expression);
			break;

		case STATEMENT_TYPE_EQUS_STRING:
			String_Destroy(&statement->shared.equs_string.string);
			break;

		case STATEMENT_TYPE_EQUS_IDENTIFIER:
			String_Destroy(&statement->shared.equs_identifier.string);
			break;

		case STATEMENT_TYPE_PUSHP:
			String_Destroy(&statement->shared.pushp.string);
			break;

		case STATEMENT_TYPE_POPP:
			String_Destroy(&statement->shared.popp.string);
			break;

		case STATEMENT_TYPE_PURGE:
			String_Destroy(&statement->shared.purge.string);
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
