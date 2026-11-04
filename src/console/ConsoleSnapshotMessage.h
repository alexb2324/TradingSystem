#pragma once

#include <array>

#include "../orderbook/OrderbookLevel.h"
#include "../tradebook/Trade.h"

struct ConsoleSnapshotMessage
{
    std::array<OrderbookLevel, 10> asks {};
    std::array<OrderbookLevel, 10> bids {};
    std::array<Trade, 10> recentTrades {};
    std::array<char, 8> symbol {};
    std::uint64_t   messages {};
    std::uint64_t   liveOrders {};
    itch::Timestamp exchangeTime {};   // ns since midnight
    double          wallSeconds {};
    std::uint8_t askCount {};
    std::uint8_t bidCount {};
    std::uint8_t tradeCount {};
};


