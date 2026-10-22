#pragma once

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

    void insert(const Symbol& stock, const itch::StockLocate stockLocate)
    {
        stockMapping_[stockLocate] = stock;
        ++directorySize_;
    }

    [[nodiscard]] Symbol find(const itch::StockLocate stockLocate) const noexcept
    {
        return stockMapping_[stockLocate];
    };

private:
    std::array<Symbol, Constants::kMaxStockDirectorySize> stockMapping_ {};
    std::size_t directorySize_ {};
};
