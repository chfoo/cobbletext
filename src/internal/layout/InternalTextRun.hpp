#pragma once

#include <cstdint>
#include <ostream>

#ifdef __EMSCRIPTEN__
    #include <hb.h>
#else
    #include <harfbuzz/hb.h>
#endif


#include "internal/input/TextRun.hpp"

namespace cobbletext::internal {

class InternalTextRun {
public:
    const TextRun & source;

    // These values are in code units to the text buffer, not code points!
    int32_t textIndex;
    int32_t textLength;

    hb_language_t language = HB_LANGUAGE_INVALID;
    hb_script_t script = HB_SCRIPT_UNKNOWN;
    hb_direction_t direction = HB_DIRECTION_INVALID;

    explicit InternalTextRun(const TextRun & source);
    explicit InternalTextRun(const TextRun & source, int32_t textIndex, int32_t textLength);

private:
    friend std::ostream & operator<<(std::ostream & stream, const InternalTextRun & run);
};

}
