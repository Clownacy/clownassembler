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
#line 648 "syntactic.y"


#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

YY_DECL;
void m68kasm_warning(void *scanner, Statement *statement, const char *message);
void m68kasm_warning_pedantic(void *scanner, Statement *statement, const char *message);
void m68kasm_error(void *scanner, Statement *statement, const char *message);

static void DestroyOperand(Operand *operand);
static void DestroyStatementInstruction(StatementInstruction *instruction);

void m68kasm::parser::error(const std::string &message)
{
	m68kasm_error(scanner, statement, message.c_str());
}


#line 70 "syntactic.cpp"


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
#line 144 "syntactic.cpp"

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
#line 906 "syntactic.y"
        {
		statement->emplace<StatementEmpty>();
	}
#line 814 "syntactic.cpp"
    break;

  case 3: // statement: instruction
#line 910 "syntactic.y"
        {
		statement->emplace<StatementInstruction>(YY_MOVE (yystack_[0].value.as < StatementInstruction > ()));
	}
#line 822 "syntactic.cpp"
    break;

  case 4: // statement: DIRECTIVE_DC size expression_list
#line 914 "syntactic.y"
        {
		statement->emplace<StatementDc>(YY_MOVE (yystack_[1].value.as < Size > ()), YY_MOVE (yystack_[0].value.as < ExpressionList > ()));
	}
#line 830 "syntactic.cpp"
    break;

  case 5: // statement: DIRECTIVE_DCB size expression "," expression
