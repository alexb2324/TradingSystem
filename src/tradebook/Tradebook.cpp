#include <print>
#include <utility>

#include "Tradebook.h"


void TradeBook::displayLastTrade() const
{
    const auto& trade = trades_.back();

    std::println("TradeMessage - TradeId: {}, Timestamp: {}, Stock: {}, Price: {}, Quantity: {}",
        trade.matchNumber,
        trade.header.timestamp,
        trade.stock.displaySymbol(),
        trade.price.priceOriginal,
        trade.shares
    );
}

void TradeBook::displayLastCrossTrade() const
{
    const auto& trade = crossTrades_.back();

    std::println("CrossTradeMessage - TradeId: {}, Timestamp: {}, Stock: {}, CrossPrice: {}, Quantity: {}, CrossType: {}",
        trade.matchNumber,
        trade.header.timestamp,
        trade.stock.displaySymbol(),
        trade.crossPrice.priceOriginal,
        trade.shares,
        trade.crossType
    );
}

void TradeBook::displayLastBrokenTrade() const
{
    const auto& trade = brokenTrades_.back();

    std::println("BrokenTradeMessage - TradeId: {}, Timestamp: {}",
        trade.matchNumber,
        trade.header.timestamp
    );
}

void TradeBook::displayLastExecutedOrder() const
{
    const auto& trade = executedOrders_.back();

    std::println("OrderExecutedMessage - TradeId: {}, Timestamp: {}, Stock: {}, Quantity: {}, OrderReference: {}",
        trade.matchNumber,
        trade.header.timestamp,
        trade.header.stockLocate,
        trade.executedShares,
        trade.orderReferenceNumber
    );
}

void TradeBook::displayLastExecutedPriceOrder() const
{
    const auto& trade = executedPriceOrders_.back();

    std::println("OrderExecutedMessage - TradeId: {}, Timestamp: {}, Stock: {}, Price: {}, Quantity: {}, OrderReference: {}",
        trade.matchNumber,
        trade.header.timestamp,
        trade.header.stockLocate,
        trade.executionPrice.priceOriginal,
        trade.executedShares,
        trade.orderReferenceNumber
    );
}

void TradeBook::addTrade(const TradeMessage& trade)
{
    trades_.push_back(trade);
}

void TradeBook::addCrossTrade(const CrossTradeMessage& trade)
{
    crossTrades_.push_back(trade);
}

void TradeBook::addBrokenTrade(const BrokenTradeMessage& trade)
{
    brokenTrades_.push_back(trade);
}

void TradeBook::addExecutedOrder(const OrderExecutedMessage& trade)
{
    executedOrders_.push_back(trade);
}

void TradeBook::addExecutedPriceOrder(const OrderExecutedWithPriceMessage& trade)
{
    executedPriceOrders_.push_back(trade);
}