#pragma once
#include <cstdint>

namespace market
{
	using Price = std::uint32_t;
	using Quantity = std::uint32_t;
	using OrderId = std::uint64_t;

	enum class Side : char
	{
		Buy = 'B',
		Sell = 'S'
	};
}
