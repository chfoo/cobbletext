#pragma once

#include <unordered_map>
#include <vector>

#include "internal/FreeType.hpp"
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

public:
    GlyphTable(std::shared_ptr<FreeType> freeType, std::shared_ptr<FontTable> fontTable);

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
};

}
