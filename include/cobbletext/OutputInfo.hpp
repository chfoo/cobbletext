#pragma once

#include <ostream>

#include "common.hpp"
#include "macros.h"

namespace cobbletext {

class OutputInfo {
public:
    uint32_t textWidth = 0;
    uint32_t textHeight = 0;

private:
    friend std::ostream & operator<<(std::ostream & stream,
        const OutputInfo & outputInfo);
};

}
