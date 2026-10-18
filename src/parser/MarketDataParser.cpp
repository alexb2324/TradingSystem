#include "MarketDataParser.h"


static std::string readPaddedString(const std::span<const std::uint8_t> message, const std::size_t offset, const std::size_t length)
{
    std::string result{ reinterpret_cast<const char*>(message.data() + offset), length };

    while (!result.empty() && result.back() == ' ')
    {
        result.pop_back();
    }

    return result;
}



// Reads the next length-prefixed ITCH message.
// Returns false if EOF occurs cleanly before another message.
// Throws if the length prefix or message is truncated.
bool MessageParser::readNextMessage()
{
    std::array<uint8_t, 2> lengthBuffer {};

    file_.read(reinterpret_cast<char*>(lengthBuffer.data()), static_cast<std::streamsize>(lengthBuffer.size()));

    const auto bytesRead = file_.gcount();

    if (bytesRead == 0 && file_.eof()) { return false; }
    if (bytesRead != 2) { throw std::runtime_error("Truncated message length prefix"); }

    const std::uint64_t length = readBigEndian(lengthBuffer);

    if (length == 0)
    {
        throw std::runtime_error("Invalid zero-length message");
    }

    if (length > messageBuffer_.size())
    {
        throw std::runtime_error("Message exceeds buffer capacity");
    }

    msgLength_ = static_cast<std::size_t>(length);

    // Read the message body. The prefix length includes the message
    // type byte but does not include the two-byte prefix itself.
    file_.read(reinterpret_cast<char*>(messageBuffer_.data()), static_cast<std::streamsize>(msgLength_));

    if (file_.gcount() != static_cast<std::streamsize>(msgLength_))
    {
        throw std::runtime_error("Truncated message body");
    }

    return true;
}


// Decode a big-endian integer of a specified length.
// Supports fields up to 8 bytes.
std::uint64_t MessageParser::readBigEndian(const std::span<const std::uint8_t> data)
{
    if (data.size() > sizeof(std::uint64_t))
    {
        throw std::invalid_argument("Integer field exceeds 8 bytes");
    }

    std::uint64_t result = 0;

    for (const std::uint8_t byte : data)
    {
        result = (result << 8) | byte;
    }

    return result;
}


SystemEventMessage MessageParser::parseSystemEventMessage(std::span<const std::uint8_t> message)
{
    SystemEventMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.eventCode = static_cast<char>(message[11]);

    return result;
}

StockDirectoryMessage MessageParser::parseStockDirectoryMessage(std::span<const std::uint8_t> message)
{

    StockDirectoryMessage result {};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.stock = std::string(reinterpret_cast<const char*>(message.data() + 11), 8);

    // Trim trailing spaces
    while (!result.stock.empty() && result.stock.back() == ' ')
    {
        result.stock.pop_back();
    }

    result.marketCategory = static_cast<char>(message[19]);
    result.financialStatusIndicator = static_cast<char>(message[20]);
    result.roundLotSize = static_cast<std::uint32_t>(readBigEndian({message.data() + 21, 4}));
    result.roundLotsOnly = static_cast<char>(message[25]);
    result.issueClassification = static_cast<char>(message[26]);

    // Issue sub-type: 2 bytes.
    result.issueSubType = std::string(reinterpret_cast<const char*>(message.data() + 27), 2);
    result.authenticity = static_cast<char>(message[29]);
    result.shortSaleThresholdIndicator = static_cast<char>(message[30]);
    result.ipoFlag = static_cast<char>(message[31]);
    result.luldReferencePriceTier = static_cast<char>(message[32]);
    result.etpFlag = static_cast<char>(message[33]);
    result.etpLeverageFactor = static_cast<std::uint32_t>(readBigEndian({message.data() + 34, 4}));
    result.inverseIndicator = static_cast<char>(message[38]);

    return result;
}

StockTradingActionMessage MessageParser::parseStockTradingActionMessage(std::span<const std::uint8_t> message)
{

    StockTradingActionMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.stock = readPaddedString(message, 11, 8);
    result.tradingState = static_cast<char>(message[19]);
    result.reserved = static_cast<char>(message[20]);
    result.tradingActionReason = std::string(reinterpret_cast<const char*>(message.data() + 21), 4);

    return result;
}

RegSHORestrictionMessage MessageParser::parseRegSHORestrictionMessage(std::span<const std::uint8_t> message)
{
    RegSHORestrictionMessage result{};

    result.locateCode = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.stock = readPaddedString(message, 11, 8);
    result.regSHOAction = static_cast<char>(message[19]);

    return result;
}

