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
#line 662 "syntactic.y"


#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

YY_DECL;
void m68kasm_warning(void *scanner, Statement *statement, const char *message);
void m68kasm_warning_pedantic(void *scanner, Statement *statement, const char *message);
void m68kasm_error(void *scanner, Statement *statement, const char *message);

static void DestroyExpressionList(ExpressionList *list);
static void DestroyOperand(Operand *operand);
static void DestroyStatementInstruction(StatementInstruction *instruction);

void m68kasm::parser::error(const std::string &message)
{
	m68kasm_error(scanner, statement, message.c_str());
}


#line 71 "syntactic.cpp"


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
#line 145 "syntactic.cpp"

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
#line 921 "syntactic.y"
        {
		statement->emplace<StatementEmpty>();
	}
#line 815 "syntactic.cpp"
    break;

  case 3: // statement: instruction
#line 925 "syntactic.y"
        {
		statement->emplace<StatementInstruction>(yystack_[0].value.as < StatementInstruction > ());
	}
#line 823 "syntactic.cpp"
    break;

  case 4: // statement: DIRECTIVE_DC size expression_list
#line 929 "syntactic.y"
        {
		statement->emplace<StatementDc>(yystack_[1].value.as < Size > (), yystack_[0].value.as < ExpressionList > ());
	}
#line 831 "syntactic.cpp"
    break;

  case 5: // statement: DIRECTIVE_DCB size expression "," expression
#line 933 "syntactic.y"
        {
		statement->emplace<StatementDcb>(yystack_[3].value.as < Size > (), yystack_[2].value.as < Expression > (), yystack_[0].value.as < Expression > ());
	}
#line 839 "syntactic.cpp"
    break;

  case 6: // statement: DIRECTIVE_DS size expression
#line 937 "syntactic.y"
        {
		statement->emplace<StatementDs>(yystack_[1].value.as < Size > (), yystack_[0].value.as < Expression > ());
	}
#line 847 "syntactic.cpp"
    break;

  case 7: // statement: DIRECTIVE_INCLUDE STRING
#line 941 "syntactic.y"
        {
		statement->emplace<StatementInclude>(yystack_[0].value.as < String > ());
	}
#line 855 "syntactic.cpp"
    break;

  case 8: // statement: DIRECTIVE_INCBIN STRING
#line 945 "syntactic.y"
        {		
		statement->emplace<StatementIncbin>(yystack_[0].value.as < String > (), ExpressionNumber{0}, cc_false);
	}
#line 863 "syntactic.cpp"
    break;

  case 9: // statement: DIRECTIVE_INCBIN STRING "," expression
#line 949 "syntactic.y"
        {
		statement->emplace<StatementIncbin>(yystack_[2].value.as < String > (), yystack_[0].value.as < Expression > (), cc_false);
	}
#line 871 "syntactic.cpp"
    break;

  case 10: // statement: DIRECTIVE_INCBIN STRING "," "," expression
#line 953 "syntactic.y"
        {
		statement->emplace<StatementIncbin>(yystack_[3].value.as < String > (), ExpressionNumber{0}, cc_true, yystack_[0].value.as < Expression > ());
	}
#line 879 "syntactic.cpp"
    break;

  case 11: // statement: DIRECTIVE_INCBIN STRING "," expression "," expression
#line 957 "syntactic.y"
        {
		statement->emplace<StatementIncbin>(yystack_[4].value.as < String > (), yystack_[2].value.as < Expression > (), cc_true, yystack_[0].value.as < Expression > ());
	}
#line 887 "syntactic.cpp"
    break;

  case 12: // statement: DIRECTIVE_REPT expression
#line 961 "syntactic.y"
        {
		statement->emplace<StatementRept>(yystack_[0].value.as < Expression > ());
	}
#line 895 "syntactic.cpp"
    break;

  case 13: // statement: DIRECTIVE_ENDR
#line 965 "syntactic.y"
        {
		statement->emplace<StatementEndR>();
	}
#line 903 "syntactic.cpp"
    break;

  case 14: // statement: DIRECTIVE_MACRO
#line 969 "syntactic.y"
        {
		statement->emplace<StatementMacro>(cc_false);
	}
#line 911 "syntactic.cpp"
    break;

  case 15: // statement: DIRECTIVE_MACRO "*"
#line 973 "syntactic.y"
        {
		statement->emplace<StatementMacro>(cc_true);
	}
#line 919 "syntactic.cpp"
    break;

  case 16: // statement: DIRECTIVE_MACRO identifier_list
#line 977 "syntactic.y"
        {
		statement->emplace<StatementMacro>(cc_false, yystack_[0].value.as < IdentifierList > ());
	}
#line 927 "syntactic.cpp"
    break;

  case 17: // statement: DIRECTIVE_MACRO "*" "," identifier_list
#line 981 "syntactic.y"
        {
		statement->emplace<StatementMacro>(cc_true, yystack_[0].value.as < IdentifierList > ());
	}
#line 935 "syntactic.cpp"
    break;

  case 18: // statement: DIRECTIVE_MACROS
#line 985 "syntactic.y"
        {
		// TODO: Is there a way to make this tidier?
		statement->emplace<StatementMacroS>(StatementMacro{cc_false});
	}
#line 944 "syntactic.cpp"
    break;

  case 19: // statement: DIRECTIVE_MACROS identifier_list
#line 990 "syntactic.y"
        {
		// TODO: Is there a way to make this tidier?
		statement->emplace<StatementMacroS>(StatementMacro{cc_false, yystack_[0].value.as < IdentifierList > ()});
	}
#line 953 "syntactic.cpp"
    break;

  case 20: // statement: DIRECTIVE_ENDM
#line 995 "syntactic.y"
        {
		statement->emplace<StatementEndM>();
	}
#line 961 "syntactic.cpp"
    break;

  case 21: // statement: DIRECTIVE_EQU expression
#line 999 "syntactic.y"
        {
		statement->emplace<StatementEqu>(yystack_[0].value.as < Expression > ());
	}
#line 969 "syntactic.cpp"
    break;

  case 22: // statement: DIRECTIVE_EQUS STRING
#line 1003 "syntactic.y"
        {
		statement->emplace<StatementEquSString>(yystack_[0].value.as < String > ());
	}
#line 977 "syntactic.cpp"
    break;

  case 23: // statement: DIRECTIVE_EQUS IDENTIFIER
#line 1007 "syntactic.y"
        {
		statement->emplace<StatementEquSIdentifier>(yystack_[0].value.as < String > ());
	}
#line 985 "syntactic.cpp"
    break;

  case 24: // statement: DIRECTIVE_SUBSTR "," "," STRING
#line 1011 "syntactic.y"
        {
		statement->emplace<StatementSubstr>(ExpressionNumber{1}, ExpressionNumber{String_Length(yystack_[0].value.as < String > ())}, yystack_[0].value.as < String > ());
	}
#line 993 "syntactic.cpp"
    break;

  case 25: // statement: DIRECTIVE_SUBSTR expression "," "," STRING
#line 1015 "syntactic.y"
        {
		statement->emplace<StatementSubstr>(yystack_[3].value.as < Expression > (), ExpressionNumber{String_Length(yystack_[0].value.as < String > ())}, yystack_[0].value.as < String > ());
	}
#line 1001 "syntactic.cpp"
    break;

  case 26: // statement: DIRECTIVE_SUBSTR "," expression "," STRING
#line 1019 "syntactic.y"
        {
		statement->emplace<StatementSubstr>(ExpressionNumber{1}, yystack_[2].value.as < Expression > (), yystack_[0].value.as < String > ());
	}
#line 1009 "syntactic.cpp"
    break;

  case 27: // statement: DIRECTIVE_SUBSTR expression "," expression "," STRING
#line 1023 "syntactic.y"
        {
		statement->emplace<StatementSubstr>(yystack_[4].value.as < Expression > (), yystack_[2].value.as < Expression > (), yystack_[0].value.as < String > ());
	}
#line 1017 "syntactic.cpp"
    break;

  case 28: // statement: DIRECTIVE_SET expression
#line 1027 "syntactic.y"
        {
		statement->emplace<StatementSet>(yystack_[0].value.as < Expression > ());
	}
#line 1025 "syntactic.cpp"
    break;

  case 29: // statement: "=" expression
#line 1031 "syntactic.y"
        {
		statement->emplace<StatementSet>(yystack_[0].value.as < Expression > ());
	}
#line 1033 "syntactic.cpp"
    break;

  case 30: // statement: DIRECTIVE_IF expression
#line 1035 "syntactic.y"
        {
		statement->emplace<StatementIf>(yystack_[0].value.as < Expression > ());
	}
#line 1041 "syntactic.cpp"
    break;

  case 31: // statement: DIRECTIVE_ELSEIF expression
#line 1039 "syntactic.y"
        {
		statement->emplace<StatementElseIf>(yystack_[0].value.as < Expression > ());
	}
#line 1049 "syntactic.cpp"
    break;

  case 32: // statement: DIRECTIVE_ELSEIF
#line 1043 "syntactic.y"
        {
		statement->emplace<StatementElse>();
	}
#line 1057 "syntactic.cpp"
    break;

  case 33: // statement: DIRECTIVE_ELSE
#line 1047 "syntactic.y"
        {
		statement->emplace<StatementElse>();
	}
