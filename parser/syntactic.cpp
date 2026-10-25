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
#line 644 "syntactic.y"


#include "../error.h"

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


#line 78 "syntactic.cpp"


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
#line 152 "syntactic.cpp"

  /// Build a parser object.
  parser::parser (void *scanner_yyarg, ErrorManager &error_manager_yyarg, Statement &statement_yyarg)
#if M68KASM_DEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      error_manager (error_manager_yyarg),
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
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
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
        value.move< std::string > (YY_MOVE (that.value));
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
        value.copy< std::string > (that.value);
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
        value.move< std::string > (that.value);
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
            symbol_type yylookahead (yylex (scanner, error_manager));
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
        yylhs.value.emplace< std::string > ();
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
#line 913 "syntactic.y"
        {
		statement.emplace<StatementEmpty>();
	}
#line 823 "syntactic.cpp"
    break;

  case 3: // statement: instruction
#line 917 "syntactic.y"
        {
		statement.emplace<StatementInstruction>(YY_MOVE (yystack_[0].value.as < StatementInstruction > ()));
	}
#line 831 "syntactic.cpp"
    break;

  case 4: // statement: DIRECTIVE_DC size expression_list
#line 921 "syntactic.y"
        {
		statement.emplace<StatementDc>(YY_MOVE (yystack_[1].value.as < Size > ()), YY_MOVE (yystack_[0].value.as < ExpressionList > ()));
	}
#line 839 "syntactic.cpp"
    break;

  case 5: // statement: DIRECTIVE_DCB size expression "," expression
