#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>

namespace market::configs
{
	static constexpr std::uint16_t kStockDirectoryMaxSize = std::numeric_limits<std::uint16_t>::max();
	static constexpr std::uint32_t kInvalidPrice = std::numeric_limits<std::uint32_t>::max();
	static constexpr std::uint64_t kTradeBookReserveDailyTradeCount = 10'000'000; // Testing only
	static constexpr std::size_t kOrderBookReserveDailyOrderCount = 10'000'000; // Testing only
}
