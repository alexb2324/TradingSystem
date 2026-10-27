#pragma once

#include <map>

#include "OrderbookLevel.h"
#include "../Usings.h"


class Orderbook     // Per Stock: L2 Price Levels
{
public:

    Orderbook() = default;
    Orderbook(const Orderbook& other) = delete;
    void operator=(const Orderbook& other) = delete;
    Orderbook(Orderbook&& other) = delete;
    void operator=(Orderbook&& other) = delete;
    ~Orderbook() = default;

    void addOrder(orderbook::Side side, orderbook::Price price, orderbook::Quantity quantity)
    {
        side == orderbook::Side::Buy ? addOrderToLevel(bids_, price, quantity) : addOrderToLevel(asks_, price, quantity);
    }

    void reduce(orderbook::Side side, orderbook::Price price, orderbook::Quantity quantity, bool orderRemoved)
    {
        side == orderbook::Side::Buy ? reduceLevel(bids_, price, quantity, orderRemoved) : reduceLevel(asks_, price, quantity, orderRemoved);
    }

    [[nodiscard]] const auto& bids() const { return bids_; }
    [[nodiscard]] const auto& asks() const { return asks_; }

    //[[nodiscard]] std::size_t size() const noexcept { return bids_.size() + asks_.size(); }

private:

    template <typename Map>
    void addOrderToLevel(Map& orderbookSide, orderbook::Price price, orderbook::Quantity quantity)
    {
        OrderbookLevel& level = orderbookSide[price];
        level.totalShares += quantity;
        ++level.orderCount;
    };

    template <typename Map>
    void reduceLevel(Map& orderbookSide, orderbook::Price price, orderbook::Quantity quantity, bool orderRemoved)
    {
        auto it = orderbookSide.find(price);
        if (it == orderbookSide.end()) { return; }

        it->second.totalShares -= quantity;
        if (orderRemoved) { --it->second.orderCount; } // Only reduce on fully filled orders
        if (it->second.orderCount == 0) { orderbookSide.erase(it); } // Remove empty level
    }

private:

    std::map<orderbook::Price, OrderbookLevel, std::greater<>> bids_;
    std::map<orderbook::Price, OrderbookLevel, std::less<>> asks_;
};