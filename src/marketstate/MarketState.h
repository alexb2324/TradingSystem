#pragma once

#include "../stockdirectory/StockDirectory.h"
#include "../tradebook/Tradebook.h"
#include "Market.h"

#include "../parser/Messages.h"

class MarketState {

public:
	MarketState() = default;
	MarketState(const MarketState& other) = delete;
	MarketState& operator=(const MarketState& other) = delete;
	MarketState(MarketState&& other) = delete;
	MarketState& operator=(MarketState&& other) = delete;
	~MarketState() = default;

	void onStockDirectory(const StockDirectoryMessage& message);
	void onAddOrder(const AddOrderMessage& message);
	void onAddOrderMPID(const AddOrderMPIDMessage& message);
	void onOrderExecuted(const OrderExecutedMessage& message);
	void onOrderExecutedWithPrice(const OrderExecutedWithPriceMessage& message);
	void onOrderCancelled(const OrderCancelMessage& message);
	void onOrderDelete(const OrderDeleteMessage& message);
	void onOrderReplace(const OrderReplaceMessage& message);
	void onTrade(const TradeMessage& message);
	void onCrossTrade(const CrossTradeMessage& message);
	void onBrokenTrade(const BrokenTradeMessage& message);


private:
	StockDirectory stockDirectory_ {};
	Tradebook tradeBook_ {};
	Market market_ {};
};



