
#include "internal/input/TextSource.hpp"

#include "internal/ICUError.hpp"

namespace cobbletext::internal {

TextSource::TextSource() :
    textBuffer(std::make_shared<icu::UnicodeString>()) {
}

void TextSource::clear() {
    runs.clear();
    textBuffer->remove();
}

void TextSource::addText(icu::UnicodeString text, TextFormat textFormat) {
    TextRun run;

    run.textFormat = textFormat;
    run.textIndex = textBuffer->length();
    textBuffer->append(text);
    run.textLength = textBuffer->length() - run.textIndex;

#ifdef COBBLETEXT_DEBUG
    run.text = text;
#endif

    runs.push_back(run);
}

void TextSource::addInlineObject(int id, uint32_t pixelWidth,
        uint32_t pixelHeight, TextFormat textFormat) {
    TextRun run;

    run.textFormat = textFormat;

    run.inlineObject.emplace(id, pixelWidth, pixelHeight);

    run.textIndex = textBuffer->length();
    textBuffer->append(static_cast<UChar32>(OBJECT_REPLACEMENT_CODE_POINT));
    run.textLength = textBuffer->length() - run.textIndex;

    runs.push_back(run);
}

}
