#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Alignment of text within a line.
 *
 * One of:
 *
 * - `#COBBLETEXT_TEXT_ALIGNMENT_NOT_SPECIFIED`
 * - `#COBBLETEXT_TEXT_ALIGNMENT_START`
 * - `#COBBLETEXT_TEXT_ALIGNMENT_END`
 * - `#COBBLETEXT_TEXT_ALIGNMENT_LEFT`
 * - `#COBBLETEXT_TEXT_ALIGNMENT_RIGHT`
 * - `#COBBLETEXT_TEXT_ALIGNMENT_CENTER`
 */
typedef uint8_t CobbletextTextAlignment;

/**
 * Default alias for `#COBBLETEXT_TEXT_ALIGNMENT_START`
 */
#define COBBLETEXT_TEXT_ALIGNMENT_NOT_SPECIFIED 0

/**
 * Left-align text if LTR and right-align text if RTL.
 */
#define COBBLETEXT_TEXT_ALIGNMENT_START 1

/**
 * Right-align text if LTR and left-align text if RTL.
 */
#define COBBLETEXT_TEXT_ALIGNMENT_END 2

/**
 * Force lines to be left-aligned.
 */
#define COBBLETEXT_TEXT_ALIGNMENT_LEFT 3

/**
 * Force lines to be right-aligned.
 */
#define COBBLETEXT_TEXT_ALIGNMENT_RIGHT 4

/**
 * Centers each line.
 */
#define COBBLETEXT_TEXT_ALIGNMENT_CENTER 5

#ifdef __cplusplus
}
#endif
