#include "clownassembler.h"

#include <cstdio>
#include <cstring>
#include <optional>
#include <string>

struct OutputData
{
	std::ostream &stream;
	std::ostream::pos_type base_position;
};

static int ReadCharacter(void* const user_data)
{
	auto &stream = *static_cast<std::istream*>(user_data);
	const auto value = stream.get();
	return value == std::istream::traits_type::eof() ? -1 : value;
}

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
	data.stream.put(character);
}

static void WriteCharacters(void* const user_data, const char* const characters, const size_t total_characters)
{
	const auto &data = *static_cast<OutputData*>(user_data);
	data.stream.write(characters, total_characters);
}

static void Seek(void* const user_data, const size_t position)
{
	const auto &data = *static_cast<OutputData*>(user_data);
	data.stream.seekp(data.base_position);
	data.stream.seekp(position, std::ostream::cur);
}

static void WriteString(void* const user_data, const char* const string)
{
	const auto &data = *static_cast<OutputData*>(user_data);
	data.stream << string;
}

static void PrintFormatted(void* const user_data, const char* const format, va_list args)
{
	const auto &data = *static_cast<OutputData*>(user_data);

	va_list args_copy;
	va_copy(args_copy, args);
	const auto buffer_size = std::vsnprintf(nullptr, 0, format, args_copy);
	va_end(args_copy);

	std::string buffer(buffer_size, '\0');
	std::vsnprintf(buffer.data(), buffer.size() + 1, format, args);

	data.stream << buffer;
}

bool ClownAssembler::Assemble(
	std::istream &input,
	std::ostream &output,
	std::ostream* const errors,
	std::ostream* const listing,
	std::ostream* const symbols,
	const std::filesystem::path &input_file_path,
	const bool debug,
	const bool case_insensitive,
	const bool equ_set_descope_local_labels,
	const bool output_local_labels_to_sym_file,
	const bool warnings_enabled,
	const std::function<void(void *internal, ClownAssembler_AddDefinition add_definition)>* const definition_callback
)
{
	const ClownAssembler_TextInput input_callbacks = {&input, ReadCharacter, ReadLine};
	const OutputData output_data = {output, output.tellp()};
	const ClownAssembler_BinaryOutput output_callbacks = {&output_data, WriteCharacter, WriteCharacters, Seek};

	std::optional<OutputData> error_data;
	if (errors != nullptr)
		error_data.emplace(*errors, errors->tellp());
	const ClownAssembler_TextOutput error_callbacks = {error_data.has_value() ? &*error_data : nullptr, PrintFormatted, WriteCharacter, WriteString};

	std::optional<OutputData> listing_data;
	if (listing != nullptr)
		listing_data.emplace(*listing, listing->tellp());
	const ClownAssembler_TextOutput listing_callbacks = {listing_data.has_value() ? &*listing_data : nullptr, PrintFormatted, WriteCharacter, WriteString};

	std::optional<OutputData> symbol_data;
	if (symbols != nullptr)
		symbol_data.emplace(*symbols, symbols->tellp());
	const ClownAssembler_BinaryOutput symbol_callbacks = {symbol_data.has_value() ? &*symbol_data : nullptr, WriteCharacter, WriteCharacters, Seek};

	const auto definition_callback_wrapper = [](void* const internal, void* const user_data, const ClownAssembler_AddDefinition add_definition)
	{
		const auto &callback = *static_cast<decltype(definition_callback)>(user_data);

		callback(internal, add_definition);
	};

	return ClownAssembler_Assemble(
		&input_callbacks,
		&output_callbacks,
		&error_callbacks,
		&listing_callbacks,
		&symbol_callbacks,
		input_file_path.string().c_str(),
		debug,
		case_insensitive,
		equ_set_descope_local_labels,
		output_local_labels_to_sym_file,
		warnings_enabled,
		definition_callback != nullptr ? static_cast<ClownAssembler_DefinitionCallback>(definition_callback_wrapper) : nullptr,
		definition_callback
	);
}
