#pragma once

#include <vector>
#include <algorithm>
#include <ranges>

#include "types/MarketTypes.h"


struct OrderbookLevel
{
    std::uint64_t totalShares;
    std::uint32_t price;
    std::uint32_t orderCount;
};

class VectorOrderbook
{
public:

    using BookLevels = std::vector<OrderbookLevel>;

    VectorOrderbook()
    {
        bids_.reserve(1'000);
        asks_.reserve(1'000);
    }

    VectorOrderbook(const VectorOrderbook& other) = delete;
    VectorOrderbook& operator=(const VectorOrderbook& other) = delete;
    VectorOrderbook(VectorOrderbook&& other) = delete;
    VectorOrderbook& operator=(VectorOrderbook&& other) = delete;

    ~VectorOrderbook() = default;

    void addOrder(market::Side side, market::Price price, market::Quantity quantity)
    {
        side == market::Side::Buy ? addOrderToBids(price, quantity) : addOrderToAsks(price, quantity);
    }

    void reduce(market::Side side, market::Price price, market::Quantity quantity, bool orderRemoved)
    {
        side == market::Side::Buy ? reduceLevelFromBids(price, quantity, orderRemoved) : reduceLevelFromAsks(price, quantity, orderRemoved);
    }

    [[nodiscard]] const BookLevels& bids() const noexcept { return bids_; }
    [[nodiscard]] const BookLevels& asks() const noexcept { return asks_; }

    [[nodiscard]] auto bestBids() const noexcept { return bids_ | std::views::reverse; }
    [[nodiscard]] auto bestAsks() const noexcept { return asks_ | std::views::reverse; }


private:

    void addOrderToBids(market::Price price, market::Quantity quantity)
    {   
        if (bids_.empty() || price > bids_.rbegin()->price)
        {
            bids_.emplace_back(quantity, price, 1);
            return;
        }

        for (auto it = bids_.rbegin(); it != bids_.rend(); ++it)
        {

            if (price == it->price)
            {
                it->totalShares += quantity;
                ++(it->orderCount);
                return;
            }
            else if (price > it->price)
            {
                bids_.insert(it.base(), OrderbookLevel {quantity, price, 1});
                return;
            } 
        }

        bids_.insert(bids_.begin(), OrderbookLevel {quantity, price, 1});
    };

    void addOrderToAsks(market::Price price, market::Quantity quantity)
    {
        if (asks_.empty() || price < asks_.rbegin()->price)
        {
            asks_.emplace_back(quantity, price, 1);
            return;
        }

        for (auto it = asks_.rbegin(); it != asks_.rend(); ++it)
        {

            if (price == it->price)
            {
                it->totalShares += quantity;
                ++(it->orderCount);
                return;
            }
            else if (price < it->price)
            {
                asks_.insert(it.base(), OrderbookLevel {quantity, price, 1});
                return;
            } 
        }

        asks_.insert(asks_.begin(), OrderbookLevel {quantity, price, 1});
    };

    void reduceLevelFromBids(market::Price price, market::Quantity quantity, bool orderRemoved)
    {
        auto it = std::find_if(bids_.rbegin(), bids_.rend(), [price](const OrderbookLevel& level)
        {
            return level.price == price;
        });

        if (it == bids_.rend()) { return; }

        it->totalShares -= quantity;
        if (orderRemoved) { --it->orderCount; } // Only reduce on fully filled orders
        if (it->orderCount == 0) { bids_.erase(std::prev(it.base())); } // Remove empty level
    }

    void reduceLevelFromAsks(market::Price price, market::Quantity quantity, bool orderRemoved)
    {
        auto it = std::find_if(asks_.rbegin(), asks_.rend(), [price](const OrderbookLevel& level)
        {
            return level.price == price;
        });

        if (it == asks_.rend()) { return; }

        it->totalShares -= quantity;
        if (orderRemoved) { --it->orderCount; } // Only reduce on fully filled orders
        if (it->orderCount == 0) { asks_.erase(std::prev(it.base())); } // Remove empty level
    }

private:
    BookLevels bids_;
    BookLevels asks_;
};