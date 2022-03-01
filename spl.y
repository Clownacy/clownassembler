/*%define parse.trace*/

%{

/* declare some standard headers to be used to import declarations
   and libraries into the parser. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* make forward declarations to avoid compiler warnings */
int yylex (void);
void yyerror (char *);

/*
   Some constants.
*/

  /* These constants are used later in the code */
#define SYMTABSIZE     50
#define IDLENGTH       15
#define REALLENGTH     32
#define NOTHING        -1
#define INDENTOFFSET    2

enum ParseTreeNodeType
{
    dunno
    /* Add more types here, as more nodes added to tree */
};

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

/* ------------- parse tree definition --------------------------- */

struct treeNode {
    union
    {
      long integer;
      char real[REALLENGTH];
    } value;
    int  nodeIdentifier;
    struct treeNode *first;
    struct treeNode *second;
    struct treeNode *third;
    struct treeNode *fourth;
  };

typedef  struct treeNode TREE_NODE;
typedef  TREE_NODE        *QUATERNARY_TREE;

/* ------------- forward declarations --------------------------- */
/*
static QUATERNARY_TREE create_node(int,QUATERNARY_TREE,QUATERNARY_TREE,QUATERNARY_TREE,QUATERNARY_TREE);
static void OptimiseTree(QUATERNARY_TREE t);
static void FreeTree(QUATERNARY_TREE t);
static void PrintTree(QUATERNARY_TREE t);
*/
/* ------------- symbol table definition --------------------------- */

struct symTabNode {
    char identifier[IDLENGTH];
    int type;
};

typedef  struct symTabNode SYMTABNODE;
typedef  SYMTABNODE        *SYMTABNODEPTR;

SYMTABNODEPTR  symTab[SYMTABSIZE];

long currentSymTabSize = 0;




/* clownassembler stuff */

#include "types.h"

StatementListNode *statement_list_head;

%}

/****************/
/* Start symbol */
/****************/

%start program

/**********************/
/* Action value types */
/**********************/

%union {
/*    long iVal;
    char fVal[REALLENGTH];
    QUATERNARY_TREE  tVal;*/
    struct
    {
        unsigned long integer;
        char *string;
    } generic;
    Opcode opcode;
    Operand operand;
    Operand *operand_pointer;
    Instruction instruction;
    Statement statement;
    StatementListNode *statement_list;
    Value value;
}

/* We can declare types of tree nodes */

%token TOKEN_WHITESPACE
%token TOKEN_NEWLINE
%token TOKEN_OPCODE_ORI
%token TOKEN_OPCODE_ANDI
%token TOKEN_OPCODE_SUBI
%token TOKEN_OPCODE_ADDI
%token TOKEN_OPCODE_EORI
%token TOKEN_OPCODE_CMPI
%token TOKEN_OPCODE_BTST
%token TOKEN_OPCODE_BCHG
%token TOKEN_OPCODE_BCLR
%token TOKEN_OPCODE_BSET
%token TOKEN_OPCODE_MOVEP
%token TOKEN_OPCODE_MOVEA
%token TOKEN_OPCODE_MOVE
%token TOKEN_OPCODE_NEGX
%token TOKEN_OPCODE_CLR
%token TOKEN_OPCODE_NEG
%token TOKEN_OPCODE_NOT
%token TOKEN_OPCODE_EXT
%token TOKEN_OPCODE_NBCD
%token TOKEN_OPCODE_SWAP
%token TOKEN_OPCODE_PEA
%token TOKEN_OPCODE_ILLEGAL
%token TOKEN_OPCODE_TAS
%token TOKEN_OPCODE_TST
%token TOKEN_OPCODE_TRAP
%token TOKEN_OPCODE_LINK
%token TOKEN_OPCODE_UNLK
%token TOKEN_OPCODE_RESET
%token TOKEN_OPCODE_NOP
%token TOKEN_OPCODE_STOP
%token TOKEN_OPCODE_RTE
%token TOKEN_OPCODE_RTS
%token TOKEN_OPCODE_TRAPV
%token TOKEN_OPCODE_RTR
%token TOKEN_OPCODE_JSR
%token TOKEN_OPCODE_JMP
%token TOKEN_OPCODE_MOVEM
%token TOKEN_OPCODE_LEA
%token TOKEN_OPCODE_CHK
%token TOKEN_OPCODE_ADDQ
%token TOKEN_OPCODE_SUBQ
%token TOKEN_OPCODE_ST
%token TOKEN_OPCODE_SF
%token TOKEN_OPCODE_SHI
%token TOKEN_OPCODE_SLS
%token TOKEN_OPCODE_SCC
%token TOKEN_OPCODE_SCS
%token TOKEN_OPCODE_SNE
%token TOKEN_OPCODE_SEQ
%token TOKEN_OPCODE_SVC
%token TOKEN_OPCODE_SVS
%token TOKEN_OPCODE_SPL
%token TOKEN_OPCODE_SMI
%token TOKEN_OPCODE_SGE
%token TOKEN_OPCODE_SLT
%token TOKEN_OPCODE_SGT
%token TOKEN_OPCODE_SLE
%token TOKEN_OPCODE_DBT
%token TOKEN_OPCODE_DBF
%token TOKEN_OPCODE_DBHI
%token TOKEN_OPCODE_DBLS
%token TOKEN_OPCODE_DBCC
%token TOKEN_OPCODE_DBCS
%token TOKEN_OPCODE_DBNE
%token TOKEN_OPCODE_DBEQ
%token TOKEN_OPCODE_DBVC
%token TOKEN_OPCODE_DBVS
%token TOKEN_OPCODE_DBPL
%token TOKEN_OPCODE_DBMI
%token TOKEN_OPCODE_DBGE
%token TOKEN_OPCODE_DBLT
%token TOKEN_OPCODE_DBGT
%token TOKEN_OPCODE_DBLE
%token TOKEN_OPCODE_BRA
%token TOKEN_OPCODE_BSR
%token TOKEN_OPCODE_BHI
%token TOKEN_OPCODE_BLS
%token TOKEN_OPCODE_BCC
%token TOKEN_OPCODE_BCS
%token TOKEN_OPCODE_BNE
%token TOKEN_OPCODE_BEQ
%token TOKEN_OPCODE_BVC
%token TOKEN_OPCODE_BVS
%token TOKEN_OPCODE_BPL
%token TOKEN_OPCODE_BMI
%token TOKEN_OPCODE_BGE
%token TOKEN_OPCODE_BLT
%token TOKEN_OPCODE_BGT
%token TOKEN_OPCODE_BLE

