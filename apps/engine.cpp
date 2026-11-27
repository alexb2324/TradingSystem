#include <chrono>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <print>
#include <stdexcept>
#include <cstring>
#include <string>
#include <string_view>
#include <thread>
#include <pthread.h>
#include <sched.h>

#include "../src/console/ConsoleRenderer.h"
#include "../src/console/ConsoleSnapshotMessage.h"
#include "../src/data_source/filesource/FileSource.h"
#include "../src/feed_handler/FeedHandler.h"
#include "../src/parser/Dispatch.h"
#include "../src/book_engine/BookEngine.h"
#include "../third_party/rigtorp/Seqlock.h"

#include "configs/ItchConfigs.h"
#include "../benchmarks/BenchmarkUtil.h"


using Clock = std::chrono::steady_clock;

static std::string formatNumberWithCommas(std::size_t value)
{
    std::string result = std::to_string(value);

    for (std::ptrdiff_t i = static_cast<std::ptrdiff_t>(result.size()) - 3; i > 0; i -= 3)
    {
        result.insert(static_cast<std::size_t>(i), ",");
    }
    return result;
}

template <typename Engine>
static void printTestRunStats(Engine& engine, std::chrono::duration<double> elapsedTime, itch::Bytes fileData, std::size_t readBytes)
{
    const auto messages = engine.marketState().messageCount();
    const double seconds = elapsedTime.count();

    std::println();
    std::println("------------------------- Results -------------------------------------------");
    std::println("Processed messages : {}", formatNumberWithCommas(messages));
    std::println("Processed bytes    : {} / {}{}", formatNumberWithCommas(readBytes), formatNumberWithCommas(fileData.size()),
                 readBytes == fileData.size() ? "" : "  (WARNING: stopped early, truncated/corrupt tail)");
    std::println("Processing time    : {:.3f} s", seconds);
    std::println("Throughput         : {:.2f} M msgs/s, {:.1f} MB/s",
                 messages / seconds / 1e6, readBytes / seconds / 1e6);
    std::println("Avg latency per message    : {:.1f} ns", seconds * 1e9 / static_cast<double>(messages));
}

// Pin a thread to a core. Returns the pthread error code (0 = success).
static int pinToCore(pthread_t thread, int core) noexcept
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core, &cpuset);
    return pthread_setaffinity_np(thread, sizeof(cpuset), &cpuset);
}

static void pinOrWarn(pthread_t thread, int core, std::string_view name)
{
    if (const int rc = pinToCore(thread, core); rc != 0)
    {
        std::println(stderr, "WARNING: could not pin {} thread to core {} ({})",
                     name, core, std::system_category().message(rc));
    }
}

// Live mode: UDP feed -> BookEngine -> seqlock -> console renderer
static void runProd(const std::string& host, std::uint16_t port, const std::string& symbol, int feedHandlerCore, int rendererCore)
{
    rigtorp::Seqlock<ConsoleSnapshotMessage> snapshotLock;
    prod::BookEngine                    engine   {symbol, snapshotLock};
    FeedHandler<prod::BookEngine>       feed     {host, port, engine};   // throws here if the socket fails
    ConsoleRenderer                     renderer {symbol, snapshotLock};

    std::jthread feedThread(&FeedHandler<prod::BookEngine>::run, &feed);
    /*std::jthread newFeedThread([&]
    {
        try { feed.run(); }
        catch (...) { feedError = std::current_exception(); }
    });*/
    std::jthread rendererThread(&ConsoleRenderer::run, &renderer);

    pinOrWarn(feedThread.native_handle(), feedHandlerCore, "feedHandler");
    pinOrWarn(rendererThread.native_handle(), rendererCore, "renderer");

    feedThread.join();                // end of session, timeouts or error
    rendererThread.request_stop();
    rendererThread.join();
}


