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
#line 463 "syntactic.y"


#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

int m68kasm_lex(YYSTYPE *yylval_param, void *yyscanner);
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

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v)
    : Base (t)
    , value (YY_MOVE (v))
  {}


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
    value = YY_MOVE (s.value);
  }

  // by_kind.
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



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
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value))
  {
#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value))
  {
    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
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

    // User destructor.
    switch (yysym.kind ())
    {
      case symbol_kind::S_IDENTIFIER: // IDENTIFIER
#line 706 "syntactic.y"
                    { String_Destroy(&(yysym.value.string)); }
#line 354 "syntactic.cpp"
        break;

      case symbol_kind::S_LOCAL_IDENTIFIER: // LOCAL_IDENTIFIER
#line 706 "syntactic.y"
                    { String_Destroy(&(yysym.value.string)); }
#line 360 "syntactic.cpp"
        break;

      case symbol_kind::S_STRING: // STRING
#line 706 "syntactic.y"
                    { String_Destroy(&(yysym.value.string)); }
#line 366 "syntactic.cpp"
        break;

      case symbol_kind::S_OPTION: // OPTION
#line 706 "syntactic.y"
                    { String_Destroy(&(yysym.value.string)); }
#line 372 "syntactic.cpp"
        break;

      case symbol_kind::S_expression_list: // expression_list
#line 708 "syntactic.y"
                    { DestroyExpressionList(&(yysym.value.expression_list)); }
#line 378 "syntactic.cpp"
        break;

      case symbol_kind::S_identifier_list: // identifier_list
#line 709 "syntactic.y"
                    { DestroyIdentifierList(&(yysym.value.identifier_list)); }
#line 384 "syntactic.cpp"
        break;

      case symbol_kind::S_option_list: // option_list
#line 709 "syntactic.y"
                    { DestroyIdentifierList(&(yysym.value.identifier_list)); }
#line 390 "syntactic.cpp"
        break;

      case symbol_kind::S_operand: // operand
#line 707 "syntactic.y"
                    { DestroyOperand(&(yysym.value.operand)); }
#line 396 "syntactic.cpp"
        break;

      case symbol_kind::S_expression: // expression
#line 710 "syntactic.y"
                    { DestroyExpression(&(yysym.value.expression)); }
#line 402 "syntactic.cpp"
        break;

      case symbol_kind::S_expression1: // expression1
#line 710 "syntactic.y"
                    { DestroyExpression(&(yysym.value.expression)); }
#line 408 "syntactic.cpp"
        break;

      case symbol_kind::S_expression2: // expression2
#line 710 "syntactic.y"
                    { DestroyExpression(&(yysym.value.expression)); }
#line 414 "syntactic.cpp"
        break;

      case symbol_kind::S_expression3: // expression3
#line 710 "syntactic.y"
                    { DestroyExpression(&(yysym.value.expression)); }
#line 420 "syntactic.cpp"
        break;

      case symbol_kind::S_expression4: // expression4
#line 710 "syntactic.y"
                    { DestroyExpression(&(yysym.value.expression)); }
#line 426 "syntactic.cpp"
        break;

      case symbol_kind::S_expression5: // expression5
#line 710 "syntactic.y"
                    { DestroyExpression(&(yysym.value.expression)); }
#line 432 "syntactic.cpp"
        break;

      case symbol_kind::S_expression6: // expression6
#line 710 "syntactic.y"
                    { DestroyExpression(&(yysym.value.expression)); }
#line 438 "syntactic.cpp"
        break;

      case symbol_kind::S_expression7: // expression7
#line 710 "syntactic.y"
                    { DestroyExpression(&(yysym.value.expression)); }
#line 444 "syntactic.cpp"
        break;

      case symbol_kind::S_local_identifier: // local_identifier
#line 706 "syntactic.y"
                    { String_Destroy(&(yysym.value.string)); }
#line 450 "syntactic.cpp"
        break;

      case symbol_kind::S_identifier: // identifier
#line 706 "syntactic.y"
                    { String_Destroy(&(yysym.value.string)); }
#line 456 "syntactic.cpp"
        break;

      case symbol_kind::S_expression8: // expression8
#line 710 "syntactic.y"
                    { DestroyExpression(&(yysym.value.expression)); }
#line 462 "syntactic.cpp"
        break;

      case symbol_kind::S_string: // string
#line 710 "syntactic.y"
                    { DestroyExpression(&(yysym.value.expression)); }
#line 468 "syntactic.cpp"
        break;

      default:
        break;
    }
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
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, scanner));
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
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // statement: %empty
#line 723 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 731 "syntactic.cpp"
    break;

  case 3: // statement: instruction
#line 727 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INSTRUCTION;
		statement->shared.instruction = (yystack_[0].value.instruction);
	}
#line 740 "syntactic.cpp"
    break;

  case 4: // statement: DIRECTIVE_DC size expression_list
#line 732 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DC;
		statement->shared.dc.size = (yystack_[1].value.size);
		statement->shared.dc.values = (yystack_[0].value.expression_list);
	}
#line 750 "syntactic.cpp"
    break;

  case 5: // statement: DIRECTIVE_DCB size expression ',' expression
#line 738 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DCB;
		statement->shared.dcb.size = (yystack_[3].value.size);
		statement->shared.dcb.repetitions = (yystack_[2].value.expression);
		statement->shared.dcb.value = (yystack_[0].value.expression);
	}
#line 761 "syntactic.cpp"
    break;

  case 6: // statement: DIRECTIVE_DS size expression
#line 745 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_DS;
		statement->shared.ds.size = (yystack_[1].value.size);
		statement->shared.ds.length = (yystack_[0].value.expression);
	}
#line 771 "syntactic.cpp"
    break;

  case 7: // statement: DIRECTIVE_INCLUDE STRING
#line 751 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCLUDE;
		statement->shared.include.path = (yystack_[0].value.string);
	}
#line 780 "syntactic.cpp"
    break;

  case 8: // statement: DIRECTIVE_INCBIN STRING
#line 756 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yystack_[0].value.string);
		statement->shared.incbin.start.type = EXPRESSION_NUMBER;
		statement->shared.incbin.start.shared.unsigned_long = 0;
		statement->shared.incbin.has_length = cc_false;
	}
#line 792 "syntactic.cpp"
    break;

  case 9: // statement: DIRECTIVE_INCBIN STRING ',' expression
#line 764 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yystack_[2].value.string);
		statement->shared.incbin.start = (yystack_[0].value.expression);
		statement->shared.incbin.has_length = cc_false;
	}
#line 803 "syntactic.cpp"
    break;

  case 10: // statement: DIRECTIVE_INCBIN STRING ',' ',' expression
#line 771 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yystack_[3].value.string);
		statement->shared.incbin.start.type = EXPRESSION_NUMBER;
		statement->shared.incbin.start.shared.unsigned_long = 0;
		statement->shared.incbin.has_length = cc_true;
		statement->shared.incbin.length = (yystack_[0].value.expression);
	}
#line 816 "syntactic.cpp"
    break;

  case 11: // statement: DIRECTIVE_INCBIN STRING ',' expression ',' expression
#line 780 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INCBIN;
		statement->shared.incbin.path = (yystack_[4].value.string);
		statement->shared.incbin.start = (yystack_[2].value.expression);
		statement->shared.incbin.has_length = cc_true;
		statement->shared.incbin.length = (yystack_[0].value.expression);
	}
#line 828 "syntactic.cpp"
    break;

  case 12: // statement: DIRECTIVE_REPT expression
#line 788 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_REPT;
		statement->shared.rept.repetitions = (yystack_[0].value.expression);
	}
#line 837 "syntactic.cpp"
    break;

  case 13: // statement: DIRECTIVE_ENDR
#line 793 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDR;
	}
#line 845 "syntactic.cpp"
    break;

  case 14: // statement: DIRECTIVE_MACRO
#line 797 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 856 "syntactic.cpp"
    break;

  case 15: // statement: DIRECTIVE_MACRO '*'
#line 804 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_true;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 867 "syntactic.cpp"
    break;

  case 16: // statement: DIRECTIVE_MACRO identifier_list
#line 811 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names = (yystack_[0].value.identifier_list);
	}
#line 877 "syntactic.cpp"
    break;

  case 17: // statement: DIRECTIVE_MACRO '*' ',' identifier_list
#line 817 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACRO;
		statement->shared.macro.uses_label = cc_true;
		statement->shared.macro.parameter_names = (yystack_[0].value.identifier_list);
	}
#line 887 "syntactic.cpp"
    break;

  case 18: // statement: DIRECTIVE_MACROS
#line 823 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names.head = NULL;
		statement->shared.macro.parameter_names.tail = NULL;
	}
#line 898 "syntactic.cpp"
    break;

  case 19: // statement: DIRECTIVE_MACROS identifier_list
#line 830 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MACROS;
		statement->shared.macro.uses_label = cc_false;
		statement->shared.macro.parameter_names = (yystack_[0].value.identifier_list);
	}
#line 908 "syntactic.cpp"
    break;

  case 20: // statement: DIRECTIVE_ENDM
#line 836 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDM;
	}
#line 916 "syntactic.cpp"
    break;

  case 21: // statement: DIRECTIVE_EQU expression
#line 840 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQU;
		statement->shared.expression = (yystack_[0].value.expression);
	}
