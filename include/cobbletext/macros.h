#pragma once

/**
 * @private
 */
#if defined(_WIN32) && !defined(COBBLETEXT_STATIC)
    #ifdef COBBLETEXT_EXPORTS
        #define COBBLETEXT_API __declspec(dllexport)
    #else
        #define COBBLETEXT_API __declspec(dllimport)
    #endif
#else
    #define COBBLETEXT_API
#endif
