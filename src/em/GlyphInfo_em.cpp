#include "GlyphInfo.hpp"

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(cobbletext_GlyphInfo) {
    value_object<cobbletext::GlyphInfo>("GlyphInfo")
        .field("id", &cobbletext::GlyphInfo::id)
        .field("image", &cobbletext::GlyphInfo::image)
        .field("imageWidth", &cobbletext::GlyphInfo::imageWidth)
        .field("imageHeight", &cobbletext::GlyphInfo::imageHeight)
        .field("imageOffsetX", &cobbletext::GlyphInfo::imageOffsetX)
        .field("imageOffsetY", &cobbletext::GlyphInfo::imageOffsetY)
        ;
    register_vector<uint8_t>("vector_uint8_t");
}
