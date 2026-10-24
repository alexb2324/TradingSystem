#include "Trade.h"


Trade::Trade(const OrderExecutedMessage& message, const Order* order) :
    timestamp { message.header.timestamp },
    matchNumber { message.matchNumber },
    shares { message.executedShares },
    symbol { "AAA" },
    price { order->price },
    stockLocate { message.header.stockLocate },
    tradeType { TradeType::OrderExecuted },
    side { 'B' },
    crossType {}
{ }

Trade::Trade(const OrderExecutedWithPriceMessage& message, const Order* order) :
    timestamp { message.header.timestamp },
    matchNumber { message.matchNumber },
    shares { message.executedShares },
    symbol { "AAA" },
    price { message.executionPrice },
    stockLocate { message.header.stockLocate },
    tradeType { TradeType::OrderExecutedWithPrice },
    side { 'B' },
    crossType {}
{ }

Trade::Trade(const TradeMessage& message) :
    timestamp { message.header.timestamp },
    matchNumber { message.matchNumber },
    shares { message.shares },
    symbol { message.stock },
    price { message.price },
    stockLocate { message.header.stockLocate },
    tradeType { TradeType::Trade },
    side { message.buySellIndicator },
    crossType {}
{ }

Trade::Trade(const CrossTradeMessage& message) :
    timestamp { message.header.timestamp },
    matchNumber { message.matchNumber },
    shares { message.shares },
    symbol { message.stock },
    price { message.crossPrice },
    stockLocate { message.header.stockLocate },
    tradeType { TradeType::CrossTrade },
    side {},
    crossType{ message.crossType }
{ }