#include <filesystem>
#include <string>
#include <print>
#include <cstdint>

#include "../src/replay/FeedSimulator.h"
#include "../src/replay/TimeUtil.h"


int main(int argc, char* argv[])
{
	if (argc < 4)
	{
		std::println(stderr, "Usage: {} <itch-file> <host> <port> <speed> [start HH:MM]", argv[0]);
		return 1;
	}

	const std::filesystem::path filePath = argv[1];

	std::println("ITCH file path: {}", filePath.string());

	if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath))
	{
		std::println(stderr, "Error: Path does not exist or is not a directory.");
		return 1;
	}

	const std::string host = argv[2];
	const std::uint16_t port = std::stoi(argv[3]);
	const std::uint32_t playbackSpeed = std::stoi(argv[4]);
	const auto startTime = argc == 6 ? parseTimeOfDay(argv[5]) : 0;

	if (!startTime)
	{
		std::print(stderr, "Error: Start time could not be interpreted");
		return 1;
	}

	FeedSimulator simulation {filePath, host, port, playbackSpeed, *startTime};
	simulation.run();
}

/*
"Usage:"
"  {0} <itch-file> <host> <port> <speed> [start HH:MM]"

Mandatory:
	<itch-file>        Symbol to monitor/render                (example: AAPL)
	<host>             Feed address (live)                     (example: 127.0.0.1)
	<port>             Feed port (live)                        (example: 6969)
	<speed>			   Core for the engine/feed thread         (example: 1)
Optional:
	[start HH:MM]      Core for the console renderer thread    (example: 2)

Examples/Presets:
	{0} 01302019.NASDAQ_ITCH50 127.0.0.1 6969 1 10:00"
*/