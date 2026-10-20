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
#line 912 "syntactic.y"
        {
		statement.emplace<StatementEmpty>();
	}
#line 823 "syntactic.cpp"
    break;

  case 3: // statement: instruction
#line 916 "syntactic.y"
        {
		statement.emplace<StatementInstruction>(YY_MOVE (yystack_[0].value.as < StatementInstruction > ()));
	}
#line 831 "syntactic.cpp"
    break;

  case 4: // statement: DIRECTIVE_DC size expression_list
#line 920 "syntactic.y"
        {
		statement.emplace<StatementDc>(YY_MOVE (yystack_[1].value.as < Size > ()), YY_MOVE (yystack_[0].value.as < ExpressionList > ()));
	}
#line 839 "syntactic.cpp"
    break;

  case 5: // statement: DIRECTIVE_DCB size expression "," expression
#line 924 "syntactic.y"
        {
		statement.emplace<StatementDcb>(YY_MOVE (yystack_[3].value.as < Size > ()), YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 847 "syntactic.cpp"
    break;

  case 6: // statement: DIRECTIVE_DS size expression
#line 928 "syntactic.y"
        {
		statement.emplace<StatementDs>(YY_MOVE (yystack_[1].value.as < Size > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 855 "syntactic.cpp"
    break;

  case 7: // statement: DIRECTIVE_INCLUDE STRING
#line 932 "syntactic.y"
        {
		statement.emplace<StatementInclude>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 863 "syntactic.cpp"
    break;

  case 8: // statement: DIRECTIVE_INCBIN STRING
#line 936 "syntactic.y"
        {		
		statement.emplace<StatementIncbin>(YY_MOVE (yystack_[0].value.as < std::string > ()), ExpressionNumber{0}, false);
	}
#line 871 "syntactic.cpp"
    break;

  case 9: // statement: DIRECTIVE_INCBIN STRING "," expression
#line 940 "syntactic.y"
        {
		statement.emplace<StatementIncbin>(YY_MOVE (yystack_[2].value.as < std::string > ()), YY_MOVE (yystack_[0].value.as < Expression > ()), false);
	}
#line 879 "syntactic.cpp"
    break;

  case 10: // statement: DIRECTIVE_INCBIN STRING "," "," expression
#line 944 "syntactic.y"
        {
		statement.emplace<StatementIncbin>(YY_MOVE (yystack_[3].value.as < std::string > ()), ExpressionNumber{0}, true, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 887 "syntactic.cpp"
    break;

  case 11: // statement: DIRECTIVE_INCBIN STRING "," expression "," expression
#line 948 "syntactic.y"
        {
		statement.emplace<StatementIncbin>(YY_MOVE (yystack_[4].value.as < std::string > ()), YY_MOVE (yystack_[2].value.as < Expression > ()), true, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 895 "syntactic.cpp"
    break;

  case 12: // statement: DIRECTIVE_REPT expression
#line 952 "syntactic.y"
        {
		statement.emplace<StatementRept>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 903 "syntactic.cpp"
    break;

  case 13: // statement: DIRECTIVE_ENDR
#line 956 "syntactic.y"
        {
		statement.emplace<StatementEndr>();
	}
#line 911 "syntactic.cpp"
    break;

  case 14: // statement: DIRECTIVE_MACRO
#line 960 "syntactic.y"
        {
		statement.emplace<StatementMacro>(false);
	}
#line 919 "syntactic.cpp"
    break;

  case 15: // statement: DIRECTIVE_MACRO "*"
#line 964 "syntactic.y"
        {
		statement.emplace<StatementMacro>(true);
	}
#line 927 "syntactic.cpp"
    break;

  case 16: // statement: DIRECTIVE_MACRO identifier_list
#line 968 "syntactic.y"
        {
		statement.emplace<StatementMacro>(false, YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 935 "syntactic.cpp"
    break;

  case 17: // statement: DIRECTIVE_MACRO "*" "," identifier_list
#line 972 "syntactic.y"
        {
		statement.emplace<StatementMacro>(true, YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 943 "syntactic.cpp"
    break;

  case 18: // statement: DIRECTIVE_MACROS
#line 976 "syntactic.y"
        {
		statement.emplace<StatementMacros>(StatementMacro{false, {}});
	}
#line 951 "syntactic.cpp"
    break;

  case 19: // statement: DIRECTIVE_MACROS identifier_list
#line 980 "syntactic.y"
        {
		statement.emplace<StatementMacros>(StatementMacro{false, YY_MOVE (yystack_[0].value.as < IdentifierList > ())});
	}
#line 959 "syntactic.cpp"
    break;

  case 20: // statement: DIRECTIVE_ENDM
#line 984 "syntactic.y"
        {
		statement.emplace<StatementEndm>();
	}
#line 967 "syntactic.cpp"
    break;

  case 21: // statement: DIRECTIVE_EQU expression
#line 988 "syntactic.y"
        {
		statement.emplace<StatementEqu>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 975 "syntactic.cpp"
    break;

  case 22: // statement: DIRECTIVE_EQUS STRING
#line 992 "syntactic.y"
        {
		statement.emplace<StatementEqusString>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 983 "syntactic.cpp"
    break;

  case 23: // statement: DIRECTIVE_EQUS IDENTIFIER
#line 996 "syntactic.y"
        {
		statement.emplace<StatementEqusIdentifier>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 991 "syntactic.cpp"
    break;

  case 24: // statement: DIRECTIVE_SUBSTR "," "," STRING
#line 1000 "syntactic.y"
        {
		std::string string = YY_MOVE (yystack_[0].value.as < std::string > ());
		const auto string_length = string.length();
		statement.emplace<StatementSubstr>(ExpressionNumber{1}, ExpressionNumber{string_length}, std::move(string));
	}
#line 1001 "syntactic.cpp"
    break;

  case 25: // statement: DIRECTIVE_SUBSTR expression "," "," STRING
#line 1006 "syntactic.y"
        {
		std::string string = YY_MOVE (yystack_[0].value.as < std::string > ());
		const auto string_length = string.length();
		statement.emplace<StatementSubstr>(YY_MOVE (yystack_[3].value.as < Expression > ()), ExpressionNumber{string_length}, std::move(string));
	}
#line 1011 "syntactic.cpp"
    break;

  case 26: // statement: DIRECTIVE_SUBSTR "," expression "," STRING
#line 1012 "syntactic.y"
        {
		statement.emplace<StatementSubstr>(ExpressionNumber{1}, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1019 "syntactic.cpp"
    break;

  case 27: // statement: DIRECTIVE_SUBSTR expression "," expression "," STRING
#line 1016 "syntactic.y"
        {
		statement.emplace<StatementSubstr>(YY_MOVE (yystack_[4].value.as < Expression > ()), YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1027 "syntactic.cpp"
    break;

  case 28: // statement: DIRECTIVE_SET expression
#line 1020 "syntactic.y"
        {
		statement.emplace<StatementSet>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1035 "syntactic.cpp"
    break;

  case 29: // statement: "=" expression
#line 1024 "syntactic.y"
        {
		statement.emplace<StatementSet>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1043 "syntactic.cpp"
    break;

  case 30: // statement: DIRECTIVE_IF expression
#line 1028 "syntactic.y"
        {
		statement.emplace<StatementIf>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1051 "syntactic.cpp"
    break;

  case 31: // statement: DIRECTIVE_ELSEIF expression
#line 1032 "syntactic.y"
        {
		statement.emplace<StatementElseif>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1059 "syntactic.cpp"
    break;

  case 32: // statement: DIRECTIVE_ELSEIF
#line 1036 "syntactic.y"
        {
		statement.emplace<StatementElse>();
	}
#line 1067 "syntactic.cpp"
    break;

  case 33: // statement: DIRECTIVE_ELSE
#line 1040 "syntactic.y"
        {
		statement.emplace<StatementElse>();
	}
#line 1075 "syntactic.cpp"
    break;

  case 34: // statement: DIRECTIVE_ENDC
#line 1044 "syntactic.y"
        {
		statement.emplace<StatementEndc>();
	}
#line 1083 "syntactic.cpp"
    break;

  case 35: // statement: DIRECTIVE_WHILE expression
#line 1048 "syntactic.y"
        {
		statement.emplace<StatementWhile>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1091 "syntactic.cpp"
    break;

  case 36: // statement: DIRECTIVE_ENDW
#line 1052 "syntactic.y"
        {
		statement.emplace<StatementEndw>();
	}
#line 1099 "syntactic.cpp"
    break;

  case 37: // statement: DIRECTIVE_EVEN
#line 1056 "syntactic.y"
        {
		statement.emplace<StatementEven>();
	}
#line 1107 "syntactic.cpp"
    break;

  case 38: // statement: DIRECTIVE_CNOP expression "," expression
#line 1060 "syntactic.y"
        {
		statement.emplace<StatementCnop>(YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1115 "syntactic.cpp"
    break;

  case 39: // statement: DIRECTIVE_INFORM expression "," STRING
#line 1064 "syntactic.y"
        {
		statement.emplace<StatementInform>(YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1123 "syntactic.cpp"
    break;

  case 40: // statement: DIRECTIVE_INFORM expression "," STRING "," expression_list
#line 1068 "syntactic.y"
        {
		/* TODO - parameters */
		statement.emplace<StatementInform>(YY_MOVE (yystack_[4].value.as < Expression > ()), YY_MOVE (yystack_[2].value.as < std::string > ()));
	}
#line 1132 "syntactic.cpp"
    break;

  case 41: // statement: DIRECTIVE_FAIL
#line 1073 "syntactic.y"
        {
		statement.emplace<StatementFail>();
	}
#line 1140 "syntactic.cpp"
    break;

  case 42: // statement: DIRECTIVE_END
#line 1077 "syntactic.y"
        {
		statement.emplace<StatementEnd>();
	}
#line 1148 "syntactic.cpp"
    break;

  case 43: // statement: DIRECTIVE_RS size expression
#line 1081 "syntactic.y"
        {
		statement.emplace<StatementRs>(YY_MOVE (yystack_[1].value.as < Size > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1156 "syntactic.cpp"
    break;

  case 44: // statement: DIRECTIVE_RSSET expression
#line 1085 "syntactic.y"
        {
		statement.emplace<StatementRsset>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1164 "syntactic.cpp"
    break;

  case 45: // statement: DIRECTIVE_RSRESET expression
#line 1089 "syntactic.y"
        {
		/* As mentioned in S.N. 68k's manual, this odd hack
		   is necessary for compatibility with other assemblers. */
		statement.emplace<StatementRsset>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1174 "syntactic.cpp"
    break;

  case 46: // statement: DIRECTIVE_RSRESET
#line 1095 "syntactic.y"
        {
		statement.emplace<StatementRsreset>();
	}
#line 1182 "syntactic.cpp"
    break;

  case 47: // statement: DIRECTIVE_OBJ expression
#line 1099 "syntactic.y"
        {
		statement.emplace<StatementObj>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1190 "syntactic.cpp"
    break;

  case 48: // statement: DIRECTIVE_OBJEND
#line 1103 "syntactic.y"
        {
		statement.emplace<StatementObjend>();
	}
#line 1198 "syntactic.cpp"
    break;

  case 49: // statement: DIRECTIVE_ORG expression
#line 1107 "syntactic.y"
        {
		statement.emplace<StatementOrg>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1206 "syntactic.cpp"
    break;

  case 50: // statement: DIRECTIVE_PUSHO
#line 1111 "syntactic.y"
        {
		statement.emplace<StatementPusho>();
	}
#line 1214 "syntactic.cpp"
    break;

  case 51: // statement: DIRECTIVE_POPO
#line 1115 "syntactic.y"
        {
		statement.emplace<StatementPopo>();
	}
#line 1222 "syntactic.cpp"
    break;

  case 52: // statement: DIRECTIVE_OPT option_list
#line 1119 "syntactic.y"
        {
		statement.emplace<StatementOpt>(YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 1230 "syntactic.cpp"
    break;

  case 53: // statement: DIRECTIVE_PUSHP STRING
#line 1123 "syntactic.y"
        {
		statement.emplace<StatementPushp>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1238 "syntactic.cpp"
    break;

  case 54: // statement: DIRECTIVE_POPP IDENTIFIER
#line 1127 "syntactic.y"
        {
		statement.emplace<StatementPopp>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1246 "syntactic.cpp"
    break;

  case 55: // statement: DIRECTIVE_SHIFT
#line 1131 "syntactic.y"
        {
		statement.emplace<StatementShift>();
	}
#line 1254 "syntactic.cpp"
    break;

  case 56: // statement: DIRECTIVE_MEXIT
#line 1135 "syntactic.y"
        {
		statement.emplace<StatementMexit>();
	}
#line 1262 "syntactic.cpp"
    break;

  case 57: // statement: DIRECTIVE_LOCAL identifier_list
#line 1139 "syntactic.y"
        {
		statement.emplace<StatementLocal>(YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 1270 "syntactic.cpp"
    break;

  case 58: // statement: DIRECTIVE_LIST
#line 1143 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
#line 1279 "syntactic.cpp"
    break;

  case 59: // statement: DIRECTIVE_LIST "+"
#line 1148 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
#line 1288 "syntactic.cpp"
    break;

  case 60: // statement: DIRECTIVE_LIST "-"
#line 1153 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
#line 1297 "syntactic.cpp"
    break;

  case 61: // statement: DIRECTIVE_NOLIST
#line 1158 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement.emplace<StatementEmpty>();
	}
#line 1306 "syntactic.cpp"
    break;

  case 62: // statement: DIRECTIVE_PURGE IDENTIFIER
#line 1163 "syntactic.y"
        {
		statement.emplace<StatementPurge>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1314 "syntactic.cpp"
    break;

  case 63: // expression_list: expression
#line 1170 "syntactic.y"
        {
		yylhs.value.as < ExpressionList > ().emplace_back(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1322 "syntactic.cpp"
    break;

  case 64: // expression_list: expression_list "," expression
#line 1174 "syntactic.y"
        {
		yylhs.value.as < ExpressionList > () = YY_MOVE (yystack_[2].value.as < ExpressionList > ());

		yylhs.value.as < ExpressionList > ().emplace_back(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1332 "syntactic.cpp"
    break;

  case 65: // identifier_list: IDENTIFIER
#line 1183 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > ().emplace_back(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1340 "syntactic.cpp"
    break;

  case 66: // identifier_list: identifier_list "," IDENTIFIER
#line 1187 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > () = YY_MOVE (yystack_[2].value.as < IdentifierList > ());

		yylhs.value.as < IdentifierList > ().emplace_back(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1350 "syntactic.cpp"
    break;

  case 67: // option_list: OPTION
#line 1196 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > ().emplace_back(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1358 "syntactic.cpp"
    break;

  case 68: // option_list: option_list "," OPTION
#line 1200 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > () = YY_MOVE (yystack_[2].value.as < IdentifierList > ());

		yylhs.value.as < IdentifierList > ().emplace_back(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 1368 "syntactic.cpp"
    break;

  case 69: // instruction: full_opcode
#line 1209 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = YY_MOVE (yystack_[0].value.as < Opcode > ());
		yylhs.value.as < StatementInstruction > ().operands[0].type = OPERAND_NONE;
		yylhs.value.as < StatementInstruction > ().operands[1].type = OPERAND_NONE;
	}
#line 1378 "syntactic.cpp"
    break;

  case 70: // instruction: full_opcode operand
#line 1215 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = YY_MOVE (yystack_[1].value.as < Opcode > ());
		yylhs.value.as < StatementInstruction > ().operands[0] = YY_MOVE (yystack_[0].value.as < Operand > ());
		yylhs.value.as < StatementInstruction > ().operands[1].type = OPERAND_NONE;
	}
#line 1388 "syntactic.cpp"
    break;

  case 71: // instruction: full_opcode operand "," operand
#line 1221 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = YY_MOVE (yystack_[3].value.as < Opcode > ());
		yylhs.value.as < StatementInstruction > ().operands[0] = YY_MOVE (yystack_[2].value.as < Operand > ());
		yylhs.value.as < StatementInstruction > ().operands[1] = YY_MOVE (yystack_[0].value.as < Operand > ());
	}
#line 1398 "syntactic.cpp"
    break;

  case 72: // full_opcode: opcode
#line 1230 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = YY_MOVE (yystack_[0].value.as < Opcode > ());
		yylhs.value.as < Opcode > ().size = SIZE_UNDEFINED;
	}
#line 1407 "syntactic.cpp"
    break;

  case 73: // full_opcode: opcode "."
#line 1235 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = YY_MOVE (yystack_[1].value.as < Opcode > ());
		yylhs.value.as < Opcode > ().size = SIZE_UNDEFINED;
		error_manager.WarningPedantic("Opcode has a dot but no size; either remove the dot or add an explicit size.");
	}
#line 1417 "syntactic.cpp"
    break;

  case 74: // full_opcode: opcode size
#line 1241 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = YY_MOVE (yystack_[1].value.as < Opcode > ());
		yylhs.value.as < Opcode > ().size = YY_MOVE (yystack_[0].value.as < Size > ());
	}
#line 1426 "syntactic.cpp"
    break;

  case 75: // opcode: DIRECTIVE_ORI
#line 1249 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ORI;
	}
#line 1434 "syntactic.cpp"
    break;

  case 76: // opcode: DIRECTIVE_ANDI
#line 1253 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ANDI;
	}
#line 1442 "syntactic.cpp"
    break;

  case 77: // opcode: DIRECTIVE_SUBI
#line 1257 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBI;
	}
#line 1450 "syntactic.cpp"
    break;

  case 78: // opcode: DIRECTIVE_ADDI
#line 1261 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDI;
	}
#line 1458 "syntactic.cpp"
    break;

  case 79: // opcode: DIRECTIVE_EORI
#line 1265 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EORI;
	}
#line 1466 "syntactic.cpp"
    break;

  case 80: // opcode: DIRECTIVE_CMPI
#line 1269 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPI;
	}
#line 1474 "syntactic.cpp"
    break;

  case 81: // opcode: DIRECTIVE_BTST
#line 1273 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BTST_STATIC;
	}
#line 1482 "syntactic.cpp"
    break;

  case 82: // opcode: DIRECTIVE_BCHG
#line 1277 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BCHG_STATIC;
	}
#line 1490 "syntactic.cpp"
    break;

  case 83: // opcode: DIRECTIVE_BCLR
#line 1281 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BCLR_STATIC;
	}
#line 1498 "syntactic.cpp"
    break;

  case 84: // opcode: DIRECTIVE_BSET
#line 1285 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BSET_STATIC;
	}
#line 1506 "syntactic.cpp"
    break;

  case 85: // opcode: DIRECTIVE_MOVEP
#line 1289 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEP_TO_REG;
	}
#line 1514 "syntactic.cpp"
    break;

  case 86: // opcode: DIRECTIVE_MOVEA
#line 1293 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEA;
	}
#line 1522 "syntactic.cpp"
    break;

  case 87: // opcode: DIRECTIVE_MOVE
#line 1297 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVE;
	}
#line 1530 "syntactic.cpp"
    break;

  case 88: // opcode: DIRECTIVE_NEGX
#line 1301 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NEGX;
	}
#line 1538 "syntactic.cpp"
    break;

  case 89: // opcode: DIRECTIVE_CLR
#line 1305 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CLR;
	}
#line 1546 "syntactic.cpp"
    break;

  case 90: // opcode: DIRECTIVE_NEG
#line 1309 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NEG;
	}
#line 1554 "syntactic.cpp"
    break;

  case 91: // opcode: DIRECTIVE_NOT
#line 1313 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NOT;
	}
#line 1562 "syntactic.cpp"
    break;

  case 92: // opcode: DIRECTIVE_EXT
#line 1317 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EXT;
	}
#line 1570 "syntactic.cpp"
    break;

  case 93: // opcode: DIRECTIVE_NBCD
#line 1321 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NBCD;
	}
#line 1578 "syntactic.cpp"
    break;

  case 94: // opcode: DIRECTIVE_SWAP
#line 1325 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SWAP;
	}
#line 1586 "syntactic.cpp"
    break;

  case 95: // opcode: DIRECTIVE_PEA
#line 1329 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_PEA;
	}
#line 1594 "syntactic.cpp"
    break;

  case 96: // opcode: DIRECTIVE_ILLEGAL
#line 1333 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ILLEGAL;
	}
#line 1602 "syntactic.cpp"
    break;

  case 97: // opcode: DIRECTIVE_TAS
#line 1337 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TAS;
	}
#line 1610 "syntactic.cpp"
    break;

  case 98: // opcode: DIRECTIVE_TST
#line 1341 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TST;
	}
#line 1618 "syntactic.cpp"
    break;

  case 99: // opcode: DIRECTIVE_TRAP
#line 1345 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TRAP;
	}
#line 1626 "syntactic.cpp"
    break;

  case 100: // opcode: DIRECTIVE_LINK
#line 1349 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LINK;
	}
#line 1634 "syntactic.cpp"
    break;

  case 101: // opcode: DIRECTIVE_UNLK
#line 1353 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_UNLK;
	}
#line 1642 "syntactic.cpp"
    break;

  case 102: // opcode: DIRECTIVE_RESET
#line 1357 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RESET;
	}
#line 1650 "syntactic.cpp"
    break;

  case 103: // opcode: DIRECTIVE_NOP
#line 1361 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NOP;
	}
#line 1658 "syntactic.cpp"
    break;

  case 104: // opcode: DIRECTIVE_STOP
#line 1365 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_STOP;
	}
#line 1666 "syntactic.cpp"
    break;

  case 105: // opcode: DIRECTIVE_RTE
#line 1369 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTE;
	}
#line 1674 "syntactic.cpp"
    break;

  case 106: // opcode: DIRECTIVE_RTS
#line 1373 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTS;
	}
#line 1682 "syntactic.cpp"
    break;

  case 107: // opcode: DIRECTIVE_TRAPV
#line 1377 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TRAPV;
	}
#line 1690 "syntactic.cpp"
    break;

  case 108: // opcode: DIRECTIVE_RTR
#line 1381 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTR;
	}
#line 1698 "syntactic.cpp"
    break;

  case 109: // opcode: DIRECTIVE_JSR
#line 1385 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_JSR;
	}
#line 1706 "syntactic.cpp"
    break;

  case 110: // opcode: DIRECTIVE_JMP
#line 1389 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_JMP;
	}
#line 1714 "syntactic.cpp"
    break;

  case 111: // opcode: DIRECTIVE_MOVEM
#line 1393 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEM_TO_REGS;
	}
#line 1722 "syntactic.cpp"
    break;

  case 112: // opcode: DIRECTIVE_LEA
#line 1397 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LEA;
	}
#line 1730 "syntactic.cpp"
    break;

  case 113: // opcode: DIRECTIVE_CHK
#line 1401 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CHK;
	}
#line 1738 "syntactic.cpp"
    break;

  case 114: // opcode: DIRECTIVE_ADDQ
#line 1405 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDQ;
	}
#line 1746 "syntactic.cpp"
    break;

  case 115: // opcode: DIRECTIVE_SUBQ
#line 1409 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBQ;
	}
#line 1754 "syntactic.cpp"
    break;

  case 116: // opcode: DIRECTIVE_ST
#line 1413 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_TRUE;
	}
#line 1763 "syntactic.cpp"
    break;

  case 117: // opcode: DIRECTIVE_SF
#line 1418 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_FALSE;
	}
#line 1772 "syntactic.cpp"
    break;

  case 118: // opcode: DIRECTIVE_SHI
#line 1423 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 1781 "syntactic.cpp"
    break;

  case 119: // opcode: DIRECTIVE_SLS
#line 1428 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 1790 "syntactic.cpp"
    break;

  case 120: // opcode: DIRECTIVE_SCC
#line 1433 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 1799 "syntactic.cpp"
    break;

  case 121: // opcode: DIRECTIVE_SCS
#line 1438 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 1808 "syntactic.cpp"
    break;

  case 122: // opcode: DIRECTIVE_SNE
#line 1443 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 1817 "syntactic.cpp"
    break;

  case 123: // opcode: DIRECTIVE_SEQ
#line 1448 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 1826 "syntactic.cpp"
    break;

  case 124: // opcode: DIRECTIVE_SVC
#line 1453 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 1835 "syntactic.cpp"
    break;

  case 125: // opcode: DIRECTIVE_SVS
#line 1458 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 1844 "syntactic.cpp"
    break;

  case 126: // opcode: DIRECTIVE_SPL
#line 1463 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 1853 "syntactic.cpp"
    break;

  case 127: // opcode: DIRECTIVE_SMI
#line 1468 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 1862 "syntactic.cpp"
    break;

  case 128: // opcode: DIRECTIVE_SGE
#line 1473 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 1871 "syntactic.cpp"
    break;

  case 129: // opcode: DIRECTIVE_SLT
#line 1478 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 1880 "syntactic.cpp"
    break;

  case 130: // opcode: DIRECTIVE_SGT
#line 1483 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 1889 "syntactic.cpp"
    break;

  case 131: // opcode: DIRECTIVE_SLE
#line 1488 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 1898 "syntactic.cpp"
    break;

  case 132: // opcode: DIRECTIVE_DBT
#line 1493 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_TRUE;
	}