#line 925 "syntactic.cpp"
    break;

  case 22: // statement: DIRECTIVE_EQUS STRING
#line 845 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQUS_STRING;
		statement->shared.string = (yystack_[0].value.string);
	}
#line 934 "syntactic.cpp"
    break;

  case 23: // statement: DIRECTIVE_EQUS IDENTIFIER
#line 850 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EQUS_IDENTIFIER;
		statement->shared.string = (yystack_[0].value.string);
	}
#line 943 "syntactic.cpp"
    break;

  case 24: // statement: DIRECTIVE_SUBSTR ',' ',' STRING
#line 855 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start.type = EXPRESSION_NUMBER;
		statement->shared.substr.start.shared.unsigned_long = 1;
		statement->shared.substr.end.type = EXPRESSION_NUMBER;
		statement->shared.substr.end.shared.unsigned_long = String_Length(&(yystack_[0].value.string));
		statement->shared.substr.string = (yystack_[0].value.string);
	}
#line 956 "syntactic.cpp"
    break;

  case 25: // statement: DIRECTIVE_SUBSTR expression ',' ',' STRING
#line 864 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start = (yystack_[3].value.expression);
		statement->shared.substr.end.type = EXPRESSION_NUMBER;
		statement->shared.substr.end.shared.unsigned_long = String_Length(&(yystack_[0].value.string));
		statement->shared.substr.string = (yystack_[0].value.string);
	}
#line 968 "syntactic.cpp"
    break;

  case 26: // statement: DIRECTIVE_SUBSTR ',' expression ',' STRING
#line 872 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start.type = EXPRESSION_NUMBER;
		statement->shared.substr.start.shared.unsigned_long = 1;
		statement->shared.substr.end = (yystack_[2].value.expression);
		statement->shared.substr.string = (yystack_[0].value.string);
	}
#line 980 "syntactic.cpp"
    break;

  case 27: // statement: DIRECTIVE_SUBSTR expression ',' expression ',' STRING
#line 880 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SUBSTR;
		statement->shared.substr.start = (yystack_[4].value.expression);
		statement->shared.substr.end = (yystack_[2].value.expression);
		statement->shared.substr.string = (yystack_[0].value.string);
	}
#line 991 "syntactic.cpp"
    break;

  case 28: // statement: DIRECTIVE_SET expression
#line 887 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.expression = (yystack_[0].value.expression);
	}
#line 1000 "syntactic.cpp"
    break;

  case 29: // statement: '=' expression
#line 892 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SET;
		statement->shared.expression = (yystack_[0].value.expression);
	}
#line 1009 "syntactic.cpp"
    break;

  case 30: // statement: DIRECTIVE_IF expression
#line 897 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_IF;
		statement->shared.expression = (yystack_[0].value.expression);
	}
#line 1018 "syntactic.cpp"
    break;

  case 31: // statement: DIRECTIVE_ELSEIF expression
#line 902 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSEIF;
		statement->shared.expression = (yystack_[0].value.expression);
	}
#line 1027 "syntactic.cpp"
    break;

  case 32: // statement: DIRECTIVE_ELSEIF
#line 907 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSE;
	}
#line 1035 "syntactic.cpp"
    break;

  case 33: // statement: DIRECTIVE_ELSE
#line 911 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ELSE;
	}
#line 1043 "syntactic.cpp"
    break;

  case 34: // statement: DIRECTIVE_ENDC
#line 915 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDC;
	}
#line 1051 "syntactic.cpp"
    break;

  case 35: // statement: DIRECTIVE_WHILE expression
#line 919 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_WHILE;
		statement->shared.expression = (yystack_[0].value.expression);
	}
#line 1060 "syntactic.cpp"
    break;

  case 36: // statement: DIRECTIVE_ENDW
#line 924 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ENDW;
	}
#line 1068 "syntactic.cpp"
    break;

  case 37: // statement: DIRECTIVE_EVEN
#line 928 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_EVEN;
	}
#line 1076 "syntactic.cpp"
    break;

  case 38: // statement: DIRECTIVE_CNOP expression ',' expression
#line 932 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_CNOP;
		statement->shared.cnop.offset = (yystack_[2].value.expression);
		statement->shared.cnop.size_boundary = (yystack_[0].value.expression);
	}
#line 1086 "syntactic.cpp"
    break;

  case 39: // statement: DIRECTIVE_INFORM expression ',' STRING
#line 938 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.severity = (yystack_[2].value.expression);
		statement->shared.inform.message = (yystack_[0].value.string);
	}
#line 1096 "syntactic.cpp"
    break;

  case 40: // statement: DIRECTIVE_INFORM expression ',' STRING ',' expression_list
#line 944 "syntactic.y"
        {
		(void)(yystack_[0].value.expression_list);

		/* TODO - parameters */
		statement->type = STATEMENT_TYPE_INFORM;
		statement->shared.inform.severity = (yystack_[4].value.expression);
		statement->shared.inform.message = (yystack_[2].value.string);
		DestroyExpressionList(&(yystack_[0].value.expression_list));
	}
#line 1110 "syntactic.cpp"
    break;

  case 41: // statement: DIRECTIVE_FAIL
#line 954 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_FAIL;
	}
#line 1118 "syntactic.cpp"
    break;

  case 42: // statement: DIRECTIVE_END
#line 958 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_END;
	}
#line 1126 "syntactic.cpp"
    break;

  case 43: // statement: DIRECTIVE_RS size expression
#line 962 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RS;
		statement->shared.rs.size = (yystack_[1].value.size);
		statement->shared.rs.length = (yystack_[0].value.expression);
	}
#line 1136 "syntactic.cpp"
    break;

  case 44: // statement: DIRECTIVE_RSSET expression
#line 968 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RSSET;
		statement->shared.expression = (yystack_[0].value.expression);
	}
#line 1145 "syntactic.cpp"
    break;

  case 45: // statement: DIRECTIVE_RSRESET expression
#line 973 "syntactic.y"
        {
		/* As mentioned in S.N. 68k's manual, this odd hack
		   is necessary for compatibility with other assemblers. */
		statement->type = STATEMENT_TYPE_RSSET;
		statement->shared.expression = (yystack_[0].value.expression);
	}
#line 1156 "syntactic.cpp"
    break;

  case 46: // statement: DIRECTIVE_RSRESET
#line 980 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_RSRESET;
	}
#line 1164 "syntactic.cpp"
    break;

  case 47: // statement: DIRECTIVE_OBJ expression
#line 984 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OBJ;
		statement->shared.expression = (yystack_[0].value.expression);
	}
#line 1173 "syntactic.cpp"
    break;

  case 48: // statement: DIRECTIVE_OBJEND
#line 989 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OBJEND;
	}
#line 1181 "syntactic.cpp"
    break;

  case 49: // statement: DIRECTIVE_ORG expression
#line 993 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_ORG;
		statement->shared.expression = (yystack_[0].value.expression);
	}
#line 1190 "syntactic.cpp"
    break;

  case 50: // statement: DIRECTIVE_PUSHO
#line 998 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PUSHO;
	}
#line 1198 "syntactic.cpp"
    break;

  case 51: // statement: DIRECTIVE_POPO
#line 1002 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_POPO;
	}
#line 1206 "syntactic.cpp"
    break;

  case 52: // statement: DIRECTIVE_OPT option_list
#line 1006 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_OPT;
		statement->shared.opt.options = (yystack_[0].value.identifier_list);
	}
#line 1215 "syntactic.cpp"
    break;

  case 53: // statement: DIRECTIVE_PUSHP STRING
#line 1011 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PUSHP;
		statement->shared.string = (yystack_[0].value.string);
	}
#line 1224 "syntactic.cpp"
    break;

  case 54: // statement: DIRECTIVE_POPP IDENTIFIER
#line 1016 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_POPP;
		statement->shared.string = (yystack_[0].value.string);
	}
#line 1233 "syntactic.cpp"
    break;

  case 55: // statement: DIRECTIVE_SHIFT
#line 1021 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_SHIFT;
	}
#line 1241 "syntactic.cpp"
    break;

  case 56: // statement: DIRECTIVE_MEXIT
#line 1025 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_MEXIT;
	}
#line 1249 "syntactic.cpp"
    break;

  case 57: // statement: DIRECTIVE_LOCAL identifier_list
#line 1029 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_LOCAL;
		statement->shared.local.identifiers = (yystack_[0].value.identifier_list);
	}
#line 1258 "syntactic.cpp"
    break;

  case 58: // statement: DIRECTIVE_LIST
#line 1034 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 1267 "syntactic.cpp"
    break;

  case 59: // statement: DIRECTIVE_LIST '+'
#line 1039 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 1276 "syntactic.cpp"
    break;

  case 60: // statement: DIRECTIVE_LIST '-'
#line 1044 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 1285 "syntactic.cpp"
    break;

  case 61: // statement: DIRECTIVE_NOLIST
#line 1049 "syntactic.y"
        {
		/* TODO: Make this actually do something. */
		statement->type = STATEMENT_TYPE_EMPTY;
	}
#line 1294 "syntactic.cpp"
    break;

  case 62: // statement: DIRECTIVE_PURGE IDENTIFIER
#line 1054 "syntactic.y"
        {
		statement->type = STATEMENT_TYPE_PURGE;
		statement->shared.string = (yystack_[0].value.string);
	}
