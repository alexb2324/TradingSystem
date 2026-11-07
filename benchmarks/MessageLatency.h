#pragma once
#include <cstdint>

#include "LatencyHistogram.h"
#include "../src/Constants.h"

struct NoTiming
{
    static constexpr bool kEnabled { false };
    void record(std::uint8_t type, std::uint64_t cycles) noexcept;
};

class MessageLatency
{
public:
    static constexpr bool kEnabled { true };
    void record(std::uint8_t type, std::uint64_t cycles) noexcept
    {
        allLatency_.record(cycles);
        messageLatencyHistogram_[slot(type)].record(cycles);
    }

    void reportAll(double nsPerCycle) const
    {

        std::println("Overall Results");
        std::println("-------------------");
        for (double p : {50.0, 90.0, 99.0, 99.90, 99.99})
        {
            std::println("p{:<6} {}", p, formatLatencyNumbers(allLatency_.percentile(p) * nsPerCycle));
        }
        std::println("max{:<4} {}   (count = {})", " ", formatLatencyNumbers(allLatency_.max() * nsPerCycle), formatNumberWithCommas(allLatency_.count()));
        std::println();
    }

    void reportEachMessageLatency(double nsPerCycle) const
    {
        for (const auto& [messageType, size] : Constants::kItchMessageLengthsMap)
        {
            const auto& histogram = messageLatencyHistogram_[slot(messageType)];

            if (histogram.count() == 0) { continue; }

            std::println("Message Type: {}", messageType);
            std::println("-------------------");

            for (double p : {50.0, 90.0, 99.0, 99.90, 99.99})
            {
                std::println("p{:<6} {}", p, formatLatencyNumbers(histogram.percentile(p) * nsPerCycle));
            }
            std::println("max{:<4} {}   (count = {})", " ", formatLatencyNumbers(histogram.max() * nsPerCycle), formatNumberWithCommas(histogram.count()));
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

    static std::string formatLatencyNumbers(double nanoseconds)
    {
        if (nanoseconds >= 1'000'000.0)
        {
            return std::format("{:>8.2f} ms", nanoseconds / 1'000'000.0);
        }

        if (nanoseconds >= 1'000.0)
        {
            return std::format("{:>8.2f} us", nanoseconds / 1'000.0);
        }

        return std::format("{:>8.2f} ns", nanoseconds);
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

    LatencyHistogram allLatency_ {};
    std::array<LatencyHistogram, 53> messageLatencyHistogram_ {};
};
