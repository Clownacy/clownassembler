#include <array>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <string_view>

#define CLOWNMD5_IMPLEMENTATION
#define CLOWNMD5_STATIC
#include "clownmd5.h"

#include "../utilities/io.h"
#include "../utilities/path.h"

using Hash = std::array<unsigned char, 16>;

static Hash HashFile(std::istream &file)
{
	std::array<unsigned char, 16 * 4> buffer;

	ClownMD5_State state;
	ClownMD5_Init(&state);

	for (;;)
	{
		file.read(reinterpret_cast<char*>(std::data(buffer)), std::size(buffer));

		const size_t bytes_read = file.gcount();

		if (bytes_read != std::size(buffer))
		{
			Hash hash;
			ClownMD5_PushFinalData(&state, std::data(buffer), bytes_read * 8, std::data(hash));
			return hash;
		}

		ClownMD5_PushData(&state, std::data(buffer));
	}
}

static std::optional<Hash> HashFromString(const char* const string)
{
	std::array<unsigned int, 16> values;

	const int values_read = std::sscanf(string, "%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X%2X",
		&values[0],
		&values[1],
		&values[2],
		&values[3],
		&values[4],
		&values[5],
		&values[6],
		&values[7],
		&values[8],
		&values[9],
		&values[10],
		&values[11],
		&values[12],
		&values[13],
		&values[14],
		&values[15]
	);

	if (values_read != std::size(values))
		return std::nullopt;

	Hash hash;

	for (unsigned int i = 0; i < std::size(values); ++i)
		hash[i] = values[i];

	return hash;
}

int main(const int argc, char** const argv)
{
	if (argc < 3)
	{
		Utilities::IO::Print(std::cerr, "Usage: {} filename md5hash\n", argv[0]);
		return EXIT_SUCCESS;
	}

	const auto &expected_hash = HashFromString(argv[2]);

	if (expected_hash == std::nullopt)
	{
		Utilities::IO::Print(std::cerr, "Could not parse hash.\n");
		return EXIT_FAILURE;
	}

	const std::string_view file_path = argv[1];
	std::ifstream file(Utilities::Path::ToStandardPath(file_path), file.binary);

	if (!file.is_open())
	{
		Utilities::IO::Print(std::cerr, "Could not open file '{}'.\n", file_path);
		return EXIT_FAILURE;
	}

	const auto &hash = HashFile(file);

	return hash == *expected_hash ? EXIT_SUCCESS : EXIT_FAILURE;
}