#line 1303 "syntactic.cpp"
    break;

  case 63: // expression_list: expression
#line 1062 "syntactic.y"
        {
		ExpressionListNode *node = (ExpressionListNode*)malloc(sizeof(ExpressionListNode));

		if (node == NULL)
		{
			DestroyExpression(&(yystack_[0].value.expression));
			YYNOMEM;
		}
		else
		{
			node->expression = (yystack_[0].value.expression);
			node->next = NULL;
		}

		(yylhs.value.expression_list).head = (yylhs.value.expression_list).tail = node;
	}
#line 1324 "syntactic.cpp"
    break;

  case 64: // expression_list: expression_list ',' expression
#line 1079 "syntactic.y"
        {
		ExpressionListNode *node = (ExpressionListNode*)malloc(sizeof(ExpressionListNode));

		(yylhs.value.expression_list) = (yystack_[2].value.expression_list);

		if (node == NULL)
		{
			DestroyExpressionList(&(yystack_[2].value.expression_list));
			DestroyExpression(&(yystack_[0].value.expression));
			YYNOMEM;
		}
		else
		{
			node->expression = (yystack_[0].value.expression);
			node->next = NULL;

			if ((yylhs.value.expression_list).head == NULL)
				(yylhs.value.expression_list).head = node;
			else
				((ExpressionListNode*)(yylhs.value.expression_list).tail)->next = node;

			(yylhs.value.expression_list).tail = node;
		}
	}
#line 1353 "syntactic.cpp"
    break;

  case 65: // identifier_list: IDENTIFIER
#line 1107 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		if (node == NULL)
		{
			String_Destroy(&(yystack_[0].value.string));
			YYNOMEM;
		}
		else
		{
			node->identifier = (yystack_[0].value.string);
			node->next = NULL;
		}

		(yylhs.value.identifier_list).head = (yylhs.value.identifier_list).tail = node;
	}
#line 1374 "syntactic.cpp"
    break;

  case 66: // identifier_list: identifier_list ',' IDENTIFIER
#line 1124 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		(yylhs.value.identifier_list) = (yystack_[2].value.identifier_list);

		if (node == NULL)
		{
			DestroyIdentifierList(&(yystack_[2].value.identifier_list));
			String_Destroy(&(yystack_[0].value.string));
			YYNOMEM;
		}
		else
		{
			node->identifier = (yystack_[0].value.string);
			node->next = NULL;

			if ((yylhs.value.identifier_list).head == NULL)
				(yylhs.value.identifier_list).head = node;
			else
				((IdentifierListNode*)(yylhs.value.identifier_list).tail)->next = node;

			(yylhs.value.identifier_list).tail = node;
		}
	}
#line 1403 "syntactic.cpp"
    break;

  case 67: // option_list: OPTION
#line 1152 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		if (node == NULL)
		{
			String_Destroy(&(yystack_[0].value.string));
			YYNOMEM;
		}
		else
		{
			node->identifier = (yystack_[0].value.string);
			node->next = NULL;
		}

		(yylhs.value.identifier_list).head = (yylhs.value.identifier_list).tail = node;
	}
#line 1424 "syntactic.cpp"
    break;

  case 68: // option_list: option_list ',' OPTION
#line 1169 "syntactic.y"
        {
		IdentifierListNode *node = (IdentifierListNode*)malloc(sizeof(IdentifierListNode));

		(yylhs.value.identifier_list) = (yystack_[2].value.identifier_list);

		if (node == NULL)
		{
			DestroyIdentifierList(&(yystack_[2].value.identifier_list));
			String_Destroy(&(yystack_[0].value.string));
			YYNOMEM;
		}
		else
		{
			node->identifier = (yystack_[0].value.string);
			node->next = NULL;

			if ((yylhs.value.identifier_list).head == NULL)
				(yylhs.value.identifier_list).head = node;
			else
				((IdentifierListNode*)(yylhs.value.identifier_list).tail)->next = node;

			(yylhs.value.identifier_list).tail = node;
		}
	}
#line 1453 "syntactic.cpp"
    break;

  case 69: // instruction: full_opcode
#line 1197 "syntactic.y"
        {
		(yylhs.value.instruction).opcode = (yystack_[0].value.opcode);
		(yylhs.value.instruction).operands[0].type = OPERAND_NONE;
		(yylhs.value.instruction).operands[1].type = OPERAND_NONE;
	}
#line 1463 "syntactic.cpp"
    break;

  case 70: // instruction: full_opcode operand
#line 1203 "syntactic.y"
        {
		(yylhs.value.instruction).opcode = (yystack_[1].value.opcode);
		(yylhs.value.instruction).operands[0] = (yystack_[0].value.operand);
		(yylhs.value.instruction).operands[1].type = OPERAND_NONE;
	}
#line 1473 "syntactic.cpp"
    break;

  case 71: // instruction: full_opcode operand ',' operand
#line 1209 "syntactic.y"
        {
		(yylhs.value.instruction).opcode = (yystack_[3].value.opcode);
		(yylhs.value.instruction).operands[0] = (yystack_[2].value.operand);
		(yylhs.value.instruction).operands[1] = (yystack_[0].value.operand);
	}
#line 1483 "syntactic.cpp"
    break;

  case 72: // full_opcode: opcode
#line 1218 "syntactic.y"
        {
		(yylhs.value.opcode) = (yystack_[0].value.opcode);
		(yylhs.value.opcode).size = SIZE_UNDEFINED;
	}
#line 1492 "syntactic.cpp"
    break;

  case 73: // full_opcode: opcode '.'
#line 1223 "syntactic.y"
        {
		(yylhs.value.opcode) = (yystack_[1].value.opcode);
		(yylhs.value.opcode).size = SIZE_UNDEFINED;
		m68kasm_warning_pedantic(scanner, statement, "Opcode has a dot but no size; either remove the dot or add an explicit size.");
	}
#line 1502 "syntactic.cpp"
    break;

  case 74: // full_opcode: opcode size
#line 1229 "syntactic.y"
        {
		(yylhs.value.opcode) = (yystack_[1].value.opcode);
		(yylhs.value.opcode).size = (yystack_[0].value.size);
	}
#line 1511 "syntactic.cpp"
    break;

  case 75: // opcode: DIRECTIVE_ORI
#line 1237 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ORI;
	}
#line 1519 "syntactic.cpp"
    break;

  case 76: // opcode: DIRECTIVE_ANDI
#line 1241 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ANDI;
	}
#line 1527 "syntactic.cpp"
    break;

  case 77: // opcode: DIRECTIVE_SUBI
#line 1245 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_SUBI;
	}
#line 1535 "syntactic.cpp"
    break;

  case 78: // opcode: DIRECTIVE_ADDI
#line 1249 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ADDI;
	}
#line 1543 "syntactic.cpp"
    break;

  case 79: // opcode: DIRECTIVE_EORI
#line 1253 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_EORI;
	}
#line 1551 "syntactic.cpp"
    break;

  case 80: // opcode: DIRECTIVE_CMPI
#line 1257 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_CMPI;
	}
#line 1559 "syntactic.cpp"
    break;

  case 81: // opcode: DIRECTIVE_BTST
#line 1261 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_BTST_STATIC;
	}
#line 1567 "syntactic.cpp"
    break;

  case 82: // opcode: DIRECTIVE_BCHG
#line 1265 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_BCHG_STATIC;
	}
#line 1575 "syntactic.cpp"
    break;

  case 83: // opcode: DIRECTIVE_BCLR
#line 1269 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_BCLR_STATIC;
	}
#line 1583 "syntactic.cpp"
    break;

  case 84: // opcode: DIRECTIVE_BSET
#line 1273 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_BSET_STATIC;
	}
#line 1591 "syntactic.cpp"
    break;

  case 85: // opcode: DIRECTIVE_MOVEP
#line 1277 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_MOVEP_TO_REG;
	}
#line 1599 "syntactic.cpp"
    break;

  case 86: // opcode: DIRECTIVE_MOVEA
#line 1281 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_MOVEA;
	}
#line 1607 "syntactic.cpp"
    break;

  case 87: // opcode: DIRECTIVE_MOVE
#line 1285 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_MOVE;
	}
#line 1615 "syntactic.cpp"
    break;

  case 88: // opcode: DIRECTIVE_NEGX
#line 1289 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_NEGX;
	}
#line 1623 "syntactic.cpp"
    break;

  case 89: // opcode: DIRECTIVE_CLR
#line 1293 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_CLR;
	}
#line 1631 "syntactic.cpp"
    break;

  case 90: // opcode: DIRECTIVE_NEG
#line 1297 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_NEG;
	}
#line 1639 "syntactic.cpp"
    break;

  case 91: // opcode: DIRECTIVE_NOT
#line 1301 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_NOT;
	}
#line 1647 "syntactic.cpp"
    break;

  case 92: // opcode: DIRECTIVE_EXT
#line 1305 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_EXT;
	}
#line 1655 "syntactic.cpp"
    break;

  case 93: // opcode: DIRECTIVE_NBCD
#line 1309 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_NBCD;
	}
#line 1663 "syntactic.cpp"
    break;

  case 94: // opcode: DIRECTIVE_SWAP
#line 1313 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_SWAP;
	}
