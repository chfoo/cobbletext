#include "ScriptDirection.hpp"

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(cobbletext_ScriptDirection) {
    enum_<cobbletext::ScriptDirection>("ScriptDirection")
        .value("NotSpecified", cobbletext::ScriptDirection::NotSpecified)
        .value("LTR", cobbletext::ScriptDirection::LTR)
        .value("RTL", cobbletext::ScriptDirection::RTL)
        ;
}
