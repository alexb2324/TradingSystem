#pragma once
#include <cstdint>

#include "LatencyHistogram.h"
#include "configs/ItchConfigs.h"

struct NoTiming
{
    static constexpr bool kEnabled { false };
    void record(std::uint8_t type, std::uint64_t cycles) noexcept;
};

class MessageLatency
{
    static constexpr std::array<double, 5> percentiles_ {50.0, 90.0, 99.0, 99.90, 99.99};

public:
    static constexpr bool kEnabled { true };

    void record(std::uint8_t type, std::uint64_t cycles) noexcept
    {
        overallHistogram_.record(cycles);
        messageLatencyHistograms_[slot(type)].record(cycles);
    }

    void reportOverall(double nsPerCycle) const
    {
        std::println("Overall Results");
        std::println("--------------------------------");
        for (double p : percentiles_)
        {
            std::println("{:<14} {:>14} ns", std::format("p{}", p), formatNumberWithCommas(overallHistogram_.cyclesByPercentile(p) * nsPerCycle));
        }
        std::println("{:<14} {:>14} ns", "max", formatNumberWithCommas(overallHistogram_.maxSamples() * nsPerCycle));
        std::println("--------------------------------");
        std::println("{:<14}    {:>14}", "Total Samples", formatNumberWithCommas(overallHistogram_.totalSamples()));
        std::println();
    }

    void reportEachMessageLatency(double nsPerCycle) const
    {
        for (const auto& [messageType, size] : itch::configs::kItchMessageLengthsMap)
        {
            const auto& histogram = messageLatencyHistograms_[slot(messageType)];

            if (histogram.totalSamples() == 0) { continue; }

            std::println("Message Type: {}", messageType);
            std::println("--------------------------------");

            for (double p : percentiles_)
            {
                std::println("{:<14} {:>14} ns", std::format("p{}", p), formatNumberWithCommas(histogram.cyclesByPercentile(p) * nsPerCycle));
            }
            std::println("{:<14} {:>14} ns", "max", formatNumberWithCommas(histogram.maxSamples() * nsPerCycle));
            std::println("--------------------------------");
            std::println("{:<14}    {:>14}", "Total Samples", formatNumberWithCommas(histogram.totalSamples()));
            std::println();
        }
    }

private:

    static std::size_t slot(std::uint8_t type) noexcept
    {
        if (type >= 'A' && type <= 'Z') { return type - 'A'; }
        if (type >= 'a' && type <= 'z') { return 26 + type - 'a'; }
        return 52;
    }

    static std::string formatNumberWithCommas(std::size_t value)
    {
        std::string result = std::to_string(value);

        for (std::ptrdiff_t i = static_cast<std::ptrdiff_t>(result.size()) - 3; i > 0; i -= 3)
        {
            result.insert(static_cast<std::size_t>(i), ",");
        }
        return result;
    }

    LatencyHistogram overallHistogram_ {};
    std::array<LatencyHistogram, 53> messageLatencyHistograms_ {};
};
