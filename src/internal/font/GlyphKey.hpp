#pragma once

#include <vector>
#include "stdint.h"

#include "common.hpp"

namespace cobbletext::internal {

class GlyphKey {
public:
    const FontID fontFace;
    const double fontSize;
    const uint32_t index;

    GlyphKey(FontID fontFace, double fontSize, uint32_t index) :
        fontFace(fontFace), fontSize(fontSize), index(index) {}

    bool operator==(const GlyphKey & other) const;
};

class GlyphKeyHasher {
public:
    std::size_t operator()(const GlyphKey & glyphKey) const;
};

}
