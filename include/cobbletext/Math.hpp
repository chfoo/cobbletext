#pragma once

#include <stdint.h>

#include "macros.h"

namespace cobbletext {

class Math {
public:
    static uint32_t alpha_blend_over_argb(uint32_t background,
        uint32_t foreground);

    static uint32_t gamma_argb(uint32_t color, double gamma);
};

}
