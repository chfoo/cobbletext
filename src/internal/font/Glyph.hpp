#pragma once

#include <vector>

#include "common.hpp"
#include "internal/font/GlyphKey.hpp"

namespace cobbletext::internal {

class Glyph {
public:
    GlyphID id;
    bool hasImage = false;
    std::vector<uint8_t> image;
    uint32_t imageWidth = 0;
    uint32_t imageHeight = 0;
    int32_t imageOffsetX = 0;
    int32_t imageOffsetY = 0;

    unsigned int referenceCount = 0;

    explicit Glyph(GlyphID id) : id(id) {}
};

}
