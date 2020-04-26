#include "TextAlignment.hpp"

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(cobbletext_TextAlignment) {
    enum_<cobbletext::TextAlignment>("TextAlignment")
        .value("NotSpecified", cobbletext::TextAlignment::NotSpecified)
        .value("Start", cobbletext::TextAlignment::Start)
        .value("End", cobbletext::TextAlignment::End)
        .value("Left", cobbletext::TextAlignment::Left)
        .value("Right", cobbletext::TextAlignment::Right)
        .value("Center", cobbletext::TextAlignment::Center)
        ;
}