#line 925 "syntactic.y"
        {
		statement.emplace<StatementDcb>(YY_MOVE (yystack_[3].value.as < Size > ()), YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 847 "syntactic.cpp"
    break;

  case 6: // statement: DIRECTIVE_DS size expression
#line 929 "syntactic.y"
        {
		statement.emplace<StatementDs>(YY_MOVE (yystack_[1].value.as < Size > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 855 "syntactic.cpp"
    break;

  case 7: // statement: DIRECTIVE_INCLUDE STRING
#line 933 "syntactic.y"
        {
		statement.emplace<StatementInclude>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 863 "syntactic.cpp"
    break;

  case 8: // statement: DIRECTIVE_INCBIN STRING
#line 937 "syntactic.y"
        {		
		statement.emplace<StatementIncbin>(YY_MOVE (yystack_[0].value.as < std::string > ()), ExpressionNumber{0}, false);
	}
#line 871 "syntactic.cpp"
    break;

  case 9: // statement: DIRECTIVE_INCBIN STRING "," expression
#line 941 "syntactic.y"
        {
		statement.emplace<StatementIncbin>(YY_MOVE (yystack_[2].value.as < std::string > ()), YY_MOVE (yystack_[0].value.as < Expression > ()), false);
	}
#line 879 "syntactic.cpp"
    break;

  case 10: // statement: DIRECTIVE_INCBIN STRING "," "," expression
#line 945 "syntactic.y"
        {
		statement.emplace<StatementIncbin>(YY_MOVE (yystack_[3].value.as < std::string > ()), ExpressionNumber{0}, true, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 887 "syntactic.cpp"
    break;

  case 11: // statement: DIRECTIVE_INCBIN STRING "," expression "," expression
#line 949 "syntactic.y"
        {
		statement.emplace<StatementIncbin>(YY_MOVE (yystack_[4].value.as < std::string > ()), YY_MOVE (yystack_[2].value.as < Expression > ()), true, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 895 "syntactic.cpp"
    break;

  case 12: // statement: DIRECTIVE_REPT expression
#line 953 "syntactic.y"
        {
		statement.emplace<StatementRept>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 903 "syntactic.cpp"
    break;

  case 13: // statement: DIRECTIVE_ENDR
#line 957 "syntactic.y"
        {
		statement.emplace<StatementEndr>();
	}
#line 911 "syntactic.cpp"
    break;

  case 14: // statement: DIRECTIVE_MACRO
#line 961 "syntactic.y"
        {
		statement.emplace<StatementMacro>(false);
	}
#line 919 "syntactic.cpp"
    break;

  case 15: // statement: DIRECTIVE_MACRO "*"
#line 965 "syntactic.y"
        {
		statement.emplace<StatementMacro>(true);
	}
#line 927 "syntactic.cpp"
    break;

  case 16: // statement: DIRECTIVE_MACRO identifier_list
#line 969 "syntactic.y"
        {
		statement.emplace<StatementMacro>(false, YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 935 "syntactic.cpp"
    break;

  case 17: // statement: DIRECTIVE_MACRO "*" "," identifier_list
#line 973 "syntactic.y"
        {
		statement.emplace<StatementMacro>(true, YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 943 "syntactic.cpp"
    break;

  case 18: // statement: DIRECTIVE_MACROS
#line 977 "syntactic.y"
        {
		statement.emplace<StatementMacros>(StatementMacro{false, {}});
	}
#line 951 "syntactic.cpp"
    break;

  case 19: // statement: DIRECTIVE_MACROS identifier_list
#line 981 "syntactic.y"
        {
		statement.emplace<StatementMacros>(StatementMacro{false, YY_MOVE (yystack_[0].value.as < IdentifierList > ())});
	}
#line 959 "syntactic.cpp"
    break;

  case 20: // statement: DIRECTIVE_ENDM
#line 985 "syntactic.y"
        {
		statement.emplace<StatementEndm>();
	}
#line 967 "syntactic.cpp"
    break;

  case 21: // statement: DIRECTIVE_EQU expression
#line 989 "syntactic.y"
        {
		statement.emplace<StatementEqu>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 975 "syntactic.cpp"
    break;

  case 22: // statement: DIRECTIVE_EQUS STRING
#line 993 "syntactic.y"
        {
		statement.emplace<StatementEqusString>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 983 "syntactic.cpp"
    break;

  case 23: // statement: DIRECTIVE_EQUS IDENTIFIER
#line 997 "syntactic.y"
        {
		statement.emplace<StatementEqusIdentifier>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 991 "syntactic.cpp"
    break;

  case 24: // statement: DIRECTIVE_SUBSTR "," "," STRING
#line 1001 "syntactic.y"
        {
		std::string string = YY_MOVE (yystack_[0].value.as < std::string > ());
		const auto string_length = string.length();
		statement.emplace<StatementSubstr>(ExpressionNumber{1}, ExpressionNumber{string_length}, std::move(string));
	}
#line 1001 "syntactic.cpp"
    break;

  case 25: // statement: DIRECTIVE_SUBSTR expression "," "," STRING
#line 1007 "syntactic.y"
        {
		std::string string = YY_MOVE (yystack_[0].value.as < std::string > ());
		const auto string_length = string.length();
		statement.emplace<StatementSubstr>(YY_MOVE (yystack_[3].value.as < Expression > ()), ExpressionNumber{string_length}, std::move(string));
	}
#line 1011 "syntactic.cpp"
    break;

  case 26: // statement: DIRECTIVE_SUBSTR "," expression "," STRING
#line 1013 "syntactic.y"
        {
		statement.emplace<StatementSubstr>(ExpressionNumber{1}, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1019 "syntactic.cpp"
    break;

  case 27: // statement: DIRECTIVE_SUBSTR expression "," expression "," STRING
#line 1017 "syntactic.y"
        {
		statement.emplace<StatementSubstr>(YY_MOVE (yystack_[4].value.as < Expression > ()), YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1027 "syntactic.cpp"
    break;

  case 28: // statement: DIRECTIVE_SET expression
#line 1021 "syntactic.y"
        {
		statement.emplace<StatementSet>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1035 "syntactic.cpp"
    break;

  case 29: // statement: "=" expression
#line 1025 "syntactic.y"
        {
		statement.emplace<StatementSet>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1043 "syntactic.cpp"
    break;

  case 30: // statement: DIRECTIVE_IF expression
#line 1029 "syntactic.y"
        {
		statement.emplace<StatementIf>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1051 "syntactic.cpp"
    break;

  case 31: // statement: DIRECTIVE_ELSEIF expression
#line 1033 "syntactic.y"
        {
		statement.emplace<StatementElseif>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1059 "syntactic.cpp"
    break;

  case 32: // statement: DIRECTIVE_ELSEIF
#line 1037 "syntactic.y"
        {
		statement.emplace<StatementElse>();
	}
#line 1067 "syntactic.cpp"
    break;

  case 33: // statement: DIRECTIVE_ELSE
#line 1041 "syntactic.y"
        {
		statement.emplace<StatementElse>();
	}
#line 1075 "syntactic.cpp"
    break;

  case 34: // statement: DIRECTIVE_ENDC
#line 1045 "syntactic.y"
        {
		statement.emplace<StatementEndc>();
	}
#line 1083 "syntactic.cpp"
    break;

  case 35: // statement: DIRECTIVE_WHILE expression
#line 1049 "syntactic.y"
        {
		statement.emplace<StatementWhile>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1091 "syntactic.cpp"
    break;

  case 36: // statement: DIRECTIVE_ENDW
#line 1053 "syntactic.y"
        {
		statement.emplace<StatementEndw>();
	}
#line 1099 "syntactic.cpp"
    break;

  case 37: // statement: DIRECTIVE_EVEN
#line 1057 "syntactic.y"
        {
		statement.emplace<StatementEven>();
	}
#line 1107 "syntactic.cpp"
    break;

  case 38: // statement: DIRECTIVE_CNOP expression "," expression
#line 1061 "syntactic.y"
        {
		statement.emplace<StatementCnop>(YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1115 "syntactic.cpp"
    break;

  case 39: // statement: DIRECTIVE_ALIGN expression
#line 1065 "syntactic.y"
        {
		statement.emplace<StatementCnop>(ExpressionNumber{0}, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1123 "syntactic.cpp"
    break;

  case 40: // statement: DIRECTIVE_INFORM expression "," STRING
#line 1069 "syntactic.y"
        {
		statement.emplace<StatementInform>(YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1131 "syntactic.cpp"
    break;

  case 41: // statement: DIRECTIVE_INFORM expression "," STRING "," expression_list
#line 1073 "syntactic.y"
        {
		/* TODO - parameters */
		statement.emplace<StatementInform>(YY_MOVE (yystack_[4].value.as < Expression > ()), YY_MOVE (yystack_[2].value.as < std::string > ()));
	}
#line 1140 "syntactic.cpp"
    break;

  case 42: // statement: DIRECTIVE_FAIL
#line 1078 "syntactic.y"
        {
		statement.emplace<StatementFail>();
	}
#line 1148 "syntactic.cpp"
    break;

  case 43: // statement: DIRECTIVE_END
#line 1082 "syntactic.y"
        {
		statement.emplace<StatementEnd>();
	}
#line 1156 "syntactic.cpp"
    break;

  case 44: // statement: DIRECTIVE_RS size expression
#line 1086 "syntactic.y"
        {
		statement.emplace<StatementRs>(YY_MOVE (yystack_[1].value.as < Size > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1164 "syntactic.cpp"
    break;

  case 45: // statement: DIRECTIVE_RSSET expression
#line 1090 "syntactic.y"
        {
		statement.emplace<StatementRsset>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1172 "syntactic.cpp"
    break;

  case 46: // statement: DIRECTIVE_RSRESET expression
#line 1094 "syntactic.y"
        {
		/* As mentioned in S.N. 68k's manual, this odd hack
		   is necessary for compatibility with other assemblers. */
		statement.emplace<StatementRsset>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1182 "syntactic.cpp"
    break;

  case 47: // statement: DIRECTIVE_RSRESET
#line 1100 "syntactic.y"
        {
		statement.emplace<StatementRsreset>();
	}
#line 1190 "syntactic.cpp"
    break;

  case 48: // statement: DIRECTIVE_OBJ expression
#line 1104 "syntactic.y"
        {
		statement.emplace<StatementObj>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1198 "syntactic.cpp"
    break;

  case 49: // statement: DIRECTIVE_OBJEND
#line 1108 "syntactic.y"
        {
		statement.emplace<StatementObjend>();
	}
#line 1206 "syntactic.cpp"
    break;

  case 50: // statement: DIRECTIVE_ORG expression
#line 1112 "syntactic.y"
        {
		statement.emplace<StatementOrg>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1214 "syntactic.cpp"
    break;

  case 51: // statement: DIRECTIVE_PUSHO
#line 1116 "syntactic.y"
        {
		statement.emplace<StatementPusho>();
	}
#line 1222 "syntactic.cpp"
    break;

  case 52: // statement: DIRECTIVE_POPO
#line 1120 "syntactic.y"
        {
		statement.emplace<StatementPopo>();
	}
#line 1230 "syntactic.cpp"
    break;

  case 53: // statement: DIRECTIVE_OPT option_list
#line 1124 "syntactic.y"
        {
		statement.emplace<StatementOpt>(YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 1238 "syntactic.cpp"
    break;

  case 54: // statement: DIRECTIVE_PUSHP STRING
#line 1128 "syntactic.y"
        {
		statement.emplace<StatementPushp>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1246 "syntactic.cpp"
    break;

  case 55: // statement: DIRECTIVE_POPP IDENTIFIER
#line 1132 "syntactic.y"
        {
		statement.emplace<StatementPopp>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1254 "syntactic.cpp"
    break;

  case 56: // statement: DIRECTIVE_SHIFT
#line 1136 "syntactic.y"
        {
		statement.emplace<StatementShift>();
	}
#line 1262 "syntactic.cpp"
    break;

  case 57: // statement: DIRECTIVE_MEXIT
#line 1140 "syntactic.y"
        {
		statement.emplace<StatementMexit>();
	}
#line 1270 "syntactic.cpp"
    break;

  case 58: // statement: DIRECTIVE_LOCAL identifier_list
#line 1144 "syntactic.y"
        {
		statement.emplace<StatementLocal>(YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 1278 "syntactic.cpp"
    break;

  case 59: // statement: DIRECTIVE_LIST
#line 1148 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
#line 1287 "syntactic.cpp"
    break;

  case 60: // statement: DIRECTIVE_LIST "+"
#line 1153 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
#line 1296 "syntactic.cpp"
    break;

  case 61: // statement: DIRECTIVE_LIST "-"
#line 1158 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
#line 1305 "syntactic.cpp"
    break;

  case 62: // statement: DIRECTIVE_NOLIST
#line 1163 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
#line 1314 "syntactic.cpp"
    break;

  case 63: // statement: DIRECTIVE_PURGE IDENTIFIER
#line 1168 "syntactic.y"
        {
		statement.emplace<StatementPurge>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1322 "syntactic.cpp"
    break;

  case 64: // expression_list: expression
#line 1175 "syntactic.y"
        {
		yylhs.value.as < ExpressionList > ().emplace_back(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1330 "syntactic.cpp"
    break;

  case 65: // expression_list: expression_list "," expression
#line 1179 "syntactic.y"
        {
		yylhs.value.as < ExpressionList > () = YY_MOVE (yystack_[2].value.as < ExpressionList > ());

		yylhs.value.as < ExpressionList > ().emplace_back(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1340 "syntactic.cpp"
    break;

  case 66: // identifier_list: IDENTIFIER
#line 1188 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > ().emplace_back(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1348 "syntactic.cpp"
    break;

  case 67: // identifier_list: identifier_list "," IDENTIFIER
#line 1192 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > () = YY_MOVE (yystack_[2].value.as < IdentifierList > ());

		yylhs.value.as < IdentifierList > ().emplace_back(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1358 "syntactic.cpp"
    break;

  case 68: // option_list: OPTION
#line 1201 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > ().emplace_back(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1366 "syntactic.cpp"
    break;

  case 69: // option_list: option_list "," OPTION
#line 1205 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > () = YY_MOVE (yystack_[2].value.as < IdentifierList > ());

		yylhs.value.as < IdentifierList > ().emplace_back(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1376 "syntactic.cpp"
    break;

  case 70: // instruction: full_opcode
#line 1214 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = YY_MOVE (yystack_[0].value.as < Opcode > ());
		yylhs.value.as < StatementInstruction > ().operands[0].type = OPERAND_NONE;
		yylhs.value.as < StatementInstruction > ().operands[1].type = OPERAND_NONE;
	}
#line 1386 "syntactic.cpp"
    break;

  case 71: // instruction: full_opcode operand
#line 1220 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = YY_MOVE (yystack_[1].value.as < Opcode > ());
		yylhs.value.as < StatementInstruction > ().operands[0] = YY_MOVE (yystack_[0].value.as < Operand > ());
		yylhs.value.as < StatementInstruction > ().operands[1].type = OPERAND_NONE;
	}
#line 1396 "syntactic.cpp"
    break;

  case 72: // instruction: full_opcode operand "," operand
#line 1226 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = YY_MOVE (yystack_[3].value.as < Opcode > ());
		yylhs.value.as < StatementInstruction > ().operands[0] = YY_MOVE (yystack_[2].value.as < Operand > ());
		yylhs.value.as < StatementInstruction > ().operands[1] = YY_MOVE (yystack_[0].value.as < Operand > ());
	}
#line 1406 "syntactic.cpp"
    break;

  case 73: // full_opcode: opcode
#line 1235 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = YY_MOVE (yystack_[0].value.as < Opcode > ());
		yylhs.value.as < Opcode > ().size = SIZE_UNDEFINED;
	}
#line 1415 "syntactic.cpp"
    break;

  case 74: // full_opcode: opcode "."
#line 1240 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = YY_MOVE (yystack_[1].value.as < Opcode > ());
		yylhs.value.as < Opcode > ().size = SIZE_UNDEFINED;
		error_manager.WarningPedantic("Opcode has a dot but no size; either remove the dot or add an explicit size.");
	}
#line 1425 "syntactic.cpp"
    break;

  case 75: // full_opcode: opcode size
#line 1246 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = YY_MOVE (yystack_[1].value.as < Opcode > ());
		yylhs.value.as < Opcode > ().size = YY_MOVE (yystack_[0].value.as < Size > ());
	}
#line 1434 "syntactic.cpp"
    break;

  case 76: // opcode: DIRECTIVE_ORI
#line 1254 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ORI;
	}
#line 1442 "syntactic.cpp"
    break;

  case 77: // opcode: DIRECTIVE_ANDI
#line 1258 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ANDI;
	}
#line 1450 "syntactic.cpp"
    break;

  case 78: // opcode: DIRECTIVE_SUBI
#line 1262 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBI;
	}
#line 1458 "syntactic.cpp"
    break;

  case 79: // opcode: DIRECTIVE_ADDI
#line 1266 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDI;
	}
#line 1466 "syntactic.cpp"
    break;

  case 80: // opcode: DIRECTIVE_EORI
#line 1270 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EORI;
	}
#line 1474 "syntactic.cpp"
    break;

  case 81: // opcode: DIRECTIVE_CMPI
#line 1274 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPI;
	}
#line 1482 "syntactic.cpp"
    break;

  case 82: // opcode: DIRECTIVE_BTST
#line 1278 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BTST_STATIC;
	}
#line 1490 "syntactic.cpp"
    break;

  case 83: // opcode: DIRECTIVE_BCHG
#line 1282 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BCHG_STATIC;
	}
#line 1498 "syntactic.cpp"
    break;

  case 84: // opcode: DIRECTIVE_BCLR
#line 1286 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BCLR_STATIC;
	}
#line 1506 "syntactic.cpp"
    break;

  case 85: // opcode: DIRECTIVE_BSET
#line 1290 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BSET_STATIC;
	}
#line 1514 "syntactic.cpp"
    break;

  case 86: // opcode: DIRECTIVE_MOVEP
#line 1294 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEP_TO_REG;
	}
#line 1522 "syntactic.cpp"
    break;

  case 87: // opcode: DIRECTIVE_MOVEA
#line 1298 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEA;
	}
#line 1530 "syntactic.cpp"
    break;

  case 88: // opcode: DIRECTIVE_MOVE
#line 1302 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVE;
	}
#line 1538 "syntactic.cpp"
    break;

  case 89: // opcode: DIRECTIVE_NEGX
#line 1306 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NEGX;
	}
#line 1546 "syntactic.cpp"
    break;

  case 90: // opcode: DIRECTIVE_CLR
#line 1310 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CLR;
	}
#line 1554 "syntactic.cpp"
    break;

  case 91: // opcode: DIRECTIVE_NEG
#line 1314 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NEG;
	}
#line 1562 "syntactic.cpp"
    break;

  case 92: // opcode: DIRECTIVE_NOT
#line 1318 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NOT;
	}
#line 1570 "syntactic.cpp"
    break;

  case 93: // opcode: DIRECTIVE_EXT
#line 1322 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EXT;
	}
#line 1578 "syntactic.cpp"
    break;

  case 94: // opcode: DIRECTIVE_NBCD
#line 1326 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NBCD;
	}
#line 1586 "syntactic.cpp"
    break;

  case 95: // opcode: DIRECTIVE_SWAP
#line 1330 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SWAP;
	}
#line 1594 "syntactic.cpp"
    break;

  case 96: // opcode: DIRECTIVE_PEA
#line 1334 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_PEA;
	}
#line 1602 "syntactic.cpp"
    break;

  case 97: // opcode: DIRECTIVE_ILLEGAL
#line 1338 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ILLEGAL;
	}
#line 1610 "syntactic.cpp"
    break;

  case 98: // opcode: DIRECTIVE_TAS
#line 1342 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TAS;
	}
#line 1618 "syntactic.cpp"
    break;

  case 99: // opcode: DIRECTIVE_TST
#line 1346 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TST;
	}
#line 1626 "syntactic.cpp"
    break;

  case 100: // opcode: DIRECTIVE_TRAP
#line 1350 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TRAP;
	}
#line 1634 "syntactic.cpp"
    break;

  case 101: // opcode: DIRECTIVE_LINK
#line 1354 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LINK;
	}
#line 1642 "syntactic.cpp"
    break;

  case 102: // opcode: DIRECTIVE_UNLK
#line 1358 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_UNLK;
	}
#line 1650 "syntactic.cpp"
    break;

  case 103: // opcode: DIRECTIVE_RESET
#line 1362 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RESET;
	}
#line 1658 "syntactic.cpp"
    break;

  case 104: // opcode: DIRECTIVE_NOP
#line 1366 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NOP;
	}
#line 1666 "syntactic.cpp"
    break;

  case 105: // opcode: DIRECTIVE_STOP
#line 1370 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_STOP;
	}
#line 1674 "syntactic.cpp"
    break;

  case 106: // opcode: DIRECTIVE_RTE
#line 1374 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTE;
	}
#line 1682 "syntactic.cpp"
    break;

  case 107: // opcode: DIRECTIVE_RTS
#line 1378 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTS;
	}
#line 1690 "syntactic.cpp"
    break;

  case 108: // opcode: DIRECTIVE_TRAPV
#line 1382 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TRAPV;
	}
#line 1698 "syntactic.cpp"
    break;

  case 109: // opcode: DIRECTIVE_RTR
#line 1386 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTR;
	}
#line 1706 "syntactic.cpp"
    break;

  case 110: // opcode: DIRECTIVE_JSR
#line 1390 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_JSR;
	}
#line 1714 "syntactic.cpp"
    break;

  case 111: // opcode: DIRECTIVE_JMP
#line 1394 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_JMP;
	}
#line 1722 "syntactic.cpp"
    break;

  case 112: // opcode: DIRECTIVE_MOVEM
#line 1398 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEM_TO_REGS;
	}
#line 1730 "syntactic.cpp"
    break;

  case 113: // opcode: DIRECTIVE_LEA
#line 1402 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LEA;
	}
#line 1738 "syntactic.cpp"
    break;

  case 114: // opcode: DIRECTIVE_CHK
#line 1406 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CHK;
	}
#line 1746 "syntactic.cpp"
    break;

  case 115: // opcode: DIRECTIVE_ADDQ
#line 1410 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDQ;
	}
#line 1754 "syntactic.cpp"
    break;

  case 116: // opcode: DIRECTIVE_SUBQ
#line 1414 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBQ;
	}
#line 1762 "syntactic.cpp"
    break;

  case 117: // opcode: DIRECTIVE_ST
#line 1418 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_TRUE;
	}
#line 1771 "syntactic.cpp"
    break;

  case 118: // opcode: DIRECTIVE_SF
#line 1423 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_FALSE;
	}
#line 1780 "syntactic.cpp"
    break;

  case 119: // opcode: DIRECTIVE_SHI
#line 1428 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 1789 "syntactic.cpp"
    break;

  case 120: // opcode: DIRECTIVE_SLS
#line 1433 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 1798 "syntactic.cpp"
    break;

  case 121: // opcode: DIRECTIVE_SCC
#line 1438 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 1807 "syntactic.cpp"
    break;

  case 122: // opcode: DIRECTIVE_SCS
#line 1443 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 1816 "syntactic.cpp"
    break;

  case 123: // opcode: DIRECTIVE_SNE
#line 1448 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 1825 "syntactic.cpp"
    break;

  case 124: // opcode: DIRECTIVE_SEQ
#line 1453 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 1834 "syntactic.cpp"
    break;

  case 125: // opcode: DIRECTIVE_SVC
#line 1458 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 1843 "syntactic.cpp"
    break;

  case 126: // opcode: DIRECTIVE_SVS
#line 1463 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 1852 "syntactic.cpp"
    break;

  case 127: // opcode: DIRECTIVE_SPL
#line 1468 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 1861 "syntactic.cpp"
    break;

  case 128: // opcode: DIRECTIVE_SMI
#line 1473 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 1870 "syntactic.cpp"
    break;

  case 129: // opcode: DIRECTIVE_SGE
#line 1478 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 1879 "syntactic.cpp"
    break;

  case 130: // opcode: DIRECTIVE_SLT
#line 1483 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 1888 "syntactic.cpp"
    break;

  case 131: // opcode: DIRECTIVE_SGT
#line 1488 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 1897 "syntactic.cpp"
    break;

  case 132: // opcode: DIRECTIVE_SLE
#line 1493 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 1906 "syntactic.cpp"
    break;

  case 133: // opcode: DIRECTIVE_DBT
#line 1498 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_TRUE;
	}
#line 1915 "syntactic.cpp"
    break;

  case 134: // opcode: DIRECTIVE_DBF
#line 1503 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_FALSE;
	}
#line 1924 "syntactic.cpp"
    break;

  case 135: // opcode: DIRECTIVE_DBHI
#line 1508 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 1933 "syntactic.cpp"
    break;

  case 136: // opcode: DIRECTIVE_DBLS
#line 1513 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 1942 "syntactic.cpp"
    break;

  case 137: // opcode: DIRECTIVE_DBCC
#line 1518 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 1951 "syntactic.cpp"
    break;

  case 138: // opcode: DIRECTIVE_DBCS
#line 1523 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 1960 "syntactic.cpp"
    break;

  case 139: // opcode: DIRECTIVE_DBNE
#line 1528 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 1969 "syntactic.cpp"
    break;

  case 140: // opcode: DIRECTIVE_DBEQ
#line 1533 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 1978 "syntactic.cpp"
    break;

  case 141: // opcode: DIRECTIVE_DBVC
#line 1538 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 1987 "syntactic.cpp"
    break;

  case 142: // opcode: DIRECTIVE_DBVS
#line 1543 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 1996 "syntactic.cpp"
    break;

  case 143: // opcode: DIRECTIVE_DBPL
#line 1548 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 2005 "syntactic.cpp"
    break;

  case 144: // opcode: DIRECTIVE_DBMI
#line 1553 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 2014 "syntactic.cpp"
    break;

  case 145: // opcode: DIRECTIVE_DBGE
#line 1558 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2023 "syntactic.cpp"
    break;

  case 146: // opcode: DIRECTIVE_DBLT
#line 1563 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 2032 "syntactic.cpp"
    break;

  case 147: // opcode: DIRECTIVE_DBGT
#line 1568 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 2041 "syntactic.cpp"
    break;

  case 148: // opcode: DIRECTIVE_DBLE
#line 1573 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2050 "syntactic.cpp"
    break;

  case 149: // opcode: DIRECTIVE_BRA
#line 1578 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BRA;
	}
#line 2058 "syntactic.cpp"
    break;

  case 150: // opcode: DIRECTIVE_BSR
#line 1582 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BSR;
	}
