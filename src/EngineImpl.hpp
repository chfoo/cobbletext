#pragma once

#include <memory>

#include <unicode/ustring.h>

#include "Engine.hpp"
#include "Library.hpp"
#include "internal/input/TextSource.hpp"
#include "internal/input/TextFormat.hpp"
#include "internal/Context.hpp"
#include "internal/Codec.hpp"
#include "internal/layout/LayoutEngine.hpp"

namespace cobbletext {

class Engine::Impl {
public:
    Engine & parent;
    std::shared_ptr<internal::Context> context;
    std::shared_ptr<internal::TextSource> textSource;
    std::shared_ptr<internal::LayoutEngine> layoutEngine;
    internal::Codec codec;


    Impl(Engine & parent, std::shared_ptr<internal::Context> context);

    internal::TextFormat makeCurrentTextFormat();

    void addUnicodeString(icu::UnicodeString string);

    void addInlineObject(InlineObjectID id, uint32_t size);

    void clear();

    void layOut();

    bool tilesValid();

    void rasterize();

    bool packTiles(uint32_t width, uint32_t height);
};

}