#line 1907 "syntactic.cpp"
    break;

  case 133: // opcode: DIRECTIVE_DBF
#line 1498 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_FALSE;
	}
#line 1916 "syntactic.cpp"
    break;

  case 134: // opcode: DIRECTIVE_DBHI
#line 1503 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 1925 "syntactic.cpp"
    break;

  case 135: // opcode: DIRECTIVE_DBLS
#line 1508 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 1934 "syntactic.cpp"
    break;

  case 136: // opcode: DIRECTIVE_DBCC
#line 1513 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 1943 "syntactic.cpp"
    break;

  case 137: // opcode: DIRECTIVE_DBCS
#line 1518 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 1952 "syntactic.cpp"
    break;

  case 138: // opcode: DIRECTIVE_DBNE
#line 1523 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 1961 "syntactic.cpp"
    break;

  case 139: // opcode: DIRECTIVE_DBEQ
#line 1528 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 1970 "syntactic.cpp"
    break;

  case 140: // opcode: DIRECTIVE_DBVC
#line 1533 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 1979 "syntactic.cpp"
    break;

  case 141: // opcode: DIRECTIVE_DBVS
#line 1538 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 1988 "syntactic.cpp"
    break;

  case 142: // opcode: DIRECTIVE_DBPL
#line 1543 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 1997 "syntactic.cpp"
    break;

  case 143: // opcode: DIRECTIVE_DBMI
