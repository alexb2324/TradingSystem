#pragma once
#include <cstddef>

namespace network::configs
{
	inline constexpr int kReceiverSocketBufferSize = 8 * 1024 * 1024; // 8 MB socket buffer
	inline constexpr std::size_t	kPacketBufferSize = 1'500;
	inline constexpr std::size_t	kMaxPacketSize = 1'400;
}
