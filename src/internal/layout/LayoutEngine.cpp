#include "internal/layout/LayoutEngine.hpp"

#include <cassert>
#include <unordered_set>

#include <unicode/schriter.h>

#include "internal/ICUError.hpp"
#include "internal/Debug.hpp"
#include "ScriptDirection.hpp"

namespace cobbletext::internal {


LayoutEngine::LayoutEngine(std::shared_ptr<Context> context,
        std::shared_ptr<TextSource> textSource) :
        context(context),
        textSource(textSource),
        shaper(context->fontTable) {
    bidiTable.setTextBuffer(textSource->textBuffer);
    scriptTable.setTextBuffer(textSource->textBuffer);
    shaper.setTextBuffer(textSource->textBuffer);
    lineBreaker.setTextBuffer(textSource->textBuffer);
}

bool LayoutEngine::tilesValid() {
    return tilesValid_;
}

void LayoutEngine::clear() {
    textSource->clear();
    // TODO:
}

void LayoutEngine::layOut() {
    tilesValid_ = true;

    ICUError errorCode;

    defaultDirection = textSource->locale.isRightToLeft() ?
        ScriptDirection::RTL : ScriptDirection::LTR;
    defaultDirectionHB = textSource->locale.isRightToLeft() ?
        HB_DIRECTION_RTL : HB_DIRECTION_LTR;
    defaultLanguageHB = ScriptTable::languageToHarfBuzz(
        textSource->locale.toLanguageTag<std::string>(errorCode));

    if (errorCode.get()) {
        COBBLETEXT_DEBUG_PRINT("default language " << errorCode.get());
    }

    bidiTable.analyze(defaultDirection);
    createInternalRuns();
    shapeResults = shaper.shapeRuns(internalRuns);
    registerGlyphsAndMakeTiles();
    lineBreaker.setTextBuffer(textSource->textBuffer);
    lineBreaker.locale(textSource->locale);
    lineBreaker.lineLength = lineLength;
    auto lines = lineBreaker.applyBreaks(shapeResults);
    makeAdvances(lines);
}

void LayoutEngine::createInternalRuns() {
    internalRuns.clear();

    for (auto const & textRun : textSource->runs) {
        if (textRun.textFormat) {
            processTextRun(textRun);
        } else if (textRun.inlineObject) {
            processInlineObject(textRun);
        }
    }
}

void LayoutEngine::processInlineObject(const TextRun & textRun) {
    InternalTextRun internalRun(textRun);

    internalRun.direction = defaultDirectionHB;
    internalRuns.push_back(internalRun);
}

void LayoutEngine::processTextRun(const TextRun & textRun) {
    if (textRun.textLength <= 0) {
        return;

    } else if (textRun.textFormat->script != ""
    && textRun.textFormat->scriptDirection != ScriptDirection::NotSpecified
    && textRun.textFormat->language != "") {
        fastPathTextRun(textRun);
        return;
    }

    auto endIndex = textRun.textIndex + textRun.textLength;
    auto iterator = icu::StringCharacterIterator(*textSource->textBuffer);
    iterator.setIndex(textRun.textIndex);

    ScriptDirection runDirection = bidiTable.getDirection(textRun.textIndex);
    hb_script_t runScript = scriptTable.getHarfBuzzScript(textRun.textIndex);

    int32_t runStartIndex = textRun.textIndex;

    while (iterator.getIndex() <= endIndex) {
        auto index = iterator.getIndex();
        bool shouldMakeNewRun = false;
        ScriptDirection codeUnitDirection;
        hb_script_t codeUnitScript;

        if (index < endIndex) {
            codeUnitDirection = bidiTable.getDirection(index);
            codeUnitScript = scriptTable.getHarfBuzzScript(index);

            bool isScriptDifferent = codeUnitScript != runScript
                && codeUnitScript != HB_SCRIPT_COMMON;
            bool isDirectionDifferent = codeUnitDirection != runDirection;

            shouldMakeNewRun = isScriptDifferent || isDirectionDifferent;
        } else {
            shouldMakeNewRun = true;
        }

        if (shouldMakeNewRun) {
            auto runLength = index - runStartIndex;
            InternalTextRun internalRun(textRun, runStartIndex, runLength);

            internalRun.direction = BidiTable::directionToHarfBuzz(
                runDirection);
            internalRun.script = runScript;

            if (textRun.textFormat->language == "") {
                internalRun.language = defaultLanguageHB;
            } else {
                internalRun.language = ScriptTable::languageToHarfBuzz(
                    textRun.textFormat->language);
            }

            assert(internalRun.direction != HB_DIRECTION_INVALID);
            assert(internalRun.language != HB_LANGUAGE_INVALID);
            assert(internalRun.script != HB_SCRIPT_INVALID);
            internalRuns.push_back(internalRun);

            runStartIndex = index;
            runDirection = codeUnitDirection;
            runScript = codeUnitScript;
        }

        if (index < endIndex) {
            iterator.next();
        } else {
            break;
        }
    }
}

void LayoutEngine::fastPathTextRun(const TextRun & textRun) {
    InternalTextRun internalRun(textRun);

    internalRun.direction = BidiTable::directionToHarfBuzz(textRun.textFormat->scriptDirection);
    internalRun.script = ScriptTable::scriptToHarfBuzz(textRun.textFormat->script);
    internalRun.language = ScriptTable::languageToHarfBuzz(textRun.textFormat->language);

    internalRuns.push_back(internalRun);
}

void LayoutEngine::registerGlyphsAndMakeTiles() {
    tiles.clear();
    std::unordered_set<GlyphKey,GlyphKeyHasher> glyphsSeen;

    for (auto const & shapeResult : shapeResults) {
        if (shapeResult.run.source.textFormat) {
            auto const & textFormat = *shapeResult.run.source.textFormat;
            auto key = GlyphKey(textFormat.fontFace, textFormat.fontSize,
                shapeResult.glyphIndex);

            if (glyphsSeen.find(key) != glyphsSeen.end()) {
                continue;
            }

            bool isNew = context->glyphTable->registerGlyph(key);

            if (isNew) {
                tilesValid_ = false;
            }

            TileInfo tile;
            tile.glyphID = context->glyphTable->keyToID(key);
            tiles.push_back(tile);

            glyphsSeen.insert(key);
        }
    }
}

void LayoutEngine::makeAdvances(std::vector<LineRun> & lineRuns) {
    advances.clear();

    AdvanceInfo baselineAdjust;
    baselineAdjust.type = AdvanceType::Layout;
    baselineAdjust.advanceY = 50; // TODO: fix this

    advances.push_back(baselineAdjust);

    for (const auto & lineRun : lineRuns) {
        for (const auto & shapeResultRef : lineRun.shapeResults) {
            const auto & shapeResult = shapeResultRef.get();
            AdvanceInfo advance;

            if (shapeResult.run.source.textFormat) {
                advance.type = AdvanceType::Glyph;
                // TODO: go backwards if RTL
                advance.advanceX = shapeResult.xAdvance;
                advance.advanceY = shapeResult.yAdvance;
                advance.glyphOffsetX = shapeResult.xOffset;
                advance.glyphOffsetY = shapeResult.yOffset;
                advance.glyph = shapeResult.glyphIndex;
                advance.textIndex = shapeResult.cluster;
                advance.customProperty =
                    shapeResult.run.source.textFormat->customProperty;
            } else if (shapeResult.run.source.inlineObject) {
                // TODO: handle vertical
                // TODO: go backwards if RTL
                const auto & inlineObject =
                    *shapeResult.run.source.inlineObject;
                advance.advanceX = inlineObject.pixelSize;
                advance.inlineObject = inlineObject.id;
            } else {
                assert("unknown shape result type");
            }

            advances.push_back(advance);
        }

        AdvanceInfo lineBreakAdvance;
        lineBreakAdvance.type = AdvanceType::LineBreak;
        lineBreakAdvance.advanceY = 50; // TODO: fix this
        advances.push_back(lineBreakAdvance);
    }
}

}
