#include "MarketState.h"

void MarketState::onStockDirectory(const StockDirectoryMessage& message)
{
    stockDirectory_.insert(message.stock, message.header.stockLocate);
}

void MarketState::onAddOrder(AddOrderMessage message)
{}
void MarketState::onAddOrderMPID(AddOrderMPIDMessage message)
{}
void MarketState::onOrderExecuted(OrderExecutedMessage message)
{}
void MarketState::onOrderExecutedWithPrice(OrderExecutedWithPriceMessage message)
{}
void MarketState::onOrderCancelled(OrderCancelMessage message)
{}
void MarketState::onTrade(const TradeMessage& message)
{
    tradeBook_.addTrade(message);
}

void MarketState::onCrossTrade(CrossTradeMessage message)
{

}
void MarketState::onBrokenTrade(BrokenTradeMessage message)
{}