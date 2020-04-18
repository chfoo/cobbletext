#pragma once

#include <string>

#include "common.hpp"

namespace cobbletext {

class COBBLETEXT_API FontInfo {
public:
    FontID id;
    std::string familyName;
    std::string styleName;
    uint16_t unitsPerEM;
    int16_t ascender;
    int16_t descender;
    int16_t height;
    int16_t underlinePosition;
    int16_t underlineThickness;
};

}
