#pragma once

#include <vector>
#include <cstddef>

#include "../parser/Messages.h"
#include "Trade.h"
#include "../Constants.h"

class TradeBook 
{

public:

    TradeBook() { tradeTape_.reserve(Constants::kReserveDailyTradeCount); }
    TradeBook(const TradeBook& other) = delete;
    TradeBook& operator=(const TradeBook& other) = delete;
    TradeBook(TradeBook&& other) = delete;
    TradeBook& operator=(TradeBook&& other) = delete;
    ~TradeBook() = default;

    [[nodiscard]] Trade& lastTrade() { return tradeTape_.back(); }

    [[nodiscard]] std::size_t size() const noexcept { return tradeTape_.size(); }


private:
    std::vector<Trade> tradeTape_;
};