#line 1671 "syntactic.cpp"
    break;

  case 95: // opcode: DIRECTIVE_PEA
#line 1317 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_PEA;
	}
#line 1679 "syntactic.cpp"
    break;

  case 96: // opcode: DIRECTIVE_ILLEGAL
#line 1321 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ILLEGAL;
	}
#line 1687 "syntactic.cpp"
    break;

  case 97: // opcode: DIRECTIVE_TAS
#line 1325 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_TAS;
	}
#line 1695 "syntactic.cpp"
    break;

  case 98: // opcode: DIRECTIVE_TST
#line 1329 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_TST;
	}
#line 1703 "syntactic.cpp"
    break;

  case 99: // opcode: DIRECTIVE_TRAP
#line 1333 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_TRAP;
	}
#line 1711 "syntactic.cpp"
    break;

  case 100: // opcode: DIRECTIVE_LINK
#line 1337 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_LINK;
	}
#line 1719 "syntactic.cpp"
    break;

  case 101: // opcode: DIRECTIVE_UNLK
#line 1341 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_UNLK;
	}
#line 1727 "syntactic.cpp"
    break;

  case 102: // opcode: DIRECTIVE_RESET
#line 1345 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_RESET;
	}
#line 1735 "syntactic.cpp"
    break;

  case 103: // opcode: DIRECTIVE_NOP
#line 1349 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_NOP;
	}
#line 1743 "syntactic.cpp"
    break;

  case 104: // opcode: DIRECTIVE_STOP
#line 1353 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_STOP;
	}
#line 1751 "syntactic.cpp"
    break;

  case 105: // opcode: DIRECTIVE_RTE
#line 1357 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_RTE;
	}
#line 1759 "syntactic.cpp"
    break;

  case 106: // opcode: DIRECTIVE_RTS
#line 1361 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_RTS;
	}
#line 1767 "syntactic.cpp"
    break;

  case 107: // opcode: DIRECTIVE_TRAPV
#line 1365 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_TRAPV;
	}
#line 1775 "syntactic.cpp"
    break;

  case 108: // opcode: DIRECTIVE_RTR
#line 1369 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_RTR;
	}
#line 1783 "syntactic.cpp"
    break;

  case 109: // opcode: DIRECTIVE_JSR
#line 1373 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_JSR;
	}
#line 1791 "syntactic.cpp"
    break;

  case 110: // opcode: DIRECTIVE_JMP
#line 1377 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_JMP;
	}
#line 1799 "syntactic.cpp"
    break;

  case 111: // opcode: DIRECTIVE_MOVEM
#line 1381 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_MOVEM_TO_REGS;
	}
#line 1807 "syntactic.cpp"
    break;

  case 112: // opcode: DIRECTIVE_LEA
#line 1385 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_LEA;
	}
#line 1815 "syntactic.cpp"
    break;

  case 113: // opcode: DIRECTIVE_CHK
#line 1389 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_CHK;
	}
#line 1823 "syntactic.cpp"
    break;

  case 114: // opcode: DIRECTIVE_ADDQ
#line 1393 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ADDQ;
	}
#line 1831 "syntactic.cpp"
    break;

  case 115: // opcode: DIRECTIVE_SUBQ
#line 1397 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_SUBQ;
	}
#line 1839 "syntactic.cpp"
    break;

  case 116: // opcode: DIRECTIVE_ST
#line 1401 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_TRUE;
	}
#line 1848 "syntactic.cpp"
    break;

  case 117: // opcode: DIRECTIVE_SF
#line 1406 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_FALSE;
	}
#line 1857 "syntactic.cpp"
    break;

  case 118: // opcode: DIRECTIVE_SHI
#line 1411 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_HIGHER;
	}
#line 1866 "syntactic.cpp"
    break;

  case 119: // opcode: DIRECTIVE_SLS
#line 1416 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_LOWER_OR_SAME;
	}
#line 1875 "syntactic.cpp"
    break;

  case 120: // opcode: DIRECTIVE_SCC
#line 1421 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_CARRY_CLEAR;
	}
#line 1884 "syntactic.cpp"
    break;

  case 121: // opcode: DIRECTIVE_SCS
#line 1426 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_CARRY_SET;
	}
#line 1893 "syntactic.cpp"
    break;

  case 122: // opcode: DIRECTIVE_SNE
#line 1431 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_NOT_EQUAL;
	}
#line 1902 "syntactic.cpp"
    break;

  case 123: // opcode: DIRECTIVE_SEQ
#line 1436 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_EQUAL;
	}
#line 1911 "syntactic.cpp"
    break;

  case 124: // opcode: DIRECTIVE_SVC
#line 1441 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 1920 "syntactic.cpp"
    break;

  case 125: // opcode: DIRECTIVE_SVS
#line 1446 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_OVERFLOW_SET;
	}
#line 1929 "syntactic.cpp"
    break;

  case 126: // opcode: DIRECTIVE_SPL
#line 1451 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_PLUS;
	}
#line 1938 "syntactic.cpp"
    break;

  case 127: // opcode: DIRECTIVE_SMI
#line 1456 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_MINUS;
	}
#line 1947 "syntactic.cpp"
    break;

  case 128: // opcode: DIRECTIVE_SGE
#line 1461 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 1956 "syntactic.cpp"
    break;

  case 129: // opcode: DIRECTIVE_SLT
#line 1466 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_LESS_THAN;
	}
#line 1965 "syntactic.cpp"
    break;

  case 130: // opcode: DIRECTIVE_SGT
#line 1471 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_GREATER_THAN;
	}
#line 1974 "syntactic.cpp"
    break;

  case 131: // opcode: DIRECTIVE_SLE
#line 1476 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Scc;
		(yylhs.value.opcode).condition = CONDITION_LESS_OR_EQUAL;
	}
#line 1983 "syntactic.cpp"
    break;

  case 132: // opcode: DIRECTIVE_DBT
#line 1481 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_TRUE;
	}
#line 1992 "syntactic.cpp"
    break;

  case 133: // opcode: DIRECTIVE_DBF
#line 1486 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_FALSE;
	}
#line 2001 "syntactic.cpp"
    break;

  case 134: // opcode: DIRECTIVE_DBHI
#line 1491 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_HIGHER;
	}
#line 2010 "syntactic.cpp"
    break;

  case 135: // opcode: DIRECTIVE_DBLS
#line 1496 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_LOWER_OR_SAME;
	}
#line 2019 "syntactic.cpp"
    break;

  case 136: // opcode: DIRECTIVE_DBCC
#line 1501 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_CARRY_CLEAR;
	}
#line 2028 "syntactic.cpp"
    break;

  case 137: // opcode: DIRECTIVE_DBCS
#line 1506 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_CARRY_SET;
	}
#line 2037 "syntactic.cpp"
    break;

  case 138: // opcode: DIRECTIVE_DBNE
#line 1511 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_NOT_EQUAL;
	}
#line 2046 "syntactic.cpp"
    break;

  case 139: // opcode: DIRECTIVE_DBEQ
#line 1516 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_EQUAL;
	}
#line 2055 "syntactic.cpp"
    break;

  case 140: // opcode: DIRECTIVE_DBVC
#line 1521 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 2064 "syntactic.cpp"
    break;

  case 141: // opcode: DIRECTIVE_DBVS
#line 1526 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_OVERFLOW_SET;
	}
#line 2073 "syntactic.cpp"
    break;

  case 142: // opcode: DIRECTIVE_DBPL
#line 1531 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_PLUS;
	}
#line 2082 "syntactic.cpp"
    break;

  case 143: // opcode: DIRECTIVE_DBMI
#line 1536 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_MINUS;
	}
#line 2091 "syntactic.cpp"
    break;

  case 144: // opcode: DIRECTIVE_DBGE
#line 1541 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2100 "syntactic.cpp"
    break;

  case 145: // opcode: DIRECTIVE_DBLT
#line 1546 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_LESS_THAN;
	}
#line 2109 "syntactic.cpp"
    break;

  case 146: // opcode: DIRECTIVE_DBGT
#line 1551 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_GREATER_THAN;
	}
#line 2118 "syntactic.cpp"
    break;

  case 147: // opcode: DIRECTIVE_DBLE
#line 1556 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DBcc;
		(yylhs.value.opcode).condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2127 "syntactic.cpp"
    break;

  case 148: // opcode: DIRECTIVE_BRA
#line 1561 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_BRA;
	}
#line 2135 "syntactic.cpp"
    break;

  case 149: // opcode: DIRECTIVE_BSR
#line 1565 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_BSR;
	}
#line 2143 "syntactic.cpp"
    break;

  case 150: // opcode: DIRECTIVE_BHI
#line 1569 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_HIGHER;
	}
#line 2152 "syntactic.cpp"
    break;

  case 151: // opcode: DIRECTIVE_BLS
#line 1574 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_LOWER_OR_SAME;
	}
#line 2161 "syntactic.cpp"
    break;

  case 152: // opcode: DIRECTIVE_BCC
#line 1579 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_CARRY_CLEAR;
	}
#line 2170 "syntactic.cpp"
    break;

  case 153: // opcode: DIRECTIVE_BCS
#line 1584 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_CARRY_SET;
	}
#line 2179 "syntactic.cpp"
    break;

  case 154: // opcode: DIRECTIVE_BNE
#line 1589 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_NOT_EQUAL;
	}
