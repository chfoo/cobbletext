#pragma once

#include <vector>

#include <unicode/unistr.h>
#include <unicode/locid.h>

#include "internal/input/TextFormat.hpp"
#include "internal/input/InlineObject.hpp"
#include "ScriptDirection.hpp"
#include "internal/input/TextRun.hpp"

namespace cobbletext::internal {

class TextSource {
    static constexpr char32_t OBJECT_REPLACEMENT_CODE_POINT = 0xFFFC;

public:
    icu::Locale locale;
    std::vector<TextRun> runs;
    icu::UnicodeString textBuffer;

private:

public:
    TextSource(icu::Locale locale);

    void clear();
    void addText(icu::UnicodeString text, TextFormat & textFormat);
    void addInlineObject(int id, uint32_t pixelSize);
};


}
