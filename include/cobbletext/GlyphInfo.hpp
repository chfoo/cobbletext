#pragma once

#include <vector>

#include "common.hpp"

namespace cobbletext {

class COBBLETEXT_API GlyphInfo {
public:
    GlyphID id;
    std::vector<uint8_t> image;
    uint32_t imageWidth;
    uint32_t imageHeight;
    uint32_t atlasX;
    uint32_t atlasY;
};

}