#line 2188 "syntactic.cpp"
    break;

  case 155: // opcode: DIRECTIVE_BEQ
#line 1594 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_EQUAL;
	}
#line 2197 "syntactic.cpp"
    break;

  case 156: // opcode: DIRECTIVE_BVC
#line 1599 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_OVERFLOW_CLEAR;
	}
#line 2206 "syntactic.cpp"
    break;

  case 157: // opcode: DIRECTIVE_BVS
#line 1604 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_OVERFLOW_SET;
	}
#line 2215 "syntactic.cpp"
    break;

  case 158: // opcode: DIRECTIVE_BPL
#line 1609 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_PLUS;
	}
#line 2224 "syntactic.cpp"
    break;

  case 159: // opcode: DIRECTIVE_BMI
#line 1614 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_MINUS;
	}
#line 2233 "syntactic.cpp"
    break;

  case 160: // opcode: DIRECTIVE_BGE
#line 1619 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_GREATER_OR_EQUAL;
	}
#line 2242 "syntactic.cpp"
    break;

  case 161: // opcode: DIRECTIVE_BLT
#line 1624 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_LESS_THAN;
	}
#line 2251 "syntactic.cpp"
    break;

  case 162: // opcode: DIRECTIVE_BGT
#line 1629 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_GREATER_THAN;
	}
#line 2260 "syntactic.cpp"
    break;

  case 163: // opcode: DIRECTIVE_BLE
#line 1634 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_Bcc;
		(yylhs.value.opcode).condition = CONDITION_LESS_OR_EQUAL;
	}
#line 2269 "syntactic.cpp"
    break;

  case 164: // opcode: DIRECTIVE_MOVEQ
#line 1639 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_MOVEQ;
	}
#line 2277 "syntactic.cpp"
    break;

  case 165: // opcode: DIRECTIVE_DIVU
#line 1643 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DIVU;
	}
#line 2285 "syntactic.cpp"
    break;

  case 166: // opcode: DIRECTIVE_DIVS
#line 1647 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_DIVS;
	}
#line 2293 "syntactic.cpp"
    break;

  case 167: // opcode: DIRECTIVE_SBCD
#line 1651 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_SBCD_DATA_REGS;
	}
#line 2301 "syntactic.cpp"
    break;

  case 168: // opcode: DIRECTIVE_OR
#line 1655 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_OR_TO_REG;
	}
#line 2309 "syntactic.cpp"
    break;

  case 169: // opcode: DIRECTIVE_SUB
#line 1659 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_SUB_TO_REG;
	}
#line 2317 "syntactic.cpp"
    break;

  case 170: // opcode: DIRECTIVE_SUBX
#line 1663 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_SUBX_DATA_REGS;
	}
#line 2325 "syntactic.cpp"
    break;

  case 171: // opcode: DIRECTIVE_SUBA
#line 1667 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_SUBA;
	}
#line 2333 "syntactic.cpp"
    break;

  case 172: // opcode: DIRECTIVE_EOR
#line 1671 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_EOR;
	}
#line 2341 "syntactic.cpp"
    break;

  case 173: // opcode: DIRECTIVE_CMPM
#line 1675 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_CMPM;
	}
#line 2349 "syntactic.cpp"
    break;

  case 174: // opcode: DIRECTIVE_CMP
#line 1679 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_CMP;
	}
#line 2357 "syntactic.cpp"
    break;

  case 175: // opcode: DIRECTIVE_CMPA
#line 1683 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_CMPA;
	}
#line 2365 "syntactic.cpp"
    break;

  case 176: // opcode: DIRECTIVE_MULU
#line 1687 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_MULU;
	}
#line 2373 "syntactic.cpp"
    break;

  case 177: // opcode: DIRECTIVE_MULS
#line 1691 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_MULS;
	}
#line 2381 "syntactic.cpp"
    break;

  case 178: // opcode: DIRECTIVE_ABCD
#line 1695 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ABCD_DATA_REGS;
	}
#line 2389 "syntactic.cpp"
    break;

  case 179: // opcode: DIRECTIVE_EXG
#line 1699 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_EXG;
	}
#line 2397 "syntactic.cpp"
    break;

  case 180: // opcode: DIRECTIVE_AND
#line 1703 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_AND_TO_REG;
	}
#line 2405 "syntactic.cpp"
    break;

  case 181: // opcode: DIRECTIVE_ADD
#line 1707 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ADD_TO_REG;
	}
#line 2413 "syntactic.cpp"
    break;

  case 182: // opcode: DIRECTIVE_ADDX
#line 1711 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ADDX_DATA_REGS;
	}
#line 2421 "syntactic.cpp"
    break;

  case 183: // opcode: DIRECTIVE_ADDA
#line 1715 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ADDA;
	}
#line 2429 "syntactic.cpp"
    break;

  case 184: // opcode: DIRECTIVE_ASL
#line 1719 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ASL_STATIC;
	}
#line 2437 "syntactic.cpp"
    break;

  case 185: // opcode: DIRECTIVE_ASR
#line 1723 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ASR_STATIC;
	}
#line 2445 "syntactic.cpp"
    break;

  case 186: // opcode: DIRECTIVE_LSL
#line 1727 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_LSL_STATIC;
	}
#line 2453 "syntactic.cpp"
    break;

  case 187: // opcode: DIRECTIVE_LSR
#line 1731 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_LSR_STATIC;
	}
#line 2461 "syntactic.cpp"
    break;

  case 188: // opcode: DIRECTIVE_ROXL
#line 1735 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ROXL_STATIC;
	}
#line 2469 "syntactic.cpp"
    break;

  case 189: // opcode: DIRECTIVE_ROXR
#line 1739 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ROXR_STATIC;
	}
#line 2477 "syntactic.cpp"
    break;

  case 190: // opcode: DIRECTIVE_ROL
#line 1743 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ROL_STATIC;
	}
#line 2485 "syntactic.cpp"
    break;

  case 191: // opcode: DIRECTIVE_ROR
#line 1747 "syntactic.y"
        {
		(yylhs.value.opcode).type = OPCODE_ROR_STATIC;
	}
#line 2493 "syntactic.cpp"
    break;

  case 192: // size: SIZE_BYTE
#line 1754 "syntactic.y"
        {
		(yylhs.value.size) = SIZE_BYTE;
	}
#line 2501 "syntactic.cpp"
    break;

  case 193: // size: SIZE_SHORT
#line 1758 "syntactic.y"
        {
		(yylhs.value.size) = SIZE_SHORT;
	}
#line 2509 "syntactic.cpp"
    break;

  case 194: // size: SIZE_WORD
#line 1762 "syntactic.y"
        {
		(yylhs.value.size) = SIZE_WORD;
	}
#line 2517 "syntactic.cpp"
    break;

  case 195: // size: SIZE_LONGWORD
#line 1766 "syntactic.y"
        {
		(yylhs.value.size) = SIZE_LONGWORD;
	}
#line 2525 "syntactic.cpp"
    break;

  case 196: // operand: '(' ADDRESS_REGISTER ')'
#line 1774 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT;
		(yylhs.value.operand).main_register = (yystack_[1].value.unsigned_long);
	}
#line 2534 "syntactic.cpp"
    break;

  case 197: // operand: '(' ADDRESS_REGISTER ')' '+'
#line 1779 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
		(yylhs.value.operand).main_register = (yystack_[2].value.unsigned_long);
	}
#line 2543 "syntactic.cpp"
    break;

  case 198: // operand: '-' '(' ADDRESS_REGISTER ')'
#line 1784 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
		(yylhs.value.operand).main_register = (yystack_[1].value.unsigned_long);
	}
#line 2552 "syntactic.cpp"
    break;

  case 199: // operand: expression '(' ADDRESS_REGISTER ')'
#line 1789 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		(yylhs.value.operand).literal = (yystack_[3].value.expression);
		(yylhs.value.operand).main_register = (yystack_[1].value.unsigned_long);
	}
#line 2562 "syntactic.cpp"
    break;

  case 200: // operand: '(' expression ',' ADDRESS_REGISTER ')'
#line 1795 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		(yylhs.value.operand).literal = (yystack_[3].value.expression);
		(yylhs.value.operand).main_register = (yystack_[1].value.unsigned_long);
	}
#line 2572 "syntactic.cpp"
    break;

  case 201: // operand: '(' ADDRESS_REGISTER ',' expression ')'
#line 1801 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
		(yylhs.value.operand).literal = (yystack_[1].value.expression);
		(yylhs.value.operand).main_register = (yystack_[3].value.unsigned_long);
	}
#line 2582 "syntactic.cpp"
    break;

  case 202: // operand: '(' ADDRESS_REGISTER ',' data_or_address_register size ')'
#line 1807 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal.type = EXPRESSION_NUMBER;
		(yylhs.value.operand).literal.shared.unsigned_long = 0;
		(yylhs.value.operand).main_register = (yystack_[4].value.unsigned_long);
		(yylhs.value.operand).index_register = (yystack_[2].value.unsigned_long) % 8;
		(yylhs.value.operand).size = (yystack_[1].value.size);
		(yylhs.value.operand).index_register_is_address_register = (yystack_[2].value.unsigned_long) / 8 != 0;
	}
#line 2596 "syntactic.cpp"
    break;

  case 203: // operand: expression '(' ADDRESS_REGISTER ',' data_or_address_register size ')'
