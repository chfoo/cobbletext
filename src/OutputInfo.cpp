#include "OutputInfo.hpp"

#include <ostream>

namespace cobbletext {

std::ostream & operator<<(std::ostream & stream, const OutputInfo & info) {
    stream
        << "[OutputInfo "
        << "text size=" << info.textWidth << "," << info.textHeight << " "
        << "]";

    return stream;
}

}
