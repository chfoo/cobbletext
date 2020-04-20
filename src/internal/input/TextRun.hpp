#pragma once

#include <optional>
#include <ostream>

#ifdef COBBLETEXT_DEBUG
    #include <string>
    #include <unicode/unistr.h>
#endif

#include "internal/input/TextFormat.hpp"
#include "internal/input/InlineObject.hpp"

namespace cobbletext::internal {

class TextRun {
public:
    // These values are in code units to the text buffer, not code points!
    int32_t textIndex;
    int32_t textLength;

    TextFormat textFormat;

    std::optional<InlineObject> inlineObject;

#ifdef COBBLETEXT_DEBUG
    icu::UnicodeString text;
#endif

private:
    friend std::ostream & operator<<(std::ostream & stream,
        const TextRun & textRun);
};

}
