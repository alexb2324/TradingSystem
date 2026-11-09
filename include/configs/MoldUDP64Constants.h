#pragma once
#include <cstddef>
#include <cstdint>

namespace moldudp64::constants
{
	inline constexpr std::uint32_t	kPacketHeaderLength = 20;
	inline constexpr std::uint16_t	kEOSMessageCount = 0xFFFF;
	inline constexpr std::size_t	kMoldSessionIdSize = 10;
}
