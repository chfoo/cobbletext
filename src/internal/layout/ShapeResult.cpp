#include "internal/layout/ShapeResult.hpp"

namespace cobbletext::internal {

std::ostream & operator<<(std::ostream & stream, const ShapeResult & shapeResult) {
    stream
        << "[ShapeResult "
        << "glyphIndex=" << shapeResult.glyphIndex << " "
        << "cluster=" << shapeResult.cluster << " "
        << "advance=" << shapeResult.xAdvance << "," << shapeResult.yAdvance << " "
        << "offset=" << shapeResult.xOffset << "," << shapeResult.yOffset << " "
        << "]";

    return stream;
}

}