#line 2066 "syntactic.cpp"
    break;

  case 151: // opcode: DIRECTIVE_BHI
#line 1586 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 2075 "syntactic.cpp"
    break;

  case 152: // opcode: DIRECTIVE_BLS
#line 1591 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 2084 "syntactic.cpp"
    break;

  case 153: // opcode: DIRECTIVE_BCC
#line 1596 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 2093 "syntactic.cpp"
    break;

  case 154: // opcode: DIRECTIVE_BCS
#line 1601 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 2102 "syntactic.cpp"
    break;

  case 155: // opcode: DIRECTIVE_BNE
#line 1606 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 2111 "syntactic.cpp"
    break;

  case 156: // opcode: DIRECTIVE_BEQ
#line 1611 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 2120 "syntactic.cpp"
    break;

  case 157: // opcode: DIRECTIVE_BVC
#line 1616 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 2129 "syntactic.cpp"
    break;

  case 158: // opcode: DIRECTIVE_BVS
#line 1621 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 2138 "syntactic.cpp"
    break;

  case 159: // opcode: DIRECTIVE_BPL
#line 1626 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 2147 "syntactic.cpp"
    break;

  case 160: // opcode: DIRECTIVE_BMI
#line 1631 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 2156 "syntactic.cpp"
    break;

  case 161: // opcode: DIRECTIVE_BGE