%token TOKEN_OPCODE_DIVU
%token TOKEN_OPCODE_DIVS

%token TOKEN_OPCODE_MULU
%token TOKEN_OPCODE_MULS

%token TOKEN_OPCODE_ADD
%token TOKEN_SIZE_BYTE
%token TOKEN_SIZE_WORD
%token TOKEN_SIZE_LONG
%token<generic.integer> TOKEN_DATA_REGISTER
%token<generic.integer> TOKEN_ADDRESS_REGISTER
%token<generic.integer> TOKEN_NUMBER
%token<generic.string> TOKEN_IDENTIFIER
%token TOKEN_STATUS_REGISTER
%token TOKEN_CONDITION_CODE_REGISTER
%token TOKEN_USER_STACK_POINTER_REGISTER
%token TOKEN_PROGRAM_COUNTER

%type<instruction> instruction
%type<opcode> opcode
%type<generic.integer> size
%type<opcode> full_opcode
%type<operand_pointer> operand_list
%type<operand> operand
%type<generic.integer> register_list
%type<generic.integer> register_span
%type<generic.integer> data_or_address_register
%type<statement> statement
%type<statement_list> statement_list
%type<value> value

/*
%token<iVal> CHARACTER_CONSTANT IDENTIFIER NUMBER
%token<fVal> REAL_NUMBER

%type<tVal> program block declaration_block declaration type statement_list statement assignment_statement if_statement do_statement while_statement for_statement write_statement read_statement output_list conditional subconditional comparator expression term value constant number_constant
*/
%%

end_of_line          : TOKEN_NEWLINE
                     | TOKEN_WHITESPACE TOKEN_NEWLINE
                     ;

program              : statement_list
                     {
                       statement_list_head = $1;
                     }
                     ;

statement_list       : statement
                     {
                       /* Don't bother adding empty statements to the statement list */
                       if ($1.label == NULL && $1.type == STATEMENT_TYPE_EMPTY)
                       {
                         $$ = NULL;
                       }
                       else
                       {
                         $$ = malloc(sizeof(StatementListNode));

                         if ($$ == NULL)
                         {
                           yyerror("Could not allocate memory for statement list node");
                         }
                         else
                         {
                           $$->statement = $1;
                           $$->next = NULL;
                         }
                       }
                     }
                     | statement statement_list
                     {
                       /* Don't bother adding empty statements to the statement list */
                       if ($1.label == NULL && $1.type == STATEMENT_TYPE_EMPTY)
                       {
                         $$ = $2;
                       }
                       else
                       {
                         $$ = malloc(sizeof(StatementListNode));

                         if ($$ == NULL)
                         {
                           yyerror("Could not allocate memory for statement list node");
                         }
                         else
                         {
                           $$->statement = $1;
                           $$->next = $2;
                         }
                       }
                     }
                     ;

