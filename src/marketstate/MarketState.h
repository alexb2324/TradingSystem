#pragma once

#include "../stockdirectory/StockDirectory.h"
#include "../tradebook/Tradebook.h"

#include "../parser/Messages.h"

class MarketState {

public:
	MarketState() = default;
	MarketState(const MarketState& other) = delete;
	MarketState& operator=(const MarketState& other) = delete;
	MarketState(MarketState&& other) = delete;
	MarketState& operator=(MarketState&& other) = delete;
	~MarketState() = default;

	void onStockDirectory(StockDirectoryMessage message);
	void onAddOrder(AddOrderMessage message);
	void onAddOrderMPID(AddOrderMPIDMessage message);
	void onOrderExecuted(OrderExecutedMessage message);
	void onOrderExecutedWithPrice(OrderExecutedWithPriceMessage message);
	void onOrderCancelled(OrderCancelMessage message);
	void onTrade(TradeMessage message);
	void onCrossTrade(CrossTradeMessage message);
	void onBrokenTrade(BrokenTradeMessage message);

private:

	StockDirectory stockDirectory_;
	Tradebook tradeBook_;
	//Orderbook orderbook_;
};