template <typename Engine>
static std::size_t processFile(Engine& engine, itch::Bytes fileData)
{
    std::size_t readOffset {};
    while (readOffset + itch::configs::kItchMessageOffset <= fileData.size())
    {
        const auto msgLength = convertBLEndian<itch::MessageLength>(fileData.data() + readOffset);
        if (msgLength == 0) { break; }
        if (readOffset + itch::configs::kItchMessageOffset + msgLength > fileData.size()) { break; }

        engine.onMessage(fileData.subspan(readOffset + itch::configs::kItchMessageOffset, msgLength));

        readOffset += itch::configs::kItchMessageOffset + msgLength;
    }
    return readOffset;
}

// Benchmark mode: file -> BookEngine as fast as possible, no network
static void runTest(const std::filesystem::path& filePath, int engineCoreOpt, bool measureLatencyOpt)
{
    if (!std::filesystem::exists(filePath))
    {
        throw std::runtime_error("File not found: " + filePath.string());
    }

    // Ensure sufficient RAM before trying to fault in all pages
    std::println("Pre-faulting all file pages...");

    FileSource fileSource {filePath, true};
    const auto fileData = fileSource.bytes();
    if (fileData.empty()) { throw std::runtime_error("File is empty: " + filePath.string()); }

    pinOrWarn(pthread_self(), engineCoreOpt, "main");
    std::println("Processing {} ({} bytes)...", filePath.string(), formatNumberWithCommas(fileData.size()));

    if (measureLatencyOpt)
    {
        const auto startTime = Clock::now();

        test::BookEngine<MessageLatency> engine {};
        const auto readBytes = processFile(engine, fileData);

        const std::chrono::duration<double> elapsedTime = Clock::now() - startTime;

        const double nsPerCycle = calibrateNsPerCycle(); // Samples 1'000'000 times for average nsPerCycle
        engine.timing().printEachMessageLatency(nsPerCycle);
        engine.timing().printOverall(nsPerCycle);

        printTestRunStats(engine, elapsedTime, fileData, readBytes);
    }
    else
    {
        const auto startTime = Clock::now();

        test::BookEngine engine {};
        const auto readBytes = processFile(engine, fileData);

        const std::chrono::duration<double> elapsedTime = Clock::now() - startTime;

        printTestRunStats(engine, elapsedTime, fileData, readBytes);
    }
}

int main(int argc, char* argv[])
{
    const std::string_view mode = argv[1];

    try
    {
        if (mode == "LIVE")
        {
            const std::string host = argv[2];
            const std::uint16_t port = std::stoi(argv[3]);
            const std::string symbol = argv[4];
            const int engineCore = std::stoi(argv[5]);
            const int rendererCore = std::stoi(argv[6]);
            runProd(host, port, symbol, engineCore, rendererCore);
        }
        else if (mode == "TEST")
        {
            bool measureLatencyFlag = false;
            const std::filesystem::path file = argv[2];
            const int engineCore = std::stoi(argv[3]);
            if (argc == 5) { measureLatencyFlag = std::strcmp(argv[4], "--latency") ? true : false; }
            runTest(file, engineCore, measureLatencyFlag);
        }
        else
        {
            std::println(stderr, "Invalid mode '{}'. Use PROD or TEST.", mode);
            return 1;
        }
    }
    catch(const std::exception& e)
    {
        std::println(stderr, "Fatal error: {}", e.what());
        return 1;
    }

    return 0;
}

/*
"Usage:"
"  {0} TEST <itch-file> <engineCore> [--latency]"
"  {0} LIVE <host> <port> <symbol> <engineCore> <rendererCore>"

Mandatory:
    <symbol>           Symbol to monitor/render                (example: AAPL)
    <host>             Feed address (live)                     (example: 127.0.0.1)
    <port>             Feed port (live)                        (example: 6969)
    <engine-core>      Core for the engine/feed thread         (example: 1)
    <renderer-core>    Core for the console renderer thread    (example: 2)
Test Option:
    --latency          Per-message latency histograms results

Examples/Presets:
    {0} TEST 01302019.NASDAQ_ITCH50 1
    {0} TEST 01302019.NASDAQ_ITCH50 1 --latency
    {0} LIVE 127.0.0.1 6969 AAPL 1 2
*/