#pragma once

#include <memory>
#include <string>
#include <stdint.h>

#include "common.hpp"
#include "macros.h"
#include "FontInfo.hpp"
#include "GlyphInfo.hpp"

namespace cobbletext {


class Library {
public:
    static int32_t versionMajor();
    static int32_t versionMinor();
    static int32_t versionPatch();
    static std::string version();

    friend class Engine;

    Library();
    ~Library();

    FontID fallbackFont();

    FontID loadFont(const std::string path);

    FontID loadFontBytes(const uint8_t * bytes, size_t length);

    FontInfo getFontInfo(FontID id);

    GlyphInfo getGlyphInfo(GlyphID id);

    void setFontAlternative(FontID id, FontID fallbackID);

    FontID getFontAlternative(FontID id);

    void clearGlyphs();

private:
    class Impl;
    std::unique_ptr<Impl> impl;
};


}