#line 918 "syntactic.y"
        {
		statement->emplace<StatementDcb>(YY_MOVE (yystack_[3].value.as < Size > ()), YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 838 "syntactic.cpp"
    break;

  case 6: // statement: DIRECTIVE_DS size expression
#line 922 "syntactic.y"
        {
		statement->emplace<StatementDs>(YY_MOVE (yystack_[1].value.as < Size > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 846 "syntactic.cpp"
    break;

  case 7: // statement: DIRECTIVE_INCLUDE STRING
#line 926 "syntactic.y"
        {
		statement->emplace<StatementInclude>(YY_MOVE (yystack_[0].value.as < String > ()));
	}
#line 854 "syntactic.cpp"
    break;

  case 8: // statement: DIRECTIVE_INCBIN STRING
#line 930 "syntactic.y"
        {		
		statement->emplace<StatementIncbin>(YY_MOVE (yystack_[0].value.as < String > ()), ExpressionNumber{0}, cc_false);
	}
#line 862 "syntactic.cpp"
    break;

  case 9: // statement: DIRECTIVE_INCBIN STRING "," expression
#line 934 "syntactic.y"
        {
		statement->emplace<StatementIncbin>(YY_MOVE (yystack_[2].value.as < String > ()), YY_MOVE (yystack_[0].value.as < Expression > ()), cc_false);
	}
#line 870 "syntactic.cpp"
    break;

  case 10: // statement: DIRECTIVE_INCBIN STRING "," "," expression
#line 938 "syntactic.y"
        {
		statement->emplace<StatementIncbin>(YY_MOVE (yystack_[3].value.as < String > ()), ExpressionNumber{0}, cc_true, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 878 "syntactic.cpp"
    break;

  case 11: // statement: DIRECTIVE_INCBIN STRING "," expression "," expression
#line 942 "syntactic.y"
        {
		statement->emplace<StatementIncbin>(YY_MOVE (yystack_[4].value.as < String > ()), YY_MOVE (yystack_[2].value.as < Expression > ()), cc_true, YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 886 "syntactic.cpp"
    break;

  case 12: // statement: DIRECTIVE_REPT expression
#line 946 "syntactic.y"
        {
		statement->emplace<StatementRept>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 894 "syntactic.cpp"
    break;

  case 13: // statement: DIRECTIVE_ENDR
#line 950 "syntactic.y"
        {
		statement->emplace<StatementEndR>();
	}
#line 902 "syntactic.cpp"
    break;

  case 14: // statement: DIRECTIVE_MACRO
#line 954 "syntactic.y"
        {
		statement->emplace<StatementMacro>(cc_false);
	}
#line 910 "syntactic.cpp"
    break;

  case 15: // statement: DIRECTIVE_MACRO "*"
#line 958 "syntactic.y"
        {
		statement->emplace<StatementMacro>(cc_true);
	}
#line 918 "syntactic.cpp"
    break;

  case 16: // statement: DIRECTIVE_MACRO identifier_list
#line 962 "syntactic.y"
        {
		statement->emplace<StatementMacro>(cc_false, YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 926 "syntactic.cpp"
    break;

  case 17: // statement: DIRECTIVE_MACRO "*" "," identifier_list
#line 966 "syntactic.y"
        {
		statement->emplace<StatementMacro>(cc_true, YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 934 "syntactic.cpp"
    break;

  case 18: // statement: DIRECTIVE_MACROS
#line 970 "syntactic.y"
        {
		// TODO: Is there a way to make this tidier?
		statement->emplace<StatementMacroS>(StatementMacro{cc_false});
	}
#line 943 "syntactic.cpp"
    break;

  case 19: // statement: DIRECTIVE_MACROS identifier_list
#line 975 "syntactic.y"
        {
		// TODO: Is there a way to make this tidier?
		statement->emplace<StatementMacroS>(StatementMacro{cc_false, YY_MOVE (yystack_[0].value.as < IdentifierList > ())});
	}
#line 952 "syntactic.cpp"
    break;

  case 20: // statement: DIRECTIVE_ENDM
#line 980 "syntactic.y"
        {
		statement->emplace<StatementEndM>();
	}
#line 960 "syntactic.cpp"
    break;

  case 21: // statement: DIRECTIVE_EQU expression
#line 984 "syntactic.y"
        {
		statement->emplace<StatementEqu>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 968 "syntactic.cpp"
    break;

  case 22: // statement: DIRECTIVE_EQUS STRING
#line 988 "syntactic.y"
        {
		statement->emplace<StatementEquSString>(YY_MOVE (yystack_[0].value.as < String > ()));
	}
#line 976 "syntactic.cpp"
    break;

  case 23: // statement: DIRECTIVE_EQUS IDENTIFIER
#line 992 "syntactic.y"
        {
		statement->emplace<StatementEquSIdentifier>(YY_MOVE (yystack_[0].value.as < String > ()));
	}
#line 984 "syntactic.cpp"
    break;

  case 24: // statement: DIRECTIVE_SUBSTR "," "," STRING
#line 996 "syntactic.y"
        {
		String string = YY_MOVE (yystack_[0].value.as < String > ());
		const auto string_length = String_Length(string);
		statement->emplace<StatementSubstr>(ExpressionNumber{1}, ExpressionNumber{string_length}, std::move(string));
	}
#line 994 "syntactic.cpp"
    break;

  case 25: // statement: DIRECTIVE_SUBSTR expression "," "," STRING
#line 1002 "syntactic.y"
        {
		String string = YY_MOVE (yystack_[0].value.as < String > ());
		const auto string_length = String_Length(string);
		statement->emplace<StatementSubstr>(YY_MOVE (yystack_[3].value.as < Expression > ()), ExpressionNumber{string_length}, std::move(string));
	}
#line 1004 "syntactic.cpp"
    break;

  case 26: // statement: DIRECTIVE_SUBSTR "," expression "," STRING
#line 1008 "syntactic.y"
        {
		statement->emplace<StatementSubstr>(ExpressionNumber{1}, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < String > ()));
	}
#line 1012 "syntactic.cpp"
    break;

  case 27: // statement: DIRECTIVE_SUBSTR expression "," expression "," STRING
#line 1012 "syntactic.y"
        {
		statement->emplace<StatementSubstr>(YY_MOVE (yystack_[4].value.as < Expression > ()), YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < String > ()));
	}
#line 1020 "syntactic.cpp"
    break;

  case 28: // statement: DIRECTIVE_SET expression
#line 1016 "syntactic.y"
        {
		statement->emplace<StatementSet>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1028 "syntactic.cpp"
    break;

  case 29: // statement: "=" expression
#line 1020 "syntactic.y"
        {
		statement->emplace<StatementSet>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1036 "syntactic.cpp"
    break;

  case 30: // statement: DIRECTIVE_IF expression
#line 1024 "syntactic.y"
        {
		statement->emplace<StatementIf>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1044 "syntactic.cpp"
    break;

  case 31: // statement: DIRECTIVE_ELSEIF expression
#line 1028 "syntactic.y"
        {
		statement->emplace<StatementElseIf>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1052 "syntactic.cpp"
    break;

  case 32: // statement: DIRECTIVE_ELSEIF
#line 1032 "syntactic.y"
        {
		statement->emplace<StatementElse>();
	}
#line 1060 "syntactic.cpp"
    break;

  case 33: // statement: DIRECTIVE_ELSE
#line 1036 "syntactic.y"
        {
		statement->emplace<StatementElse>();
	}
#line 1068 "syntactic.cpp"
    break;

  case 34: // statement: DIRECTIVE_ENDC
#line 1040 "syntactic.y"
        {
		statement->emplace<StatementEndC>();
	}
#line 1076 "syntactic.cpp"
    break;

  case 35: // statement: DIRECTIVE_WHILE expression
#line 1044 "syntactic.y"
        {
		statement->emplace<StatementWhile>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1084 "syntactic.cpp"
    break;

  case 36: // statement: DIRECTIVE_ENDW
#line 1048 "syntactic.y"
        {
		statement->emplace<StatementEndW>();
	}
#line 1092 "syntactic.cpp"
    break;

  case 37: // statement: DIRECTIVE_EVEN
#line 1052 "syntactic.y"
        {
		statement->emplace<StatementEven>();
	}
#line 1100 "syntactic.cpp"
    break;

  case 38: // statement: DIRECTIVE_CNOP expression "," expression
#line 1056 "syntactic.y"
        {
		statement->emplace<StatementCnop>(YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1108 "syntactic.cpp"
    break;

  case 39: // statement: DIRECTIVE_INFORM expression "," STRING
#line 1060 "syntactic.y"
        {
		statement->emplace<StatementInform>(YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < String > ()));
	}
#line 1116 "syntactic.cpp"
    break;

  case 40: // statement: DIRECTIVE_INFORM expression "," STRING "," expression_list
#line 1064 "syntactic.y"
        {
		/* TODO - parameters */
		statement->emplace<StatementInform>(YY_MOVE (yystack_[4].value.as < Expression > ()), YY_MOVE (yystack_[2].value.as < String > ()));
	}
#line 1125 "syntactic.cpp"
    break;

  case 41: // statement: DIRECTIVE_FAIL
#line 1069 "syntactic.y"
        {
		statement->emplace<StatementFail>();
	}
#line 1133 "syntactic.cpp"
    break;

  case 42: // statement: DIRECTIVE_END
#line 1073 "syntactic.y"
        {
		statement->emplace<StatementEnd>();
	}
#line 1141 "syntactic.cpp"
    break;

  case 43: // statement: DIRECTIVE_RS size expression
#line 1077 "syntactic.y"
        {
		statement->emplace<StatementRS>(YY_MOVE (yystack_[1].value.as < Size > ()), YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1149 "syntactic.cpp"
    break;

  case 44: // statement: DIRECTIVE_RSSET expression
#line 1081 "syntactic.y"
        {
		statement->emplace<StatementRSSet>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1157 "syntactic.cpp"
    break;

  case 45: // statement: DIRECTIVE_RSRESET expression
#line 1085 "syntactic.y"
        {
		/* As mentioned in S.N. 68k's manual, this odd hack
		   is necessary for compatibility with other assemblers. */
		statement->emplace<StatementRSSet>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1167 "syntactic.cpp"
    break;

  case 46: // statement: DIRECTIVE_RSRESET
#line 1091 "syntactic.y"
        {
		statement->emplace<StatementRSReset>();
	}
#line 1175 "syntactic.cpp"
    break;

  case 47: // statement: DIRECTIVE_OBJ expression
#line 1095 "syntactic.y"
        {
		statement->emplace<StatementObj>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1183 "syntactic.cpp"
    break;

  case 48: // statement: DIRECTIVE_OBJEND
#line 1099 "syntactic.y"
        {
		statement->emplace<StatementObjEnd>();
	}
#line 1191 "syntactic.cpp"
    break;

  case 49: // statement: DIRECTIVE_ORG expression
#line 1103 "syntactic.y"
        {
		statement->emplace<StatementOrg>(YY_MOVE (yystack_[0].value.as < Expression > ()));
	}
#line 1199 "syntactic.cpp"
    break;

  case 50: // statement: DIRECTIVE_PUSHO
#line 1107 "syntactic.y"
        {
		statement->emplace<StatementPushO>();
	}
#line 1207 "syntactic.cpp"
    break;

  case 51: // statement: DIRECTIVE_POPO
#line 1111 "syntactic.y"
        {
		statement->emplace<StatementPopO>();
	}
#line 1215 "syntactic.cpp"
    break;

  case 52: // statement: DIRECTIVE_OPT option_list
#line 1115 "syntactic.y"
        {
		statement->emplace<StatementOpt>(YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 1223 "syntactic.cpp"
    break;

  case 53: // statement: DIRECTIVE_PUSHP STRING
#line 1119 "syntactic.y"
        {
		statement->emplace<StatementPushP>(YY_MOVE (yystack_[0].value.as < String > ()));
	}
#line 1231 "syntactic.cpp"
    break;

  case 54: // statement: DIRECTIVE_POPP IDENTIFIER
#line 1123 "syntactic.y"
        {
		statement->emplace<StatementPopP>(YY_MOVE (yystack_[0].value.as < String > ()));
	}
#line 1239 "syntactic.cpp"
    break;

  case 55: // statement: DIRECTIVE_SHIFT
#line 1127 "syntactic.y"
        {
		statement->emplace<StatementShift>();
	}
#line 1247 "syntactic.cpp"
    break;

  case 56: // statement: DIRECTIVE_MEXIT
#line 1131 "syntactic.y"
        {
		statement->emplace<StatementMExit>();
	}
#line 1255 "syntactic.cpp"
    break;

  case 57: // statement: DIRECTIVE_LOCAL identifier_list
#line 1135 "syntactic.y"
        {
		statement->emplace<StatementLocal>(YY_MOVE (yystack_[0].value.as < IdentifierList > ()));
	}
#line 1263 "syntactic.cpp"
    break;

  case 58: // statement: DIRECTIVE_LIST
#line 1139 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->emplace<StatementEmpty>();
	}
#line 1272 "syntactic.cpp"
    break;

  case 59: // statement: DIRECTIVE_LIST "+"
#line 1144 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->emplace<StatementEmpty>();
	}
#line 1281 "syntactic.cpp"
    break;

  case 60: // statement: DIRECTIVE_LIST "-"
#line 1149 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->emplace<StatementEmpty>();
	}
#line 1290 "syntactic.cpp"
    break;

  case 61: // statement: DIRECTIVE_NOLIST
#line 1154 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->emplace<StatementEmpty>();
	}
#line 1299 "syntactic.cpp"
    break;

  case 62: // statement: DIRECTIVE_PURGE IDENTIFIER
#line 1159 "syntactic.y"
        {
		statement->emplace<StatementPurge>(YY_MOVE (yystack_[0].value.as < String > ()));
	}
#line 1307 "syntactic.cpp"
    break;

  case 63: // expression_list: expression
#line 1166 "syntactic.y"
        {
		yylhs.value.as < ExpressionList > ().emplace_back(std::move(YY_MOVE (yystack_[0].value.as < Expression > ())));
	}
#line 1315 "syntactic.cpp"
    break;

  case 64: // expression_list: expression_list "," expression
#line 1170 "syntactic.y"
        {
		yylhs.value.as < ExpressionList > () = std::move(YY_MOVE (yystack_[2].value.as < ExpressionList > ()));

		yylhs.value.as < ExpressionList > ().emplace_back(std::move(YY_MOVE (yystack_[0].value.as < Expression > ())));
	}
#line 1325 "syntactic.cpp"
    break;

  case 65: // identifier_list: IDENTIFIER
#line 1179 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > ().emplace_back(std::move(YY_MOVE (yystack_[0].value.as < String > ())));
	}
#line 1333 "syntactic.cpp"
    break;

  case 66: // identifier_list: identifier_list "," IDENTIFIER
#line 1183 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > () = std::move(YY_MOVE (yystack_[2].value.as < IdentifierList > ()));

		yylhs.value.as < IdentifierList > ().emplace_back(std::move(YY_MOVE (yystack_[0].value.as < String > ())));
	}
#line 1343 "syntactic.cpp"
    break;

  case 67: // option_list: OPTION
#line 1192 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > ().emplace_back(std::move(YY_MOVE (yystack_[0].value.as < String > ())));
	}
#line 1351 "syntactic.cpp"
    break;

  case 68: // option_list: option_list "," OPTION
#line 1196 "syntactic.y"
        {
		yylhs.value.as < IdentifierList > () = std::move(YY_MOVE (yystack_[2].value.as < IdentifierList > ()));

		yylhs.value.as < IdentifierList > ().emplace_back(std::move(YY_MOVE (yystack_[0].value.as < String > ())));
	}
#line 1361 "syntactic.cpp"
    break;

  case 69: // instruction: full_opcode
#line 1205 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = YY_MOVE (yystack_[0].value.as < Opcode > ());
		yylhs.value.as < StatementInstruction > ().operands[0].type = OPERAND_NONE;
		yylhs.value.as < StatementInstruction > ().operands[1].type = OPERAND_NONE;
	}
#line 1371 "syntactic.cpp"
    break;

  case 70: // instruction: full_opcode operand
#line 1211 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = YY_MOVE (yystack_[1].value.as < Opcode > ());
		yylhs.value.as < StatementInstruction > ().operands[0] = YY_MOVE (yystack_[0].value.as < Operand > ());
		yylhs.value.as < StatementInstruction > ().operands[1].type = OPERAND_NONE;
	}
#line 1381 "syntactic.cpp"
    break;

  case 71: // instruction: full_opcode operand "," operand
#line 1217 "syntactic.y"
        {
		yylhs.value.as < StatementInstruction > ().opcode = YY_MOVE (yystack_[3].value.as < Opcode > ());
		yylhs.value.as < StatementInstruction > ().operands[0] = YY_MOVE (yystack_[2].value.as < Operand > ());
		yylhs.value.as < StatementInstruction > ().operands[1] = YY_MOVE (yystack_[0].value.as < Operand > ());
	}
#line 1391 "syntactic.cpp"
    break;

  case 72: // full_opcode: opcode
#line 1226 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = YY_MOVE (yystack_[0].value.as < Opcode > ());
		yylhs.value.as < Opcode > ().size = SIZE_UNDEFINED;
	}
#line 1400 "syntactic.cpp"
    break;

  case 73: // full_opcode: opcode "."
#line 1231 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = YY_MOVE (yystack_[1].value.as < Opcode > ());
		yylhs.value.as < Opcode > ().size = SIZE_UNDEFINED;
		m68kasm_warning_pedantic(scanner, statement, "Opcode has a dot but no size; either remove the dot or add an explicit size.");
	}
#line 1410 "syntactic.cpp"
    break;

  case 74: // full_opcode: opcode size
#line 1237 "syntactic.y"
        {
		yylhs.value.as < Opcode > () = YY_MOVE (yystack_[1].value.as < Opcode > ());
		yylhs.value.as < Opcode > ().size = YY_MOVE (yystack_[0].value.as < Size > ());
	}
#line 1419 "syntactic.cpp"
    break;

  case 75: // opcode: DIRECTIVE_ORI
#line 1245 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ORI;
	}
#line 1427 "syntactic.cpp"
    break;

  case 76: // opcode: DIRECTIVE_ANDI
#line 1249 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ANDI;
	}
#line 1435 "syntactic.cpp"
    break;

  case 77: // opcode: DIRECTIVE_SUBI
#line 1253 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBI;
	}
#line 1443 "syntactic.cpp"
    break;

  case 78: // opcode: DIRECTIVE_ADDI
#line 1257 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDI;
	}
#line 1451 "syntactic.cpp"
    break;

  case 79: // opcode: DIRECTIVE_EORI
#line 1261 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EORI;
	}
#line 1459 "syntactic.cpp"
    break;

  case 80: // opcode: DIRECTIVE_CMPI
#line 1265 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPI;
	}
#line 1467 "syntactic.cpp"
    break;

  case 81: // opcode: DIRECTIVE_BTST
#line 1269 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BTST_STATIC;
	}
#line 1475 "syntactic.cpp"
    break;

  case 82: // opcode: DIRECTIVE_BCHG
#line 1273 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BCHG_STATIC;
	}
#line 1483 "syntactic.cpp"
    break;

  case 83: // opcode: DIRECTIVE_BCLR
#line 1277 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BCLR_STATIC;
	}
#line 1491 "syntactic.cpp"
    break;

  case 84: // opcode: DIRECTIVE_BSET
#line 1281 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BSET_STATIC;
	}
#line 1499 "syntactic.cpp"
    break;

  case 85: // opcode: DIRECTIVE_MOVEP
#line 1285 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEP_TO_REG;
	}
#line 1507 "syntactic.cpp"
    break;

  case 86: // opcode: DIRECTIVE_MOVEA
#line 1289 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEA;
	}
#line 1515 "syntactic.cpp"
    break;

  case 87: // opcode: DIRECTIVE_MOVE
#line 1293 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVE;
	}
#line 1523 "syntactic.cpp"
    break;

  case 88: // opcode: DIRECTIVE_NEGX
#line 1297 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NEGX;
	}
#line 1531 "syntactic.cpp"
    break;

  case 89: // opcode: DIRECTIVE_CLR
#line 1301 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CLR;
	}