MarketParticipantPositionMessage MessageParser::parseMarketParticipantPositionMessage(std::span<const std::uint8_t> message)
{
    MarketParticipantPositionMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.mpid = readPaddedString(message, 11, 4);
    result.stock = readPaddedString(message, 15, 8);
    result.primaryMarketMaker = static_cast<char>(message[23]);
    result.marketMakerMode = static_cast<char>(message[24]);
    result.marketParticipantState = static_cast<char>(message[25]);

    return result;
}

MWCBDeclineLevelMessage MessageParser::parseMWCBDeclineLevelMessage(std::span<const std::uint8_t> message)
{
    MWCBDeclineLevelMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.level1 = readBigEndian({message.data() + 11, 8});
    result.level2 = readBigEndian({message.data() + 19, 8});
    result.level3 = readBigEndian({message.data() + 27, 8});

    return result;
}

MWCBStatusMessage MessageParser::parseMWCBStatusMessage(std::span<const std::uint8_t> message)
{
    MWCBStatusMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.breachedLevel = static_cast<char>(message[11]);

    return result;
}

IPOQuotingPeriodUpdateMessage MessageParser::parseIPOQuotingPeriodUpdateMessage(std::span<const std::uint8_t> message)
{
    IPOQuotingPeriodUpdateMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.stock = readPaddedString(message, 11, 8);
    result.ipoQuotationReleaseTime = static_cast<std::uint32_t>(readBigEndian({message.data() + 19, 4}));
    result.ipoQuotationReleaseQualifier = static_cast<char>(message[23]);
    result.ipoPrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 24, 4}));

    return result;
}

LULDAuctionCollarMessage MessageParser::parseLULDAuctionCollarMessage(std::span<const std::uint8_t> message)
{
    LULDAuctionCollarMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.stock = readPaddedString(message, 11, 8);
    result.auctionCollarReferencePrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 19, 4}));
    result.upperAuctionCollarPrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 23, 4}));
    result.lowerAuctionCollarPrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 27, 4}));
    result.AuctionCollarExtension = static_cast<std::uint32_t>(readBigEndian({message.data() + 31, 4}));

    return result;
}

OperationalHaltMessage MessageParser::parseOperationalHaltMessage(std::span<const std::uint8_t> message)
{
    OperationalHaltMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.stock = readPaddedString(message, 11, 8);
    result.marketCode = static_cast<char>(message[19]);
    result.operationalHaltAction = static_cast<char>(message[20]);

    return result;
}

AddOrderMessage MessageParser::parseAddOrderMessage(std::span<const std::uint8_t> message)
{
    AddOrderMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.orderReferenceNumber = readBigEndian({message.data() + 11, 8});
    result.bSIndicator = static_cast<char>(message[19]);
    result.shares = static_cast<std::uint32_t>(readBigEndian({message.data() + 20, 4}));
    result.stock = readPaddedString(message, 24, 8);
    result.price = static_cast<std::uint32_t>(readBigEndian({message.data() + 32, 4}));

    return result;
}

AddOrderMPIDMessage MessageParser::parseAddOrderMPIDMessage(std::span<const std::uint8_t> message)
{
    AddOrderMPIDMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.orderReferenceNumber = readBigEndian({message.data() + 11, 8});
    result.bSIndicator = static_cast<char>(message[19]);
    result.shares = static_cast<std::uint32_t>(readBigEndian({message.data() + 20, 4}));
    result.stock = readPaddedString(message, 24, 8);
    result.price = static_cast<std::uint32_t>(readBigEndian({message.data() + 32, 4}));
    result.attribution = std::string(reinterpret_cast<const char*>(message.data() + 36), 4);

    return result;
}

OrderExecutedMessage MessageParser::parseOrderExecutedMessage(std::span<const std::uint8_t> message)
{
    OrderExecutedMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.orderReferenceNumber = readBigEndian({message.data() + 11, 8});
    result.executedShares = static_cast<std::uint32_t>(readBigEndian({message.data() + 19, 4}));
    result.matchNumber = readBigEndian({message.data() + 23, 8});

    return result;
}

OrderExecutedWithPriceMessage MessageParser::parseOrderExecutedWithPriceMessage(std::span<const std::uint8_t> message)
{
    OrderExecutedWithPriceMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.orderReferenceNumber = readBigEndian({message.data() + 11, 8});
    result.executedShares = static_cast<std::uint32_t>(readBigEndian({message.data() + 19, 4}));
    result.matchNumber = readBigEndian({message.data() + 23, 8});
    result.printable = static_cast<char>(message[31]);
    result.executionPrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 32, 4}));

    return result;
}

