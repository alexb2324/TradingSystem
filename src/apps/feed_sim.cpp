#include <filesystem>
#include <print>

#include <chrono>
#include <iostream>

#include "../replay/FeedSimulator.h"
#include "../replay/TimeUtil.h"


namespace fs = std::filesystem;

int main(int argv, char* argc[])
{
	const std::filesystem::path filePath = std::filesystem::current_path().parent_path() / "res/01302019.NASDAQ_ITCH50";

	std::println("ITCH file path: {}", filePath.string());

	if (!fs::exists(filePath) || !fs::is_regular_file(filePath)) {
		std::cerr << "Error: Path does not exist or is not a directory.\n";
		return 1;
	}

	if (const auto startTime = parseTimeOfDay("09:30"); startTime)
	{
		FeedSimulator simulation {filePath, "127.0.0.1", 6969, *startTime};
		simulation.run();
	}
}