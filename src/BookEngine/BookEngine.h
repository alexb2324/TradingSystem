#pragma once

#include <chrono>

#include "../../libs/rigtorp/Seqlock.h"
#include "../console/ConsoleSnapshotMessage.h"
#include "../marketstate/MarketState.h"
#include "../parser/Dispatch.h"

class BookEngine {

    using Clock = std::chrono::steady_clock;
    static constexpr auto kPublishInterval = std::chrono::milliseconds(10);

public:
    BookEngine(std::string_view symbol, rigtorp::Seqlock<ConsoleSnapshotMessage>& seqlock)
    :   marketState_ { std::make_unique<MarketState>() }, symbol_ { symbol }, seqlock_ { seqlock }
    {}

    void onMessage(itch::Bytes message) const { dispatch(message, *marketState_); }

    [[nodiscard]] MarketState& marketState() const { return *marketState_; }

    void tryPublishSnapshot(const bool force = false) noexcept
    {
        const auto timeNow = Clock::now();
        if (!force && timeNow - lastPublishTime_ < kPublishInterval) { return; }
        lastPublishTime_ = timeNow;

        if (!stockLocate_) { stockLocate_ = marketState_->stockDirectory().findLocate(symbol_); }
        marketState_->fillSnapshot(stockLocate_, currentSnapshot_);
        currentSnapshot_.wallSeconds = std::chrono::duration<double>(timeNow - startTime_).count();
        seqlock_.store(currentSnapshot_);
    }

private:
    std::unique_ptr<MarketState> marketState_;
    std::string symbol_;
    std::optional<itch::StockLocate> stockLocate_;
    rigtorp::Seqlock<ConsoleSnapshotMessage>& seqlock_;
    ConsoleSnapshotMessage currentSnapshot_ {};
    Clock::time_point startTime_ {};
    Clock::time_point lastPublishTime_ {};
};


