#include "OutputInfo.hpp"

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(cobbletext_OutputInfo) {

    value_object<cobbletext::OutputInfo>("OutputInfo")
        .field("textWidth", &cobbletext::OutputInfo::textWidth)
        .field("textHeight", &cobbletext::OutputInfo::textHeight)
        ;

}
