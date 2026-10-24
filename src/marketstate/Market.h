#pragma once

#include <unordered_map>
#include <vector>

#include "../orderbook/Order.h"
#include "../orderbook/Orderbook.h"

#include "../Usings.h"
#include "../Constants.h"

class Market {
public:
    Market() : books_ { Constants::kMaxStockDirectorySize }
    {
        orderTable_.reserve(10'000'000u);
    }

    Market(const Market& other) = delete;
    Market& operator=(const Market& other) = delete;
    Market(Market&& other) = delete;
    Market& operator=(Market&& other) = delete;
    ~Market() = default;

    // Add new order to global table and individual book
    void add(const orderbook::OrderId orderId,
        const orderbook::Price price,
        const orderbook::Quantity quantity,
        const orderbook::Side side,
        const itch::StockLocate stockLocate)
    {
        orderTable_.emplace(orderId, Order { quantity, price, side, stockLocate });
        books_[stockLocate].addOrder(side, price, quantity);
    }

    // Message types E, C and X, return order for trade tape
    Order reduce(const orderbook::OrderId orderId, const orderbook::Quantity quantity)
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
    void remove(const orderbook::OrderId orderId)
    {
        if (auto it = orderTable_.find(orderId); it != orderTable_.end())
        {
            reduce(orderId, it->second.quantity);
        }
    }

    // Message type U
    void replace(const orderbook::OrderId prevOrderId,
        const orderbook::OrderId newOrderId,
        const orderbook::Price newPrice,
        const orderbook::Quantity newQuantity)
    {
        const Order prevOrder = orderTable_.at(prevOrderId);
        remove(prevOrderId);
        add(newOrderId, newPrice, newQuantity, prevOrder.side, prevOrder.stockLocate);
    }

    const Orderbook& orderbook(const itch::StockLocate& stockLocate) const { return books_[stockLocate]; };

private:
    std::unordered_map<orderbook::OrderId, Order> orderTable_;
    std::vector<Orderbook> books_;
};
