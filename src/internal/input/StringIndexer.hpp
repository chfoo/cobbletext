#pragma once

#include <vector>
#include <memory>
#include <stdint.h>

#include <unicode/unistr.h>
#include <unicode/schriter.h>

namespace cobbletext::internal {

class StringIndexer {
    std::shared_ptr<icu::UnicodeString> textBuffer;
    icu::StringCharacterIterator iterator;
    int32_t currentIndex32;

public:
    StringIndexer();
    explicit StringIndexer(std::shared_ptr<icu::UnicodeString> textBuffer);

    void setTextBuffer(std::shared_ptr<icu::UnicodeString> textBuffer);
    void reset();
    int32_t codePointToCodeUnit(int32_t index32);
};


}
