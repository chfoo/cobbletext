#include "internal/font/GlyphTable.hpp"

#include <limits>

#include "internal/FreeType.hpp"
#include "internal/Debug.hpp"
#include "Exception.hpp"
#include "internal/ExceptionUtil.hpp"

namespace cobbletext::internal {

GlyphTable::GlyphTable(std::shared_ptr<FreeType> freeType,
    std::shared_ptr<FontTable> fontTable) :
    freeType(freeType),
    fontTable(fontTable) {}

bool GlyphTable::registerGlyph(const GlyphKey & glyphKey) {
    const auto & result = glyphs.find(glyphKey);

    if (result != glyphs.end())  {
        return false;
    }

    GlyphID id = getFreeID();
    idMap.emplace(id, glyphKey);

    Glyph glyph(id);
    glyphs.emplace(glyphKey, glyph);

    return true;
}

bool GlyphTable::isRegistered(const GlyphKey & glyphKey) {
    const auto & result = glyphs.find(glyphKey);

    return result != glyphs.end();
}

const GlyphKey & GlyphTable::idToKey(GlyphID glyphID) {
    return idMap.at(glyphID);
}

GlyphID GlyphTable::keyToID(const GlyphKey & glyphKey) {
    return glyphs.at(glyphKey).id;
}

Glyph & GlyphTable::getGlyph(const GlyphKey & glyphKey) {
    auto & glyph = glyphs.at(glyphKey);

    return glyph;
}

void GlyphTable::rasterize(const GlyphKey & glyphKey) {
    auto & glyph = glyphs.at(glyphKey);

    if (glyph.hasImage) {
        return;
    }

    auto font = fontTable->getFont(glyphKey.fontFace);

    FT_Error errorCode = FT_Load_Glyph(font.freeTypeFace,
        glyphKey.index, FT_LOAD_DEFAULT);

    FreeType::throwIfError(errorCode);

    if (font.freeTypeFace->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
        errorCode = FT_Render_Glyph(font.freeTypeFace->glyph,
            FT_RENDER_MODE_NORMAL);

        FreeType::throwIfError(errorCode);
    }

    auto bitmap = font.freeTypeFace->glyph->bitmap;
    glyph.imageWidth = bitmap.width;
    glyph.imageHeight = bitmap.rows;
    auto buffer = static_cast<uint8_t *>(bitmap.buffer);
    glyph.image = std::vector<uint8_t>(buffer,
        buffer + bitmap.width * bitmap.rows);

    glyph.hasImage = true;
}

GlyphInfo GlyphTable::getGlyphInfo(GlyphID glyphID) {
    auto & glyphKey = idToKey(glyphID);
    auto & glyph = glyphs.at(glyphKey);
    GlyphInfo glyphInfo;

    rasterize(glyphKey);

    glyphInfo.id = glyph.id;
    glyphInfo.image = glyph.image;
    glyphInfo.imageHeight = glyph.imageHeight;
    glyphInfo.imageWidth = glyph.imageWidth;

    return glyphInfo;
}

void GlyphTable::clearRasterCache() {
    for (auto & item : glyphs) {
        item.second.image.clear();
        item.second.imageWidth = 0;
        item.second.imageHeight = 0;
        item.second.hasImage = false;
    }
}

GlyphID GlyphTable::getFreeID() {
    GlyphID id = idCounter;

    if (id == std::numeric_limits<GlyphID>::max()) {
        throw_with_trace(RuntimeError("max glyph id"));
    }

    idCounter++;
    return id;
}


}