#line 1817 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal = (yystack_[6].value.expression);
		(yylhs.value.operand).main_register = (yystack_[4].value.unsigned_long);
		(yylhs.value.operand).index_register = (yystack_[2].value.unsigned_long) % 8;
		(yylhs.value.operand).size = (yystack_[1].value.size);
		(yylhs.value.operand).index_register_is_address_register = (yystack_[2].value.unsigned_long) / 8 !=0;
	}
#line 2609 "syntactic.cpp"
    break;

  case 204: // operand: '(' expression ',' ADDRESS_REGISTER ',' data_or_address_register size ')'
#line 1826 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal = (yystack_[6].value.expression);
		(yylhs.value.operand).main_register = (yystack_[4].value.unsigned_long);
		(yylhs.value.operand).index_register = (yystack_[2].value.unsigned_long) % 8;
		(yylhs.value.operand).size = (yystack_[1].value.size);
		(yylhs.value.operand).index_register_is_address_register = (yystack_[2].value.unsigned_long) / 8 !=0;
	}
#line 2622 "syntactic.cpp"
    break;

  case 205: // operand: '(' ADDRESS_REGISTER ',' data_or_address_register ')'
#line 1835 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal.type = EXPRESSION_NUMBER;
		(yylhs.value.operand).literal.shared.unsigned_long = 0;
		(yylhs.value.operand).main_register = (yystack_[3].value.unsigned_long);
		(yylhs.value.operand).index_register = (yystack_[1].value.unsigned_long) % 8;
		(yylhs.value.operand).size = SIZE_WORD;
		(yylhs.value.operand).index_register_is_address_register = (yystack_[1].value.unsigned_long) / 8 != 0;
	}
#line 2637 "syntactic.cpp"
    break;

  case 206: // operand: expression '(' ADDRESS_REGISTER ',' data_or_address_register ')'
#line 1846 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal = (yystack_[5].value.expression);
		(yylhs.value.operand).main_register = (yystack_[3].value.unsigned_long);
		(yylhs.value.operand).index_register = (yystack_[1].value.unsigned_long) % 8;
		(yylhs.value.operand).size = SIZE_WORD;
		(yylhs.value.operand).index_register_is_address_register = (yystack_[1].value.unsigned_long) / 8 !=0;
	}
#line 2651 "syntactic.cpp"
    break;

  case 207: // operand: '(' expression ',' ADDRESS_REGISTER ',' data_or_address_register ')'
#line 1856 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal = (yystack_[5].value.expression);
		(yylhs.value.operand).main_register = (yystack_[3].value.unsigned_long);
		(yylhs.value.operand).index_register = (yystack_[1].value.unsigned_long) % 8;
		(yylhs.value.operand).size = SIZE_WORD;
		(yylhs.value.operand).index_register_is_address_register = (yystack_[1].value.unsigned_long) / 8 !=0;
	}
#line 2665 "syntactic.cpp"
    break;

  case 208: // operand: expression '(' PROGRAM_COUNTER ')'
#line 1866 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		(yylhs.value.operand).literal = (yystack_[3].value.expression);
	}
#line 2674 "syntactic.cpp"
    break;

  case 209: // operand: '(' expression ',' PROGRAM_COUNTER ')'
#line 1871 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		(yylhs.value.operand).literal = (yystack_[3].value.expression);
	}
#line 2683 "syntactic.cpp"
    break;

  case 210: // operand: '(' PROGRAM_COUNTER ',' expression ')'
#line 1876 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
		(yylhs.value.operand).literal = (yystack_[1].value.expression);
	}
#line 2692 "syntactic.cpp"
    break;

  case 211: // operand: '(' PROGRAM_COUNTER ',' data_or_address_register size ')'
#line 1881 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal.type = EXPRESSION_NUMBER;
		(yylhs.value.operand).literal.shared.unsigned_long = 0;
		(yylhs.value.operand).index_register = (yystack_[2].value.unsigned_long) % 8;
		(yylhs.value.operand).size = (yystack_[1].value.size);
		(yylhs.value.operand).index_register_is_address_register = (yystack_[2].value.unsigned_long) / 8 != 0;
	}
#line 2705 "syntactic.cpp"
    break;

  case 212: // operand: expression '(' PROGRAM_COUNTER ',' data_or_address_register size ')'
#line 1890 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal = (yystack_[6].value.expression);
		(yylhs.value.operand).index_register = (yystack_[2].value.unsigned_long) % 8;
		(yylhs.value.operand).size = (yystack_[1].value.size);
		(yylhs.value.operand).index_register_is_address_register = (yystack_[2].value.unsigned_long) / 8 != 0;
	}
#line 2717 "syntactic.cpp"
    break;

  case 213: // operand: '(' expression ',' PROGRAM_COUNTER ',' data_or_address_register size ')'
#line 1898 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal = (yystack_[6].value.expression);
		(yylhs.value.operand).index_register = (yystack_[2].value.unsigned_long) % 8;
		(yylhs.value.operand).size = (yystack_[1].value.size);
		(yylhs.value.operand).index_register_is_address_register = (yystack_[2].value.unsigned_long) / 8 != 0;
	}
#line 2729 "syntactic.cpp"
    break;

  case 214: // operand: '(' PROGRAM_COUNTER ',' data_or_address_register ')'
#line 1906 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yylhs.value.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal.type = EXPRESSION_NUMBER;
		(yylhs.value.operand).literal.shared.unsigned_long = 0;
		(yylhs.value.operand).index_register = (yystack_[1].value.unsigned_long) % 8;
		(yylhs.value.operand).size = SIZE_WORD;
		(yylhs.value.operand).index_register_is_address_register = (yystack_[1].value.unsigned_long) / 8 != 0;
	}
#line 2743 "syntactic.cpp"
    break;

  case 215: // operand: expression '(' PROGRAM_COUNTER ',' data_or_address_register ')'
#line 1916 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yylhs.value.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal = (yystack_[5].value.expression);
		(yylhs.value.operand).index_register = (yystack_[1].value.unsigned_long) % 8;
		(yylhs.value.operand).size = SIZE_WORD;
		(yylhs.value.operand).index_register_is_address_register = (yystack_[1].value.unsigned_long) / 8 != 0;
	}
#line 2756 "syntactic.cpp"
    break;

  case 216: // operand: '(' expression ',' PROGRAM_COUNTER ',' data_or_address_register ')'
#line 1925 "syntactic.y"
        {
		m68kasm_warning_pedantic(scanner, statement, "Index register lacks a size specifier (assuming word-size for now, but you should really add an explicit size).");
		(yylhs.value.operand).type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
		(yylhs.value.operand).literal = (yystack_[5].value.expression);
		(yylhs.value.operand).index_register = (yystack_[1].value.unsigned_long) % 8;
		(yylhs.value.operand).size = SIZE_WORD;
		(yylhs.value.operand).index_register_is_address_register = (yystack_[1].value.unsigned_long) / 8 != 0;
	}
#line 2769 "syntactic.cpp"
    break;

  case 217: // operand: '#' expression
#line 1935 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_LITERAL;
		(yylhs.value.operand).literal = (yystack_[0].value.expression);
	}
#line 2778 "syntactic.cpp"
    break;

  case 218: // operand: STATUS_REGISTER
#line 1951 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_STATUS_REGISTER;
	}
#line 2786 "syntactic.cpp"
    break;

  case 219: // operand: CONDITION_CODE_REGISTER
#line 1955 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_CONDITION_CODE_REGISTER;
	}
#line 2794 "syntactic.cpp"
    break;

  case 220: // operand: USER_STACK_POINTER_REGISTER
#line 1959 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_USER_STACK_POINTER_REGISTER;
	}
#line 2802 "syntactic.cpp"
    break;

  case 221: // operand: expression
#line 1964 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_ADDRESS;
		(yylhs.value.operand).literal = (yystack_[0].value.expression);
		(yylhs.value.operand).size = SIZE_UNDEFINED;
	}
#line 2812 "syntactic.cpp"
    break;

  case 222: // operand: expression size
#line 1970 "syntactic.y"
        {
		(yylhs.value.operand).type = OPERAND_ADDRESS_ABSOLUTE;
		(yylhs.value.operand).literal = (yystack_[1].value.expression);
		(yylhs.value.operand).size = (yystack_[0].value.size);
	}
#line 2822 "syntactic.cpp"
    break;

  case 223: // operand: register_list
#line 1977 "syntactic.y"
        {
		/* Neat trick to check if only one bit is set (yes, I know that it evaluates to true for 0, but that doesn't matter here). */
		if (((yystack_[0].value.unsigned_long) & ((yystack_[0].value.unsigned_long) - 1)) == 0)
		{
			/* This is a single register. */
			(yylhs.value.operand).main_register = 0;

			while (((yystack_[0].value.unsigned_long) & (1 << (yylhs.value.operand).main_register)) == 0)
				++(yylhs.value.operand).main_register;

			if ((yylhs.value.operand).main_register >= 8)
			{
				(yylhs.value.operand).type = OPERAND_ADDRESS_REGISTER;
				(yylhs.value.operand).main_register -= 8;
			}
			else
			{
				(yylhs.value.operand).type = OPERAND_DATA_REGISTER;
			}
		}
		else
		{
			/* This is multiple registers. */
			(yylhs.value.operand).type = OPERAND_REGISTER_LIST;
			(yylhs.value.operand).main_register = (yystack_[0].value.unsigned_long); /* Such a hack... */
		}
	}
