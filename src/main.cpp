#include <filesystem>
#include <print>
#include <cstdio>


#include "parser/Dispatch.h"
#include "datasource/filesource/FileSource.h"
#include "CountingHandler.h"
#include "marketstate/MarketState.h"


int main()
{

    std::filesystem::path filePath {"/Users/unknownuser/Desktop/Git/TradingSystem/res/01302019.NASDAQ_ITCH50"};
    FileSource fileSource { filePath };
    const auto data = fileSource.bytes();

    if (data.empty())
    {
        std::println("File is empty");
        return 0;
    }

    CountingHandler counter {};
    MarketState marketStateHandler {};


    const auto parsedBytes = parseData(data, counter);
    counter.report(stdout);

    if (parsedBytes == data.size())
    {
        std::print("Parsing finished");
    }

    return 0;
}