#line 1065 "syntactic.cpp"
    break;

  case 34: // statement: DIRECTIVE_ENDC
#line 1051 "syntactic.y"
        {
		statement->emplace<StatementEndC>();
	}
#line 1073 "syntactic.cpp"
    break;

  case 35: // statement: DIRECTIVE_WHILE expression
#line 1055 "syntactic.y"
        {
		statement->emplace<StatementWhile>(yystack_[0].value.as < Expression > ());
	}
#line 1081 "syntactic.cpp"
    break;

  case 36: // statement: DIRECTIVE_ENDW
#line 1059 "syntactic.y"
        {
		statement->emplace<StatementEndW>();
	}
#line 1089 "syntactic.cpp"
    break;

  case 37: // statement: DIRECTIVE_EVEN
#line 1063 "syntactic.y"
        {
		statement->emplace<StatementEven>();
	}
#line 1097 "syntactic.cpp"
    break;

  case 38: // statement: DIRECTIVE_CNOP expression "," expression
#line 1067 "syntactic.y"
        {
		statement->emplace<StatementCnop>(yystack_[2].value.as < Expression > (), yystack_[0].value.as < Expression > ());
	}
#line 1105 "syntactic.cpp"
    break;

  case 39: // statement: DIRECTIVE_INFORM expression "," STRING
#line 1071 "syntactic.y"
        {
		statement->emplace<StatementInform>(yystack_[2].value.as < Expression > (), yystack_[0].value.as < String > ());
	}
#line 1113 "syntactic.cpp"
    break;

  case 40: // statement: DIRECTIVE_INFORM expression "," STRING "," expression_list
#line 1075 "syntactic.y"
        {
		/* TODO - parameters */
		DestroyExpressionList(&yystack_[0].value.as < ExpressionList > ());

		statement->emplace<StatementInform>(yystack_[4].value.as < Expression > (), yystack_[2].value.as < String > ());
	}
#line 1124 "syntactic.cpp"
    break;

  case 41: // statement: DIRECTIVE_FAIL
#line 1082 "syntactic.y"
        {
		statement->emplace<StatementFail>();
	}
#line 1132 "syntactic.cpp"
    break;

  case 42: // statement: DIRECTIVE_END
#line 1086 "syntactic.y"
        {
		statement->emplace<StatementEnd>();
	}
#line 1140 "syntactic.cpp"
    break;

  case 43: // statement: DIRECTIVE_RS size expression
#line 1090 "syntactic.y"
        {
		statement->emplace<StatementRS>(yystack_[1].value.as < Size > (), yystack_[0].value.as < Expression > ());
	}
#line 1148 "syntactic.cpp"
    break;

  case 44: // statement: DIRECTIVE_RSSET expression
#line 1094 "syntactic.y"
        {
		statement->emplace<StatementRSSet>(yystack_[0].value.as < Expression > ());
	}
#line 1156 "syntactic.cpp"
    break;

  case 45: // statement: DIRECTIVE_RSRESET expression
#line 1098 "syntactic.y"
        {
		/* As mentioned in S.N. 68k's manual, this odd hack
		   is necessary for compatibility with other assemblers. */
		statement->emplace<StatementRSSet>(yystack_[0].value.as < Expression > ());
	}
#line 1166 "syntactic.cpp"
    break;

  case 46: // statement: DIRECTIVE_RSRESET
#line 1104 "syntactic.y"
        {
		statement->emplace<StatementRSReset>();
	}
#line 1174 "syntactic.cpp"
    break;

  case 47: // statement: DIRECTIVE_OBJ expression
#line 1108 "syntactic.y"
        {
		statement->emplace<StatementObj>(yystack_[0].value.as < Expression > ());
	}
#line 1182 "syntactic.cpp"
    break;

  case 48: // statement: DIRECTIVE_OBJEND
#line 1112 "syntactic.y"
        {
		statement->emplace<StatementObjEnd>();
	}
#line 1190 "syntactic.cpp"
    break;

  case 49: // statement: DIRECTIVE_ORG expression
#line 1116 "syntactic.y"
        {
		statement->emplace<StatementOrg>(yystack_[0].value.as < Expression > ());
	}
#line 1198 "syntactic.cpp"
    break;

  case 50: // statement: DIRECTIVE_PUSHO
#line 1120 "syntactic.y"
        {
		statement->emplace<StatementPushO>();
	}
#line 1206 "syntactic.cpp"
    break;

  case 51: // statement: DIRECTIVE_POPO
#line 1124 "syntactic.y"
        {
		statement->emplace<StatementPopO>();
	}
#line 1214 "syntactic.cpp"
    break;

  case 52: // statement: DIRECTIVE_OPT option_list
#line 1128 "syntactic.y"
        {
		statement->emplace<StatementOpt>(yystack_[0].value.as < IdentifierList > ());
	}
#line 1222 "syntactic.cpp"
    break;

  case 53: // statement: DIRECTIVE_PUSHP STRING
#line 1132 "syntactic.y"
        {
		statement->emplace<StatementPushP>(yystack_[0].value.as < String > ());
	}
#line 1230 "syntactic.cpp"
    break;

  case 54: // statement: DIRECTIVE_POPP IDENTIFIER
#line 1136 "syntactic.y"
        {
		statement->emplace<StatementPopP>(yystack_[0].value.as < String > ());
	}
#line 1238 "syntactic.cpp"
    break;

  case 55: // statement: DIRECTIVE_SHIFT
#line 1140 "syntactic.y"
        {
		statement->emplace<StatementShift>();
	}
#line 1246 "syntactic.cpp"
    break;

  case 56: // statement: DIRECTIVE_MEXIT
#line 1144 "syntactic.y"
        {
		statement->emplace<StatementMExit>();
	}
#line 1254 "syntactic.cpp"
    break;

  case 57: // statement: DIRECTIVE_LOCAL identifier_list
#line 1148 "syntactic.y"
        {
		statement->emplace<StatementLocal>(yystack_[0].value.as < IdentifierList > ());
	}
#line 1262 "syntactic.cpp"
    break;

  case 58: // statement: DIRECTIVE_LIST
#line 1152 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->emplace<StatementEmpty>();
	}
#line 1271 "syntactic.cpp"
    break;

  case 59: // statement: DIRECTIVE_LIST "+"
#line 1157 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->emplace<StatementEmpty>();
	}
#line 1280 "syntactic.cpp"
    break;

  case 60: // statement: DIRECTIVE_LIST "-"
#line 1162 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->emplace<StatementEmpty>();
	}
#line 1289 "syntactic.cpp"
    break;

  case 61: // statement: DIRECTIVE_NOLIST
#line 1167 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->emplace<StatementEmpty>();
	}
#line 1298 "syntactic.cpp"
    break;

  case 62: // statement: DIRECTIVE_PURGE IDENTIFIER
#line 1172 "syntactic.y"
        {
		statement->emplace<StatementPurge>(yystack_[0].value.as < String > ());
	}
#line 1306 "syntactic.cpp"
    break;

  case 63: // expression_list: expression
#line 1179 "syntactic.y"
        {
		ExpressionListNode *node = new ExpressionListNode;

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
#line 1327 "syntactic.cpp"
    break;

  case 64: // expression_list: expression_list "," expression
#line 1196 "syntactic.y"
        {
		ExpressionListNode *node = new ExpressionListNode;

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
#line 1356 "syntactic.cpp"
    break;

  case 65: // identifier_list: IDENTIFIER
#line 1224 "syntactic.y"
        {
		IdentifierListNode *node = new IdentifierListNode;

		if (node == NULL)
		{
			String_Destroy(yystack_[0].value.as < String > ());
			YYNOMEM;
		}
		else
		{
			node->identifier = yystack_[0].value.as < String > ();
			node->next = NULL;
		}

		yylhs.value.as < IdentifierList > ().head = yylhs.value.as < IdentifierList > ().tail = node;
	}
#line 1377 "syntactic.cpp"
    break;

  case 66: // identifier_list: identifier_list "," IDENTIFIER
#line 1241 "syntactic.y"
        {
		IdentifierListNode *node = new IdentifierListNode;

		yylhs.value.as < IdentifierList > () = yystack_[2].value.as < IdentifierList > ();

		if (node == NULL)
		{
			DestroyIdentifierList(&yystack_[2].value.as < IdentifierList > ());
			String_Destroy(yystack_[0].value.as < String > ());
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
#line 1406 "syntactic.cpp"
    break;

  case 67: // option_list: OPTION
#line 1269 "syntactic.y"
        {
		IdentifierListNode *node = new IdentifierListNode;

		if (node == NULL)
		{
			String_Destroy(yystack_[0].value.as < String > ());
			YYNOMEM;
		}
		else
		{
			node->identifier = yystack_[0].value.as < String > ();
			node->next = NULL;
		}

		yylhs.value.as < IdentifierList > ().head = yylhs.value.as < IdentifierList > ().tail = node;
	}
#line 1427 "syntactic.cpp"
    break;

  case 68: // option_list: option_list "," OPTION
#line 1286 "syntactic.y"
        {
		IdentifierListNode *node = new IdentifierListNode;

		yylhs.value.as < IdentifierList > () = yystack_[2].value.as < IdentifierList > ();

		if (node == NULL)
		{
			DestroyIdentifierList(&yystack_[2].value.as < IdentifierList > ());
			String_Destroy(yystack_[0].value.as < String > ());
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
#line 1456 "syntactic.cpp"
    break;

  case 69: // instruction: full_opcode
#line 1314 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = yystack_[0].value.as < Opcode > ();
		yylhs.value.as < StatementInstruction > ().operands[0].type = OPERAND_NONE;
		yylhs.value.as < StatementInstruction > ().operands[1].type = OPERAND_NONE;
	}
#line 1466 "syntactic.cpp"
    break;

  case 70: // instruction: full_opcode operand
#line 1320 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = yystack_[1].value.as < Opcode > ();
		yylhs.value.as < StatementInstruction > ().operands[0] = yystack_[0].value.as < Operand > ();
		yylhs.value.as < StatementInstruction > ().operands[1].type = OPERAND_NONE;
	}
#line 1476 "syntactic.cpp"
    break;

  case 71: // instruction: full_opcode operand "," operand
#line 1326 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = yystack_[3].value.as < Opcode > ();
		yylhs.value.as < StatementInstruction > ().operands[0] = yystack_[2].value.as < Operand > ();
		yylhs.value.as < StatementInstruction > ().operands[1] = yystack_[0].value.as < Operand > ();
	}
#line 1486 "syntactic.cpp"
    break;

  case 72: // full_opcode: opcode
#line 1335 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = yystack_[0].value.as < Opcode > ();
		yylhs.value.as < Opcode > ().size = SIZE_UNDEFINED;
	}
#line 1495 "syntactic.cpp"
    break;

  case 73: // full_opcode: opcode "."
#line 1340 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = yystack_[1].value.as < Opcode > ();
		yylhs.value.as < Opcode > ().size = SIZE_UNDEFINED;
		m68kasm_warning_pedantic(scanner, statement, "Opcode has a dot but no size; either remove the dot or add an explicit size.");
	}
#line 1505 "syntactic.cpp"
    break;

  case 74: // full_opcode: opcode size
#line 1346 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = yystack_[1].value.as < Opcode > ();
		yylhs.value.as < Opcode > ().size = yystack_[0].value.as < Size > ();
	}
