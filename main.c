#include <stdio.h>
#include <stdlib.h>

#include "clowncommon.h"

#include "types.h"

#define YY_NO_UNISTD_H
#include "lexer.h"
#include "parser.h"

#define ERROR(message) do { fputs("Error: " message "\n", stderr); exit_code = EXIT_FAILURE; } while (0)

/* TODO - Stupid hack */
extern StatementListNode *statement_list_head;

static cc_bool success;

int yywrap(void)
{
	return 1;
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

static unsigned int ConstructSizeBits(Size size)
{
	switch (size)
	{
		case SIZE_BYTE:
			return 0x0000;

		default:
		case SIZE_WORD:
			return 0x0040;

		case SIZE_LONGWORD:
			return 0x0080;
	}
}

static unsigned int ConstructEffectiveAddressBits(const Operand *operand)
{
	unsigned int m, xn;

	switch (operand->type)
	{
		case OPERAND_DATA_REGISTER:
			m = 0; /* 000 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER:
			m = 1; /* 001 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER_INDIRECT:
			m = 2; /* 010 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT:
			m = 3; /* 011 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT:
			m = 4; /* 100 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT:
			m = 5; /* 101 */
			xn = operand->main_register;
			break;

		case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
			m = 6; /* 110 */
			xn = operand->main_register;
			break;

		case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT:
			m = 7;  /* 111 */
			xn = 2; /* 010 */
			break;

		case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
			m = 7;  /* 111 */
			xn = 3; /* 011 */
			break;

		case OPERAND_ADDRESS:
			m = 7; /* 111 */

			switch (operand->size)
			{
				case SIZE_WORD:
					xn = 0; /* 000 */
					break;

				default:
					fprintf(stderr, "Error: Absolute address can only be word- or longword-sized - assuming longword\n");
					success = cc_false;
					/* Fallthrough */
				case SIZE_LONGWORD:
					xn = 1; /* 001 */
					break;

				case SIZE_UNDEFINED:
				{
					/* Automatically determine size. */
					const unsigned long value = ResolveValue(&operand->literal);

					if (value >= 0xFFFF8000 || value < 0x8000)
						xn = 0; /* 000 */
					else
						xn = 1; /* 001 */

					break;
				}
			}

			break;

		case OPERAND_LITERAL:
			m = 7;  /* 111 */
			xn = 4; /* 100 */
			break;

		default:
			fprintf(stderr, "Error: Invalid operand type - register lists, USP, SR, and CCR cannot be used here\n");
			success = cc_false;
			/* Just pretend it's data register 0 to keep things moving along. */
			m = 0;
			xn = 0;
			break;
	}

	return (m << 3) | (xn << 0);
}

static void OutputOperands(FILE *file, const Operand *operands, Size opcode_size)
{
	const Operand *operand;

	for (operand = operands; operand != NULL; operand = operand->next)
	{
		switch (operand->type)
		{
			case OPERAND_ADDRESS:
			case OPERAND_LITERAL:
			case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT:
			case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
			case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT:
			case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
			{
				unsigned int i = 2;
				unsigned long value = ResolveValue(&operand->literal);

				switch (operand->type)
				{
					default:
						break;

					case OPERAND_ADDRESS:
						switch (operand->size)
						{
							case SIZE_BYTE:
								fprintf(stderr, "Error: Address cannot be byte-sized\n");
								success = cc_false;
								i = 2;
								break;

							case SIZE_WORD:
								i = 2;

								if (value >= 0x8000 && value < 0xFFFF8000)
								{
									fprintf(stderr, "Error: Word-sized address cannot be higher than $7FFF or lower than $FFFF8000\n");
									success = cc_false;
								}

								break;

							case SIZE_LONGWORD:
								i = 4;
								break;

							case SIZE_UNDEFINED:
							{
								/* Automatically determine size. */
								const unsigned long value = ResolveValue(&operand->literal);

								if (value >= 0xFFFF8000 || value < 0x8000)
									i = 2;
								else
									i = 4;

								break;
							}
						}

						break;

					case OPERAND_LITERAL:
						switch (opcode_size)
						{
							case SIZE_BYTE:
								i = 2;

								if (value >= 0x100 && value < 0xFFFFFF00)
								{
									fprintf(stderr, "Error: Byte-sized literal cannot be larger than $FF or smaller than -$100\n");
									success = cc_false;
								}

								break;

							case SIZE_WORD:
								i = 2;

								if (value >= 0x10000 && value < 0xFFFF0000)
								{
									fprintf(stderr, "Error: Word-sized literal cannot be larger than $FFFF or smaller than -$10000\n");
									success = cc_false;
								}

								break;

							case SIZE_LONGWORD:
								i = 4;
								break;

							case SIZE_UNDEFINED:
							{
								/* Automatically determine size. */
								const unsigned long value = ResolveValue(&operand->literal);

								if (value >= 0xFFFF0000 || value < 0x10000)
									i = 2;
								else
									i = 4;

								break;
							}
						}

						break;

					case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
					case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
						i = 2;

						if (value >= 0x80 && value < 0xFFFFFF80)
						{
							fprintf(stderr, "Error: Displacement value cannot be larger than $7F or smaller than -$80\n");
							success = cc_false;
						}

						if (operand->size == SIZE_BYTE)
						{
							fprintf(stderr, "Error: Index register cannot be byte-sized\n");
							success = cc_false;
						}

						value |= operand->index_register << 12;

						if (operand->size == SIZE_LONGWORD)
							value |= 0x800;

						if (operand->index_register_is_address_register)
							value |= 0x8000;

						break;

					case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT:
					case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT:
						i = 2;

						if (value >= 0x8000 && value < 0xFFFF8000)
						{
							fprintf(stderr, "Error: Displacement value cannot be larger than $7FFF or smaller than -$8000\n");
							success = cc_false;
						}

						break;
				}

				while (i-- > 0)
					fputc((value >> (8 * i)) & 0xFF, file);

				break;
			}

			case OPERAND_REGISTER_LIST:
			{
				unsigned int i;
				unsigned int register_list = operand->main_register;

				/* Ugly hack to reverse the register list when doing `movem.w/.l d0-a7,-(aN)` */
				if (operands != NULL && operands->next != NULL && operands->next->type == OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT)
				{
					static unsigned int reverse_nibble[0x10] = {0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE, 0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF};

					register_list = reverse_nibble[(register_list >> (4 * 0)) & 0xF] << (4 * 3)
					              | reverse_nibble[(register_list >> (4 * 1)) & 0xF] << (4 * 2)
					              | reverse_nibble[(register_list >> (4 * 2)) & 0xF] << (4 * 1)
					              | reverse_nibble[(register_list >> (4 * 3)) & 0xF] << (4 * 0);
				}

				for (i = 2; i-- > 0; )
					fputc((register_list >> (8 * i)) & 0xFF, file);

				break;
			}

			case OPERAND_DATA_REGISTER:
			case OPERAND_ADDRESS_REGISTER:
			case OPERAND_ADDRESS_REGISTER_INDIRECT:
			case OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT:
			case OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT:
			case OPERAND_STATUS_REGISTER:
			case OPERAND_CONDITION_CODE_REGISTER:
			case OPERAND_USER_STACK_POINTER_REGISTER:
				break;
		}
	}
}
/*
static cc_bool OperandIsUnusual(const Operand *operand)
{
	switch (operand->type)
	{
		case OPERAND_DATA_REGISTER:
		case OPERAND_ADDRESS_REGISTER:
		case OPERAND_ADDRESS:
		case OPERAND_LITERAL:
			return cc_false;

		case OPERAND_STATUS_REGISTER:
		case OPERAND_CONDITION_CODE_REGISTER:
		case OPERAND_USER_STACK_REGISTER:
			return cc_true;
	}
}
*/
static unsigned int ToAlternateEffectiveAddressBits(unsigned int bits)
{
	const unsigned int m = (bits >> 3) & 7;
	const unsigned int dn = (bits >> 0) & 7;

	return (m << 6) | (dn << 9);
}

typedef struct InstructionMetadata
{
	const char *name;

	Size allowed_sizes;

	OperandType *allowed_operands;
} InstructionMetadata;

/*
	OPCODE_MOVE,
	OPCODE_MOVE_TO_USP,
	OPCODE_MOVE_FROM_USP,
	OPCODE_MOVE_TO_CCR,
	OPCODE_MOVE_TO_SR,
	OPCODE_MOVE_FROM_SR,
	OPCODE_ADD,
	OPCODE_ORI_TO_CCR,
	OPCODE_ORI_TO_SR,
	OPCODE_ORI
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
*/

/* The order of this array absolutely must match the order of the OperandType enum! */
static const InstructionMetadata instruction_metadata_all[] = {
	{	/* OPCODE_ORI_TO_CCR */
		"ORI",
		SIZE_BYTE | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_CONDITION_CODE_REGISTER,
			0
		}
	},
	{	/* OPCODE_ORI_TO_SR */
		"ORI",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_STATUS_REGISTER,
			0
		}
	},
	{	/* OPCODE_ORI */
		"ORI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_ANDI_TO_CCR */
		"ANDI",
		SIZE_BYTE | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_CONDITION_CODE_REGISTER,
			0
		}
	},
	{	/* OPCODE_ANDI_TO_SR */
		"ANDI",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_STATUS_REGISTER,
			0
		}
	},
	{	/* OPCODE_ANDI */
		"ANDI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_SUBI */
		"SUBI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_ADDI */
		"ADDI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_EORI_TO_CCR */
		"EORI",
		SIZE_BYTE | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_CONDITION_CODE_REGISTER,
			0
		}
	},
	{	/* OPCODE_EORI_TO_SR */
		"EORI",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_STATUS_REGISTER,
			0
		}
	},
	{	/* OPCODE_EORI */
		"EORI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_CMPI */
		"CMPI",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_BTST_STATIC */
		"BTST",
		SIZE_BYTE | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_BCHG_STATIC */
		"BCHG",
		SIZE_BYTE | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_BCLR_STATIC */
		"BCLR",
		SIZE_BYTE | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_BSET_STATIC */
		"BSET",
		SIZE_BYTE | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_BTST_DYNAMIC */
		"BTST",
		SIZE_BYTE | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_BCHG_DYNAMIC */
		"BCHG",
		SIZE_BYTE | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_BCLR_DYNAMIC */
		"BCLR",
		SIZE_BYTE | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_BSET_DYNAMIC */
		"BSET",
		SIZE_BYTE | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_MOVEP_TO_REG */
		"MOVEP",
		SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT,
			OPERAND_DATA_REGISTER,
			0
		}
	},
	{	/* OPCODE_MOVEP_FROM_REG */
		"MOVEP",
		SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER,
			OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT,
			0
		}
	},
	{	/* OPCODE_MOVEA */
		"MOVEA",
		SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_ADDRESS_REGISTER,
			0
		}
	},
	{	/* OPCODE_MOVE */
		"MOVE",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_MOVE_FROM_SR */
		"MOVE",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_STATUS_REGISTER,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_MOVE_TO_CCR */
		"MOVE",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_CONDITION_CODE_REGISTER,
			0
		}
	},
	{	/* OPCODE_MOVE_TO_SR */
		"MOVE",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_STATUS_REGISTER,
			0
		}
	},
	{	/* OPCODE_NEGX */
		"NEGX",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_CLR */
		"CLR",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_NEG */
		"NEG",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_NOT */
		"NOT",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_EXT */
		"EXT",
		SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER,
			0
		}
	},
	{	/* OPCODE_NBCD */
		"NBCD",
		SIZE_BYTE | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_SWAP */
		"SWAP",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER,
			0
		}
	},
	{	/* OPCODE_PEA */
		"PEA",
		SIZE_LONGWORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_ILLEGAL */
		"ILLEGAL",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			0
		}
	},
	{	/* OPCODE_TAS */
		"TAS",
		SIZE_BYTE | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_TST */
		"TST",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_TRAP */
		"TRAP",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_LITERAL,
			0
		}
	},
	{	/* OPCODE_LINK */
		"LINK",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_ADDRESS_REGISTER,
			OPERAND_LITERAL,
			0
		}
	},
	{	/* OPCODE_UNLK */
		"UNLK",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_ADDRESS_REGISTER,
			0
		}
	},
	{	/* OPCODE_MOVE_TO_USP */
		"MOVE",
		SIZE_LONGWORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_ADDRESS_REGISTER,
			OPERAND_USER_STACK_POINTER_REGISTER,
			0
		}
	},
	{	/* OPCODE_MOVE_FROM_USP */
		"MOVE",
		SIZE_LONGWORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_USER_STACK_POINTER_REGISTER,
			OPERAND_ADDRESS_REGISTER,
			0
		}
	},
	{	/* OPCODE_RESET */
		"RESET",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			0
		}
	},
	{	/* OPCODE_NOP */
		"NOP",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			0
		}
	},
	{	/* OPCODE_STOP */
		"STOP",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			0
		}
	},
	{	/* OPCODE_RTE */
		"RTE",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			0
		}
	},
	{	/* OPCODE_RTS */
		"RTS",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			0
		}
	},
	{	/* OPCODE_TRAPV */
		"TRAPV",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			0
		}
	},
	{	/* OPCODE_RTR */
		"RTR",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			0
		}
	},
	{	/* OPCODE_JSR */
		"JSR",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_JMP */
		"JMP",
		SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			0
		}
	},
	{	/* OPCODE_MOVEM_TO_REGS */
		"MOVEM",
		SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_REGISTER_LIST,
			0
		}
	},
	{	/* OPCODE_MOVEM_FROM_REGS */
		"MOVEM",
		SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_REGISTER_LIST,
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_LEA */
		"LEA",
		SIZE_LONGWORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER
				| OPERAND_ADDRESS | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_ADDRESS_REGISTER,
			0
		}
	},
	{	/* OPCODE_CHK */
		"CHK",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
			0
		}
	},
	{	/* OPCODE_ADDQ */
		"ADDQ",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_SUBQ */
		"SUBQ",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			OPERAND_LITERAL,
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},
	{	/* OPCODE_Scc */
		"Scc",
		SIZE_BYTE | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS,
			0
		}
	},

	{	/* OPCODE_DIVU */
		"DIVU",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
			0
		}
	},
	{	/* OPCODE_DIVS */
		"DIVS",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
			0
		}
	},

	{	/* OPCODE_MULU */
		"MULU",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
			0
		}
	},
	{	/* OPCODE_MULS */
		"MULS",
		SIZE_WORD | SIZE_UNDEFINED,
		(OperandType[])
		{
			OPERAND_DATA_REGISTER | OPERAND_ADDRESS_REGISTER_INDIRECT | OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT | OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT
				| OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER | OPERAND_ADDRESS | OPERAND_LITERAL
				| OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT | OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER,
			OPERAND_DATA_REGISTER,
			0
		}
	},

	{	/* OPCODE_ADD */
		"ADD",
		SIZE_BYTE | SIZE_WORD | SIZE_LONGWORD,
		(OperandType[])
		{
			/* TODO */
			OPERAND_DATA_REGISTER,
			OPERAND_DATA_REGISTER,
			0
		}
	},
};

