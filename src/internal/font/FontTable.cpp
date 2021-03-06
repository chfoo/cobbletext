#include "internal/font/FontTable.hpp"

#include <limits>

#include <gsl/span>

#include "Exception.hpp"
#include "Resource.hpp"
#include "internal/ExceptionUtil.hpp"

namespace cobbletext::internal {

FontTable::FontTable(std::shared_ptr<FreeType> freeType) :
    freeType(freeType),
    fallbackFont(
        loadBytes(
            gsl::span(Resource::ADOBE_NOTDEF, Resource::ADOBE_NOTDEF_SIZE))
    ) {}

FontTable::~FontTable() {
    for (auto const & item : fonts) {
        FT_Done_Face(item.second.freeTypeFace);
        hb_font_destroy(item.second.harfBuzzFont);
    }
}

FontID FontTable::load(const char * path, int32_t faceIndex) {
    auto library = freeType->library.get();
    FT_Face fontFace;

    auto errorCode = FT_New_Face(library, path, faceIndex, &fontFace);

    FreeType::throwIfError(errorCode);

    auto hbFont = hb_ft_font_create_referenced(fontFace);
    auto id = getFreeID();

    fonts.emplace(id, Font(fontFace, hbFont));

    return id;
}

FontID FontTable::loadBytes(const gsl::span<const uint8_t> & data,
        int32_t faceIndex) {
    auto library = freeType->library.get();
    FT_Face fontFace;

    auto dataVector = std::make_shared<std::vector<uint8_t>>(
        data.data(), data.data() + data.size());

    auto errorCode = FT_New_Memory_Face(library,
        reinterpret_cast<const FT_Byte *>(dataVector->data()),
        dataVector->size(), faceIndex, &fontFace);

    FreeType::throwIfError(errorCode);

    auto hbFont = hb_ft_font_create_referenced(fontFace);
    auto id = getFreeID();

    fonts.emplace(id, Font(fontFace, hbFont));
    fonts.at(id).fontData = dataVector;

    return id;
}

FontID FontTable::getFreeID() {
    auto id = idCounter;

    if (id == std::numeric_limits<FontID>::max()) {
        throw_with_trace(RuntimeError("max font id"));
    }

    idCounter++;
    return id;
}

bool FontTable::hasFont(FontID fontID) {
    return fonts.find(fontID) != fonts.end();
}

Font & FontTable::getFont(FontID fontID) {
    try {
        return fonts.at(fontID);
    } catch (const std::out_of_range & exception) {
        throw_with_trace(
            LogicError("font not in table: " + std::to_string(fontID)));
    }
}

Font & FontTable::getFontWithFallback(FontID fontID) {
    if (fonts.find(fontID) != fonts.end()) {
        return fonts.at(fontID);
    }

    return getFont(fallbackFont);
}

FontInfo FontTable::getFontInfo(FontID fontID) {
    auto & font = getFont(fontID);

    FontInfo fontInfo;

    fontInfo.id = fontID;

    if (font.freeTypeFace->family_name) {
        fontInfo.familyName = font.freeTypeFace->family_name;
    }

    if (font.freeTypeFace->style_name) {
        fontInfo.styleName = font.freeTypeFace->style_name;
    }

    fontInfo.unitsPerEM = font.freeTypeFace->units_per_EM;
    fontInfo.ascender = font.freeTypeFace->ascender;
    fontInfo.descender = font.freeTypeFace->descender;
    fontInfo.height = font.freeTypeFace->height;
    fontInfo.underlinePosition = font.freeTypeFace->underline_position;
    fontInfo.underlineThickness = font.freeTypeFace->underline_thickness;

    return fontInfo;
}

bool FontTable::setFontSize(FontID fontID, double fontSize) {
    auto & font = getFont(fontID);

    if (font.fontSize == fontSize) {
        return false;
    }

    if (FT_IS_SCALABLE(font.freeTypeFace)) {
        auto errorCode = FT_Set_Char_Size(font.freeTypeFace,
            0, fontSize * 64, 0, 0);

        FreeType::throwIfError(errorCode);
    } else {
        FT_Int sizeCount = font.freeTypeFace->num_fixed_sizes;
        FT_Int selectedIndex = 0;
        int16_t selectedSize = 0;

        for (FT_Int index = 0; index < sizeCount; index++) {
            auto strikeInfo = &font.freeTypeFace->available_sizes[index];
            int16_t candidateSize = strikeInfo->height;

            if (fontSize > candidateSize && candidateSize > selectedSize) {
                selectedIndex = index;
                selectedSize = candidateSize;
            }
        }

        auto errorCode = FT_Select_Size(font.freeTypeFace, selectedIndex);

        FreeType::throwIfError(errorCode);

        auto strikeInfo = &font.freeTypeFace->available_sizes[selectedIndex];

        if (strikeInfo->height) {
            font.bitmapScale = fontSize / strikeInfo->height;
        }
    }

    font.fontSize = fontSize;

    return true;
}

int32_t FontTable::fontUnitsToPixels(FontID fontID, int32_t value) {
    auto & font = getFont(fontID);

    return FT_MulFix(value, font.freeTypeFace->size->metrics.y_scale) / 64;
}


void FontTable::setFontAlternative(FontID fontID, FontID fallbackFontID) {
    alternativeMap[fontID] = fallbackFontID;
}

FontID FontTable::getFontAlternative(FontID fontID) {
    const auto & iter = alternativeMap.find(fontID);

    if (iter != alternativeMap.end()) {
        return iter->second;
    } else {
        return 0;
    }
}

}
