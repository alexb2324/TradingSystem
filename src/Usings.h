#pragma once

#include <array>
#include <cstdint>

struct Symbol
{
    std::array<char, 8> symbolPadded;

    [[nodiscard]] std::string_view displaySymbol() const
    {
        std::string_view symbol {symbolPadded.data(), symbolPadded.size()};
        return symbol.substr(0, symbol.find_last_not_of(' ') + 1);
    };
};

namespace orderbook
{
    using Price = std::uint64_t;
    using Quantity = std::uint64_t;
    using OrderId = std::uint64_t;

    enum class Side
    {
        Buy,
        Sell
    };
}

namespace tradebook
{
    using Price = std::uint64_t;
    using Quantity = std::uint64_t;
    using OrderId = std::uint64_t;
    using TradeId = std::uint64_t;
    using Timestamp = std::uint64_t;

    enum class Side
    {
        Buy,
        Sell
    };
}


namespace itch 
{
    using Timestamp = std::uint64_t;
    using StockLocate = std::uint16_t;
    using TrackingNumber = std::uint16_t;
    using String16 = std::array<char, 2>;
    using String32 = std::array<char, 4>;
    using String64 = std::array<char, 8>;
}