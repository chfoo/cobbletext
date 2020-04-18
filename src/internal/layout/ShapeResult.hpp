#pragma once

#include <cstdint>
#include <ostream>

#include "internal/layout/InternalTextRun.hpp"

namespace cobbletext::internal {

class ShapeResult {
public:
    const InternalTextRun & run;
    int32_t xAdvance;
    int32_t yAdvance;
    int32_t xOffset;
    int32_t yOffset;
    uint32_t cluster;
    uint32_t glyphIndex;

    explicit ShapeResult(const InternalTextRun & run) : run(run) {}

private:
    friend std::ostream & operator<<(std::ostream & stream, const ShapeResult & shapeResult);
};

}
