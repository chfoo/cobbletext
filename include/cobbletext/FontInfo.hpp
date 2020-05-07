#pragma once

#include <string>
#include <ostream>

#include "common.hpp"
#include "macros.h"

namespace cobbletext {

class COBBLETEXT_CPP_API FontInfo {
public:
    FontID id = 0;
    std::string familyName;
    std::string styleName;
    uint16_t unitsPerEM = 0;
    int16_t ascender = 0;
    int16_t descender = 0;
    int16_t height = 0;
    int16_t underlinePosition = 0;
    int16_t underlineThickness = 0;

private:
    friend std::ostream & operator<<(std::ostream & stream,
        const FontInfo & fontInfo);
};

}
