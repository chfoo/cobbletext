#include "TileInfo.hpp"

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(cobbletext_TileInfo) {
    value_object<cobbletext::TileInfo>("TileInfo")
        .field("glyphID", &cobbletext::TileInfo::glyphID)
        .field("atlasX", &cobbletext::TileInfo::atlasX)
        .field("atlasY", &cobbletext::TileInfo::atlasY)
        ;

}
