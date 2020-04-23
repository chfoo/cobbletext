#include "Library.hpp"
#include "LibraryImpl.hpp"

#ifndef __INTELLISENSE__
    #include "Version.h"
#else
    #define COBBLETEXT_VERSION_MAJOR 0
    #define COBBLETEXT_VERSION_MINOR 0
    #define COBBLETEXT_VERSION_PATCH 0
#endif

#include <gsl/span>

namespace cobbletext {

int32_t Library::versionMajor() {
    return COBBLETEXT_VERSION_MAJOR;
}

int32_t Library::versionMinor() {
    return COBBLETEXT_VERSION_MINOR;
}

int32_t Library::versionPatch() {
    return COBBLETEXT_VERSION_PATCH;
}

std::string Library::version() {
    return std::to_string(COBBLETEXT_VERSION_MAJOR) + "." +
        std::to_string(COBBLETEXT_VERSION_MINOR) + "." +
        std::to_string(COBBLETEXT_VERSION_PATCH);
}

Library::Library() :
    impl(std::make_unique<Impl>()) {}

FontID Library::fallbackFont() {
    return impl->context->fontTable->fallbackFont;
}

FontID Library::loadFont(const std::string path) {
    return impl->context->fontTable->load(path.c_str());
}

FontID Library::loadFontBytes(const uint8_t * bytes, size_t length) {
    return impl->context->fontTable->loadBytes(gsl::span(bytes, length));
}

FontInfo Library::getFontInfo(FontID id) {
    return impl->context->fontTable->getFontInfo(id);
}

GlyphInfo Library::getGlyphInfo(GlyphID id) {
    return impl->context->glyphTable->getGlyphInfo(id);
}

void Library::setFontAlternative(FontID id, FontID fallbackID) {
    impl->context->fontTable->setFontAlternative(id, fallbackID);
}

FontID Library::getFontAlternative(FontID id) {
    return impl->context->fontTable->getFontAlternative(id);
}

}
