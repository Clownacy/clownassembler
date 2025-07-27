#ifndef IO_H
#define IO_H

#include <stdarg.h>
#include <stddef.h>

#include "clowncommon/clowncommon.h"

typedef char* (*ClownAssembler_ReadLine)(void *user_data, char *buffer, size_t buffer_size);
typedef void (*ClownAssembler_Seek)(void *user_data, size_t position);
typedef void (*ClownAssembler_WriteCharacter)(void *user_data, int character);
typedef void (*ClownAssembler_WriteCharacters)(void *user_data, const char *characters, size_t total_characters);
typedef void (*ClownAssembler_WriteString)(void *user_data, const char *string);
typedef void (*ClownAssembler_PrintFormatted)(void *user_data, const char *format, va_list args);

typedef struct ClownAssembler_TextInput
{
	const void *user_data;
	ClownAssembler_ReadLine read_line;
} ClownAssembler_TextInput;

typedef struct ClownAssembler_BinaryOutput
{
	const void *user_data;
	ClownAssembler_WriteCharacter write_character;
	ClownAssembler_WriteCharacters write_characters;
	ClownAssembler_Seek seek;
} ClownAssembler_BinaryOutput;

typedef struct ClownAssembler_TextOutput
{
	const void *user_data;
	ClownAssembler_PrintFormatted print_formatted;
	ClownAssembler_WriteCharacter write_character;
	ClownAssembler_WriteString write_string;
} ClownAssembler_TextOutput;

char* TextInput_fgets(char *buffer, size_t buffer_size, const ClownAssembler_TextInput *callbacks);

cc_bool BinaryOutput_exists(const ClownAssembler_BinaryOutput *callbacks);
void BinaryOutput_fputc(int character, const ClownAssembler_BinaryOutput *callbacks);
void BinaryOutput_fseek(const ClownAssembler_BinaryOutput *callbacks, size_t position);
void BinaryOutput_fwrite(const void *buffer, size_t size, size_t count, const ClownAssembler_BinaryOutput *callbacks);

cc_bool TextOutput_exists(const ClownAssembler_TextOutput *callbacks);
void TextOutput_vfprintf(const ClownAssembler_TextOutput *callbacks, const char *format, va_list args);
CC_ATTRIBUTE_PRINTF(2, 3) void TextOutput_fprintf(const ClownAssembler_TextOutput *callbacks, const char *format, ...);
void TextOutput_fputs(const char *string, const ClownAssembler_TextOutput *callbacks);
void TextOutput_fputc(int character, const ClownAssembler_TextOutput *callbacks);

#endif /* IO_H */
