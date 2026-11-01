#pragma once

#include <cstdint>
#include <optional>
#include <string>

[[nodiscard]] inline std::optional<std::uint64_t> parseTimeOfDay(const std::string& timeText) noexcept
{
	auto field = [&timeText](const std::uint8_t pos, const std::uint8_t max) -> std::optional<std::size_t>
	{
		auto value = std::stoi(timeText.substr(pos, 2));
		if (value > max) { return std::nullopt; }
		return value;
	};


	if (timeText.size() != 5 && timeText[2] != ':') { return std::nullopt; }

	const auto hours = field(0, 23);
	const auto mins = field(3, 59);

	if (!hours || !mins) { return std::nullopt; }

	return (*hours * 3'600 + *mins * 60) * 1'000'000'000ull;
}