#line 1514 "syntactic.cpp"
    break;

  case 75: // opcode: DIRECTIVE_ORI
#line 1354 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ORI;
	}
#line 1522 "syntactic.cpp"
    break;

  case 76: // opcode: DIRECTIVE_ANDI
#line 1358 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ANDI;
	}
#line 1530 "syntactic.cpp"
    break;

  case 77: // opcode: DIRECTIVE_SUBI
#line 1362 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBI;
	}
#line 1538 "syntactic.cpp"
    break;

  case 78: // opcode: DIRECTIVE_ADDI
#line 1366 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDI;
	}
#line 1546 "syntactic.cpp"
    break;

  case 79: // opcode: DIRECTIVE_EORI
#line 1370 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EORI;
	}
#line 1554 "syntactic.cpp"
    break;

  case 80: // opcode: DIRECTIVE_CMPI
#line 1374 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPI;
	}
#line 1562 "syntactic.cpp"
    break;

  case 81: // opcode: DIRECTIVE_BTST
#line 1378 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BTST_STATIC;
	}
#line 1570 "syntactic.cpp"
    break;

  case 82: // opcode: DIRECTIVE_BCHG
#line 1382 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BCHG_STATIC;
	}
#line 1578 "syntactic.cpp"
    break;

  case 83: // opcode: DIRECTIVE_BCLR
#line 1386 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BCLR_STATIC;
	}
#line 1586 "syntactic.cpp"
    break;

  case 84: // opcode: DIRECTIVE_BSET
#line 1390 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BSET_STATIC;
	}
#line 1594 "syntactic.cpp"
    break;

  case 85: // opcode: DIRECTIVE_MOVEP
#line 1394 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEP_TO_REG;
	}
#line 1602 "syntactic.cpp"
    break;

  case 86: // opcode: DIRECTIVE_MOVEA
#line 1398 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEA;
	}
#line 1610 "syntactic.cpp"
    break;

  case 87: // opcode: DIRECTIVE_MOVE
#line 1402 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVE;
	}
#line 1618 "syntactic.cpp"
    break;

  case 88: // opcode: DIRECTIVE_NEGX
#line 1406 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NEGX;
	}
#line 1626 "syntactic.cpp"
    break;

  case 89: // opcode: DIRECTIVE_CLR
#line 1410 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CLR;
	}
#line 1634 "syntactic.cpp"
    break;

  case 90: // opcode: DIRECTIVE_NEG
#line 1414 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NEG;
	}
#line 1642 "syntactic.cpp"
    break;

  case 91: // opcode: DIRECTIVE_NOT
#line 1418 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NOT;
	}
#line 1650 "syntactic.cpp"
    break;

  case 92: // opcode: DIRECTIVE_EXT
#line 1422 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EXT;
	}
#line 1658 "syntactic.cpp"
    break;

  case 93: // opcode: DIRECTIVE_NBCD
#line 1426 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NBCD;
	}
#line 1666 "syntactic.cpp"
    break;

  case 94: // opcode: DIRECTIVE_SWAP
#line 1430 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SWAP;
	}
#line 1674 "syntactic.cpp"
    break;

  case 95: // opcode: DIRECTIVE_PEA
#line 1434 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_PEA;
	}
#line 1682 "syntactic.cpp"
    break;

  case 96: // opcode: DIRECTIVE_ILLEGAL
#line 1438 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ILLEGAL;
	}
#line 1690 "syntactic.cpp"
    break;

  case 97: // opcode: DIRECTIVE_TAS
#line 1442 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TAS;
	}
#line 1698 "syntactic.cpp"
    break;

  case 98: // opcode: DIRECTIVE_TST
#line 1446 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TST;
	}
#line 1706 "syntactic.cpp"
    break;

  case 99: // opcode: DIRECTIVE_TRAP
#line 1450 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TRAP;
	}
#line 1714 "syntactic.cpp"
    break;

  case 100: // opcode: DIRECTIVE_LINK
#line 1454 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LINK;
	}
#line 1722 "syntactic.cpp"
    break;

  case 101: // opcode: DIRECTIVE_UNLK
#line 1458 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_UNLK;
	}
#line 1730 "syntactic.cpp"
    break;

  case 102: // opcode: DIRECTIVE_RESET
#line 1462 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RESET;
	}
#line 1738 "syntactic.cpp"
    break;

  case 103: // opcode: DIRECTIVE_NOP
#line 1466 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NOP;
	}
#line 1746 "syntactic.cpp"
    break;

  case 104: // opcode: DIRECTIVE_STOP
#line 1470 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_STOP;
	}
#line 1754 "syntactic.cpp"
    break;

  case 105: // opcode: DIRECTIVE_RTE
#line 1474 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTE;
	}
#line 1762 "syntactic.cpp"
    break;

  case 106: // opcode: DIRECTIVE_RTS
#line 1478 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTS;
	}
#line 1770 "syntactic.cpp"
    break;

  case 107: // opcode: DIRECTIVE_TRAPV
#line 1482 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TRAPV;
	}
#line 1778 "syntactic.cpp"
    break;

  case 108: // opcode: DIRECTIVE_RTR
#line 1486 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTR;
	}
#line 1786 "syntactic.cpp"
    break;

  case 109: // opcode: DIRECTIVE_JSR
#line 1490 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_JSR;
	}
#line 1794 "syntactic.cpp"
    break;

  case 110: // opcode: DIRECTIVE_JMP
#line 1494 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_JMP;
	}
#line 1802 "syntactic.cpp"
    break;

  case 111: // opcode: DIRECTIVE_MOVEM
#line 1498 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEM_TO_REGS;
	}
#line 1810 "syntactic.cpp"
    break;

  case 112: // opcode: DIRECTIVE_LEA
#line 1502 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LEA;
	}
#line 1818 "syntactic.cpp"
    break;

  case 113: // opcode: DIRECTIVE_CHK
#line 1506 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CHK;
	}
#line 1826 "syntactic.cpp"
    break;

  case 114: // opcode: DIRECTIVE_ADDQ
#line 1510 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDQ;
	}
#line 1834 "syntactic.cpp"
    break;

  case 115: // opcode: DIRECTIVE_SUBQ
#line 1514 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBQ;
	}
#line 1842 "syntactic.cpp"
    break;

  case 116: // opcode: DIRECTIVE_ST
#line 1518 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_TRUE;
	}
#line 1851 "syntactic.cpp"
    break;

  case 117: // opcode: DIRECTIVE_SF
#line 1523 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_FALSE;
	}
#line 1860 "syntactic.cpp"
    break;

  case 118: // opcode: DIRECTIVE_SHI
#line 1528 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 1869 "syntactic.cpp"
    break;

  case 119: // opcode: DIRECTIVE_SLS
#line 1533 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 1878 "syntactic.cpp"
    break;

  case 120: // opcode: DIRECTIVE_SCC
#line 1538 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 1887 "syntactic.cpp"
    break;

  case 121: // opcode: DIRECTIVE_SCS
#line 1543 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 1896 "syntactic.cpp"
    break;

  case 122: // opcode: DIRECTIVE_SNE
#line 1548 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 1905 "syntactic.cpp"
    break;

  case 123: // opcode: DIRECTIVE_SEQ
#line 1553 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 1914 "syntactic.cpp"
    break;

  case 124: // opcode: DIRECTIVE_SVC
