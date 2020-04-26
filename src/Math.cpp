#include "Math.hpp"

#include <math.h>

#include "internal/ColorUtil.hpp"

namespace cobbletext {

uint32_t Math::alpha_blend_over_argb(uint32_t background, uint32_t foreground) {
    using C = cobbletext::internal::ColorUtil;

    double alphaBG = C::alphaARGB(background) / 255.0;
    double redBG = C::redARGB(background) / 255.0;
    double greenBG = C::greenARGB(background) / 255.0;
    double blueBG = C::blueARGB(background) / 255.0;

    double alphaFG = C::alphaARGB(foreground) / 255.0;
    double redFG = C::redARGB(foreground) / 255.0;
    double greenFG = C::greenARGB(foreground) / 255.0;
    double blueFG = C::blueARGB(foreground) / 255.0;

    double divisor = alphaFG + alphaBG * (1 - alphaFG);

    double newAlpha = divisor;
    double newRed = redFG * alphaFG + redBG * alphaBG * (1 - alphaFG);
    double newGreen = greenFG * alphaFG + greenBG * alphaBG * (1 - alphaFG);
    double newBlue = blueFG * alphaFG + blueBG * alphaBG * (1 - alphaFG);

    return C::makeARGB(newRed * 255, newGreen * 255, newBlue * 255,
        newAlpha * 255);
}

uint32_t Math::gamma_argb(uint32_t color, double gamma) {
    using C = cobbletext::internal::ColorUtil;

    uint8_t alpha = C::alphaARGB(color);
    uint8_t red = C::redARGB(color);
    uint8_t green = C::greenARGB(color);
    uint8_t blue = C::blueARGB(color);

    uint8_t newAlpha = alpha;
    uint8_t newRed = 255.0 * pow(red / 255.0, gamma);
    uint8_t newGreen = 255.0 * pow(green / 255.0, gamma);
    uint8_t newBlue = 255.0 * pow(blue / 255.0, gamma);

    return C::makeARGB(newRed, newGreen, newBlue, newAlpha);
}

}
