
#include "internal/input/TextSource.hpp"

#include "internal/ICUError.hpp"

namespace cobbletext::internal {

TextSource::TextSource(icu::Locale locale) :
    locale(locale) {
}

void TextSource::clear() {
    runs.clear();
    textBuffer.remove();
}

void TextSource::addText(icu::UnicodeString text, TextFormat & textFormat) {
    TextRun run;

    run.textFormat = textFormat;
    run.textIndex = textBuffer.length();
    textBuffer.append(text);
    run.textLength = textBuffer.length() - run.textIndex;

    runs.push_back(run);
}

void TextSource::addInlineObject(int id, uint32_t pixelSize) {
    TextRun run;

    run.inlineObject.emplace(id, pixelSize);

    run.textIndex = textBuffer.length();
    textBuffer.append(static_cast<UChar32>(OBJECT_REPLACEMENT_CODE_POINT));
    run.textLength = textBuffer.length() - run.textIndex;

    runs.push_back(run);
}

}