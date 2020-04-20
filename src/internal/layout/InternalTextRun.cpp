#include "internal/layout/InternalTextRun.hpp"

namespace cobbletext::internal {

InternalTextRun::InternalTextRun(const TextRun & source) :
    InternalTextRun(source, source.textIndex, source.textLength) {
}

InternalTextRun::InternalTextRun(const TextRun & source, int32_t textIndex,
    int32_t textLength) :
    source(source),
    textIndex(textIndex),
    textLength(textLength) {
}

std::ostream & operator<<(std::ostream & stream, const InternalTextRun & run) {
    char tagBuffer[5] = {0};
    hb_tag_to_string(hb_script_to_iso15924_tag(run.script), tagBuffer);
    const char * language = hb_language_to_string(run.language);

#ifdef COBBLETEXT_DEBUG
    std::string text;
    auto subString = run.source.text.tempSubString(
        run.textIndex - run.source.textIndex, run.textLength);
    subString.toUTF8String(text);
#endif

    stream
        << "[InternalTextRun "
        << run.textIndex << ":" << run.textLength << " "
#ifdef COBBLETEXT_DEBUG
        << "text='" << text << "' "
#endif
        << "lang=" << (language != nullptr ? language : "(no lang)") << " "
        << "script=" << tagBuffer << " "
        << "direction=" << hb_direction_to_string(run.direction)
        << "]";

    return stream;
}

}
