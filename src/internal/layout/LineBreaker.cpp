#include "internal/layout/LineBreaker.hpp"

#include <algorithm>

#include "internal/ICUError.hpp"
#include "internal/Debug.hpp"

namespace cobbletext::internal {

LineBreaker::LineBreaker(std::shared_ptr<Context> context) :
    context(context) {}

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

std::vector<LineRun> LineBreaker::applyBreaks(
        std::shared_ptr<std::vector<ShapeResult>> shapeResults) {

    stringIndexer.reset();
    lineBreaker->setText(*text);
    characterBreaker->setText(*text);
    this->shapeResults = shapeResults;
    shapeResultIterator.emplace(shapeResults->begin());
    currentLine = LineRun();

    while (*shapeResultIterator != shapeResults->end()) {
        fillLine();
        analyzeLineHeight();
        pushCurrentLine();
    }

    return lines;
}

void LineBreaker::pushCurrentLine() {
    lines.push_back(currentLine);

    currentLine = LineRun();
}

void LineBreaker::fillLine() {
    while (*shapeResultIterator != shapeResults->end()) {
        auto & shapeResult = **shapeResultIterator;
        uint32_t shapeLength;
        bool breakRequired = false;

        if (shapeResult.run.source.inlineObject) {
            shapeLength = shapeResult.run.source.inlineObject->pixelSize;
        } else {
            auto codePointIndex = shapeResult.cluster;
            auto codeUnitIndex =
                stringIndexer.codePointToCodeUnit(codePointIndex);

            auto isBoundary = lineBreaker->isBoundary(codeUnitIndex);
            auto rule = lineBreaker->getRuleStatus();

            // TODO: make this support vertical lines
            shapeLength = shapeResult.xAdvance;

            if (rule == ULineBreakTag::UBRK_LINE_HARD) {
                breakRequired = true;
            }
        }

        currentLine.shapeResults.push_back(shapeResult);
        currentLine.totalAdvance += shapeLength;

        ++*shapeResultIterator;

        if (breakRequired) {
            break;
        }

        // TODO: handle line overflow
        // if (lineLength && shapeLength + currentLine.advanceWidth >= lineLength) {
        //     rewindLine();

        //     if (currentLine.shapeResults.empty()) {
        //         fillLineWithOneChar();
        //     }
        // }
    }
}

void LineBreaker::rewindLine() {
    while (currentLine.shapeResults.size()) {
        auto & shapeResult = currentLine.shapeResults.back().get();
        currentLine.shapeResults.pop_back();
        --*shapeResultIterator;

        auto codePointIndex = shapeResult.cluster;
        auto codeUnitIndex = stringIndexer.codePointToCodeUnit(codePointIndex);
        auto isBoundary = lineBreaker->isBoundary(codeUnitIndex);

        if (isBoundary) {
            break;
        }
    }
}

void LineBreaker::fillLineWithOneChar() {
    while (*shapeResultIterator != shapeResults->end()) {
        auto & shapeResult = **shapeResultIterator;
        uint32_t shapeLength;

        if (shapeResult.run.source.inlineObject) {
            shapeLength = shapeResult.run.source.inlineObject->pixelSize;
        } else {
            shapeLength = shapeResult.xAdvance;
        }

        auto codePointIndex = shapeResult.cluster;
        auto codeUnitIndex =
            stringIndexer.codePointToCodeUnit(codePointIndex);
        auto hasAtLastOneChar = !currentLine.shapeResults.empty();
        auto isBoundary = characterBreaker->isBoundary(codeUnitIndex);

        currentLine.shapeResults.push_back(shapeResult);
        // TODO: make this support vertical lines
        currentLine.totalAdvance += shapeLength;
        ++*shapeResultIterator;

        if (hasAtLastOneChar && isBoundary) {
            break;
        }
    }
}

void LineBreaker::analyzeLineHeight() {
    // TODO: make this support vertical lines
    std::vector<FontID> fontIDs;
    std::vector<double> fontSizes;

    for (const auto & shapeResultRef : currentLine.shapeResults) {
        const auto & shapeResult = shapeResultRef.get();
        auto fontFace = shapeResult.run.source.textFormat.fontFace;
        auto fontSize = shapeResult.run.source.textFormat.fontSize;

        if (fontIDs.empty() || fontIDs.back() != fontFace
        || fontSizes.back() != fontSize) {
            fontIDs.push_back(fontFace);
            fontSizes.push_back(fontSize);
        }
    }

    for (size_t index = 0; index < fontIDs.size(); index++) {
        auto fontID = fontIDs[index];
        auto fontSize = fontSizes[index];

        if (!context->fontTable->hasFont(fontID)) {
            fontID = context->fontTable->fallbackFont;
        }

        auto & font = context->fontTable->getFont(fontID);

        context->fontTable->setFontSize(fontID, fontSize);

        currentLine.lineHeight = std::max<uint32_t>(
            currentLine.lineHeight,
            context->fontTable->fontUnitsToPixels(fontID,
                font.freeTypeFace->height)
        );
    }
}


}
