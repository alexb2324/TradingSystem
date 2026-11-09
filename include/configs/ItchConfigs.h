#pragma once
#include <cstddef>
#include <cstdint>
#include <unordered_map>

namespace itch::configs
{
	inline constexpr std::uint16_t	kItchMessageOffset = 2;
	inline constexpr std::size_t kItchMaxMessageLength = 64;
	static std::unordered_map<char, int> kItchMessageLengthsMap
	{
	        {'S', 12}, {'R', 39}, {'H', 25}, {'Y', 20},
			{'L', 26}, {'V', 35}, {'W', 12}, {'K', 28},
			{'J', 35}, {'h', 21}, {'A', 36}, {'F', 40},
			{'E', 31}, {'C', 36}, {'X', 23}, {'D', 19},
			{'U', 35}, {'P', 44}, {'Q', 40}, {'B', 19},
			{'I', 50}, {'N', 20}, {'O', 48}
	};
}
