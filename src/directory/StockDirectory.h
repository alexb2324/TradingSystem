#pragma once
#include <cstdint>
#include <stdexcept>
#include <unordered_map>
#include <print>

#include "../Constants.h"
#include "../Usings.h"


class StockDirectory {

public:
    StockDirectory() { stockMapping_.reserve(Constants::kMaxStockDirectorySize); }
    StockDirectory(const StockDirectory& other) = delete;
    StockDirectory& operator=(const StockDirectory& other) = delete;
    StockDirectory(StockDirectory&& other) = delete;
    StockDirectory& operator=(StockDirectory&& other) = delete;
    ~StockDirectory() = default;

    void displayAllStocks() const
    {
        for (const auto& [stockLocate, symbol] : stockMapping_)
        {
            std::println("Stock Locate Number: {}, Symbol: {}",
                stockLocate,
                symbol.displaySymbol()
            );
        }
    }

    [[nodiscard]] std::size_t size() const noexcept { return stockMapping_.size(); }
    void insert(const Symbol stock, const std::uint16_t stockLocate) { stockMapping_[stockLocate] = stock; }

    Symbol& find(const std::uint16_t stockLocate)
    {
        if (const auto it = stockMapping_.find(stockLocate); it != stockMapping_.end())
        {
            return it->second;
        }

        throw std::runtime_error("Stock locate not found");
    };


private:
    std::unordered_map<std::uint16_t, Symbol> stockMapping_;
};
