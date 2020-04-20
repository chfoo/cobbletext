#pragma once

#include <vector>
#include <memory>
#include <ostream>
#include <optional>

#include "internal/layout/ShapeResult.hpp"

namespace cobbletext::internal {

class LineRun {
public:
    std::vector<std::reference_wrapper<const ShapeResult>> shapeResults;
    std::optional<ShapeResult> lineBreakShapeResult;
    uint32_t totalAdvance = 0;
    uint32_t lineHeight = 0;

    void clear();

private:
    friend std::ostream & operator<<(std::ostream & stream, const LineRun & lineRun);
};

}
