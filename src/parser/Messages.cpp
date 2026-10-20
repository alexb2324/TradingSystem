#include "Messages.h"

#include "../../libs/nlohmann/json.hpp"
#include <iostream>

using json = nlohmann::json;

// ------------------------------------------------------------
// System Event Message ('S')
// ------------------------------------------------------------
/*
void printSystemEventMessage(const SystemEventMessage& msg)
{
    const json j = {
        {"System Event Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Event Code", std::string(1, msg.eventCode)}
        }}
    };

    std::cout << j.dump(2) << '\n';
}

// ------------------------------------------------------------
// Stock Directory Message ('R')
// ------------------------------------------------------------

void printStockDirectoryMessage(const StockDirectoryMessage& msg)
{
    const json j = {
        {"Stock Directory Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Stock", msg.stock.displaySymbol()},
            {"Market Category", std::string(1, msg.marketCategory)},
            {"Financial Status Indicator", std::string(1, msg.financialStatusIndicator)},
            {"Round Lot Size", msg.roundLotSize},
            {"Round Lots Only", std::string(1, msg.roundLotsOnly)},
            {"Issue Classification", std::string(1, msg.issueClassification)},
            {"Issue Sub-Type", msg.issueSubType},
            {"Authenticity", std::string(1, msg.authenticity)},
            {"Short Sale Threshold Indicator", std::string(1, msg.shortSaleThresholdIndicator)},
            {"IPO Flag", std::string(1, msg.ipoFlag)},
            {"LULD Reference Price Tier", std::string(1, msg.luldReferencePriceTier)},
            {"ETP Flag", std::string(1, msg.etpFlag)},
            {"ETP Leverage Factor", msg.etpLeverageFactor},
            {"Inverse Indicator", std::string(1, msg.inverseIndicator)}
        }}
    };

    std::cout << j.dump(4) << '\n';
}

// ------------------------------------------------------------
// Stock Trading Action Message ('H')
// ------------------------------------------------------------

void printStockTradingActionMessage(const StockTradingActionMessage& msg)
{

    std::string str(msg.stock.symbolPadded.data(), 4);
    const json j = {
        {"Stock Trading Action Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Stock", str},
            {"Trading State", std::string(1, msg.tradingState)},
            {"Reserved", std::string(1, msg.reserved)},
            {"Trading Action Reason", msg.tradingActionReason}
        }}
    };

    std::cout << j.dump(2) << '\n';
}

// ------------------------------------------------------------
// Reg SHO Restriction Message ('Y')
// ------------------------------------------------------------

void printRegSHORestrictionMessage(const RegSHORestrictionMessage& msg)
{
    const json j = {
        {"Reg SHO Restriction Message", {
            {"Locate Code", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Stock", msg.stock.displaySymbol()},
            {"Reg SHO Action", std::string(1, msg.regSHOAction)}
        }}
    };

    std::cout << j.dump(2) << '\n';
}

// ------------------------------------------------------------
// Market Participant Position Message ('L')
// ------------------------------------------------------------

void printMarketParticipantPositionMessage(const MarketParticipantPositionMessage& msg)
{
    const json j = {
        {"Market Participant Position Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"MPID", std::string {msg.mpid.data(), 4}},
            {"Stock", msg.stock.displaySymbol() },
            {"Primary Market Maker", std::string(1, msg.primaryMarketMaker)},
            {"Market Maker Mode", std::string(1, msg.marketMakerMode)},
            {"Market Participant State", std::string(1, msg.marketParticipantState)}
        }}
    };

    std::cout << j.dump(2) << '\n';
}

// ------------------------------------------------------------
// Market-Wide Circuit Breaker Decline Level Message ('V')
// ------------------------------------------------------------

void printMWCBDeclineLevelMessage(const MWCBDeclineLevelMessage& msg)
{
    const json j = {
        {"MWCB Decline Level Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Level 1", msg.level1},
            {"Level 2", msg.level2},
            {"Level 3", msg.level3}
        }}
    };

    std::cout << j.dump(2) << '\n';
}

// ------------------------------------------------------------
// Market-Wide Circuit Breaker Status Message ('W')
// ------------------------------------------------------------

void printMWCBStatusMessage(const MWCBStatusMessage& msg)
{
    const json j = {
        {"MWCB Status Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Breached Level", std::string(1, msg.breachedLevel)}
        }}
    };

    std::cout << j.dump(2) << '\n';
}

// ------------------------------------------------------------
// IPO Quoting Period Update Message ('K')
// ------------------------------------------------------------

void printIPOQuotingPeriodUpdateMessage(
    const IPOQuotingPeriodUpdateMessage& msg)
{
    const json j = {
        {"IPO Quoting Period Update Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Stock", msg.stock.displaySymbol()},
            {"IPO Quotation Release Time", msg.ipoQuotationReleaseTime},
            {"IPO Quotation Release Qualifier",
                std::string(1, msg.ipoQuotationReleaseQualifier)},
            {"IPO Price", msg.ipoPrice}
        }}
    };

    std::cout << j.dump(2) << '\n';
}

// ------------------------------------------------------------
// LULD Auction Collar Message ('J')
// ------------------------------------------------------------

void printLULDAuctionCollarMessage(const LULDAuctionCollarMessage& msg)
{
    const json j = {
        {"LULD Auction Collar Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Stock", msg.stock.displaySymbol()},
            {"Auction Collar Reference Price", msg.auctionCollarReferencePrice},
            {"Upper Auction Collar Price", msg.upperAuctionCollarPrice},
            {"Lower Auction Collar Price", msg.lowerAuctionCollarPrice},
            {"Auction Collar Extension", msg.AuctionCollarExtension}
        }}
    };

    std::cout << j.dump(2) << '\n';
}

// ------------------------------------------------------------
// Operational Halt Message ('h')
// ------------------------------------------------------------

void printOperationalHaltMessage(const OperationalHaltMessage& msg)
{
    const json j = {
        {"Operational Halt Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Stock", msg.stock.displaySymbol()},
            {"Market Code", std::string(1, msg.marketCode)},
            {"Operational Halt Action", std::string(1, msg.operationalHaltAction)}
        }}
    };

    std::cout << j.dump(2) << '\n';
}

// ------------------------------------------------------------
// Add Order Message ('A')
// ------------------------------------------------------------

void printAddOrderMessage(const AddOrderMessage& msg)
{
    const json j = {
        {"Add Order Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Order Reference Number", msg.orderReferenceNumber},
            {"Buy/Sell Indicator", std::string(1, msg.bSIndicator)},
            {"Shares", msg.shares},
            {"Price", msg.price}
        }}
    };

    std::cout << j.dump(2) << '\n';
}

// ------------------------------------------------------------
// Add Order With MPID Attribution Message ('F')
// ------------------------------------------------------------

void printAddOrderMPIDMessage(const AddOrderMPIDMessage& msg)
{
    const json j = {
        {"Add Order With MPID Attribution Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Order Reference Number", msg.orderReferenceNumber},
            {"Buy/Sell Indicator", std::string(1, msg.bSIndicator)},
            {"Shares", msg.shares},
            {"Price", msg.price},
            {"Attribution", msg.attribution}
        }}
    };

    std::cout << j.dump(2) << '\n';
}

// ------------------------------------------------------------
// Order Executed Message ('E')
// ------------------------------------------------------------

void printOrderExecutedMessage(const OrderExecutedMessage& msg)
{
    json j = {
        {"Order Executed Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Order Reference Number", msg.orderReferenceNumber},
            {"Executed Shares", msg.executedShares},
            {"Match Number", msg.matchNumber}
        }}
    };

    std::cout << j.dump(4) << '\n';
}

// ------------------------------------------------------------
// Order Executed With Price Message ('C')
// ------------------------------------------------------------

void printOrderExecutedWithPriceMessage(const OrderExecutedWithPriceMessage& msg)
{
    json j = {
        {"Order Executed With Price Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Order Reference Number", msg.orderReferenceNumber},
            {"Executed Shares", msg.executedShares},
            {"Match Number", msg.matchNumber},
            {"Printable", std::string(1, msg.printable)},
            {"Execution Price", msg.executionPrice}
        }}
    };

    std::cout << j.dump(4) << '\n';
}

// ------------------------------------------------------------
// Order Cancel Message ('X')
// ------------------------------------------------------------

void printOrderCancelMessage(const OrderCancelMessage& msg)
{
    json j = {
        {"Order Cancel Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Order Reference Number", msg.orderReferenceNumber},
            {"Cancelled Shares", msg.cancelledShares}
        }}
    };

    std::cout << j.dump(4) << '\n';
}

// ------------------------------------------------------------
// Order Delete Message ('D')
// ------------------------------------------------------------

void printOrderDeleteMessage(const OrderDeleteMessage& msg)
{
    json j = {
        {"Order Delete Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Order Reference Number", msg.orderReferenceNumber}
        }}
    };

    std::cout << j.dump(4) << '\n';
}

// ------------------------------------------------------------
// Order Replace Message ('U')
// ------------------------------------------------------------

void printOrderReplaceMessage(const OrderReplaceMessage& msg)
{
    json j = {
        {"Order Replace Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Original Order Reference Number", msg.originalOrderReferenceNumber},
            {"New Order Reference Number", msg.newOrderReferenceNumber},
            {"Shares", msg.shares},
            {"Price", msg.price}
        }}
    };

    std::cout << j.dump(4) << '\n';
}

// ------------------------------------------------------------
// Trade Message ('P')
// ------------------------------------------------------------

void printTradeMessage(const TradeMessage& msg)
{
    json j = {
        {"Trade Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Order Reference Number", msg.orderReferenceNumber},
            {"Buy/Sell Indicator", std::string(1, msg.buySellIndicator)},
            {"Shares", msg.shares},
            {"Stock", msg.stock.displaySymbol()},
            {"Price", msg.price.displayPrice()},
            {"Match Number", msg.matchNumber}
        }}
    };

    std::cout << j.dump(4) << '\n';
}

// ------------------------------------------------------------
// Cross Trade Message ('Q')
// ------------------------------------------------------------

void printCrossTradeMessage(const CrossTradeMessage& msg)
{
    json j = {
        {"Cross Trade Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Shares", msg.shares},
            {"Cross Price", msg.crossPrice},
            {"Match Number", msg.matchNumber},
            {"Cross Type", std::string(1, msg.crossType)}
        }}
    };

    std::cout << j.dump(4) << '\n';
}

// ------------------------------------------------------------
// Broken Trade Message ('B')
// ------------------------------------------------------------

void printBrokenTradeMessage(const BrokenTradeMessage& msg)
{
    json j = {
        {"Broken Trade Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Match Number", msg.matchNumber}
        }}
    };

    std::cout << j.dump(4) << '\n';
}

// ------------------------------------------------------------
// Net Order Imbalance Indicator Message ('I')
// ------------------------------------------------------------

void printNOIIMessage(const NOIIMessage& msg)
{
    json j = {
        {"Net Order Imbalance Indicator Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Paired Shares", msg.pairedShares},
            {"Imbalance Shares", msg.imbalanceShares},
            {"Imbalance Direction", std::string(1, msg.imbalanceDirection)},
            {"Far Price", msg.farPrice},
            {"Near Price", msg.nearPrice},
            {"Current Reference Price", msg.currentReferencePrice},
            {"Cross Type", std::string(1, msg.crossType)},
            {"Price Variation Indicator", std::string(1, msg.priceVariationIndicator)}
        }}
    };

    std::cout << j.dump(4) << '\n';
}

// ------------------------------------------------------------
// Retail Price Improvement Indicator Message ('N')
// ------------------------------------------------------------

void printRetailPriceImprovementMessage(const RetailPriceImprovementMessage& msg)
{
    json j = {
        {"Retail Price Improvement Indicator Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Interest Flag", std::string(1, msg.interestFlag)}
        }}
    };

    std::cout << j.dump(4) << '\n';
}

// ------------------------------------------------------------
// Direct Listing With Capital Raise Message ('O')
// ------------------------------------------------------------

void printDirectListingCapitalRaiseMessage(const DirectListingCapitalRaiseMessage& msg)
{
    json j = {
        {"Direct Listing With Capital Raise Message", {
            {"Stock Locate", msg.header.stockLocate},
            {"Tracking Number", msg.header.trackingNumber},
            {"Timestamp", msg.header.timestamp},
            {"Open Eligibility Status", std::string(1, msg.openEligibilityStatus)},
            {"Minimum Allowable Price", msg.minimumAllowablePrice},
            {"Maximum Allowable Price", msg.maximumAllowablePrice},
            {"Near Execution Price", msg.nearExecutionPrice},
            {"Near Execution Time", msg.nearExecutionTime},
            {"Lower Price Range Collar", msg.lowerPriceRangeCollar},
            {"Upper Price Range Collar", msg.upperPriceRangeCollar}
        }}
    };

    std::cout << j.dump(4) << '\n';
}
*/