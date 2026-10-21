#include "Messages.h"
#include "Decode.h"
#include <cstring>
#include <iostream>
#include <bit>


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


// Reads the next length-prefixed ITCH message.
// Returns false if EOF occurs cleanly before another message.
// Throws if the length prefix or message is truncated.
/*
bool MessageParser::
*/

SystemEventMessage decodeSystemEventMessage(itch::Bytes message)
{
    SystemEventMessage result {};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.eventCode = static_cast<char>(message[11]);

    return result;
}

StockDirectoryMessage decodeStockDirectoryMessage(itch::Bytes message)
{
    StockDirectoryMessage result {};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);

    // Copy stock symbol
    std::copy_n(message.data() + 11, result.stock.symbolPadded.size(), result.stock.symbolPadded.begin());

    result.marketCategory = static_cast<char>(message[19]);
    result.financialStatusIndicator = static_cast<char>(message[20]);
    result.roundLotSize = convertBLEndian<std::uint32_t>(message.data() + 21);
    result.roundLotsOnly = static_cast<char>(message[25]);
    result.issueClassification = static_cast<char>(message[26]);

    // Copy issue sub-type
    std::copy_n(message.data() + 27, result.issueSubType.size(), result.issueSubType.begin());
    result.authenticity = static_cast<char>(message[29]);
    result.shortSaleThresholdIndicator = static_cast<char>(message[30]);
    result.ipoFlag = static_cast<char>(message[31]);
    result.luldReferencePriceTier = static_cast<char>(message[32]);
    result.etpFlag = static_cast<char>(message[33]);
    result.etpLeverageFactor = convertBLEndian<std::uint32_t>(message.data() + 34);
    result.inverseIndicator = static_cast<char>(message[38]);

    return result;
}

StockTradingActionMessage decodeStockTradingActionMessage(itch::Bytes message)
{

    StockTradingActionMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);

    // Copy stock symbol
    std::copy_n(message.data() + 11, result.stock.symbolPadded.size(), result.stock.symbolPadded.begin());

    // Copy trading action reason
    std::copy_n(message.data() + 21, result.tradingActionReason.size(), result.tradingActionReason.begin());
    result.tradingState = static_cast<char>(message[19]);
    result.reserved = static_cast<char>(message[20]);

    return result;
}

RegSHORestrictionMessage decodeRegSHORestrictionMessage(itch::Bytes message)
{
    RegSHORestrictionMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);

    // Copy stock symbol
    std::copy_n(message.data() + 11, result.stock.symbolPadded.size(), result.stock.symbolPadded.begin());
    result.regSHOAction = static_cast<char>(message[19]);

    return result;
}

MarketParticipantPositionMessage decodeMarketParticipantPositionMessage(itch::Bytes message)
{
    MarketParticipantPositionMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);

    // Copy MPID
    std::copy_n(message.data() + 11, result.mpid.size(), result.mpid.begin());

    // Copy stock symbol
    std::copy_n(message.data() + 15, result.stock.symbolPadded.size(), result.stock.symbolPadded.begin());
    result.primaryMarketMaker = static_cast<char>(message[23]);
    result.marketMakerMode = static_cast<char>(message[24]);
    result.marketParticipantState = static_cast<char>(message[25]);

    return result;
}

MWCBDeclineLevelMessage decodeMWCBDeclineLevelMessage(itch::Bytes message)
{
    MWCBDeclineLevelMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.level1 = convertBLEndian<std::uint64_t>(message.data() + 11);
    result.level2 = convertBLEndian<std::uint64_t>(message.data() + 19);
    result.level3 = convertBLEndian<std::uint64_t>(message.data() + 27);

    return result;
}

MWCBStatusMessage decodeMWCBStatusMessage(itch::Bytes message)
{
    MWCBStatusMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.breachedLevel = static_cast<char>(message[11]);

    return result;
}

