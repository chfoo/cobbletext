#include "internal/font/GlyphTable.hpp"

#include <limits>

#include "internal/FreeType.hpp"
#include "internal/Debug.hpp"
#include "Exception.hpp"
#include "internal/ExceptionUtil.hpp"
#include "internal/image/ImageResize.hpp"

namespace cobbletext::internal {

GlyphTable::GlyphTable(std::shared_ptr<FreeType> freeType,
    std::shared_ptr<FontTable> fontTable) :
    freeType(freeType),
    fontTable(fontTable) {

    FT_Bitmap_Init(&tempBitmap);
}

GlyphTable::~GlyphTable() {
    FT_Bitmap_Done(freeType->library.get(), &tempBitmap);
}

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

bool GlyphTable::removeGlyph(const GlyphKey & glyphKey) {
    const auto & result = glyphs.find(glyphKey);

    if (result == glyphs.end())  {
        return false;
    }

    freeIDList.push_back(result->second.id);

    idMap.erase(result->second.id);
    glyphs.erase(result);

    return true;
}

bool GlyphTable::isRegistered(const GlyphKey & glyphKey) {
    const auto & result = glyphs.find(glyphKey);

    return result != glyphs.end();
}

void GlyphTable::incrementReference(const GlyphKey & glyphKey) {
    auto & result = glyphs.at(glyphKey);
    result.referenceCount += 1;
}

void GlyphTable::decrementReference(const GlyphKey & glyphKey) {
    auto & result = glyphs.at(glyphKey);
    result.referenceCount -= 1;

    if (result.referenceCount == 0) {
        removeGlyph(glyphKey);
    }
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

    fontTable->setFontSize(glyphKey.fontFace,
        glyphKey.fontSize);

    auto errorCode = FT_Load_Glyph(font.freeTypeFace,
        glyphKey.index, FT_LOAD_DEFAULT);

    FreeType::throwIfError(errorCode);

    if (font.freeTypeFace->glyph->format != FT_GLYPH_FORMAT_BITMAP) {
        errorCode = FT_Render_Glyph(font.freeTypeFace->glyph,
            FT_RENDER_MODE_NORMAL);

        FreeType::throwIfError(errorCode);
    }

    auto & bitmap = font.freeTypeFace->glyph->bitmap;

    if (!font.bitmapScale) {
        // vector font
        glyph.imageWidth = bitmap.width;
        glyph.imageHeight = bitmap.rows;

        auto buffer = static_cast<uint8_t *>(bitmap.buffer);

        glyph.image = std::vector<uint8_t>(buffer,
            buffer + bitmap.width * bitmap.rows);

        glyph.imageOffsetX = font.freeTypeFace->glyph->bitmap_left;
        glyph.imageOffsetY = -font.freeTypeFace->glyph->bitmap_top;
    } else {
        scaleBitmapGlyph(bitmap, glyph, font);
    }

    glyph.hasImage = true;
}

void GlyphTable::scaleBitmapGlyph(FT_Bitmap & bitmap, Glyph & glyph, Font & font) {
    switch (bitmap.pixel_mode) {
        case FT_PIXEL_MODE_MONO:
            scaleBitmapMono(bitmap, glyph, font);
            break;
        case FT_PIXEL_MODE_GRAY:
            scaleBitmapGrayscale(bitmap, glyph, font);
            break;
        default:
            COBBLETEXT_DEBUG_PRINT(
                "unknown bitmap pixel_mode=" << bitmap.pixel_mode);
    }
}

void GlyphTable::scaleBitmapMono(FT_Bitmap & bitmap, Glyph & glyph,
        Font & font) {
    auto errorCode = FT_Bitmap_Convert(freeType->library.get(),
        &bitmap, &tempBitmap, 1);

    FreeType::throwIfError(errorCode);

    if (tempBitmap.num_grays != 256) {
        // FT_Bitmap_Convert doesn't ramp the values
        size_t size = bitmap.width * bitmap.rows;

        for (size_t index = 0; index < size; index++) {
            tempBitmap.buffer[index] = tempBitmap.buffer[index] ? 255 : 0;
        }
    }

    scaleBitmapGrayscale(tempBitmap, glyph, font);
}

void GlyphTable::scaleBitmapGrayscale(FT_Bitmap & bitmap, Glyph & glyph,
        Font & font) {
    uint32_t scaledWidth = bitmap.width * font.bitmapScale;
    uint32_t scaledHeight = bitmap.rows * font.bitmapScale;

    glyph.imageWidth = scaledWidth;
    glyph.imageHeight = scaledHeight;

    glyph.image = std::vector<uint8_t>(scaledWidth * scaledHeight, 0);

    auto buffer = static_cast<uint8_t *>(bitmap.buffer);

    stbir_resize_uint8_linear(buffer, bitmap.width, bitmap.rows, 0,
        reinterpret_cast<unsigned char *>(glyph.image.data()),
        scaledWidth, scaledHeight, 0, static_cast<stbir_pixel_layout>(1));

    glyph.imageOffsetX = font.freeTypeFace->glyph->bitmap_left
        * font.bitmapScale;
    glyph.imageOffsetY = -font.freeTypeFace->glyph->bitmap_top
        * font.bitmapScale;
}

GlyphInfo GlyphTable::getGlyphInfo(GlyphID glyphID) {
    if (idMap.find(glyphID) == idMap.end()) {
        throw_with_trace(
            LogicError("glyph not in table: " + std::to_string(glyphID)));
    }

    auto & glyphKey = idToKey(glyphID);
    auto & glyph = glyphs.at(glyphKey);
    GlyphInfo glyphInfo;

    rasterize(glyphKey);

    glyphInfo.id = glyph.id;
    glyphInfo.image = glyph.image;
    glyphInfo.imageHeight = glyph.imageHeight;
    glyphInfo.imageWidth = glyph.imageWidth;
    glyphInfo.imageOffsetX = glyph.imageOffsetX;
    glyphInfo.imageOffsetY = glyph.imageOffsetY;

    return glyphInfo;
}

GlyphID GlyphTable::getFreeID() {
    if (!freeIDList.empty()) {
        GlyphID id = freeIDList.back();
        freeIDList.pop_back();
        return id;
    }

    GlyphID id = idCounter;

    if (id == std::numeric_limits<GlyphID>::max()) {
        throw_with_trace(RuntimeError("max glyph id"));
    }

    idCounter++;
    return id;
}

}