#line 1548 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 2006 "syntactic.cpp"
    break;

  case 144: // opcode: DIRECTIVE_DBGE
#line 1553 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2015 "syntactic.cpp"
    break;

  case 145: // opcode: DIRECTIVE_DBLT
#line 1558 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 2024 "syntactic.cpp"
    break;

  case 146: // opcode: DIRECTIVE_DBGT
#line 1563 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 2033 "syntactic.cpp"
    break;

  case 147: // opcode: DIRECTIVE_DBLE
#line 1568 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2042 "syntactic.cpp"
    break;

  case 148: // opcode: DIRECTIVE_BRA
#line 1573 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BRA;
	}
#line 2050 "syntactic.cpp"
    break;

  case 149: // opcode: DIRECTIVE_BSR
#line 1577 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BSR;
	}
#line 2058 "syntactic.cpp"
    break;

  case 150: // opcode: DIRECTIVE_BHI
#line 1581 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 2067 "syntactic.cpp"
    break;

  case 151: // opcode: DIRECTIVE_BLS
#line 1586 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 2076 "syntactic.cpp"
    break;

  case 152: // opcode: DIRECTIVE_BCC
#line 1591 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 2085 "syntactic.cpp"
    break;

  case 153: // opcode: DIRECTIVE_BCS
