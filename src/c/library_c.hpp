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
};

namespace cobbletext::c {

static void handleSuccess(struct CobbletextLibrary * library);

static void handleException(struct CobbletextLibrary * library,
    std::exception * exception);


}
