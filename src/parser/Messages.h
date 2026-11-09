#pragma once

#include <cstdint>
#include <array>

#include "types/ItchTypes.h"


enum class MessageType : char
{
    SystemEventMessage = 'S',

    StockDirectoryMessage = 'R',
    StockTradingActionMessage = 'H',
    RegSHORestrictionMessage = 'Y',
    MarketParticipantPositionMessage = 'L',
    MWCBDeclineLevelMessage = 'V',
    MWCBStatusMessage = 'W',
    IPOQuotingPeriodUpdateMessage = 'K',
    LULDAuctionCollarMessage = 'J',
    OperationalHaltMessage= 'h',

    AddOrderMessage = 'A',
    AddOrderMPIDMessage = 'F',

    OrderExecutedMessage = 'E',
    OrderExecutedWithPriceMessage = 'C',
    OrderCancelMessage = 'X',
    OrderDeleteMessage = 'D',
    OrderReplaceMessage = 'U',

    TradeMessage = 'P',
    CrossTradeMessage = 'Q',
    BrokenTradeMessage = 'B',

    NOIIMessage = 'I',
    RetailPriceImprovementMessage = 'N',
    DirectListingCapitalRaiseMessage = 'O'
};

struct Header
{
    std::uint64_t timestamp;
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
};

enum class Side : char
{
    Buy = 'B',
    Sell = 'S'
};


// Message type 'S', payload length: 12 bytes
struct SystemEventMessage
{
    Header header;
    char eventCode;
};


// Message type 'R', payload length: 39 bytes
struct StockDirectoryMessage
{
    Header header;
    itch::Symbol stock;
    std::uint32_t roundLotSize;
    std::uint32_t etpLeverageFactor;
    std::array<char, 2> issueSubType;
    char marketCategory;
    char financialStatusIndicator;
    char roundLotsOnly;
    char issueClassification;
    char authenticity;
    char shortSaleThresholdIndicator;
    char ipoFlag;
    char luldReferencePriceTier;
    char etpFlag;
    char inverseIndicator;
};


// Message type 'H', payload length: 25 bytes
struct StockTradingActionMessage
{
    Header header;
    itch::Symbol stock;
    std::array<char, 4> tradingActionReason;
    char tradingState;
    char reserved;
};


// Message type 'Y', payload length: 20 bytes
struct RegSHORestrictionMessage
{
    Header header;
    itch::Symbol stock;
    char regSHOAction;
};

// Message type 'L', payload length: 26 bytes
struct MarketParticipantPositionMessage
{
    Header header;
    itch::Symbol stock;
    std::array<char, 4> mpid;
    char primaryMarketMaker;
    char marketMakerMode;
    char marketParticipantState;
};

// Message type 'V', payload length: 35 bytes
struct MWCBDeclineLevelMessage
{
    Header header;
    std::uint64_t level1;
    std::uint64_t level2;
    std::uint64_t level3;
};

// Message type 'W', payload length: 12 bytes
struct MWCBStatusMessage
{
    Header header;
    char breachedLevel;
};

// Message type 'K', payload length: 28 bytes
struct IPOQuotingPeriodUpdateMessage
{
    Header header;
    itch::Symbol stock;
    std::uint32_t ipoQuotationReleaseTime;
    std::uint32_t ipoPrice;
    char ipoQuotationReleaseQualifier;
};

// Message type 'J', payload length: 35 bytes
struct LULDAuctionCollarMessage
{
    Header header;
    itch::Symbol stock;
    std::uint32_t auctionCollarReferencePrice;
    std::uint32_t upperAuctionCollarPrice;
    std::uint32_t lowerAuctionCollarPrice;
    std::uint32_t AuctionCollarExtension;
};

// Message type 'h', payload length: 21 bytes
struct OperationalHaltMessage
{
    Header header;
    itch::Symbol stock;
    char marketCode;
    char operationalHaltAction;
};


// Message type 'A', payload length: 36 bytes
// Stock removed, lookup stock locate
struct AddOrderMessage
{
    Header header;
    std::uint64_t orderReferenceNumber;
    std::uint32_t shares;
    std::uint32_t price;
    Side bSIndicator;
};

// Message type 'F', payload length: 40 bytes
// Stock removed, lookup stock locate
struct AddOrderMPIDMessage
{
    Header header;
    std::uint64_t orderReferenceNumber;
    std::uint32_t shares;
    std::uint32_t price;
    std::array<char, 4> attribution;
    Side bSIndicator;
};


// Message type 'E', payload length: 31 bytes
struct OrderExecutedMessage {
    Header header;
    std::uint64_t orderReferenceNumber;
    std::uint64_t matchNumber;
    std::uint32_t executedShares;
};

// Message type 'C', payload length: 36 bytes
struct OrderExecutedWithPriceMessage {
    Header header;
    std::uint64_t orderReferenceNumber;
    std::uint64_t matchNumber;
    std::uint32_t executedShares;
    std::uint32_t executionPrice;
    char printable;
};

// Message type 'X', payload length: 23 bytes
struct OrderCancelMessage {
    Header header;
    std::uint64_t orderReferenceNumber;
    std::uint32_t cancelledShares;
};

// Message type 'D', payload length: 19 bytes
struct OrderDeleteMessage {
    Header header;
    std::uint64_t orderReferenceNumber{};
};

// Message type 'U', payload length: 35 bytes
struct OrderReplaceMessage {
    Header header;
    std::uint64_t originalOrderReferenceNumber{};
    std::uint64_t newOrderReferenceNumber{};
    std::uint32_t shares{};
    std::uint32_t price{};
};


// Message type 'P', payload length: 44 bytes
// Stock removed, lookup stock locate
struct TradeMessage {
    Header header;
    std::uint64_t orderReferenceNumber{};
    std::uint64_t matchNumber{};
    itch::Symbol stock{};
    std::uint32_t shares{};
    std::uint32_t price{};
    Side buySellIndicator{};
};

// Message type 'Q', payload length: 40 bytes
// Stock removed, lookup stock locate
struct CrossTradeMessage {
    Header header;
    std::uint64_t shares{};
    std::uint64_t matchNumber{};
    itch::Symbol stock{};
    std::uint32_t crossPrice{};
    char crossType{};
};

// Message type 'B', payload length: 19 bytes
struct BrokenTradeMessage {
    Header header;
    std::uint64_t matchNumber{};
};


// Message type 'I', payload length: 50 bytes
// Stock removed, lookup stock locate
struct NOIIMessage {
    Header header;
    std::uint64_t pairedShares{};
    std::uint64_t imbalanceShares{};
    std::uint32_t farPrice{};
    std::uint32_t nearPrice{};
    std::uint32_t currentReferencePrice{};
    char imbalanceDirection{};
    char crossType{};
    char priceVariationIndicator{};
};


// Message type 'N', payload length: 20 bytes
// Stock removed, lookup stock locate
struct RetailPriceImprovementMessage {
    Header header;
    char interestFlag{};
};


// Message type 'O', payload length: 48 bytes
// Stock removed, lookup stock locate
struct DirectListingCapitalRaiseMessage {
    Header header;
    std::uint64_t nearExecutionTime{};
    std::uint32_t minimumAllowablePrice{};
    std::uint32_t maximumAllowablePrice{};
    std::uint32_t nearExecutionPrice{};
    std::uint32_t lowerPriceRangeCollar{};
    std::uint32_t upperPriceRangeCollar{};
    char openEligibilityStatus{};
};