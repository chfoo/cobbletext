#pragma once

#include <optional>
#include <ostream>

#include "internal/input/TextFormat.hpp"
#include "internal/input/InlineObject.hpp"

namespace cobbletext::internal {

class TextRun {
public:
    // These values are in code units to the text buffer, not code points!
    int32_t textIndex;
    int32_t textLength;

    std::optional<TextFormat> textFormat;
    std::optional<InlineObject> inlineObject;

private:
    friend std::ostream & operator<<(std::ostream & stream, const TextRun & textRun);
};

}
