#include "EngineImpl.hpp"

#include <unicode/uloc.h>

namespace cobbletext {

Engine::Impl::Impl(Engine & parent,
    std::shared_ptr<internal::Context> context) :
    parent(parent),
    context(context),
    textSource(std::make_shared<internal::TextSource>()),
    layoutEngine(std::make_shared<internal::LayoutEngine>(
        context, textSource)) {}

internal::TextFormat Engine::Impl::makeCurrentTextFormat() {
    internal::TextFormat textFormat;

    textFormat.customProperty = parent.customProperty;
    textFormat.fontFace = parent.font;
    textFormat.fontSize = parent.fontSize;
    textFormat.language = parent.language;
    textFormat.script = parent.script;
    textFormat.scriptDirection = parent.scriptDirection;

    return textFormat;
}

void Engine::Impl::addUnicodeString(icu::UnicodeString string) {
    textSource->addText(string, makeCurrentTextFormat());
}

void Engine::Impl::addInlineObject(InlineObjectID id, uint32_t size) {
    textSource->addInlineObject(id, size, makeCurrentTextFormat());
}

void Engine::Impl::clear() {
    textSource->clear();
    layoutEngine->clear();
    parent.outputInfo.textWidth = parent.outputInfo.textHeight = 0;
}

void Engine::Impl::layOut() {
    if (parent.locale != "") {
        textSource->locale = parent.locale.c_str();
    } else {
        textSource->locale = icu::Locale::getDefault();
    }
    layoutEngine->lineLength = parent.lineLength;
    layoutEngine->layOut();

    parent.outputInfo.textWidth = layoutEngine->textWidth();
    parent.outputInfo.textHeight = layoutEngine->textHeight();
}

bool Engine::Impl::tilesValid() {
    return layoutEngine->tilesValid();
}

void Engine::Impl::rasterize() {
    for (auto & tile : layoutEngine->tiles()) {
        const auto & glyphKey = context->glyphTable->idToKey(tile.glyphID);
        context->glyphTable->rasterize(glyphKey);
    }
}

bool Engine::Impl::packTiles(uint32_t width, uint32_t height) {
    return context->atlasPacker->pack(layoutEngine->tiles(), width, height);
}

}
