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
    lines.clear();
    currentLine.clear();

#ifdef COBBLETEXT_DEBUG
    for (int32_t index = 0; index < text->length(); index++) {
        std::string character;
        auto subString = text->tempSubString(index, 1);
        subString.toUTF8String(character);

        COBBLETEXT_DEBUG_PRINT(index << " "
            << character << " "
            << std::to_string(lineBreaker->isBoundary(index)) << " "
            << std::to_string(lineBreaker->getRuleStatus())
        );
    }
#endif

    while (*shapeResultIterator != shapeResults->end()) {
        fillLine();
        analyzeLineHeight();
        pushCurrentLine();
    }

    return lines;
}

void LineBreaker::pushCurrentLine() {
    lines.push_back(currentLine);

    currentLine.clear();
}

bool LineBreaker::isMandatoryLineBreakAfter(int32_t codePointIndex) {
    auto codeUnitIndex =
        stringIndexer.codePointToCodeUnit(codePointIndex + 1);
    auto isBoundary = lineBreaker->isBoundary(codeUnitIndex);

    if (isBoundary) {
        auto rule = lineBreaker->getRuleStatus();
        return rule == ULineBreakTag::UBRK_LINE_HARD;
    } else {
        return false;
    }
}

void LineBreaker::fillLine() {
    while (*shapeResultIterator != shapeResults->end()) {
        auto & shapeResult = **shapeResultIterator;
        uint32_t shapeLength;

        if (shapeResult.run.source.inlineObject) {
            shapeLength = shapeResult.run.source.inlineObject->pixelSize;
        } else {
            // TODO: make this support vertical lines
            shapeLength = shapeResult.xAdvance;
        }

        bool breakRequired = isMandatoryLineBreakAfter(shapeResult.cluster);

        if (!breakRequired) {
            currentLine.shapeResults.push_back(shapeResult);
            currentLine.totalAdvance += shapeLength;
            ++*shapeResultIterator;
        } else {
            // Skip over the newline character. It's not always 0 size and
            // visually empty.
            currentLine.lineBreakShapeResult.emplace(shapeResult);
            ++*shapeResultIterator;
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

    if (currentLine.shapeResults.empty() && currentLine.lineBreakShapeResult) {
        const auto & shapeResult = *currentLine.lineBreakShapeResult;
        auto fontFace = shapeResult.run.source.textFormat.fontFace;
        auto fontSize = shapeResult.run.source.textFormat.fontSize;
        fontIDs.push_back(fontFace);
        fontSizes.push_back(fontSize);
    }

    for (size_t index = 0; index < fontIDs.size(); index++) {
        auto fontID = fontIDs[index];
        auto fontSize = fontSizes[index];

        if (!context->fontTable->hasFont(fontID)) {
            fontID = context->fontTable->fallbackFont;
        }

        auto & font = context->fontTable->getFont(fontID);

        context->fontTable->setFontSize(fontID, fontSize);

        auto fontPixelHeight = context->fontTable->fontUnitsToPixels(
            fontID, font.freeTypeFace->height);

        currentLine.lineHeight =
            std::max<uint32_t>(currentLine.lineHeight, fontPixelHeight);

        // Positive above the baseline
        if (font.freeTypeFace->ascender > 0) {
            auto fontPixelAscent = context->fontTable->fontUnitsToPixels(
                fontID, font.freeTypeFace->ascender);

            currentLine.ascent =
                std::max<uint32_t>(currentLine.ascent, fontPixelAscent);
        }

        // Negative below the baseline
        if (font.freeTypeFace->descender < 0) {
            auto fontPixelAscent = context->fontTable->fontUnitsToPixels(
                fontID, font.freeTypeFace->descender);

            currentLine.descent =
                std::max<uint32_t>(currentLine.descent, -fontPixelAscent);
        }
    }
}


}
