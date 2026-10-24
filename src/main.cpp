#include <filesystem>
#include <print>

#include "parser/Dispatch.h"
#include "datasource/filesource/FileSource.h"


// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main()
{

    MarketState marketStateHandler {};

    std::filesystem::path filePath {"/home/hehexd2324/CLionProjects/TradingSystem/res/01302019.NASDAQ_ITCH50"};
    FileSource fileSource { filePath };
    const auto data = fileSource.bytes();

    if (data.empty())
    {
        std::println("File is empty");
        return 0;
    }

    const auto parsedBytes = parseData(data, marketStateHandler);

    if (parsedBytes == data.size())
    {
        std::print("Parsing finished");
    }

    return 0;
}