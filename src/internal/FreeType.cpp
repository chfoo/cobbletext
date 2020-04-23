#include "internal/FreeType.hpp"

#include "Exception.hpp"
#include "internal/ExceptionUtil.hpp"
#include "internal/Debug.hpp"

namespace cobbletext::internal {

void FreeTypeLibraryDeleter::operator()(FT_Library library) const {
    if (library != nullptr) {
        FT_Done_FreeType(library);
    }
}

FreeType::FreeType() {
    FT_Library pointer;
    auto errorCode = FT_Init_FreeType(&pointer);

    throwIfError(errorCode);

    library.reset(pointer);

    COBBLETEXT_DEBUG_PRINT("freetype initialized");
}

LibraryError FreeType::makeError(FT_Error errorCode) {
#if FREETYPE_MAJOR * 100 + FREETYPE_MINOR < 210
    return LibraryError("freetype error " + std::to_string(errorCode), errorCode);
#else
    auto message = FT_Error_String(errorCode);

    if (message) {
        return LibraryError(FT_Error_String(errorCode), errorCode);
    } else {
        return LibraryError("freetype error " + std::to_string(errorCode), errorCode);
    }
#endif
}

void FreeType::throwIfError(FT_Error errorCode) {
    if (errorCode) {
        throw_with_trace(FreeType::makeError(errorCode));
    }
}

}
