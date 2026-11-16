cmake_minimum_required(VERSION 3.31.6)
project(TradingSystem)

set(CMAKE_CXX_STANDARD 23)
set(PROJECT_INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/third_party
)

add_executable(OrderbookEngine apps/engine.cpp
        src/parser/Decode.cpp
        src/parser/Decode.h
        src/parser/Dispatch.h
        src/parser/Messages.h
        src/orderbook/Orderbook.h
        src/tradebook/Trade.h
        src/tradebook/Tradebook.h
        src/stock_directory/StockDirectory.h
        src/market_state/MarketState.cpp
        src/market_state/MarketState.h
        src/data_source/filesource/FileSource.h
        src/orderbook/Order.h
        src/market_state/Market.h
        src/console/ConsoleRenderer.h
        src/feed_handler/FeedHandler.h
        src/network/UdpReceiver.h
        src/protocol/moldudp64/MoldUDP64Decoder.h
        src/console/ConsoleSnapshotMessage.h
        third_party/rigtorp/Seqlock.h
        src/book_engine/BookEngine.h
        benchmarks/LatencyHistogram.h
        benchmarks/BenchmarkUtil.h
        benchmarks/MessageLatency.h
        include/types/MarketTypes.h
        include/types/TradebookTypes.h
        include/types/OrderbookTypes.h
        include/types/ItchTypes.h
        include/configs/MoldUDP64Constants.h
        include/configs/NetworkConfigs.h
        include/configs/ConsoleConfigs.h
        include/configs/MarketConfigs.h
        src/orderbook/MapOrderbook.h
        src/orderbook/VectorOrderbook.h
)

target_include_directories(OrderbookEngine PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/third_party
)

add_executable(FeedReplay apps/feed_sim.cpp
        src/replay/MessageGenerator.h
        src/protocol/moldudp64/MoldUDP64Encoder.h
        src/replay/Pacer.h
        src/network/UdpSender.h
        src/replay/FeedSimulator.h
        src/data_source/filesource/FileSource.h
        src/replay/TimeUtil.h
        include/types/MarketTypes.h
        include/types/TradebookTypes.h
        include/types/OrderbookTypes.h
        include/types/ItchTypes.h
        include/configs/MoldUDP64Constants.h
        include/configs/NetworkConfigs.h
        include/configs/ItchConfigs.h
        include/configs/MarketConfigs.h
)

target_include_directories(FeedReplay PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)