#line 1636 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2165 "syntactic.cpp"
    break;

  case 162: // opcode: DIRECTIVE_BLT
#line 1641 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 2174 "syntactic.cpp"
    break;

  case 163: // opcode: DIRECTIVE_BGT
#line 1646 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 2183 "syntactic.cpp"
    break;

  case 164: // opcode: DIRECTIVE_BLE
#line 1651 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2192 "syntactic.cpp"
    break;

  case 165: // opcode: DIRECTIVE_MOVEQ
#line 1656 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEQ;
	}
#line 2200 "syntactic.cpp"
    break;

  case 166: // opcode: DIRECTIVE_DIVU
#line 1660 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DIVU;
	}
#line 2208 "syntactic.cpp"
    break;

  case 167: // opcode: DIRECTIVE_DIVS
#line 1664 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DIVS;
	}
#line 2216 "syntactic.cpp"
    break;

  case 168: // opcode: DIRECTIVE_SBCD
#line 1668 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SBCD_DATA_REGS;
	}
#line 2224 "syntactic.cpp"
    break;

  case 169: // opcode: DIRECTIVE_OR
#line 1672 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_OR_TO_REG;
	}
#line 2232 "syntactic.cpp"
    break;

  case 170: // opcode: DIRECTIVE_SUB
#line 1676 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUB_TO_REG;
	}
#line 2240 "syntactic.cpp"
    break;

  case 171: // opcode: DIRECTIVE_SUBX
#line 1680 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBX_DATA_REGS;
	}
#line 2248 "syntactic.cpp"
    break;

  case 172: // opcode: DIRECTIVE_SUBA
#line 1684 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBA;
	}
#line 2256 "syntactic.cpp"
    break;

  case 173: // opcode: DIRECTIVE_EOR
#line 1688 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EOR;
	}
#line 2264 "syntactic.cpp"
    break;

  case 174: // opcode: DIRECTIVE_CMPM
#line 1692 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPM;
	}
#line 2272 "syntactic.cpp"
    break;

  case 175: // opcode: DIRECTIVE_CMP
#line 1696 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMP;
	}
#line 2280 "syntactic.cpp"
    break;

  case 176: // opcode: DIRECTIVE_CMPA
#line 1700 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPA;
	}
#line 2288 "syntactic.cpp"
    break;

  case 177: // opcode: DIRECTIVE_MULU
#line 1704 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MULU;
	}
#line 2296 "syntactic.cpp"
    break;

  case 178: // opcode: DIRECTIVE_MULS
#line 1708 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MULS;
	}
#line 2304 "syntactic.cpp"
    break;

  case 179: // opcode: DIRECTIVE_ABCD
#line 1712 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ABCD_DATA_REGS;
	}
#line 2312 "syntactic.cpp"
    break;

  case 180: // opcode: DIRECTIVE_EXG
#line 1716 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EXG;
	}
#line 2320 "syntactic.cpp"
    break;

  case 181: // opcode: DIRECTIVE_AND
#line 1720 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_AND_TO_REG;
	}
#line 2328 "syntactic.cpp"
    break;

  case 182: // opcode: DIRECTIVE_ADD
#line 1724 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADD_TO_REG;
	}
#line 2336 "syntactic.cpp"
    break;

  case 183: // opcode: DIRECTIVE_ADDX
#line 1728 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDX_DATA_REGS;
	}
#line 2344 "syntactic.cpp"
    break;

  case 184: // opcode: DIRECTIVE_ADDA
#line 1732 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDA;
	}
#line 2352 "syntactic.cpp"
    break;

  case 185: // opcode: DIRECTIVE_ASL
#line 1736 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ASL_STATIC;
	}
#line 2360 "syntactic.cpp"
    break;

  case 186: // opcode: DIRECTIVE_ASR
#line 1740 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ASR_STATIC;
	}
#line 2368 "syntactic.cpp"
    break;

  case 187: // opcode: DIRECTIVE_LSL
#line 1744 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LSL_STATIC;
	}
#line 2376 "syntactic.cpp"
    break;

  case 188: // opcode: DIRECTIVE_LSR
#line 1748 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LSR_STATIC;
	}
#line 2384 "syntactic.cpp"
    break;

  case 189: // opcode: DIRECTIVE_ROXL
#line 1752 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROXL_STATIC;
	}
#line 2392 "syntactic.cpp"
    break;

  case 190: // opcode: DIRECTIVE_ROXR
#line 1756 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROXR_STATIC;
	}
#line 2400 "syntactic.cpp"
    break;

  case 191: // opcode: DIRECTIVE_ROL
#line 1760 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROL_STATIC;
	}
#line 2408 "syntactic.cpp"
    break;

  case 192: // opcode: DIRECTIVE_ROR
#line 1764 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROR_STATIC;
	}
#line 2416 "syntactic.cpp"
    break;

  case 193: // size: SIZE_BYTE
#line 1771 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_BYTE;
	}
#line 2424 "syntactic.cpp"
    break;

  case 194: // size: SIZE_SHORT
#line 1775 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_SHORT;
	}
#line 2432 "syntactic.cpp"
    break;

  case 195: // size: SIZE_WORD
#line 1779 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_WORD;
	}
#line 2440 "syntactic.cpp"
    break;

  case 196: // size: SIZE_LONGWORD
#line 1783 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_LONGWORD;
	}
#line 2448 "syntactic.cpp"
    break;

  case 197: // operand: "(" ADDRESS_REGISTER ")"
#line 1791 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT;
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2457 "syntactic.cpp"
    break;

  case 198: // operand: "(" ADDRESS_REGISTER ")" "+"
#line 1796 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());
	}
#line 2466 "syntactic.cpp"
    break;

  case 199: // operand: "-" "(" ADDRESS_REGISTER ")"
#line 1801 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2475 "syntactic.cpp"
    break;

  case 200: // operand: expression "(" ADDRESS_REGISTER ")"
#line 1806 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2485 "syntactic.cpp"
    break;

  case 201: // operand: "(" expression "," ADDRESS_REGISTER ")"
#line 1812 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2495 "syntactic.cpp"
    break;

  case 202: // operand: "(" ADDRESS_REGISTER "," expression ")"
#line 1818 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[1].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[3].value.as < unsigned long > ());
	}
#line 2505 "syntactic.cpp"
    break;

  case 203: // operand: "(" ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1824 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[4].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2520 "syntactic.cpp"
    break;

  case 204: // operand: expression "(" ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1835 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[6].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[4].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2535 "syntactic.cpp"
    break;

  case 205: // operand: "(" expression "," ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1846 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[6].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[4].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2550 "syntactic.cpp"
    break;

  case 206: // operand: "(" ADDRESS_REGISTER "," data_or_address_register ")"
#line 1857 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[3].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2566 "syntactic.cpp"
    break;

  case 207: // operand: expression "(" ADDRESS_REGISTER "," data_or_address_register ")"
#line 1869 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[5].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[3].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2582 "syntactic.cpp"
    break;

  case 208: // operand: "(" expression "," ADDRESS_REGISTER "," data_or_address_register ")"
#line 1881 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[5].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[3].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2598 "syntactic.cpp"
    break;

  case 209: // operand: expression "(" PROGRAM_COUNTER ")"
#line 1893 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
	}
#line 2607 "syntactic.cpp"
    break;

  case 210: // operand: "(" expression "," PROGRAM_COUNTER ")"
#line 1898 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
	}
#line 2616 "syntactic.cpp"
    break;

  case 211: // operand: "(" PROGRAM_COUNTER "," expression ")"
#line 1903 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[1].value.as < Expression > ());
	}
