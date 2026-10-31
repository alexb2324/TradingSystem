#pragma once

#include <fstream>
#include "Decode.h"
#include "Messages.h"
#include "../Constants.h"

#include <cstdio>

template <typename Handler>
std::size_t parseData(const itch::Bytes data, Handler& handler)
{
    std::size_t readOffset {};

    while (readOffset + Constants::kItchMessageOffset <= data.size())
    {
        const auto msgLength = convertBLEndian<itch::MessageLength>(data.data() + readOffset);

        if (readOffset + Constants::kItchMessageOffset + msgLength > data.size()) { break; }
        if (msgLength == 0) { break; }

        dispatch(data.subspan(readOffset + Constants::kItchMessageOffset, msgLength), handler);

        readOffset += Constants::kItchMessageOffset + msgLength;
    }

    return readOffset;
}


template <typename Handler>
void dispatch(itch::Bytes message, Handler& handler)
{
    if constexpr (requires { handler.noteMessage(); })
    {
        handler.noteMessage(convertBLEndian48(message.data() + 5));
    }

    if constexpr (requires { handler.onRawMessage(message); })
    {
        handler.onRawMessage(message);
    }

	switch (static_cast<MessageType>(message[0]))
    {
        case MessageType::SystemEventMessage:
            if constexpr (requires (const SystemEventMessage& msg) { handler.onSystemEvent(msg); })
            {
                handler.onSystemEvent(decodeSystemEventMessage(message));
            }
            break;
        case MessageType::StockDirectoryMessage:
	        if constexpr (requires (const StockDirectoryMessage& msg) { handler.onStockDirectory(msg); })
            {
                handler.onStockDirectory(decodeStockDirectoryMessage(message));
            }
	        break;
        case MessageType::StockTradingActionMessage:
            if constexpr (requires (const StockTradingActionMessage& msg) { handler.onStockTradingAction(msg); })
            {
                handler.onStockTradingAction(decodeStockTradingActionMessage(message));
            }
            break;
        case MessageType::RegSHORestrictionMessage:
	        if constexpr (requires (const RegSHORestrictionMessage& msg) { handler.onRegSHORestriction(msg); })
	        {
	            handler.onRegSHORestriction(decodeRegSHORestrictionMessage(message));
	        }
	        break;
	    case MessageType::MarketParticipantPositionMessage:
			if constexpr (requires (const MarketParticipantPositionMessage& msg) { handler.onMarketParticipantPosition(msg); })
			{
				handler.onMarketParticipantPosition(decodeMarketParticipantPositionMessage(message));
			}
	        break;
        case MessageType::MWCBDeclineLevelMessage:
	        if constexpr (requires (const MWCBDeclineLevelMessage& msg) { handler.onMWCBDeclineLevel(msg); })
	        {
	            handler.onMWCBDeclineLevel(decodeMWCBDeclineLevelMessage(message));
	        }
	        break;
        case MessageType::MWCBStatusMessage:
	        if constexpr (requires (const MWCBStatusMessage& msg) { handler.onMWCBStatus(msg); })
	        {
	            handler.onMWCBStatus(decodeMWCBStatusMessage(message));
	        }
	        break;
        case MessageType::IPOQuotingPeriodUpdateMessage:
	        if constexpr (requires (const IPOQuotingPeriodUpdateMessage& msg) { handler.onIPOQuotingPeriodUpdate(msg); })
	        {
	            handler.onIPOQuotingPeriodUpdate(decodeIPOQuotingPeriodUpdateMessage(message));
	        }
	        break;
        case MessageType::LULDAuctionCollarMessage:
	        if constexpr (requires (const LULDAuctionCollarMessage& msg) { handler.onLULDAuctionCollar(msg); })
	        {
	            handler.onLULDAuctionCollar(decodeLULDAuctionCollarMessage(message));
	        }
	        break;
        case MessageType::OperationalHaltMessage:
			if constexpr (requires (const OperationalHaltMessage& msg) { handler.onOperationalHalt(msg); })
			{
				handler.onOperationalHalt(decodeOperationalHaltMessage(message));
			}
			break;
        case MessageType::AddOrderMessage:
	        if constexpr (requires (const AddOrderMessage& msg) { handler.onAddOrder(msg); })
            {
                handler.onAddOrder(decodeAddOrderMessage(message));
            }
	        break;
        case MessageType::AddOrderMPIDMessage:
	        if constexpr (requires (const AddOrderMPIDMessage& msg) { handler.onAddOrderMPID(msg); })
            {
                handler.onAddOrderMPID(decodeAddOrderMPIDMessage(message));
            }
	        break;
        case MessageType::OrderExecutedMessage:
	        if constexpr (requires (const OrderExecutedMessage& msg) { handler.onOrderExecuted(msg); })
            {
                handler.onOrderExecuted(decodeOrderExecutedMessage(message));
            }
	        break;
        case MessageType::OrderExecutedWithPriceMessage:
	        if constexpr (requires (const OrderExecutedWithPriceMessage& msg) { handler.onOrderExecutedWithPrice(msg); })
            {
                handler.onOrderExecutedWithPrice(decodeOrderExecutedWithPriceMessage(message));
            }
	        break;
        case MessageType::OrderCancelMessage:
	        if constexpr (requires (const OrderCancelMessage& msg) { handler.onOrderCancelled(msg); })
            {
                handler.onOrderCancelled(decodeOrderCancelMessage(message));
            }
	        break;
        case MessageType::OrderDeleteMessage:
	        if constexpr (requires (const OrderDeleteMessage& msg) { handler.onOrderDelete(msg); })
            {
                handler.onOrderDelete(decodeOrderDeleteMessage(message));
            }
	        break;
        case MessageType::OrderReplaceMessage:
	        if constexpr (requires (const OrderReplaceMessage& msg) { handler.onOrderReplace(msg); })
            {
                handler.onOrderReplace(decodeOrderReplaceMessage(message));
            }
	        break;
        case MessageType::TradeMessage:
	        if constexpr (requires (const TradeMessage& msg) { handler.onTrade(msg); })
            {
                handler.onTrade(decodeTradeMessage(message));
            }
	        break;
        case MessageType::CrossTradeMessage:
	        if constexpr (requires (const CrossTradeMessage& msg) { handler.onCrossTrade(msg); })
            {
                handler.onCrossTrade(decodeCrossTradeMessage(message));
            }
	        break;
        case MessageType::BrokenTradeMessage:
	        if constexpr (requires (const BrokenTradeMessage& msg) { handler.onBrokenTrade(msg); })
            {
                handler.onBrokenTrade(decodeBrokenTradeMessage(message));
            }
	        break;
        case MessageType::NOIIMessage:
			if constexpr (requires (const NOIIMessage& msg) { handler.onNOII(msg); })
			{
				handler.onNOII(decodeNOIIMessage(message));
			}
			break;
        case MessageType::RetailPriceImprovementMessage:
			if constexpr (requires (const RetailPriceImprovementMessage& msg) { handler.onRetailPriceImprovement(msg); })
			{
				handler.onRetailPriceImprovement(decodeRetailPriceImprovementMessage(message));
			}
			break;
        case MessageType::DirectListingCapitalRaiseMessage:
			if constexpr (requires (const DirectListingCapitalRaiseMessage& msg) { handler.onDirectListingCapitalRaise(msg); })
			{
				handler.onDirectListingCapitalRaise(decodeDirectListingCapitalRaiseMessage(message));
			}
			break;
        default:
            throw std::runtime_error("Unknown message type");
    }
}
