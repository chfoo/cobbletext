#pragma once

/**
 * Macro for Windows C symbol import and export, Emscripten attribute
 *
 * @private
 */
#if defined(_WIN32) && !defined(COBBLETEXT_STATIC)
    #ifdef cobbletext_EXPORTS
        #define COBBLETEXT_API __declspec(dllexport)
    #else
        #define COBBLETEXT_API __declspec(dllimport)
    #endif
#elif defined(__EMSCRIPTEN__) && defined(COBBLETEXT_ENABLE_EM_API)
    #include <emscripten.h>
    #define COBBLETEXT_API EMSCRIPTEN_KEEPALIVE
#else
    #define COBBLETEXT_API
#endif

/**
 * Macro for Windows C++ symbol import and export
 *
 * @private
 */
#if defined(_WIN32) && defined(COBBLETEXT_ENABLE_CPP_API) && !defined(COBBLETEXT_STATIC)
    #ifdef cobbletext_EXPORTS
        #define COBBLETEXT_CPP_API __declspec(dllexport)
    #else
        #define COBBLETEXT_CPP_API __declspec(dllimport)
    #endif
#else
    #define COBBLETEXT_CPP_API
#endif
