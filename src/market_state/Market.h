#pragma once

#include <unordered_map>
#include <vector>

#include "../orderbook/Order.h"
#include "../orderbook/Orderbook.h"

#include "types/ItchTypes.h"
#include "configs/MarketConfigs.h"

class Market {
public:
    Market() : books_ { market::configs::kStockDirectoryMaxSize }
    {
        orderTable_.reserve(market::configs::kOrderBookReserveDailyOrderCount);
    }

    Market(const Market& other) = delete;
    Market& operator=(const Market& other) = delete;
    Market(Market&& other) = delete;
    Market& operator=(Market&& other) = delete;
    ~Market() = default;

    // Add new order to global table and individual book
    void add(const market::OrderId orderId,
        const market::Price price,
        const market::Quantity quantity,
        const market::Side side,
        const itch::StockLocate stockLocate)
    {
        orderTable_.emplace(orderId, Order { quantity, price, side, stockLocate });
        books_[stockLocate].addOrder(side, price, quantity);
    }

    // Message types E, C and X, return order for trade tape
    Order reduce(const market::OrderId orderId, const market::Quantity quantity)
    {
        const auto it = orderTable_.find(orderId);
        const Order order = it->second;

        it->second.quantity -= quantity;
        const bool orderRemoved = it->second.quantity == 0;

        books_[order.stockLocate].reduce(order.side, order.price, quantity, orderRemoved);
        if (orderRemoved) { orderTable_.erase(it); }

        return order;
    }

    // Message type D
    void remove(const market::OrderId orderId)
    {
        if (auto it = orderTable_.find(orderId); it != orderTable_.end())
        {
            reduce(orderId, it->second.quantity);
        }
    }

    // Message type U
    void replace(const market::OrderId prevOrderId,
        const market::OrderId newOrderId,
        const market::Price newPrice,
        const market::Quantity newQuantity)
    {
        const Order prevOrder = orderTable_.at(prevOrderId);
        remove(prevOrderId);
        add(newOrderId, newPrice, newQuantity, prevOrder.side, prevOrder.stockLocate);
    }

    [[nodiscard]] const Orderbook& orderbook(const itch::StockLocate& stockLocate) const { return books_[stockLocate]; };
    [[nodiscard]] std::size_t liveOrders() const { return orderTable_.size(); }
    [[nodiscard]] std::size_t unknownOrders() const noexcept { return unknownOrders_; }

private:
    std::unordered_map<market::OrderId, Order> orderTable_;
    std::vector<Orderbook> books_;
    std::size_t unknownOrders_ {};
};
