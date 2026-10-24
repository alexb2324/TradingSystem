#include "MarketState.h"

void MarketState::onStockDirectory(const StockDirectoryMessage& message)
{
    stockDirectory_.insert(message.stock, message.header.stockLocate);
}
void MarketState::onAddOrder(const AddOrderMessage& message)
{}
void MarketState::onAddOrderMPID(const AddOrderMPIDMessage& message)
{}
void MarketState::onOrderExecuted(const OrderExecutedMessage& message)
{}
void MarketState::onOrderExecutedWithPrice(const OrderExecutedWithPriceMessage& message)
{}
void MarketState::onOrderCancelled(const OrderCancelMessage& message)
{}
void MarketState::onTrade(const TradeMessage& message)
{
    tradeBook_.addTrade(message);
    tradeBook_.printTrade(tradeBook_.lastTrade());
}

void MarketState::onCrossTrade(const CrossTradeMessage& message)
{
    tradeBook_.addTrade(message);
    tradeBook_.printTrade(tradeBook_.lastTrade());
}
void MarketState::onBrokenTrade(const BrokenTradeMessage& message)
{}