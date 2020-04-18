#pragma once

#include <harfbuzz/hb.h>

#include "internal/FreeType.hpp"

namespace cobbletext::internal {

class Font {
public:
    FT_Face const freeTypeFace;
    hb_font_t * const harfBuzzFont;

    Font(FT_Face freeTypeFace, hb_font_t * harfBuzzFont) :
        freeTypeFace(freeTypeFace),
        harfBuzzFont(harfBuzzFont) {};

};

}