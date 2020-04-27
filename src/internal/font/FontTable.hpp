#pragma once

#include <cstddef>
#include <unordered_map>
#include <memory>

#include <gsl/span>

#ifdef __EMSCRIPTEN__
    #include <hb-ft.h>
#else
    #include <harfbuzz/hb-ft.h>
#endif

#include "FontInfo.hpp"
#include "internal/font/Font.hpp"
#include "internal/FreeType.hpp"

namespace cobbletext::internal {

class FontTable {
private:
    unsigned long int idCounter = 1;
    std::unordered_map<FontID,Font> fonts;
    std::unordered_map<FontID,FontID> alternativeMap;
    std::shared_ptr<FreeType> freeType;

public:
    const FontID fallbackFont;

    explicit FontTable(std::shared_ptr<FreeType> freeType);
    ~FontTable();

    FontID load(const char * path, int32_t faceIndex = 0);
    FontID loadBytes(const gsl::span<const uint8_t> & data,
        int32_t faceIndex = 0);

    bool hasFont(FontID fontID);

    Font & getFont(FontID fontID);
    Font & getFontWithFallback(FontID fontID);
    FontInfo getFontInfo(FontID fontID);

    bool setFontSize(FontID fontID, double fontSize);

    int32_t fontUnitsToPixels(FontID fontID, int32_t value);

    void setFontAlternative(FontID fontID, FontID fallbackFontID);

    FontID getFontAlternative(FontID fontID);

private:
    FontID getFreeID();
};

}
