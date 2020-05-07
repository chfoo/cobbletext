#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "uchar_helper.h"

#include "common.h"
#include "encoding.h"
#include "engine_properties.h"
#include "library.h"
#include "macros.h"
#include "script_direction.h"
#include "text_properties.h"

#ifdef __cplusplus
extern "C" {
#endif

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
 * - Null on error.
 */
COBBLETEXT_API
CobbletextEngine * cobbletext_engine_new(CobbletextLibrary * library);

/**
 * Frees the engine.
 */
COBBLETEXT_API
void cobbletext_engine_delete(CobbletextEngine * engine);

/**
 * Returns the engine properties.
 *
 * - Never null.
 * - Do not free.
 * - Owned by engine.
 */
COBBLETEXT_API
const struct CobbletextEngineProperties * cobbletext_engine_get_properties(
    CobbletextEngine * engine);

/**
 * Sets engine properties.
 * *
 * @param properties The properties to be applied. Fields will be copied.
 */
COBBLETEXT_API
void cobbletext_engine_set_properties(CobbletextEngine * engine,
    const struct CobbletextEngineProperties * properties);

/**
 * Returns the current text properties.
 *
 * - Never null.
 * - Do not free.
 * - Owned by engine.
 */
COBBLETEXT_API
const struct CobbletextTextProperties * cobbletext_engine_get_text_properties(
    CobbletextEngine * engine);

/**
 * Sets the current text properties.
 *
 * The propreties will be applied to subsequent text.
 *
 * @param text_properties The properties to be applied. Fields will be copied.
 */
COBBLETEXT_API
void cobbletext_engine_set_text_properties(CobbletextEngine * engine,
    const struct CobbletextTextProperties * text_properties);

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
    const CobbletextChar16 * text, int32_t length);

/**
 * Append UTF-32 code units to the text buffer.
 *
 * @param text UTF-32 code units.
 * @param length Number of code units, otherwise,
 *      negative value to detect null terminator.
 */
COBBLETEXT_API
void cobbletext_engine_add_text_utf32(CobbletextEngine * engine,
    const CobbletextChar32 * text, int32_t length);

/**
 * Append a placeholder for an object to the text buffer.
 *
 * @param id A user-provided value.
 * @param width Width in pixels of the object.
 * @param height Height in pixels of the object.
 */
COBBLETEXT_API
void cobbletext_engine_add_inline_object(CobbletextEngine * engine,
    CobbletextInlineObjectID id, uint32_t width, uint32_t height);

/**
 * Empty the text buffer.
 *
 * This empties the text buffer so the engine can be used to process
 * anther set of text. The properties are not reset and tiles are not cleared.
 */
COBBLETEXT_API
void cobbletext_engine_clear(CobbletextEngine * engine);

/**
 * Clear associated tiles and glyphs.
 *
 * The library context caches glyphs until no engine has a reference to them.
 * This function can be called to reduce memory usage or clear a full
 * texture atlas. This is especially important if your text sources are
 * from user generated content.
 *
 * This function doesn't affect properties or buffered text, and
 * does not affect other engines. If you have associated data structures,
 * such as a texture atlas, remember to clear those too.
 */
COBBLETEXT_API
void cobbletext_engine_clear_tiles(CobbletextEngine * engine);

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
 * If the value is `true`, then a call to `cobbletext_engine_rasterize()`
 * and `cobbletext_engine_pack_tiles()` is not required.
 *
 * If the value is `false`, a new tile and glyph as been associated with this
 * engine instance. If you're using a texture atlas, you will need to rebuild
 * your texture by adding additional or replacing existing tile metadata,
 * and redrawing the entire texture.
 *
 * @pre Call `cobbletext_engine_lay_out()`
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
 *
 * - Can error.
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
 *
 * - Can error
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

#ifdef __cplusplus
}
#endif
