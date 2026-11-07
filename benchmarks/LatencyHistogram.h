#pragma once

#include <array>
#include <cstdint>

class LatencyHistogram {

    static constexpr int kSubBits = 7;
    static constexpr std::size_t kSub = 1u << kSubBits;
    static constexpr std::size_t kBuckets = (64 - kSubBits + 1) * kSub;

public:
    void record(std::uint64_t value) noexcept
    {
        ++counts_[index(value)];
        ++total_;
        max_ = std::max(max_, value);
    }

    [[nodiscard]] std::uint64_t percentile(double p) const noexcept
    {
        const auto target = static_cast<std::uint64_t>(p / 100.0 * static_cast<double>(total_));
        std::uint64_t seen = 0;
        for (std::size_t i {}; i < kBuckets; ++i)
        {
            if ((seen += counts_[i]) > target) { return lowerBound(i); }
        }
        return max_;
    }

    [[nodiscard]] std::uint64_t count() const noexcept { return total_; }
    [[nodiscard]] std::uint64_t max() const noexcept { return max_; }

private:

    static std::size_t index(std::uint64_t value) noexcept
    {
        if (value < kSub) { return value; }
        const int shift = std::bit_width(value) - 1 - kSubBits;
        return ((shift + 1) << kSubBits) + ((value >> shift) & (kSub - 1));
    }

    static std::uint64_t lowerBound(std::size_t i) noexcept
    {
        if (i < kSub) { return i; }
        const int shift = static_cast<int>(i >> kSubBits) - 1;
        return (kSub + (i & (kSub - 1))) << shift;
    }

    std::array<std::uint64_t, kBuckets> counts_ {};
    std::uint64_t total_{}, max_ {};
};


