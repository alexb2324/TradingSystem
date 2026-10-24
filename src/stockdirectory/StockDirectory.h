#pragma once

#include <print>
#include <optional>
#include "../Constants.h"
#include "../Usings.h"

class StockDirectory {

public:
    StockDirectory() = default;
    StockDirectory(const StockDirectory& other) = delete;
    StockDirectory& operator=(const StockDirectory& other) = delete;
    StockDirectory(StockDirectory&& other) = delete;
    StockDirectory& operator=(StockDirectory&& other) = delete;
    ~StockDirectory() = default;

    [[nodiscard]] std::size_t size() const noexcept { return directorySize_; }

    void printStockEntry(const itch::StockLocate stockLocate)
    {
        std::println("Stock Locate - {}, Stock Symbol - {}", stockLocate, stockMapping_[stockLocate].displaySymbol());
    }


    void insert(const Symbol& stock, const itch::StockLocate stockLocate)
    {
        stockMapping_[stockLocate] = stock;
        ++directorySize_;
        printStockEntry(stockLocate);
    }

    [[nodiscard]] Symbol find(const itch::StockLocate stockLocate) const noexcept
    {
        return stockMapping_[stockLocate];
    };

    [[nodiscard]] std::optional<itch::StockLocate> findLocate(const std::string_view symbol) const noexcept
    {
        for (std::size_t i {}; i < stockMapping_.size(); ++i)
        {
            if (stockMapping_[i].displaySymbol() == symbol)
            {
                return static_cast<itch::StockLocate>(i);
            }
        }
        return std::nullopt;
    }

private:
    std::array<Symbol, Constants::kMaxStockDirectorySize> stockMapping_ {};
    std::size_t directorySize_ {};
};
