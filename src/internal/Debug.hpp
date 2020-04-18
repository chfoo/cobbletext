#pragma once

#include <string>
#include <iostream>
#include <cstddef>

#ifdef COBBLETEXT_DEBUG
    #include <iostream>
    #define COBBLETEXT_DEBUG_PRINT(code) { std::cerr << code << std::endl; }
#else
    #define COBBLETEXT_DEBUG_PRINT(code)
#endif

namespace cobbletext::internal {

class Debug {
public:

    [[noreturn]] static void abort(std::string & message) {
        std::cerr << message << std::endl;
        ::abort();
    }

    [[noreturn]] static void abort(const char * message) {
        std::cerr << message << std::endl;
        ::abort();
    }

};

}