#line 1539 "syntactic.cpp"
    break;

  case 90: // opcode: DIRECTIVE_NEG
#line 1305 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NEG;
	}
#line 1547 "syntactic.cpp"
    break;

  case 91: // opcode: DIRECTIVE_NOT
#line 1309 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NOT;
	}
#line 1555 "syntactic.cpp"
    break;

  case 92: // opcode: DIRECTIVE_EXT
#line 1313 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EXT;
	}
#line 1563 "syntactic.cpp"
    break;

  case 93: // opcode: DIRECTIVE_NBCD
#line 1317 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NBCD;
	}
#line 1571 "syntactic.cpp"
    break;

  case 94: // opcode: DIRECTIVE_SWAP
#line 1321 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SWAP;
	}
#line 1579 "syntactic.cpp"
    break;

  case 95: // opcode: DIRECTIVE_PEA
#line 1325 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_PEA;
	}
#line 1587 "syntactic.cpp"
    break;

  case 96: // opcode: DIRECTIVE_ILLEGAL
#line 1329 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ILLEGAL;
	}
#line 1595 "syntactic.cpp"
    break;

  case 97: // opcode: DIRECTIVE_TAS
#line 1333 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TAS;
	}
#line 1603 "syntactic.cpp"
    break;

  case 98: // opcode: DIRECTIVE_TST