#line 1558 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 1923 "syntactic.cpp"
    break;

  case 125: // opcode: DIRECTIVE_SVS
#line 1563 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 1932 "syntactic.cpp"
    break;

  case 126: // opcode: DIRECTIVE_SPL
#line 1568 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 1941 "syntactic.cpp"
    break;

  case 127: // opcode: DIRECTIVE_SMI
#line 1573 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 1950 "syntactic.cpp"
    break;

  case 128: // opcode: DIRECTIVE_SGE
#line 1578 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 1959 "syntactic.cpp"
    break;

  case 129: // opcode: DIRECTIVE_SLT
#line 1583 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 1968 "syntactic.cpp"
    break;

  case 130: // opcode: DIRECTIVE_SGT
#line 1588 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 1977 "syntactic.cpp"
    break;

  case 131: // opcode: DIRECTIVE_SLE
#line 1593 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 1986 "syntactic.cpp"
    break;

  case 132: // opcode: DIRECTIVE_DBT
#line 1598 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_TRUE;
	}
#line 1995 "syntactic.cpp"
    break;

  case 133: // opcode: DIRECTIVE_DBF
#line 1603 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_FALSE;
	}
#line 2004 "syntactic.cpp"
    break;

  case 134: // opcode: DIRECTIVE_DBHI
#line 1608 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 2013 "syntactic.cpp"
    break;

  case 135: // opcode: DIRECTIVE_DBLS
#line 1613 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 2022 "syntactic.cpp"
    break;

  case 136: // opcode: DIRECTIVE_DBCC
#line 1618 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 2031 "syntactic.cpp"
    break;

  case 137: // opcode: DIRECTIVE_DBCS
#line 1623 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 2040 "syntactic.cpp"
    break;

  case 138: // opcode: DIRECTIVE_DBNE
#line 1628 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 2049 "syntactic.cpp"
    break;

  case 139: // opcode: DIRECTIVE_DBEQ
#line 1633 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 2058 "syntactic.cpp"
    break;

  case 140: // opcode: DIRECTIVE_DBVC
#line 1638 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 2067 "syntactic.cpp"
    break;

  case 141: // opcode: DIRECTIVE_DBVS
#line 1643 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 2076 "syntactic.cpp"
    break;

  case 142: // opcode: DIRECTIVE_DBPL
#line 1648 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 2085 "syntactic.cpp"
    break;

  case 143: // opcode: DIRECTIVE_DBMI
#line 1653 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 2094 "syntactic.cpp"
    break;

  case 144: // opcode: DIRECTIVE_DBGE
#line 1658 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2103 "syntactic.cpp"
    break;

  case 145: // opcode: DIRECTIVE_DBLT
#line 1663 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 2112 "syntactic.cpp"
    break;

  case 146: // opcode: DIRECTIVE_DBGT
#line 1668 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 2121 "syntactic.cpp"
    break;

  case 147: // opcode: DIRECTIVE_DBLE
#line 1673 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2130 "syntactic.cpp"
    break;

  case 148: // opcode: DIRECTIVE_BRA
#line 1678 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BRA;
	}
#line 2138 "syntactic.cpp"
    break;

  case 149: // opcode: DIRECTIVE_BSR
#line 1682 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BSR;
	}
#line 2146 "syntactic.cpp"
    break;

  case 150: // opcode: DIRECTIVE_BHI
#line 1686 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 2155 "syntactic.cpp"
    break;

  case 151: // opcode: DIRECTIVE_BLS
#line 1691 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 2164 "syntactic.cpp"
    break;

  case 152: // opcode: DIRECTIVE_BCC
#line 1696 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 2173 "syntactic.cpp"
    break;

  case 153: // opcode: DIRECTIVE_BCS
#line 1701 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 2182 "syntactic.cpp"
    break;

  case 154: // opcode: DIRECTIVE_BNE
#line 1706 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 2191 "syntactic.cpp"
    break;

  case 155: // opcode: DIRECTIVE_BEQ
#line 1711 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 2200 "syntactic.cpp"
    break;

  case 156: // opcode: DIRECTIVE_BVC
#line 1716 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 2209 "syntactic.cpp"
    break;

  case 157: // opcode: DIRECTIVE_BVS
#line 1721 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 2218 "syntactic.cpp"
    break;

  case 158: // opcode: DIRECTIVE_BPL
#line 1726 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 2227 "syntactic.cpp"
    break;

  case 159: // opcode: DIRECTIVE_BMI
#line 1731 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 2236 "syntactic.cpp"
    break;

  case 160: // opcode: DIRECTIVE_BGE
#line 1736 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2245 "syntactic.cpp"
    break;

  case 161: // opcode: DIRECTIVE_BLT
#line 1741 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 2254 "syntactic.cpp"
    break;

  case 162: // opcode: DIRECTIVE_BGT
#line 1746 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 2263 "syntactic.cpp"
    break;

  case 163: // opcode: DIRECTIVE_BLE
#line 1751 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2272 "syntactic.cpp"
    break;

  case 164: // opcode: DIRECTIVE_MOVEQ
#line 1756 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEQ;
	}
#line 2280 "syntactic.cpp"
    break;

  case 165: // opcode: DIRECTIVE_DIVU
#line 1760 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DIVU;
	}
#line 2288 "syntactic.cpp"
    break;

  case 166: // opcode: DIRECTIVE_DIVS
#line 1764 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DIVS;
	}
#line 2296 "syntactic.cpp"
    break;

  case 167: // opcode: DIRECTIVE_SBCD
#line 1768 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SBCD_DATA_REGS;
	}
#line 2304 "syntactic.cpp"
    break;

  case 168: // opcode: DIRECTIVE_OR
#line 1772 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_OR_TO_REG;
	}
#line 2312 "syntactic.cpp"
    break;

  case 169: // opcode: DIRECTIVE_SUB
#line 1776 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUB_TO_REG;
	}
#line 2320 "syntactic.cpp"
    break;

  case 170: // opcode: DIRECTIVE_SUBX
#line 1780 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBX_DATA_REGS;
	}
#line 2328 "syntactic.cpp"
    break;

  case 171: // opcode: DIRECTIVE_SUBA
#line 1784 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBA;
	}
#line 2336 "syntactic.cpp"
    break;

  case 172: // opcode: DIRECTIVE_EOR
#line 1788 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EOR;
	}
#line 2344 "syntactic.cpp"
    break;

  case 173: // opcode: DIRECTIVE_CMPM
#line 1792 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPM;
	}
#line 2352 "syntactic.cpp"
    break;

  case 174: // opcode: DIRECTIVE_CMP
#line 1796 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMP;
	}
#line 2360 "syntactic.cpp"
    break;

  case 175: // opcode: DIRECTIVE_CMPA
#line 1800 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPA;
	}
#line 2368 "syntactic.cpp"
    break;

  case 176: // opcode: DIRECTIVE_MULU
#line 1804 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MULU;
	}
#line 2376 "syntactic.cpp"
    break;

  case 177: // opcode: DIRECTIVE_MULS
#line 1808 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MULS;
	}
#line 2384 "syntactic.cpp"
    break;

  case 178: // opcode: DIRECTIVE_ABCD
#line 1812 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ABCD_DATA_REGS;
	}
#line 2392 "syntactic.cpp"
    break;

  case 179: // opcode: DIRECTIVE_EXG
#line 1816 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EXG;
	}
#line 2400 "syntactic.cpp"
    break;

  case 180: // opcode: DIRECTIVE_AND
#line 1820 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_AND_TO_REG;
	}
#line 2408 "syntactic.cpp"
    break;

  case 181: // opcode: DIRECTIVE_ADD
#line 1824 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADD_TO_REG;
	}
#line 2416 "syntactic.cpp"
    break;

  case 182: // opcode: DIRECTIVE_ADDX
#line 1828 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDX_DATA_REGS;
	}
#line 2424 "syntactic.cpp"
    break;

  case 183: // opcode: DIRECTIVE_ADDA
#line 1832 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDA;
	}
#line 2432 "syntactic.cpp"
    break;

  case 184: // opcode: DIRECTIVE_ASL
#line 1836 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ASL_STATIC;
	}
#line 2440 "syntactic.cpp"
    break;

  case 185: // opcode: DIRECTIVE_ASR
#line 1840 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ASR_STATIC;
	}
#line 2448 "syntactic.cpp"
    break;

  case 186: // opcode: DIRECTIVE_LSL
#line 1844 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LSL_STATIC;
	}
#line 2456 "syntactic.cpp"
    break;

  case 187: // opcode: DIRECTIVE_LSR
#line 1848 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LSR_STATIC;
	}
#line 2464 "syntactic.cpp"
    break;

  case 188: // opcode: DIRECTIVE_ROXL
#line 1852 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROXL_STATIC;
	}
#line 2472 "syntactic.cpp"
    break;

  case 189: // opcode: DIRECTIVE_ROXR
#line 1856 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROXR_STATIC;
	}
#line 2480 "syntactic.cpp"
    break;

  case 190: // opcode: DIRECTIVE_ROL
#line 1860 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROL_STATIC;
	}
#line 2488 "syntactic.cpp"
    break;

  case 191: // opcode: DIRECTIVE_ROR
#line 1864 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROR_STATIC;
	}
#line 2496 "syntactic.cpp"
    break;

  case 192: // size: SIZE_BYTE
#line 1871 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_BYTE;
	}
