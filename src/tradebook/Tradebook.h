#pragma once

#include <algorithm>
#include <vector>
#include <cstddef>

#include "Trade.h"
#include "configs/MarketConfigs.h"

class Tradebook
{
public:

    Tradebook() { tradeTape_.reserve(market::configs::kTradeBookReserveDailyTradeCount); }
    Tradebook(const Tradebook& other) = delete;
    Tradebook& operator=(const Tradebook& other) = delete;
    Tradebook(Tradebook&& other) = delete;
    Tradebook& operator=(Tradebook&& other) = delete;
    ~Tradebook() = default;

    [[nodiscard]] Trade& lastTrade() { return tradeTape_.back(); }
    [[nodiscard]] std::size_t size() const noexcept { return tradeTape_.size(); }

    void recordTrade(const Trade& trade) { tradeTape_.push_back(trade); }

    Trade* findTrade(const itch::MatchNumber matchNumber)
    {
        const auto it = std::ranges::lower_bound(tradeTape_, matchNumber, {}, &Trade::matchNumber);

        if (it != tradeTape_.end() && it->matchNumber == matchNumber)
        {
            return &*it;
        }

        return nullptr;
    }

    [[nodiscard]] const std::vector<Trade>& trades() const { return tradeTape_; }

private:
    std::vector<Trade> tradeTape_ {};
    std::uint64_t indexOffset {};
};

/*
matchnumber == 5010
tape = [1000, 1001, 1002, .... ,5000, 5010, 5011, .... ,8000]
index = [0, 1, 2, .... ,4000, 4001, 4002, .... ,7000]
offset
index = matchnumber - offset = 4010;
*/
