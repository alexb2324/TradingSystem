#pragma once

#include <limits>
#include <cstdint>
#include <cstddef>

struct Constants
{
    static constexpr std::size_t kMaxMessageLength = 64;
    static constexpr std::uint16_t kMaxStockDirectorySize = std::numeric_limits<std::uint16_t>::max();
    static constexpr std::uint32_t kInvalidPrice = std::numeric_limits<std::uint32_t>::max();
    static constexpr std::uint64_t kReserveDailyTradeCount = 1'000'000; // Testing only
    static constexpr std::size_t kReserveDailyOrderCount = 10'000'000; // Testing only
    static constexpr std::uint16_t kItchMessageOffset = 2;
    static constexpr std::uint32_t kPacketHeaderLength = 20;

    static constexpr std::uint16_t kHeartBeatMessageCount = 0xFFFF;
    static constexpr std::size_t kMoldSessionIdSize = 10;
    static constexpr std::size_t kPacketBufferSize = 1'500;
    static constexpr std::size_t kMaxPacketSize = 1'400;
};
