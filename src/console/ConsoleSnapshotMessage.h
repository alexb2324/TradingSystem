#pragma once

#include <array>

#include "../tradebook/Trade.h"

struct SnapshotLevel
{
    std::uint64_t totalShares {};
    orderbook::Price price {};
    std::uint32_t orderCount {};
};

struct ConsoleSnapshotMessage
{
    std::array<SnapshotLevel, 10> asks {};
    std::array<SnapshotLevel, 10> bids {};
    std::array<Trade, 10> recentTrades {};
    std::uint64_t   messages {};
    std::uint64_t   liveOrders {};
    itch::Timestamp exchangeTime {};   // ns since midnight
    double          wallSeconds {};
    std::uint8_t askCount {};
    std::uint8_t bidCount {};
    std::uint8_t tradeCount {};
    bool symbolFound {};
};


