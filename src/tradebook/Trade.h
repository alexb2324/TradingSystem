#pragma once

#include <cstdint>
#include "../Usings.h"

enum class TradeType : std::uint8_t
{
	Executed,
	ExecutedWithPrice,
	Trade,
	CrossTrade
};

struct Trade
{
	itch::Timestamp timestamp;
	itch::MatchNumber matchNumber;
	itch::Price price;
	itch::Shares shares;
	itch::StockLocate stockLocate;
	TradeType tradeType;
	char side;
	bool broken = false;
};
