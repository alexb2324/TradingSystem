#include <filesystem>
#include <print>

#include <chrono>
#include <iostream>

#include "../replay/FeedSimulator.h"
#include "../replay/TimeUtil.h"


namespace fs = std::filesystem;

int main(int argv, char* argc[])
{

	if (argv < 2)
	{
		std::println(stderr, "Usage: {} <itch-file> [start HH:MM]", argc[0]);
		return 1;
	}

	const std::string_view fileName = argc[1];
	const std::filesystem::path filePath = std::filesystem::current_path().parent_path() / "res" / fileName;

	std::println("ITCH file path: {}", filePath.string());

	if (!fs::exists(filePath) || !fs::is_regular_file(filePath))
	{
		std::cerr << "Error: Path does not exist or is not a directory.\n";
		return 1;
	}

	const auto startTime = argv == 3 ? parseTimeOfDay(argc[2]) : 0;

	if (!startTime)
	{
		std::cerr << "Error: Start time could not be interpreted\n";
		return 1;
	}

	FeedSimulator simulation {filePath, "127.0.0.1", 6969, *startTime};
	simulation.run();
}