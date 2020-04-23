#pragma once

#include "macros.h"

/**
 * Opaque handle to the library's context.
 *
 * Use `cobbletext_library_new()` to create one.
 *
 * Once you obtain a library context, you can use it to open a layout and
 * render engine with `cobbletext_engine_new()`.
 *
 * Cobbletext and its dependencies is not thread safe. A library context
 * can be created for each thread. Otherwise, a simple application will only
 * need to use a single library context.
 */
typedef struct CobbletextLibrary CobbletextLibrary;


/**
 * Returns a new library context.
 *
 * - Can error.
 * - Free only using library.
 * - Null on handle allocation error. Otherwise, not null.
 */
COBBLETEXT_API
CobbletextLibrary * cobbletext_library_new();

/**
 * Frees a library context.
 */
COBBLETEXT_API
void cobbletext_library_delete(CobbletextLibrary * library);

/**
 * Returns error code.
 *
 * The return code is usually passed through from the libraries used
 * by Cobbletext.
 *
 * @return 0 if there is no error, a non-zero value otherwise.
 */
COBBLETEXT_API
int32_t cobbletext_get_error_code(CobbletextLibrary * library);

/**
 * Returns a textual version of the error code.
 *
 * - Value is never null.
 * - Do not free.
 */
COBBLETEXT_API
const char * cobbletext_get_error_message(CobbletextLibrary * library);

/**
 * Clears any error code or error message.
 */
COBBLETEXT_API
void cobbletext_clear_error(CobbletextLibrary * library);

/**
 * Returns the ID for the fallback font.
 */
COBBLETEXT_API
CobbletextFontID cobbletext_library_get_fallback_font(
    CobbletextLibrary * library);

/**
 * Loads a font face from a file.
 *
 * @param path Filename of the font file.
 *
 * - Can error.
 */
COBBLETEXT_API
CobbletextFontID cobbletext_library_load_font(CobbletextLibrary * library,
    const char * path);

/**
 * Loads a font face from the given bytes of a font file.
 *
 * @param data Binary contents of a font file.
 * @param length Size in bytes.
 *
 * - Can error.
 */
COBBLETEXT_API
CobbletextFontID cobbletext_library_load_font_bytes(
    CobbletextLibrary * library, const uint8_t * data, uint32_t length);

/**
 * Returns information about a loaded font.
 *
 * - Can error.
 * - Do not free.
 * - Valid until the next function call.
 */
COBBLETEXT_API
const struct CobbletextFontInfo * cobbletext_library_get_font_info(
    CobbletextLibrary * library, CobbletextFontID font);

/**
 * Returns information about a glyph from a loaded font.
 *
 * - Can error.
 * - Do not free.
 * - Valid until the next function call.
 */
COBBLETEXT_API
const struct CobbletextGlyphInfo * cobbletext_library_get_glyph_info(
    CobbletextLibrary * library, CobbletextFontID glyph);
