#pragma once

#include <vector>
#include <cstddef>

#include "../parser/Messages.h"

class TradeBook 
{

public:

    TradeBook() = default;
    TradeBook(const TradeBook& other) = delete;
    TradeBook& operator=(const TradeBook& other) = delete;
    TradeBook(TradeBook&& other) = delete;
    TradeBook& operator=(TradeBook&& other) = delete;
    ~TradeBook() = default;

    //void displayAllTrades() const;
    void displayLastTrade() const;
    void displayLastCrossTrade() const;
    void displayLastBrokenTrade() const;
    void displayLastExecutedOrder() const;
    void displayLastExecutedPriceOrder() const;


    [[nodiscard]] std::size_t size() const noexcept { return trades_.size() + crossTrades_.size() + executedOrders_.size() + executedPriceOrders_.size() + brokenTrades_.size(); }

    void addTrade(const TradeMessage& trade);
    void addCrossTrade(const CrossTradeMessage& trade);
    void addBrokenTrade(const BrokenTradeMessage& trade);
    void addExecutedOrder(const OrderExecutedMessage& trade);
    void addExecutedPriceOrder(const OrderExecutedWithPriceMessage& trade);

private:
    std::vector<TradeMessage> trades_ {};
    std::vector<CrossTradeMessage> crossTrades_ {};
    std::vector<OrderExecutedMessage> executedOrders_ {};
    std::vector<OrderExecutedWithPriceMessage> executedPriceOrders_ {};
    std::vector<BrokenTradeMessage> brokenTrades_ {};
};