IPOQuotingPeriodUpdateMessage decodeIPOQuotingPeriodUpdateMessage(itch::Bytes message)
{
    IPOQuotingPeriodUpdateMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);

    // Copy stock symbol
    std::copy_n(message.data() + 11, result.stock.symbolPadded.size(), result.stock.symbolPadded.begin());
    result.ipoQuotationReleaseTime = convertBLEndian<std::uint32_t>(message.data() + 19);
    result.ipoQuotationReleaseQualifier = static_cast<char>(message[23]);
    result.ipoPrice = convertBLEndian<std::uint32_t>(message.data() + 24);

    return result;
}

LULDAuctionCollarMessage decodeLULDAuctionCollarMessage(itch::Bytes message)
{
    LULDAuctionCollarMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);

    // Copy stock symbol
    std::copy_n(message.data() + 11, result.stock.symbolPadded.size(), result.stock.symbolPadded.begin());
    result.auctionCollarReferencePrice = convertBLEndian<std::uint32_t>(message.data() + 19);
    result.upperAuctionCollarPrice = convertBLEndian<std::uint32_t>(message.data() + 23);
    result.lowerAuctionCollarPrice = convertBLEndian<std::uint32_t>(message.data() + 27);
    result.AuctionCollarExtension = convertBLEndian<std::uint32_t>(message.data() + 31);

    return result;
}

OperationalHaltMessage decodeOperationalHaltMessage(itch::Bytes message)
{
    OperationalHaltMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);

    // Copy stock symbol
    std::copy_n(message.data() + 11, result.stock.symbolPadded.size(), result.stock.symbolPadded.begin());
    result.marketCode = static_cast<char>(message[19]);
    result.operationalHaltAction = static_cast<char>(message[20]);

    return result;
}

AddOrderMessage decodeAddOrderMessage(itch::Bytes message)
{
    AddOrderMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.orderReferenceNumber = convertBLEndian<std::uint64_t>(message.data() + 11);
    result.bSIndicator = static_cast<char>(message[19]);
    result.shares = convertBLEndian<std::uint32_t>(message.data() + 20);
    result.price = convertBLEndian<std::uint32_t>(message.data() + 32);

    return result;
}

AddOrderMPIDMessage decodeAddOrderMPIDMessage(itch::Bytes message)
{
    AddOrderMPIDMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.orderReferenceNumber = convertBLEndian<std::uint64_t>(message.data() + 11);
    result.bSIndicator = static_cast<char>(message[19]);
    result.shares = convertBLEndian<std::uint32_t>(message.data() + 20);
    result.price = convertBLEndian<std::uint32_t>(message.data() + 32);

    // Copy attribution
    std::copy_n(message.data() + 36, result.attribution.size(), result.attribution.begin());

    return result;
}

OrderExecutedMessage decodeOrderExecutedMessage(itch::Bytes message)
{
    OrderExecutedMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.orderReferenceNumber = convertBLEndian<std::uint64_t>(message.data() + 11);
    result.executedShares = convertBLEndian<std::uint32_t>(message.data() + 19);
    result.matchNumber = convertBLEndian<std::uint64_t>(message.data() + 23);

    return result;
}

OrderExecutedWithPriceMessage decodeOrderExecutedWithPriceMessage(itch::Bytes message)
{
    OrderExecutedWithPriceMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.orderReferenceNumber = convertBLEndian<std::uint64_t>(message.data() + 11);
    result.executedShares = convertBLEndian<std::uint32_t>(message.data() + 19);
    result.matchNumber = convertBLEndian<std::uint64_t>(message.data() + 23);
    result.printable = static_cast<char>(message[31]);
    result.executionPrice.priceOriginal = convertBLEndian<std::uint32_t>(message.data() + 32);

    return result;
}

OrderCancelMessage decodeOrderCancelMessage(itch::Bytes message)
{
    OrderCancelMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.orderReferenceNumber = convertBLEndian<std::uint64_t>(message.data() + 11);
    result.cancelledShares = convertBLEndian<std::uint32_t>(message.data() + 19);

    return result;
}

OrderDeleteMessage decodeOrderDeleteMessage(itch::Bytes message)
{
    OrderDeleteMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.orderReferenceNumber = convertBLEndian<std::uint64_t>(message.data() + 11);

    return result;
}

