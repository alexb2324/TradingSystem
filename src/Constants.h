#pragma once

#include <limits>
#include <cstdint>
#include <cstddef>
#include <unordered_map>

namespace Constants
{
    static std::unordered_map<char, int> kItchMessageLengthsMap
    {
    {'S', 12}, {'R', 39}, {'H', 25},{'Y', 20},
    {'L', 26}, {'V',35},  {'W', 12},{'K', 28},
    {'J', 35}, {'h', 21}, {'A', 36},{'F', 40},
    {'E', 31}, {'C', 36}, {'X', 23},{'D', 19},
    {'U', 35}, {'P', 44}, {'Q', 40},{'B', 19},
    {'I', 50}, {'N', 20}, {'O', 48}
    };
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