#line 2625 "syntactic.cpp"
    break;

  case 212: // operand: "(" PROGRAM_COUNTER "," data_or_address_register size ")"
#line 1908 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2639 "syntactic.cpp"
    break;

  case 213: // operand: expression "(" PROGRAM_COUNTER "," data_or_address_register size ")"
#line 1918 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[6].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2653 "syntactic.cpp"
    break;

  case 214: // operand: "(" expression "," PROGRAM_COUNTER "," data_or_address_register size ")"
#line 1928 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[6].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2667 "syntactic.cpp"
    break;

  case 215: // operand: "(" PROGRAM_COUNTER "," data_or_address_register ")"
#line 1938 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2682 "syntactic.cpp"
    break;

  case 216: // operand: expression "(" PROGRAM_COUNTER "," data_or_address_register ")"
#line 1949 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[5].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2697 "syntactic.cpp"
    break;

  case 217: // operand: "(" expression "," PROGRAM_COUNTER "," data_or_address_register ")"
#line 1960 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[5].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2712 "syntactic.cpp"
    break;

  case 218: // operand: "#" expression
#line 1972 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_LITERAL;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2721 "syntactic.cpp"
    break;

  case 219: // operand: STATUS_REGISTER
#line 1988 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_STATUS_REGISTER;
	}
#line 2729 "syntactic.cpp"
    break;

  case 220: // operand: CONDITION_CODE_REGISTER
#line 1992 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_CONDITION_CODE_REGISTER;
	}
#line 2737 "syntactic.cpp"
    break;

  case 221: // operand: USER_STACK_POINTER_REGISTER
#line 1996 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_USER_STACK_POINTER_REGISTER;
	}
#line 2745 "syntactic.cpp"
    break;

  case 222: // operand: expression
#line 2001 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[0].value.as < Expression > ());
		yylhs.value.as < Operand > ().size = SIZE_UNDEFINED;
	}
#line 2755 "syntactic.cpp"
    break;

  case 223: // operand: expression size
#line 2007 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_ABSOLUTE;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[1].value.as < Expression > ());
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[0].value.as < Size > ());
	}
#line 2765 "syntactic.cpp"
    break;

  case 224: // operand: register_list
#line 2014 "syntactic.y"
        {
		const auto register_list = YY_MOVE (yystack_[0].value.as < unsigned long > ());

		/* Neat trick to check if only one bit is set (yes, I know that it evaluates to true for 0, but that doesn't matter here). */
		if ((register_list & (register_list - 1)) == 0)
		{
			/* This is a single register. */
			yylhs.value.as < Operand > ().main_register = 0;

			while ((register_list & (1 << yylhs.value.as < Operand > ().main_register)) == 0)
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
			yylhs.value.as < Operand > ().main_register = register_list; /* Such a hack... */
		}
	}
#line 2799 "syntactic.cpp"
    break;

  case 225: // register_list: register_span
#line 2047 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2807 "syntactic.cpp"
    break;

  case 226: // register_list: register_span "/" register_list
#line 2051 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = YY_MOVE (yystack_[2].value.as < unsigned long > ()) | YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2815 "syntactic.cpp"
    break;

  case 227: // register_span: data_or_address_register
#line 2058 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 1 << YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2823 "syntactic.cpp"
    break;

  case 228: // register_span: data_or_address_register "-" data_or_address_register
#line 2062 "syntactic.y"
        {
		const auto first_data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());
		const auto second_data_or_address_register = YY_MOVE (yystack_[0].value.as < unsigned long > ());

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

		yylhs.value.as < unsigned long > () = 0;

		for (unsigned long i = start; i <= end; ++i)
			yylhs.value.as < unsigned long > () |= 1 << i;
	}
#line 2851 "syntactic.cpp"
    break;

  case 229: // data_or_address_register: DATA_REGISTER
#line 2089 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 0 + YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2859 "syntactic.cpp"
    break;

  case 230: // data_or_address_register: ADDRESS_REGISTER
#line 2093 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 8 + YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2867 "syntactic.cpp"
    break;

  case 231: // expression: expression1
#line 2106 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2875 "syntactic.cpp"
    break;

  case 232: // expression: expression LOGICAL_AND expression1
#line 2111 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LOGICAL_AND, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2883 "syntactic.cpp"
    break;

  case 233: // expression: expression LOGICAL_OR expression1
#line 2116 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LOGICAL_OR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2891 "syntactic.cpp"
    break;

  case 234: // expression1: expression2
#line 2123 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2899 "syntactic.cpp"
    break;

  case 235: // expression1: expression1 "=" expression2
#line 2127 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::EQUALITY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2907 "syntactic.cpp"
    break;

  case 236: // expression1: expression1 EQUALITY expression2
#line 2131 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::EQUALITY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2915 "syntactic.cpp"
    break;

  case 237: // expression1: expression1 INEQUALITY expression2
#line 2135 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::INEQUALITY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2923 "syntactic.cpp"
    break;

  case 238: // expression2: expression3
#line 2142 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2931 "syntactic.cpp"
    break;

  case 239: // expression2: expression2 "<" expression3
#line 2146 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LESS_THAN, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2939 "syntactic.cpp"
    break;

  case 240: // expression2: expression2 LESS_OR_EQUAL expression3
#line 2150 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LESS_OR_EQUAL, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2947 "syntactic.cpp"
    break;

  case 241: // expression2: expression2 ">" expression3
#line 2154 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MORE_THAN, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2955 "syntactic.cpp"
    break;

  case 242: // expression2: expression2 MORE_OR_EQUAL expression3
#line 2158 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MORE_OR_EQUAL, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2963 "syntactic.cpp"
    break;

  case 243: // expression3: expression4
#line 2165 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2971 "syntactic.cpp"
    break;

  case 244: // expression3: expression3 "+" expression4
#line 2169 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::ADD, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2979 "syntactic.cpp"
    break;

  case 245: // expression3: expression3 "-" expression4
#line 2173 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::SUBTRACT, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2987 "syntactic.cpp"
    break;

  case 246: // expression4: expression5
#line 2180 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2995 "syntactic.cpp"
    break;

  case 247: // expression4: expression4 "*" expression5
#line 2184 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MULTIPLY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3003 "syntactic.cpp"
    break;

  case 248: // expression4: expression4 "/" expression5
#line 2188 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::DIVIDE, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3011 "syntactic.cpp"
    break;

  case 249: // expression4: expression4 "%" expression5
#line 2192 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MODULO, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3019 "syntactic.cpp"
    break;

  case 250: // expression5: expression6
#line 2199 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3027 "syntactic.cpp"
    break;

  case 251: // expression5: expression5 "&" expression6
#line 2203 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_AND, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3035 "syntactic.cpp"
    break;

  case 252: // expression5: expression5 "!" expression6
#line 2207 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_OR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3043 "syntactic.cpp"
    break;

  case 253: // expression5: expression5 "|" expression6
#line 2211 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_OR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3051 "syntactic.cpp"
    break;

  case 254: // expression5: expression5 "^" expression6
#line 2215 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_XOR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3059 "syntactic.cpp"
    break;

  case 255: // expression6: expression7
#line 2222 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3067 "syntactic.cpp"
    break;

  case 256: // expression6: expression6 LEFT_SHIFT expression7
#line 2226 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LEFT_SHIFT, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3075 "syntactic.cpp"
    break;

  case 257: // expression6: expression6 RIGHT_SHIFT expression7
#line 2230 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::RIGHT_SHIFT, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3083 "syntactic.cpp"
    break;

  case 258: // expression7: expression8
#line 2237 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3091 "syntactic.cpp"
    break;

  case 259: // expression7: "+" expression7
#line 2241 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3099 "syntactic.cpp"
    break;

  case 260: // expression7: "-" expression7
#line 2245 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::NEGATE, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3107 "syntactic.cpp"
    break;

  case 261: // expression7: "~" expression7
#line 2249 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::BITWISE_NOT, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3115 "syntactic.cpp"
    break;

  case 262: // expression7: "!" expression7
#line 2254 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::LOGICAL_NOT, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3123 "syntactic.cpp"
    break;

  case 263: // local_identifier: LOCAL_IDENTIFIER
#line 2260 "syntactic.y"
          { yylhs.value.as < std::string > () = YY_MOVE (yystack_[0].value.as < std::string > ()); }
#line 3129 "syntactic.cpp"
    break;

  case 264: // local_identifier: SIZE_BYTE
#line 2262 "syntactic.y"
        {
		yylhs.value.as < std::string > () = ".b";
	}
#line 3137 "syntactic.cpp"
    break;

  case 265: // local_identifier: SIZE_SHORT
#line 2266 "syntactic.y"
        {
		yylhs.value.as < std::string > () = ".s";
	}
#line 3145 "syntactic.cpp"
    break;

  case 266: // local_identifier: SIZE_WORD
#line 2270 "syntactic.y"
        {
		yylhs.value.as < std::string > () = ".w";
	}
#line 3153 "syntactic.cpp"
    break;

  case 267: // local_identifier: SIZE_LONGWORD
#line 2274 "syntactic.y"
        {
		yylhs.value.as < std::string > () = ".l";
	}
#line 3161 "syntactic.cpp"
    break;

  case 268: // identifier: IDENTIFIER
#line 2280 "syntactic.y"
          { yylhs.value.as < std::string > () = YY_MOVE (yystack_[0].value.as < std::string > ()); }
