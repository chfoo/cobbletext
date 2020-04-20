#pragma once

#include <cstddef>
#include <unordered_map>
#include <memory>

#include <gsl/span>
#include <harfbuzz/hb-ft.h>

#include "FontInfo.hpp"
#include "internal/font/Font.hpp"
#include "internal/FreeType.hpp"

namespace cobbletext::internal {

class FontTable {
private:
    unsigned long int idCounter = 1;
    std::unordered_map<FontID,Font> fonts;
    std::shared_ptr<FreeType> freeType;

public:
    const FontID fallbackFont;

    explicit FontTable(std::shared_ptr<FreeType> freeType);
    ~FontTable();

    FontID load(const char * path);
    FontID loadBytes(const gsl::span<const uint8_t> & data);

    bool hasFont(FontID fontID);

    Font & getFont(FontID fontID);
    Font & getFontWithFallback(FontID fontID);
    FontInfo getFontInfo(FontID fontID);

    int32_t fontUnitsToPixels(FontID fontID, double fontSize, int32_t value);

private:
    FontID getFreeID();
};

}
