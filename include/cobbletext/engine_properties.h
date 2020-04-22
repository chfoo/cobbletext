#pragma once

#include <stdint.h>

#include "text_alignment.h"

/**
 * Properties for controlling output parameters on a
 * `struct CobbletextEngine` instance.
 */
struct CobbletextEngineProperties {
    /**
     * Line length in pixels.
     *
     * If the value is 0, no word wrapping is performed. Otherwise, lines will
     * be word wrapped at the given pixel width.
     */
    uint32_t line_length;

    /**
     * Locale as a BCP 47 language tag.
     *
     * This value is used for assisting lower-level functions to tailor the
     * presention of the text to your application's user. It is typically
     * the GUI's locale or a document's language.
     *
     * The default is an empty string which indicates automatic detection of
     * the user's locale if possible.
     *
     * A tag is 2 characters or longer.
     *
     * - Getter: Never null.
     * - Setter: May be null. (Null interpreted as empty string).
     */
    const char * locale;

    /**
     * Controls the alignment of text in each line.
     *
     * Default is not specified.
     */
    CobbletextTextAlignment text_alignment;
};
