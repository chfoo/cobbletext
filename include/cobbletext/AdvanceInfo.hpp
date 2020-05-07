#pragma once

#include <ostream>

#include "common.hpp"
#include "macros.h"

namespace cobbletext {


enum class AdvanceType {
    Invalid = 0,
    Glyph = 1,
    InlineObject = 2,
    LineBreak = 3,
    Bidi = 4,
    Layout = 5
};

class COBBLETEXT_CPP_API AdvanceInfo {
public:
    AdvanceType type = AdvanceType::Invalid;
    uint32_t textIndex = 0;

    int32_t advanceX = 0;
    int32_t advanceY = 0;

    GlyphID glyphID = 0;
    int32_t glyphOffsetX = 0;
    int32_t glyphOffsetY = 0;

    InlineObjectID inlineObject = 0;

    CustomPropertyID customProperty = 0;

private:
    friend std::ostream & operator<<(std::ostream & stream,
        const AdvanceInfo & advanceInfo);
};


}
