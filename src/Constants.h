#pragma once

#include <limits>
#include <cstdint>
#include <cstddef>
#include <chrono>

struct Constants
{
    static constexpr std::size_t kMaxMessageLength = 64;
    static constexpr std::uint16_t kMaxStockDirectorySize = std::numeric_limits<std::uint16_t>::max();
    static constexpr std::uint32_t kInvalidPrice = std::numeric_limits<std::uint32_t>::max();
    static constexpr std::uint64_t kReserveDailyTradeCount = 1'000'000; // Testing only
    static constexpr std::uint16_t kItchMessageOffset = 2;
    static constexpr std::size_t kSnapshotDepth = 10;
    static constexpr std::size_t kTradeDepth = 10;
    static constexpr auto kSnapshotIntervalTime = std::chrono::milliseconds(100);
};
