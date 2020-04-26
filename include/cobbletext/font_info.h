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
