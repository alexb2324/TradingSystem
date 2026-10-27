#pragma once
#include <cstdint>

struct OrderbookLevel // L2 Orderbook Level Information
{
    std::uint64_t totalShares {};
    std::uint32_t orderCount {};
};
