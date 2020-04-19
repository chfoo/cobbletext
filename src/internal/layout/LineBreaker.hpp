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

namespace cobbletext::internal {

class LineBreaker {
    icu::Locale breakerLocale;
    std::unique_ptr<icu::BreakIterator> lineBreaker;
    std::unique_ptr<icu::BreakIterator> characterBreaker;
    std::shared_ptr<icu::UnicodeString> text;
    StringIndexer stringIndexer;

public:
    uint32_t lineLength = 0;

    void locale(const icu::Locale & locale);

    void setTextBuffer(std::shared_ptr<icu::UnicodeString> text);

    std::vector<LineRun> applyBreaks(const std::vector<ShapeResult> & shapeResults);

private:
    void remakeBreakers();


};

}