#line 2504 "syntactic.cpp"
    break;

  case 193: // size: SIZE_SHORT
#line 1875 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_SHORT;
	}
#line 2512 "syntactic.cpp"
    break;

  case 194: // size: SIZE_WORD
#line 1879 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_WORD;
	}
#line 2520 "syntactic.cpp"
    break;

  case 195: // size: SIZE_LONGWORD
#line 1883 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_LONGWORD;
	}
#line 2528 "syntactic.cpp"
    break;

  case 196: // operand: "(" ADDRESS_REGISTER ")"
#line 1891 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT;
		yylhs.value.as < Operand > ().main_register = yystack_[1].value.as < unsigned long > ();
	}
#line 2537 "syntactic.cpp"
    break;

  case 197: // operand: "(" ADDRESS_REGISTER ")" "+"
#line 1896 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
		yylhs.value.as < Operand > ().main_register = yystack_[2].value.as < unsigned long > ();
	}
#line 2546 "syntactic.cpp"
    break;

  case 198: // operand: "-" "(" ADDRESS_REGISTER ")"
#line 1901 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
		yylhs.value.as < Operand > ().main_register = yystack_[1].value.as < unsigned long > ();
	}
#line 2555 "syntactic.cpp"
    break;

  case 199: // operand: expression "(" ADDRESS_REGISTER ")"
#line 1906 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[3].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[1].value.as < unsigned long > ();
	}
#line 2565 "syntactic.cpp"
    break;

  case 200: // operand: "(" expression "," ADDRESS_REGISTER ")"
#line 1912 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[3].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[1].value.as < unsigned long > ();
	}
#line 2575 "syntactic.cpp"
    break;

  case 201: // operand: "(" ADDRESS_REGISTER "," expression ")"
#line 1918 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[1].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[3].value.as < unsigned long > ();
	}
#line 2585 "syntactic.cpp"
    break;

  case 202: // operand: "(" ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1924 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().main_register = yystack_[4].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 != 0;
	}
#line 2598 "syntactic.cpp"
    break;

  case 203: // operand: expression "(" ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1933 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[6].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[4].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 !=0;
	}
#line 2611 "syntactic.cpp"
    break;

  case 204: // operand: "(" expression "," ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1942 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[6].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[4].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 !=0;
	}
#line 2624 "syntactic.cpp"
    break;

  case 205: // operand: "(" ADDRESS_REGISTER "," data_or_address_register ")"
#line 1951 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().main_register = yystack_[3].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 != 0;
	}
#line 2638 "syntactic.cpp"
    break;

  case 206: // operand: expression "(" ADDRESS_REGISTER "," data_or_address_register ")"
#line 1961 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[5].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[3].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 !=0;
	}
#line 2652 "syntactic.cpp"
    break;

  case 207: // operand: "(" expression "," ADDRESS_REGISTER "," data_or_address_register ")"
#line 1971 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[5].value.as < Expression > ();
		yylhs.value.as < Operand > ().main_register = yystack_[3].value.as < unsigned long > ();
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 !=0;
	}
#line 2666 "syntactic.cpp"
    break;

  case 208: // operand: expression "(" PROGRAM_COUNTER ")"
#line 1981 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[3].value.as < Expression > ();
	}
#line 2675 "syntactic.cpp"
    break;

  case 209: // operand: "(" expression "," PROGRAM_COUNTER ")"
#line 1986 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[3].value.as < Expression > ();
	}
#line 2684 "syntactic.cpp"
    break;

  case 210: // operand: "(" PROGRAM_COUNTER "," expression ")"
#line 1991 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = yystack_[1].value.as < Expression > ();
	}
#line 2693 "syntactic.cpp"
    break;

  case 211: // operand: "(" PROGRAM_COUNTER "," data_or_address_register size ")"
#line 1996 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 != 0;
	}
#line 2705 "syntactic.cpp"
    break;

  case 212: // operand: expression "(" PROGRAM_COUNTER "," data_or_address_register size ")"
#line 2004 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[6].value.as < Expression > ();
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 != 0;
	}
#line 2717 "syntactic.cpp"
    break;

  case 213: // operand: "(" expression "," PROGRAM_COUNTER "," data_or_address_register size ")"
#line 2012 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[6].value.as < Expression > ();
		yylhs.value.as < Operand > ().index_register = yystack_[2].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = yystack_[1].value.as < Size > ();
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[2].value.as < unsigned long > () / 8 != 0;
	}
#line 2729 "syntactic.cpp"
    break;

  case 214: // operand: "(" PROGRAM_COUNTER "," data_or_address_register ")"
#line 2020 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 != 0;
	}
#line 2742 "syntactic.cpp"
    break;

  case 215: // operand: expression "(" PROGRAM_COUNTER "," data_or_address_register ")"
#line 2029 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[5].value.as < Expression > ();
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 != 0;
	}
#line 2755 "syntactic.cpp"
    break;

  case 216: // operand: "(" expression "," PROGRAM_COUNTER "," data_or_address_register ")"
#line 2038 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = yystack_[5].value.as < Expression > ();
		yylhs.value.as < Operand > ().index_register = yystack_[1].value.as < unsigned long > () % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = yystack_[1].value.as < unsigned long > () / 8 != 0;
	}
#line 2768 "syntactic.cpp"
    break;

  case 217: // operand: "#" expression
#line 2048 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_LITERAL;
		yylhs.value.as < Operand > ().literal = yystack_[0].value.as < Expression > ();
	}
#line 2777 "syntactic.cpp"
    break;

  case 218: // operand: STATUS_REGISTER
#line 2064 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_STATUS_REGISTER;
	}
#line 2785 "syntactic.cpp"
    break;

  case 219: // operand: CONDITION_CODE_REGISTER
#line 2068 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_CONDITION_CODE_REGISTER;
	}
#line 2793 "syntactic.cpp"
    break;

  case 220: // operand: USER_STACK_POINTER_REGISTER
#line 2072 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_USER_STACK_POINTER_REGISTER;
	}
#line 2801 "syntactic.cpp"
    break;

  case 221: // operand: expression
#line 2077 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS;
		yylhs.value.as < Operand > ().literal = yystack_[0].value.as < Expression > ();
		yylhs.value.as < Operand > ().size = SIZE_UNDEFINED;
	}
#line 2811 "syntactic.cpp"
    break;

  case 222: // operand: expression size
#line 2083 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_ABSOLUTE;
		yylhs.value.as < Operand > ().literal = yystack_[1].value.as < Expression > ();
		yylhs.value.as < Operand > ().size = yystack_[0].value.as < Size > ();
	}
#line 2821 "syntactic.cpp"
    break;

  case 223: // operand: register_list
#line 2090 "syntactic.y"
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
#line 2853 "syntactic.cpp"
    break;

  case 224: // register_list: register_span
#line 2121 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = yystack_[0].value.as < unsigned long > ();
	}
#line 2861 "syntactic.cpp"
    break;

  case 225: // register_list: register_span "/" register_list
#line 2125 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = yystack_[2].value.as < unsigned long > () | yystack_[0].value.as < unsigned long > ();
	}
#line 2869 "syntactic.cpp"
    break;

  case 226: // register_span: data_or_address_register
#line 2132 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 1 << yystack_[0].value.as < unsigned long > ();
	}
#line 2877 "syntactic.cpp"
    break;

  case 227: // register_span: data_or_address_register "-" data_or_address_register
#line 2136 "syntactic.y"
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
#line 2903 "syntactic.cpp"
    break;

  case 228: // data_or_address_register: DATA_REGISTER
#line 2161 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 0 + yystack_[0].value.as < unsigned long > ();
	}
#line 2911 "syntactic.cpp"
    break;

  case 229: // data_or_address_register: ADDRESS_REGISTER
#line 2165 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 8 + yystack_[0].value.as < unsigned long > ();
	}
#line 2919 "syntactic.cpp"
    break;

  case 230: // expression: expression1
#line 2178 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 2927 "syntactic.cpp"
    break;

  case 231: // expression: expression LOGICAL_AND expression1
#line 2183 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LOGICAL_AND, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 2936 "syntactic.cpp"
    break;

  case 232: // expression: expression LOGICAL_OR expression1
#line 2189 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LOGICAL_OR, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 2945 "syntactic.cpp"
    break;

  case 233: // expression1: expression2
#line 2197 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 2953 "syntactic.cpp"
    break;

  case 234: // expression1: expression1 "=" expression2
#line 2201 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::EQUALITY, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 2962 "syntactic.cpp"
    break;

  case 235: // expression1: expression1 EQUALITY expression2
#line 2206 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::EQUALITY, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 2971 "syntactic.cpp"
    break;

  case 236: // expression1: expression1 INEQUALITY expression2
#line 2211 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::INEQUALITY, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 2980 "syntactic.cpp"
    break;

  case 237: // expression2: expression3
#line 2219 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 2988 "syntactic.cpp"
    break;

  case 238: // expression2: expression2 "<" expression3
#line 2223 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LESS_THAN, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 2997 "syntactic.cpp"
    break;

  case 239: // expression2: expression2 LESS_OR_EQUAL expression3
#line 2228 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LESS_OR_EQUAL, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3006 "syntactic.cpp"
    break;

  case 240: // expression2: expression2 ">" expression3
