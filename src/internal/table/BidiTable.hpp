#pragma once

#include <functional>
#include <optional>
#include <memory>

#ifdef __EMSCRIPTEN__
    #include <hb.h>
#else
    #include <harfbuzz/hb.h>
#endif

#include <unicode/ubidi.h>
#include <unicode/unistr.h>


#include "ScriptDirection.hpp"

namespace cobbletext::internal {

class UbidiDeleter {
public:
    void operator()(UBiDi * bidiStruct) const {
        ubidi_close(bidiStruct);
    }
};

class BidiTable {
    std::unique_ptr<UBiDi,UbidiDeleter> bidiStruct;
    std::shared_ptr<const icu::UnicodeString> text;

public:
    BidiTable();

    static hb_direction_t directionToHarfBuzz(ScriptDirection direction);

    void setTextBuffer(std::shared_ptr<const icu::UnicodeString> text);

    void analyze(ScriptDirection direction);

    bool isMixed();

    ScriptDirection getDirection(int32_t codeUnitIndex);
};

}
