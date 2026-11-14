#pragma once

#include <map>
#include <functional>
#include <cstdint>


#include "types/MarketTypes.h"


struct OrderbookLevel // L2 Orderbook Level Information
{
    std::uint64_t totalShares {};
    std::uint32_t orderCount {};
};

class MapOrderbook     // Per Stock: L2 Price Levels
{
public:

    using BidLevels = std::map<market::Price, OrderbookLevel, std::greater<>>;
    using AskLevels = std::map<market::Price, OrderbookLevel, std::less<>>;

    MapOrderbook() = default;

    MapOrderbook(const MapOrderbook& other) = delete;
    MapOrderbook& operator=(const MapOrderbook& other) = delete;
    MapOrderbook(MapOrderbook&& other) = delete;
    MapOrderbook& operator=(MapOrderbook&& other) = delete;

    ~MapOrderbook() = default;

    void addOrder(market::Side side, market::Price price, market::Quantity quantity)
    {
        side == market::Side::Buy ? addOrderToLevel(bids_, price, quantity) : addOrderToLevel(asks_, price, quantity);
    }

    void reduce(market::Side side, market::Price price, market::Quantity quantity, bool orderRemoved)
    {
        side == market::Side::Buy ? reduceLevel(bids_, price, quantity, orderRemoved) : reduceLevel(asks_, price, quantity, orderRemoved);
    }

    [[nodiscard]] const BidLevels& bids() const noexcept { return bids_; }
    [[nodiscard]] const AskLevels& asks() const noexcept { return asks_; }

private:

    template <typename Map>
    void addOrderToLevel(Map& orderbookSide, market::Price price, market::Quantity quantity)
    {
        OrderbookLevel& level = orderbookSide[price];
        level.totalShares += quantity;
        ++level.orderCount;
    };

    template <typename Map>
    void reduceLevel(Map& orderbookSide, market::Price price, market::Quantity quantity, bool orderRemoved)
    {
        auto it = orderbookSide.find(price);
        if (it == orderbookSide.end()) { return; }

        it->second.totalShares -= quantity;
        if (orderRemoved) { --it->second.orderCount; } // Only reduce on fully filled orders
        if (it->second.orderCount == 0) { orderbookSide.erase(it); } // Remove empty level
    }

private:
    BidLevels bids_;
    AskLevels asks_;
};
