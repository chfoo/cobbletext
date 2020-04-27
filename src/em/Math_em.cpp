#include "Math.hpp"

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(cobbletext_Math) {
    class_<cobbletext::Math>("Math")

        .class_function("alpha_blend_over_argb", &cobbletext::Math::alpha_blend_over_argb)
        .class_function("gamma_correction_argb", &cobbletext::Math::gamma_argb)
        ;


}
