#include <filesystem>
#include <print>
#include <cstdio>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <optional>


#include "console/ConsoleViewer.h"
#include "parser/Dispatch.h"
#include "datasource/filesource/FileSource.h"
#include "console/CountingHandler.h"
#include "marketstate/MarketState.h"


int main(int argc, char* argv[])
{

    if (argc < 3)
    {
        std::println(stderr, "Usage: {} <itch-file> <SYMBOL> [speed] [start HH:MM]", argv[0]);
        return 1;
    }

    std::filesystem::path filePath {argv[1]};
    const std::string symbol = argv[2];


    const double speed = argc > 3 ? std::atof(argv[3]) : 0.0;               // 0 = as fast as possible
    const itch::Timestamp startingTime = argc > 4
        ? (std::stoull(std::string{argv[4]}.substr(0, 2)) * 3600 + std::stoull(std::string{argv[4]}.substr(3, 2)) * 60) * 1'000'000'000ull
        : 0;

    FileSource fileSource { filePath };
    const auto data = fileSource.bytes();

    if (data.empty())
    {
        std::println("File is empty");
        return 0;
    }

    auto marketState = std::make_unique<MarketState>();


    // Console viewer
    ConsoleView view;

    using Clock = std::chrono::steady_clock;
    constexpr std::size_t kSlice = 64 * 1024;                                // ~2k messages per slice
    constexpr auto kRedraw = std::chrono::milliseconds(100);
    const auto wallStart = Clock::now();
    auto lastDraw = wallStart;
    Clock::time_point paceWall {};
    itch::Timestamp paceTs = 0;
    std::optional<itch::StockLocate> locate;

    std::size_t offset = 0;
    while (offset < data.size())
    {
        const auto slice = data.subspan(offset, std::min(kSlice, data.size() - offset));
        const auto used = parseData(slice, *marketState);
        if (used == 0) { break; }                                             // truncated / corrupt tail
        offset += used;

        const auto ts = marketState->lastTimestamp();
        if (speed > 0 && ts >= startingTime)
        {                                    // pace by exchange time
            if (paceTs == 0)
            {
                paceTs = ts;
                paceWall = Clock::now();
            }
            std::this_thread::sleep_until(
                paceWall + std::chrono::nanoseconds(static_cast<std::int64_t>((ts - paceTs) / speed)));
        }

        const auto now = Clock::now();
        if (now - lastDraw >= kRedraw || offset == data.size())
        {
            lastDraw = now;
            if (!locate)
            {
                locate = marketState->stockDirectory().findLocate(symbol);  // 'R' messages arrive pre-open
            }
            const ReplayStats st { marketState->messageCount(), ts,
                                   std::chrono::duration<double>(now - wallStart).count() };
            const auto frame = view.render(symbol, locate, marketState->market(), marketState->tradeBook(), st);
            std::fwrite(frame.data(), 1, frame.size(), stdout);
            std::fflush(stdout);
        }
    }
    std::println("\nDone: {}/{} bytes, unknown refs {}", offset, data.size(), marketState->market().unknownOrders());

    // Counter handling
    /*
    CountingHandler counter {};
    const auto parsedBytes = parseData(data, counter);
    counter.report(stdout);
    */


    return 0;
}



//"/Users/unknownuser/Desktop/Git/TradingSystem/res/01302019.NASDAQ_ITCH50"