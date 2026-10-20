#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <filesystem>

#include "parser/Messages.h"
#include "parser/MarketDataParser.h"

#include "tradebook/Tradebook.h"

#include "directory/StockDirectory.h"

namespace fs = std::filesystem;

// ------------------------------------------------------------
// Main
// ------------------------------------------------------------

int main()
{

    fs::path filePath = "/home/hehexd2324/CLionProjects/TradingSystem/res/01302019.NASDAQ_ITCH50";
    std::ifstream inputFile {filePath, std::ios::binary };

    if (!inputFile)
    {
        std::cerr << "Failed to open ITCH file\n";
        return 1;
    }

    MessageParser messageParser { std::move(inputFile) };
    TradeBook tradeBook {};
    StockDirectory stockDirectory {};

    try
    {
        while (messageParser.readNextMessage())
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
                    stockDirectory.displayAllStocks();
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
                    break;
                }
                case MessageType::OrderExecutedWithPriceMessage:
                {
                    const auto msg = messageParser.parseOrderExecutedWithPriceMessage(message);
                    //printOrderExecutedWithPriceMessage(msg);
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
                    //tradeBook.addTrade(msg);
                    //tradeBook.displayMostRecentTrade();
                    break;
                }
                case MessageType::CrossTradeMessage:
                {
                    const auto msg = messageParser.parseCrossTradeMessage(message);
                   // printCrossTradeMessage(msg);
                    break;
                }
                case MessageType::BrokenTradeMessage:
                {
                    const auto msg = messageParser.parseBrokenTradeMessage(message);
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
    }
    catch (const std::exception& e)
    {
        std::cerr << "Parsing error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}