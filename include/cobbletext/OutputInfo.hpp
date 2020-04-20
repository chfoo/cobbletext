#pragma once

#include <ostream>

#include "common.hpp"

namespace cobbletext {

class COBBLETEXT_API OutputInfo {
public:
    uint32_t textWidth;
    uint32_t textHeight;

private:
    friend std::ostream & operator<<(std::ostream & stream,
        const OutputInfo & outputInfo);
};

}
