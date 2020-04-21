#pragma once

#include <stdint.h>
#include <uchar.h>
#include <stdbool.h>

#include "common.h"
#include "encoding.h"
#include "library.h"
#include "macros.h"
#include "script_direction.h"


/**
 * Opaque handle to a layout and render engine.
 *
 * Use `cobbletext_engine_new()` to create one.
 *
 * An engine is used repeatedly to process text.
 *
 * You can have multiple engines such as one engine for GUI elements that
 * do not change frequently and multiple engines for user chat room messages.
 */
typedef struct CobbletextEngine CobbletextEngine;


/**
 * Creates a new layout engine.
 *
 * - Can error.
 * - Free only using library.
 */
COBBLETEXT_API
CobbletextEngine * cobbletext_engine_new(CobbletextLibrary * library);

/**
 * Frees the engine.
 */
COBBLETEXT_API
void cobbletext_engine_delete(CobbletextEngine * engine);

/**
 * Returns the line length in pixels.
 *
 * If the value is 0, no word wrapping is performed.
 */
COBBLETEXT_API
uint32_t cobbletext_engine_get_line_length(CobbletextEngine * engine);

/**
 * Sets the line length in pixels.
 */
COBBLETEXT_API
void cobbletext_engine_set_line_length(CobbletextEngine * engine,
    uint32_t length);

/**
 * Returns the locale as a BCP 47 language tag.
 *
 * This value is used for assisting lower-level functions to tailor the
 * presention of the text to your application's user. It is typically
 * the GUI's locale or a document's language.
 *
 * The default is an empty string which indicates automatic detection of the
 * user's locale if possible.
 *
 * - Never null.
 * - Do not free.
 * - Owned by the engine.
 */
COBBLETEXT_API
const char * cobbletext_engine_get_locale(CobbletextEngine * engine);

/**
 * Sets the locale as a BCP 47 language tag.
 *
 * @param locale Language tag (2 or more characters) or empty string.
 */
COBBLETEXT_API
void cobbletext_engine_set_locale(CobbletextEngine * engine,
    const char * locale);

/**
 * Returns the current language as a BCP 47 language tag.
 *
 * This low-level value typically controls the glyph variant selection to
 * use from a font file.
 *
 * Default value is an empty string which indicates automatic detection.
 */
COBBLETEXT_API
const char * cobbletext_engine_get_language(CobbletextEngine * engine);

/**
 * Sets the current language as a BCP 47 language tag.
 *
 * This value will be applied to subsequent text.
 *
 * @param language Language tag (2 characters or more) or empty string.
 */
COBBLETEXT_API
void cobbletext_engine_set_language(CobbletextEngine * engine,
    const char * language);

/**
 * Returns the current script as a ISO 15924 string.
 *
 * This low-level value typically controls the rules for shaping glyphs.
 *
 * A ISO 15924 string is a 4 character string such as "Latn".
 *
 * Default value is an empty string which indicates automatic detection.
 *
 * - Never null.
 * - Do not free.
 * - Owned by engine.
 */
COBBLETEXT_API
const char * cobbletext_engine_get_script(CobbletextEngine * engine);

/**
 * Sets the current script as a ISO 15924 string.
 *
 * This value will be applied to subsequent text.
 *
 * @param string 4 character string or empty string.
 */
COBBLETEXT_API
void cobbletext_engine_set_script(CobbletextEngine * engine,
    const char * script);

/**
 * Returns the current script direction.
 *
 * Default value "not specified" indicates automatic detection.
 */
COBBLETEXT_API
CobbletextScriptDirection cobbletext_engine_get_script_direction(
    CobbletextEngine * engine);

/**
 * Sets the current script direction.
 *
 * This value will be applied to subsequent text.
 */
COBBLETEXT_API
void cobbletext_engine_set_script_direction(CobbletextEngine * engine,
    CobbletextScriptDirection direction);

/**
 * Returns the current font face.
 */
COBBLETEXT_API
CobbletextFontID cobbletext_engine_get_font(CobbletextEngine * engine);

/**
 * Sets the current font face.
 *
 * This value will be applied to subsequent text.
 */
COBBLETEXT_API
void cobbletext_engine_set_font(CobbletextEngine * engine,
    CobbletextFontID font);

/**
 * Gets the current font size in points.
 */
COBBLETEXT_API
double cobbletext_engine_get_font_size(CobbletextEngine * engine);

/**
 * Sets the current font size in points.
 *
 * This value will be applied to subsequent text.
 */
COBBLETEXT_API
void cobbletext_engine_set_font_size(CobbletextEngine * engine, double size);

/**
 * Gets the current user-provided custom property.
 *
 * Default value is 0.
 */
COBBLETEXT_API
CobbletextCustomPropertyID cobbletext_engine_get_custom_property(
    CobbletextEngine * engine);

/**
 * Sets the current user-supplied custom property.
 *
 * This value will be applied to subsequent text.
 */
