#pragma once

#include <memory>
#include <string>
#include <stdint.h>

#include "common.hpp"
#include "macros.h"
#include "FontInfo.hpp"
#include "GlyphInfo.hpp"

namespace cobbletext {


class COBBLETEXT_API Library {
public:
    static const int32_t VERSION_MAJOR;
    static const int32_t VERSION_MINOR;
    static const int32_t VERSION_PATCH;
    static const std::string VERSION;

    friend class Engine;

    Library();

    FontID fallbackFont();

    FontID loadFont(const std::string path);

    FontID loadFontBytes(const uint8_t * bytes, size_t length);

    FontInfo getFontInfo(FontID id);

    GlyphInfo getGlyphInfo(GlyphID id);

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};


}
