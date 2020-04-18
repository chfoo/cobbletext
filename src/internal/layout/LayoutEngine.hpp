#pragma once

#include <memory>

#include "internal/Context.hpp"
#include "internal/input/TextSource.hpp"
#include "TileInfo.hpp"
#include "AdvanceInfo.hpp"

namespace cobbletext::internal {

class LayoutEngine {
    std::shared_ptr<Context> context;
    std::shared_ptr<TextSource> textSource;

public:
    std::vector<TileInfo> tiles;
    std::vector<AdvanceInfo> advances;
    bool tilesValid = false;

    LayoutEngine(std::shared_ptr<Context> context,
        std::shared_ptr<TextSource> textSource);

    void clear();

    void layOut();

};

}
