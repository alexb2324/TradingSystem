#include <chrono>
#include <cstdio>
#include <exception>
#include <filesystem>
#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <pthread.h>
#include <sched.h>

#include "../console/ConsoleRenderer.h"
#include "../console/ConsoleSnapshotMessage.h"
#include "../datasource/filesource/FileSource.h"
#include "../feedhandler/FeedHandler.h"
#include "../parser/Dispatch.h"
#include "../BookEngine/BookEngine.h"
#include "../../libs/rigtorp/Seqlock.h"

using Clock = std::chrono::steady_clock;


static std::string formatNumber(std::size_t value)
{
    std::string result = std::to_string(value);

    for (std::ptrdiff_t i = static_cast<std::ptrdiff_t>(result.size()) - 3; i > 0; i -= 3)
    {
        result.insert(static_cast<std::size_t>(i), ",");
    }

    return result;
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

static void printUsage(const char* prog)
{
    std::println(stderr, "Usage: {} <SYMBOL> PROD", prog);
    std::println(stderr, "       {} <SYMBOL> TEST <itch-file>", prog);
}

// Live mode: UDP feed -> BookEngine -> seqlock -> console renderer
static void runProd(const std::string& symbol)
{
    rigtorp::Seqlock<ConsoleSnapshotMessage> snapshotLock;
    BookEngine      engine   {symbol, snapshotLock};
    FeedHandler     feed     {"127.0.0.1", 6969, engine};   // throws here if the socket fails
    ConsoleRenderer renderer {symbol, snapshotLock};

    std::exception_ptr feedError;
    std::jthread feedThread(&FeedHandler::run, &feed);
    std::jthread rendererThread(&ConsoleRenderer::run, &renderer);

    pinOrWarn(feedThread.native_handle(), 1, "feed");
    pinOrWarn(rendererThread.native_handle(), 2, "renderer");

    feedThread.join();                // end of session, timeouts or error
    rendererThread.request_stop();
    rendererThread.join();

    if (feedError) { std::rethrow_exception(feedError); }
}

// Benchmark mode: file -> BookEngine as fast as possible, no network
static void runTest(const std::string& symbol, const std::filesystem::path& fileName)
{

    const std::filesystem::path filePath = std::filesystem::current_path().parent_path() / "res" / fileName;
    if (!std::filesystem::exists(filePath))
    {
        throw std::runtime_error("File not found: " + filePath.string());
    }

    rigtorp::Seqlock<ConsoleSnapshotMessage> snapshotLock;
    BookEngine engine {symbol, snapshotLock};

    // Ensure sufficient RAM before trying to fault in all pages
    FileSource fileSource {filePath, true};
    const auto fileData = fileSource.bytes();
    if (fileData.empty()) { throw std::runtime_error("File is empty: " + filePath.string()); }

    pinOrWarn(pthread_self(), 1, "main");
    std::println("Processing {} ({} bytes)...", filePath.string(), formatNumber(fileData.size()));

    std::size_t readOffset {};
    const auto startTime = Clock::now();

    while (readOffset + Constants::kItchMessageOffset <= fileData.size())
    {
        const auto msgLength = convertBLEndian<itch::MessageLength>(fileData.data() + readOffset);
        if (msgLength == 0) { break; }
        if (readOffset + Constants::kItchMessageOffset + msgLength > fileData.size()) { break; }

        engine.onMessage(fileData.subspan(readOffset + Constants::kItchMessageOffset, msgLength));
        readOffset += Constants::kItchMessageOffset + msgLength;
    }

    const auto endTime = Clock::now();


    const std::chrono::duration<double> elapsed = endTime - startTime;
    const double seconds  = elapsed.count();
    const auto   messages = engine.marketState().messageCount();

    std::println("Processed messages : {}", formatNumber(messages));
    std::println("Processed bytes    : {} / {}{}", formatNumber(readOffset), formatNumber(fileData.size()),
                 readOffset == fileData.size() ? "" : "  (WARNING: stopped early, truncated/corrupt tail)");
    std::println("Processing time    : {:.3f} s", seconds);
    std::println("Throughput         : {:.2f} M msgs/s, {:.1f} MB/s",
                 messages / seconds / 1e6, readOffset / seconds / 1e6);
    std::println("Avg per message    : {:.1f} ns", seconds * 1e9 / static_cast<double>(messages));
}

int main(int argc, char* argv[])
{
    if (argc < 3) { printUsage(argv[0]); return 1; }

    const std::string      symbol = argv[1];
    const std::string_view mode   = argv[2];

    try
    {
        if (mode == "PROD")
        {
            runProd(symbol);
        }
        else if (mode == "TEST")
        {
            if (argc < 4) { printUsage(argv[0]); return 1; }
            runTest(symbol, argv[3]);
        }
        else
        {
            std::println(stderr, "Invalid mode '{}'. Use PROD or TEST.", mode);
            return 1;
        }
    }
    catch (const std::exception& e)
    {
        std::println(stderr, "Fatal error: {}", e.what());
        return 1;
    }
    return 0;
}