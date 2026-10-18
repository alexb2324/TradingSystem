#pragma once

#include <cstdint>
#include <string>

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


// ---------------------------------------------------
// System Event
// ---------------------------------------------------

// Message type 'S', payload length: 12 bytes
struct SystemEventMessage
{
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    char eventCode;
};


// ---------------------------------------------------
// Stock Related
// ---------------------------------------------------

// Message type 'R', payload length: 39 bytes
struct StockDirectoryMessage
{
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::string stock;
    char marketCategory;
    char financialStatusIndicator;
    std::uint32_t roundLotSize;
    char roundLotsOnly;
    char issueClassification;
    std::string issueSubType;
    char authenticity;
    char shortSaleThresholdIndicator;
    char ipoFlag;
    char luldReferencePriceTier;
    char etpFlag;
    std::uint32_t etpLeverageFactor;
    char inverseIndicator;
};


// Message type 'H', payload length: 25 bytes
struct StockTradingActionMessage
{
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::string stock;
    char tradingState;
    char reserved;
    std::string tradingActionReason;
};


// Message type 'Y', payload length: 20 bytes
struct RegSHORestrictionMessage
{
    std::uint16_t locateCode;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::string stock;
    char regSHOAction;

};


// Message type 'L', payload length: 26 bytes
struct MarketParticipantPositionMessage
{
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::string mpid;
    std::string stock;
    char primaryMarketMaker;
    char marketMakerMode;
    char marketParticipantState;
};


// Message type 'V', payload length: 35 bytes
struct MWCBDeclineLevelMessage
{
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::uint64_t level1;
    std::uint64_t level2;
    std::uint64_t level3;
};


// Message type 'W', payload length: 12 bytes
struct MWCBStatusMessage
{
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    char breachedLevel;
};


// Message type 'K', payload length: 28 bytes
struct IPOQuotingPeriodUpdateMessage
{
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::string stock;
    std::uint32_t ipoQuotationReleaseTime;
    char ipoQuotationReleaseQualifier;
    std::uint32_t ipoPrice;
};


// Message type 'J', payload length: 35 bytes
struct LULDAuctionCollarMessage
{
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::string stock;
    std::uint32_t auctionCollarReferencePrice;
    std::uint32_t upperAuctionCollarPrice;
    std::uint32_t lowerAuctionCollarPrice;
    std::uint32_t AuctionCollarExtension;
    char ipoQuotationReleaseQualifier;
    std::uint32_t ipoPrice;
};


// Message type 'h', payload length: 21 bytes
struct OperationalHaltMessage
{
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::string stock;
    char marketCode;
    char operationalHaltAction;
};



// ---------------------------------------------------
// Add Order
// ---------------------------------------------------

// Message type 'A', payload length: 36 bytes
struct AddOrderMessage
{
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::uint64_t orderReferenceNumber;
    char bSIndicator;
    std::uint32_t shares;
    std::string stock;
    std::uint32_t price;
};


// Message type 'F', payload length: 40 bytes
struct AddOrderMPIDMessage
{
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::uint64_t orderReferenceNumber;
    char bSIndicator;
    std::uint32_t shares;
    std::string stock;
    std::uint32_t price;
    std::string attribution;
};


// ---------------------------------------------------
// Modify Order
// ---------------------------------------------------

// Message type 'E', payload length: 31 bytes
struct OrderExecutedMessage {
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::uint64_t orderReferenceNumber;
    std::uint32_t executedShares;
    std::uint64_t matchNumber;
};


// Message type 'C', payload length: 36 bytes
struct OrderExecutedWithPriceMessage {
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::uint64_t orderReferenceNumber;
    std::uint32_t executedShares;
    std::uint64_t matchNumber;
    char printable;
    std::uint32_t executionPrice;
};


// Message type 'X', payload length: 23 bytes
struct OrderCancelMessage {
    std::uint16_t stockLocate;
    std::uint16_t trackingNumber;
    std::uint64_t timestamp;
    std::uint64_t orderReferenceNumber;
    std::uint32_t cancelledShares;
};


// Message type 'D', payload length: 19 bytes
struct OrderDeleteMessage {
    std::uint16_t stockLocate{};
    std::uint16_t trackingNumber{};
    std::uint64_t timestamp{};
    std::uint64_t orderReferenceNumber{};
};


// Message type 'U', payload length: 35 bytes
struct OrderReplaceMessage {
    std::uint16_t stockLocate{};
    std::uint16_t trackingNumber{};
    std::uint64_t timestamp{};
    std::uint64_t originalOrderReferenceNumber{};
    std::uint64_t newOrderReferenceNumber{};
    std::uint32_t shares{};
    std::uint32_t price{};
};



