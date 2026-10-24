#pragma once

#include <cstdint>
#include "../Usings.h"
#include "../parser/Messages.h"

enum class TradeType : char
{
	OrderExecuted = 'O',
	OrderExecutedWithPrice = 'P',
	Trade = 'T',
	CrossTrade = 'C'
};

struct Trade
{
	itch::Timestamp timestamp;
	itch::MatchNumber matchNumber;
	std::uint64_t shares;
	Price price;
	itch::StockLocate stockLocate;
	TradeType tradeType;
	char side;
	char crossType;
	bool broken = false;
};
