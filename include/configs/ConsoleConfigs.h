#pragma once
#include <chrono>

namespace console::config
{
	inline constexpr auto kPublishToConsoleInterval = std::chrono::milliseconds(50);
}