#line 1337 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TST;
	}
#line 1611 "syntactic.cpp"
    break;

  case 99: // opcode: DIRECTIVE_TRAP
#line 1341 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TRAP;
	}
#line 1619 "syntactic.cpp"
    break;

  case 100: // opcode: DIRECTIVE_LINK
#line 1345 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LINK;
	}
#line 1627 "syntactic.cpp"
    break;

  case 101: // opcode: DIRECTIVE_UNLK
#line 1349 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_UNLK;
	}
#line 1635 "syntactic.cpp"
    break;

  case 102: // opcode: DIRECTIVE_RESET
#line 1353 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RESET;
	}
#line 1643 "syntactic.cpp"
    break;

  case 103: // opcode: DIRECTIVE_NOP
#line 1357 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_NOP;
	}
#line 1651 "syntactic.cpp"
    break;

  case 104: // opcode: DIRECTIVE_STOP
#line 1361 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_STOP;
	}
#line 1659 "syntactic.cpp"
    break;

  case 105: // opcode: DIRECTIVE_RTE
#line 1365 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTE;
	}
#line 1667 "syntactic.cpp"
    break;

  case 106: // opcode: DIRECTIVE_RTS
#line 1369 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTS;
	}
#line 1675 "syntactic.cpp"
    break;

  case 107: // opcode: DIRECTIVE_TRAPV
#line 1373 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_TRAPV;
	}
#line 1683 "syntactic.cpp"
    break;

  case 108: // opcode: DIRECTIVE_RTR
#line 1377 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_RTR;
	}
#line 1691 "syntactic.cpp"
    break;

  case 109: // opcode: DIRECTIVE_JSR
#line 1381 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_JSR;
	}
#line 1699 "syntactic.cpp"
    break;

  case 110: // opcode: DIRECTIVE_JMP
#line 1385 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_JMP;
	}
#line 1707 "syntactic.cpp"
    break;

  case 111: // opcode: DIRECTIVE_MOVEM
#line 1389 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEM_TO_REGS;
	}
#line 1715 "syntactic.cpp"
    break;

  case 112: // opcode: DIRECTIVE_LEA
#line 1393 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LEA;
	}
#line 1723 "syntactic.cpp"
    break;

  case 113: // opcode: DIRECTIVE_CHK
#line 1397 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CHK;
	}
#line 1731 "syntactic.cpp"
    break;

  case 114: // opcode: DIRECTIVE_ADDQ
#line 1401 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDQ;
	}
#line 1739 "syntactic.cpp"
    break;

  case 115: // opcode: DIRECTIVE_SUBQ
#line 1405 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBQ;
	}
#line 1747 "syntactic.cpp"
    break;

  case 116: // opcode: DIRECTIVE_ST
#line 1409 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_TRUE;
	}
#line 1756 "syntactic.cpp"
    break;

  case 117: // opcode: DIRECTIVE_SF
#line 1414 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_FALSE;
	}
#line 1765 "syntactic.cpp"
    break;

  case 118: // opcode: DIRECTIVE_SHI
#line 1419 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 1774 "syntactic.cpp"
    break;

  case 119: // opcode: DIRECTIVE_SLS
#line 1424 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 1783 "syntactic.cpp"
    break;

  case 120: // opcode: DIRECTIVE_SCC
#line 1429 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 1792 "syntactic.cpp"
    break;

  case 121: // opcode: DIRECTIVE_SCS
#line 1434 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 1801 "syntactic.cpp"
    break;

  case 122: // opcode: DIRECTIVE_SNE
#line 1439 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 1810 "syntactic.cpp"
    break;

  case 123: // opcode: DIRECTIVE_SEQ
#line 1444 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 1819 "syntactic.cpp"
    break;

  case 124: // opcode: DIRECTIVE_SVC
#line 1449 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 1828 "syntactic.cpp"
    break;

  case 125: // opcode: DIRECTIVE_SVS
#line 1454 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 1837 "syntactic.cpp"
    break;

  case 126: // opcode: DIRECTIVE_SPL
#line 1459 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 1846 "syntactic.cpp"
    break;

  case 127: // opcode: DIRECTIVE_SMI
#line 1464 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 1855 "syntactic.cpp"
    break;

  case 128: // opcode: DIRECTIVE_SGE
#line 1469 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 1864 "syntactic.cpp"
    break;

  case 129: // opcode: DIRECTIVE_SLT
#line 1474 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 1873 "syntactic.cpp"
    break;

  case 130: // opcode: DIRECTIVE_SGT
#line 1479 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 1882 "syntactic.cpp"
    break;

  case 131: // opcode: DIRECTIVE_SLE
#line 1484 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Scc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 1891 "syntactic.cpp"
    break;

  case 132: // opcode: DIRECTIVE_DBT
#line 1489 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_TRUE;
	}
#line 1900 "syntactic.cpp"
    break;

  case 133: // opcode: DIRECTIVE_DBF
#line 1494 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_FALSE;
	}
#line 1909 "syntactic.cpp"
    break;

  case 134: // opcode: DIRECTIVE_DBHI
#line 1499 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 1918 "syntactic.cpp"
    break;

  case 135: // opcode: DIRECTIVE_DBLS
#line 1504 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 1927 "syntactic.cpp"
    break;

  case 136: // opcode: DIRECTIVE_DBCC
#line 1509 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 1936 "syntactic.cpp"
    break;

  case 137: // opcode: DIRECTIVE_DBCS
#line 1514 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 1945 "syntactic.cpp"
    break;

  case 138: // opcode: DIRECTIVE_DBNE
#line 1519 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 1954 "syntactic.cpp"
    break;

  case 139: // opcode: DIRECTIVE_DBEQ
#line 1524 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 1963 "syntactic.cpp"
    break;

  case 140: // opcode: DIRECTIVE_DBVC
#line 1529 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 1972 "syntactic.cpp"
    break;

  case 141: // opcode: DIRECTIVE_DBVS
#line 1534 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 1981 "syntactic.cpp"
    break;

  case 142: // opcode: DIRECTIVE_DBPL
#line 1539 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 1990 "syntactic.cpp"
    break;

  case 143: // opcode: DIRECTIVE_DBMI
#line 1544 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 1999 "syntactic.cpp"
    break;

  case 144: // opcode: DIRECTIVE_DBGE
#line 1549 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2008 "syntactic.cpp"
    break;

  case 145: // opcode: DIRECTIVE_DBLT
#line 1554 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 2017 "syntactic.cpp"
    break;

  case 146: // opcode: DIRECTIVE_DBGT
#line 1559 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 2026 "syntactic.cpp"
    break;

  case 147: // opcode: DIRECTIVE_DBLE
