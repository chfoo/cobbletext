#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Glyph properties
 */
struct CobbletextGlyphInfo {
    /**
     * ID for this glyph.
     */
    CobbletextGlyphID id;

    /**
     * Coverage map of the glyph.
     *
     * 0 is transparent, 255 is opaque.
     *
     * The value is null if the glyph has not yet been rasterized.
     *
     * - May be null
     */
    const uint8_t * image;

    /**
     * Width of the coverage map.
     *
     * - Value is 0 if no `#image`
     */
    uint32_t image_width;

    /**
     * Height of the coverage map.
     *
     * - Value is 0 if no `#image`
     */
    uint32_t image_height;

    /**
     * Horizontal offset when drawing the image.
     *
     * Additive to your pen position.
     */
    int32_t image_offset_x;

    /**
     * Vertical offset when drawing the image.
     *
     * Additive to your pen position.
     */
    int32_t image_offset_y;
};

#ifdef __cplusplus
}
#endif