#line 1596 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 2094 "syntactic.cpp"
    break;

  case 154: // opcode: DIRECTIVE_BNE
#line 1601 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 2103 "syntactic.cpp"
    break;

  case 155: // opcode: DIRECTIVE_BEQ
#line 1606 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 2112 "syntactic.cpp"
    break;

  case 156: // opcode: DIRECTIVE_BVC
#line 1611 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 2121 "syntactic.cpp"
    break;

  case 157: // opcode: DIRECTIVE_BVS
#line 1616 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 2130 "syntactic.cpp"
    break;

  case 158: // opcode: DIRECTIVE_BPL
#line 1621 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 2139 "syntactic.cpp"
    break;

  case 159: // opcode: DIRECTIVE_BMI
#line 1626 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 2148 "syntactic.cpp"
    break;

  case 160: // opcode: DIRECTIVE_BGE
#line 1631 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2157 "syntactic.cpp"
    break;

  case 161: // opcode: DIRECTIVE_BLT
#line 1636 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 2166 "syntactic.cpp"
    break;

  case 162: // opcode: DIRECTIVE_BGT
#line 1641 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 2175 "syntactic.cpp"
    break;

  case 163: // opcode: DIRECTIVE_BLE
#line 1646 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2184 "syntactic.cpp"
    break;

  case 164: // opcode: DIRECTIVE_MOVEQ
#line 1651 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEQ;
	}
#line 2192 "syntactic.cpp"
    break;

  case 165: // opcode: DIRECTIVE_DIVU
#line 1655 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DIVU;
	}
#line 2200 "syntactic.cpp"
    break;

  case 166: // opcode: DIRECTIVE_DIVS
#line 1659 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DIVS;
	}
#line 2208 "syntactic.cpp"
    break;

  case 167: // opcode: DIRECTIVE_SBCD
#line 1663 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SBCD_DATA_REGS;
	}
#line 2216 "syntactic.cpp"
    break;

  case 168: // opcode: DIRECTIVE_OR
#line 1667 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_OR_TO_REG;
	}
#line 2224 "syntactic.cpp"
    break;

  case 169: // opcode: DIRECTIVE_SUB
#line 1671 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUB_TO_REG;
	}
#line 2232 "syntactic.cpp"
    break;

  case 170: // opcode: DIRECTIVE_SUBX
#line 1675 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBX_DATA_REGS;
	}
#line 2240 "syntactic.cpp"
    break;

  case 171: // opcode: DIRECTIVE_SUBA
#line 1679 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBA;
	}
#line 2248 "syntactic.cpp"
    break;

  case 172: // opcode: DIRECTIVE_EOR
#line 1683 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EOR;
	}
#line 2256 "syntactic.cpp"
    break;

  case 173: // opcode: DIRECTIVE_CMPM
#line 1687 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPM;
	}
#line 2264 "syntactic.cpp"
    break;

  case 174: // opcode: DIRECTIVE_CMP
#line 1691 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMP;
	}
#line 2272 "syntactic.cpp"
    break;

  case 175: // opcode: DIRECTIVE_CMPA
#line 1695 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPA;
	}
#line 2280 "syntactic.cpp"
    break;

  case 176: // opcode: DIRECTIVE_MULU
#line 1699 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MULU;
	}
#line 2288 "syntactic.cpp"
    break;

  case 177: // opcode: DIRECTIVE_MULS
#line 1703 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MULS;
	}
#line 2296 "syntactic.cpp"
    break;

  case 178: // opcode: DIRECTIVE_ABCD
#line 1707 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ABCD_DATA_REGS;
	}
#line 2304 "syntactic.cpp"
    break;

  case 179: // opcode: DIRECTIVE_EXG
#line 1711 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EXG;
	}
#line 2312 "syntactic.cpp"
    break;

  case 180: // opcode: DIRECTIVE_AND
#line 1715 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_AND_TO_REG;
	}
#line 2320 "syntactic.cpp"
    break;

  case 181: // opcode: DIRECTIVE_ADD
#line 1719 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADD_TO_REG;
	}
#line 2328 "syntactic.cpp"
    break;

  case 182: // opcode: DIRECTIVE_ADDX
#line 1723 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDX_DATA_REGS;
	}
#line 2336 "syntactic.cpp"
    break;

  case 183: // opcode: DIRECTIVE_ADDA
#line 1727 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDA;
	}
#line 2344 "syntactic.cpp"
    break;

  case 184: // opcode: DIRECTIVE_ASL
#line 1731 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ASL_STATIC;
	}
#line 2352 "syntactic.cpp"
    break;

  case 185: // opcode: DIRECTIVE_ASR
#line 1735 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ASR_STATIC;
	}
#line 2360 "syntactic.cpp"
    break;

  case 186: // opcode: DIRECTIVE_LSL
#line 1739 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LSL_STATIC;
	}
#line 2368 "syntactic.cpp"
    break;

  case 187: // opcode: DIRECTIVE_LSR
#line 1743 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LSR_STATIC;
	}
#line 2376 "syntactic.cpp"
    break;

  case 188: // opcode: DIRECTIVE_ROXL
#line 1747 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROXL_STATIC;
	}
#line 2384 "syntactic.cpp"
    break;

  case 189: // opcode: DIRECTIVE_ROXR
#line 1751 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROXR_STATIC;
	}
#line 2392 "syntactic.cpp"
    break;

  case 190: // opcode: DIRECTIVE_ROL
#line 1755 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROL_STATIC;
	}
#line 2400 "syntactic.cpp"
    break;

  case 191: // opcode: DIRECTIVE_ROR
#line 1759 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROR_STATIC;
	}
#line 2408 "syntactic.cpp"
    break;

  case 192: // size: SIZE_BYTE
#line 1766 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_BYTE;
	}
#line 2416 "syntactic.cpp"
    break;

  case 193: // size: SIZE_SHORT
#line 1770 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_SHORT;
	}
#line 2424 "syntactic.cpp"
    break;

  case 194: // size: SIZE_WORD
#line 1774 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_WORD;
	}
#line 2432 "syntactic.cpp"
    break;

  case 195: // size: SIZE_LONGWORD
#line 1778 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_LONGWORD;
	}
#line 2440 "syntactic.cpp"
    break;

  case 196: // operand: "(" ADDRESS_REGISTER ")"
#line 1786 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT;
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2449 "syntactic.cpp"
    break;

  case 197: // operand: "(" ADDRESS_REGISTER ")" "+"
#line 1791 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());
	}
#line 2458 "syntactic.cpp"
    break;

  case 198: // operand: "-" "(" ADDRESS_REGISTER ")"
#line 1796 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2467 "syntactic.cpp"
    break;

  case 199: // operand: expression "(" ADDRESS_REGISTER ")"
