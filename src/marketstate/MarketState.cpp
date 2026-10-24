#include "MarketState.h"
#include "../tradebook/Trade.h"

void MarketState::onStockDirectory(const StockDirectoryMessage& message)
{
    stockDirectory_.insert(message.stock, message.header.stockLocate);
}

void MarketState::onAddOrder(const AddOrderMessage& message)
{
    market_.add(message.orderReferenceNumber,
        message.price,
        message.shares,
        static_cast<orderbook::Side>(message.bSIndicator),
        message.header.stockLocate);
}

void MarketState::onAddOrderMPID(const AddOrderMPIDMessage& message)
{
    market_.add(message.orderReferenceNumber,
        message.price,
        message.shares,
        static_cast<orderbook::Side>(message.bSIndicator),
        message.header.stockLocate);
}

void MarketState::onOrderExecuted(const OrderExecutedMessage& message)
{
    auto order = market_.reduce(message.orderReferenceNumber, message.executedShares);
    tradeBook_.recordTrade(message.header.timestamp,
        message.matchNumber,
        message.executedShares,
        order.price,
        message.header.stockLocate,
        TradeType::OrderExecuted,
        order.side,
        ' ');
}

void MarketState::onOrderExecutedWithPrice(const OrderExecutedWithPriceMessage& message)
{
    auto order = market_.reduce(message.orderReferenceNumber, message.executedShares);
    tradeBook_.recordTrade(message.header.timestamp,
        message.matchNumber,
        message.executedShares,
        order.price,
        message.header.stockLocate,
        TradeType::OrderExecutedWithPrice,
        order.side,
        ' ');
}

void MarketState::onOrderCancelled(const OrderCancelMessage& message)
{
    market_.reduce(message.orderReferenceNumber, message.cancelledShares);
}

void MarketState::onTrade(const TradeMessage& message)
{
    tradeBook_.recordTrade(message.header.timestamp,
        message.matchNumber,
        message.shares,
        message.price,
        message.header.stockLocate,
        TradeType::Trade,
        'B',
        ' ');
    tradeBook_.printTrade(tradeBook_.lastTrade());
}

void MarketState::onCrossTrade(const CrossTradeMessage& message)
{
    tradeBook_.recordTrade(message.header.timestamp,
        message.matchNumber,
        message.shares,
        message.crossPrice,
        message.header.stockLocate,
        TradeType::Trade,
        'B',
        ' ');
    tradeBook_.printTrade(tradeBook_.lastTrade());
}

void MarketState::onBrokenTrade(const BrokenTradeMessage& message)
{

}