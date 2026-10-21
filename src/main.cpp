#include <cstdint>
#include <cstddef>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <filesystem>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "parser/Messages.h"
#include "parser/Decode.h"

#include "tradebook/TradeBook.h"

#include "directory/StockDirectory.h"
#include "parser/Dispatch.h"

namespace fs = std::filesystem;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main()
{
    const int fd = open("/home/hehexd2324/CLionProjects/TradingSystem/res/01302019.NASDAQ_ITCH50", O_RDONLY);

    if (fd == -1) { throw std::runtime_error("Couldn't open file"); }

    struct stat fileInfo {};

    if (fstat(fd, &fileInfo) == -1)
    {
        close(fd);
        throw std::runtime_error("Couldn't get file stats");
    }

    if (fileInfo.st_size == 0)
    {
        close(fd);
        std::println("File is empty");
        return 0;
    }

    const auto fileSize = static_cast<std::size_t>(fileInfo.st_size);

    void* fileMapping = mmap(nullptr, fileSize, PROT_READ, MAP_PRIVATE, fd, 0);

    if (fileMapping == MAP_FAILED)
    {
        close(fd);
        throw std::runtime_error("Failed to map file");
    }

    close(fd);

    const auto* data = static_cast<const std::uint8_t*>(fileMapping);

    TradeBook tradeBook {};
    StockDirectory stockDirectory {};



    while (readNextMessage(inputFile))
    {
        const auto message = messageParser.message();
        switch (static_cast<MessageType>(message[0]))
        {
            case MessageType::SystemEventMessage:
            {
                const auto msg = messageParser.parseSystemEventMessage(message);
                //printSystemEventMessage(msg);
                break;
            }
            case MessageType::StockDirectoryMessage:
            {
                const auto msg = messageParser.parseStockDirectoryMessage(message);
                stockDirectory.insert(msg.stock, msg.header.stockLocate);
                //printStockDirectoryMessage(msg);
                break;
            }
            case MessageType::StockTradingActionMessage:
            {
                const auto msg = messageParser.parseStockTradingActionMessage(message);
                //printStockTradingActionMessage(msg);
                break;
            }
            case MessageType::RegSHORestrictionMessage:
            {
                const auto msg = messageParser.parseRegSHORestrictionMessage(message);
                //printRegSHORestrictionMessage(msg);
                break;
            }
            case MessageType::MarketParticipantPositionMessage:
            {
                const auto msg = messageParser.parseMarketParticipantPositionMessage(message);
                //printMarketParticipantPositionMessage(msg);
                break;
            }
            case MessageType::MWCBDeclineLevelMessage:
            {
                const auto msg = messageParser.parseMWCBDeclineLevelMessage(message);
                //printMWCBDeclineLevelMessage(msg);
                break;
            }
            case MessageType::MWCBStatusMessage:
            {
                const auto msg = messageParser.parseMWCBStatusMessage(message);
                //printMWCBStatusMessage(msg);
                break;
            }
            case MessageType::IPOQuotingPeriodUpdateMessage:
            {
                const auto msg = messageParser.parseIPOQuotingPeriodUpdateMessage(message);
                //printIPOQuotingPeriodUpdateMessage(msg);
                break;
            }
            case MessageType::LULDAuctionCollarMessage:
            {
                const auto msg = messageParser.parseLULDAuctionCollarMessage(message);
                //printLULDAuctionCollarMessage(msg);
                break;
            }
            case MessageType::OperationalHaltMessage:
            {
                const auto msg = messageParser.parseOperationalHaltMessage(message);
                //printOperationalHaltMessage(msg);
                break;
            }
            case MessageType::AddOrderMessage:
            {
                const auto msg = messageParser.parseAddOrderMessage(message);
                //printAddOrderMessage(msg);
                break;
            }
            case MessageType::AddOrderMPIDMessage:
            {
                const auto msg = messageParser.parseAddOrderMPIDMessage(message);
                //printAddOrderMPIDMessage(msg);
                break;
            }
            case MessageType::OrderExecutedMessage:
            {
                const auto msg = messageParser.parseOrderExecutedMessage(message);
                //printOrderExecutedMessage(msg);
                tradeBook.addExecutedOrder(msg);
                tradeBook.displayLastExecutedOrder();
                break;
            }
            case MessageType::OrderExecutedWithPriceMessage:
            {
                const auto msg = messageParser.parseOrderExecutedWithPriceMessage(message);
                //printOrderExecutedWithPriceMessage(msg);
                tradeBook.addExecutedPriceOrder(msg);
                tradeBook.displayLastExecutedPriceOrder();
                break;
            }
            case MessageType::OrderCancelMessage:
            {
                const auto msg = messageParser.parseOrderCancelMessage(message);
                //printOrderCancelMessage(msg);
                break;
            }
            case MessageType::OrderDeleteMessage:
            {
                const auto msg = messageParser.parseOrderDeleteMessage(message);
                //printOrderDeleteMessage(msg);
                break;
            }
            case MessageType::OrderReplaceMessage:
            {
                const auto msg = messageParser.parseOrderReplaceMessage(message);
               // printOrderReplaceMessage(msg);
                break;
            }
            case MessageType::TradeMessage:
            {
                const auto msg = messageParser.parseTradeMessage(message);
                tradeBook.addTrade(msg);
                tradeBook.displayLastTrade();
                break;
            }
            case MessageType::CrossTradeMessage:
            {
                const auto msg = messageParser.parseCrossTradeMessage(message);
                tradeBook.addCrossTrade(msg);
                tradeBook.displayLastCrossTrade();
               // printCrossTradeMessage(msg);
                break;
            }
            case MessageType::BrokenTradeMessage:
            {
                const auto msg = messageParser.parseBrokenTradeMessage(message);
                tradeBook.addBrokenTrade(msg);
                tradeBook.displayLastBrokenTrade();
                //printBrokenTradeMessage(msg);
                break;
            }
            case MessageType::NOIIMessage:
            {
                const auto msg = messageParser.parseNOIIMessage(message);
               // printNOIIMessage(msg);
                break;
            }
            case MessageType::RetailPriceImprovementMessage:
            {
                const auto msg = messageParser.parseRetailPriceImprovementMessage(message);
                //printRetailPriceImprovementMessage(msg);
                break;
            }
            case MessageType::DirectListingCapitalRaiseMessage:
            {
                const auto msg = messageParser.parseDirectListingCapitalRaiseMessage(message);
               // printDirectListingCapitalRaiseMessage(msg);
                break;
            }
            default:
                throw std::runtime_error("Unknown message type");
        }
    }

    if (munmap(fileMapping, fileSize) == -1)
    {
        throw std::runtime_error("Failed to unmap file");
    }

    return 0;
}