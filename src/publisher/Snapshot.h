#pragma once

#include <array>
#include <utility>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>

#include "../marketstate/Market.h"
#include "../tradebook/Tradebook.h"

#include "../Usings.h"
#include "../Constants.h"


struct Snapshot
{
    std::array<std::pair<orderbook::Price, OrderbookLevel>, Constants::kSnapshotDepth> bids {};
    std::array<std::pair<orderbook::Price, OrderbookLevel>, Constants::kSnapshotDepth> asks {};
    std::string symbol {};
    std::array<Trade, Constants::kTradeDepth> trades {};
};


[[nodiscard]] Snapshot generateSnapshot(std::string_view symbol,
        std::optional<itch::StockLocate> locate,
        const Market& market,
        const Tradebook& tradeTape) const
{
    const auto& book = market.orderbook(*locate);
    Snapshot snapshot;
    snapshot.symbol = symbol;

    auto bid = book.bids().begin();
    auto ask = book.asks().begin();
    for (std::size_t i {}; i < Constants::kSnapshotDepth; ++i) {

        if (bid != book.bids().end())
        {
            snapshot.bids[i] = *bid;
            ++bid;
        }

        if (ask != book.asks().end())
        {
            snapshot.asks[i] = *ask;
            ++ask;
        }
    }

    // Scan the global tape backwards, bounded so a quiet symbol doesn't scan the whole day.
    const auto& allTrades = tradeTape.trades();
    std::size_t shown {}, scanned {};
    for (auto it = allTrades.rbegin(); it != allTrades.rend() && shown < Constants::kTradeDepth && scanned < 200'000; ++it, ++scanned)
    {
        if (it->stockLocate != *locate) { continue; }
        snapshot.trades[shown] = *it;
        ++shown;
    }

    return snapshot;
}