#line 1801 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2477 "syntactic.cpp"
    break;

  case 200: // operand: "(" expression "," ADDRESS_REGISTER ")"
#line 1807 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2487 "syntactic.cpp"
    break;

  case 201: // operand: "(" ADDRESS_REGISTER "," expression ")"
#line 1813 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[1].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[3].value.as < unsigned long > ());
	}
#line 2497 "syntactic.cpp"
    break;

  case 202: // operand: "(" ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1819 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[4].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2512 "syntactic.cpp"
    break;

  case 203: // operand: expression "(" ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1830 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[6].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[4].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2527 "syntactic.cpp"
    break;

  case 204: // operand: "(" expression "," ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1841 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[6].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[4].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2542 "syntactic.cpp"
    break;

  case 205: // operand: "(" ADDRESS_REGISTER "," data_or_address_register ")"
#line 1852 "syntactic.y"
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
#line 2558 "syntactic.cpp"
    break;

  case 206: // operand: expression "(" ADDRESS_REGISTER "," data_or_address_register ")"
#line 1864 "syntactic.y"
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
#line 2574 "syntactic.cpp"
    break;

  case 207: // operand: "(" expression "," ADDRESS_REGISTER "," data_or_address_register ")"
#line 1876 "syntactic.y"
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
#line 2590 "syntactic.cpp"
    break;

  case 208: // operand: expression "(" PROGRAM_COUNTER ")"
#line 1888 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
	}
#line 2599 "syntactic.cpp"
    break;

  case 209: // operand: "(" expression "," PROGRAM_COUNTER ")"
#line 1893 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
	}
#line 2608 "syntactic.cpp"
    break;

  case 210: // operand: "(" PROGRAM_COUNTER "," expression ")"
#line 1898 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[1].value.as < Expression > ());
	}
#line 2617 "syntactic.cpp"
    break;

  case 211: // operand: "(" PROGRAM_COUNTER "," data_or_address_register size ")"
#line 1903 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2631 "syntactic.cpp"
    break;

  case 212: // operand: expression "(" PROGRAM_COUNTER "," data_or_address_register size ")"
#line 1913 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[6].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2645 "syntactic.cpp"
    break;

  case 213: // operand: "(" expression "," PROGRAM_COUNTER "," data_or_address_register size ")"
#line 1923 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[6].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2659 "syntactic.cpp"
    break;

  case 214: // operand: "(" PROGRAM_COUNTER "," data_or_address_register ")"
#line 1933 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2674 "syntactic.cpp"
    break;

  case 215: // operand: expression "(" PROGRAM_COUNTER "," data_or_address_register ")"
#line 1944 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[5].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2689 "syntactic.cpp"
    break;

  case 216: // operand: "(" expression "," PROGRAM_COUNTER "," data_or_address_register ")"
#line 1955 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		error_manager.WarningPedantic("Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[5].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2704 "syntactic.cpp"
    break;

  case 217: // operand: "#" expression
#line 1967 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_LITERAL;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2713 "syntactic.cpp"
    break;

  case 218: // operand: STATUS_REGISTER
#line 1983 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_STATUS_REGISTER;
	}
#line 2721 "syntactic.cpp"
    break;

  case 219: // operand: CONDITION_CODE_REGISTER
#line 1987 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_CONDITION_CODE_REGISTER;
	}
#line 2729 "syntactic.cpp"
    break;

  case 220: // operand: USER_STACK_POINTER_REGISTER
#line 1991 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_USER_STACK_POINTER_REGISTER;
	}
#line 2737 "syntactic.cpp"
    break;

  case 221: // operand: expression
#line 1996 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[0].value.as < Expression > ());
		yylhs.value.as < Operand > ().size = SIZE_UNDEFINED;
	}
#line 2747 "syntactic.cpp"
    break;

  case 222: // operand: expression size
#line 2002 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_ABSOLUTE;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[1].value.as < Expression > ());
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[0].value.as < Size > ());
	}
#line 2757 "syntactic.cpp"
    break;

  case 223: // operand: register_list
#line 2009 "syntactic.y"
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
#line 2791 "syntactic.cpp"
    break;

  case 224: // register_list: register_span
#line 2042 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2799 "syntactic.cpp"
    break;

  case 225: // register_list: register_span "/" register_list
#line 2046 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = YY_MOVE (yystack_[2].value.as < unsigned long > ()) | YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2807 "syntactic.cpp"
    break;

  case 226: // register_span: data_or_address_register
#line 2053 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 1 << YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2815 "syntactic.cpp"
    break;

  case 227: // register_span: data_or_address_register "-" data_or_address_register
#line 2057 "syntactic.y"
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
#line 2843 "syntactic.cpp"
    break;

  case 228: // data_or_address_register: DATA_REGISTER
#line 2084 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 0 + YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2851 "syntactic.cpp"
    break;

  case 229: // data_or_address_register: ADDRESS_REGISTER
#line 2088 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 8 + YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2859 "syntactic.cpp"
    break;

  case 230: // expression: expression1
#line 2101 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2867 "syntactic.cpp"
    break;

  case 231: // expression: expression LOGICAL_AND expression1
#line 2106 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LOGICAL_AND, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2875 "syntactic.cpp"
    break;

  case 232: // expression: expression LOGICAL_OR expression1
#line 2111 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LOGICAL_OR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2883 "syntactic.cpp"
    break;

  case 233: // expression1: expression2
#line 2118 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2891 "syntactic.cpp"
    break;

  case 234: // expression1: expression1 "=" expression2
#line 2122 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::EQUALITY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2899 "syntactic.cpp"
    break;

  case 235: // expression1: expression1 EQUALITY expression2
#line 2126 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::EQUALITY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2907 "syntactic.cpp"
    break;

  case 236: // expression1: expression1 INEQUALITY expression2
#line 2130 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::INEQUALITY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2915 "syntactic.cpp"
    break;

  case 237: // expression2: expression3
#line 2137 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2923 "syntactic.cpp"
    break;

  case 238: // expression2: expression2 "<" expression3
#line 2141 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LESS_THAN, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2931 "syntactic.cpp"
    break;

  case 239: // expression2: expression2 LESS_OR_EQUAL expression3
#line 2145 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LESS_OR_EQUAL, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2939 "syntactic.cpp"
    break;

  case 240: // expression2: expression2 ">" expression3
#line 2149 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MORE_THAN, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2947 "syntactic.cpp"
    break;

  case 241: // expression2: expression2 MORE_OR_EQUAL expression3
#line 2153 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MORE_OR_EQUAL, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2955 "syntactic.cpp"
    break;

  case 242: // expression3: expression4
#line 2160 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2963 "syntactic.cpp"
    break;

  case 243: // expression3: expression3 "+" expression4
#line 2164 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::ADD, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2971 "syntactic.cpp"
    break;

  case 244: // expression3: expression3 "-" expression4
#line 2168 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::SUBTRACT, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2979 "syntactic.cpp"
    break;

  case 245: // expression4: expression5
#line 2175 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2987 "syntactic.cpp"
    break;

  case 246: // expression4: expression4 "*" expression5
#line 2179 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MULTIPLY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 2995 "syntactic.cpp"
    break;

  case 247: // expression4: expression4 "/" expression5
#line 2183 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::DIVIDE, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3003 "syntactic.cpp"
    break;

  case 248: // expression4: expression4 "%" expression5
#line 2187 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MODULO, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3011 "syntactic.cpp"
    break;

  case 249: // expression5: expression6
#line 2194 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3019 "syntactic.cpp"
    break;

  case 250: // expression5: expression5 "&" expression6
#line 2198 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_AND, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3027 "syntactic.cpp"
    break;

  case 251: // expression5: expression5 "!" expression6
#line 2202 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_OR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3035 "syntactic.cpp"
    break;

  case 252: // expression5: expression5 "|" expression6
#line 2206 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_OR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3043 "syntactic.cpp"
    break;

  case 253: // expression5: expression5 "^" expression6
#line 2210 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_XOR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3051 "syntactic.cpp"
    break;

  case 254: // expression6: expression7
#line 2217 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3059 "syntactic.cpp"
    break;

  case 255: // expression6: expression6 LEFT_SHIFT expression7
#line 2221 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LEFT_SHIFT, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3067 "syntactic.cpp"
    break;

  case 256: // expression6: expression6 RIGHT_SHIFT expression7
#line 2225 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::RIGHT_SHIFT, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3075 "syntactic.cpp"
    break;

  case 257: // expression7: expression8
#line 2232 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3083 "syntactic.cpp"
    break;

  case 258: // expression7: "+" expression7
#line 2236 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3091 "syntactic.cpp"
    break;

  case 259: // expression7: "-" expression7
