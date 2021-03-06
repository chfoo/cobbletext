#pragma once

#include <memory>
#include <unordered_set>

#include <unicode/uloc.h>

#ifdef __EMSCRIPTEN__
    #include <hb.h>
#else
    #include <harfbuzz/hb.h>
#endif


#include "AdvanceInfo.hpp"
#include "internal/Context.hpp"
#include "internal/input/TextSource.hpp"
#include "internal/layout/InternalTextRun.hpp"
#include "internal/layout/LineBreaker.hpp"
#include "internal/layout/Shaper.hpp"
#include "internal/layout/ShapeResult.hpp"
#include "internal/table/BidiTable.hpp"
#include "internal/table/ScriptTable.hpp"
#include "TextAlignment.hpp"
#include "TileInfo.hpp"

namespace cobbletext::internal {

class LayoutEngine {
    BidiTable bidiTable;
    ScriptTable scriptTable;
    Shaper shaper;
    LineBreaker lineBreaker;

    std::shared_ptr<Context> context;
    std::shared_ptr<TextSource> textSource;

    hb_language_t defaultLanguageHB = HB_LANGUAGE_INVALID;
    ScriptDirection defaultDirection = ScriptDirection::NotSpecified;
    hb_direction_t defaultDirectionHB = HB_DIRECTION_INVALID;

    std::vector<InternalTextRun> internalRuns;
    std::shared_ptr<std::vector<ShapeResult>> shapeResults;

    std::unordered_set<GlyphKey,GlyphKeyHasher> glyphs;
    std::vector<TileInfo> tiles_;
    std::vector<AdvanceInfo> advances_;

    uint32_t textWidth_;
    uint32_t textHeight_;

    bool tilesValid_ = true;

    uint32_t previousTextIndex;

public:
    uint32_t lineLength = 0;
    TextAlignment textAlignment = TextAlignment::NotSpecified;

    std::vector<TileInfo> & tiles();
    std::vector<AdvanceInfo> & advances();

    uint32_t textWidth();
    uint32_t textHeight();

    bool tilesValid();

    LayoutEngine(std::shared_ptr<Context> context,
        std::shared_ptr<TextSource> textSource);
    ~LayoutEngine();

    void clearText();
    void clearTiles();

    void layOut();

private:
    void createInternalRuns();
    void processInlineObject(const TextRun & textRun);
    void processTextRun(const TextRun & textRun);
    void fastPathTextRun(const TextRun & textRun);

    void registerGlyphsAndMakeTiles();
    void makeAdvances(std::vector<LineRun> & lineRuns);
    void processLineRun(const LineRun & lineRun);
    int32_t getTextAlignmentOffset(const LineRun & lineRun);
};

}