#line 2233 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MORE_THAN, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3015 "syntactic.cpp"
    break;

  case 241: // expression2: expression2 MORE_OR_EQUAL expression3
#line 2238 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MORE_OR_EQUAL, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3024 "syntactic.cpp"
    break;

  case 242: // expression3: expression4
#line 2246 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3032 "syntactic.cpp"
    break;

  case 243: // expression3: expression3 "+" expression4
#line 2250 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::ADD, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3041 "syntactic.cpp"
    break;

  case 244: // expression3: expression3 "-" expression4
#line 2255 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::SUBTRACT, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3050 "syntactic.cpp"
    break;

  case 245: // expression4: expression5
#line 2263 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3058 "syntactic.cpp"
    break;

  case 246: // expression4: expression4 "*" expression5
#line 2267 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MULTIPLY, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3067 "syntactic.cpp"
    break;

  case 247: // expression4: expression4 "/" expression5
#line 2272 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::DIVIDE, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3076 "syntactic.cpp"
    break;

  case 248: // expression4: expression4 "%" expression5
#line 2277 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MODULO, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3085 "syntactic.cpp"
    break;

  case 249: // expression5: expression6
#line 2285 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3093 "syntactic.cpp"
    break;

  case 250: // expression5: expression5 "&" expression6
#line 2289 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_AND, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3102 "syntactic.cpp"
    break;

  case 251: // expression5: expression5 "!" expression6
#line 2294 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_OR, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3111 "syntactic.cpp"
    break;

  case 252: // expression5: expression5 "|" expression6
#line 2299 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_OR, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3120 "syntactic.cpp"
    break;

  case 253: // expression5: expression5 "^" expression6
#line 2304 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_XOR, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3129 "syntactic.cpp"
    break;

  case 254: // expression6: expression7
#line 2312 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3137 "syntactic.cpp"
    break;

  case 255: // expression6: expression6 LEFT_SHIFT expression7
#line 2316 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LEFT_SHIFT, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3146 "syntactic.cpp"
    break;

  case 256: // expression6: expression6 RIGHT_SHIFT expression7
#line 2321 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::RIGHT_SHIFT, &yystack_[2].value.as < Expression > (), &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3155 "syntactic.cpp"
    break;

  case 257: // expression7: expression8
#line 2329 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3163 "syntactic.cpp"
    break;

  case 258: // expression7: "+" expression7
#line 2333 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3171 "syntactic.cpp"
    break;

  case 259: // expression7: "-" expression7
#line 2337 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::NEGATE, &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3180 "syntactic.cpp"
    break;

  case 260: // expression7: "~" expression7
#line 2342 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::BITWISE_NOT, &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3189 "syntactic.cpp"
    break;

  case 261: // expression7: "!" expression7
#line 2348 "syntactic.y"
        {
		if (!DoExpression(&yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::LOGICAL_NOT, &yystack_[0].value.as < Expression > ()))
			YYNOMEM;
	}
#line 3198 "syntactic.cpp"
    break;

  case 262: // local_identifier: LOCAL_IDENTIFIER
#line 2355 "syntactic.y"
          { yylhs.value.as < String > () = yystack_[0].value.as < String > (); }
#line 3204 "syntactic.cpp"
    break;

  case 263: // local_identifier: SIZE_BYTE
#line 2357 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".b");
		if (!String_CreateCopyView(yylhs.value.as < String > (), literal))
			YYNOMEM;
	}
#line 3214 "syntactic.cpp"
    break;

  case 264: // local_identifier: SIZE_SHORT
#line 2363 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".s");
		if (!String_CreateCopyView(yylhs.value.as < String > (), literal))
			YYNOMEM;
	}
#line 3224 "syntactic.cpp"
    break;

  case 265: // local_identifier: SIZE_WORD
#line 2369 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".w");
		if (!String_CreateCopyView(yylhs.value.as < String > (), literal))
			YYNOMEM;
	}
#line 3234 "syntactic.cpp"
    break;

  case 266: // local_identifier: SIZE_LONGWORD
#line 2375 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".l");
		if (!String_CreateCopyView(yylhs.value.as < String > (), literal))
			YYNOMEM;
	}
#line 3244 "syntactic.cpp"
    break;

  case 267: // identifier: IDENTIFIER
#line 2383 "syntactic.y"
          { yylhs.value.as < String > () = yystack_[0].value.as < String > (); }
#line 3250 "syntactic.cpp"
    break;

  case 268: // identifier: local_identifier
#line 2384 "syntactic.y"
          { yylhs.value.as < String > () = yystack_[0].value.as < String > (); }
#line 3256 "syntactic.cpp"
    break;

  case 269: // identifier: identifier local_identifier
#line 2386 "syntactic.y"
        {
		const cc_bool success = String_CreateAppend(yylhs.value.as < String > (), yystack_[1].value.as < String > (), yystack_[0].value.as < String > ());
		String_Destroy(yystack_[1].value.as < String > ());
		String_Destroy(yystack_[0].value.as < String > ());

		if (!success)
			YYNOMEM;
	}
#line 3269 "syntactic.cpp"
    break;

  case 270: // expression8: NUMBER
#line 2398 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionNumber>(yystack_[0].value.as < unsigned long > ());
	}
#line 3277 "syntactic.cpp"
    break;

  case 271: // expression8: identifier
#line 2402 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionIdentifier>(yystack_[0].value.as < String > ());
	}
#line 3285 "syntactic.cpp"
    break;

  case 272: // expression8: string
#line 2406 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[0].value.as < Expression > ();
	}
#line 3293 "syntactic.cpp"
    break;

  case 273: // expression8: "*"
#line 2410 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionProgramCounterOfStatement>();
	}
#line 3301 "syntactic.cpp"
    break;

  case 274: // expression8: "@"
#line 2414 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionProgramCounterOfExpression>();
	}
#line 3309 "syntactic.cpp"
    break;

  case 275: // expression8: "(" expression ")"
#line 2418 "syntactic.y"
        {
		yylhs.value.as < Expression > () = yystack_[1].value.as < Expression > ();
	}
#line 3317 "syntactic.cpp"
    break;

  case 276: // expression8: STRLEN "(" STRING ")"
#line 2422 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionStrlen>(yystack_[1].value.as < String > ());
	}
#line 3325 "syntactic.cpp"
    break;

  case 277: // expression8: STRCMP "(" STRING "," STRING ")"
#line 2426 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionStrcmp>(yystack_[3].value.as < String > (), yystack_[1].value.as < String > ());
	}
#line 3333 "syntactic.cpp"
    break;

  case 278: // expression8: INSTR "(" expression "," expression ")"
#line 2430 "syntactic.y"
        {
		Expression *subexpressions = new Expression[3];

		if (subexpressions == nullptr)
			YYNOMEM;

		subexpressions[0].emplace<ExpressionNumber>(1);
		subexpressions[1] = std::move(yystack_[3].value.as < Expression > ());
		subexpressions[2] = std::move(yystack_[1].value.as < Expression > ());

		yylhs.value.as < Expression > ().emplace<ExpressionInstr>(subexpressions);
	}
#line 3350 "syntactic.cpp"
    break;

  case 279: // expression8: INSTR "(" expression "," expression "," expression ")"
#line 2443 "syntactic.y"
        {
		Expression *subexpressions = new Expression[3];

		if (subexpressions == nullptr)
			YYNOMEM;

		subexpressions[0] = std::move(yystack_[5].value.as < Expression > ());
		subexpressions[1] = std::move(yystack_[3].value.as < Expression > ());
		subexpressions[2] = std::move(yystack_[1].value.as < Expression > ());

		yylhs.value.as < Expression > ().emplace<ExpressionInstr>(subexpressions);
	}
#line 3367 "syntactic.cpp"
    break;

  case 280: // expression8: DEF "(" IDENTIFIER ")"
#line 2456 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionDef>(yystack_[1].value.as < String > ());
	}
#line 3375 "syntactic.cpp"
    break;

  case 281: // expression8: TYPE "(" IDENTIFIER ")"
#line 2460 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionTypeWithIdentifier>(yystack_[1].value.as < String > ());
	}
#line 3383 "syntactic.cpp"
    break;

  case 282: // expression8: TYPE "(" NUMBER ")"
#line 2464 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionTypeWithNumber>(yystack_[1].value.as < unsigned long > ());
	}
#line 3391 "syntactic.cpp"
    break;

  case 283: // expression8: FILESIZE "(" STRING ")"
#line 2468 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionFilesize>(yystack_[1].value.as < String > ());
	}
#line 3399 "syntactic.cpp"
    break;

  case 284: // expression8: OFFSET "(" expression ")"
#line 2472 "syntactic.y"
        {
		/* TODO: Make this actually do something besides pass the expression through. */
		yylhs.value.as < Expression > () = yystack_[1].value.as < Expression > ();
	}
#line 3408 "syntactic.cpp"
    break;

  case 285: // string: STRING
#line 2480 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionString>(yystack_[0].value.as < String > ());
	}
#line 3416 "syntactic.cpp"
    break;


