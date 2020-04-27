#pragma once

#include <memory>

#ifdef __EMSCRIPTEN__
    #include <hb.h>
#else
    #include <harfbuzz/hb.h>
#endif

#include "internal/FreeType.hpp"

namespace cobbletext::internal {

class Font {
public:
    FT_Face const freeTypeFace;
    hb_font_t * const harfBuzzFont;
    double fontSize = 0;
    std::shared_ptr<std::vector<uint8_t>> fontData;

    Font(FT_Face freeTypeFace, hb_font_t * harfBuzzFont) :
        freeTypeFace(freeTypeFace),
        harfBuzzFont(harfBuzzFont) {};

};

}
