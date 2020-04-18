#include "internal/image/AtlasPacker.hpp"

#include <algorithm>
#include <functional>

#include "internal/Debug.hpp"

namespace cobbletext::internal {

AtlasPacker::AtlasPacker(std::shared_ptr<GlyphTable> glyphTable) :
    glyphTable(glyphTable) {}

bool AtlasPacker::glyphSortComparator(
        const std::unique_ptr<AtlasPackerSlot> & slot1,
        const std::unique_ptr<AtlasPackerSlot> & slot2) {
    return slot1->glyph.imageHeight > slot2->glyph.imageHeight;
}

bool AtlasPacker::pack(std::vector<TileInfo> & tiles, uint32_t width, uint32_t height) {
    // This packs glyphs into rows starting with largest height to smallest.

    std::vector<std::unique_ptr<AtlasPackerSlot>> slots;

    for (auto & tile : tiles) {
        const auto & key = glyphTable->idToKey(tile.glyphID);
        const auto & glyph = glyphTable->getGlyph(key);
        slots.push_back(std::make_unique<AtlasPackerSlot>(tile, glyph));
    }

    std::sort(slots.begin(), slots.end(), glyphSortComparator);

    uint32_t currentX = 0;
    uint32_t currentY = 0;
    uint32_t currentRowHeight = 0;

    for (auto & slot : slots) {
        if (currentX + slot->glyph.imageWidth > width) {
            if (currentRowHeight == 0) {
                // The glyph is too wide.
                return true;
            }

            // Move to a new row.
            currentX = 0;
            currentY += currentRowHeight;
            currentRowHeight = 0;
        }

        slot->tileInfo.atlasX = currentX;
        slot->tileInfo.atlasY = currentY;
        currentRowHeight = std::max(currentRowHeight, slot->glyph.imageHeight);
        currentX += slot->glyph.imageWidth;

        if (currentY + currentRowHeight > height) {
            return true;
        }
    }

    COBBLETEXT_DEBUG_PRINT("Packed atlas. Pen="
        << currentX << " " << currentY);

    return false;
}

}