#line 3420 "syntactic.cpp"

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


  const short parser::yypact_ninf_ = -305;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     292,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,   107,   107,
     107,   563,  -305,  -152,  -143,  -305,  -141,  -125,   563,  -151,
     338,   563,   563,   563,  -305,  -305,   563,  -305,  -305,   563,
     563,  -305,  -305,   107,   563,   563,   563,  -305,   563,  -305,
    -305,  -128,  -105,   -98,  -305,  -305,  -143,  -108,  -305,   -96,
     563,    83,  -305,   -50,   -62,  -305,  -305,  -305,  -305,   563,
     563,   563,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -101,   -72,   -60,   -51,   -49,   -43,   -20,   563,   563,   563,
    -305,  -305,   563,   563,   -23,   -93,  -149,   -35,     7,    69,
      32,  -305,  -305,   -74,  -305,  -305,  -305,   -83,   -22,   -22,
    -305,     2,   -23,  -305,  -305,   383,    -1,   -23,   -23,   -23,
     -23,    36,    61,   563,   -23,   -23,   -23,   -23,  -305,    19,
    -305,  -305,   -22,  -305,  -305,  -305,   -23,  -305,  -305,  -305,
    -305,  -305,  -305,    34,   608,   563,    25,  -305,     0,   -10,
      -5,  -305,  -305,    37,   -23,    64,   -23,    56,    63,   563,
      86,   120,    90,   563,  -170,  -305,  -305,  -305,  -305,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   563,   563,
     563,   563,   563,   563,   563,   563,   563,   563,   563,  -305,
    -143,    89,   428,    96,    70,   473,   563,   110,   -23,    74,
    -144,    97,   -13,   518,   -23,   -50,    77,    77,  -155,  -305,
     563,   563,   259,    99,    73,   281,   282,   290,   291,   -86,
    -305,   -93,   -93,  -149,  -149,  -149,   -35,   -35,   -35,   -35,
       7,     7,    69,    69,    69,    32,    32,    32,    32,  -305,
    -305,   -22,  -305,   563,    75,  -305,   317,   318,    87,   -23,
     301,  -305,   293,   299,   293,   -82,   302,  -305,  -305,  -305,
    -126,   -66,   -23,   -23,  -305,   327,   563,  -305,  -305,  -305,
    -305,  -305,   -23,   563,  -305,  -305,   334,   563,  -127,   -45,
    -305,  -123,   -11,   -12,    16,  -305,    77,  -305,    77,  -305,
     315,    -8,   -23,  -305,    37,  -305,   316,  -305,  -305,   319,
    -305,    77,  -305,    77,  -305,  -107,  -100,  -305,   563,  -305,
    -305,  -305,   -88,   -84,  -305,   320,  -305,   321,    13,  -305,
     322,  -305,   323,  -305,  -305,  -305,  -305,  -305
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
    -305,  -305,   135,  -122,  -305,  -305,  -305,  -305,  -119,   208,
     213,  -305,  -304,  -117,   203,     6,     8,   196,   189,   182,
    -183,   330,  -305,  -305,  -305
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
     170,   171,   209,   359,   194,   265,   266,   269,   270,   267,
     268,   212,   360,   216,   217,   218,   219,   206,   213,   220,
     214,   361,   221,   222,   223,   320,   206,   224,   225,   226,
     210,   227,   274,   275,   232,   165,   166,   167,   168,   165,
     166,   167,   168,   236,   228,   252,   211,   207,   378,   352,
     381,   353,   254,   255,   256,   165,   166,   167,   168,   276,
     277,   266,   165,   166,   167,   168,   230,   386,   395,   387,
     264,   231,   398,   235,   165,   166,   167,   168,   165,   166,
     167,   168,   405,   237,   406,   383,   271,   272,   414,   233,
     234,   269,   270,   257,   384,   416,   178,   412,   294,   413,
     165,   166,   167,   168,   339,   340,   298,   419,   273,   371,
     290,   421,   172,   173,   174,   175,   238,   239,   176,   177,
     178,   179,   258,   240,   241,   242,   302,   388,   304,   389,
     251,   309,   269,   270,   259,   180,   181,   182,   183,   184,
     185,   186,   314,   260,   243,   261,   319,   188,   244,   190,
     397,   262,   191,   245,   269,   270,   192,   165,   166,   167,
     168,   193,   278,   279,   269,   270,   269,   270,   341,   269,
     270,   291,   269,   270,   263,   344,   269,   270,   348,   349,
     355,   401,   320,   402,   400,   408,   264,   409,   307,   308,
     269,   270,   295,   362,   363,   292,   172,   173,   174,   175,
     306,   300,   176,   177,   178,   179,   280,   281,   425,   403,
     301,   404,   299,   269,   270,   287,   288,   282,   305,   180,
     181,   182,   183,   184,   185,   186,   372,   312,   187,   296,
     310,   188,   189,   190,   313,   379,   191,   382,   269,   270,
     192,   269,   270,   238,   239,   193,   351,   269,   270,   391,
     269,   270,   269,   270,   297,   315,   392,   311,   342,   396,
     254,   318,   399,   346,   269,   270,   366,   345,   373,   165,
     166,   167,   168,   283,   284,   285,   286,   323,   324,   325,
     376,   350,   326,   327,   328,   329,   415,   417,   316,   317,
     354,   418,   365,   420,   422,     1,     2,     3,     4,     5,
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
     156,   157,   158,   159,   364,   172,   173,   174,   175,   238,
     239,   176,   177,   178,   179,   335,   336,   337,   338,   332,
     333,   334,   321,   322,   330,   331,   367,   368,   180,   181,
     182,   183,   184,   185,   186,   369,   370,   187,   374,   375,
     188,   189,   190,   160,   377,   191,   380,   385,   390,   192,
     172,   173,   174,   175,   193,   393,   176,   177,   178,   179,
     407,   410,   394,   357,   411,   423,   424,   426,   427,   358,
       0,     0,     0,   180,   181,   182,   183,   184,   185,   186,
       0,   215,   187,   289,     0,   188,   189,   190,     0,     0,
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
     119,   120,   124,   307,   121,   188,   189,   177,   178,   192,
     193,   128,   167,   130,   131,   132,   133,   169,   169,   136,
     171,   176,   139,   140,   143,   195,   169,   144,   145,   146,
     171,   148,   181,   182,   156,   162,   163,   164,   165,   162,
     163,   164,   165,   160,   172,   164,   171,   199,   352,   193,
     354,   195,   169,   170,   171,   162,   163,   164,   165,   208,
     209,   244,   162,   163,   164,   165,   171,   193,   195,   195,
     187,   169,   195,   169,   162,   163,   164,   165,   162,   163,
     164,   165,   386,     0,   388,   167,   179,   180,   195,   197,
     198,   177,   178,   194,   176,   195,   170,   401,   215,   403,
     162,   163,   164,   165,   287,   288,   223,   195,   201,   195,
     193,   195,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   194,   173,   174,   175,   243,   193,   245,   195,
     192,   250,   177,   178,   194,   185,   186,   187,   188,   189,
     190,   191,   259,   194,   194,   194,   263,   197,   198,   199,
     195,   194,   202,   203,   177,   178,   206,   162,   163,   164,
     165,   211,   197,   198,   177,   178,   177,   178,   290,   177,
     178,   193,   177,   178,   194,   292,   177,   178,   295,   296,
     193,   193,   195,   195,   195,   193,   303,   195,   198,   194,
     177,   178,   193,   310,   311,   193,   162,   163,   164,   165,
     200,   167,   168,   169,   170,   171,   199,   200,   195,   193,
     176,   195,   193,   177,   178,   183,   184,   210,   193,   185,
     186,   187,   188,   189,   190,   191,   343,   171,   194,   193,
     193,   197,   198,   199,   171,   352,   202,   354,   177,   178,
     206,   177,   178,   166,   167,   211,   172,   177,   178,   366,
     177,   178,   177,   178,   193,   169,   373,   193,   169,   378,
     377,   171,   381,   193,   177,   178,   193,   171,   193,   162,
     163,   164,   165,   204,   205,   206,   207,   271,   272,   273,
     193,   171,   274,   275,   276,   277,   405,   406,   168,   169,
     193,   408,   193,   412,   413,     3,     4,     5,     6,     7,
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
     158,   159,   160,   161,   195,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   283,   284,   285,   286,   280,
     281,   282,   269,   270,   278,   279,   195,   195,   185,   186,
     187,   188,   189,   190,   191,   195,   195,   194,   171,   171,
     197,   198,   199,   201,   193,   202,   197,   195,   171,   206,
     162,   163,   164,   165,   211,   171,   168,   169,   170,   171,
     195,   195,   377,   305,   195,   195,   195,   195,   195,   306,
      -1,    -1,    -1,   185,   186,   187,   188,   189,   190,   191,
      -1,   193,   194,   203,    -1,   197,   198,   199,    -1,    -1,
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
     193,   193,   171,   171,   227,   169,   168,   169,   171,   227,
     195,   228,   228,   229,   229,   229,   230,   230,   230,   230,
     231,   231,   232,   232,   232,   233,   233,   233,   233,   234,
     234,   217,   169,   193,   227,   171,   193,   193,   227,   227,
     171,   172,   193,   195,   193,   193,   167,   223,   224,   226,
     167,   176,   227,   227,   195,   193,   193,   195,   195,   195,
     195,   195,   227,   193,   171,   171,   193,   193,   226,   227,
     197,   226,   227,   167,   176,   195,   193,   195,   193,   195,
     171,   227,   227,   171,   216,   195,   222,   195,   195,   222,
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
       0,   921,   921,   924,   928,   932,   936,   940,   944,   948,
     952,   956,   960,   964,   968,   972,   976,   980,   984,   989,
     994,   998,  1002,  1006,  1010,  1014,  1018,  1022,  1026,  1030,
    1034,  1038,  1042,  1046,  1050,  1054,  1058,  1062,  1066,  1070,
    1074,  1081,  1085,  1089,  1093,  1097,  1103,  1107,  1111,  1115,
    1119,  1123,  1127,  1131,  1135,  1139,  1143,  1147,  1151,  1156,
    1161,  1166,  1171,  1178,  1195,  1223,  1240,  1268,  1285,  1313,
    1319,  1325,  1334,  1339,  1345,  1353,  1357,  1361,  1365,  1369,
    1373,  1377,  1381,  1385,  1389,  1393,  1397,  1401,  1405,  1409,
    1413,  1417,  1421,  1425,  1429,  1433,  1437,  1441,  1445,  1449,
    1453,  1457,  1461,  1465,  1469,  1473,  1477,  1481,  1485,  1489,
    1493,  1497,  1501,  1505,  1509,  1513,  1517,  1522,  1527,  1532,
    1537,  1542,  1547,  1552,  1557,  1562,  1567,  1572,  1577,  1582,
    1587,  1592,  1597,  1602,  1607,  1612,  1617,  1622,  1627,  1632,
    1637,  1642,  1647,  1652,  1657,  1662,  1667,  1672,  1677,  1681,
    1685,  1690,  1695,  1700,  1705,  1710,  1715,  1720,  1725,  1730,
    1735,  1740,  1745,  1750,  1755,  1759,  1763,  1767,  1771,  1775,
    1779,  1783,  1787,  1791,  1795,  1799,  1803,  1807,  1811,  1815,
    1819,  1823,  1827,  1831,  1835,  1839,  1843,  1847,  1851,  1855,
    1859,  1863,  1870,  1874,  1878,  1882,  1890,  1895,  1900,  1905,
    1911,  1917,  1923,  1932,  1941,  1950,  1960,  1970,  1980,  1985,
    1990,  1995,  2003,  2011,  2019,  2028,  2037,  2047,  2063,  2067,
    2071,  2076,  2082,  2089,  2120,  2124,  2131,  2135,  2160,  2164,
    2177,  2182,  2188,  2196,  2200,  2205,  2210,  2218,  2222,  2227,
    2232,  2237,  2245,  2249,  2254,  2262,  2266,  2271,  2276,  2284,
    2288,  2293,  2298,  2303,  2311,  2315,  2320,  2328,  2332,  2336,
    2341,  2347,  2355,  2356,  2362,  2368,  2374,  2383,  2384,  2385,
    2397,  2401,  2405,  2409,  2413,  2417,  2421,  2425,  2429,  2442,
    2455,  2459,  2463,  2467,  2471,  2479
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
#line 4308 "syntactic.cpp"

