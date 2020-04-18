#include "internal/layout/InternalTextRun.hpp"

namespace cobbletext::internal {

InternalTextRun::InternalTextRun(const TextRun & source) :
    source(source),
    textIndex(source.textIndex),
    textLength(source.textLength) {
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

    stream
        << "[InternalTextRun "
        << run.textIndex << ":" << run.textLength << " "
        << "lang=" << (language != nullptr ? language : "(no lang)") << " "
        << "script=" << tagBuffer << " "
        << "direction=" << hb_direction_to_string(run.direction)
        << "]";

    return stream;
}

}
