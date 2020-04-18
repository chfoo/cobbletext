#pragma once

#include <cstdint>

namespace cobbletext::internal {

class RandomUtil {
public:

private:
    static std::size_t seed;

public:
    static std::size_t getSeed();
};

}
