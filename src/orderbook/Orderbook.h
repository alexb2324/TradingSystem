#pragma once

#define VECTOR_ORDERBOOK

#if defined(MAP_ORDERBOOK)
    #include "MapOrderbook.h"
    using Orderbook = MapOrderbook;
#elif defined(VECTOR_ORDERBOOK)
    #include "VectorOrderbook.h"
    using Orderbook = VectorOrderbook;
#endif


