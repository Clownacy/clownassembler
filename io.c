#include "io.h"

typedef ClownAssembler_TextInput TextInput;
typedef ClownAssembler_BinaryOutput BinaryOutput;
typedef ClownAssembler_TextOutput TextOutput;

char* TextInput_fgets(char* const buffer, const size_t buffer_size, const TextInput* const callbacks)
{
	return callbacks->read_line((void*)callbacks->user_data, buffer, buffer_size);
}

cc_bool BinaryOutput_exists(const BinaryOutput* const callbacks)
{
	return callbacks != NULL && callbacks->user_data != NULL;
}

void BinaryOutput_fputc(const int character, const BinaryOutput* const callbacks)
{
	callbacks->write_character((void*)callbacks->user_data, character);
}

void BinaryOutput_fseek(const BinaryOutput* const callbacks, const size_t position)
{
	callbacks->seek((void*)callbacks->user_data, position);
}

void BinaryOutput_fwrite(const void* const buffer, const size_t size, const size_t count, const BinaryOutput* const callbacks)
{
	callbacks->write_characters((void*)callbacks->user_data, buffer, size * count);
}

cc_bool TextOutput_exists(const TextOutput* const callbacks)
{
	return callbacks != NULL && callbacks->user_data != NULL;
}

void TextOutput_vfprintf(const TextOutput* const callbacks, const char* const format, va_list args)
{
	if (TextOutput_exists(callbacks))
		callbacks->print_formatted((void*)callbacks->user_data, format, args);
}

void TextOutput_fprintf(const TextOutput* const callbacks, const char* const format, ...)
{
	if (TextOutput_exists(callbacks))
	{
		va_list args;
		va_start(args, format);
		TextOutput_vfprintf(callbacks, format, args);
		va_end(args);
	}
}

void TextOutput_fputs(const char* const string, const TextOutput* const callbacks)
{
	if (TextOutput_exists(callbacks))
		callbacks->write_string((void*)callbacks->user_data, string);
}

void TextOutput_fputc(const int character, const TextOutput* const callbacks)
{
	if (TextOutput_exists(callbacks))
		callbacks->write_character((void*)callbacks->user_data, character);
}