#line 1564 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DBcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2035 "syntactic.cpp"
    break;

  case 148: // opcode: DIRECTIVE_BRA
#line 1569 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BRA;
	}
#line 2043 "syntactic.cpp"
    break;

  case 149: // opcode: DIRECTIVE_BSR
#line 1573 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_BSR;
	}
#line 2051 "syntactic.cpp"
    break;

  case 150: // opcode: DIRECTIVE_BHI
#line 1577 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_HIGHER;
	}
#line 2060 "syntactic.cpp"
    break;

  case 151: // opcode: DIRECTIVE_BLS
#line 1582 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LOWER_OR_SAME;
	}
#line 2069 "syntactic.cpp"
    break;

  case 152: // opcode: DIRECTIVE_BCC
#line 1587 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_CLEAR;
	}
#line 2078 "syntactic.cpp"
    break;

  case 153: // opcode: DIRECTIVE_BCS
#line 1592 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_CARRY_SET;
	}
#line 2087 "syntactic.cpp"
    break;

  case 154: // opcode: DIRECTIVE_BNE
#line 1597 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_NOT_EQUAL;
	}
#line 2096 "syntactic.cpp"
    break;

  case 155: // opcode: DIRECTIVE_BEQ
#line 1602 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_EQUAL;
	}
#line 2105 "syntactic.cpp"
    break;

  case 156: // opcode: DIRECTIVE_BVC
#line 1607 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 2114 "syntactic.cpp"
    break;

  case 157: // opcode: DIRECTIVE_BVS
#line 1612 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_OVERFLOW_SET;
	}
#line 2123 "syntactic.cpp"
    break;

  case 158: // opcode: DIRECTIVE_BPL
#line 1617 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_PLUS;
	}
#line 2132 "syntactic.cpp"
    break;

  case 159: // opcode: DIRECTIVE_BMI
#line 1622 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_MINUS;
	}
#line 2141 "syntactic.cpp"
    break;

  case 160: // opcode: DIRECTIVE_BGE
#line 1627 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2150 "syntactic.cpp"
    break;

  case 161: // opcode: DIRECTIVE_BLT
#line 1632 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_THAN;
	}
#line 2159 "syntactic.cpp"
    break;

  case 162: // opcode: DIRECTIVE_BGT
#line 1637 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_GREATER_THAN;
	}
#line 2168 "syntactic.cpp"
    break;

  case 163: // opcode: DIRECTIVE_BLE
#line 1642 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_Bcc;
		yylhs.value.as < Opcode > ().condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2177 "syntactic.cpp"
    break;

  case 164: // opcode: DIRECTIVE_MOVEQ
#line 1647 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MOVEQ;
	}
#line 2185 "syntactic.cpp"
    break;

  case 165: // opcode: DIRECTIVE_DIVU
#line 1651 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DIVU;
	}
#line 2193 "syntactic.cpp"
    break;

  case 166: // opcode: DIRECTIVE_DIVS
#line 1655 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_DIVS;
	}
#line 2201 "syntactic.cpp"
    break;

  case 167: // opcode: DIRECTIVE_SBCD
#line 1659 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SBCD_DATA_REGS;
	}
#line 2209 "syntactic.cpp"
    break;

  case 168: // opcode: DIRECTIVE_OR
#line 1663 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_OR_TO_REG;
	}
#line 2217 "syntactic.cpp"
    break;

  case 169: // opcode: DIRECTIVE_SUB
#line 1667 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUB_TO_REG;
	}
#line 2225 "syntactic.cpp"
    break;

  case 170: // opcode: DIRECTIVE_SUBX
#line 1671 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBX_DATA_REGS;
	}
#line 2233 "syntactic.cpp"
    break;

  case 171: // opcode: DIRECTIVE_SUBA
#line 1675 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_SUBA;
	}
#line 2241 "syntactic.cpp"
    break;

  case 172: // opcode: DIRECTIVE_EOR
#line 1679 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EOR;
	}
#line 2249 "syntactic.cpp"
    break;

  case 173: // opcode: DIRECTIVE_CMPM
#line 1683 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPM;
	}
#line 2257 "syntactic.cpp"
    break;

  case 174: // opcode: DIRECTIVE_CMP
#line 1687 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMP;
	}
#line 2265 "syntactic.cpp"
    break;

  case 175: // opcode: DIRECTIVE_CMPA
#line 1691 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_CMPA;
	}
#line 2273 "syntactic.cpp"
    break;

  case 176: // opcode: DIRECTIVE_MULU
#line 1695 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MULU;
	}
#line 2281 "syntactic.cpp"
    break;

  case 177: // opcode: DIRECTIVE_MULS
#line 1699 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_MULS;
	}
#line 2289 "syntactic.cpp"
    break;

  case 178: // opcode: DIRECTIVE_ABCD
#line 1703 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ABCD_DATA_REGS;
	}
#line 2297 "syntactic.cpp"
    break;

  case 179: // opcode: DIRECTIVE_EXG
#line 1707 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_EXG;
	}
#line 2305 "syntactic.cpp"
    break;

  case 180: // opcode: DIRECTIVE_AND
#line 1711 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_AND_TO_REG;
	}
#line 2313 "syntactic.cpp"
    break;

  case 181: // opcode: DIRECTIVE_ADD
#line 1715 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADD_TO_REG;
	}
#line 2321 "syntactic.cpp"
    break;

  case 182: // opcode: DIRECTIVE_ADDX
#line 1719 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDX_DATA_REGS;
	}
#line 2329 "syntactic.cpp"
    break;

  case 183: // opcode: DIRECTIVE_ADDA
#line 1723 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ADDA;
	}
#line 2337 "syntactic.cpp"
    break;

  case 184: // opcode: DIRECTIVE_ASL
#line 1727 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ASL_STATIC;
	}
#line 2345 "syntactic.cpp"
    break;

  case 185: // opcode: DIRECTIVE_ASR
#line 1731 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ASR_STATIC;
	}
#line 2353 "syntactic.cpp"
    break;

  case 186: // opcode: DIRECTIVE_LSL
#line 1735 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LSL_STATIC;
	}
#line 2361 "syntactic.cpp"
    break;

  case 187: // opcode: DIRECTIVE_LSR
#line 1739 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_LSR_STATIC;
	}
#line 2369 "syntactic.cpp"
    break;

  case 188: // opcode: DIRECTIVE_ROXL
#line 1743 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROXL_STATIC;
	}
#line 2377 "syntactic.cpp"
    break;

  case 189: // opcode: DIRECTIVE_ROXR
#line 1747 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROXR_STATIC;
	}
#line 2385 "syntactic.cpp"
    break;

  case 190: // opcode: DIRECTIVE_ROL
#line 1751 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROL_STATIC;
	}
#line 2393 "syntactic.cpp"
    break;

  case 191: // opcode: DIRECTIVE_ROR
#line 1755 "syntactic.y"
        {
		yylhs.value.as < Opcode > ().type = OPCODE_ROR_STATIC;
	}
#line 2401 "syntactic.cpp"
    break;

  case 192: // size: SIZE_BYTE
#line 1762 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_BYTE;
	}
#line 2409 "syntactic.cpp"
    break;

  case 193: // size: SIZE_SHORT
#line 1766 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_SHORT;
	}
#line 2417 "syntactic.cpp"
    break;

  case 194: // size: SIZE_WORD
#line 1770 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_WORD;
	}
#line 2425 "syntactic.cpp"
    break;

  case 195: // size: SIZE_LONGWORD
