#pragma once

#include <stdint.h>

namespace cobbletext::internal {

class InlineObject {
public:
    InlineObject(int id, uint32_t pixelWidth, uint32_t pixelHeight)
        : id(id), pixelWidth(pixelWidth), pixelHeight(pixelHeight) {}

    int id;
    uint32_t pixelWidth;
    uint32_t pixelHeight;
};

}
