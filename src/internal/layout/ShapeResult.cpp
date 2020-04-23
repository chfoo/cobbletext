#include "internal/layout/ShapeResult.hpp"

#ifdef COBBLETEXT_DEBUG
    #include <string>
    #include <unicode/unistr.h>
#endif

namespace cobbletext::internal {

std::ostream & operator<<(std::ostream & stream, const ShapeResult & shapeResult) {
#ifdef COBBLETEXT_DEBUG
    std::string text;
    icu::UnicodeString uString;
    uString.append(static_cast<UChar32>(shapeResult.codePoint));
    uString.toUTF8String(text);
#endif

    stream
        << "[ShapeResult "
#ifdef COBBLETEXT_DEBUG
        << "codePoint=" << shapeResult.codePoint << " "
        << "text='" << text << "' "
#endif
        << "glyphIndex=" << shapeResult.glyphIndex << " "
        << "cluster=" << shapeResult.cluster << " "
        << "advance=" << shapeResult.xAdvance << "," << shapeResult.yAdvance << " "
        << "offset=" << shapeResult.xOffset << "," << shapeResult.yOffset << " "
        << "fontID=" << shapeResult.fontID << " "
        << "]";

    return stream;
}

}
