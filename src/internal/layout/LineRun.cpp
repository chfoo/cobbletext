#include "internal/layout/LineRun.hpp"

namespace cobbletext::internal {

std::ostream & operator<<(std::ostream & stream, const LineRun & line) {
    stream
        << "[LineRun "
        << "totalAdvance=" << line.totalAdvance << " "
        << "lineHeight=" << line.lineHeight << " "
        << "shapeResults=" << line.shapeResults.size() << " "
        << "]";

    return stream;
}

}
