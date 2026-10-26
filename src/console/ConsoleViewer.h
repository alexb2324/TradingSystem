#pragma once

#include <format>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>

#include "../marketstate/Market.h"
#include "../tradebook/Tradebook.h"
#include "../Usings.h"

struct ReplayStats
{
    std::uint64_t   messages {};
    itch::Timestamp exchangeTime {};   // ns since midnight
    double          wallSeconds {};
};



class ConsoleView
{
public:
    explicit ConsoleView(std::size_t depth = 10, std::size_t trades = 10)
    :   depth_{depth},
        trades_{trades}
    {}

    [[nodiscard]] std::string render(std::string_view symbol,
        std::optional<itch::StockLocate> locate,
        const Market& market,
        const Tradebook& tape,
        const ReplayStats& st) const
    {
        std::string out = "\x1b[H\x1b[J";                       // cursor home + clear: one write, no flicker
        auto o = std::back_inserter(out);
        std::format_to(o, "{:<6} {}   msgs {:>11}   {:>5.2f}M msg/s   live orders {:>8}   unknown refs {}\n\n",
                       symbol, formatTime(st.exchangeTime), st.messages,
                       st.wallSeconds > 0 ? st.messages / st.wallSeconds / 1e6 : 0.0,
                       market.liveOrders(), market.unknownOrders());
        if (!locate)
        {
            out += "  (symbol not in stock directory yet)\n"; return out;
        }

        const auto& book = market.orderbook(*locate);
        std::format_to(o, "  {:>6} {:>9} {:>11}  |  {:<11} {:<9} {:<6}\n",
                       "orders", "bid size", "bid", "ask", "ask size", "orders");
        auto bid = book.bids().begin();
        auto ask = book.asks().begin();
        for (std::size_t i = 0; i < depth_; ++i) {
            std::string left(29, ' '), right;

            if (bid != book.bids().end()) {
                left = std::format("  {:>6} {:>9} {:>11}", bid->second.orderCount, bid->second.totalShares, formatPrice(bid->first));
                ++bid;
            }

            if (ask != book.asks().end()) {
                right = std::format("{:<11} {:<9} {:<6}", formatPrice(ask->first), ask->second.totalShares, ask->second.orderCount);
                ++ask;
            }
            std::format_to(o, "{}  |  {}\n", left, right);
        }

        std::format_to(o, "\n  Recent trades\n  {:<18} {:>11} {:>8} {:>4} {:>4}\n", "time", "price", "shares", "side", "type");
        // Scan the global tape backwards, bounded so a quiet symbol doesn't scan the whole day.
        const auto& all = tape.trades();
        std::size_t shown = 0, scanned = 0;
        for (auto it = all.rbegin(); it != all.rend() && shown < trades_ && scanned < kMaxScan; ++it, ++scanned) {
            if (it->stockLocate != *locate) continue;
            std::format_to(o, "  {:<18} {:>11} {:>8} {:>4} {:>4}{}\n", formatTime(it->timestamp), formatPrice(it->price),
                           it->shares, it->side, static_cast<char>(it->tradeType), it->broken ? "  BROKEN" : "");
            ++shown;
        }
        return out;
    }

    static std::string formatPrice(orderbook::Price p) { return std::format("{}.{:04}", p / 10000, p % 10000); }
    static std::string formatTime(itch::Timestamp ns)
    {
        const auto s = ns / 1'000'000'000;
        return std::format("{:02}:{:02}:{:02}.{:09}", s / 3600, s / 60 % 60, s % 60, ns % 1'000'000'000);
    }

private:
    static constexpr std::size_t kMaxScan = 200'000;
    std::size_t depth_, trades_;
};