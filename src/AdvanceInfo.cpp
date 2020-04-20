#include "AdvanceInfo.hpp"

#include <ostream>

namespace cobbletext {

std::ostream & operator<<(std::ostream & stream, const AdvanceInfo & advance) {
    stream
        << "[AdvanceInfo "
        << "type=" << static_cast<int>(advance.type) << " "
        << "textIndex=" << advance.textIndex << " "
        << "advance=" << advance.advanceX << "," << advance.advanceY << " "
        << "customProperty=" << advance.customProperty << " "
        << "glyphID=" << advance.glyphID << " "
        << "glyph offset=" << advance.glyphOffsetX << "," << advance.glyphOffsetY << " ";

    if (advance.type == AdvanceType::InlineObject) {
        stream << "inlineObject=" << advance.inlineObject << " ";
    }

    stream << "]";

    return stream;
}

}
