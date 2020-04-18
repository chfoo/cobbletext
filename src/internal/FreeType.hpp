#pragma once

#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Exception.hpp"

namespace cobbletext::internal {

class FreeTypeLibraryDeleter {
public:
    void operator()(FT_Library library) const;
};

class FreeType {
public:

    // FreeType has typedef `struct FT_LibraryRec_ *` as `FT_Library`
    // which is annoying
    std::unique_ptr<struct FT_LibraryRec_,FreeTypeLibraryDeleter> library;

    FreeType();

    static LibraryError makeError(FT_Error errorCode);
    static void throwIfError(FT_Error errorCode);
};

}