OrderCancelMessage MessageParser::parseOrderCancelMessage(std::span<const std::uint8_t> message)
{
    OrderCancelMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.orderReferenceNumber = readBigEndian({message.data() + 11, 8});
    result.cancelledShares = static_cast<std::uint32_t>(readBigEndian({message.data() + 19, 4}));

    return result;
}

OrderDeleteMessage MessageParser::parseOrderDeleteMessage(std::span<const std::uint8_t> message)
{
    OrderDeleteMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.orderReferenceNumber = readBigEndian({message.data() + 11, 8});

    return result;
}

OrderReplaceMessage MessageParser::parseOrderReplaceMessage(std::span<const std::uint8_t> message)
{
    OrderReplaceMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.originalOrderReferenceNumber = readBigEndian({message.data() + 11, 8});
    result.newOrderReferenceNumber = readBigEndian({message.data() + 19, 8});
    result.shares = static_cast<std::uint32_t>(readBigEndian({message.data() + 27, 4}));
    result.price = static_cast<std::uint32_t>(readBigEndian({message.data() + 31, 4}));

    return result;
}

TradeMessage MessageParser::parseTradeMessage(std::span<const std::uint8_t> message)
{
    TradeMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.orderReferenceNumber = readBigEndian({message.data() + 11, 8});
    result.buySellIndicator = static_cast<char>(message[19]);
    result.shares = static_cast<std::uint32_t>(readBigEndian({message.data() + 20, 4}));
    result.stock = readPaddedString(message, 24, 8);
    result.price = static_cast<std::uint32_t>(readBigEndian({message.data() + 32, 4}));
    result.matchNumber = readBigEndian({message.data() + 36, 8});

    return result;
}

CrossTradeMessage MessageParser::parseCrossTradeMessage(std::span<const std::uint8_t> message)
{
    CrossTradeMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.shares = readBigEndian({message.data() + 11, 8});
    result.stock = readPaddedString(message, 19, 8);
    result.crossPrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 27, 4}));
    result.matchNumber = readBigEndian({message.data() + 31, 8});
    result.crossType = static_cast<char>(message[39]);

    return result;
}

BrokenTradeMessage MessageParser::parseBrokenTradeMessage(std::span<const std::uint8_t> message)
{
    BrokenTradeMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.matchNumber = readBigEndian({message.data() + 11, 8});

    return result;
}

NOIIMessage MessageParser::parseNOIIMessage(std::span<const std::uint8_t> message)
{

    NOIIMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.pairedShares = readBigEndian({message.data() + 11, 8});
    result.imbalanceShares = readBigEndian({message.data() + 19, 8});
    result.imbalanceDirection = static_cast<char>(message[27]);
    result.stock = readPaddedString(message, 28, 8);
    result.farPrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 36, 4}));
    result.nearPrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 40, 4}));
    result.currentReferencePrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 44, 4}));
    result.crossType = static_cast<char>(message[48]);
    result.priceVariationIndicator = static_cast<char>(message[49]);

    return result;
}

RetailPriceImprovementMessage MessageParser::parseRetailPriceImprovementMessage(std::span<const std::uint8_t> message)
{

    RetailPriceImprovementMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.stock = readPaddedString(message, 11, 8);
    result.interestFlag = static_cast<char>(message[19]);

    return result;
}

DirectListingCapitalRaiseMessage MessageParser::parseDirectListingCapitalRaiseMessage(std::span<const std::uint8_t> message)
{
    DirectListingCapitalRaiseMessage result{};

    result.stockLocate = static_cast<std::uint16_t>(readBigEndian({message.data() + 1, 2}));
    result.trackingNumber = static_cast<std::uint16_t>(readBigEndian({message.data() + 3, 2}));
    result.timestamp = readBigEndian({message.data() + 5, 6});
    result.stock = readPaddedString(message, 11, 8);
    result.openEligibilityStatus = static_cast<char>(message[19]);
    result.minimumAllowablePrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 20, 4}));
    result.maximumAllowablePrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 24, 4}));
    result.nearExecutionPrice = static_cast<std::uint32_t>(readBigEndian({message.data() + 28, 4}));
    result.nearExecutionTime = readBigEndian({message.data() + 32, 8});
    result.lowerPriceRangeCollar = static_cast<std::uint32_t>(readBigEndian({message.data() + 40, 4}));
    result.upperPriceRangeCollar = static_cast<std::uint32_t>(readBigEndian({message.data() + 44, 4}));

    return result;
}