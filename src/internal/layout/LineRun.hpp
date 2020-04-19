#pragma once

#include <vector>

#include "internal/layout/ShapeResult.hpp"

namespace cobbletext::internal {

class LineRun {
public:
    std::vector<std::reference_wrapper<const ShapeResult>> shapeResults;
    uint32_t length;


};

}
