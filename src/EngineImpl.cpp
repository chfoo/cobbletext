#include "EngineImpl.hpp"

#include <unicode/uloc.h>

#include "internal/input/TextFormat.hpp"

namespace cobbletext {

Engine::Impl::Impl(Engine & parent,
    std::shared_ptr<internal::Context> context) :
    parent(parent),
    context(context),
    textSource(std::make_shared<internal::TextSource>()),
    layoutEngine(std::make_shared<internal::LayoutEngine>(
        context, textSource)) {}

void Engine::Impl::addUnicodeString(icu::UnicodeString string) {
    internal::TextFormat textFormat;

    textFormat.customProperty = parent.customProperty;
    textFormat.fontFace = parent.font;
    textFormat.fontSize = parent.fontSize;
    textFormat.language = parent.language;
    textFormat.script = parent.script;
    textFormat.scriptDirection = parent.scriptDirection;

    textSource->addText(string, textFormat);
}

void Engine::Impl::addInlineObject(InlineObjectID id, uint32_t size) {
    textSource->addInlineObject(id, size);
}

void Engine::Impl::clear() {
    textSource->clear();
    layoutEngine->clear();
    parent.imageHeight = parent.imageWidth = 0;
}

void Engine::Impl::layOut() {
    if (parent.locale != "") {
        textSource->locale = parent.locale.c_str();
    } else {
        textSource->locale = icu::Locale::getDefault();
    }
    layoutEngine->lineLength = parent.lineLength;
    layoutEngine->layOut();
}

bool Engine::Impl::tilesValid() {
    return layoutEngine->tilesValid();
}

void Engine::Impl::rasterize() {
    for (auto & tile : layoutEngine->tiles) {
        const auto & glyphKey = context->glyphTable->idToKey(tile.glyphID);
        context->glyphTable->rasterize(glyphKey);
    }
}

bool Engine::Impl::packTiles(uint32_t width, uint32_t height) {
    return context->atlasPacker->pack(layoutEngine->tiles, width, height);
}

}
