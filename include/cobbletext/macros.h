#pragma once

/**
 * Macro for Windows symbol import and export
 *
 * @private
 */
#if defined(_WIN32) && !defined(COBBLETEXT_STATIC)
    #ifdef COBBLETEXT_EXPORTS
        #define COBBLETEXT_API __declspec(dllexport)
    #else
        #define COBBLETEXT_API __declspec(dllimport)
    #endif
#elif defined(__EMSCRIPTEN__)
    #include <emscripten.h>
    #define COBBLETEXT_API EMSCRIPTEN_KEEPALIVE
#else
    #define COBBLETEXT_API
#endif
