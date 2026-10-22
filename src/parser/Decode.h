#pragma once

#include "../Usings.h"
#include "Messages.h"
#include <bit>
#include <cstring>


template <std::unsigned_integral T>
[[nodiscard]] static T convertBLEndian(const std::uint8_t* p) noexcept
{
    T v;
    std::memcpy(&v, p, sizeof(T));
    return std::byteswap(v);
}

[[nodiscard]] static std::uint64_t convertBLEndian48(const std::uint8_t* p) noexcept
{
    std::uint64_t v {};
    std::memcpy(&v, p, 6);
    return std::byteswap(v) >> 16;
}

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


