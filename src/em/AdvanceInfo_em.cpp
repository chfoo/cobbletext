#include "AdvanceInfo.hpp"

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(cobbletext_AdvanceInfo) {
    enum_<cobbletext::AdvanceType>("AdvanceType")
        .value("Invalid", cobbletext::AdvanceType::Invalid)
        .value("Glyph", cobbletext::AdvanceType::Glyph)
        .value("InlineObject", cobbletext::AdvanceType::InlineObject)
        .value("LineBreak", cobbletext::AdvanceType::LineBreak)
        .value("Bidi", cobbletext::AdvanceType::Bidi)
        .value("Layout", cobbletext::AdvanceType::Layout)
        ;

    value_object<cobbletext::AdvanceInfo>("AdvanceInfo")
        .field("type", &cobbletext::AdvanceInfo::type)
        .field("textIndex", &cobbletext::AdvanceInfo::textIndex)
        .field("advanceX", &cobbletext::AdvanceInfo::advanceX)
        .field("advanceY", &cobbletext::AdvanceInfo::advanceY)
        .field("glyphID", &cobbletext::AdvanceInfo::glyphID)
        .field("glyphOffsetX", &cobbletext::AdvanceInfo::glyphOffsetX)
        .field("glyphOffsetY", &cobbletext::AdvanceInfo::glyphOffsetY)
        .field("inlineObject", &cobbletext::AdvanceInfo::inlineObject)
        .field("customProperty", &cobbletext::AdvanceInfo::customProperty)
        ;
}
