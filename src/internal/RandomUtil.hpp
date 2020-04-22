#pragma once

#include <cstddef>

namespace cobbletext::internal {

class RandomUtil {
public:

private:
    static std::size_t seed;

public:
    static std::size_t getSeed();
};

}