OrderReplaceMessage decodeOrderReplaceMessage(itch::Bytes message)
{
    OrderReplaceMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.originalOrderReferenceNumber = convertBLEndian<std::uint64_t>(message.data() + 11);
    result.newOrderReferenceNumber = convertBLEndian<std::uint64_t>(message.data() + 19);
    result.shares = convertBLEndian<std::uint32_t>(message.data() + 27);
    result.price = convertBLEndian<std::uint32_t>(message.data() + 31);

    return result;
}

TradeMessage decodeTradeMessage(itch::Bytes message)
{
    TradeMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.orderReferenceNumber = convertBLEndian<std::uint64_t>(message.data() + 11);
    result.buySellIndicator = static_cast<char>(message[19]);
    result.shares = convertBLEndian<std::uint32_t>(message.data() + 20);

    // Copy stock symbol
    std::copy_n(message.data() + 24, result.stock.symbolPadded.size(), result.stock.symbolPadded.begin());
    result.price.priceOriginal = convertBLEndian<std::uint32_t>(message.data() + 32);
    result.matchNumber = convertBLEndian<std::uint64_t>(message.data() + 36);

    return result;
}

CrossTradeMessage decodeCrossTradeMessage(itch::Bytes message)
{
    CrossTradeMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);

    // Copy stock symbol
    std::copy_n(message.data() + 19, result.stock.symbolPadded.size(), result.stock.symbolPadded.begin());
    result.shares = convertBLEndian<std::uint64_t>(message.data() + 11);
    result.crossPrice.priceOriginal = convertBLEndian<std::uint32_t>(message.data() + 27);
    result.matchNumber = convertBLEndian<std::uint64_t>(message.data() + 31);
    result.crossType = static_cast<char>(message[39]);

    return result;
}

BrokenTradeMessage decodeBrokenTradeMessage(itch::Bytes message)
{
    BrokenTradeMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.matchNumber = convertBLEndian<std::uint64_t>(message.data() + 11);

    return result;
}

NOIIMessage decodeNOIIMessage(itch::Bytes message)
{

    NOIIMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.pairedShares = convertBLEndian<std::uint64_t>(message.data() + 11);
    result.imbalanceShares = convertBLEndian<std::uint64_t>(message.data() + 19);
    result.imbalanceDirection = static_cast<char>(message[27]);
    result.farPrice = convertBLEndian<std::uint32_t>(message.data() + 36);
    result.nearPrice = convertBLEndian<std::uint32_t>(message.data() + 40);
    result.currentReferencePrice = convertBLEndian<std::uint32_t>(message.data() + 44);
    result.crossType = static_cast<char>(message[48]);
    result.priceVariationIndicator = static_cast<char>(message[49]);

    return result;
}

RetailPriceImprovementMessage decodeRetailPriceImprovementMessage(itch::Bytes message)
{

    RetailPriceImprovementMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.interestFlag = static_cast<char>(message[19]);

    return result;
}

DirectListingCapitalRaiseMessage decodeDirectListingCapitalRaiseMessage(itch::Bytes message)
{
    DirectListingCapitalRaiseMessage result{};

    result.header.timestamp = convertBLEndian48(message.data() + 5);
    result.header.trackingNumber = convertBLEndian<std::uint16_t>(message.data() + 3);
    result.header.stockLocate = convertBLEndian<std::uint16_t>(message.data() + 1);
    result.openEligibilityStatus = static_cast<char>(message[19]);
    result.minimumAllowablePrice = convertBLEndian<std::uint32_t>(message.data() + 20);
    result.maximumAllowablePrice = convertBLEndian<std::uint32_t>(message.data() + 24);
    result.nearExecutionPrice = convertBLEndian<std::uint32_t>(message.data() + 28);
    result.nearExecutionTime = convertBLEndian<std::uint64_t>(message.data() + 32);
    result.lowerPriceRangeCollar = convertBLEndian<std::uint32_t>(message.data() + 40);
    result.upperPriceRangeCollar = convertBLEndian<std::uint32_t>(message.data() + 44);

    return result;
}