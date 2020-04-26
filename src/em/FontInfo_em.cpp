#include "FontInfo.hpp"

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(cobbletext_FontInfo) {
    value_object<cobbletext::FontInfo>("FontInfo")
        .field("id", &cobbletext::FontInfo::id)
        .field("familyName", &cobbletext::FontInfo::familyName)
        .field("styleName", &cobbletext::FontInfo::styleName)
        .field("unitsPerEM", &cobbletext::FontInfo::unitsPerEM)
        .field("ascender", &cobbletext::FontInfo::ascender)
        .field("descender", &cobbletext::FontInfo::descender)
        .field("height", &cobbletext::FontInfo::height)
        .field("underlinePosition", &cobbletext::FontInfo::underlinePosition)
        .field("underlineThickness", &cobbletext::FontInfo::underlineThickness)
        ;
}
