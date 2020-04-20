#include "internal/layout/LineRun.hpp"

namespace cobbletext::internal {

std::ostream & operator<<(std::ostream & stream, const LineRun & line) {
    stream
        << "[LineRun "
        << "totalAdvance=" << line.totalAdvance << " "
        << "lineHeight=" << line.lineHeight << " "
        << "shapeResults=" << line.shapeResults.size() << " ";

    if (!line.shapeResults.empty()) {
        stream << line.shapeResults.front() << " "
            << line.shapeResults.back() << " ";
    }

    stream << "]";

    return stream;
}

}