// ---------------------------------------------------
// Trade
// ---------------------------------------------------

// Message type 'P', payload length: 44 bytes
struct TradeMessage {
    std::uint16_t stockLocate{};
    std::uint16_t trackingNumber{};
    std::uint64_t timestamp{};
    std::uint64_t orderReferenceNumber{};
    char buySellIndicator{};
    std::uint32_t shares{};
    std::string stock;
    std::uint32_t price{};
    std::uint64_t matchNumber{};
};


// Message type 'Q', payload length: 40 bytes
struct CrossTradeMessage {
    std::uint16_t stockLocate{};
    std::uint16_t trackingNumber{};
    std::uint64_t timestamp{};
    std::uint64_t shares{};
    std::string stock;
    std::uint32_t crossPrice{};
    std::uint64_t matchNumber{};
    char crossType{};
};


// Message type 'B', payload length: 19 bytes
struct BrokenTradeMessage {
    std::uint16_t stockLocate{};
    std::uint16_t trackingNumber{};
    std::uint64_t timestamp{};
    std::uint64_t matchNumber{};
};


// ---------------------------------------------------
// Net Order Imbalance Indicator
// ---------------------------------------------------


// Message type 'I', payload length: 50 bytes
struct NOIIMessage {
    std::uint16_t stockLocate{};
    std::uint16_t trackingNumber{};
    std::uint64_t timestamp{};
    std::uint64_t pairedShares{};
    std::uint64_t imbalanceShares{};
    char imbalanceDirection{};
    std::string stock;
    std::uint32_t farPrice{};
    std::uint32_t nearPrice{};
    std::uint32_t currentReferencePrice{};
    char crossType{};
    char priceVariationIndicator{};
};


// ---------------------------------------------------
// Retail Price Improvement Indicator
// ---------------------------------------------------

// Message type 'N', payload length: 20 bytes
struct RetailPriceImprovementMessage {
    std::uint16_t stockLocate{};
    std::uint16_t trackingNumber{};
    std::uint64_t timestamp{};
    std::string stock;
    char interestFlag{};
};



// ---------------------------------------------------
// Direct Listing with Capital Raise Price Discovery
// ---------------------------------------------------

// Message type 'O', payload length: 48 bytes
struct DirectListingCapitalRaiseMessage {
    std::uint16_t stockLocate{};
    std::uint16_t trackingNumber{};
    std::uint64_t timestamp{};
    std::string stock;
    char openEligibilityStatus{};
    std::uint32_t minimumAllowablePrice{};
    std::uint32_t maximumAllowablePrice{};
    std::uint32_t nearExecutionPrice{};
    std::uint64_t nearExecutionTime{};
    std::uint32_t lowerPriceRangeCollar{};
    std::uint32_t upperPriceRangeCollar{};
};



void printSystemEventMessage(const SystemEventMessage& msg);

void printStockDirectoryMessage(const StockDirectoryMessage& msg);
void printStockTradingActionMessage(const StockTradingActionMessage& msg);
void printRegSHORestrictionMessage(const RegSHORestrictionMessage& msg);
void printMarketParticipantPositionMessage(const MarketParticipantPositionMessage& msg);
void printMWCBDeclineLevelMessage(const MWCBDeclineLevelMessage& msg);
void printMWCBStatusMessage(const MWCBStatusMessage& msg);
void printIPOQuotingPeriodUpdateMessage(const IPOQuotingPeriodUpdateMessage& msg);
void printLULDAuctionCollarMessage(const LULDAuctionCollarMessage& msg);
void printOperationalHaltMessage(const OperationalHaltMessage& msg);

void printAddOrderMessage(const AddOrderMessage& msg);
void printAddOrderMPIDMessage(const AddOrderMPIDMessage& msg);

void printOrderExecutedMessage(const OrderExecutedMessage& msg);
void printOrderExecutedWithPriceMessage(const OrderExecutedWithPriceMessage& msg);
void printOrderCancelMessage(const OrderCancelMessage& msg);
void printOrderDeleteMessage(const OrderDeleteMessage& msg);
void printOrderReplaceMessage(const OrderReplaceMessage& msg);

void printTradeMessage(const TradeMessage& msg);
void printCrossTradeMessage(const CrossTradeMessage& msg);
void printBrokenTradeMessage(const BrokenTradeMessage& msg);

void printNOIIMessage(const NOIIMessage& msg);
void printRetailPriceImprovementMessage(const RetailPriceImprovementMessage& msg);
void printDirectListingCapitalRaiseMessage(const DirectListingCapitalRaiseMessage& msg);