#line 2240 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::NEGATE, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3099 "syntactic.cpp"
    break;

  case 260: // expression7: "~" expression7
#line 2244 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::BITWISE_NOT, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3107 "syntactic.cpp"
    break;

  case 261: // expression7: "!" expression7
#line 2249 "syntactic.y"
        {
		DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::LOGICAL_NOT, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 3115 "syntactic.cpp"
    break;

  case 262: // local_identifier: LOCAL_IDENTIFIER
#line 2255 "syntactic.y"
          { yylhs.value.as < std::string > () = YY_MOVE (yystack_[0].value.as < std::string > ()); }
#line 3121 "syntactic.cpp"
    break;

  case 263: // local_identifier: SIZE_BYTE
#line 2257 "syntactic.y"
        {
		yylhs.value.as < std::string > () = ".b";
	}
#line 3129 "syntactic.cpp"
    break;

  case 264: // local_identifier: SIZE_SHORT
#line 2261 "syntactic.y"
        {
		yylhs.value.as < std::string > () = ".s";
	}
#line 3137 "syntactic.cpp"
    break;

  case 265: // local_identifier: SIZE_WORD
#line 2265 "syntactic.y"
        {
		yylhs.value.as < std::string > () = ".w";
	}
#line 3145 "syntactic.cpp"
    break;

  case 266: // local_identifier: SIZE_LONGWORD
#line 2269 "syntactic.y"
        {
		yylhs.value.as < std::string > () = ".l";
	}
#line 3153 "syntactic.cpp"
    break;

  case 267: // identifier: IDENTIFIER
#line 2275 "syntactic.y"
          { yylhs.value.as < std::string > () = YY_MOVE (yystack_[0].value.as < std::string > ()); }
#line 3159 "syntactic.cpp"
    break;

  case 268: // identifier: local_identifier
#line 2276 "syntactic.y"
          { yylhs.value.as < std::string > () = YY_MOVE (yystack_[0].value.as < std::string > ()); }
#line 3165 "syntactic.cpp"
    break;

  case 269: // identifier: identifier local_identifier
#line 2278 "syntactic.y"
        {
		yylhs.value.as < std::string > () = YY_MOVE (yystack_[1].value.as < std::string > ()) + YY_MOVE (yystack_[0].value.as < std::string > ());
	}
#line 3173 "syntactic.cpp"
    break;

  case 270: // expression8: NUMBER
#line 2285 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionNumber>(YY_MOVE (yystack_[0].value.as < unsigned long > ()));
	}
#line 3181 "syntactic.cpp"
    break;

  case 271: // expression8: identifier
#line 2289 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionIdentifier>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 3189 "syntactic.cpp"
    break;

  case 272: // expression8: string
#line 2293 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3197 "syntactic.cpp"
    break;

  case 273: // expression8: "*"
#line 2297 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionProgramCounterOfStatement>();
	}
#line 3205 "syntactic.cpp"
    break;

  case 274: // expression8: "@"
#line 2301 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionProgramCounterOfExpression>();
	}
#line 3213 "syntactic.cpp"
    break;

  case 275: // expression8: RS
#line 2305 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionRs>();
	}
#line 3221 "syntactic.cpp"
    break;

  case 276: // expression8: NARG
#line 2309 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionNarg>();
	}
#line 3229 "syntactic.cpp"
    break;

  case 277: // expression8: "(" expression ")"
#line 2313 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[1].value.as < Expression > ());
	}
#line 3237 "syntactic.cpp"
    break;

  case 278: // expression8: STRLEN "(" STRING ")"
#line 2317 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionStrlen>(YY_MOVE (yystack_[1].value.as < std::string > ()));
	}
#line 3245 "syntactic.cpp"
    break;

  case 279: // expression8: STRCMP "(" STRING "," STRING ")"
#line 2321 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionStrcmp>(YY_MOVE (yystack_[3].value.as < std::string > ()), YY_MOVE (yystack_[1].value.as < std::string > ()));
	}
#line 3253 "syntactic.cpp"
    break;

  case 280: // expression8: INSTR "(" expression "," expression ")"
#line 2325 "syntactic.y"
        {
		auto subexpressions = std::make_unique<Expression[]>(3);

		subexpressions[0] = ExpressionNumber{1};
		subexpressions[1] = YY_MOVE (yystack_[3].value.as < Expression > ());
		subexpressions[2] = YY_MOVE (yystack_[1].value.as < Expression > ());

		yylhs.value.as < Expression > ().emplace<ExpressionInstr>(std::move(subexpressions));
	}
#line 3267 "syntactic.cpp"
    break;

  case 281: // expression8: INSTR "(" expression "," expression "," expression ")"
#line 2335 "syntactic.y"
        {
		auto subexpressions = std::make_unique<Expression[]>(3);

		subexpressions[0] = YY_MOVE (yystack_[5].value.as < Expression > ());
		subexpressions[1] = YY_MOVE (yystack_[3].value.as < Expression > ());
		subexpressions[2] = YY_MOVE (yystack_[1].value.as < Expression > ());

		yylhs.value.as < Expression > ().emplace<ExpressionInstr>(std::move(subexpressions));
	}
#line 3281 "syntactic.cpp"
    break;

  case 282: // expression8: DEF "(" IDENTIFIER ")"
#line 2345 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionDef>(YY_MOVE (yystack_[1].value.as < std::string > ()));
	}
#line 3289 "syntactic.cpp"
    break;

  case 283: // expression8: TYPE "(" IDENTIFIER ")"
#line 2349 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionTypeWithIdentifier>(YY_MOVE (yystack_[1].value.as < std::string > ()));
	}
#line 3297 "syntactic.cpp"
    break;

  case 284: // expression8: TYPE "(" NUMBER ")"
#line 2353 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionTypeWithNumber>(YY_MOVE (yystack_[1].value.as < unsigned long > ()));
	}
#line 3305 "syntactic.cpp"
    break;

  case 285: // expression8: FILESIZE "(" STRING ")"
#line 2357 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionFilesize>(YY_MOVE (yystack_[1].value.as < std::string > ()));
	}
#line 3313 "syntactic.cpp"
    break;

  case 286: // expression8: OFFSET "(" expression ")"
#line 2361 "syntactic.y"
        {
		/* TODO: Make this actually do something besides pass the expression through. */
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[1].value.as < Expression > ());
	}
#line 3322 "syntactic.cpp"
    break;

  case 287: // string: STRING
#line 2369 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionString>(YY_MOVE (yystack_[0].value.as < std::string > ()));
	}
#line 3330 "syntactic.cpp"
    break;


