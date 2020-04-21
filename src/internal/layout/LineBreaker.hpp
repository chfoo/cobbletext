#pragma once

#include <stdint.h>
#include <memory>
#include <vector>

#include <unicode/brkiter.h>
#include <unicode/uloc.h>
#include <unicode/ustring.h>

#include "internal/layout/ShapeResult.hpp"
#include "internal/layout/LineRun.hpp"
#include "internal/input/StringIndexer.hpp"
#include "internal/Context.hpp"

namespace cobbletext::internal {

class LineBreaker {
    std::shared_ptr<Context> context;
    icu::Locale breakerLocale;
    std::unique_ptr<icu::BreakIterator> lineBreaker;
    std::unique_ptr<icu::BreakIterator> characterBreaker;
    std::shared_ptr<icu::UnicodeString> text;
    StringIndexer stringIndexer;

    std::shared_ptr<std::vector<ShapeResult>> shapeResults;
    std::optional<std::vector<ShapeResult>::iterator> shapeResultIterator;

    std::vector<LineRun> lines;

    LineRun currentLine;

public:
    uint32_t lineLength = 0;

    explicit LineBreaker(std::shared_ptr<Context> context);

    void locale(const icu::Locale & locale);

    void setTextBuffer(std::shared_ptr<icu::UnicodeString> text);

    std::vector<LineRun> applyBreaks(
        std::shared_ptr<std::vector<ShapeResult>> shapeResults);

private:
    void remakeBreakers();

    void pushCurrentLine();

    bool isMandatoryLineBreakAfter(int32_t codePointIndex);

    bool isCharacterBreakableBefore(int32_t codePointIndex);

    void fillLine();

    void rewindLine();

    void emergencyFillLine();

    void analyzeLineHeight();

    uint32_t getShapeLength(const ShapeResult & shapeResult);
};

}
