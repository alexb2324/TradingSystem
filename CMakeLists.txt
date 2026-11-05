cmake_minimum_required(VERSION 3.31.6)
project(TradingSystem)

set(CMAKE_CXX_STANDARD 23)

add_executable(OrderbookEngine src/apps/engine.cpp
        src/parser/Decode.cpp
        src/parser/Decode.h
        src/parser/Dispatch.cpp
        src/parser/Dispatch.h
        src/parser/Messages.cpp
        src/parser/Messages.h
        src/orderbook/Orderbook.cpp
        src/orderbook/Orderbook.h
        src/tradebook/Trade.h
        src/tradebook/Tradebook.h
        src/stockdirectory/StockDirectory.cpp
        src/stockdirectory/StockDirectory.h
        src/marketstate/MarketState.cpp
        src/marketstate/MarketState.h
        src/Constants.h
        src/Usings.h
        src/datasource/filesource/FileSource.h
        src/orderbook/Order.cpp
        src/orderbook/Order.h
        src/marketstate/Market.cpp
        src/marketstate/Market.h
        src/console/ConsoleRenderer.cpp
        src/console/ConsoleRenderer.h
        src/feedhandler/FeedHandler.h
        src/feedhandler/UdpReceiver.h
        src/feedhandler/MoldUDP64Decoder.h
        src/console/ConsoleSnapshotMessage.h
        src/orderbook/OrderbookLevel.h
        libs/rigtorp/Seqlock.h
        src/BookEngine/BookEngine.cpp
        src/BookEngine/BookEngine.h
)

add_executable(FeedReplay src/apps/feed_sim.cpp
        src/replay/MessageGenerator.h
        src/replay/MoldUDP64Encoder.cpp
        src/replay/MoldUDP64Encoder.h
        src/replay/Pacer.h
        src/replay/UdpSender.h
        src/replay/FeedSimulator.h
        src/datasource/filesource/FileSource.h
        src/replay/TimeUtil.h
)
