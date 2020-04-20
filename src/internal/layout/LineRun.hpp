#pragma once

#include <vector>
#include <memory>
#include <ostream>
#include <optional>

#include "internal/layout/ShapeResult.hpp"

namespace cobbletext::internal {

class LineRun {
public:
    // Don't forget clear() if adding new members
    std::vector<std::reference_wrapper<const ShapeResult>> shapeResults;
    std::optional<ShapeResult> lineBreakShapeResult;
    uint32_t totalAdvance = 0;
    uint32_t lineHeight = 0;
    uint32_t ascent = 0;
    uint32_t descent = 0;

    void clear();

private:
    friend std::ostream & operator<<(std::ostream & stream, const LineRun & lineRun);
};

}
