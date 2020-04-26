#pragma once

#include <stdint.h>

#include "macros.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Composite two ARGB color values using alpha blending and the
 * "over" operator.
 */

COBBLETEXT_API
uint32_t cobbletext_math_alpha_blend_over_argb(uint32_t background,
    uint32_t foreground);

/**
 * Apply gamma function to the ARGB color value.
 *
 * To apply gamma correction function, use `1 / gamma`.
 */
COBBLETEXT_API
uint32_t cobbletext_math_gamma_argb(uint32_t color, double gamma);

#ifdef __cplusplus
}
#endif
