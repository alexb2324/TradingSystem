#pragma once

#include <print>
#include <optional>

#include "types/ItchTypes.h"
#include "configs/MarketConfigs.h"

class StockDirectory {

public:
    StockDirectory() = default;
    StockDirectory(const StockDirectory& other) = delete;
    StockDirectory& operator=(const StockDirectory& other) = delete;
    StockDirectory(StockDirectory&& other) = delete;
    StockDirectory& operator=(StockDirectory&& other) = delete;
    ~StockDirectory() = default;

    [[nodiscard]] std::size_t size() const noexcept { return directorySize_; }

    void printStockEntry(const itch::StockLocate stockLocate) const
    {
        std::println("Stock Locate - {}, Stock Symbol - {}", stockLocate, stockMapping_[stockLocate].displaySymbol());
    }


    void insert(const itch::Symbol& stock, const itch::StockLocate stockLocate)
    {
        stockMapping_[stockLocate] = stock;
        ++directorySize_;
        //printStockEntry(stockLocate);
    }

    [[nodiscard]] itch::Symbol find(const itch::StockLocate stockLocate) const noexcept
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
    std::array<itch::Symbol, market::configs::kStockDirectoryMaxSize> stockMapping_ {};
    std::size_t directorySize_ {};
};