#line 1774 "syntactic.y"
        {
		yylhs.value.as < Size > () = SIZE_LONGWORD;
	}
#line 2433 "syntactic.cpp"
    break;

  case 196: // operand: "(" ADDRESS_REGISTER ")"
#line 1782 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT;
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2442 "syntactic.cpp"
    break;

  case 197: // operand: "(" ADDRESS_REGISTER ")" "+"
#line 1787 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());
	}
#line 2451 "syntactic.cpp"
    break;

  case 198: // operand: "-" "(" ADDRESS_REGISTER ")"
#line 1792 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2460 "syntactic.cpp"
    break;

  case 199: // operand: expression "(" ADDRESS_REGISTER ")"
#line 1797 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2470 "syntactic.cpp"
    break;

  case 200: // operand: "(" expression "," ADDRESS_REGISTER ")"
#line 1803 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());
	}
#line 2480 "syntactic.cpp"
    break;

  case 201: // operand: "(" ADDRESS_REGISTER "," expression ")"
#line 1809 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[1].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[3].value.as < unsigned long > ());
	}
#line 2490 "syntactic.cpp"
    break;

  case 202: // operand: "(" ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1815 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[4].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2505 "syntactic.cpp"
    break;

  case 203: // operand: expression "(" ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1826 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[6].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[4].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2520 "syntactic.cpp"
    break;

  case 204: // operand: "(" expression "," ADDRESS_REGISTER "," data_or_address_register size ")"
#line 1837 "syntactic.y"
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

  case 205: // operand: "(" ADDRESS_REGISTER "," data_or_address_register ")"
#line 1848 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[3].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2551 "syntactic.cpp"
    break;

  case 206: // operand: expression "(" ADDRESS_REGISTER "," data_or_address_register ")"
#line 1860 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[5].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[3].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2567 "syntactic.cpp"
    break;

  case 207: // operand: "(" expression "," ADDRESS_REGISTER "," data_or_address_register ")"
#line 1872 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[5].value.as < Expression > ());
		yylhs.value.as < Operand > ().main_register = YY_MOVE (yystack_[3].value.as < unsigned long > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2583 "syntactic.cpp"
    break;

  case 208: // operand: expression "(" PROGRAM_COUNTER ")"
#line 1884 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
	}
#line 2592 "syntactic.cpp"
    break;

  case 209: // operand: "(" expression "," PROGRAM_COUNTER ")"
#line 1889 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[3].value.as < Expression > ());
	}
#line 2601 "syntactic.cpp"
    break;

  case 210: // operand: "(" PROGRAM_COUNTER "," expression ")"
#line 1894 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[1].value.as < Expression > ());
	}
#line 2610 "syntactic.cpp"
    break;

  case 211: // operand: "(" PROGRAM_COUNTER "," data_or_address_register size ")"
#line 1899 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2624 "syntactic.cpp"
    break;

  case 212: // operand: expression "(" PROGRAM_COUNTER "," data_or_address_register size ")"
#line 1909 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[6].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2638 "syntactic.cpp"
    break;

  case 213: // operand: "(" expression "," PROGRAM_COUNTER "," data_or_address_register size ")"
#line 1919 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());

		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[6].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[1].value.as < Size > ());
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2652 "syntactic.cpp"
    break;

  case 214: // operand: "(" PROGRAM_COUNTER "," data_or_address_register ")"
#line 1929 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal.emplace<ExpressionNumber>(0);
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2667 "syntactic.cpp"
    break;

  case 215: // operand: expression "(" PROGRAM_COUNTER "," data_or_address_register ")"
#line 1940 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[5].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2682 "syntactic.cpp"
    break;

  case 216: // operand: "(" expression "," PROGRAM_COUNTER "," data_or_address_register ")"
#line 1951 "syntactic.y"
        {
		const auto data_or_address_register = YY_MOVE (yystack_[1].value.as < unsigned long > ());

		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		yylhs.value.as < Operand > ().type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[5].value.as < Expression > ());
		yylhs.value.as < Operand > ().index_register = data_or_address_register % 8;
		yylhs.value.as < Operand > ().size = SIZE_WORD;
		yylhs.value.as < Operand > ().index_register_is_address_register = data_or_address_register / 8 != 0;
	}
#line 2697 "syntactic.cpp"
    break;

  case 217: // operand: "#" expression
#line 1963 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_LITERAL;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2706 "syntactic.cpp"
    break;

  case 218: // operand: STATUS_REGISTER
#line 1979 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_STATUS_REGISTER;
	}
#line 2714 "syntactic.cpp"
    break;

  case 219: // operand: CONDITION_CODE_REGISTER
#line 1983 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_CONDITION_CODE_REGISTER;
	}
#line 2722 "syntactic.cpp"
    break;

  case 220: // operand: USER_STACK_POINTER_REGISTER
#line 1987 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_USER_STACK_POINTER_REGISTER;
	}
#line 2730 "syntactic.cpp"
    break;

  case 221: // operand: expression
#line 1992 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[0].value.as < Expression > ());
		yylhs.value.as < Operand > ().size = SIZE_UNDEFINED;
	}
#line 2740 "syntactic.cpp"
    break;

  case 222: // operand: expression size
#line 1998 "syntactic.y"
        {
		yylhs.value.as < Operand > ().type = OPERAND_ADDRESS_ABSOLUTE;
		yylhs.value.as < Operand > ().literal = YY_MOVE (yystack_[1].value.as < Expression > ());
		yylhs.value.as < Operand > ().size = YY_MOVE (yystack_[0].value.as < Size > ());
	}
#line 2750 "syntactic.cpp"
    break;

  case 223: // operand: register_list
#line 2005 "syntactic.y"
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
#line 2784 "syntactic.cpp"
    break;

  case 224: // register_list: register_span
#line 2038 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2792 "syntactic.cpp"
    break;

  case 225: // register_list: register_span "/" register_list
#line 2042 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = YY_MOVE (yystack_[2].value.as < unsigned long > ()) | YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2800 "syntactic.cpp"
    break;

  case 226: // register_span: data_or_address_register
#line 2049 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 1 << YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2808 "syntactic.cpp"
    break;

  case 227: // register_span: data_or_address_register "-" data_or_address_register
#line 2053 "syntactic.y"
        {
		const auto first_data_or_address_register = YY_MOVE (yystack_[2].value.as < unsigned long > ());
		const auto second_data_or_address_register = YY_MOVE (yystack_[0].value.as < unsigned long > ());

		unsigned long start;
		unsigned long end;
		unsigned long i;

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

		for (i = start; i <= end; ++i)
			yylhs.value.as < unsigned long > () |= 1 << i;
	}
#line 2837 "syntactic.cpp"
    break;

  case 228: // data_or_address_register: DATA_REGISTER
#line 2081 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 0 + YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2845 "syntactic.cpp"
    break;

  case 229: // data_or_address_register: ADDRESS_REGISTER
#line 2085 "syntactic.y"
        {
		yylhs.value.as < unsigned long > () = 8 + YY_MOVE (yystack_[0].value.as < unsigned long > ());
	}
#line 2853 "syntactic.cpp"
    break;

  case 230: // expression: expression1
#line 2098 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2861 "syntactic.cpp"
    break;

  case 231: // expression: expression LOGICAL_AND expression1
#line 2103 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LOGICAL_AND, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 2870 "syntactic.cpp"
    break;

  case 232: // expression: expression LOGICAL_OR expression1