statement            : end_of_line
                     {
                       $$.label = NULL;
                       $$.type = STATEMENT_TYPE_EMPTY;
                     }
                     | TOKEN_IDENTIFIER end_of_line
                     {
                       $$.label = $1;
                       $$.type = STATEMENT_TYPE_EMPTY;
                     }
                     | TOKEN_IDENTIFIER ':' end_of_line
                     {
                       $$.label = $1;
                       $$.type = STATEMENT_TYPE_EMPTY;
                     }
                     | instruction
                     {
                       $$.label = NULL;
                       $$.type = STATEMENT_TYPE_INSTRUCTION;
                       $$.data.instruction = $1;
                     }
                     | TOKEN_IDENTIFIER instruction
                     {
                       $$.label = $1;
                       $$.type = STATEMENT_TYPE_INSTRUCTION;
                       $$.data.instruction = $2;
                     }
                     | TOKEN_IDENTIFIER ':' instruction
                     {
                       $$.label = $1;
                       $$.type = STATEMENT_TYPE_INSTRUCTION;
                       $$.data.instruction = $3;
                     }
                     ;

instruction          : TOKEN_WHITESPACE full_opcode end_of_line
                     {
                       $$.opcode = $2;
                       $$.operands = NULL;
                     }
                     | TOKEN_WHITESPACE full_opcode TOKEN_WHITESPACE operand_list end_of_line
                     {
                       $$.opcode = $2;
                       $$.operands = $4;
                     }
                     ;

full_opcode          : opcode
                     {
                       $$ = $1;
                       $$.size = SIZE_UNDEFINED;
                     }
                     | opcode '.' size
                     {
                       $$ = $1;
                       $$.size = $3;
                     }
                     ;

