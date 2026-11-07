#pragma once

#include <cstdint>
#include <x86intrin.h>
#include <chrono>
#include <thread>


static constexpr std::size_t kRuns { 1'000'000 };

using Clock = std::chrono::steady_clock;

inline std::uint64_t cycles() noexcept
{
    _mm_lfence();
    const auto t = __rdtsc();
    _mm_lfence();
    return t;
}

inline double calibrateNsPerCycle()
{
    double running = 0.0;
    for (std::size_t i {}; i < kRuns; ++i)
    {
        const auto c0 = cycles();
        const auto t0 = Clock::now();
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        const auto c1 = cycles();
        const auto t1 = Clock::now();
        running += std::chrono::duration<double, std::nano>(t1 - t0).count() / static_cast<double>(c1 - c0);
    }
    return running / kRuns;
}