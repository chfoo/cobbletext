#pragma once

#include <functional>
#include <optional>
#include <memory>

#include <unicode/ubidi.h>
#include <unicode/unistr.h>
#include <harfbuzz/hb.h>

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
    std::optional<std::reference_wrapper<const icu::UnicodeString>> text;

public:
    BidiTable();

    static hb_direction_t directionToHarfBuzz(ScriptDirection direction);

    void setText(const icu::UnicodeString & text,
        ScriptDirection direction = ScriptDirection::LTR);

    bool isMixed();

    ScriptDirection getDirection(int32_t codeUnitIndex);
};

}
