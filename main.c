#include <stdio.h>
#include <stdlib.h>

#include "types.h"

#define YY_NO_UNISTD_H
#include "lexer.h"
#include "parser.h"

#define ERROR(message) do { fputs("Error: " message "\n", stderr); exit_code = EXIT_FAILURE; } while (0)

/* TODO - Stupid hack */
extern StatementListNode *statement_list_head;

int yywrap(void)
{
	return 1;
}

static unsigned int ConstructEffectiveAddressBits(const Operand *operand)
{
	unsigned int m, xn;

	switch (operand->type)
	{
		case OPERAND_TYPE_DATA_REGISTER:
			m = 0; /* 000 */
			xn = operand->data.data_register;
			break;

		case OPERAND_TYPE_ADDRESS_REGISTER:
			m = 1; /* 001 */
			xn = operand->data.address_register;
			break;

		case OPERAND_TYPE_ADDRESS:
			m = 0x7; /* 111 */

			switch (operand->data.address.size)
			{
				case TOKEN_SIZE_WORD:
					xn = 0;  /* 000 */
					break;

				default:
					fprintf(stderr, "Error: Absolute address can only be word- or longword-sized - assuming longword\n");
					/* Fallthrough */
				case TOKEN_SIZE_LONG:
					xn = 1;  /* 001 */
					break;
#if 0
				case -1:
					/* Automatically determine size */
					if (operand->data.address.value
					break;
#endif
			}

			break;

		case OPERAND_TYPE_LITERAL:
			m = 0x7; /* 111 */
			xn = 4;  /* 100 */
			break;

		default:
			fprintf(stderr, "Internal error: Invalid operand type in ConstructEffectiveAddressBits\n");
			m = 0;
			xn = 0;
			break;
	}

	return (m << 3) | (xn << 0);
}

static unsigned long ResolveValue(const Value *value)
{
	if (value->type != TOKEN_NUMBER)
	{
		fprintf(stderr, "Error: Unable to resolve this type of Value right now\n");
		return 0;
	}
	else
	{
		return value->data.integer;
	}
}

static void AssembleInstruction(FILE *file, const Instruction *instruction)
{
	/* Count operands. */
	unsigned int total_operands = 0;
	const Operand *operand;

	for (operand = instruction->operands; operand != NULL; operand = operand->next)
		++total_operands;

	switch (instruction->opcode.type)
	{
		case TOKEN_OPCODE_MOVE:
		{
			if (total_operands != 2)
			{
				fprintf(stderr, "Error: MOVE instruction must have two operands\n");
			}
			else
			{
				switch (instruction->operands->next->type)
				{
					case OPERAND_TYPE_STATUS_REGISTER:
						/* MOVE to SR */
						if (instruction->operands->type == OPERAND_TYPE_ADDRESS_REGISTER)
						{
							fprintf(stderr, "Error: MOVE TO SR instruction's source operand cannot be an address register\n");
						}
						else if (instruction->opcode.size != TOKEN_SIZE_WORD && instruction->opcode.size != -1)
						{
							fprintf(stderr, "Warning: MOVE TO SR instruction can only be word-sized - the specified size will be ignored\n");
						}
						else
						{
							const unsigned int machine_code = 0x46C0 | ConstructEffectiveAddressBits(instruction->operands);

							fputc((machine_code >> (8 * 1)) & 0xFF, file);
							fputc((machine_code >> (8 * 0)) & 0xFF, file);

							if (instruction->operands->type == OPERAND_TYPE_LITERAL)
							{
								const unsigned long value = ResolveValue(&instruction->operands->data.literal);

								if (instruction->opcode.size == TOKEN_SIZE_LONG)
								{
									fputc((value >> (8 * 3)) & 0xFF, file);
									fputc((value >> (8 * 2)) & 0xFF, file);
								}

								fputc((value >> (8 * 1)) & 0xFF, file);
								fputc((value >> (8 * 0)) & 0xFF, file);
							}
						}

						break;

					case OPERAND_TYPE_CONDITION_CODE_REGISTER:
						/* MOVE to CCR */
						break;

					case OPERAND_TYPE_DATA_REGISTER:
						/* MOVE to data register */
						break;

					case OPERAND_TYPE_ADDRESS_REGISTER:
						/* MOVEA */
						break;

					case OPERAND_TYPE_ADDRESS:
						/* MOVE to memory */
						break;

					case OPERAND_TYPE_LITERAL:
						fprintf(stderr, "Error: MOVE instruction's destination operand cannot be a literal\n");
						break;
				}
			}

			break;
		}

		default:
			fprintf(stderr, "Internal error: Unrecognised instruction\n");
			break;
	}
}

int main(int argc, char **argv)
{
	int exit_code = EXIT_SUCCESS;

	if (argc < 2)
	{
		ERROR("Input file must be passed as a parameter");
	}
	else
	{
		yyin = fopen(argv[1], "r");

		if (yyin == NULL)
		{
			ERROR("Could not open input file");
		}
		else
		{
			/*yylex(); */

		#if YYDEBUG
			yydebug = 1;
		#endif

			(void)yyparse();

			fclose(yyin);

			/* Parse the parse tree */
			{
				FILE *output_file = fopen("output.bin", "wb");

				if (output_file == NULL)
				{
					ERROR("Could not open output file");
				}
				else
				{
					StatementListNode *statement_list_node;

					for (statement_list_node = statement_list_head; statement_list_node != NULL; statement_list_node = statement_list_node->next)
					{
						fprintf(stderr, "What do we have here? A statement of type %d!\n", statement_list_node->statement.type);

						switch (statement_list_node->statement.type)
						{
							case STATEMENT_TYPE_EMPTY:
								fprintf(stderr, "  What do we have here? An empty statement!\n");
								break;

							case STATEMENT_TYPE_INSTRUCTION:
								fprintf(stderr, "  What do we have here? An instruction of type %d!\n", statement_list_node->statement.data.instruction.opcode.type);
								AssembleInstruction(output_file, &statement_list_node->statement.data.instruction);
								break;

							case STATEMENT_TYPE_MACRO:
								fprintf(stderr, "  What do we have here? A macro!\n");
								break;
						}
					}

					fclose(output_file);
				}
			}
		}
	}

	return exit_code;
}

void yyerror(char *s)
{
    fprintf(stderr, "Error : Exiting %s\n", s);
}
