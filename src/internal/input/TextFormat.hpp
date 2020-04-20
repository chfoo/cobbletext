#pragma once

#include <string>

#include "common.hpp"
#include "ScriptDirection.hpp"

namespace cobbletext::internal {

class TextFormat {
public:
    FontID fontFace = 0;
    double fontSize = 12;
    std::string language;
    std::string script;
    ScriptDirection scriptDirection = ScriptDirection::NotSpecified;
    CustomPropertyID customProperty = 0;
};

}
