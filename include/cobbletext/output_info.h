#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sizes for the rendered text.
 *
 * There are two types of bounding boxes:
 *
 * - Text bounding box
 * - Raster bounding box
 *
 * The text bounding box (or text box) only includes sizes computed by pen
 * movements.
 *
 * The raster bounding box (or image size) includes what is actually drawn
 * to the image. TODO
 */
struct CobbletextOutputInfo {
    /**
     * Width of the text box in pixels.
     */
    uint32_t text_width;

    /**
     * Height of the text box in pixels.
     */
    uint32_t text_height;
};

#ifdef __cplusplus
}
#endif
