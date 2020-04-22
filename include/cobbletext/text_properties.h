#pragma once

#include "common.h"
#include "script_direction.h"

/**
 * Properties for text runs on a `struct CobbletextEngine` instance.
 */
struct CobbletextTextProperties {
    /**
     * The current language as a BCP 47 language tag.
     *
     * This low-level value typically controls the glyph variant selection to
     * use from a font file.
     *
     * Default value is an empty string which indicates automatic detection.
     *
     * A tag is 2 characters or longer.
     *
     * - Getter: Never null.
     * - Setter: May be null. (Null interpreted as empty string).
     */
    const char * language;

    /**
     * The current script as a ISO 15924 string.
     *
     * This low-level value typically controls the rules for shaping glyphs.
     *
     * A ISO 15924 string is a 4 character string such as "Latn".
     *
     * Default value is an empty string which indicates automatic detection.
     *
     * - Getter: Never null.
     * - Setter: May be null. (Null interpreted as empty string).
     */
    const char * script;

    /**
     * The current script direction.
     *
     * Default value "not specified" indicates automatic detection.
     */
    CobbletextScriptDirection script_direction;

    /**
     * The current font face.
     *
     * Default is 0 (not corresponding to a valid font face).
     */
    CobbletextFontID font;

    /**
     * The current font size in points.
     *
     * Default is 12.
     */
    double font_size;

    /**
     * The current user-provided custom property.
     *
     * Default value is 0.
     */
    CobbletextCustomPropertyID custom_property;
};
