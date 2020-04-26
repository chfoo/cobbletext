#include "Encoding.hpp"

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(cobbletext_Encoding) {
    enum_<cobbletext::Encoding>("Encoding")
        .value("UTF8", cobbletext::Encoding::UTF8)
        .value("UTF16LE", cobbletext::Encoding::UTF16LE)
        ;
}