#line 3167 "syntactic.cpp"
    break;

  case 269: // identifier: local_identifier
#line 2281 "syntactic.y"
          { yylhs.value.as < std::string > () = YY_MOVE (yystack_[0].value.as < std::string > ()); }
#line 3173 "syntactic.cpp"
    break;

  case 270: // identifier: identifier local_identifier
#line 2283 "syntactic.y"
        {
		yylhs.value.as < std::string > () = YY_MOVE (yystack_[1].value.as < std::string > ()) + YY_MOVE (yystack_[0].value.as < std::string > ());
	}
#line 3181 "syntactic.cpp"
    break;

  case 271: // expression8: NUMBER
#line 2290 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionNumber>(YY_MOVE (yystack_[0].value.as < unsigned long > ()));
	}
#line 3189 "syntactic.cpp"
    break;

  case 272: // expression8: identifier
#line 2294 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionIdentifier>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 3197 "syntactic.cpp"
    break;

  case 273: // expression8: string
#line 2298 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3205 "syntactic.cpp"
    break;

  case 274: // expression8: "*"
#line 2302 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionProgramCounterOfStatement>();
	}
#line 3213 "syntactic.cpp"
    break;

  case 275: // expression8: "@"
#line 2306 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionProgramCounterOfExpression>();
	}
#line 3221 "syntactic.cpp"
    break;

  case 276: // expression8: RS
#line 2310 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionRs>();
	}
#line 3229 "syntactic.cpp"
    break;

  case 277: // expression8: NARG
#line 2314 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionNarg>();
	}
#line 3237 "syntactic.cpp"
    break;

  case 278: // expression8: "(" expression ")"
#line 2318 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[1].value.as < Expression > ());
	}
#line 3245 "syntactic.cpp"
    break;

  case 279: // expression8: STRLEN "(" STRING ")"
#line 2322 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionStrlen>(YY_MOVE (yystack_[1].value.as < std::string > ()));
	}
#line 3253 "syntactic.cpp"
    break;

  case 280: // expression8: STRCMP "(" STRING "," STRING ")"
#line 2326 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionStrcmp>(YY_MOVE (yystack_[3].value.as < std::string > ()), YY_MOVE (yystack_[1].value.as < std::string > ()));
	}
#line 3261 "syntactic.cpp"
    break;

  case 281: // expression8: INSTR "(" expression "," expression ")"
#line 2330 "syntactic.y"
        {
		auto subexpressions = std::make_unique<Expression[]>(3);

		subexpressions[0] = ExpressionNumber{1};
		subexpressions[1] = YY_MOVE (yystack_[3].value.as < Expression > ());
		subexpressions[2] = YY_MOVE (yystack_[1].value.as < Expression > ());

		yylhs.value.as < Expression > ().emplace<ExpressionInstr>(std::move(subexpressions));
	}
#line 3275 "syntactic.cpp"
    break;

  case 282: // expression8: INSTR "(" expression "," expression "," expression ")"
#line 2340 "syntactic.y"
        {
		auto subexpressions = std::make_unique<Expression[]>(3);

		subexpressions[0] = YY_MOVE (yystack_[5].value.as < Expression > ());
		subexpressions[1] = YY_MOVE (yystack_[3].value.as < Expression > ());
		subexpressions[2] = YY_MOVE (yystack_[1].value.as < Expression > ());

		yylhs.value.as < Expression > ().emplace<ExpressionInstr>(std::move(subexpressions));
	}
#line 3289 "syntactic.cpp"
    break;

  case 283: // expression8: DEF "(" IDENTIFIER ")"
#line 2350 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionDef>(YY_MOVE (yystack_[1].value.as < std::string > ()));
	}
#line 3297 "syntactic.cpp"
    break;

  case 284: // expression8: TYPE "(" IDENTIFIER ")"
#line 2354 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionTypeWithIdentifier>(YY_MOVE (yystack_[1].value.as < std::string > ()));
	}
#line 3305 "syntactic.cpp"
    break;

  case 285: // expression8: TYPE "(" NUMBER ")"
#line 2358 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionTypeWithNumber>(YY_MOVE (yystack_[1].value.as < unsigned long > ()));
	}
#line 3313 "syntactic.cpp"
    break;

  case 286: // expression8: FILESIZE "(" STRING ")"
#line 2362 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionFilesize>(YY_MOVE (yystack_[1].value.as < std::string > ()));
	}
#line 3321 "syntactic.cpp"
    break;

  case 287: // expression8: OFFSET "(" expression ")"
#line 2366 "syntactic.y"
        {
		/* TODO: Make this actually do something besides pass the expression through. */
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[1].value.as < Expression > ());
	}
#line 3330 "syntactic.cpp"
    break;

  case 288: // string: STRING
#line 2374 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionString>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 3338 "syntactic.cpp"
    break;


