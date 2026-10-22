#pragma once

#include <vector>
#include <cstddef>

#include "Trade.h"
#include "../Constants.h"

class Tradebook
{
public:

    Tradebook() { tradeTape_.reserve(Constants::kReserveDailyTradeCount); }
    Tradebook(const Tradebook& other) = delete;
    Tradebook& operator=(const Tradebook& other) = delete;
    Tradebook(Tradebook&& other) = delete;
    Tradebook& operator=(Tradebook&& other) = delete;
    ~Tradebook() = default;

    [[nodiscard]] Trade& lastTrade() { return tradeTape_.back(); }
    [[nodiscard]] std::size_t size() const noexcept { return tradeTape_.size(); }
    void addTrade(const Trade& trade) { tradeTape_.push_back(trade); }

    template <typename... TradeArgs>
    void addTrade(TradeArgs&&... tradeArgs)
    {
        tradeTape_.emplace_back(tradeArgs...);
    }


private:
    std::vector<Trade> tradeTape_ {};
};