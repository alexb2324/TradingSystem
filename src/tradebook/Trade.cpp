#include "Trade.h"


Trade::Trade(const OrderExecutedMessage& message, const Order* order) :
    timestamp { message.header.timestamp },
    matchNumber { message.matchNumber },
    price { order->getPrice() },
    shares { message.executedShares },
    stockLocate { message.header.stockLocate },
    symbol { "AAA" },
    tradeType { TradeType::OrderExecuted },
    crossType {},
    side { 'B' }
{ }

Trade::Trade(const OrderExecutedWithPriceMessage& message, const Order* order) :
    timestamp { message.header.timestamp },
    matchNumber { message.matchNumber },
    price { order->getPrice() },
    shares { message.executedShares },
    stockLocate { message.header.stockLocate },
    symbol { "AAA" },
    tradeType { TradeType::OrderExecutedWithPrice },
    crossType {},
    side { 'B' }
{ }

Trade::Trade(const TradeMessage& message) :
    timestamp { message.header.timestamp },
    matchNumber { message.matchNumber },
    price { message.price },
    shares { message.shares },
    stockLocate { message.header.stockLocate },
    symbol { message.stock },
    tradeType { TradeType::Trade },
    crossType {},
    side { message.buySellIndicator }
{ }

Trade::Trade(const CrossTradeMessage& message) :
    timestamp { message.header.timestamp },
    matchNumber { message.matchNumber },
    price { message.crossPrice },
    shares { message.shares },
    stockLocate { message.header.stockLocate },
    symbol { message.stock },
    tradeType { TradeType::CrossTrade },
    crossType{ message.crossType },
    side {}
{ }