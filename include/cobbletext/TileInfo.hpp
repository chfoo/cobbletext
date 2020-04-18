#pragma once

#include "common.hpp"

namespace cobbletext {

class COBBLETEXT_API TileInfo {
public:
    GlyphID glyphID;
    uint32_t atlasX;
    uint32_t atlasY;
};

}
