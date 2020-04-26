#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif
