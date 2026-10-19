#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <span>
#include <utility>

#include "../Constants.h"
#include "Messages.h"

class MessageParser
{
public:
    explicit MessageParser(std::ifstream&& inputFile)
    : file_ {std::move(inputFile)}
    {};

    bool readNextMessage();
    [[nodiscard]] std::span<const std::uint8_t> message() const
    {
        return {messageBuffer_.data(), msgLength_};
    };

    SystemEventMessage parseSystemEventMessage(std::span<const std::uint8_t> message);

    StockDirectoryMessage parseStockDirectoryMessage(std::span<const std::uint8_t> message);
    StockTradingActionMessage parseStockTradingActionMessage(std::span<const std::uint8_t> message);
    RegSHORestrictionMessage parseRegSHORestrictionMessage(std::span<const std::uint8_t> message);
    MarketParticipantPositionMessage parseMarketParticipantPositionMessage(std::span<const std::uint8_t> message);
    MWCBDeclineLevelMessage parseMWCBDeclineLevelMessage(std::span<const std::uint8_t> message);
    MWCBStatusMessage parseMWCBStatusMessage(std::span<const std::uint8_t> message);
    IPOQuotingPeriodUpdateMessage parseIPOQuotingPeriodUpdateMessage(std::span<const std::uint8_t> message);
    LULDAuctionCollarMessage parseLULDAuctionCollarMessage(std::span<const std::uint8_t> message);
    OperationalHaltMessage parseOperationalHaltMessage(std::span<const std::uint8_t> message);

    AddOrderMessage parseAddOrderMessage(std::span<const std::uint8_t> message);
    AddOrderMPIDMessage parseAddOrderMPIDMessage(std::span<const std::uint8_t> message);

    OrderExecutedMessage parseOrderExecutedMessage(std::span<const std::uint8_t> message);
    OrderExecutedWithPriceMessage parseOrderExecutedWithPriceMessage(std::span<const std::uint8_t> message);
    OrderCancelMessage parseOrderCancelMessage(std::span<const std::uint8_t> message);
    OrderDeleteMessage parseOrderDeleteMessage(std::span<const std::uint8_t> message);
    OrderReplaceMessage parseOrderReplaceMessage(std::span<const std::uint8_t> message);

    TradeMessage parseTradeMessage(std::span<const std::uint8_t> message);
    CrossTradeMessage parseCrossTradeMessage(std::span<const std::uint8_t> message);
    BrokenTradeMessage parseBrokenTradeMessage(std::span<const std::uint8_t> message);

    NOIIMessage parseNOIIMessage(std::span<const std::uint8_t> message);
    RetailPriceImprovementMessage parseRetailPriceImprovementMessage(std::span<const std::uint8_t> message);
    DirectListingCapitalRaiseMessage parseDirectListingCapitalRaiseMessage(std::span<const std::uint8_t> message);

private:
    std::array<uint8_t, Constants::kMaxMessageLength> messageBuffer_ {};
    std::ifstream file_;
    uint64_t msgLength_ {};

};
