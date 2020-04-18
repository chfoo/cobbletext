#pragma once

#ifdef _WIN32
    #ifdef COBBLETEXT_EXPORTS
        #define COBBLETEXT_API __declspec(dllexport)
    #else
        #define COBBLETEXT_API __declspec(dllimport)
    #endif
#else
    #define COBBLETEXT_API
#endif

#include <stdint.h>

namespace cobbletext {

typedef uint32_t FontID;
typedef uint32_t GlyphID;
typedef uint32_t InlineObjectID;
typedef uint64_t CustomPropertyID;

}
