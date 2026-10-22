#include "MarketState.h"

void MarketState::onStockDirectory(StockDirectoryMessage message)
{};
void MarketState::onAddOrder(AddOrderMessage message)
{};
void MarketState::onAddOrderMPID(AddOrderMPIDMessage message)
{};
void MarketState::onOrderExecuted(OrderExecutedMessage message)
{};
void MarketState::onOrderExecutedWithPrice(OrderExecutedWithPriceMessage message)
{};
void MarketState::onOrderCancelled(OrderCancelMessage message)
{};
void MarketState::onTrade(TradeMessage message)
{};
void MarketState::onCrossTrade(CrossTradeMessage message)
{};
void MarketState::onBrokenTrade(BrokenTradeMessage message)
{};