#line 2854 "syntactic.cpp"
    break;

  case 224: // register_list: register_span
#line 2008 "syntactic.y"
        {
		(yylhs.value.unsigned_long) = (yystack_[0].value.unsigned_long);
	}
#line 2862 "syntactic.cpp"
    break;

  case 225: // register_list: register_span '/' register_list
#line 2012 "syntactic.y"
        {
		(yylhs.value.unsigned_long) = (yystack_[2].value.unsigned_long) | (yystack_[0].value.unsigned_long);
	}
#line 2870 "syntactic.cpp"
    break;

  case 226: // register_span: data_or_address_register
#line 2019 "syntactic.y"
        {
		(yylhs.value.unsigned_long) = 1 << (yystack_[0].value.unsigned_long);
	}
#line 2878 "syntactic.cpp"
    break;

  case 227: // register_span: data_or_address_register '-' data_or_address_register
#line 2023 "syntactic.y"
        {
		unsigned long start;
		unsigned long end;
		unsigned long i;

		if ((yystack_[2].value.unsigned_long) < (yystack_[0].value.unsigned_long))
		{
			start = (yystack_[2].value.unsigned_long);
			end = (yystack_[0].value.unsigned_long);
		}
		else
		{
			start = (yystack_[0].value.unsigned_long);
			end = (yystack_[2].value.unsigned_long);
		}

		(yylhs.value.unsigned_long) = 0;

		for (i = start; i <= end; ++i)
			(yylhs.value.unsigned_long) |= 1 << i;
	}
#line 2904 "syntactic.cpp"
    break;

  case 228: // data_or_address_register: DATA_REGISTER
#line 2048 "syntactic.y"
        {
		(yylhs.value.unsigned_long) = 0 + (yystack_[0].value.unsigned_long);
	}
#line 2912 "syntactic.cpp"
    break;

  case 229: // data_or_address_register: ADDRESS_REGISTER
#line 2052 "syntactic.y"
        {
		(yylhs.value.unsigned_long) = 8 + (yystack_[0].value.unsigned_long);
	}
#line 2920 "syntactic.cpp"
    break;

  case 230: // expression: expression1
#line 2065 "syntactic.y"
        {
		(yylhs.value.expression) = (yystack_[0].value.expression);
	}
#line 2928 "syntactic.cpp"
    break;

  case 231: // expression: expression LOGICAL_AND expression1
#line 2070 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_LOGICAL_AND, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 2937 "syntactic.cpp"
    break;

  case 232: // expression: expression LOGICAL_OR expression1
#line 2076 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_LOGICAL_OR, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 2946 "syntactic.cpp"
    break;

  case 233: // expression1: expression2
#line 2084 "syntactic.y"
        {
		(yylhs.value.expression) = (yystack_[0].value.expression);
	}
#line 2954 "syntactic.cpp"
    break;

  case 234: // expression1: expression1 '=' expression2
#line 2088 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_EQUALITY, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 2963 "syntactic.cpp"
    break;

  case 235: // expression1: expression1 EQUALITY expression2
#line 2093 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_EQUALITY, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 2972 "syntactic.cpp"
    break;

  case 236: // expression1: expression1 INEQUALITY expression2
#line 2098 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_INEQUALITY, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 2981 "syntactic.cpp"
    break;

  case 237: // expression2: expression3
#line 2106 "syntactic.y"
        {
		(yylhs.value.expression) = (yystack_[0].value.expression);
	}
#line 2989 "syntactic.cpp"
    break;

  case 238: // expression2: expression2 '<' expression3
#line 2110 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_LESS_THAN, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 2998 "syntactic.cpp"
    break;

  case 239: // expression2: expression2 LESS_OR_EQUAL expression3
#line 2115 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_LESS_OR_EQUAL, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3007 "syntactic.cpp"
    break;

  case 240: // expression2: expression2 '>' expression3
#line 2120 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_MORE_THAN, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3016 "syntactic.cpp"
    break;

  case 241: // expression2: expression2 MORE_OR_EQUAL expression3
#line 2125 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_MORE_OR_EQUAL, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3025 "syntactic.cpp"
    break;

  case 242: // expression3: expression4
#line 2133 "syntactic.y"
        {
		(yylhs.value.expression) = (yystack_[0].value.expression);
	}
#line 3033 "syntactic.cpp"
    break;

  case 243: // expression3: expression3 '+' expression4
#line 2137 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_ADD, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3042 "syntactic.cpp"
    break;

  case 244: // expression3: expression3 '-' expression4
#line 2142 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_SUBTRACT, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3051 "syntactic.cpp"
    break;

  case 245: // expression4: expression5
#line 2150 "syntactic.y"
        {
		(yylhs.value.expression) = (yystack_[0].value.expression);
	}
#line 3059 "syntactic.cpp"
    break;

  case 246: // expression4: expression4 '*' expression5
#line 2154 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_MULTIPLY, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3068 "syntactic.cpp"
    break;

  case 247: // expression4: expression4 '/' expression5
#line 2159 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_DIVIDE, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3077 "syntactic.cpp"
    break;

  case 248: // expression4: expression4 '%' expression5
#line 2164 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_MODULO, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3086 "syntactic.cpp"
    break;

  case 249: // expression5: expression6
#line 2172 "syntactic.y"
        {
		(yylhs.value.expression) = (yystack_[0].value.expression);
	}
#line 3094 "syntactic.cpp"
    break;

  case 250: // expression5: expression5 '&' expression6
#line 2176 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_BITWISE_AND, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3103 "syntactic.cpp"
    break;

  case 251: // expression5: expression5 '!' expression6
#line 2181 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_BITWISE_OR, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3112 "syntactic.cpp"
    break;

  case 252: // expression5: expression5 '|' expression6
#line 2186 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_BITWISE_OR, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3121 "syntactic.cpp"
    break;

  case 253: // expression5: expression5 '^' expression6
#line 2191 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_BITWISE_XOR, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3130 "syntactic.cpp"
    break;

  case 254: // expression6: expression7
#line 2199 "syntactic.y"
        {
		(yylhs.value.expression) = (yystack_[0].value.expression);
	}
#line 3138 "syntactic.cpp"
    break;

  case 255: // expression6: expression6 LEFT_SHIFT expression7
#line 2203 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_LEFT_SHIFT, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3147 "syntactic.cpp"
    break;

  case 256: // expression6: expression6 RIGHT_SHIFT expression7
#line 2208 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_RIGHT_SHIFT, &(yystack_[2].value.expression), &(yystack_[0].value.expression)))
			YYNOMEM;
	}
#line 3156 "syntactic.cpp"
    break;

  case 257: // expression7: expression8
#line 2216 "syntactic.y"
        {
		(yylhs.value.expression) = (yystack_[0].value.expression);
	}
#line 3164 "syntactic.cpp"
    break;

  case 258: // expression7: '+' expression7
#line 2220 "syntactic.y"
        {
		(yylhs.value.expression) = (yystack_[0].value.expression);
	}
#line 3172 "syntactic.cpp"
    break;

  case 259: // expression7: '-' expression7
#line 2224 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_NEGATE, &(yystack_[0].value.expression), NULL))
			YYNOMEM;
	}
#line 3181 "syntactic.cpp"
    break;

  case 260: // expression7: '~' expression7
#line 2229 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_BITWISE_NOT, &(yystack_[0].value.expression), NULL))
			YYNOMEM;
	}
#line 3190 "syntactic.cpp"
    break;

  case 261: // expression7: '!' expression7
#line 2235 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_LOGICAL_NOT, &(yystack_[0].value.expression), NULL))
			YYNOMEM;
	}
#line 3199 "syntactic.cpp"
    break;

  case 262: // local_identifier: LOCAL_IDENTIFIER
#line 2242 "syntactic.y"
          { (yylhs.value.string) = (yystack_[0].value.string); }
#line 3205 "syntactic.cpp"
    break;

  case 263: // local_identifier: SIZE_BYTE
#line 2244 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".b");
		if (!String_CreateCopyView(&(yylhs.value.string), &literal))
			YYNOMEM;
	}
#line 3215 "syntactic.cpp"
    break;

  case 264: // local_identifier: SIZE_SHORT
#line 2250 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".s");
		if (!String_CreateCopyView(&(yylhs.value.string), &literal))
			YYNOMEM;
	}
#line 3225 "syntactic.cpp"
    break;

  case 265: // local_identifier: SIZE_WORD
#line 2256 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".w");
		if (!String_CreateCopyView(&(yylhs.value.string), &literal))
			YYNOMEM;
	}
#line 3235 "syntactic.cpp"
    break;

  case 266: // local_identifier: SIZE_LONGWORD
#line 2262 "syntactic.y"
        {
		const StringView literal = STRING_VIEW_INITIALISER(".l");
		if (!String_CreateCopyView(&(yylhs.value.string), &literal))
			YYNOMEM;
	}
#line 3245 "syntactic.cpp"
    break;

  case 267: // identifier: IDENTIFIER
#line 2270 "syntactic.y"
          { (yylhs.value.string) = (yystack_[0].value.string); }
