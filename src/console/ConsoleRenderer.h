#pragma once

#include <format>
#include <iterator>
#include <string>

#include "../tradebook/Tradebook.h"
#include "../Usings.h"
#include "../../libs/rigtorp/Seqlock.h"
#include "ConsoleSnapshotMessage.h"

class ConsoleRenderer
{
public:
    explicit ConsoleRenderer(std::string_view symbol, const rigtorp::Seqlock<ConsoleSnapshotMessage>& seqLock)
    :   symbol_ { symbol },
        snapshotSeqLock_ { seqLock }
    {}

    void run(const std::stop_token& stopToken) const
    {
        while (!stopToken.stop_requested())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            const auto snapshot = snapshotSeqLock_.load(); // Read snapshot from SeqLock

            const auto frame = render(snapshot);
            std::fwrite(frame.data(), 1, frame.size(), stdout);
            std::fflush(stdout);
        }
    }

    [[nodiscard]] std::string render(const ConsoleSnapshotMessage& message) const
    {
        std::string outputString = "\x1b[H\x1b[J";                       // cursor home + clear: one write, no flicker

        const auto outputIt = std::back_inserter(outputString);

        std::format_to(outputIt, "Msgs Processed: {:<9}  {:>5.2f}M msg/s   Total Live Orders: {:<9}\n",
            message.messages,
            message.wallSeconds > 0 ? message.messages / message.wallSeconds / 1e6 : 0.0,
            message.liveOrders);

        if (!message.symbolFound)
        {
            outputString += "  (Symbol not in stock directory yet)\n"; return outputString;
        }

        std::format_to(outputIt, "Stock: {:<6}  Price: {:<8}  Exchange Time: {}\n\n\n", symbol_, formatPriceHeader(message.recentTrades.front().price), formatTime(message.exchangeTime));
        std::format_to(outputIt, " -------------------------- Orderbook ----------------------------\n");
        std::format_to(outputIt, " | {:>6} {:>9} {:>11}     {:<11} {:<9} {:<6} |\n", "Volume", "Shares", "Price", "Price", "Shares", "Volume");

        std::format_to(outputIt, "");

        for (int i {}; i < message.bids.size(); ++i)
        {
            std::string left(29, ' '), right;
            left = std::format(" | {:>6} {:>9} {:>11}", message.bids[i].orderCount, message.bids[i].totalShares, formatPrice(message.bids[i].price));
            right = std::format("{:<11} {:<9} {:<6} |", formatPrice(message.asks[i].price), message.asks[i].totalShares, message.asks[i].orderCount);

            std::format_to(outputIt, "{}  |  {}\n", left, right);
        }
        std::format_to(outputIt, " -----------------------------------------------------------------\n");

        std::format_to(outputIt, "\n  Recent trades\n  {:<14} {:>11} {:>8} {:>6} {:>6}\n", "Time", "Price", "Shares", "Side", "Type");
        for (const auto& trade : message.recentTrades)
        {
            std::format_to(outputIt, "  {:<14} {:>11} {:>8} {:>6} {:>6}{}\n", formatTime(trade.timestamp), formatPrice(trade.price),
                               trade.shares, trade.side, static_cast<char>(trade.tradeType),
                               trade.broken ? "  BROKEN" : "");
        }
        return outputString;
    }

    static std::string formatPriceHeader(orderbook::Price p) { return std::format("{}.{:02}", p / 10'000, (p % 10'000) / 100); }
    static std::string formatPrice(orderbook::Price p) { return std::format("{}.{:04}", p / 10'000, p % 10'000); }
    static std::string formatTime(itch::Timestamp ns)
    {
        const auto s = ns / 1'000'000'000;
        return std::format("{:02}:{:02}:{:02}.{:03}", s / 3600, s / 60 % 60, s % 60, ns % 1'000);
    }

private:
    std::string symbol_ {};
    const rigtorp::Seqlock<ConsoleSnapshotMessage>& snapshotSeqLock_;
};