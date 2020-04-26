#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Specifies the encoding of bytes.
 *
 * One of:
 *
 * - `#COBBLETEXT_ENCODING_UTF8`
 * - `#COBBLETEXT_ENCODING_UTF16LE`
 */
typedef uint8_t CobbletextEncoding;

/**
 * UTF-8 encoding
 */
#define COBBLETEXT_ENCODING_UTF8 1

/**
 * UTF-16 little endian encoding
 */
#define COBBLETEXT_ENCODING_UTF16LE 2

#ifdef __cplusplus
}
#endif