opcode               : TOKEN_OPCODE_ORI
                     {
                       $$.type = OPCODE_ORI;
                     }
                     | TOKEN_OPCODE_ANDI
                     {
                       $$.type = OPCODE_ANDI;
                     }
                     | TOKEN_OPCODE_SUBI
                     {
                       $$.type = OPCODE_SUBI;
                     }
                     | TOKEN_OPCODE_ADDI
                     {
                       $$.type = OPCODE_ADDI;
                     }
                     | TOKEN_OPCODE_EORI
                     {
                       $$.type = OPCODE_EORI;
                     }
                     | TOKEN_OPCODE_CMPI
                     {
                       $$.type = OPCODE_CMPI;
                     }
                     | TOKEN_OPCODE_BTST
                     {
                       $$.type = OPCODE_BTST_STATIC;
                     }
                     | TOKEN_OPCODE_BCHG
                     {
                       $$.type = OPCODE_BCHG_STATIC;
                     }
                     | TOKEN_OPCODE_BCLR
                     {
                       $$.type = OPCODE_BCLR_STATIC;
                     }
                     | TOKEN_OPCODE_BSET
                     {
                       $$.type = OPCODE_BSET_STATIC;
                     }
                     | TOKEN_OPCODE_MOVEP
                     {
                       $$.type = OPCODE_MOVEP_TO_REG;
                     }
                     | TOKEN_OPCODE_MOVEA
                     {
                       $$.type = OPCODE_MOVEA;
                     }
                     | TOKEN_OPCODE_MOVE
                     {
                       $$.type = OPCODE_MOVE;
                     }
                     | TOKEN_OPCODE_NEGX
                     {
                       $$.type = OPCODE_NEGX;
                     }
                     | TOKEN_OPCODE_CLR
                     {
                       $$.type = OPCODE_CLR;
                     }
                     | TOKEN_OPCODE_NEG
                     {
                       $$.type = OPCODE_NEG;
                     }
                     | TOKEN_OPCODE_NOT
                     {
                       $$.type = OPCODE_NOT;
                     }
                     | TOKEN_OPCODE_EXT
                     {
                       $$.type = OPCODE_EXT;
                     }
                     | TOKEN_OPCODE_NBCD
                     {
                       $$.type = OPCODE_NBCD;
                     }
                     | TOKEN_OPCODE_SWAP
                     {
                       $$.type = OPCODE_SWAP;
                     }
                     | TOKEN_OPCODE_PEA
                     {
                       $$.type = OPCODE_PEA;
                     }
                     | TOKEN_OPCODE_ILLEGAL
                     {
                       $$.type = OPCODE_ILLEGAL;
                     }
                     | TOKEN_OPCODE_TAS
                     {
                       $$.type = OPCODE_TAS;
                     }
                     | TOKEN_OPCODE_TST
                     {
                       $$.type = OPCODE_TST;
                     }
                     | TOKEN_OPCODE_TRAP
                     {
                       $$.type = OPCODE_TRAP;
                     }
                     | TOKEN_OPCODE_LINK
                     {
                       $$.type = OPCODE_LINK;
                     }
                     | TOKEN_OPCODE_UNLK
                     {
                       $$.type = OPCODE_UNLK;
                     }
                     | TOKEN_OPCODE_RESET
                     {
                       $$.type = OPCODE_RESET;
                     }
                     | TOKEN_OPCODE_NOP
                     {
                       $$.type = OPCODE_NOP;
                     }
                     | TOKEN_OPCODE_STOP
                     {
                       $$.type = OPCODE_STOP;
                     }
                     | TOKEN_OPCODE_RTE
                     {
                       $$.type = OPCODE_RTE;
                     }
                     | TOKEN_OPCODE_RTS
                     {
                       $$.type = OPCODE_RTS;
                     }
                     | TOKEN_OPCODE_TRAPV
                     {
                       $$.type = OPCODE_TRAPV;
                     }
                     | TOKEN_OPCODE_RTR
                     {
                       $$.type = OPCODE_RTR;
                     }
                     | TOKEN_OPCODE_JSR
                     {
                       $$.type = OPCODE_JSR;
                     }
                     | TOKEN_OPCODE_JMP
                     {
                       $$.type = OPCODE_JMP;
                     }
                     | TOKEN_OPCODE_MOVEM
                     {
                       $$.type = OPCODE_MOVEM_TO_REGS;
                     }
                     | TOKEN_OPCODE_LEA
                     {
                       $$.type = OPCODE_LEA;
                     }
                     | TOKEN_OPCODE_CHK
                     {
                       $$.type = OPCODE_CHK;
                     }
                     | TOKEN_OPCODE_ADDQ
                     {
                       $$.type = OPCODE_ADDQ;
                     }
                     | TOKEN_OPCODE_SUBQ
                     {
                       $$.type = OPCODE_SUBQ;
                     }
                     | TOKEN_OPCODE_ST
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_TRUE;
                     }
                     | TOKEN_OPCODE_SF
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_FALSE;
                     }
                     | TOKEN_OPCODE_SHI
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_HIGHER;
                     }
                     | TOKEN_OPCODE_SLS
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_LOWER_OR_SAME;
                     }
                     | TOKEN_OPCODE_SCC
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_CARRY_CLEAR;
                     }
                     | TOKEN_OPCODE_SCS
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_CARRY_SET;
                     }
                     | TOKEN_OPCODE_SNE
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_NOT_EQUAL;
                     }
                     | TOKEN_OPCODE_SEQ
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_EQUAL;
                     }
                     | TOKEN_OPCODE_SVC
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_OVERFLOW_CLEAR;
                     }
                     | TOKEN_OPCODE_SVS
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_OVERFLOW_SET;
                     }
                     | TOKEN_OPCODE_SPL
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_PLUS;
                     }
                     | TOKEN_OPCODE_SMI
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_MINUS;
                     }
                     | TOKEN_OPCODE_SGE
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_GREATER_OR_EQUAL;
                     }
                     | TOKEN_OPCODE_SLT
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_LESS_THAN;
                     }
                     | TOKEN_OPCODE_SGT
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_GREATER_THAN;
                     }
                     | TOKEN_OPCODE_SLE
                     {
                       $$.type = OPCODE_Scc;
					   $$.condition = CONDITION_LESS_OR_EQUAL;
                     }
                     | TOKEN_OPCODE_DBT
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_TRUE;
                     }
                     | TOKEN_OPCODE_DBF
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_FALSE;
                     }
                     | TOKEN_OPCODE_DBHI
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_HIGHER;
                     }
                     | TOKEN_OPCODE_DBLS
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_LOWER_OR_SAME;
                     }
                     | TOKEN_OPCODE_DBCC
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_CARRY_CLEAR;
                     }
                     | TOKEN_OPCODE_DBCS
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_CARRY_SET;
                     }
                     | TOKEN_OPCODE_DBNE
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_NOT_EQUAL;
                     }
                     | TOKEN_OPCODE_DBEQ
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_EQUAL;
                     }
                     | TOKEN_OPCODE_DBVC
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_OVERFLOW_CLEAR;
                     }
                     | TOKEN_OPCODE_DBVS
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_OVERFLOW_SET;
                     }
                     | TOKEN_OPCODE_DBPL
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_PLUS;
                     }
                     | TOKEN_OPCODE_DBMI
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_MINUS;
                     }
                     | TOKEN_OPCODE_DBGE
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_GREATER_OR_EQUAL;
                     }
                     | TOKEN_OPCODE_DBLT
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_LESS_THAN;
                     }
                     | TOKEN_OPCODE_DBGT
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_GREATER_THAN;
                     }
                     | TOKEN_OPCODE_DBLE
                     {
                       $$.type = OPCODE_DBcc;
					   $$.condition = CONDITION_LESS_OR_EQUAL;
                     }
                     | TOKEN_OPCODE_BRA
                     {
                       $$.type = OPCODE_BRA;
                     }
                     | TOKEN_OPCODE_BSR
                     {
                       $$.type = OPCODE_BSR;
                     }
                     | TOKEN_OPCODE_BHI
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_HIGHER;
                     }
                     | TOKEN_OPCODE_BLS
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_LOWER_OR_SAME;
                     }
                     | TOKEN_OPCODE_BCC
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_CARRY_CLEAR;
                     }
                     | TOKEN_OPCODE_BCS
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_CARRY_SET;
                     }
                     | TOKEN_OPCODE_BNE
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_NOT_EQUAL;
                     }
                     | TOKEN_OPCODE_BEQ
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_EQUAL;
                     }
                     | TOKEN_OPCODE_BVC
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_OVERFLOW_CLEAR;
                     }
                     | TOKEN_OPCODE_BVS
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_OVERFLOW_SET;
                     }
                     | TOKEN_OPCODE_BPL
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_PLUS;
                     }
                     | TOKEN_OPCODE_BMI
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_MINUS;
                     }
                     | TOKEN_OPCODE_BGE
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_GREATER_OR_EQUAL;
                     }
                     | TOKEN_OPCODE_BLT
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_LESS_THAN;
                     }
                     | TOKEN_OPCODE_BGT
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_GREATER_THAN;
                     }
                     | TOKEN_OPCODE_BLE
                     {
                       $$.type = OPCODE_Bcc;
					   $$.condition = CONDITION_LESS_OR_EQUAL;
                     }

                     | TOKEN_OPCODE_DIVU
                     {
                       $$.type = OPCODE_DIVU;
                     }
                     | TOKEN_OPCODE_DIVS
                     {
                       $$.type = OPCODE_DIVS;
                     }

                     | TOKEN_OPCODE_MULU
                     {
                       $$.type = OPCODE_MULU;
                     }
                     | TOKEN_OPCODE_MULS
                     {
                       $$.type = OPCODE_MULS;
                     }

                     | TOKEN_OPCODE_ADD
                     {
                       $$.type = OPCODE_ADD;
                     }
                     ;

