#include "internal/table/BidiTable.hpp"

#include <unicode/schriter.h>
#include <cassert>

#include "internal/ICUError.hpp"

namespace cobbletext::internal {

BidiTable::BidiTable()
    : bidiStruct(ubidi_open()) {
}

hb_direction_t BidiTable::directionToHarfBuzz(ScriptDirection direction) {
    switch (direction) {
        case ScriptDirection::LTR:
            return HB_DIRECTION_LTR;
        case ScriptDirection::RTL:
            return HB_DIRECTION_RTL;
        default:
            return HB_DIRECTION_INVALID;
    }
}


void BidiTable::setTextBuffer(std::shared_ptr<const icu::UnicodeString> text) {
    this->text = text;
}

void BidiTable::analyze(ScriptDirection direction) {
    assert(text);

    UBiDiLevel paragraphLevel;

    switch (direction) {
        case ScriptDirection::LTR:
        case ScriptDirection::NotSpecified:
            paragraphLevel = UBIDI_DEFAULT_LTR;
        case ScriptDirection::RTL:
            paragraphLevel = UBIDI_DEFAULT_RTL;
    }

    ICUError errorCode;

    ubidi_setPara(bidiStruct.get(), text->getBuffer(), text->length(),
        paragraphLevel, nullptr, errorCode);
}

bool BidiTable::isMixed() {
    UBiDiDirection direction = ubidi_getDirection(bidiStruct.get());

    return direction == UBIDI_MIXED;
}

ScriptDirection BidiTable::getDirection(int32_t codeUnitIndex) {
    std::vector<ScriptDirection> directions;

    UBiDiLevel level = ubidi_getLevelAt(bidiStruct.get(), codeUnitIndex);

    if (level % 2 == 0) {
        return ScriptDirection::LTR;
    } else {
        return ScriptDirection::RTL;
    }
}

}
