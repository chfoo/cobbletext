#pragma once

#include <stdint.h>

namespace cobbletext::internal {

class ColorUtil {
public:

    static uint32_t makeARGB(uint8_t red, uint8_t green, uint8_t blue,
        uint8_t alpha);
    static uint8_t alphaARGB(uint32_t color);
    static uint8_t redARGB(uint32_t color);
    static uint8_t greenARGB(uint32_t color);
    static uint8_t blueARGB(uint32_t color);

};

inline
uint32_t ColorUtil::makeARGB(uint8_t red, uint8_t green, uint8_t blue,
        uint8_t alpha) {
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

inline
uint8_t ColorUtil::alphaARGB(uint32_t color) {
    return (color >> 24) & 0xff;
}

inline
uint8_t ColorUtil::redARGB(uint32_t color) {
    return (color >> 16) & 0xff;
}

inline
uint8_t ColorUtil::greenARGB(uint32_t color) {
    return (color >> 8) & 0xff;
}

inline
uint8_t ColorUtil::blueARGB(uint32_t color) {
    return (color >> 0) & 0xff;
}


}