#line 3334 "syntactic.cpp"

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


  const short parser::yypact_ninf_ = -307;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     294,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,   116,   116,
     116,   577,  -307,  -157,  -149,  -307,  -120,  -102,   577,   -88,
     342,   577,   577,   577,  -307,  -307,   577,  -307,  -307,   577,
     577,  -307,  -307,   116,   577,   577,   577,  -307,   577,  -307,
    -307,   -92,   -98,   -83,  -307,  -307,  -149,  -192,  -307,   -62,
     577,   101,  -307,   -50,     3,  -307,  -307,  -307,  -307,   577,
     577,   577,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,   -65,   -51,   -49,   -35,   -23,    -2,    21,   577,
     577,   577,  -307,  -307,   577,   577,  -145,  -154,  -164,  -104,
       4,   261,    20,  -307,  -307,     8,  -307,  -307,  -307,   -20,
      32,    32,  -307,    34,  -145,  -307,  -307,   389,   -89,  -145,
    -145,  -145,  -145,    47,    71,   577,  -145,  -145,  -145,  -145,
    -307,    41,  -307,  -307,    32,  -307,  -307,  -307,  -145,  -307,
    -307,  -307,  -307,  -307,  -307,    22,   624,   577,    43,  -307,
      48,    63,    69,  -307,  -307,    76,  -145,    75,  -145,    98,
     102,   577,   115,    51,   119,   577,  -118,  -307,  -307,  -307,
    -307,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,   577,   577,   577,   577,   577,   577,   577,   577,   577,
     577,  -307,  -149,   117,   436,   120,    77,   483,   577,   121,
    -145,   122,   -86,    92,  -110,   530,  -145,   -50,    74,    74,
    -146,  -307,   577,   577,   259,   284,    79,   292,   293,   296,
     301,   -75,  -307,  -154,  -154,  -164,  -164,  -164,  -104,  -104,
    -104,  -104,     4,     4,   261,   261,   261,    20,    20,    20,
      20,  -307,  -307,    32,  -307,   577,    82,  -307,   321,   329,
      90,  -145,   306,  -307,   295,   303,   295,  -105,   312,  -307,
    -307,  -307,   -68,   -63,  -145,  -145,  -307,   343,   577,  -307,
    -307,  -307,  -307,  -307,  -145,   577,  -307,  -307,   344,   577,
    -127,   -25,  -307,  -123,   -18,   -33,   -26,  -307,    74,  -307,
      74,  -307,   319,   -21,  -145,  -307,    76,  -307,   320,  -307,
    -307,   322,  -307,    74,  -307,    74,  -307,  -107,   -99,  -307,
     577,  -307,  -307,  -307,   -87,   -71,  -307,   323,  -307,   324,
       5,  -307,   325,  -307,   326,  -307,  -307,  -307,  -307,  -307
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
       0,     0,   263,   264,   265,   266,   270,   267,   262,   287,
     275,   276,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   273,   274,     0,     0,    12,   230,   233,   237,
     242,   245,   249,   254,   268,   271,   257,   272,    65,    15,
      16,    19,     7,     8,    21,    23,    22,     0,     0,    28,
      30,    31,    35,     0,     0,     0,    44,    45,    47,    49,
      67,    52,    53,    54,    57,    59,    60,    62,    29,     1,
     228,   229,   218,   219,   220,     0,     0,     0,    70,   223,
     224,   226,   221,    73,    74,     4,    63,     0,     6,     0,
       0,     0,     0,     0,     0,     0,     0,   258,   259,   261,
     260,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   269,     0,     0,     0,     0,     0,     0,     0,     0,
      43,     0,     0,     0,     0,     0,   217,     0,     0,     0,
       0,   222,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   277,   231,   232,   235,   236,   234,   239,   241,
     238,   240,   243,   244,   246,   247,   248,   250,   253,   251,
     252,   255,   256,    17,    66,     0,     9,    24,     0,     0,
       0,    38,    39,    68,     0,   196,     0,     0,     0,    71,
     225,   227,     0,     0,    64,     5,   278,     0,     0,   282,
     284,   283,   285,   286,    10,     0,    26,    25,     0,     0,
       0,     0,   197,     0,     0,     0,     0,   198,     0,   199,
       0,   208,     0,     0,    11,    27,    40,   205,     0,   201,
     214,     0,   210,     0,   200,     0,   209,     0,     0,   279,
       0,   280,   202,   211,     0,     0,   206,     0,   215,     0,
       0,   207,     0,   216,     0,   203,   212,   281,   204,   213
  };

  const short
  parser::yypgoto_[] =
  {
    -307,  -307,   139,  -122,  -307,  -307,  -307,  -307,  -119,   217,
     218,  -307,  -306,  -117,    11,   -74,   195,    -5,   -39,   190,
    -185,   331,  -307,  -307,  -307
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,   161,   255,   210,   231,   162,   163,   164,   169,   248,
     249,   250,   251,   252,   197,   198,   199,   200,   201,   202,
     203,   204,   205,   206,   207
  };

  const short
  parser::yytable_[] =
  {
     170,   171,   211,   361,   196,   267,   268,   235,   236,   269,
     270,   214,   208,   218,   219,   220,   221,   276,   277,   222,
     208,   362,   223,   224,   225,   273,   274,   226,   227,   228,
     363,   229,   271,   272,   234,   165,   166,   167,   168,   165,
     166,   167,   168,   238,   209,   254,   278,   279,   380,   275,
     383,   212,   256,   257,   258,   165,   166,   167,   168,   271,
     272,   268,   385,   165,   166,   167,   168,   271,   272,   213,
     397,   386,   266,   232,   400,   165,   166,   167,   168,   322,
     230,   215,   407,   216,   408,   357,   233,   322,   271,   272,
     416,   165,   166,   167,   168,   280,   281,   414,   418,   415,
     296,   239,   271,   272,   341,   342,   297,   237,   300,   354,
     421,   355,   172,   173,   174,   175,   240,   241,   176,   177,
     178,   179,   373,   242,   243,   244,   423,   388,   304,   389,
     306,   259,   390,   311,   391,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   316,   260,   245,   261,   321,   190,
     246,   192,   271,   272,   193,   247,   271,   272,   194,   271,
     272,   262,   403,   195,   404,   165,   166,   167,   168,   405,
     343,   406,   399,   263,   410,   292,   411,   346,   178,   402,
     350,   351,   271,   272,   172,   173,   174,   175,   266,   302,
     176,   177,   178,   179,   264,   364,   365,   253,   303,   325,
     326,   327,   427,   289,   290,   282,   283,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   284,   265,   189,   318,
     319,   190,   191,   192,   271,   272,   193,   293,   374,   294,
     194,   165,   166,   167,   168,   195,   301,   381,   307,   384,
     240,   241,   298,   334,   335,   336,   271,   272,   271,   272,
     308,   393,   271,   272,   271,   272,   271,   272,   394,   271,
     272,   398,   256,   309,   401,   310,   299,   271,   272,   314,
     313,   312,   348,   315,   368,   332,   333,   375,   165,   166,
     167,   168,   323,   324,   317,   378,   344,   356,   417,   419,
     320,   347,   352,   420,   353,   422,   424,     1,     2,     3,
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
     154,   155,   156,   157,   158,   159,   366,   172,   173,   174,
     175,   240,   241,   176,   177,   178,   179,   285,   286,   287,
     288,   328,   329,   330,   331,   337,   338,   339,   340,   367,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   369,
     370,   189,   376,   371,   190,   191,   192,   160,   372,   193,
     377,   379,   382,   194,   172,   173,   174,   175,   195,   387,
     176,   177,   178,   179,   392,   395,   409,   412,   396,   413,
     425,   426,   428,   429,   359,     0,   360,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   291,   217,   189,     0,
       0,   190,   191,   192,     0,     0,   193,     0,     0,     0,
     194,   172,   173,   174,   175,   195,     0,   176,   177,   178,
     179,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   180,   181,   182,   183,   184,   185,
     186,   187,   188,     0,   295,   189,     0,     0,   190,   191,
     192,     0,     0,   193,     0,     0,     0,   194,   172,   173,
     174,   175,   195,     0,   176,   177,   178,   179,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   180,   181,   182,   183,   184,   185,   186,   187,   188,
       0,   345,   189,     0,     0,   190,   191,   192,     0,     0,
     193,     0,     0,     0,   194,   172,   173,   174,   175,   195,
       0,   176,   177,   178,   179,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   180,   181,
     182,   183,   184,   185,   186,   187,   188,     0,   349,   189,
       0,     0,   190,   191,   192,     0,     0,   193,     0,     0,
       0,   194,   172,   173,   174,   175,   195,   358,   176,   177,
     178,   179,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   180,   181,   182,   183,   184,
     185,   186,   187,   188,     0,     0,   189,     0,     0,   190,
     191,   192,     0,     0,   193,     0,     0,     0,   194,   172,
     173,   174,   175,   195,     0,   176,   177,   178,   179,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   180,   181,   182,   183,   184,   185,   186,   187,
     188,     0,     0,   189,     0,     0,   190,   191,   192,     0,
       0,   193,     0,     0,     0,   194,   172,   173,   174,   175,
     195,     0,   176,   177,   178,   179,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   180,
     181,   182,   183,   184,   185,   186,   187,   188,     0,     0,
     305,     0,     0,   190,   191,   192,     0,     0,   193,     0,
       0,     0,   194,     0,     0,     0,     0,   195
  };

  const short
  parser::yycheck_[] =
  {
     119,   120,   124,   309,   121,   190,   191,   199,   200,   194,
     195,   128,   169,   130,   131,   132,   133,   181,   182,   136,
     169,   167,   139,   140,   143,   179,   180,   144,   145,   146,
     176,   148,   177,   178,   156,   162,   163,   164,   165,   162,
     163,   164,   165,   160,   201,   164,   210,   211,   354,   203,
     356,   171,   169,   170,   171,   162,   163,   164,   165,   177,
     178,   246,   167,   162,   163,   164,   165,   177,   178,   171,
     197,   176,   189,   171,   197,   162,   163,   164,   165,   197,
     172,   169,   388,   171,   390,   195,   169,   197,   177,   178,
     197,   162,   163,   164,   165,   199,   200,   403,   197,   405,
     217,     0,   177,   178,   289,   290,   195,   169,   225,   195,
     197,   197,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   197,   173,   174,   175,   197,   195,   245,   197,
     247,   196,   195,   252,   197,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   261,   196,   196,   196,   265,   199,
     200,   201,   177,   178,   204,   205,   177,   178,   208,   177,
     178,   196,   195,   213,   197,   162,   163,   164,   165,   195,
     292,   197,   197,   196,   195,   195,   197,   294,   170,   197,
     297,   298,   177,   178,   162,   163,   164,   165,   305,   167,
     168,   169,   170,   171,   196,   312,   313,   194,   176,   273,
     274,   275,   197,   183,   184,   201,   202,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   212,   196,   196,   168,
     169,   199,   200,   201,   177,   178,   204,   195,   345,   195,
     208,   162,   163,   164,   165,   213,   195,   354,   195,   356,
     166,   167,   195,   282,   283,   284,   177,   178,   177,   178,
     202,   368,   177,   178,   177,   178,   177,   178,   375,   177,
     178,   380,   379,   200,   383,   196,   195,   177,   178,   171,
     195,   195,   195,   171,   195,   280,   281,   195,   162,   163,
     164,   165,   271,   272,   169,   195,   169,   195,   407,   408,
     171,   171,   171,   410,   172,   414,   415,     3,     4,     5,
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
     156,   157,   158,   159,   160,   161,   197,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   206,   207,   208,
     209,   276,   277,   278,   279,   285,   286,   287,   288,   195,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   197,
     197,   196,   171,   197,   199,   200,   201,   203,   197,   204,
     171,   195,   199,   208,   162,   163,   164,   165,   213,   197,
     168,   169,   170,   171,   171,   171,   197,   197,   379,   197,
     197,   197,   197,   197,   307,    -1,   308,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   205,   195,   196,    -1,
      -1,   199,   200,   201,    -1,    -1,   204,    -1,    -1,    -1,
     208,   162,   163,   164,   165,   213,    -1,   168,   169,   170,
     171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   185,   186,   187,   188,   189,   190,
     191,   192,   193,    -1,   195,   196,    -1,    -1,   199,   200,
     201,    -1,    -1,   204,    -1,    -1,    -1,   208,   162,   163,
     164,   165,   213,    -1,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,   187,   188,   189,   190,   191,   192,   193,
      -1,   195,   196,    -1,    -1,   199,   200,   201,    -1,    -1,
     204,    -1,    -1,    -1,   208,   162,   163,   164,   165,   213,
      -1,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,   186,
     187,   188,   189,   190,   191,   192,   193,    -1,   195,   196,
      -1,    -1,   199,   200,   201,    -1,    -1,   204,    -1,    -1,
      -1,   208,   162,   163,   164,   165,   213,   167,   168,   169,
     170,   171,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   185,   186,   187,   188,   189,
     190,   191,   192,   193,    -1,    -1,   196,    -1,    -1,   199,
     200,   201,    -1,    -1,   204,    -1,    -1,    -1,   208,   162,
     163,   164,   165,   213,    -1,   168,   169,   170,   171,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   185,   186,   187,   188,   189,   190,   191,   192,
     193,    -1,    -1,   196,    -1,    -1,   199,   200,   201,    -1,
      -1,   204,    -1,    -1,    -1,   208,   162,   163,   164,   165,
     213,    -1,   168,   169,   170,   171,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   185,
     186,   187,   188,   189,   190,   191,   192,   193,    -1,    -1,
     196,    -1,    -1,   199,   200,   201,    -1,    -1,   204,    -1,
      -1,    -1,   208,    -1,    -1,    -1,    -1,   213
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
     203,   217,   221,   222,   223,   162,   163,   164,   165,   224,
     224,   224,   162,   163,   164,   165,   168,   169,   170,   171,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   196,
     199,   200,   201,   204,   208,   213,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   240,   169,   201,
     219,   219,   171,   171,   229,   169,   171,   195,   229,   229,
     229,   229,   229,   229,   229,   224,   229,   229,   229,   229,
     172,   220,   171,   169,   219,   199,   200,   169,   229,     0,
     166,   167,   173,   174,   175,   196,   200,   205,   225,   226,
     227,   228,   229,   194,   224,   218,   229,   229,   229,   196,
     196,   196,   196,   196,   196,   196,   229,   236,   236,   236,
     236,   177,   178,   179,   180,   203,   181,   182,   210,   211,
     199,   200,   201,   202,   212,   206,   207,   208,   209,   183,
     184,   237,   195,   195,   195,   195,   229,   195,   195,   195,
     229,   195,   167,   176,   229,   196,   229,   195,   202,   200,
     196,   224,   195,   195,   171,   171,   229,   169,   168,   169,
     171,   229,   197,   230,   230,   231,   231,   231,   232,   232,
     232,   232,   233,   233,   234,   234,   234,   235,   235,   235,
     235,   236,   236,   219,   169,   195,   229,   171,   195,   195,
     229,   229,   171,   172,   195,   197,   195,   195,   167,   225,
     226,   228,   167,   176,   229,   229,   197,   195,   195,   197,
     197,   197,   197,   197,   229,   195,   171,   171,   195,   195,
     228,   229,   199,   228,   229,   167,   176,   197,   195,   197,
     195,   197,   171,   229,   229,   171,   218,   197,   224,   197,
     197,   224,   197,   195,   197,   195,   197,   228,   228,   197,
     195,   197,   197,   197,   228,   228,   197,   224,   197,   224,
     229,   197,   224,   197,   224,   197,   197,   197,   197,   197
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,   216,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   217,   217,   217,   217,   217,   217,   217,
     217,   217,   217,   218,   218,   219,   219,   220,   220,   221,
     221,   221,   222,   222,   222,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   223,   223,   223,   223,   223,
     223,   223,   224,   224,   224,   224,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   225,   225,   225,   225,   225,   225,
     225,   225,   225,   225,   226,   226,   227,   227,   228,   228,
     229,   229,   229,   230,   230,   230,   230,   231,   231,   231,
     231,   231,   232,   232,   232,   233,   233,   233,   233,   234,
     234,   234,   234,   234,   235,   235,   235,   236,   236,   236,
     236,   236,   237,   237,   237,   237,   237,   238,   238,   238,
     239,   239,   239,   239,   239,   239,   239,   239,   239,   239,
     239,   239,   239,   239,   239,   239,   239,   240
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
       1,     1,     1,     1,     1,     1,     1,     3,     4,     6,
       6,     8,     4,     4,     4,     4,     4,     1
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
       0,   912,   912,   915,   919,   923,   927,   931,   935,   939,
     943,   947,   951,   955,   959,   963,   967,   971,   975,   979,
     983,   987,   991,   995,   999,  1005,  1011,  1015,  1019,  1023,
    1027,  1031,  1035,  1039,  1043,  1047,  1051,  1055,  1059,  1063,
    1067,  1072,  1076,  1080,  1084,  1088,  1094,  1098,  1102,  1106,
    1110,  1114,  1118,  1122,  1126,  1130,  1134,  1138,  1142,  1147,
    1152,  1157,  1162,  1169,  1173,  1182,  1186,  1195,  1199,  1208,
    1214,  1220,  1229,  1234,  1240,  1248,  1252,  1256,  1260,  1264,
    1268,  1272,  1276,  1280,  1284,  1288,  1292,  1296,  1300,  1304,
    1308,  1312,  1316,  1320,  1324,  1328,  1332,  1336,  1340,  1344,
    1348,  1352,  1356,  1360,  1364,  1368,  1372,  1376,  1380,  1384,
    1388,  1392,  1396,  1400,  1404,  1408,  1412,  1417,  1422,  1427,
    1432,  1437,  1442,  1447,  1452,  1457,  1462,  1467,  1472,  1477,
    1482,  1487,  1492,  1497,  1502,  1507,  1512,  1517,  1522,  1527,
    1532,  1537,  1542,  1547,  1552,  1557,  1562,  1567,  1572,  1576,
    1580,  1585,  1590,  1595,  1600,  1605,  1610,  1615,  1620,  1625,
    1630,  1635,  1640,  1645,  1650,  1654,  1658,  1662,  1666,  1670,
    1674,  1678,  1682,  1686,  1690,  1694,  1698,  1702,  1706,  1710,
    1714,  1718,  1722,  1726,  1730,  1734,  1738,  1742,  1746,  1750,
    1754,  1758,  1765,  1769,  1773,  1777,  1785,  1790,  1795,  1800,
    1806,  1812,  1818,  1829,  1840,  1851,  1863,  1875,  1887,  1892,
    1897,  1902,  1912,  1922,  1932,  1943,  1954,  1966,  1982,  1986,
    1990,  1995,  2001,  2008,  2041,  2045,  2052,  2056,  2083,  2087,
    2100,  2105,  2110,  2117,  2121,  2125,  2129,  2136,  2140,  2144,
    2148,  2152,  2159,  2163,  2167,  2174,  2178,  2182,  2186,  2193,
    2197,  2201,  2205,  2209,  2216,  2220,  2224,  2231,  2235,  2239,
    2243,  2248,  2255,  2256,  2260,  2264,  2268,  2275,  2276,  2277,
    2284,  2288,  2292,  2296,  2300,  2304,  2308,  2312,  2316,  2320,
    2324,  2334,  2344,  2348,  2352,  2356,  2360,  2368
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
#line 4226 "syntactic.cpp"

