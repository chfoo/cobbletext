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

FontID FontTable::load(const char * path) {
    auto library = freeType->library.get();
    FT_Face fontFace;

    auto errorCode = FT_New_Face(library, path, 0, &fontFace);

    FreeType::throwIfError(errorCode);

    auto hbFont = hb_ft_font_create_referenced(fontFace);
    auto id = getFreeID();

    fonts.emplace(id, Font(fontFace, hbFont));

    return id;
}

FontID FontTable::loadBytes(const gsl::span<const uint8_t> & data) {
    auto library = freeType->library.get();
    FT_Face fontFace;

    auto errorCode = FT_New_Memory_Face(library,
        (const FT_Byte *)data.data(), data.size(), 0, &fontFace);

    FreeType::throwIfError(errorCode);

    auto hbFont = hb_ft_font_create_referenced(fontFace);
    auto id = getFreeID();

    fonts.emplace(id, Font(fontFace, hbFont));

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

Font & FontTable::getFont(FontID fontID) {
    try {
        return fonts.at(fontID);
    } catch (const std::out_of_range & exception) {
        throw_with_trace(
            LogicError("font not in table: " + std::to_string(fontID)));
    }
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

}
