#include "Library.hpp"

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(cobbletext_Library) {
    class_<cobbletext::Library>("Library")
        .class_function("versionMajor", &cobbletext::Library::versionMajor)
        .class_function("versionMinor", &cobbletext::Library::versionMinor)
        .class_function("versionPatch", &cobbletext::Library::versionPatch)
        .class_function("version", &cobbletext::Library::version)

        // .constructor<>()
        // .smart_ptr<std::shared_ptr<cobbletext::Library>>("Library")
        .smart_ptr_constructor("Library", &std::make_shared<cobbletext::Library>)

        .function("fallbackFont", &cobbletext::Library::fallbackFont)
        .function("loadFont", &cobbletext::Library::loadFont)
        .function("loadFontBytes", &cobbletext::Library::loadFontBytes, allow_raw_pointers())
        .function("getFontInfo", &cobbletext::Library::getFontInfo)
        .function("getGlyphInfo", &cobbletext::Library::getGlyphInfo)
        .function("setFontAlternative", &cobbletext::Library::setFontAlternative)
        .function("getFontAlternative", &cobbletext::Library::getFontAlternative)
        .function("clearGlyphs", &cobbletext::Library::clearGlyphs)
        ;
}
