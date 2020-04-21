#pragma once

/**
 * Script direction of a run of text.
 *
 * One of:
 *
 * - `#COBBLETEXT_SCRIPT_DIRECTION_NOT_SPECIFIED`
 * - `#COBBLETEXT_SCRIPT_DIRECTION_LTR`
 * - `#COBBLETEXT_SCRIPT_DIRECTION_RTL`
 */
typedef uint8_t CobbletextScriptDirection;

/**
 * Automatically detect script direction.
 */
#define COBBLETEXT_SCRIPT_DIRECTION_NOT_SPECIFIED 0

/**
 * Force script to be left-to-right.
 */
#define COBBLETEXT_SCRIPT_DIRECTION_LTR 1

/**
 * Force script to be left-to-right.
 */
#define COBBLETEXT_SCRIPT_DIRECTION_RTL 2
