#pragma once

#include <chrono>

class Pacer
{
public:

	Pacer() = default;

	void startPacer(const std::uint64_t exchangeStartTime)
	{
		exchangeStartTime_ = exchangeStartTime;
		wallStartTime_ = std::chrono::steady_clock::now();
		running_ = true;
	}

	[[nodiscard]] auto dueAt(const std::uint64_t nextTimestamp) const
	{
		const std::chrono::nanoseconds nextTimestampDur(nextTimestamp);
		const std::chrono::nanoseconds exchangeStartDur(exchangeStartTime_);
		return wallStartTime_ + nextTimestampDur - exchangeStartDur;
	}

	[[nodiscard]] bool running() const noexcept { return running_; }

private:
	std::uint64_t exchangeStartTime_;
	std::chrono::time_point<std::chrono::steady_clock> wallStartTime_;
	bool running_ { false };
};
