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

const int32_t Library::VERSION_MAJOR = COBBLETEXT_VERSION_MAJOR;

const int32_t Library::VERSION_MINOR = COBBLETEXT_VERSION_MINOR;

const int32_t Library::VERSION_PATCH = COBBLETEXT_VERSION_PATCH;

const std::string Library::VERSION =
    std::to_string(Library::VERSION_MAJOR) + "." +
    std::to_string(Library::VERSION_MINOR) + "." +
    std::to_string(Library::VERSION_PATCH);

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

}
