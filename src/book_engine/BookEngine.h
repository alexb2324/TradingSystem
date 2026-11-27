#pragma once

#include <chrono>

#include "../../third_party/rigtorp/Seqlock.h"

#include "configs/ConsoleConfigs.h"
#include "../console/ConsoleSnapshotMessage.h"
#include "../market_state/MarketState.h"
#include "../parser/Dispatch.h"

#include "../../benchmarks/MessageLatency.h"
#include "../../benchmarks/BenchmarkUtil.h"

namespace prod
{
    class BookEngine
    {
        using Clock = std::chrono::steady_clock;

    public:
        BookEngine(std::string_view symbol, rigtorp::Seqlock<ConsoleSnapshotMessage>& seqlock)
        :   marketState_ { std::make_unique<MarketState>() },
            symbol_ { symbol },
            seqlock_ { seqlock },
            startTime_ { Clock::now() }
        {}

        void onMessage(const itch::Bytes message) const
        {
            dispatch(message, *marketState_);
        }

        [[nodiscard]] MarketState& marketState() const { return *marketState_; }

        void tryPublishSnapshot(const bool force = false) noexcept
        {
            const auto timeNow = Clock::now();
            if (!force && timeNow - lastPublishTime_ < console::config::kPublishToConsoleInterval) { return; }
            lastPublishTime_ = timeNow;

            if (!stockLocate_) { stockLocate_ = marketState_->stockDirectory().findLocate(symbol_); }
            marketState_->fillSnapshot(stockLocate_, currentSnapshot_);
            currentSnapshot_.wallSeconds = std::chrono::duration<double>(timeNow - startTime_).count();

            // Store snapshot
            seqlock_.store(currentSnapshot_);
        }

    private:
        std::unique_ptr<MarketState> marketState_;
        std::string symbol_;
        rigtorp::Seqlock<ConsoleSnapshotMessage>& seqlock_;
        std::optional<itch::StockLocate> stockLocate_ {};
        ConsoleSnapshotMessage currentSnapshot_ {};
        Clock::time_point startTime_ {};
        Clock::time_point lastPublishTime_ {};
    };
}


namespace test
{
    template <typename Timing = NoTiming>
    class BookEngine
    {
        using Clock = std::chrono::steady_clock;

    public:
        BookEngine()
        :   marketState_ { std::make_unique<MarketState>() }
        {}

        void onMessage(const itch::Bytes message)
        {
            if constexpr (Timing::kEnabled)
            {
                const auto t0 = cycles();
                dispatch(message, *marketState_);
                timing_.record(static_cast<char>(message[0]), cycles() - t0);
            }
            else
            {
                dispatch(message, *marketState_);
            }
        }

        [[nodiscard]] MarketState& marketState() const { return *marketState_; }
        [[nodiscard]] Timing& timing() { return timing_; }

    private:
        std::unique_ptr<MarketState> marketState_;
        [[no_unique_address]] Timing timing_ {};
    };
}




