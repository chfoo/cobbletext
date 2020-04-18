#pragma once

#include <stdint.h>

namespace cobbletext::internal {

class InlineObject {
public:
    InlineObject(int id, uint32_t pixelSize)
        : id(id), pixelSize(pixelSize) {}

    int id;
    uint32_t pixelSize;
};

}
