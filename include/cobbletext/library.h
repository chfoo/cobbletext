#pragma once

#include "macros.h"

#ifdef __cplusplus
extern "C" {
#endif

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
 * Returns the ID for the built-in fallback font when there is no font loaded.
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
 * Sets an alternative font to be used if glyphs are not in the font.
 *
 * By default, the engine does not try another font if a font does not
 * have the required glyphs. By specifying an alternative font, the engine
 * will try to use another font. This is also known as font fallback.
 *
 * This function can be used to chain multiple fonts so the engine can try
 * them in order. You can also add the built-in fallback font at the of
 * the chain to guarantee something will be drawn.
 *
 * @param font_id The font in interest.
 * @param fallback_font_id The alternative font to select when the font in
 *      interest does not have the required glyphs.
 */
COBBLETEXT_API
void cobbletext_library_set_font_alternative(CobbletextLibrary * library,
    CobbletextFontID font_id, CobbletextFontID fallback_font_id);

/**
 * Returns the alternative font for the given font.
 *
 * @return A font ID if there is an alternative set, otherwise 0.
 */
COBBLETEXT_API
CobbletextFontID cobbletext_library_get_font_alternative(
    CobbletextLibrary * library, CobbletextFontID font_id);

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

/**
 * Clear and reset any glyph information and state.
 *
 * The library context caches glyphs until the library context is deleted.
 * This function can be called to reduce memory usage especially if your
 * text sources are from user generated content.
 *
 * All registered glyphs and images will be removed and the
 * unique glyph ID assignment counter will be reset. This means that
 * glyph IDs will be reassigned and no longer unique unless you clear
 * glyph ID references in your application.
 */
COBBLETEXT_API
void cobbletext_library_clear_glyphs(CobbletextLibrary * library);

#ifdef __cplusplus
}
#endif