size                 : TOKEN_SIZE_BYTE
                     {
                       $$ = SIZE_BYTE;
                     }
                     | TOKEN_SIZE_WORD
                     {
                       $$ = SIZE_WORD;
                     }
                     | TOKEN_SIZE_LONG
                     {
                       $$ = SIZE_LONGWORD;
                     }
                     ;

operand_list         : operand
                     {
                       $$ = malloc(sizeof(Operand));

                       if ($$ == NULL)
                       {
                         yyerror("Could not allocate memory for operand list node");
                       }

                       *$$ = $1;
                       $$->next = NULL;
                     }
                     | operand ',' operand_list
                     {
                       $$ = malloc(sizeof(Operand));

                       if ($$ == NULL)
                       {
                         yyerror("Could not allocate memory for operand list node");
                       }

                       *$$ = $1;
                       $$->next = $3;
                     }
                     ;

                     /* Indirect address register */
operand              : '(' TOKEN_ADDRESS_REGISTER ')'
                     {
                       $$.type = OPERAND_ADDRESS_REGISTER_INDIRECT;
                       $$.main_register = $2;
                     }
                     | '(' TOKEN_ADDRESS_REGISTER ')' '+'
                     {
                       $$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT;
                       $$.main_register = $2;
                     }
                     | '-' '(' TOKEN_ADDRESS_REGISTER ')'
                     {
                       $$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT;
                       $$.main_register = $3;
                     }
                     | value '(' TOKEN_ADDRESS_REGISTER ')'
                     {
                       $$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT;
                       $$.literal = $1;
                       $$.main_register = $3;
                     }
                     | '(' TOKEN_ADDRESS_REGISTER ',' TOKEN_DATA_REGISTER '.' size ')'
                     {
                       $$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
                       $$.literal.type = TOKEN_NUMBER;
                       $$.literal.data.integer = 0;
                       $$.main_register = $2;
                       $$.index_register = $4;
                       $$.size = $6;
                       $$.index_register_is_address_register = cc_false;
                     }
                     | value '(' TOKEN_ADDRESS_REGISTER ',' TOKEN_DATA_REGISTER '.' size ')'
                     {
                       $$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
                       $$.literal = $1;
                       $$.main_register = $3;
                       $$.index_register = $5;
                       $$.size = $7;
                       $$.index_register_is_address_register = cc_false;
                     }
                     | '(' TOKEN_ADDRESS_REGISTER ',' TOKEN_ADDRESS_REGISTER '.' size ')'
                     {
                       $$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
                       $$.literal.type = TOKEN_NUMBER;
                       $$.literal.data.integer = 0;
                       $$.main_register = $2;
                       $$.index_register = $4;
                       $$.size = $6;
                       $$.index_register_is_address_register = cc_true;
                     }
                     | value '(' TOKEN_ADDRESS_REGISTER ',' TOKEN_ADDRESS_REGISTER '.' size ')'
                     {
                       $$.type = OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
                       $$.literal = $1;
                       $$.main_register = $3;
                       $$.index_register = $5;
                       $$.size = $7;
                       $$.index_register_is_address_register = cc_true;
                     }
                     | value '(' TOKEN_PROGRAM_COUNTER ')'
                     {
                       $$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT;
                       $$.literal = $1;
                     }
                     | '(' TOKEN_PROGRAM_COUNTER ',' TOKEN_DATA_REGISTER '.' size ')'
                     {
                       $$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
                       $$.literal.type = TOKEN_NUMBER;
                       $$.literal.data.integer = 0;
                       $$.index_register = $4;
                       $$.size = $6;
                       $$.index_register_is_address_register = cc_false;
                     }
                     | value '(' TOKEN_PROGRAM_COUNTER ',' TOKEN_DATA_REGISTER '.' size ')'
                     {
                       $$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
                       $$.literal = $1;
                       $$.index_register = $5;
                       $$.size = $7;
                       $$.index_register_is_address_register = cc_false;
                     }
                     | '(' TOKEN_PROGRAM_COUNTER ',' TOKEN_ADDRESS_REGISTER '.' size ')'
                     {
                       $$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
                       $$.literal.type = TOKEN_NUMBER;
                       $$.literal.data.integer = 0;
                       $$.index_register = $4;
                       $$.size = $6;
                       $$.index_register_is_address_register = cc_true;
                     }
                     | value '(' TOKEN_PROGRAM_COUNTER ',' TOKEN_ADDRESS_REGISTER '.' size ')'
                     {
                       $$.type = OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER;
                       $$.literal = $1;
                       $$.index_register = $5;
                       $$.size = $7;
                       $$.index_register_is_address_register = cc_true;
                     }
                     /* Literal */
                     | '#' value
                     {
                       $$.type = OPERAND_LITERAL;
                       $$.literal = $2;
                     }
                     /* Registers */
