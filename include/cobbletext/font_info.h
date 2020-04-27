#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Font face properties.
 */
struct CobbletextFontInfo {
    /**
     * ID for this face.
     */
    CobbletextFontID id;

    /**
     * Font family name.
     *
     * - Never null pointer.
     */
    const char * family_name;

    /**
     * Font style name.
     *
     * - Never null pointer.
     */
    const char * style_name;

    /**
     * Number of font units per EM square.
     *
     * If value is 0, the font is a bitmap font and not a vector font.
     * Additionally, properties, such as ascender or height,
     * related to this EM size is not valid.
     */
    uint16_t units_per_em;

    /**
     * Ascender in font units.
     *
     * Positive is above baseline.
     */
    int16_t ascender;

    /**
     * Descencder in font units.
     *
     * Negative is below baseline.
     */
    int16_t descender;

    /**
     * Distance between two baselines in font units.
     *
     * If 0, it is not a vector, but a bitmap font.
     */
    int16_t height;

    /**
     * Position of underline in font units.
     */
    int16_t underline_position;

    /**
     * Thickness of underline in font units.
     */
    int16_t underline_thickness;
};

#ifdef __cplusplus
}
#endif
