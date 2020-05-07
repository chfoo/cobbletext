#pragma once

#include <vector>
#include <ostream>

#include "common.hpp"
#include "macros.h"

namespace cobbletext {

class COBBLETEXT_CPP_API GlyphInfo {
public:
    GlyphID id = 0;
    std::vector<uint8_t> image;
    uint32_t imageWidth = 0;
    uint32_t imageHeight = 0;
    int32_t imageOffsetX = 0;
    int32_t imageOffsetY = 0;

private:
    friend std::ostream & operator<<(std::ostream & stream,
        const GlyphInfo & glyphInfo);
};

}
