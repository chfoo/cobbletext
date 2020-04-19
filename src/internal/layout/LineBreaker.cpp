#include "internal/layout/LineBreaker.hpp"

#include "internal/ICUError.hpp"
#include "internal/Debug.hpp"

namespace cobbletext::internal {

void LineBreaker::locale(const icu::Locale & locale) {
    if (lineBreaker && characterBreaker && breakerLocale == locale) {
        return;
    }
    breakerLocale = locale;

    remakeBreakers();
}

void LineBreaker::remakeBreakers() {
    ICUError errorCode;

    auto lineBreakerPointer = icu::BreakIterator::createLineInstance(
        breakerLocale, errorCode);
    lineBreaker.reset(lineBreakerPointer);

    if (errorCode.get()) {
        COBBLETEXT_DEBUG_PRINT("line breaker " << errorCode.get());
    }

    errorCode.reset();

    auto characterBreakerPointer = icu::BreakIterator::createCharacterInstance(
        breakerLocale, errorCode);
    characterBreaker.reset(characterBreakerPointer);

    if (errorCode.get()) {
        COBBLETEXT_DEBUG_PRINT("character breaker " << errorCode.get());
    }
}

void LineBreaker::setTextBuffer(
        std::shared_ptr<icu::UnicodeString> text) {
    this->text = text;
    stringIndexer.setTextBuffer(text);
}

std::vector<LineRun> LineBreaker::applyBreaks(const std::vector<ShapeResult> & shapeResults) {
    // TODO: this method is not finished!
    stringIndexer.reset();
    std::vector<LineRun> lines;

    LineRun line;

    auto shapeResultIterator = shapeResults.begin();

    while (shapeResultIterator != shapeResults.end()) {
        auto & shapeResult = *shapeResultIterator;
        uint32_t shapeLength;

        if (shapeResult.run.source.textFormat) {
            auto codePointIndex = shapeResult.cluster;
            auto codeUnitIndex =
                stringIndexer.codePointToCodeUnit(codePointIndex);
            // if glyph is mandatory line break, do break

            // TODO: make this support vertical lines
            shapeLength = shapeResult.xAdvance;
        } else if (shapeResult.run.source.inlineObject) {
            shapeLength = shapeResult.run.source.inlineObject->pixelSize;
        }

        if (lineLength > 0 && shapeLength + line.length >= lineLength) {
            // go backwards looking for soft breaks

            // if the line becomes empty, look for character to break at

        }

        line.length += shapeLength;

        line.shapeResults.push_back(std::ref(shapeResult));

        shapeResultIterator++;
    }

    lines.push_back(line);

    return lines;
}

}