static cc_bool AssembleInstruction(FILE *file, const Instruction *instruction)
{
	unsigned int total_operands_wanted;
	unsigned int total_operands_have;
	const Operand *operand;
	/* Default to NOP in case errors occur later on and we can't get the correct machine code. */
	unsigned int machine_code = 0x4E71;
	unsigned int i;
	const InstructionMetadata *instruction_metadata = &instruction_metadata_all[instruction->opcode.type];
	const Operand *operands_to_output = instruction->operands;

	success = cc_true;

	/* Count operands that we want. */
	total_operands_wanted = 0;

	while (instruction_metadata->allowed_operands[total_operands_wanted] != 0)
		++total_operands_wanted;

	/* Count operands that we have. */
	total_operands_have = 0;

	for (operand = instruction->operands; operand != NULL; operand = operand->next)
		++total_operands_have;

	if (total_operands_wanted != total_operands_have)
	{
		fprintf(stderr, "Error: '%s' instruction has %u operands, but it should have %u\n", instruction_metadata->name, total_operands_have, total_operands_wanted);
		success = cc_false;
	}
	else
	{
		/* Determine the machine code for the opcode and perform sanity-checking. */
		switch (instruction->opcode.type)
		{
			case OPCODE_ORI:
			case OPCODE_ANDI:
			case OPCODE_SUBI:
			case OPCODE_ADDI:
			case OPCODE_EORI:
			case OPCODE_CMPI:
			{
				/* ORI */
				/* ANDI */
				/* SUBI */
				/* ADDI */
				/* EORI */
				/* CMPI */
				const Operand* const destination_operand = instruction->operands->next;

				switch (instruction->opcode.type)
				{
					case OPCODE_ORI:
						machine_code = 0x000;
						break;

					case OPCODE_ANDI:
						machine_code = 0x200;
						break;

					case OPCODE_SUBI:
						machine_code = 0x400;
						break;

					case OPCODE_ADDI:
						machine_code = 0x600;
						break;

					case OPCODE_EORI:
						machine_code = 0xA00;
						break;

					case OPCODE_CMPI:
						machine_code = 0xC00;
						break;
				}

				switch (destination_operand->type)
				{
					case OPERAND_STATUS_REGISTER:
						/* ORI TO SR */
						/* ANDI TO SR */
						/* EORI TO SR */
						switch (instruction->opcode.type)
						{
							case OPCODE_ORI:
								instruction_metadata = &instruction_metadata_all[OPCODE_ORI_TO_SR];
								break;

							case OPCODE_ANDI:
								instruction_metadata = &instruction_metadata_all[OPCODE_ANDI_TO_SR];
								break;

							case OPCODE_EORI:
								instruction_metadata = &instruction_metadata_all[OPCODE_EORI_TO_SR];
								break;
						}

						machine_code |= 0x007C;

						break;

					case OPERAND_CONDITION_CODE_REGISTER:
						/* ORI TO CCR */
						/* ANDI TO CCR */
						/* EORI TO CCR */
						switch (instruction->opcode.type)
						{
							case OPCODE_ORI:
								instruction_metadata = &instruction_metadata_all[OPCODE_ORI_TO_CCR];
								break;

							case OPCODE_ANDI:
								instruction_metadata = &instruction_metadata_all[OPCODE_ANDI_TO_CCR];
								break;

							case OPCODE_EORI:
								instruction_metadata = &instruction_metadata_all[OPCODE_EORI_TO_CCR];
								break;
						}

						machine_code |= 0x003C;

						break;

					default:
						machine_code |= ConstructEffectiveAddressBits(destination_operand);
						break;
				}

				switch (instruction->opcode.size)
				{
					case SIZE_BYTE:
						machine_code |= 0x0000;
						break;

					default:
					case SIZE_WORD:
						machine_code |= 0x0040;
						break;

					case SIZE_LONGWORD:
						machine_code |= 0x0080;
						break;
				}

				break;
			}

			case OPCODE_BTST_STATIC:
			case OPCODE_BCHG_STATIC:
			case OPCODE_BCLR_STATIC:
			case OPCODE_BSET_STATIC:
			{
				const Operand* const source_operand = instruction->operands;
				const Operand* const destination_operand = instruction->operands->next;

				if (source_operand->type == OPERAND_DATA_REGISTER)
				{
					machine_code = 0x0100 | (source_operand->main_register << 9);

					/* Switch from the static versions of these instructions to the dynamic ones. */
					switch (instruction->opcode.type)
					{
						case OPCODE_BTST_STATIC:
							instruction_metadata = &instruction_metadata_all[OPCODE_BTST_DYNAMIC];
							break;

						case OPCODE_BCHG_STATIC:
							instruction_metadata = &instruction_metadata_all[OPCODE_BCHG_DYNAMIC];
							break;

						case OPCODE_BCLR_STATIC:
							instruction_metadata = &instruction_metadata_all[OPCODE_BCLR_DYNAMIC];
							break;

						case OPCODE_BSET_STATIC:
							instruction_metadata = &instruction_metadata_all[OPCODE_BSET_DYNAMIC];
							break;
					}
				}
				else
				{
					const unsigned long value = ResolveValue(&source_operand->literal);

					/* Check whether the literal value will wrap or not, and warn the user if so. */
					if (destination_operand->type == OPERAND_DATA_REGISTER)
					{
						if (value >= 32)
							fprintf(stderr, "Warning: 'BTST/BCHG/BCLR/BSET' instruction's literal value will be modulo 32\n");
					}
					else
					{
						if (value >= 8)
							fprintf(stderr, "Warning: 'BTST/BCHG/BCLR/BSET' instruction's literal value will be modulo 8\n");
					}

					machine_code = 0x0800;
				}

				switch (instruction->opcode.type)
				{
					case OPCODE_BTST_STATIC:
						machine_code |= 0x0000;
						break;

					case OPCODE_BCHG_STATIC:
						machine_code |= 0x0040;
						break;

					case OPCODE_BCLR_STATIC:
						machine_code |= 0x0080;
						break;

					case OPCODE_BSET_STATIC:
						machine_code |= 0x00C0;
						break;
				}

				/* Check that the opcode size is suitable for the destination operand. */
				if (destination_operand->type == OPERAND_DATA_REGISTER)
				{
					if (instruction->opcode.size != SIZE_LONGWORD && instruction->opcode.size != SIZE_UNDEFINED)
					{
						fprintf(stderr, "Error: 'BTST/BCHG/BCLR/BSET' instruction must be longword-sized when its destination operand is a data register\n");
						success = cc_false;
					}
				}
				else
				{
					if (instruction->opcode.size != SIZE_BYTE && instruction->opcode.size != SIZE_UNDEFINED)
					{
						fprintf(stderr, "Error: 'BTST/BCHG/BCLR/BSET' instruction must be byte-sized when its destination operand is memory\n");
						success = cc_false;
					}
				}

				machine_code |= ConstructEffectiveAddressBits(destination_operand);

				break;
			}

			case OPCODE_MOVEP_TO_REG:
			{
				unsigned int data_register = 0;
				unsigned int address_register = 0;

				const Operand* const source_operand = instruction->operands;
				const Operand* const destination_operand = instruction->operands->next;

				if (source_operand->type == OPERAND_DATA_REGISTER)
				{
					instruction_metadata = &instruction_metadata_all[OPCODE_MOVEP_FROM_REG];

					data_register = source_operand->main_register;
					address_register = destination_operand->main_register;
				}
				else if (destination_operand->type == OPERAND_DATA_REGISTER)
				{
					address_register = source_operand->main_register;
					data_register = destination_operand->main_register;
				}

				machine_code = 0x0108;
				machine_code |= data_register << 9;
				machine_code |= (source_operand->type == OPERAND_DATA_REGISTER) << 7;
				machine_code |= (instruction->opcode.size == SIZE_LONGWORD) << 6;
				machine_code |= address_register;

				break;
			}

			case OPCODE_MOVEA:
			case OPCODE_MOVE:
			{
				const Operand* const source_operand = instruction->operands;
				const Operand* const destination_operand = instruction->operands->next;

				if (instruction->opcode.type == OPCODE_MOVE && (source_operand->type == OPERAND_USER_STACK_POINTER_REGISTER || destination_operand->type == OPERAND_USER_STACK_POINTER_REGISTER))
				{
					/* MOVE USP */

					/* Default to address register 0, in case the other operand isn't an address register. */
					unsigned int address_register = 0;

					const cc_bool from_usp_to_address_register = source_operand->type == OPERAND_USER_STACK_POINTER_REGISTER;

					/* Handle operands and perform validation. */
					if (from_usp_to_address_register)
					{
						instruction_metadata = &instruction_metadata_all[OPCODE_MOVE_FROM_USP];

						if (destination_operand->type == OPERAND_ADDRESS_REGISTER)
							address_register = destination_operand->main_register;
					}
					else
					{
						instruction_metadata = &instruction_metadata_all[OPCODE_MOVE_TO_USP];

						if (source_operand->type == OPERAND_ADDRESS_REGISTER)
							address_register = source_operand->main_register;
					}

					/* Produce the machine code for this instruction. */
					machine_code = 0x4E60 | (from_usp_to_address_register << 3) | address_register;
				}
				else if (instruction->opcode.type == OPCODE_MOVE && (source_operand->type == OPERAND_STATUS_REGISTER || destination_operand->type == OPERAND_STATUS_REGISTER))
				{
					/* MOVE TO SR */
					/* MOVE FROM SR */
					const cc_bool from_sr = source_operand->type == OPERAND_STATUS_REGISTER;

					if (from_sr)
					{
						/* MOVE FROM SR */
						instruction_metadata = &instruction_metadata_all[OPCODE_MOVE_FROM_SR];

						machine_code = 0x40C0 | ConstructEffectiveAddressBits(destination_operand);
					}
					else
					{
						/* MOVE TO SR */
						instruction_metadata = &instruction_metadata_all[OPCODE_MOVE_TO_SR];

						machine_code = 0x46C0 | ConstructEffectiveAddressBits(source_operand);
					}
				}
				else if (instruction->opcode.type == OPCODE_MOVE && (destination_operand->type == OPERAND_CONDITION_CODE_REGISTER))
				{
					/* MOVE TO CCR */
					instruction_metadata = &instruction_metadata_all[OPCODE_MOVE_TO_CCR];

					machine_code = 0x44C0 | ConstructEffectiveAddressBits(source_operand);
				}
				else
				{
					/* MOVE */
					/* MOVEA */
					if (instruction->opcode.type == OPCODE_MOVEA && destination_operand->type == OPERAND_ADDRESS_REGISTER)
					{
						/* MOVEA mistyped as MOVE */
						instruction_metadata = &instruction_metadata_all[OPCODE_MOVEA];
					}

					switch (instruction->opcode.size)
					{
						case SIZE_BYTE:
							machine_code = 0x1000;
							break;

						default:
						case SIZE_WORD:
							machine_code = 0x3000;
							break;

						case SIZE_LONGWORD:
							machine_code = 0x2000;
							break;
					}

					machine_code |= ConstructEffectiveAddressBits(source_operand);
					machine_code |= ToAlternateEffectiveAddressBits(ConstructEffectiveAddressBits(destination_operand));
				}

				break;
			}

			case OPCODE_NEGX:
			case OPCODE_CLR:
			case OPCODE_NEG:
			case OPCODE_NOT:
			case OPCODE_TST:
				switch (instruction->opcode.type)
				{
					case OPCODE_NEGX:
						machine_code = 0x4000;
						break;

					case OPCODE_CLR:
						machine_code = 0x4200;
						break;

					case OPCODE_NEG:
						machine_code = 0x4400;
						break;

					case OPCODE_NOT:
						machine_code = 0x4600;
						break;

					case OPCODE_TST:
						machine_code = 0x4A00;
						break;
				}

				machine_code |= ConstructSizeBits(instruction->opcode.size);
				machine_code |= ConstructEffectiveAddressBits(instruction->operands);

				break;

			case OPCODE_EXT:
				machine_code = 0x4880;
				machine_code |= (instruction->opcode.size == SIZE_LONGWORD) << 6;
				machine_code |= ConstructEffectiveAddressBits(instruction->operands);
				break;

			case OPCODE_NBCD:
				machine_code = 0x4800;
				machine_code |= ConstructEffectiveAddressBits(instruction->operands);
				break;

			case OPCODE_SWAP:
				machine_code = 0x4840;

				/* Just a check to prevent reading uninitialised memory. */
				if (instruction->operands->type == OPERAND_DATA_REGISTER)
					machine_code |= instruction->operands->main_register;

				break;

			case OPCODE_PEA:
				machine_code = 0x4840;
				machine_code |= ConstructEffectiveAddressBits(instruction->operands);
				break;

			case OPCODE_ILLEGAL:
				machine_code = 0x4AFC;
				break;

			case OPCODE_TAS:
				machine_code = 0x4AC0;
				machine_code |= ConstructEffectiveAddressBits(instruction->operands);
				break;

			case OPCODE_TRAP:
				machine_code = 0x4E40;

				/* Just a check to prevent reading uninitialised memory. */
				if (instruction->operands->type == OPERAND_LITERAL)
				{
					const unsigned long value = ResolveValue(&instruction->operands->literal);

					if (value > 15)
					{
						fprintf(stderr, "Error: 'TRAP' instruction's vector cannot be higher than 15\n");
						success = cc_false;
					}
					else
					{
						machine_code |= value;
					}
				}

				break;

			case OPCODE_LINK:
				machine_code = 0x4E50;

				/* Just a check to prevent reading uninitialised memory. */
				if (instruction->operands->type == OPERAND_ADDRESS_REGISTER)
					machine_code |= instruction->operands->main_register;

				break;

			case OPCODE_UNLK:
				machine_code = 0x4E58;

				/* Just a check to prevent reading uninitialised memory. */
				if (instruction->operands->type == OPERAND_ADDRESS_REGISTER)
					machine_code |= instruction->operands->main_register;

				break;

			case OPCODE_RESET:
				machine_code = 0x4E70;
				break;

			case OPCODE_NOP:
				machine_code = 0x4E71;
				break;

			case OPCODE_STOP:
				machine_code = 0x4E72;
				break;

			case OPCODE_RTE:
				machine_code = 0x4E73;
				break;

			case OPCODE_RTS:
				machine_code = 0x4E75;
				break;

			case OPCODE_TRAPV:
				machine_code = 0x4E76;
				break;

			case OPCODE_RTR:
				machine_code = 0x4E77;
				break;

			case OPCODE_JSR:
				machine_code = 0x4E80;
				machine_code |= ConstructEffectiveAddressBits(instruction->operands);
				break;

			case OPCODE_JMP:
				machine_code = 0x4EC0;
				machine_code |= ConstructEffectiveAddressBits(instruction->operands);
				break;

			case OPCODE_MOVEM_TO_REGS:
				machine_code = 0x4880;
				machine_code |= (instruction->opcode.size == SIZE_LONGWORD) << 6;

				if (instruction->operands->next->type == OPERAND_REGISTER_LIST)
				{
					machine_code |= 1 << 10;
					machine_code |= ConstructEffectiveAddressBits(instruction->operands);
				}
				else
				{
					instruction_metadata = &instruction_metadata_all[OPCODE_MOVEM_FROM_REGS];
					machine_code |= ConstructEffectiveAddressBits(instruction->operands->next);
				}

				break;

			case OPCODE_LEA:
			case OPCODE_CHK:
			case OPCODE_DIVU:
			case OPCODE_DIVS:
			case OPCODE_MULU:
			case OPCODE_MULS:
			{
				const Operand* const source_operand = instruction->operands;
				const Operand* const destination_operand = instruction->operands->next;

				switch (instruction->opcode.type)
				{
					case OPCODE_LEA:
						machine_code = 0x41C0;
						break;

					case OPCODE_CHK:
						machine_code = 0x4180;
						break;

					case OPCODE_DIVU:
						machine_code = 0x80C0;
						break;

					case OPCODE_DIVS:
						machine_code = 0x81C0;
						break;

					case OPCODE_MULU:
						machine_code = 0xC0C0;
						break;

					case OPCODE_MULS:
						machine_code = 0xC1C0;
						break;
				}

				/* Just a check to prevent reading uninitialised memory. */
				if (destination_operand->type == OPERAND_DATA_REGISTER || destination_operand->type == OPERAND_ADDRESS_REGISTER)
					machine_code |= destination_operand->main_register << 9;

				machine_code |= ConstructEffectiveAddressBits(source_operand);

				break;
			}

			case OPCODE_ADDQ:
			case OPCODE_SUBQ:
			{
				const Operand* const source_operand = instruction->operands;
				const Operand* const destination_operand = instruction->operands->next;

				/* Skip the immediate operand since that goes in the machine code instead. */
				operands_to_output = destination_operand;

				switch (instruction->opcode.type)
				{
					case OPCODE_ADDQ:
						machine_code = 0x5000;
						break;

					case OPCODE_SUBQ:
						machine_code = 0x5100;
						break;
				}

				machine_code |= ConstructSizeBits(instruction->opcode.size);
				machine_code |= ConstructEffectiveAddressBits(destination_operand);

				if (source_operand->type == OPERAND_LITERAL)
				{
					const unsigned long value = ResolveValue(&source_operand->literal);

					if (value < 1 || value > 8)
					{
						fprintf(stderr, "Error: 'ADDQ'/'SUBQ' instruction's immediate value cannot be lower than 1 or higher than 8\n");
						success = cc_false;
					}
					else
					{
						machine_code |= (value - 1) << 9;
					}
				}

				break;
			}

			case OPCODE_Scc:
				machine_code = 0x50C0;
				machine_code |= instruction->opcode.condition << 8;
				machine_code |= ConstructEffectiveAddressBits(instruction->operands);
				break;

			case OPCODE_ADD:
				/* TODO */
				break;

			default:
				fprintf(stderr, "Internal error: Unrecognised instruction\n");
				success = cc_false;
				break;
		}

		/* Check whether the operands are of the correct types. */
		operand = instruction->operands;

		for (i = 0; instruction_metadata->allowed_operands[i] != 0; ++i)
		{
			if ((operand->type & ~instruction_metadata->allowed_operands[i]) != 0)
			{
				const char *operand_string = "[REDACTED]"; /* Dumb joke - this should never be seen. */

				switch (operand->type)
				{
					case OPERAND_DATA_REGISTER:
						operand_string = "a data register";
						break;

					case OPERAND_ADDRESS_REGISTER:
						operand_string = "an address register";
						break;

					case OPERAND_ADDRESS_REGISTER_INDIRECT:
						operand_string = "an indirect address register";
						break;

					case OPERAND_ADDRESS_REGISTER_INDIRECT_POSTINCREMENT:
						operand_string = "a post-increment indirect address register";
						break;

					case OPERAND_ADDRESS_REGISTER_INDIRECT_PREDECREMENT:
						operand_string = "a pre-decrement indirect address register";
						break;

					case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT:
						operand_string = "an indirect address register with displacement";
						break;

					case OPERAND_ADDRESS_REGISTER_INDIRECT_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
						operand_string = "an indirect address register with displacement and index register";
						break;

					case OPERAND_ADDRESS:
						operand_string = "an address";
						break;

					case OPERAND_LITERAL:
						operand_string = "a literal";
						break;

					case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT:
						operand_string = "the program counter with displacement";
						break;

					case OPERAND_PROGRAM_COUNTER_WITH_DISPLACEMENT_AND_INDEX_REGISTER:
						operand_string = "the program counter with displacement and index register";
						break;

					case OPERAND_STATUS_REGISTER:
						operand_string = "the status register";
						break;

					case OPERAND_CONDITION_CODE_REGISTER:
						operand_string = "the condition code register";
						break;

					case OPERAND_USER_STACK_POINTER_REGISTER:
						operand_string = "the user stack pointer register";
						break;

					case OPERAND_REGISTER_LIST:
						operand_string = "a register list";
						break;
				}

				fprintf(stderr, "Error: '%s' instruction parameter %u cannot be %s\n", instruction_metadata->name, i, operand_string);
				success = cc_false;
			}

			operand = operand->next;
		}
	}

	if ((instruction->opcode.size & ~instruction_metadata->allowed_sizes) != 0)
	{
		fprintf(stderr, "Error: '%s' instruction cannot be this size - allowed sizes are...\n", instruction_metadata->name);

		if (instruction_metadata->allowed_sizes & SIZE_BYTE)
			fprintf(stderr, "  %s.B\n", instruction_metadata->name);

		if (instruction_metadata->allowed_sizes & SIZE_WORD)
			fprintf(stderr, "  %s.W\n", instruction_metadata->name);

		if (instruction_metadata->allowed_sizes & SIZE_LONGWORD)
			fprintf(stderr, "  %s.L\n", instruction_metadata->name);

		if (instruction_metadata->allowed_sizes & SIZE_UNDEFINED)
			fprintf(stderr, "  %s\n", instruction_metadata->name);

		success = cc_false;
	}

	fprintf(stderr, "Machine code: 0x%X\n", machine_code);

	/* Output the machine code for the opcode. */
	for (i = 2; i-- > 0; )
		fputc((machine_code >> (8 * i)) & 0xFF, file);

	/* Output the data for the operands. */
	OutputOperands(file, operands_to_output, instruction->opcode.size);

	return success;
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
						switch (statement_list_node->statement.type)
						{
							case STATEMENT_TYPE_EMPTY:
								break;

							case STATEMENT_TYPE_INSTRUCTION:
								if (!AssembleInstruction(output_file, &statement_list_node->statement.data.instruction))
								{
									exit_code = EXIT_FAILURE;
								}

								break;

							case STATEMENT_TYPE_MACRO:
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