#line 2109 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LOGICAL_OR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 2879 "syntactic.cpp"
    break;

  case 233: // expression1: expression2
#line 2117 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2887 "syntactic.cpp"
    break;

  case 234: // expression1: expression1 "=" expression2
#line 2121 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::EQUALITY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 2896 "syntactic.cpp"
    break;

  case 235: // expression1: expression1 EQUALITY expression2
#line 2126 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::EQUALITY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 2905 "syntactic.cpp"
    break;

  case 236: // expression1: expression1 INEQUALITY expression2
#line 2131 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::INEQUALITY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 2914 "syntactic.cpp"
    break;

  case 237: // expression2: expression3
#line 2139 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2922 "syntactic.cpp"
    break;

  case 238: // expression2: expression2 "<" expression3
#line 2143 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LESS_THAN, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 2931 "syntactic.cpp"
    break;

  case 239: // expression2: expression2 LESS_OR_EQUAL expression3
#line 2148 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LESS_OR_EQUAL, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 2940 "syntactic.cpp"
    break;

  case 240: // expression2: expression2 ">" expression3
#line 2153 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MORE_THAN, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 2949 "syntactic.cpp"
    break;

  case 241: // expression2: expression2 MORE_OR_EQUAL expression3
#line 2158 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MORE_OR_EQUAL, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 2958 "syntactic.cpp"
    break;

  case 242: // expression3: expression4
#line 2166 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2966 "syntactic.cpp"
    break;

  case 243: // expression3: expression3 "+" expression4
#line 2170 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::ADD, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 2975 "syntactic.cpp"
    break;

  case 244: // expression3: expression3 "-" expression4
#line 2175 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::SUBTRACT, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 2984 "syntactic.cpp"
    break;

  case 245: // expression4: expression5
#line 2183 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 2992 "syntactic.cpp"
    break;

  case 246: // expression4: expression4 "*" expression5
#line 2187 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MULTIPLY, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3001 "syntactic.cpp"
    break;

  case 247: // expression4: expression4 "/" expression5
#line 2192 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::DIVIDE, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3010 "syntactic.cpp"
    break;

  case 248: // expression4: expression4 "%" expression5
#line 2197 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::MODULO, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3019 "syntactic.cpp"
    break;

  case 249: // expression5: expression6
#line 2205 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3027 "syntactic.cpp"
    break;

  case 250: // expression5: expression5 "&" expression6
#line 2209 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_AND, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3036 "syntactic.cpp"
    break;

  case 251: // expression5: expression5 "!" expression6
#line 2214 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_OR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3045 "syntactic.cpp"
    break;

  case 252: // expression5: expression5 "|" expression6
#line 2219 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_OR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3054 "syntactic.cpp"
    break;

  case 253: // expression5: expression5 "^" expression6
#line 2224 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::BITWISE_XOR, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3063 "syntactic.cpp"
    break;

  case 254: // expression6: expression7
#line 2232 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3071 "syntactic.cpp"
    break;

  case 255: // expression6: expression6 LEFT_SHIFT expression7
#line 2236 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::LEFT_SHIFT, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3080 "syntactic.cpp"
    break;

  case 256: // expression6: expression6 RIGHT_SHIFT expression7
#line 2241 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic2Operand::Type::RIGHT_SHIFT, YY_MOVE (yystack_[2].value.as < Expression > ()), YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3089 "syntactic.cpp"
    break;

  case 257: // expression7: expression8
#line 2249 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3097 "syntactic.cpp"
    break;

  case 258: // expression7: "+" expression7
#line 2253 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3105 "syntactic.cpp"
    break;

  case 259: // expression7: "-" expression7
#line 2257 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::NEGATE, YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3114 "syntactic.cpp"
    break;

  case 260: // expression7: "~" expression7
#line 2262 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::BITWISE_NOT, YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3123 "syntactic.cpp"
    break;

  case 261: // expression7: "!" expression7
#line 2268 "syntactic.y"
        {
		if (!DoExpression(yylhs.value.as < Expression > (), ExpressionArithmetic1Operand::Type::LOGICAL_NOT, YY_MOVE (yystack_[0].value.as < Expression > ())))
			YYNOMEM;
	}
#line 3132 "syntactic.cpp"
    break;

  case 262: // local_identifier: LOCAL_IDENTIFIER
#line 2275 "syntactic.y"
          { yylhs.value.as < String > () = YY_MOVE (yystack_[0].value.as < String > ()); }
#line 3138 "syntactic.cpp"
    break;

  case 263: // local_identifier: SIZE_BYTE
#line 2277 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".b");
		if (!String_CreateCopyView(yylhs.value.as < String > (), literal))
			YYNOMEM;
	}
#line 3148 "syntactic.cpp"
    break;

  case 264: // local_identifier: SIZE_SHORT
#line 2283 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".s");
		if (!String_CreateCopyView(yylhs.value.as < String > (), literal))
			YYNOMEM;
	}
#line 3158 "syntactic.cpp"
    break;

  case 265: // local_identifier: SIZE_WORD
#line 2289 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".w");
		if (!String_CreateCopyView(yylhs.value.as < String > (), literal))
			YYNOMEM;
	}
#line 3168 "syntactic.cpp"
    break;

  case 266: // local_identifier: SIZE_LONGWORD
#line 2295 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".l");
		if (!String_CreateCopyView(yylhs.value.as < String > (), literal))
			YYNOMEM;
	}
#line 3178 "syntactic.cpp"
    break;

  case 267: // identifier: IDENTIFIER
#line 2303 "syntactic.y"
          { yylhs.value.as < String > () = YY_MOVE (yystack_[0].value.as < String > ()); }
#line 3184 "syntactic.cpp"
    break;

  case 268: // identifier: local_identifier
#line 2304 "syntactic.y"
          { yylhs.value.as < String > () = YY_MOVE (yystack_[0].value.as < String > ()); }
#line 3190 "syntactic.cpp"
    break;

  case 269: // identifier: identifier local_identifier
#line 2306 "syntactic.y"
        {
		auto string_a = YY_MOVE (yystack_[1].value.as < String > ());
		auto string_b = YY_MOVE (yystack_[0].value.as < String > ());
		const cc_bool success = String_CreateAppend(yylhs.value.as < String > (), string_a, string_b);
		String_Destroy(string_a);
		String_Destroy(string_b);

		if (!success)
			YYNOMEM;
	}
#line 3205 "syntactic.cpp"
    break;

  case 270: // expression8: NUMBER
#line 2320 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionNumber>(YY_MOVE (yystack_[0].value.as < unsigned long > ()));
	}
#line 3213 "syntactic.cpp"
    break;

  case 271: // expression8: identifier
#line 2324 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionIdentifier>(YY_MOVE (yystack_[0].value.as < String > ()));
	}
#line 3221 "syntactic.cpp"
    break;

  case 272: // expression8: string
#line 2328 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[0].value.as < Expression > ());
	}
#line 3229 "syntactic.cpp"
    break;

  case 273: // expression8: "*"
#line 2332 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionProgramCounterOfStatement>();
	}
#line 3237 "syntactic.cpp"
    break;

  case 274: // expression8: "@"
#line 2336 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionProgramCounterOfExpression>();
	}
#line 3245 "syntactic.cpp"
    break;

  case 275: // expression8: "(" expression ")"
#line 2340 "syntactic.y"
        {
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[1].value.as < Expression > ());
	}
#line 3253 "syntactic.cpp"
    break;

  case 276: // expression8: STRLEN "(" STRING ")"