#line 2484 "syntactic.y"


static cc_bool DoExpression(Expression *expression, ExpressionArithmetic1Operand::Type type, Expression *subexpression)
{
	Expression *subexpression_copy = new Expression;

	if (subexpression_copy == NULL)
	{
		DestroyExpression(subexpression);
		return cc_false;
	}

	*subexpression_copy = std::move(*subexpression);

	expression->emplace<ExpressionArithmetic1Operand>(type, subexpression_copy);

	return cc_true;
}

static cc_bool DoExpression(Expression *expression, ExpressionArithmetic2Operand::Type type, Expression *left_expression, Expression *right_expression)
{
	Expression *subexpressions = new Expression[2];

	if (subexpressions == NULL)
	{
		DestroyExpression(left_expression);
		DestroyExpression(right_expression);
		return cc_false;
	}

	subexpressions[0] = std::move(*left_expression);
	subexpressions[1] = std::move(*right_expression);

	expression->emplace<ExpressionArithmetic2Operand>(type, subexpressions);

	return cc_true;
}

void DestroyExpression(Expression *expression)
{
	std::visit(
		CC_VisitHelper{
			[](ExpressionInstr &expression)
			{
				DestroyExpression(&expression.subexpressions[0]);
				DestroyExpression(&expression.subexpressions[1]);
				DestroyExpression(&expression.subexpressions[2]);
				delete[] expression.subexpressions;
			},
			[](ExpressionArithmetic2Operand &expression)
			{
				DestroyExpression(&expression.subexpressions[0]);
				DestroyExpression(&expression.subexpressions[1]);
				delete[] expression.subexpressions;
			},
			[](ExpressionArithmetic1Operand &expression)
			{
				DestroyExpression(expression.subexpressions);
				delete expression.subexpressions;
			},
			[](ExpressionStrcmp &expression)
			{
				String_Destroy(expression.string_a);
				String_Destroy(expression.string_b);
			},
			[](ExpressionIdentifier &expression)
			{
				String_Destroy(expression.string);
			},
			[](ExpressionString &expression)
			{
				String_Destroy(expression.string);
			},
			[](ExpressionStrlen &expression)
			{
				String_Destroy(expression.string);
			},
			[](ExpressionDef &expression)
			{
				String_Destroy(expression.string);
			},
			[](ExpressionTypeWithIdentifier &expression)
			{
				String_Destroy(expression.string);
			},
			[](ExpressionFilesize &expression)
			{
				String_Destroy(expression.string);
			},
			[](ExpressionNumber &expression) {},
			[](ExpressionProgramCounterOfStatement &expression) {},
			[](ExpressionProgramCounterOfExpression &expression) {},
			[](ExpressionTypeWithNumber &expression) {}
		}, *expression
	);
}

void DestroyIdentifierList(IdentifierList *list)
{
	IdentifierListNode *node = list->head;

	while (node != NULL)
	{
		IdentifierListNode* const next_node = node->next;

		String_Destroy(node->identifier);

		delete node;

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

		delete node;

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
	std::visit(
		CC_VisitHelper{
			[](StatementEmpty &statement) {},
			[](StatementEndR &statement) {},
			[](StatementEndM &statement) {},
			[](StatementElse &statement) {},
			[](StatementEndC &statement) {},
			[](StatementEndW &statement) {},
			[](StatementEven &statement) {},
			[](StatementEnd &statement) {},
			[](StatementFail &statement) {},
			[](StatementRSReset &statement) {},
			[](StatementObjEnd &statement) {},
			[](StatementPushO &statement) {},
			[](StatementPopO &statement) {},
			[](StatementShift &statement) {},
			[](StatementMExit &statement) {},
			[](StatementInstruction &statement)
			{
				DestroyStatementInstruction(&statement);
			},
			[](StatementDc &statement)
			{
				DestroyExpressionList(&statement.values);
			},
			[](StatementDcb &statement)
			{
				DestroyExpression(&statement.repetitions);
				DestroyExpression(&statement.value);
			},
			[](StatementDs &statement)
			{
				DestroyExpression(&statement.length);
			},
			[](StatementInclude &statement)
			{
				String_Destroy(statement.path);
			},
			[](StatementIncbin &statement)
			{
				String_Destroy(statement.path);
				DestroyExpression(&statement.start);

				if (statement.has_length)
					DestroyExpression(&statement.length);
			},
			[](StatementRept &statement)
			{
				DestroyExpression(&statement.repetitions);
			},
			[](StatementMacro &statement)
			{
				DestroyIdentifierList(&statement.parameter_names);
			},
			[](StatementMacroS &statement)
			{
				DestroyIdentifierList(&statement.macro.parameter_names);
			},
			[](StatementEqu &statement)
			{
				DestroyExpression(&statement.expression);
			},
			[](StatementSet &statement)
			{
				DestroyExpression(&statement.expression);
			},
			[](StatementIf &statement)
			{
				DestroyExpression(&statement.expression);
			},
			[](StatementElseIf &statement)
			{
				DestroyExpression(&statement.expression);
			},
			[](StatementWhile &statement)
			{
				DestroyExpression(&statement.expression);
			},
			[](StatementRSSet &statement)
			{
				DestroyExpression(&statement.expression);
			},
			[](StatementObj &statement)
			{
				DestroyExpression(&statement.expression);
			},
			[](StatementOrg &statement)
			{
				DestroyExpression(&statement.expression);
			},
			[](StatementEquSString &statement)
			{
				String_Destroy(statement.string);
			},
			[](StatementEquSIdentifier &statement)
			{
				String_Destroy(statement.string);
			},
			[](StatementPushP &statement)
			{
				String_Destroy(statement.string);
			},
			[](StatementPopP &statement)
			{
				String_Destroy(statement.string);
			},
			[](StatementPurge &statement)
			{
				String_Destroy(statement.string);
			},
			[](StatementSubstr &statement)
			{
				DestroyExpression(&statement.start);
				DestroyExpression(&statement.end);
				String_Destroy(statement.string);
			},
			[](StatementCnop &statement)
			{
				DestroyExpression(&statement.offset);
				DestroyExpression(&statement.size_boundary);
			},
			[](StatementInform &statement)
			{
				DestroyExpression(&statement.severity);
				String_Destroy(statement.message);
			},
			[](StatementRS &statement)
			{
				DestroyExpression(&statement.length);
			},
			[](StatementOpt &statement)
			{
				DestroyIdentifierList(&statement.options);
			},
			[](StatementLocal &statement)
			{
				DestroyIdentifierList(&statement.identifiers);
			}
		}, *statement
	);
}
