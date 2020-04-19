#pragma once

#include "common.hpp"

namespace cobbletext {


enum class AdvanceType {
    Glyph = 1,
    InlineObject = 2,
    LineBreak = 3,
    Bidi = 4,
    Layout = 5
};

class COBBLETEXT_API AdvanceInfo {
public:
    AdvanceType type;
    uint32_t textIndex;

    int32_t advanceX;
    int32_t advanceY;

    GlyphID glyph;
    int32_t glyphOffsetX;
    int32_t glyphOffsetY;

    InlineObjectID inlineObject;

    CustomPropertyID customProperty;
};


}
