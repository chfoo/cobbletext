#include "internal/RandomUtil.hpp"

#include <random>

namespace cobbletext::internal {

std::size_t RandomUtil::seed = 0;


std::size_t RandomUtil::getSeed() {
    while (seed == 0) {
        std::random_device random;
        seed = (random() << 16ull) | random();
    }

    return seed;
}

}
