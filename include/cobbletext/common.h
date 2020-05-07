#pragma once

#include <stdint.h>

#include "macros.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Unique font face ID.
 *
 * A plain integer that can be used to identify a font face.
 *
 * It will always be unique per library context.
 */
typedef uint32_t CobbletextFontID;

/**
 * Unique glyph ID.
 *
 * A plain integer that can be used to identify a glyph for a font face,
 * at a specific font size.
 *
 * It will always be unique per library context.
 *
 * IDs are recycled to different glyphs if no engine holds references to the
 * ID.
 */
typedef uint32_t CobbletextGlyphID;

/**
 * A user-provided identifer for inline objects.
 *
 * The user decides how to interpret this identifer for custom objects.
 */
typedef uint32_t CobbletextInlineObjectID;

/**
 * A user-provided integer for custom properties.
 *
 * The user decides on how to interpret this integer for implementing custom
 * properties to runs of text.
 *
 * The value could be a ID to a mapping or a direct color value.
 */
typedef uint64_t CobbletextCustomPropertyID;


/**
 * Returns X from the X.Y.Z version.
 */
COBBLETEXT_API
int32_t cobbletext_get_version_major();

/**
 * Returns Y from the X.Y.Z version.
 */
COBBLETEXT_API
int32_t cobbletext_get_version_minor();

/**
 * Returns Z from the X.Y.Z version.
 */
COBBLETEXT_API
int32_t cobbletext_get_version_patch();

/**
 * Returns the version string formatted as X.Y.Z
 *
 * - Do not free.
 */
COBBLETEXT_API
const char * cobbletext_get_version();

#ifdef __cplusplus
}
#endif