COBBLETEXT_API
void cobbletext_engine_set_custom_property(CobbletextEngine * engine,
    CobbletextCustomPropertyID id);

/**
 * Append encoded bytes to the text buffer.
 *
 * @param data Encoded bytes.
 * @param length Non-negative size of the given bytes.
 * @param encoding Encoding of the bytes.
 */
COBBLETEXT_API
void cobbletext_engine_add_text(CobbletextEngine * engine, const uint8_t * data,
    int32_t length, CobbletextEncoding encoding);

/**
 * Append UTF-8 code units to the text buffer.
 *
 * @param text UTF-8 code units.
 * @param length Number of code units, otherwise,
 *      negative value to detect null terminator.
 */
COBBLETEXT_API
void cobbletext_engine_add_text_utf8(CobbletextEngine * engine,
    const char * text, int32_t length);

/**
 * Append UTF-16 code units to the text buffer.
 *
 * @param text UTF-16 code units.
 * @param length Number of code units, otherwise,
 *      negative value to detect null terminator.
 */
COBBLETEXT_API
void cobbletext_engine_add_text_utf16(CobbletextEngine * engine,
    const char16_t * text, int32_t length);

/**
 * Append UTF-32 code units to the text buffer.
 *
 * @param text UTF-32 code units.
 * @param length Number of code units, otherwise,
 *      negative value to detect null terminator.
 */
COBBLETEXT_API
void cobbletext_engine_add_text_utf32(CobbletextEngine * engine,
    const char32_t * text, int32_t length);

/**
 * Append a placeholder for an object to the text buffer.
 *
 * @param id A user-provided value.
 * @param size Width in pixels of the object.
 */
COBBLETEXT_API
void cobbletext_engine_add_inline_object(CobbletextEngine * engine,
    CobbletextInlineObjectID id, uint32_t size);

/**
 * Empty the text buffer.
 *
 * This empties the text buffer so the engine can be used to process
 * anther set of text. The properties are not reset.
 */
COBBLETEXT_API
void cobbletext_engine_clear(CobbletextEngine * engine);

/**
 * Process and shape the text.
 *
 * - Can error.
 */
COBBLETEXT_API
void cobbletext_engine_lay_out(CobbletextEngine * engine);

/**
 * Returns whether the associated glyphs to this engine is not stale.
 *
 * @pre Call `cobbletext_engine_lay_out()`
 * @return `true` if no rasterization or texture packing is required
 */
COBBLETEXT_API
bool cobbletext_engine_tiles_valid(CobbletextEngine * engine);

/**
 * Convert the glyphs required by this engine to coverages maps for drawing
 * the text.
 *
 * To get the glyphs needed, use `cobbletext_engine_prepare_tiles()`.
 *
 * - Can error.
 */
COBBLETEXT_API
void cobbletext_engine_rasterize(CobbletextEngine * engine);

/**
 * Texture pack the associated glyphs.
 *
 * @return `true` if the glyphs won't fit within the texture size, `false`
 *      if everything is OK.
 */
COBBLETEXT_API
bool cobbletext_engine_pack_tiles(CobbletextEngine * engine, uint32_t width,
    uint32_t height);

/**
 * Prepares an array of tiles required for getting glyph images.
 *
 * If you have rasterized the glyphs, use
 * `cobbletext_library_get_glyph_info()` to retreive the images.
 */
COBBLETEXT_API
void cobbletext_engine_prepare_tiles(CobbletextEngine * engine);

/**
 * Returns the number of tiles in the tiles array.
 *
 * @pre Call `cobbletext_engine_prepare_tiles()`.
 */
COBBLETEXT_API
uint32_t cobbletext_engine_get_tile_count(CobbletextEngine * engine);

/**
 * Returns the array of tiles.
 *
 * @pre Call `cobbletext_engine_prepare_tiles()`.
 *
 * - Do not free.
 * - Valid until the next call (excluding getters).
 */
COBBLETEXT_API
const struct CobbletextTileInfo ** cobbletext_engine_get_tiles(
    CobbletextEngine * engine);

/**
 * Prepare an array of advances required for positioning glyphs.
 */
COBBLETEXT_API
void cobbletext_engine_prepare_advances(CobbletextEngine * engine);

/**
 * Return the number advances in the advances array.
 *
 * @pre Call `cobbletext_engine_prepare_advances()`.
 */
COBBLETEXT_API
uint32_t cobbletext_engine_get_advance_count(CobbletextEngine * engine);

/**
 * Return the advances array.
 *
 * - Do not free.
 * - Valid until the next call (excluding getters).
 */
COBBLETEXT_API
const struct CobbletextAdvanceInfo ** cobbletext_engine_get_advances(
    CobbletextEngine * engine);

/**
 * Returns information about the lay out operation.
 *
 * - Do not free.
 * - Valid until the next call (excluding getters).
 */
COBBLETEXT_API
const struct CobbletextOutputInfo * cobbletext_engine_get_output_info(
    CobbletextEngine * engine);
