#pragma once

#include <memory>

#include "internal/FreeType.hpp"
#include "internal/font/FontTable.hpp"
#include "internal/font/GlyphTable.hpp"
#include "internal/image/AtlasPacker.hpp"

namespace cobbletext::internal {

class Context {
public:
    std::shared_ptr<FreeType> freeType;
    std::shared_ptr<FontTable> fontTable;
    std::shared_ptr<GlyphTable> glyphTable;
    std::shared_ptr<AtlasPacker> atlasPacker;

    Context() :
        freeType(std::make_shared<FreeType>()),
        fontTable(std::make_shared<FontTable>(freeType)),
        glyphTable(std::make_shared<GlyphTable>(freeType, fontTable)),
        atlasPacker(std::make_shared<AtlasPacker>(glyphTable)) {}
};

}
