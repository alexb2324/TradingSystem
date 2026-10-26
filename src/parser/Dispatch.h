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
        {
            break;
        }
        case MessageType::StockDirectoryMessage:
	        if constexpr (requires (const StockDirectoryMessage& msg) { handler.onStockDirectory(msg); })
            {
                handler.onStockDirectory(decodeStockDirectoryMessage(message));
            }
	        break;
        case MessageType::StockTradingActionMessage:
        {
            break;
        }
        case MessageType::RegSHORestrictionMessage:
        {
            break;
        }
        case MessageType::MarketParticipantPositionMessage:
        {
            break;
        }
        case MessageType::MWCBDeclineLevelMessage:
        {
            break;
        }
        case MessageType::MWCBStatusMessage:
        {
            break;
        }
        case MessageType::IPOQuotingPeriodUpdateMessage:
        {
            break;
        }
        case MessageType::LULDAuctionCollarMessage:
        {
            break;
        }
        case MessageType::OperationalHaltMessage:
        {
            break;
        }
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
        {
            break;
        }
        case MessageType::RetailPriceImprovementMessage:
        {
            break;
        }
        case MessageType::DirectListingCapitalRaiseMessage:
        {
            break;
        }
        default:
            throw std::runtime_error("Unknown message type");
    }
}
