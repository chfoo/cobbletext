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

    explicit Glyph(GlyphID id) : id(id) {}
};

}
