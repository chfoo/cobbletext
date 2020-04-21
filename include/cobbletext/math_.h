#pragma once

#include <stdint.h>

#include "macros.h"

COBBLETEXT_API
uint32_t cobbletext_math_alpha_blend_over_argb(uint32_t background,
    uint32_t foreground);

COBBLETEXT_API
uint32_t cobbletext_math_gamma_correction_argb(uint32_t color, double gamma);