/*                     | TOKEN_DATA_REGISTER
                     {
                       $$.type = OPERAND_DATA_REGISTER;
                       $$.main_register = $1;
                     }
                     | TOKEN_ADDRESS_REGISTER
                     {
                       $$.type = OPERAND_ADDRESS_REGISTER;
                       $$.main_register = $1;
                     }
*/                     | TOKEN_STATUS_REGISTER
                     {
                       $$.type = OPERAND_STATUS_REGISTER;
                     }
                     | TOKEN_CONDITION_CODE_REGISTER
                     {
                       $$.type = OPERAND_CONDITION_CODE_REGISTER;
                     }
                     | TOKEN_USER_STACK_POINTER_REGISTER
                     {
                       $$.type = OPERAND_USER_STACK_POINTER_REGISTER;
                     }
                     /* Addresses */
                     | value
                     {
                       $$.type = OPERAND_ADDRESS;
                       $$.literal = $1;
                       $$.size = SIZE_UNDEFINED;
                     }
                     | '(' value ')' '.' size
                     {
                       $$.type = OPERAND_ADDRESS_ABSOLUTE;
                       $$.literal = $2;
                       $$.size = $5;
                     }
                     /* Register list */
                     | register_list
                     {
                       /* Neat trick to check if only one bit is set (yes, I know that it evaluates to true for 0, but that doesn't matter here). */
                       if (($1 & ($1 - 1)) == 0)
                       {
                         /* This is a single register. */
                         $$.main_register = 0;

                         while (($1 & (1 << $$.main_register)) == 0)
                             ++$$.main_register;

                         if ($1 >= 8)
                         {
                           $$.type = OPERAND_ADDRESS_REGISTER;
                           $$.main_register -= 8;
                         }
                         else
                         {
                           $$.type = OPERAND_DATA_REGISTER;
                         }
                       }
                       else
                       {
                           /* This is multiple registers. */
                           $$.type = OPERAND_REGISTER_LIST;
                           $$.main_register = $1; /* Such a hack... */
                       }
                     }
                     ;

register_list        : register_span
                     {
                       $$ = $1;
                     }
                     | register_span '/' register_list
                     {
                       $$ = $1 | $3;
                     }

register_span        : data_or_address_register
                     {
                       $$ = 1 << $1;
                     }
                     | data_or_address_register '-' data_or_address_register
                     {
                       unsigned long start;
                       unsigned long end;
                       unsigned long i;

                       if ($1 < $3)
                       {
                         start = $1;
                         end = $3;
                       }
                       else
                       {
                         start = $3;
                         end = $1;
                       }

                       $$ = 0;

                       for (i = start; i <= end; ++i)
                       {
                         $$ |= 1 << i;
                       }
                     }
                     ;

