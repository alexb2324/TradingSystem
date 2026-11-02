#pragma once

#include <chrono>
#include <cstdint>

using Timepoint = std::chrono::time_point<std::chrono::steady_clock>;

class Pacer
{
public:

	explicit Pacer(std::uint64_t startTime)
	:	startAt_ { startTime }
	{ }

	// Returns timepoint when simulator thread should wake and process the next message
	[[nodiscard]] Timepoint dueAt(const std::uint64_t nextTimestamp) const
	{
		const std::chrono::nanoseconds nextTimestampDur { nextTimestamp };
		const std::chrono::nanoseconds exchangeStartDur { exchangeStartTime_ };
		return wallStartTime_ + nextTimestampDur - exchangeStartDur;
	}

	[[nodiscard]] bool isDue(const std::uint64_t timestamp, const Timepoint now) noexcept
	{
		if (timestamp < startAt_) { return true; } // Fast-forward to startAt_ time

		if (!startPacer_) [[unlikely]]
		{
			exchangeStartTime_ = timestamp;
			wallStartTime_ = now;
			startPacer_ = true;
			return true;
		}

		return dueAt(timestamp) <= now;
	}

private:
	std::uint64_t startAt_ {};
	std::uint64_t exchangeStartTime_ {};
	Timepoint wallStartTime_;
	bool startPacer_ { false };
};
