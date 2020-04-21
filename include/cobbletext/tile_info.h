#pragma once

#include "common.h"

/**
 * Representation of a rendered glyph for an `#CobbletextEngine`.
 */
struct CobbletextTileInfo {
    /**
     * The glyph ID represented.
     */
    CobbletextGlyphID glyph_id;

    /**
     * Horizontal offset within a texture atlas.
     *
     * - Value is 0 if tile packing is not performed.
     */
    uint32_t atlas_x;

    /**
     * Vertical offset within a texture atlas.
     *
     * - Value is 0 if tile packing is not performed.
     */
    uint32_t atlas_y;
};
