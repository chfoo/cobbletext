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
            << std::to_string(lineBreaker->getRuleStatus()) << " "
            << std::to_string(characterBreaker->isBoundary(index))
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

bool LineBreaker::isCharacterBreakableBefore(int32_t codePointIndex) {
    auto codeUnitIndex =
        stringIndexer.codePointToCodeUnit(codePointIndex);
    auto isBoundary = characterBreaker->isBoundary(codeUnitIndex);

    return isBoundary;
}

void LineBreaker::fillLine() {
    while (*shapeResultIterator != shapeResults->end()) {
        auto & shapeResult = **shapeResultIterator;
        uint32_t shapeLength;

        if (shapeResult.run.source.inlineObject) {
            shapeLength = shapeResult.run.source.inlineObject->pixelWidth;
        } else {
            // TODO: make this support vertical lines
            shapeLength = shapeResult.xAdvance;
        }

        bool breakRequired = isMandatoryLineBreakAfter(shapeResult.cluster);

        if (breakRequired) {
            // Skip over the newline character. It's not always 0 size and
            // visually empty.
            currentLine.lineBreakShapeResult.emplace(shapeResult);
            ++*shapeResultIterator;
            break;
        }

        currentLine.shapeResults.push_back(shapeResult);
        currentLine.totalAdvance += shapeLength;
        ++*shapeResultIterator;

        bool hasOverflown = lineLength
            && currentLine.totalAdvance >= lineLength;

        if (hasOverflown) {
            rewindLine();
            break;
        }
    }
}

void LineBreaker::rewindLine() {
    while (currentLine.shapeResults.size()) {
        auto & shapeResult = currentLine.shapeResults.back().get();
        currentLine.shapeResults.pop_back();

        uint32_t shapeLength = getShapeLength(shapeResult);

        currentLine.totalAdvance -= shapeLength;
        --*shapeResultIterator;

        auto codePointIndex = shapeResult.cluster;
        auto codeUnitIndex = stringIndexer.codePointToCodeUnit(codePointIndex);
        auto canBreakBeforeChar = lineBreaker->isBoundary(codeUnitIndex);

        if (canBreakBeforeChar) {
            break;
        }
    }

    if (currentLine.shapeResults.empty()) {
        emergencyFillLine();
    }
}

void LineBreaker::emergencyFillLine() {
    while (*shapeResultIterator != shapeResults->end()) {
        auto & shapeResult = **shapeResultIterator;
        uint32_t shapeLength = getShapeLength(shapeResult);
        bool isBreakable = isCharacterBreakableBefore(shapeResult.cluster);
        bool isOverflow = currentLine.totalAdvance + shapeLength >= lineLength;

        if (!currentLine.shapeResults.empty() && isOverflow && isBreakable) {
            break;
        }

        currentLine.shapeResults.push_back(shapeResult);
        currentLine.totalAdvance += shapeLength;
        ++*shapeResultIterator;
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

        auto scaleFactor = 1 / 64.0; // FreeType pixel units
        if (font.bitmapScale) {
            // Not a vector font, but a bitmap font. Scale the bitmap
            // to fit size.
            scaleFactor *= font.bitmapScale;
        }

        auto fontPixelHeight =
            font.freeTypeFace->size->metrics.height * scaleFactor;

        currentLine.lineHeight =
            std::max<uint32_t>(currentLine.lineHeight, fontPixelHeight);

        // Positive above the baseline
        if (font.freeTypeFace->size->metrics.ascender > 0) {
            auto fontPixelAscent =
                font.freeTypeFace->size->metrics.ascender * scaleFactor;

            currentLine.ascent =
                std::max<uint32_t>(currentLine.ascent, fontPixelAscent);
        }

        // Negative below the baseline
        if (font.freeTypeFace->size->metrics.descender < 0) {
            auto fontPixelAscent =
                font.freeTypeFace->size->metrics.descender * scaleFactor;

            currentLine.descent =
                std::max<uint32_t>(currentLine.descent, -fontPixelAscent);
        }
    }
}

uint32_t LineBreaker::getShapeLength(const ShapeResult & shapeResult) {
    if (shapeResult.run.source.inlineObject) {
        return shapeResult.run.source.inlineObject->pixelWidth;
    } else {
        // TODO: Support vertical lines
        return shapeResult.xAdvance;
    }
}

}
