#pragma once

#include <limits>
#include <cstdint>
#include <cstddef>

struct Constants
{
    static constexpr std::size_t kMaxMessageLength = 64;
    static constexpr std::uint16_t kMaxStockDirectorySize = std::numeric_limits<std::uint16_t>::max();
    static constexpr std::uint32_t InvalidPrice = std::numeric_limits<std::uint32_t>::max();
};
