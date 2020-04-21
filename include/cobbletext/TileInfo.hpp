#pragma once

#include <ostream>

#include "common.hpp"
#include "macros.h"

namespace cobbletext {

class COBBLETEXT_API TileInfo {
public:
    GlyphID glyphID = 0;
    uint32_t atlasX = 0;
    uint32_t atlasY = 0;

private:
    friend std::ostream & operator<<(std::ostream & stream,
        const TileInfo & tileInfo);
};

}
