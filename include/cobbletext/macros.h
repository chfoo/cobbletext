#pragma once

/**
 * @private
 */
#ifdef _WIN32
    #ifdef COBBLETEXT_EXPORTS
        #define COBBLETEXT_API __declspec(dllexport)
    #else
        #define COBBLETEXT_API __declspec(dllimport)
    #endif
#else
    #define COBBLETEXT_API
#endif