#line 3342 "syntactic.cpp"

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


  const short parser::yypact_ninf_ = -309;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     296,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,     5,     5,
       5,   579,  -309,  -153,  -144,  -309,  -152,   -76,   579,  -125,
     344,   579,   579,   579,  -309,  -309,   579,  -309,  -309,   579,
     579,   579,  -309,  -309,     5,   579,   579,   579,  -309,   579,
    -309,  -309,   -90,   -68,   -70,  -309,  -309,  -144,  -167,  -309,
     -42,   579,   108,  -309,   -49,   -71,  -309,  -309,  -309,  -309,
     579,   579,   579,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,   -64,   -50,   -48,    -1,     8,    11,    30,
     579,   579,   579,  -309,  -309,   579,   579,  -118,  -173,  -131,
    -113,   -36,    73,   -72,  -309,  -309,     9,  -309,  -309,  -309,
      25,    33,    33,  -309,    35,  -118,  -309,  -309,   391,   -93,
    -118,  -118,  -118,  -118,    65,  -118,    72,   579,  -118,  -118,
    -118,  -118,  -309,    42,  -309,  -309,    33,  -309,  -309,  -309,
    -118,  -309,  -309,  -309,  -309,  -309,  -309,    23,   626,   579,
      44,  -309,    39,    61,    70,  -309,  -309,    56,  -118,    76,
    -118,    93,    99,   579,   103,    -6,   105,   579,   -24,  -309,
    -309,  -309,  -309,   579,   579,   579,   579,   579,   579,   579,
     579,   579,   579,   579,   579,   579,   579,   579,   579,   579,
     579,   579,   579,  -309,  -144,   124,   438,   287,    78,   485,
     579,   306,  -118,   102,  -124,   283,   -89,   532,  -118,   -49,
      51,    51,  -156,  -309,   579,   579,   282,   285,    80,   284,
     294,   295,   298,   -17,  -309,  -173,  -173,  -131,  -131,  -131,
    -113,  -113,  -113,  -113,   -36,   -36,    73,    73,    73,   -72,
     -72,   -72,   -72,  -309,  -309,    33,  -309,   579,    91,  -309,
     323,   329,   100,  -118,   307,  -309,   297,   304,   297,  -150,
     314,  -309,  -309,  -309,   -67,   -62,  -118,  -118,  -309,   333,
     579,  -309,  -309,  -309,  -309,  -309,  -118,   579,  -309,  -309,
     345,   579,  -127,     6,  -309,  -123,    24,   -23,     3,  -309,
      51,  -309,    51,  -309,   320,   -20,  -118,  -309,    56,  -309,
     321,  -309,  -309,   322,  -309,    51,  -309,    51,  -309,  -107,
    -100,  -309,   579,  -309,  -309,  -309,   -96,   -87,  -309,   324,
    -309,   325,    28,  -309,   326,  -309,   327,  -309,  -309,  -309,
    -309,  -309
  };

  const short
  parser::yydefact_[] =
  {
       2,    76,    77,    78,    79,    80,    81,    82,    83,    84,
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
     185,   186,   187,   188,   189,   190,   191,   192,     0,     0,
       0,     0,    13,    14,    18,    20,     0,     0,     0,     0,
       0,     0,     0,    32,    33,    34,     0,    36,    37,     0,
       0,     0,    42,    43,     0,     0,    47,     0,    49,     0,
      51,    52,     0,     0,     0,    56,    57,     0,    59,    62,
       0,     0,     0,     3,    70,    73,   193,   194,   195,   196,
       0,     0,     0,   264,   265,   266,   267,   271,   268,   263,
     288,   276,   277,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   274,   275,     0,     0,    12,   231,   234,
     238,   243,   246,   250,   255,   269,   272,   258,   273,    66,
      15,    16,    19,     7,     8,    21,    23,    22,     0,     0,
      28,    30,    31,    35,     0,    39,     0,     0,    45,    46,
      48,    50,    68,    53,    54,    55,    58,    60,    61,    63,
      29,     1,   229,   230,   219,   220,   221,     0,     0,     0,
      71,   224,   225,   227,   222,    74,    75,     4,    64,     0,
       6,     0,     0,     0,     0,     0,     0,     0,     0,   259,
     260,   262,   261,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   270,     0,     0,     0,     0,     0,     0,
       0,     0,    44,     0,     0,     0,     0,     0,   218,     0,
       0,     0,     0,   223,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   278,   232,   233,   236,   237,   235,
     240,   242,   239,   241,   244,   245,   247,   248,   249,   251,
     254,   252,   253,   256,   257,    17,    67,     0,     9,    24,
       0,     0,     0,    38,    40,    69,     0,   197,     0,     0,
       0,    72,   226,   228,     0,     0,    65,     5,   279,     0,
       0,   283,   285,   284,   286,   287,    10,     0,    26,    25,
       0,     0,     0,     0,   198,     0,     0,     0,     0,   199,
       0,   200,     0,   209,     0,     0,    11,    27,    41,   206,
       0,   202,   215,     0,   211,     0,   201,     0,   210,     0,
       0,   280,     0,   281,   203,   212,     0,     0,   207,     0,
     216,     0,     0,   208,     0,   217,     0,   204,   213,   282,
     205,   214
  };

  const short
  parser::yypgoto_[] =
  {
    -309,  -309,   140,  -122,  -309,  -309,  -309,  -309,  -119,   217,
     218,  -309,  -308,  -117,    15,   -30,   192,    10,     0,   187,
    -186,   336,  -309,  -309,  -309
  };

  const short
  parser::yydefgoto_[] =
  {
       0,   162,   257,   211,   233,   163,   164,   165,   170,   250,
     251,   252,   253,   254,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208
  };

  const short
  parser::yytable_[] =
  {
     171,   172,   212,   363,   197,   269,   270,   275,   276,   271,
     272,   215,   364,   219,   220,   221,   222,   209,   387,   223,
     213,   365,   224,   225,   226,   227,   209,   388,   228,   229,
     230,   277,   231,   237,   238,   236,   166,   167,   168,   169,
     166,   167,   168,   169,   240,   216,   256,   217,   382,   210,
     385,   278,   279,   258,   259,   260,   166,   167,   168,   169,
     273,   274,   270,   166,   167,   168,   169,   166,   167,   168,
     169,   399,   356,   268,   357,   402,   166,   167,   168,   169,
     280,   281,   409,   232,   410,   273,   274,   282,   283,   273,
     274,   418,   166,   167,   168,   169,   214,   416,   420,   417,
     235,   298,   423,   299,   234,   343,   344,   359,   241,   324,
     302,   425,   291,   292,   173,   174,   175,   176,   242,   243,
     177,   178,   179,   180,   255,   244,   245,   246,   239,   390,
     306,   391,   308,   261,   392,   313,   393,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   318,   262,   247,   263,
     323,   191,   248,   193,   273,   274,   194,   249,   273,   274,
     195,   273,   274,   320,   321,   196,   284,   285,   166,   167,
     168,   169,   345,   405,   324,   406,   412,   286,   413,   348,
     179,   375,   352,   353,   273,   274,   173,   174,   175,   176,
     268,   304,   177,   178,   179,   180,   264,   366,   367,   407,
     305,   408,   273,   274,   401,   265,   273,   274,   266,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   242,   243,
     190,   294,   404,   191,   192,   193,   429,   267,   194,   295,
     376,   296,   195,   166,   167,   168,   169,   196,   303,   383,
     309,   386,   310,   273,   274,   327,   328,   329,   273,   274,
     273,   274,   314,   395,   273,   274,   273,   274,   273,   274,
     396,   300,   311,   400,   258,   316,   403,   312,   301,   273,
     274,   317,   315,   319,   350,   355,   370,   322,   273,   274,
     287,   288,   289,   290,   336,   337,   338,   377,   325,   326,
     419,   421,   334,   335,   346,   422,   380,   424,   426,     1,
       2,     3,     4,     5,     6,     7,     8,     9,    10,    11,
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
     152,   153,   154,   155,   156,   157,   158,   159,   160,   349,
     173,   174,   175,   176,   242,   243,   177,   178,   179,   180,
     330,   331,   332,   333,   339,   340,   341,   342,   354,   358,
     368,   369,   371,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   372,   373,   190,   378,   374,   191,   192,   193,
     161,   379,   194,   381,   384,   394,   195,   173,   174,   175,
     176,   196,   389,   177,   178,   179,   180,   397,   411,   414,
     415,   398,   427,   428,   430,   431,   361,     0,   362,     0,
     181,   182,   183,   184,   185,   186,   187,   188,   189,     0,
     218,   190,   293,     0,   191,   192,   193,     0,     0,   194,
       0,     0,     0,   195,   173,   174,   175,   176,   196,     0,
     177,   178,   179,   180,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   181,   182,   183,
     184,   185,   186,   187,   188,   189,     0,   297,   190,     0,
       0,   191,   192,   193,     0,     0,   194,     0,     0,     0,
     195,   173,   174,   175,   176,   196,     0,   177,   178,   179,
     180,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   181,   182,   183,   184,   185,   186,
     187,   188,   189,     0,   347,   190,     0,     0,   191,   192,
     193,     0,     0,   194,     0,     0,     0,   195,   173,   174,
     175,   176,   196,     0,   177,   178,   179,   180,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   181,   182,   183,   184,   185,   186,   187,   188,   189,
       0,   351,   190,     0,     0,   191,   192,   193,     0,     0,
     194,     0,     0,     0,   195,   173,   174,   175,   176,   196,
     360,   177,   178,   179,   180,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   181,   182,
     183,   184,   185,   186,   187,   188,   189,     0,     0,   190,
       0,     0,   191,   192,   193,     0,     0,   194,     0,     0,
       0,   195,   173,   174,   175,   176,   196,     0,   177,   178,
     179,   180,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   181,   182,   183,   184,   185,
     186,   187,   188,   189,     0,     0,   190,     0,     0,   191,
     192,   193,     0,     0,   194,     0,     0,     0,   195,   173,
     174,   175,   176,   196,     0,   177,   178,   179,   180,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   181,   182,   183,   184,   185,   186,   187,   188,
     189,     0,     0,   307,     0,     0,   191,   192,   193,     0,
       0,   194,     0,     0,     0,   195,     0,     0,     0,     0,
     196
  };

  const short
  parser::yycheck_[] =
  {
     119,   120,   124,   311,   121,   191,   192,   180,   181,   195,
     196,   128,   168,   130,   131,   132,   133,   170,   168,   136,
     172,   177,   139,   140,   141,   144,   170,   177,   145,   146,
     147,   204,   149,   200,   201,   157,   163,   164,   165,   166,
     163,   164,   165,   166,   161,   170,   165,   172,   356,   202,
     358,   182,   183,   170,   171,   172,   163,   164,   165,   166,
     178,   179,   248,   163,   164,   165,   166,   163,   164,   165,
     166,   198,   196,   190,   198,   198,   163,   164,   165,   166,
     211,   212,   390,   173,   392,   178,   179,   200,   201,   178,
     179,   198,   163,   164,   165,   166,   172,   405,   198,   407,
     170,   218,   198,   196,   172,   291,   292,   196,     0,   198,
     227,   198,   184,   185,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   195,   174,   175,   176,   170,   196,
     247,   198,   249,   197,   196,   254,   198,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   263,   197,   197,   197,
     267,   200,   201,   202,   178,   179,   205,   206,   178,   179,
     209,   178,   179,   169,   170,   214,   202,   203,   163,   164,
     165,   166,   294,   196,   198,   198,   196,   213,   198,   296,
     171,   198,   299,   300,   178,   179,   163,   164,   165,   166,
     307,   168,   169,   170,   171,   172,   197,   314,   315,   196,
     177,   198,   178,   179,   198,   197,   178,   179,   197,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   167,   168,
     197,   196,   198,   200,   201,   202,   198,   197,   205,   196,
     347,   196,   209,   163,   164,   165,   166,   214,   196,   356,
     196,   358,   203,   178,   179,   275,   276,   277,   178,   179,
     178,   179,   196,   370,   178,   179,   178,   179,   178,   179,
     377,   196,   201,   382,   381,   172,   385,   197,   196,   178,
     179,   172,   196,   170,   196,   173,   196,   172,   178,   179,
     207,   208,   209,   210,   284,   285,   286,   196,   273,   274,
     409,   410,   282,   283,   170,   412,   196,   416,   417,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   172,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     278,   279,   280,   281,   287,   288,   289,   290,   172,   196,
     198,   196,   198,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   198,   198,   197,   172,   198,   200,   201,   202,
     204,   172,   205,   196,   200,   172,   209,   163,   164,   165,
     166,   214,   198,   169,   170,   171,   172,   172,   198,   198,
     198,   381,   198,   198,   198,   198,   309,    -1,   310,    -1,
     186,   187,   188,   189,   190,   191,   192,   193,   194,    -1,
     196,   197,   206,    -1,   200,   201,   202,    -1,    -1,   205,
      -1,    -1,    -1,   209,   163,   164,   165,   166,   214,    -1,
     169,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,   188,
     189,   190,   191,   192,   193,   194,    -1,   196,   197,    -1,
      -1,   200,   201,   202,    -1,    -1,   205,    -1,    -1,    -1,
     209,   163,   164,   165,   166,   214,    -1,   169,   170,   171,
     172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   186,   187,   188,   189,   190,   191,
     192,   193,   194,    -1,   196,   197,    -1,    -1,   200,   201,
     202,    -1,    -1,   205,    -1,    -1,    -1,   209,   163,   164,
     165,   166,   214,    -1,   169,   170,   171,   172,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   186,   187,   188,   189,   190,   191,   192,   193,   194,
      -1,   196,   197,    -1,    -1,   200,   201,   202,    -1,    -1,
     205,    -1,    -1,    -1,   209,   163,   164,   165,   166,   214,
     168,   169,   170,   171,   172,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    -1,    -1,   197,
      -1,    -1,   200,   201,   202,    -1,    -1,   205,    -1,    -1,
      -1,   209,   163,   164,   165,   166,   214,    -1,   169,   170,
     171,   172,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   186,   187,   188,   189,   190,
     191,   192,   193,   194,    -1,    -1,   197,    -1,    -1,   200,
     201,   202,    -1,    -1,   205,    -1,    -1,    -1,   209,   163,
     164,   165,   166,   214,    -1,   169,   170,   171,   172,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   186,   187,   188,   189,   190,   191,   192,   193,
     194,    -1,    -1,   197,    -1,    -1,   200,   201,   202,    -1,
      -1,   205,    -1,    -1,    -1,   209,    -1,    -1,    -1,    -1,
     214
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
     162,   204,   218,   222,   223,   224,   163,   164,   165,   166,
     225,   225,   225,   163,   164,   165,   166,   169,   170,   171,
     172,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     197,   200,   201,   202,   205,   209,   214,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   241,   170,
     202,   220,   220,   172,   172,   230,   170,   172,   196,   230,
     230,   230,   230,   230,   230,   230,   230,   225,   230,   230,
     230,   230,   173,   221,   172,   170,   220,   200,   201,   170,
     230,     0,   167,   168,   174,   175,   176,   197,   201,   206,
     226,   227,   228,   229,   230,   195,   225,   219,   230,   230,
     230,   197,   197,   197,   197,   197,   197,   197,   230,   237,
     237,   237,   237,   178,   179,   180,   181,   204,   182,   183,
     211,   212,   200,   201,   202,   203,   213,   207,   208,   209,
     210,   184,   185,   238,   196,   196,   196,   196,   230,   196,
     196,   196,   230,   196,   168,   177,   230,   197,   230,   196,
     203,   201,   197,   225,   196,   196,   172,   172,   230,   170,
     169,   170,   172,   230,   198,   231,   231,   232,   232,   232,
     233,   233,   233,   233,   234,   234,   235,   235,   235,   236,
     236,   236,   236,   237,   237,   220,   170,   196,   230,   172,
     196,   196,   230,   230,   172,   173,   196,   198,   196,   196,
     168,   226,   227,   229,   168,   177,   230,   230,   198,   196,
     196,   198,   198,   198,   198,   198,   230,   196,   172,   172,
     196,   196,   229,   230,   200,   229,   230,   168,   177,   198,
     196,   198,   196,   198,   172,   230,   230,   172,   219,   198,
     225,   198,   198,   225,   198,   196,   198,   196,   198,   229,
     229,   198,   196,   198,   198,   198,   229,   229,   198,   225,
     198,   225,   230,   198,   225,   198,   225,   198,   198,   198,
     198,   198
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   217,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   218,   218,   218,   218,   218,   218,
     218,   218,   218,   218,   219,   219,   220,   220,   221,   221,
     222,   222,   222,   223,   223,   223,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     224,   224,   224,   225,   225,   225,   225,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   226,   226,   226,   226,   226,
     226,   226,   226,   226,   226,   227,   227,   228,   228,   229,
     229,   230,   230,   230,   231,   231,   231,   231,   232,   232,
     232,   232,   232,   233,   233,   233,   234,   234,   234,   234,
     235,   235,   235,   235,   235,   236,   236,   236,   237,   237,
     237,   237,   237,   238,   238,   238,   238,   238,   239,   239,
     239,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   241
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     0,     1,     3,     5,     3,     2,     2,     4,
       5,     6,     2,     1,     1,     2,     2,     4,     1,     2,
       1,     2,     2,     2,     4,     5,     5,     6,     2,     2,
       2,     2,     1,     1,     1,     2,     1,     1,     4,     2,
       4,     6,     1,     1,     3,     2,     2,     1,     2,     1,
       2,     1,     1,     2,     2,     2,     1,     1,     2,     1,
       2,     2,     1,     2,     1,     3,     1,     3,     1,     3,
       1,     2,     4,     1,     2,     2,     1,     1,     1,     1,
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
       1,     1,     1,     1,     1,     1,     1,     3,     4,     4,
       4,     5,     5,     6,     7,     8,     5,     6,     7,     4,
       5,     5,     6,     7,     8,     5,     6,     7,     2,     1,
       1,     1,     1,     2,     1,     1,     3,     1,     3,     1,
       1,     1,     3,     3,     1,     3,     3,     3,     1,     3,
       3,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     2,
       2,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     3,     4,
       6,     6,     8,     4,     4,     4,     4,     4,     1
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
  "DIRECTIVE_ALIGN", "DIRECTIVE_INFORM", "DIRECTIVE_FAIL", "DIRECTIVE_END",
  "DIRECTIVE_RS", "DIRECTIVE_RSSET", "DIRECTIVE_RSRESET", "DIRECTIVE_OBJ",
  "DIRECTIVE_OBJEND", "DIRECTIVE_ORG", "DIRECTIVE_PUSHO", "DIRECTIVE_POPO",
  "DIRECTIVE_OPT", "DIRECTIVE_PUSHP", "DIRECTIVE_POPP", "DIRECTIVE_SHIFT",
  "DIRECTIVE_MEXIT", "DIRECTIVE_LOCAL", "DIRECTIVE_LIST",
  "DIRECTIVE_NOLIST", "DIRECTIVE_PURGE", "SIZE_BYTE", "SIZE_SHORT",
  "SIZE_WORD", "SIZE_LONGWORD", "DATA_REGISTER", "ADDRESS_REGISTER",
  "NUMBER", "IDENTIFIER", "LOCAL_IDENTIFIER", "STRING", "OPTION",
  "STATUS_REGISTER", "CONDITION_CODE_REGISTER",
  "USER_STACK_POINTER_REGISTER", "PROGRAM_COUNTER", "LOGICAL_AND",
  "LOGICAL_OR", "EQUALITY", "INEQUALITY", "LESS_OR_EQUAL", "MORE_OR_EQUAL",
  "LEFT_SHIFT", "RIGHT_SHIFT", "RS", "NARG", "STRLEN", "STRCMP", "INSTR",
  "DEF", "TYPE", "FILESIZE", "OFFSET", "\".\"", "\",\"", "\"(\"", "\")\"",
  "\"$\"", "\"+\"", "\"-\"", "\"*\"", "\"/\"", "\"=\"", "\"@\"", "\"#\"",
  "\"&\"", "\"^\"", "\"!\"", "\"|\"", "\"<\"", "\">\"", "\"%\"", "\"~\"",
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
       0,   913,   913,   916,   920,   924,   928,   932,   936,   940,
     944,   948,   952,   956,   960,   964,   968,   972,   976,   980,
     984,   988,   992,   996,  1000,  1006,  1012,  1016,  1020,  1024,
    1028,  1032,  1036,  1040,  1044,  1048,  1052,  1056,  1060,  1064,
    1068,  1072,  1077,  1081,  1085,  1089,  1093,  1099,  1103,  1107,
    1111,  1115,  1119,  1123,  1127,  1131,  1135,  1139,  1143,  1147,
    1152,  1157,  1162,  1167,  1174,  1178,  1187,  1191,  1200,  1204,
    1213,  1219,  1225,  1234,  1239,  1245,  1253,  1257,  1261,  1265,
    1269,  1273,  1277,  1281,  1285,  1289,  1293,  1297,  1301,  1305,
    1309,  1313,  1317,  1321,  1325,  1329,  1333,  1337,  1341,  1345,
    1349,  1353,  1357,  1361,  1365,  1369,  1373,  1377,  1381,  1385,
    1389,  1393,  1397,  1401,  1405,  1409,  1413,  1417,  1422,  1427,
    1432,  1437,  1442,  1447,  1452,  1457,  1462,  1467,  1472,  1477,
    1482,  1487,  1492,  1497,  1502,  1507,  1512,  1517,  1522,  1527,
    1532,  1537,  1542,  1547,  1552,  1557,  1562,  1567,  1572,  1577,
    1581,  1585,  1590,  1595,  1600,  1605,  1610,  1615,  1620,  1625,
    1630,  1635,  1640,  1645,  1650,  1655,  1659,  1663,  1667,  1671,
    1675,  1679,  1683,  1687,  1691,  1695,  1699,  1703,  1707,  1711,
    1715,  1719,  1723,  1727,  1731,  1735,  1739,  1743,  1747,  1751,
    1755,  1759,  1763,  1770,  1774,  1778,  1782,  1790,  1795,  1800,
    1805,  1811,  1817,  1823,  1834,  1845,  1856,  1868,  1880,  1892,
    1897,  1902,  1907,  1917,  1927,  1937,  1948,  1959,  1971,  1987,
    1991,  1995,  2000,  2006,  2013,  2046,  2050,  2057,  2061,  2088,
    2092,  2105,  2110,  2115,  2122,  2126,  2130,  2134,  2141,  2145,
    2149,  2153,  2157,  2164,  2168,  2172,  2179,  2183,  2187,  2191,
    2198,  2202,  2206,  2210,  2214,  2221,  2225,  2229,  2236,  2240,
    2244,  2248,  2253,  2260,  2261,  2265,  2269,  2273,  2280,  2281,
    2282,  2289,  2293,  2297,  2301,  2305,  2309,  2313,  2317,  2321,
    2325,  2329,  2339,  2349,  2353,  2357,  2361,  2365,  2373
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
#line 4239 "syntactic.cpp"

