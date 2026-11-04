#include <filesystem>
#include <print>

#include <chrono>
#include <iostream>

#include "../replay/FeedSimulator.h"
#include "../replay/TimeUtil.h"


namespace fs = std::filesystem;

int main(int argc, char* argv[])
{

	if (argc < 4)
	{
		std::println(stderr, "Usage: {} <itch-file> <speed> [start HH:MM]", argv[0]);
		return 1;
	}

	const std::string_view fileName = argv[1];
	const std::filesystem::path filePath = std::filesystem::current_path().parent_path() / "res" / fileName;

	std::println("ITCH file path: {}", filePath.string());

	if (!fs::exists(filePath) || !fs::is_regular_file(filePath))
	{
		std::cerr << "Error: Path does not exist or is not a directory.\n";
		return 1;
	}

	const std::uint32_t playbackSpeed = std::atoi(argv[2]);
	const auto startTime = argc == 4 ? parseTimeOfDay(argv[3]) : 0;

	if (!startTime)
	{
		std::cerr << "Error: Start time could not be interpreted\n";
		return 1;
	}

	FeedSimulator simulation {filePath, "127.0.0.1", 6969, playbackSpeed, *startTime};
	simulation.run();
}