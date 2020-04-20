#include "internal/input/TextRun.hpp"

namespace cobbletext::internal {

std::ostream & operator<<(std::ostream & stream, const TextRun & textRun) {
#ifdef COBBLETEXT_DEBUG
    std::string text;
    textRun.text.toUTF8String(text);
#endif

    stream
        << "[TextRun "
        << textRun.textIndex << ":" << textRun.textLength << " "
#ifdef COBBLETEXT_DEBUG
        << "text='" << text << "' "
#endif
        << "lang=" << textRun.textFormat.language << " "
        << "script=" << textRun.textFormat.script << " "
        << "direction=" <<
            static_cast<int>(textRun.textFormat.scriptDirection) << " "
        << "font=" << textRun.textFormat.fontFace << " "
        << "fontSize=" << textRun.textFormat.fontSize << " "
        << "customProperty=" << textRun.textFormat.customProperty << " ";

    if (textRun.inlineObject) {
        stream << "inline object";
    }

    stream << "]";

    return stream;
}

}
