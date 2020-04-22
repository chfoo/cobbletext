#pragma once

#include <ostream>

#include "common.hpp"
#include "macros.h"

namespace cobbletext {

class OutputInfo {
public:
    uint32_t textWidth;
    uint32_t textHeight;

private:
    friend std::ostream & operator<<(std::ostream & stream,
        const OutputInfo & outputInfo);
};

}
