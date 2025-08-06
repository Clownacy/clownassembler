#include "clownassembler.h"

#include <cstdio>
#include <cstring>
#include <type_traits>
#include <vector>

struct OutputData
{
	std::ostream *stream;
	std::ostream::pos_type base_position;
};

static char* ReadLine(void* const user_data, char* const buffer, const std::size_t buffer_size)
{
	auto &stream = *static_cast<std::istream*>(user_data);

	stream.getline(buffer, buffer_size);

	if (stream.fail())
		return buffer[0] == '\0' ? nullptr : buffer;

	const auto end = std::strlen(buffer);

	if (end != buffer_size - 1)
	{
		buffer[end + 0] = '\n';
		buffer[end + 1] = '\0';
	}

	return buffer;
}

static void WriteCharacter(void* const user_data, const int character)
{
	const auto &data = *static_cast<OutputData*>(user_data);
	data.stream->put(character);
}

static void WriteCharacters(void* const user_data, const char* const characters, const size_t total_characters)
{
	const auto &data = *static_cast<OutputData*>(user_data);
	data.stream->write(characters, total_characters);
}

static void Seek(void* const user_data, const size_t position)
{
	const auto &data = *static_cast<OutputData*>(user_data);
	data.stream->seekp(data.base_position);
	data.stream->seekp(position, std::ostream::cur);
}

static void WriteString(void* const user_data, const char* const string)
{
	const auto &data = *static_cast<OutputData*>(user_data);
	*data.stream << string;
}

static void PrintFormatted(void* const user_data, const char* const format, va_list args)
{
	const auto &data = *static_cast<OutputData*>(user_data);

	va_list args_copy;
	va_copy(args_copy, args);
	const auto buffer_size = std::vsnprintf(nullptr, 0, format, args_copy) + 1;
	va_end(args_copy);

	std::vector<char> buffer(buffer_size);
	std::vsnprintf(buffer.data(), buffer.size(), format, args);

	*data.stream << buffer.data();
}

bool ClownAssembler::Assemble(
	std::istream &input,
	std::ostream &output,
	std::ostream* const errors,
	std::ostream* const listing,
	std::ostream* const symbols,
	const char* const input_file_path,
	const ClownAssembler_Settings &settings,
	const std::function<void(void *internal, ClownAssembler_AddDefinition add_definition)>* const definition_callback
)
{
	const ClownAssembler_TextInput input_callbacks = {&input, ReadLine};
	const OutputData output_data = {&output, output.tellp()};
	const ClownAssembler_BinaryOutput output_callbacks = {&output_data, WriteCharacter, WriteCharacters, Seek};

	OutputData error_data;
	ClownAssembler_TextOutput error_callbacks = {nullptr, PrintFormatted, WriteCharacter, WriteString};
	if (errors != nullptr)
	{
		error_data = {errors, errors->tellp()};
		error_callbacks.user_data = &error_data;
	}

	OutputData listing_data;
	ClownAssembler_TextOutput listing_callbacks = {nullptr, PrintFormatted, WriteCharacter, WriteString};
	if (listing != nullptr)
	{
		listing_data = {listing, listing->tellp()};
		listing_callbacks.user_data = &listing_data;
	}

	OutputData symbol_data;
	ClownAssembler_BinaryOutput symbol_callbacks = {nullptr, WriteCharacter, WriteCharacters, Seek};
	if (symbols != nullptr)
	{
		symbol_data = {symbols, symbols->tellp()};
		symbol_callbacks.user_data = &symbol_data;
	}

	const auto definition_callback_wrapper = [](void* const internal, void* const user_data, const ClownAssembler_AddDefinition add_definition)
	{
		const auto &callback = *static_cast<std::remove_cv<decltype(definition_callback)>::type>(user_data);

		callback(internal, add_definition);
	};

	return ClownAssembler_Assemble(
		&input_callbacks,
		&output_callbacks,
		&error_callbacks,
		&listing_callbacks,
		&symbol_callbacks,
		input_file_path,
		&settings,
		definition_callback != nullptr ? static_cast<ClownAssembler_DefinitionCallback>(definition_callback_wrapper) : nullptr,
		definition_callback
	);
}
