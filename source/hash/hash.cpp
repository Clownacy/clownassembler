#include <array>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <string_view>

#define CLOWNMD5_IMPLEMENTATION
#define CLOWNMD5_STATIC
#include "clownmd5/clownmd5.h"

#include "../utilities/io.h"
#include "../utilities/path.h"
#include "../utilities/string.h"

static ClownMD5::Hash HashFile(std::istream &file)
{
	std::array<unsigned char, 16 * 4> buffer;

	ClownMD5 clownmd5;

	for (;;)
	{
		file.read(reinterpret_cast<char*>(std::data(buffer)), std::size(buffer));

		const size_t bytes_read = file.gcount();

		if (bytes_read != std::size(buffer))
			return clownmd5.PushFinalData(buffer, bytes_read * 8);

		clownmd5.PushData(buffer);
	}
}

static std::optional<ClownMD5::Hash> HashFromString(std::string_view string)
{
	ClownMD5::Hash hash;

	for (auto &hash_byte : hash)
	{
		const auto &value = String::ToNumber<unsigned char>(String::Subview(string, 0, 2), 0x10);

		if (value == std::nullopt)
			return std::nullopt;

		hash_byte = *value;
		string.remove_prefix(2);
	}

	return hash;
}

int main(const int argc, char** const argv)
{
	if (argc < 3)
	{
		IO::PrintLine(std::cerr, "Usage: {} filename md5hash", argv[0]);
		return EXIT_SUCCESS;
	}

	const auto &expected_hash = HashFromString(argv[2]);

	if (expected_hash == std::nullopt)
	{
		IO::PrintError(std::cerr, "Could not parse hash.");
		return EXIT_FAILURE;
	}

	const std::string_view file_path = argv[1];
	std::ifstream file(Path::FromChars(file_path), file.binary);

	if (!file.is_open())
	{
		IO::PrintError(std::cerr, "Could not open file '{}'.", file_path);
		return EXIT_FAILURE;
	}

	const auto &hash = HashFile(file);

	return hash == *expected_hash ? EXIT_SUCCESS : EXIT_FAILURE;
}
