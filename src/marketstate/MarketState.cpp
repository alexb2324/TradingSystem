#include "MarketState.h"
#include "../tradebook/Trade.h"
#include <print>


// Message type S
void MarketState::onSystemEvent(const SystemEventMessage& message)
{
    std::println("System Event");
}

// Message type R
void MarketState::onStockDirectory(const StockDirectoryMessage& message)
{
    stockDirectory_.insert(message.stock, message.header.stockLocate);
}

// Message type H
void onStockTradingAction(const StockTradingActionMessage& message)
{
    std::println("Stock Trading Action");
}

// Message type Y
void onRegSHORestriction(const RegSHORestrictionMessage& message)
{
    std::println("RegSHORestriction");
}

// Message type L
void onMarketParticipantPosition(const MarketParticipantPositionMessage& message)
{
    std::println("Market Participant Position");
}

// Message type V
void onMWCBDeclineLevel(const MWCBDeclineLevelMessage& message)
{
    std::println("MWCB Decline Level");
}

// Message type W
void onMWCBStatus(const MWCBStatusMessage& message)
{
    std::println("MWCB Status");
}

// Message type K
void onIPOQuotingPeriodUpdate(const IPOQuotingPeriodUpdateMessage& message)
{
    std::println("IPOQuotingPeriodUpdate");
}

// Message type J
void onLULDAuctionCollar(const LULDAuctionCollarMessage& message)
{
    std::println("LULDAuction Collar");
}

// Message type h
void onOperationalHalt(const OperationalHaltMessage& message)
{
    std::println("Operational halt");
}

// Message type A
void MarketState::onAddOrder(const AddOrderMessage& message)
{
    market_.add(message.orderReferenceNumber,
        message.price,
        message.shares,
        static_cast<orderbook::Side>(message.bSIndicator),
        message.header.stockLocate);
}

// Message type F
void MarketState::onAddOrderMPID(const AddOrderMPIDMessage& message)
{
    market_.add(message.orderReferenceNumber,
        message.price,
        message.shares,
        static_cast<orderbook::Side>(message.bSIndicator),
        message.header.stockLocate);
}

// Message type E
void MarketState::onOrderExecuted(const OrderExecutedMessage& message)
{
    const auto order = market_.reduce(message.orderReferenceNumber, message.executedShares);
    tradeBook_.recordTrade( Trade {
        .timestamp = message.header.timestamp,
        .matchNumber = message.matchNumber,
        .shares = message.executedShares,
        .price = order.price,
        .stockLocate = message.header.stockLocate,
        .tradeType = TradeType::OrderExecuted,
        .side = static_cast<char>(order.side)
    });
}

// Message type C
void MarketState::onOrderExecutedWithPrice(const OrderExecutedWithPriceMessage& message)
{
    const auto order = market_.reduce(message.orderReferenceNumber, message.executedShares);

    if (message.printable != 'Y') { return; }

    tradeBook_.recordTrade( Trade {
        .timestamp = message.header.timestamp,
        .matchNumber = message.matchNumber,
        .shares = message.executedShares,
        .price = message.executionPrice,
        .stockLocate = message.header.stockLocate,
        .tradeType = TradeType::OrderExecutedWithPrice,
        .side = static_cast<char>(order.side)
    });
}

// Message type X
void MarketState::onOrderCancelled(const OrderCancelMessage& message)
{
    market_.reduce(message.orderReferenceNumber, message.cancelledShares);
}

// Message type X
void MarketState::onOrderDelete(const OrderDeleteMessage& message)
{
    market_.remove(message.orderReferenceNumber);
}

// Message type X
void MarketState::onOrderReplace(const OrderReplaceMessage& message)
{
    market_.replace(message.originalOrderReferenceNumber,
        message.newOrderReferenceNumber,
        message.price,
        message.shares);
}

// Message type P
void MarketState::onTrade(const TradeMessage& message)
{
    tradeBook_.recordTrade( Trade {
        .timestamp = message.header.timestamp,
        .matchNumber = message.matchNumber,
        .shares = message.shares,
        .price = message.price,
        .stockLocate = message.header.stockLocate,
        .tradeType = TradeType::OrderExecutedWithPrice,
        .side = static_cast<char>(message.buySellIndicator)
    });
    //tradeBook_.printTrade(tradeBook_.lastTrade());
}

// Message type Q
void MarketState::onCrossTrade(const CrossTradeMessage& message)
{
    tradeBook_.recordTrade( Trade {
        .timestamp = message.header.timestamp,
        .matchNumber = message.matchNumber,
        .shares = message.shares,
        .price = message.crossPrice,
        .stockLocate = message.header.stockLocate,
        .tradeType = TradeType::OrderExecutedWithPrice,
        .crossType = message.crossType
    });
    tradeBook_.printTrade(tradeBook_.lastTrade());
}

// Message type B
void MarketState::onBrokenTrade(const BrokenTradeMessage& message)
{
    if (Trade* trade = tradeBook_.findTrade(message.matchNumber))
    {
        trade->broken = true;
    }
}

// Message type I
void onNOII(const NOIIMessage& message)
{
    std::println("NOII");
}

// Message type N
void onRetailPriceImprovement(const RetailPriceImprovementMessage& message)
{
    std::println("RetailPriceImprovement");
}

// Message type O
void onDirectListingCapitalRaise(const DirectListingCapitalRaiseMessage& message)
{
    std::println("DirectListingCapitalRaise");
}