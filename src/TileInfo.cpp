#include "TileInfo.hpp"

#include <ostream>

namespace cobbletext {

std::ostream & operator<<(std::ostream & stream, const TileInfo & tile) {
    stream
        << "[TileInfo "
        << "glyphID=" << tile.glyphID << " "
        << "atlas pos=" << tile.atlasX << "," << tile.atlasY << " "
        << "]";

    return stream;
}

}
