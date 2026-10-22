#pragma once
#include <print>

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

    [[nodiscard]] std::size_t size() const noexcept { return stockMapping_.size(); }

    void insert(const Symbol& stock, const itch::StockLocate stockLocate) { stockMapping_[stockLocate] = stock; }

    [[nodiscard]] Symbol find(const itch::StockLocate stockLocate) const noexcept
    {
        return stockMapping_[stockLocate];
    };

private:
    std::array<Symbol, Constants::kMaxStockDirectorySize> stockMapping_;
};