data_or_address_register : TOKEN_DATA_REGISTER
                     {
                       $$ = 0 + $1;
                     }
                     | TOKEN_ADDRESS_REGISTER
                     {
                       $$ = 8 + $1;
                     }
                     ;

/*
number               : TOKEN_NUMBER
                     | '$' TOKEN_NUMBER
                     ;
*/

value                : TOKEN_NUMBER
                     {
                       $$.type = TOKEN_NUMBER;
                       $$.data.integer = $1;
                     }
                     | TOKEN_IDENTIFIER
                     {
                       $$.type = TOKEN_IDENTIFIER;
                       $$.data.identifier = $1;
                     }
                     ;

/*
program              : IDENTIFIER COLON block ENDP IDENTIFIER DOT
                     {
                       QUATERNARY_TREE tree;
                       tree = create_node(PROGRAM, $3, NULL, NULL, NULL);
                       if (tree != NULL)
                       {
                         tree->value.integer = $1;

                         OptimiseTree(tree);
                         PrintTree(tree);
                         FreeTree(tree);
                       }
                     }
                     ;
block                : DECLARATIONS declaration_block CODE statement_list
                     {
                       $$ = create_node(BLOCK, $2, $4, NULL, NULL);
                     }
                     | CODE statement_list
                     {
                       $$ = create_node(BLOCK, NULL, $2, NULL, NULL);
                     }
                     ;
declaration_block    : declaration OF TYPE type SEMICOLON declaration_block
                     {
                       $$ = create_node(DECLARATION_BLOCK, $1, $4, $6, NULL);
                     }
                     | declaration OF TYPE type SEMICOLON
                     {
                       $$ = create_node(DECLARATION_BLOCK, $1, $4, NULL, NULL);
                     }
                     ;
declaration          : IDENTIFIER COMMA declaration
                     {
                       $$ = create_node(DECLARATION, $3, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = $1;
                     }
                     | IDENTIFIER
                     {
                       $$ = create_node(DECLARATION, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = $1;
                     }
                     ;
type                 : CHARACTER
                     {
                       $$ = create_node(TYPE_RULE, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = CHARACTER;
                     }
                     | INTEGER
                     {
                       $$ = create_node(TYPE_RULE, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = INTEGER;
                     }
                     | REAL
                     {
                       $$ = create_node(TYPE_RULE, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = REAL;
                     }
                     ;
statement_list       : statement SEMICOLON statement_list
                     {
                       $$ = create_node(STATEMENT_LIST, $1, $3, NULL, NULL);
                     }
                     | statement
                     {
                       $$ = create_node(STATEMENT_LIST, $1, NULL, NULL, NULL);
                     }
                     ;
statement            : assignment_statement
                     {
                       $$ = create_node(STATEMENT, $1, NULL, NULL, NULL);
                     }
                     | if_statement
                     {
                       $$ = create_node(STATEMENT, $1, NULL, NULL, NULL);
                     }
                     | do_statement
                     {
                       $$ = create_node(STATEMENT, $1, NULL, NULL, NULL);
                     }
                     | while_statement
                     {
                       $$ = create_node(STATEMENT, $1, NULL, NULL, NULL);
                     }
                     | for_statement
                     {
                       $$ = create_node(STATEMENT, $1, NULL, NULL, NULL);
                     }
                     | write_statement
                     {
                       $$ = create_node(STATEMENT, $1, NULL, NULL, NULL);
                     }
                     | read_statement
                     {
                       $$ = create_node(STATEMENT, $1, NULL, NULL, NULL);
                     }
                     ;
assignment_statement : expression ARROW IDENTIFIER
                     {
                       $$ = create_node(ASSIGNMENT_STATEMENT, $1, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = $3;
                     }
                     ;
if_statement         : IF conditional THEN statement_list ELSE statement_list ENDIF
                     {
                       $$ = create_node(IF_STATEMENT, $2, $4, $6, NULL);
                     }
                     | IF conditional THEN statement_list ENDIF
                     {
                       $$ = create_node(IF_STATEMENT, $2, $4, NULL, NULL);
                     }
                     ;
do_statement         : DO statement_list WHILE conditional ENDDO
                     {
                       $$ = create_node(DO_STATEMENT, $2, $4, NULL, NULL);
                     }
                     ;
while_statement      : WHILE conditional DO statement_list ENDWHILE
                     {
                       $$ = create_node(WHILE_STATEMENT, $2, $4, NULL, NULL);
                     }
                     ;
for_statement        : FOR IDENTIFIER IS expression BY expression TO expression DO statement_list ENDFOR
                     {
                       $$ = create_node(FOR_STATEMENT, $4, $6, $8, $10);
                       if ($$ != NULL)
                         $$->value.integer = $2;
                     }
                     ;
write_statement      : NEWLINE
                     {
                       $$ = create_node(WRITE_STATEMENT, NULL, NULL, NULL, NULL);
                     }
                     | WRITE LEFT_PARENTHESIS output_list RIGHT_PARENTHESIS
                     {
                       $$ = create_node(WRITE_STATEMENT, $3, NULL, NULL, NULL);
                     }
                     ;
read_statement       : READ LEFT_PARENTHESIS IDENTIFIER RIGHT_PARENTHESIS
                     {
                       $$ = create_node(READ_STATEMENT, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = $3;
                     }
                     ;
output_list          : value COMMA output_list
                     {
                       $$ = create_node(OUTPUT_LIST, $1, $3, NULL, NULL);
                     }
                     | value
                     {
                       $$ = create_node(OUTPUT_LIST, $1, NULL, NULL, NULL);
                     }
                     ;
conditional          : subconditional AND conditional
                     {
                       $$ = create_node(CONDITIONAL, $1, $3, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = AND;
                     }
                     | subconditional OR conditional
                     {
                       $$ = create_node(CONDITIONAL, $1, $3, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = OR;
                     }
                     | subconditional
                     {
                       $$ = create_node(CONDITIONAL, $1, NULL, NULL, NULL);
                     }
                     ;
subconditional       : NOT subconditional
                     {
                       $$ = create_node(SUBCONDITIONAL, $2, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = NOT;
                     }
                     | expression comparator expression
                     {
                       $$ = create_node(SUBCONDITIONAL, $1, $2, $3, NULL);
                       if ($$ != NULL)
                         $$->value.integer = NOTHING;
                     }
                     ;
comparator           : EQUAL
                     {
                       $$ = create_node(COMPARATOR, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = EQUAL;
                     }
                     | INEQUAL
                     {
                       $$ = create_node(COMPARATOR, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = INEQUAL;
                     }
                     | LESS
                     {
                       $$ = create_node(COMPARATOR, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = LESS;
                     }
                     | MORE
                     {
                       $$ = create_node(COMPARATOR, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = MORE;
                     }
                     | LESS_OR_EQUAL
                     {
                       $$ = create_node(COMPARATOR, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = LESS_OR_EQUAL;
                     }
                     | MORE_OR_EQUAL
                     {
                       $$ = create_node(COMPARATOR, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = MORE_OR_EQUAL;
                     }
                     ;
expression           : term PLUS expression
                     {
                       $$ = create_node(EXPRESSION, $1, $3, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = PLUS;
                     }
                     | term MINUS expression
                     {
                       $$ = create_node(EXPRESSION, $1, $3, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = MINUS;
                     }
                     | term
                     {
                       $$ = create_node(EXPRESSION, $1, NULL, NULL, NULL);
                     }
                     ;
term                 : value MULTIPLY term
                     {
                       $$ = create_node(TERM, $1, $3, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = MULTIPLY;
                     }
                     | value DIVIDE term
                     {
                       $$ = create_node(TERM, $1, $3, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = DIVIDE;
                     }
                     | value
                     {
                       $$ = create_node(TERM, $1, NULL, NULL, NULL);
                     }
                     ;
value                : IDENTIFIER
                     {
                       $$ = create_node(VALUE_IDENTIFIER, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = $1;
                     }
                     | constant
                     {
                       $$ = create_node(VALUE_CONSTANT, $1, NULL, NULL, NULL);
                     }
                     | LEFT_PARENTHESIS expression RIGHT_PARENTHESIS
                     {
                       $$ = create_node(VALUE_EXPRESSION, $2, NULL, NULL, NULL);
                     }
                     ;
constant             : number_constant
                     {
                       $$ = create_node(CONSTANT_NUMBER, $1, NULL, NULL, NULL);
                     }
                     | CHARACTER_CONSTANT
                     {
                       $$ = create_node(CONSTANT_CHARACTER, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = $1;
                     }
                     ;
number_constant      : MINUS REAL_NUMBER
                     {
                       $$ = create_node(REAL_NUMBER_CONSTANT, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                       {
                         $$->value.real[0] = '-';
                         memcpy($$->value.real + 1, $2, REALLENGTH - 2);
                         $$->value.real[REALLENGTH - 1] = '\0';
                       }
                     }
                     | REAL_NUMBER
                     {
                       $$ = create_node(REAL_NUMBER_CONSTANT, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         memcpy($$->value.real, $1, REALLENGTH);
                     }
                     | MINUS NUMBER
                     {
                       $$ = create_node(NUMBER_CONSTANT, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = -$2;
                     }
                     | NUMBER
                     {
                       $$ = create_node(NUMBER_CONSTANT, NULL, NULL, NULL, NULL);
                       if ($$ != NULL)
                         $$->value.integer = $1;
                     }
                     ;
*/
%%

/* Put other auxiliary functions here */

/*#include "lexer.c"*/
