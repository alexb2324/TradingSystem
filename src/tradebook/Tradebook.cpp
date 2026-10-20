#include <print>
#include <utility>

#include "Tradebook.h"


void TradeBook::displayAllTrades() const
{
    for (const auto& trade : trades_)
    {
        std::println("TradeId: {}, Timestamp: {}, Stock: {}, Side: {}, Price: {}, Quantity: {}",
            trade.matchNumber,
            trade.header.timestamp,
            trade.stock.displaySymbol(),
            trade.buySellIndicator,
            trade.price.priceOriginal,
            trade.shares
        );
    }
}

void TradeBook::displayMostRecentTrade() const
{
    const auto& trade = trades_.back();

    std::println("TradeId: {}, Timestamp: {}, Stock: {}, Side: {}, Price: {}, Quantity: {}",
        trade.matchNumber,
        trade.header.timestamp,
        trade.stock.displaySymbol(),
        trade.buySellIndicator,
        trade.price.priceOriginal,
        trade.shares
    );
}

void TradeBook::addTrade(const TradeMessage& trade)
{
    trades_.push_back(trade);
}

template <typename... TradeArgs>
void TradeBook::addTrade(TradeArgs&&... args)
{
    trades_.emplace_back(std::forward<TradeArgs>(args)...);
}