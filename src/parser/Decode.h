#pragma once

#include "../Usings.h"

SystemEventMessage decodeSystemEventMessage(itch::Bytes message);

StockDirectoryMessage decodeStockDirectoryMessage(itch::Bytes message);
StockTradingActionMessage decodeStockTradingActionMessage(itch::Bytes message);
RegSHORestrictionMessage decodeRegSHORestrictionMessage(itch::Bytes message);
MarketParticipantPositionMessage decodeMarketParticipantPositionMessage(itch::Bytes message);
MWCBDeclineLevelMessage decodeMWCBDeclineLevelMessage(itch::Bytes message);
MWCBStatusMessage decodeMWCBStatusMessage(itch::Bytes message);
IPOQuotingPeriodUpdateMessage decodeIPOQuotingPeriodUpdateMessage(itch::Bytes message);
LULDAuctionCollarMessage decodeLULDAuctionCollarMessage(itch::Bytes message);
OperationalHaltMessage decodeOperationalHaltMessage(itch::Bytes message);

AddOrderMessage decodeAddOrderMessage(itch::Bytes message);
AddOrderMPIDMessage decodeAddOrderMPIDMessage(itch::Bytes message);
OrderExecutedMessage decodeOrderExecutedMessage(itch::Bytes message);
OrderExecutedWithPriceMessage decodeOrderExecutedWithPriceMessage(itch::Bytes message);
OrderCancelMessage decodeOrderCancelMessage(itch::Bytes message);
OrderDeleteMessage decodeOrderDeleteMessage(itch::Bytes message);
OrderReplaceMessage decodeOrderReplaceMessage(itch::Bytes message);

TradeMessage decodeTradeMessage(itch::Bytes message);
CrossTradeMessage decodeCrossTradeMessage(itch::Bytes message);
BrokenTradeMessage decodeBrokenTradeMessage(itch::Bytes message);

NOIIMessage decodeNOIIMessage(itch::Bytes message);
RetailPriceImprovementMessage decodeRetailPriceImprovementMessage(itch::Bytes message);
DirectListingCapitalRaiseMessage decodeDirectListingCapitalRaiseMessage(itch::Bytes message);





/*
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

private:
    std::array<uint8_t, Constants::kMaxMessageLength> messageBuffer_ {};
    std::ifstream file_;
    uint64_t msgLength_ {};

};
*/

