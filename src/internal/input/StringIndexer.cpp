#include "internal/input/StringIndexer.hpp"

#include <cassert>

namespace cobbletext::internal {

StringIndexer::StringIndexer() :
    iterator(icu::UnicodeString()) {}

StringIndexer::StringIndexer(std::shared_ptr<icu::UnicodeString> textBuffer) :
        textBuffer(textBuffer),
        iterator(*textBuffer),
        currentIndex32(0) {

    iterator.first32();
}

void StringIndexer::setTextBuffer(
        std::shared_ptr<icu::UnicodeString> textBuffer) {
    this->textBuffer = textBuffer;
}

void StringIndexer::reset() {
    assert(textBuffer);
    iterator.setText(*textBuffer);
    currentIndex32 = 0;
}

int32_t StringIndexer::codePointToCodeUnit(int32_t index32) {
    while (currentIndex32 < index32) {
        auto codeUnit = iterator.next32PostInc();

        if (codeUnit == icu::StringCharacterIterator::DONE) {
            break;
        }

        currentIndex32 += 1;
    }

    while (currentIndex32 > index32) {
        auto codeUnit = iterator.previous32();

        if (codeUnit == icu::StringCharacterIterator::DONE) {
            break;
        }

        currentIndex32 -= 1;
    }

    return iterator.getIndex();
}

}
