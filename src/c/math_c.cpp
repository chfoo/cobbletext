#include "cobbletext.h"

#include "Math.hpp"

uint32_t cobbletext_math_alpha_blend_over_argb(uint32_t background,
        uint32_t foreground) {
    return cobbletext::Math::alpha_blend_over_argb(background, foreground);
}


uint32_t cobbletext_math_gamma_correction_argb(uint32_t color, double gamma) {
    return cobbletext::Math::gamma_correction_argb(color, gamma);
}

