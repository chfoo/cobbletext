#pragma once

#include <unordered_map>
#include <vector>
#include <memory>

#include "internal/FreeType.hpp"
#include FT_BITMAP_H

#include "internal/font/GlyphKey.hpp"
#include "internal/font/Glyph.hpp"
#include "internal/font/FontTable.hpp"
#include "GlyphInfo.hpp"

namespace cobbletext::internal {

class GlyphTable {
private:
    std::shared_ptr<FreeType> freeType;
    std::shared_ptr<FontTable> fontTable;
    unsigned long int idCounter = 1;

    std::unordered_map<GlyphID,GlyphKey> idMap;
    std::unordered_map<GlyphKey,Glyph,GlyphKeyHasher> glyphs;

    FT_Bitmap tempBitmap;

public:
    GlyphTable(std::shared_ptr<FreeType> freeType, std::shared_ptr<FontTable> fontTable);
    ~GlyphTable();

    bool registerGlyph(const GlyphKey & glyphKey);

    bool isRegistered(const GlyphKey & glyphKey);

    const GlyphKey & idToKey(GlyphID glyphID);

    GlyphID keyToID(const GlyphKey & glyphKey);

    Glyph & getGlyph(const GlyphKey & glyphKey);

    void rasterize(const GlyphKey & glyphKey);

    GlyphInfo getGlyphInfo(GlyphID glyphID);

    void clear();

private:
    GlyphID getFreeID();

    void scaleBitmapGlyph(FT_Bitmap & bitmap, Glyph & glyph, Font & font);
    void scaleBitmapMono(FT_Bitmap & bitmap, Glyph & glyph, Font & font);
    void scaleBitmapGrayscale(FT_Bitmap & bitmap, Glyph & glyph, Font & font);


};

}
