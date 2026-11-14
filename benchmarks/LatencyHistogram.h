#pragma once

#include <array>
#include <cstdint>

class LatencyHistogram {

    static constexpr int kSubBits = 7;
    static constexpr std::size_t kSub = 1u << kSubBits;
    static constexpr std::size_t kBuckets = (64 - kSubBits + 1) * kSub;

public:
    void record(std::uint64_t cycles) noexcept
    {
        ++cycleBuckets_[index(cycles)];
        ++totalSamples_;
        maxCyclesObserved_ = std::max(maxCyclesObserved_, cycles);
    }

    [[nodiscard]] std::uint64_t cyclesByPercentile(double percentile) const noexcept
    {
        const auto targetSamples = static_cast<std::uint64_t>(percentile / 100.0 * static_cast<double>(totalSamples_));
        std::uint64_t seen = 0;
        for (std::size_t i {}; i < kBuckets; ++i)
        {
            if ((seen += cycleBuckets_[i]) > targetSamples) { return lowerBound(i); }
        }
        return maxCyclesObserved_;
    }

    [[nodiscard]] std::uint64_t totalSamples() const noexcept { return totalSamples_; }
    [[nodiscard]] std::uint64_t maxSamples() const noexcept { return maxCyclesObserved_; }

private:

    static std::size_t index(std::uint64_t value) noexcept
    {
        if (value < kSub) { return value; }
        const int shift = std::bit_width(value) - 1 - kSubBits;
        return ((shift + 1) << kSubBits) + ((value >> shift) & (kSub - 1));
    }

    static std::uint64_t lowerBound(std::size_t index) noexcept
    {
        if (index < kSub) { return index; }
        const int shift = static_cast<int>(index >> kSubBits) - 1;
        return (kSub + (index & (kSub - 1))) << shift;
    }

    std::array<std::uint64_t, kBuckets> cycleBuckets_ {};
    std::uint64_t totalSamples_{}, maxCyclesObserved_ {};
};


