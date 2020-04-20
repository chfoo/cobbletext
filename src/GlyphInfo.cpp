#include "GlyphInfo.hpp"

#include <ostream>

namespace cobbletext {

std::ostream & operator<<(std::ostream & stream, const GlyphInfo & glyph) {
    stream
        << "[GlyphInfo "
        << "id=" << glyph.id << " "
        << "image size=" << glyph.imageWidth << "," << glyph.imageHeight << " "
        << "image offset=" << glyph.imageOffsetX << "," << glyph.imageOffsetY << " "
        << "image=" << glyph.image.size() << " "
        << "]";

    return stream;
}

}
