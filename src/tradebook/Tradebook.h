#pragma once

#include <vector>
#include <cstddef>
#include <print>
#include <utility>

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


    void printTrade(const Trade& trade)
    {
        std::println("Trade type - {}, Timestamp - {}, Trade Number - {}, Shares - {}, Price - {}, stockLocate - {}",
            static_cast<char>(trade.tradeType),
            trade.timestamp,
            trade.matchNumber,
            trade.shares,
            trade.price,
            trade.stockLocate);
    }

    [[nodiscard]] Trade& lastTrade() { return tradeTape_.back(); }
    [[nodiscard]] std::size_t size() const noexcept { return tradeTape_.size(); }
    void recordTrade(const Trade& trade) { tradeTape_.push_back(trade); }

    Trade* findTrade(itch::MatchNumber matchNumber)
    {
        for (auto it = tradeTape_.rbegin(); it != tradeTape_.rend(); ++it)
        {
            if (it->matchNumber == matchNumber) return &(*it);
        }
        return nullptr;
    }

    [[nodiscard]] const std::vector<Trade>& trades() const { return tradeTape_; }

private:
    std::vector<Trade> tradeTape_ {};
};