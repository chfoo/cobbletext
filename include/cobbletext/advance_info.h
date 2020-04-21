#pragma once

#include <stdint.h>

#include "common.h"

/**
 * Type of information contained in `#CobbletextAdvanceInfo`.
 *
 * One of:
 * - `#COBBLETEXT_ADVANCE_TYPE_INVALID`
 * - `#COBBLETEXT_ADVANCE_TYPE_GLYPH`
 * - `#COBBLETEXT_ADVANCE_TYPE_INLINE_OBJECT`
 * - `#COBBLETEXT_ADVANCE_TYPE_LINE_BREAK`
 * - `#COBBLETEXT_ADVANCE_TYPE_BIDI`
 * - `#COBBLETEXT_ADVANCE_TYPE_LAYOUT`
 */
typedef uint8_t CobbletextAdvanceType;

/**
 * This value shouldn't happen.
 */
#define COBBLETEXT_ADVANCE_TYPE_INVALID 0

/**
 * Draw a glyph.
 */
#define COBBLETEXT_ADVANCE_TYPE_GLYPH 1

/**
 * Draw an inline object.
 */
#define COBBLETEXT_ADVANCE_TYPE_INLINE_OBJECT 2

/**
 * Pen movement to position a new line.
 */
#define COBBLETEXT_ADVANCE_TYPE_LINE_BREAK 3

/**
 * Pen movement to position to draw bidirectional text.
 */
#define COBBLETEXT_ADVANCE_TYPE_BIDI 4

/**
 * General purpose pen movement.
 */
#define COBBLETEXT_ADVANCE_TYPE_LAYOUT 5

/**
 * Representation of a pen drawing instruction.
 *
 * The origin of the X-Y coordinate system is defined to be the top-left
 * of the drawing area where
 *
 * - Positive X values move to the right
 * - Positive Y values move to the bottom
 *
 * The advance values are to be added to the current pen position after
 * drawing a glyph or object (if any).
 */
struct CobbletextAdvanceInfo {
    /**
     * The type of movement.
     */
    CobbletextAdvanceType type;

    /**
     * Position of the text in code points.
     */
    uint32_t text_index;

    /**
     * Horizontal movement of the pen.
     */
    int32_t advance_x;

    /**
     * Vertical movement of the pen.
     */
    int32_t advance_y;

    /**
     * Glyph to be drawn.
     *
     * - Only valid when `#type` is a glyph.
     */
    CobbletextGlyphID glyph_id;

    /**
     * Horizontal offset to the current pen position when drawing a glyph.
     */
    int32_t glyph_offset_x;

    /**
     * Vertical offset to the current pen position when drawing a glyph.
     */
    int32_t glyph_offset_y;

    /**
     * User-provided inline object ID.
     *
     * - Only valid when `#type` is a inline object.
     */
    CobbletextInlineObjectID inline_object;

    /**
     * User-provided custom property.
     *
     * - Default is 0.
     */
    CobbletextCustomPropertyID custom_property;
};
