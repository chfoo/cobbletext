#pragma once

#include <exception>
#include <memory>
#include <exception>
#include <string>

#include "cobbletext.h"
#include "Library.hpp"

struct CobbletextLibrary {
    std::shared_ptr<cobbletext::Library> obj;
    int32_t errorCode;
    std::string errorMessage;
    std::unique_ptr<struct CobbletextFontInfo> fontInfo;
    std::unique_ptr<struct CobbletextGlyphInfo> glyphInfo;
    std::unique_ptr<uint8_t[]> glyphImage;
};

namespace cobbletext::c {



}