#line 3251 "syntactic.cpp"
    break;

  case 268: // identifier: local_identifier
#line 2271 "syntactic.y"
          { (yylhs.value.string) = (yystack_[0].value.string); }
#line 3257 "syntactic.cpp"
    break;

  case 269: // identifier: identifier local_identifier
#line 2273 "syntactic.y"
        {
		const cc_bool success = String_CreateAppend(&(yylhs.value.string), &(yystack_[1].value.string), &(yystack_[0].value.string));
		String_Destroy(&(yystack_[1].value.string));
		String_Destroy(&(yystack_[0].value.string));

		if (!success)
			YYNOMEM;
	}
#line 3270 "syntactic.cpp"
    break;

  case 270: // expression8: NUMBER
#line 2285 "syntactic.y"
        {
		(yylhs.value.expression).type = EXPRESSION_NUMBER;
		(yylhs.value.expression).shared.unsigned_long = (yystack_[0].value.unsigned_long);
	}
#line 3279 "syntactic.cpp"
    break;

  case 271: // expression8: identifier
#line 2290 "syntactic.y"
        {
		(yylhs.value.expression).type = EXPRESSION_IDENTIFIER;
		(yylhs.value.expression).shared.string = (yystack_[0].value.string);
	}
#line 3288 "syntactic.cpp"
    break;

  case 272: // expression8: string
#line 2295 "syntactic.y"
        {
		(yylhs.value.expression) = (yystack_[0].value.expression);
	}
#line 3296 "syntactic.cpp"
    break;

  case 273: // expression8: '*'
#line 2299 "syntactic.y"
        {
		(yylhs.value.expression).type = EXPRESSION_PROGRAM_COUNTER_OF_STATEMENT;
	}
#line 3304 "syntactic.cpp"
    break;

  case 274: // expression8: '@'
#line 2303 "syntactic.y"
        {
		(yylhs.value.expression).type = EXPRESSION_PROGRAM_COUNTER_OF_EXPRESSION;
	}
#line 3312 "syntactic.cpp"
    break;

  case 275: // expression8: '(' expression ')'
#line 2307 "syntactic.y"
        {
		(yylhs.value.expression) = (yystack_[1].value.expression);
	}
#line 3320 "syntactic.cpp"
    break;

  case 276: // expression8: STRLEN '(' STRING ')'
#line 2311 "syntactic.y"
        {
		(yylhs.value.expression).type = EXPRESSION_STRLEN;
		(yylhs.value.expression).shared.string = (yystack_[1].value.string);
	}
#line 3329 "syntactic.cpp"
    break;

  case 277: // expression8: STRCMP '(' string ',' string ')'
#line 2316 "syntactic.y"
        {
		if (!DoExpression(&(yylhs.value.expression), EXPRESSION_STRCMP, &(yystack_[3].value.expression), &(yystack_[1].value.expression)))
			YYNOMEM;
	}
#line 3338 "syntactic.cpp"
    break;

  case 278: // expression8: INSTR '(' expression ',' expression ')'
#line 2321 "syntactic.y"
        {
		Expression expression;

		expression.type = EXPRESSION_NUMBER;
		expression.shared.unsigned_long = 1;

		if (!DoExpressionTriple(&(yylhs.value.expression), EXPRESSION_INSTR, &expression, &(yystack_[3].value.expression), &(yystack_[1].value.expression)))
			YYNOMEM;
	}
#line 3352 "syntactic.cpp"
    break;

  case 279: // expression8: INSTR '(' expression ',' expression ',' expression ')'
#line 2331 "syntactic.y"
        {
		if (!DoExpressionTriple(&(yylhs.value.expression), EXPRESSION_INSTR, &(yystack_[5].value.expression), &(yystack_[3].value.expression), &(yystack_[1].value.expression)))
			YYNOMEM;
	}
#line 3361 "syntactic.cpp"
    break;

  case 280: // expression8: DEF '(' IDENTIFIER ')'
#line 2336 "syntactic.y"
        {
		(yylhs.value.expression).type = EXPRESSION_DEF;
		(yylhs.value.expression).shared.string = (yystack_[1].value.string);
	}
#line 3370 "syntactic.cpp"
    break;

  case 281: // expression8: TYPE '(' IDENTIFIER ')'
#line 2341 "syntactic.y"
        {
		(yylhs.value.expression).type = EXPRESSION_TYPE_WITH_IDENTIFIER;
		(yylhs.value.expression).shared.string = (yystack_[1].value.string);
	}
#line 3379 "syntactic.cpp"
    break;

  case 282: // expression8: TYPE '(' NUMBER ')'
#line 2346 "syntactic.y"
        {
		(yylhs.value.expression).type = EXPRESSION_TYPE_WITH_NUMBER;
		(yylhs.value.expression).shared.unsigned_long = (yystack_[1].value.unsigned_long);
	}
#line 3388 "syntactic.cpp"
    break;

  case 283: // expression8: FILESIZE '(' STRING ')'
#line 2351 "syntactic.y"
        {
		(yylhs.value.expression).type = EXPRESSION_FILESIZE;
		(yylhs.value.expression).shared.string = (yystack_[1].value.string);
	}
#line 3397 "syntactic.cpp"
    break;

  case 284: // expression8: OFFSET '(' expression ')'
#line 2356 "syntactic.y"
        {
		/* TODO: Make this actually do something besides pass the expression through. */
		(yylhs.value.expression) = (yystack_[1].value.expression);
	}
#line 3406 "syntactic.cpp"
    break;

  case 285: // string: STRING
#line 2364 "syntactic.y"
        {
		(yylhs.value.expression).type = EXPRESSION_STRING;
		(yylhs.value.expression).shared.string = (yystack_[0].value.string);
	}
#line 3415 "syntactic.cpp"
    break;


#line 3419 "syntactic.cpp"

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


  const short parser::yypact_ninf_ = -300;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
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

  const short
  parser::yypgoto_[] =
  {
    -300,  -300,   122,  -122,  -300,  -300,  -300,  -300,  -119,   198,
     211,  -300,  -299,  -118,    19,   195,     2,    13,   191,    -3,
    -172,   301,  -300,  -300,  -254
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

  const short
  parser::yycheck_[] =
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

  const unsigned char
  parser::yyr1_[] =
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
#endif


#if M68KASM_DEBUG
  const short
  parser::yyrline_[] =
  {
       0,   723,   723,   726,   731,   737,   744,   750,   755,   763,
     770,   779,   787,   792,   796,   803,   810,   816,   822,   829,
     835,   839,   844,   849,   854,   863,   871,   879,   886,   891,
     896,   901,   906,   910,   914,   918,   923,   927,   931,   937,
     943,   953,   957,   961,   967,   972,   979,   983,   988,   992,
     997,  1001,  1005,  1010,  1015,  1020,  1024,  1028,  1033,  1038,
    1043,  1048,  1053,  1061,  1078,  1106,  1123,  1151,  1168,  1196,
    1202,  1208,  1217,  1222,  1228,  1236,  1240,  1244,  1248,  1252,
    1256,  1260,  1264,  1268,  1272,  1276,  1280,  1284,  1288,  1292,
    1296,  1300,  1304,  1308,  1312,  1316,  1320,  1324,  1328,  1332,
    1336,  1340,  1344,  1348,  1352,  1356,  1360,  1364,  1368,  1372,
    1376,  1380,  1384,  1388,  1392,  1396,  1400,  1405,  1410,  1415,
    1420,  1425,  1430,  1435,  1440,  1445,  1450,  1455,  1460,  1465,
    1470,  1475,  1480,  1485,  1490,  1495,  1500,  1505,  1510,  1515,
    1520,  1525,  1530,  1535,  1540,  1545,  1550,  1555,  1560,  1564,
    1568,  1573,  1578,  1583,  1588,  1593,  1598,  1603,  1608,  1613,
    1618,  1623,  1628,  1633,  1638,  1642,  1646,  1650,  1654,  1658,
    1662,  1666,  1670,  1674,  1678,  1682,  1686,  1690,  1694,  1698,
    1702,  1706,  1710,  1714,  1718,  1722,  1726,  1730,  1734,  1738,
    1742,  1746,  1753,  1757,  1761,  1765,  1773,  1778,  1783,  1788,
    1794,  1800,  1806,  1816,  1825,  1834,  1845,  1855,  1865,  1870,
    1875,  1880,  1889,  1897,  1905,  1915,  1924,  1934,  1950,  1954,
    1958,  1963,  1969,  1976,  2007,  2011,  2018,  2022,  2047,  2051,
    2064,  2069,  2075,  2083,  2087,  2092,  2097,  2105,  2109,  2114,
    2119,  2124,  2132,  2136,  2141,  2149,  2153,  2158,  2163,  2171,
    2175,  2180,  2185,  2190,  2198,  2202,  2207,  2215,  2219,  2223,
    2228,  2234,  2242,  2243,  2249,  2255,  2261,  2270,  2271,  2272,
    2284,  2289,  2294,  2298,  2302,  2306,  2310,  2315,  2320,  2330,
    2335,  2340,  2345,  2350,  2355,  2363
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

  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const unsigned char
    translate_table[] =
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
    // Last valid token kind.
    const int code_max = 448;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 25 "syntactic.y"
} // m68kasm
#line 4381 "syntactic.cpp"

#line 2369 "syntactic.y"


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