#line 2344 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionStrlen>(YY_MOVE (yystack_[1].value.as < String > ()));
	}
#line 3261 "syntactic.cpp"
    break;

  case 277: // expression8: STRCMP "(" STRING "," STRING ")"
#line 2348 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionStrcmp>(YY_MOVE (yystack_[3].value.as < String > ()), YY_MOVE (yystack_[1].value.as < String > ()));
	}
#line 3269 "syntactic.cpp"
    break;

  case 278: // expression8: INSTR "(" expression "," expression ")"
#line 2352 "syntactic.y"
        {
		Expression *subexpressions = new Expression[3];

		if (subexpressions == nullptr)
			YYNOMEM;

		subexpressions[0].emplace<ExpressionNumber>(1);
		subexpressions[1] = std::move(YY_MOVE (yystack_[3].value.as < Expression > ()));
		subexpressions[2] = std::move(YY_MOVE (yystack_[1].value.as < Expression > ()));

		yylhs.value.as < Expression > ().emplace<ExpressionInstr>(subexpressions);
	}
#line 3286 "syntactic.cpp"
    break;

  case 279: // expression8: INSTR "(" expression "," expression "," expression ")"
#line 2365 "syntactic.y"
        {
		Expression *subexpressions = new Expression[3];

		if (subexpressions == nullptr)
			YYNOMEM;

		subexpressions[0] = std::move(YY_MOVE (yystack_[5].value.as < Expression > ()));
		subexpressions[1] = std::move(YY_MOVE (yystack_[3].value.as < Expression > ()));
		subexpressions[2] = std::move(YY_MOVE (yystack_[1].value.as < Expression > ()));

		yylhs.value.as < Expression > ().emplace<ExpressionInstr>(subexpressions);
	}
#line 3303 "syntactic.cpp"
    break;

  case 280: // expression8: DEF "(" IDENTIFIER ")"
#line 2378 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionDef>(YY_MOVE (yystack_[1].value.as < String > ()));
	}
#line 3311 "syntactic.cpp"
    break;

  case 281: // expression8: TYPE "(" IDENTIFIER ")"
#line 2382 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionTypeWithIdentifier>(YY_MOVE (yystack_[1].value.as < String > ()));
	}
#line 3319 "syntactic.cpp"
    break;

  case 282: // expression8: TYPE "(" NUMBER ")"
#line 2386 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionTypeWithNumber>(YY_MOVE (yystack_[1].value.as < unsigned long > ()));
	}
#line 3327 "syntactic.cpp"
    break;

  case 283: // expression8: FILESIZE "(" STRING ")"
#line 2390 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionFilesize>(YY_MOVE (yystack_[1].value.as < String > ()));
	}
#line 3335 "syntactic.cpp"
    break;

  case 284: // expression8: OFFSET "(" expression ")"
#line 2394 "syntactic.y"
        {
		/* TODO: Make this actually do something besides pass the expression through. */
		yylhs.value.as < Expression > () = YY_MOVE (yystack_[1].value.as < Expression > ());
	}
#line 3344 "syntactic.cpp"
    break;

  case 285: // string: STRING
#line 2402 "syntactic.y"
        {
		yylhs.value.as < Expression > ().emplace<ExpressionString>(YY_MOVE (yystack_[0].value.as < String > ()));
	}
#line 3352 "syntactic.cpp"
    break;


#line 3356 "syntactic.cpp"

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
       0,   906,   906,   909,   913,   917,   921,   925,   929,   933,
     937,   941,   945,   949,   953,   957,   961,   965,   969,   974,
     979,   983,   987,   991,   995,  1001,  1007,  1011,  1015,  1019,
    1023,  1027,  1031,  1035,  1039,  1043,  1047,  1051,  1055,  1059,
    1063,  1068,  1072,  1076,  1080,  1084,  1090,  1094,  1098,  1102,
    1106,  1110,  1114,  1118,  1122,  1126,  1130,  1134,  1138,  1143,
    1148,  1153,  1158,  1165,  1169,  1178,  1182,  1191,  1195,  1204,
    1210,  1216,  1225,  1230,  1236,  1244,  1248,  1252,  1256,  1260,
    1264,  1268,  1272,  1276,  1280,  1284,  1288,  1292,  1296,  1300,
    1304,  1308,  1312,  1316,  1320,  1324,  1328,  1332,  1336,  1340,
    1344,  1348,  1352,  1356,  1360,  1364,  1368,  1372,  1376,  1380,
    1384,  1388,  1392,  1396,  1400,  1404,  1408,  1413,  1418,  1423,
    1428,  1433,  1438,  1443,  1448,  1453,  1458,  1463,  1468,  1473,
    1478,  1483,  1488,  1493,  1498,  1503,  1508,  1513,  1518,  1523,
    1528,  1533,  1538,  1543,  1548,  1553,  1558,  1563,  1568,  1572,
    1576,  1581,  1586,  1591,  1596,  1601,  1606,  1611,  1616,  1621,
    1626,  1631,  1636,  1641,  1646,  1650,  1654,  1658,  1662,  1666,
    1670,  1674,  1678,  1682,  1686,  1690,  1694,  1698,  1702,  1706,
    1710,  1714,  1718,  1722,  1726,  1730,  1734,  1738,  1742,  1746,
    1750,  1754,  1761,  1765,  1769,  1773,  1781,  1786,  1791,  1796,
    1802,  1808,  1814,  1825,  1836,  1847,  1859,  1871,  1883,  1888,
    1893,  1898,  1908,  1918,  1928,  1939,  1950,  1962,  1978,  1982,
    1986,  1991,  1997,  2004,  2037,  2041,  2048,  2052,  2080,  2084,
    2097,  2102,  2108,  2116,  2120,  2125,  2130,  2138,  2142,  2147,
    2152,  2157,  2165,  2169,  2174,  2182,  2186,  2191,  2196,  2204,
    2208,  2213,  2218,  2223,  2231,  2235,  2240,  2248,  2252,  2256,
    2261,  2267,  2275,  2276,  2282,  2288,  2294,  2303,  2304,  2305,
    2319,  2323,  2327,  2331,  2335,  2339,  2343,  2347,  2351,  2364,
    2377,  2381,  2385,  2389,  2393,  2401
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
#line 4244 "syntactic.cpp"

#line 2406 "syntactic.y"


static cc_bool DoExpression(Expression &expression, ExpressionArithmetic1Operand::Type type, Expression subexpression)
{
	Expression *subexpression_copy = new Expression;

	if (subexpression_copy == NULL)
	{
		DestroyExpression(&subexpression);
		return cc_false;
	}

	*subexpression_copy = std::move(subexpression);

	expression.emplace<ExpressionArithmetic1Operand>(type, subexpression_copy);

	return cc_true;
}

static cc_bool DoExpression(Expression &expression, ExpressionArithmetic2Operand::Type type, Expression left_expression, Expression right_expression)
{
	Expression *subexpressions = new Expression[2];

	if (subexpressions == NULL)
	{
		DestroyExpression(&left_expression);
		DestroyExpression(&right_expression);
		return cc_false;
	}

	subexpressions[0] = std::move(left_expression);
	subexpressions[1] = std::move(right_expression);

	expression.emplace<ExpressionArithmetic2Operand>(type, subexpressions);

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

	expression->emplace<ExpressionNumber>(0);
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
			},
			[](StatementMacroS &statement)
			{
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
			},
			[](StatementLocal &statement)
			{
			}
		}, *statement
	);

	statement->emplace<StatementEmpty>();
}
