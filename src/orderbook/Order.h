#pragma once

#include "../Usings.h"

struct Order
{
    Order(orderbook::Quantity quantity, orderbook::Price price, orderbook::Side side, itch::StockLocate stockLocate)
        : quantity { quantity }
        , price { price }
        , side { side }
        , stockLocate { stockLocate }
    { }

    orderbook::Quantity quantity;
    orderbook::Price price;
    orderbook::Side side;
    itch::StockLocate stockLocate;
};