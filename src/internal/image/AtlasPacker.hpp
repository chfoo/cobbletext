#pragma once

#include <vector>
#include <stdint.h>
#include <algorithm>

#include "TileInfo.hpp"
#include "internal/font/Glyph.hpp"
#include "internal/font/GlyphTable.hpp"

namespace cobbletext::internal {

class AtlasPackerSlot {
public:
    TileInfo & tileInfo;
    const Glyph & glyph;

    AtlasPackerSlot(TileInfo & tileInfo, const Glyph & glyph) :
        tileInfo(tileInfo), glyph(glyph) {}
};

class AtlasPacker {
    std::shared_ptr<GlyphTable> glyphTable;

public:
    explicit AtlasPacker(std::shared_ptr<GlyphTable> glyphTable);

    static bool glyphSortComparator(
        const std::unique_ptr<AtlasPackerSlot> & slot1,
        const std::unique_ptr<AtlasPackerSlot> & slot2);

    bool pack(std::vector<TileInfo> & tiles, uint32_t width, uint32_t height);

};

}
