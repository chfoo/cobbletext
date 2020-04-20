#include "internal/input/TextRun.hpp"

namespace cobbletext::internal {

std::ostream & operator<<(std::ostream & stream, const TextRun & textRun) {
    stream
        << "[TextRun "
        << textRun.textIndex << ":" << textRun.textLength << " "
        << "lang=" << textRun.textFormat.language << " "
        << "script=" << textRun.textFormat.script << " "
        << "direction=" << static_cast<int>(textRun.textFormat.scriptDirection);

    if (textRun.inlineObject) {
        stream << "inline object";
    }

    stream << "]";

    return stream;
}

}
