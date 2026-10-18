#pragma once

#include <map>
#include <string>
#include <unordered_map>

class Orderbook
{

public:

    Orderbook();
    Orderbook(const Orderbook&) = delete;
    void operator=(const Orderbook&) = delete;
    Orderbook(Orderbook&&) = delete;
    void operator=(Orderbook&&) = delete;
    ~Orderbook();

    Trades AddOrder(OrderPointer order);
    void CancelOrder(OrderId orderId);
    Trades ModifyOrder(OrderModify order);

    std::size_t Size() const;
    OrderbookLevelInfos GetOrderInfos() const;


private:

    struct OrderEntry
    {
        OrderPointer order_{ nullptr };
        OrderPointers::iterator location_;
    };

    struct LevelData
    {
        Quantity quantity_{ };
        Quantity count_{ };

        enum class Action
        {
            Add,
            Remove,
            Match,
        };
    };

    std::unordered_map<Price, LevelData> data_;
    std::map<Price, OrderPointers, std::greater<>> bids_;
    std::map<Price, OrderPointers, std::less<>> asks_;
    std::unordered_map<OrderId, OrderEntry> orders_;
    std::string symbol_;

private:
    void CancelOrders(OrderIds orderIds);
    void CancelOrderInternal(OrderId orderId);

    void OnOrderCancelled(OrderPointer order);
    void OnOrderAdded(OrderPointer order);
    void OnOrderMatched(Price price, Quantity quantity, bool isFullyFilled);
    void UpdateLevelData(Price price, Quantity quantity, LevelData::Action action);

    bool CanFullyFill(Side side, Price price, Quantity quantity) const;
    bool CanMatch(Side side, Price price) const;
};