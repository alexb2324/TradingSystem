#pragma once

#include <cstdint>
#include "../Usings.h"
#include "../parser/Messages.h"
#include "../orderbook/Order.h"

enum class TradeType : std::uint8_t
{
	OrderExecuted,
	OrderExecutedWithPrice,
	Trade,
	CrossTrade
};

struct Trade
{
	Trade(const OrderExecutedMessage& message, const Order* order);
	Trade(const OrderExecutedWithPriceMessage& message, const Order* order);
	explicit Trade(const TradeMessage& message);
	explicit Trade(const CrossTradeMessage& message);

	itch::Timestamp timestamp;
	itch::MatchNumber matchNumber;
	std::uint64_t shares;
	Symbol symbol;
	Price price;
	itch::StockLocate stockLocate;
	TradeType tradeType;
	char side;
	char crossType;
	bool broken = false;
};
