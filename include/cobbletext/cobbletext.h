#ifndef COBBLETEXT_COBBLETEXT_H_
#define COBBLETEXT_COBBLETEXT_H_

/**
 * @mainpage Cobbletext: complex text layout and rendering engine library.
 *
 * This is the reference API documentation for Cobbletext.<br>
 *
 * For the C API, see `cobbletext/cobbletext.h`.<br>
 *
 * For the C++ api, see `cobbletext/cobbletext.hpp`.<br>
 *
 * Consult the project README file for details.
 */

/**
 * @file cobbletext.h
 *
 * Cobbletext C API
 *
 * Start at `cobbletext_library_new()`, then `cobbletext_engine_new()`.
 *
 * Whenever a function is described as "can error", check the value of
 * `cobbletext_get_error_code()` before continuing. If there is an error,
 * the returned value or the internal state may be invalid.
 *
 * Pointer parameters are copied by the function unless noted. Pointer
 * return types should not be freed and become invalid on the next function
 * call unless noted.
 *
 * Whenever a pointer is described "free only using library", use
 * Cobbletext's corresponding delete function for that handle.
 * Do not call `free()` on it.
 */


#ifdef __cplusplus
extern "C" {
#endif

#include "advance_info.h"
#include "common.h"
#include "encoding.h"
#include "engine_properties.h"
#include "engine.h"
#include "font_info.h"
#include "glyph_info.h"
#include "library.h"
#include "macros.h"
#include "math_.h"
#include "output_info.h"
#include "script_direction.h"
#include "text_properties.h"
#include "tile_info.h"
#include "uchar_helper.h"

#ifdef COBBLETEXT_TYPEDEF_STRUCTS
    typedef struct CobbletextFontInfo CobbletextFontInfo;
    typedef struct CobbletextGlyphInfo CobbletextGlyphInfo;
    typedef struct CobbletextTextProperties CobbletextTextProperties;
    typedef struct CobbletextEngineProperties CobbletextEngineProperties;
    typedef struct CobbletextTileInfo CobbletextTileInfo;
    typedef struct CobbletextAdvanceInfo CobbletextAdvanceInfo;
    typedef struct CobbletextOutputInfo CobbletextOutputInfo;
#endif

#ifdef __cplusplus
}
#endif


#endif
