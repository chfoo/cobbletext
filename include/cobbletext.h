#ifndef COBBLETEXT_COBBLETEXT_H_
#define COBBLETEXT_COBBLETEXT_H_


/**
 * Cobbletext: complex text layout and rendering engine library
 *
 * C API
 */


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>

#ifdef _WIN32
    #ifdef COBBLETEXT_EXPORTS
        #define COBBLETEXT_API __declspec(dllexport)
    #else
        #define COBBLETEXT_API __declspec(dllimport)
    #endif
#else
    #define COBBLETEXT_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct CobbletextLibrary;
typedef struct CobbletextLibrary CobbletextLibrary;

struct CobbletextEngine;
typedef struct CobbletextEngine CobbletextEngine;

typedef uint32_t CobbletextFontID;

typedef uint32_t CobbletextGlyphID;

typedef uint8_t CobbletextScriptDirection;
#define COBBLETEXT_SCRIPT_DIRECTION_NOT_SPECIFIED 0
#define COBBLETEXT_SCRIPT_DIRECTION_LTR 1
#define COBBLETEXT_SCRIPT_DIRECTION_RTL 2

typedef uint8_t CobbletextEncoding;
#define COBBLETEXT_ENCODING_UTF8 1
#define COBBLETEXT_ENCODING_UTF16LE 2

typedef uint32_t CobbletextInlineObjectID;

typedef uint64_t CobbletextCustomPropertyID;

typedef uint8_t CobbletextAdvanceType;
#define COBBLETEXT_ADVANCE_TYPE_INVALID 0
#define COBBLETEXT_ADVANCE_TYPE_GLYPH 1
#define COBBLETEXT_ADVANCE_TYPE_INLINE_OBJECT 2
#define COBBLETEXT_ADVANCE_TYPE_LINE_BREAK 3
#define COBBLETEXT_ADVANCE_TYPE_BIDI 4
#define COBBLETEXT_ADVANCE_TYPE_LAYOUT 5

struct CobbletextFontInfo {
    CobbletextFontID id;
    const char * family_name;
    const char * style_name;
    uint16_t units_per_em;
    int16_t ascender;
    int16_t descender;
    int16_t height;
    int16_t underline_position;
    int16_t underline_thickness;
};

struct CobbletextGlyphInfo {
    CobbletextGlyphID id;
    const uint8_t * image;
    uint32_t image_width;
    uint32_t image_height;
    int32_t image_offset_x;
    int32_t image_offset_y;
};

struct CobbletextTileInfo {
    CobbletextGlyphID glyph_id;
    uint32_t atlas_x;
    uint32_t atlas_y;
};

struct CobbletextAdvanceInfo {
    CobbletextAdvanceType type;
    uint32_t text_index;

    int32_t advance_x;
    int32_t advance_y;

    CobbletextGlyphID glyph_id;
    int32_t glyph_offset_x;
    int32_t glyph_offset_y;

    CobbletextInlineObjectID inline_object;

    CobbletextCustomPropertyID custom_property;
};

struct CobbletextOutputInfo {
    uint32_t text_width;
    uint32_t text_height;
};

#ifdef COBBLETEXT_TYPEDEF_STRUCTS
    typedef struct CobbletextFontInfo CobbletextFontInfo;
    typedef struct CobbletextGlyphInfo CobbletextGlyphInfo;
    typedef struct CobbletextTileInfo CobbletextTileInfo;
    typedef struct CobbletextAdvanceInfo CobbletextAdvanceInfo;
    typedef struct CobbletextOutputInfo CobbletextOutputInfo;
#endif

COBBLETEXT_API
int32_t cobbletext_get_version_major();

COBBLETEXT_API
int32_t cobbletext_get_version_minor();

COBBLETEXT_API
int32_t cobbletext_get_version_patch();

COBBLETEXT_API
const char * cobbletext_get_version();


COBBLETEXT_API
CobbletextLibrary * cobbletext_library_new();

COBBLETEXT_API
void cobbletext_library_delete(CobbletextLibrary * library);

COBBLETEXT_API
int32_t cobbletext_get_error_code(CobbletextLibrary * library);

COBBLETEXT_API
const char * cobbletext_get_error_message(CobbletextLibrary * library);

COBBLETEXT_API
void cobbletext_clear_error(CobbletextLibrary * library);

COBBLETEXT_API
CobbletextFontID cobbletext_library_get_fallback_font(
    CobbletextLibrary * library);

COBBLETEXT_API
CobbletextFontID cobbletext_library_load_font(CobbletextLibrary * library,
    const char * path);

COBBLETEXT_API
CobbletextFontID cobbletext_library_load_font_bytes(
    CobbletextLibrary * library, const uint8_t * data, uint32_t length);

COBBLETEXT_API
const struct CobbletextFontInfo * cobbletext_library_get_font_info(
    CobbletextLibrary * library, CobbletextFontID font);

COBBLETEXT_API
const struct CobbletextGlyphInfo * cobbletext_library_get_glyph_info(
    CobbletextLibrary * library, CobbletextFontID glyph);

COBBLETEXT_API
CobbletextEngine * cobbletext_engine_new(CobbletextLibrary * library);

COBBLETEXT_API
void cobbletext_engine_delete(CobbletextEngine * engine);

COBBLETEXT_API
uint32_t cobbletext_engine_get_line_length(CobbletextEngine * engine);

COBBLETEXT_API
void cobbletext_engine_set_line_length(CobbletextEngine * engine,
    uint32_t length);

COBBLETEXT_API
const char * cobbletext_engine_get_locale(CobbletextEngine * engine);

COBBLETEXT_API
void cobbletext_engine_set_locale(CobbletextEngine * engine,
    const char * locale);

COBBLETEXT_API
const char * cobbletext_engine_get_language(CobbletextEngine * engine);

COBBLETEXT_API
void cobbletext_engine_set_language(CobbletextEngine * engine,
    const char * language);

COBBLETEXT_API
const char * cobbletext_engine_get_script(CobbletextEngine * engine);

COBBLETEXT_API
void cobbletext_engine_set_script(CobbletextEngine * engine,
    const char * script);

COBBLETEXT_API
CobbletextScriptDirection cobbletext_engine_get_script_direction(
    CobbletextEngine * engine);

COBBLETEXT_API
void cobbletext_engine_set_script_direction(CobbletextEngine * engine,
    CobbletextScriptDirection direction);

COBBLETEXT_API
CobbletextFontID cobbletext_engine_get_font(CobbletextEngine * engine);

COBBLETEXT_API
void cobbletext_engine_set_font(CobbletextEngine * engine,
    CobbletextFontID font);

COBBLETEXT_API
double cobbletext_engine_get_font_size(CobbletextEngine * engine);

COBBLETEXT_API
void cobbletext_engine_set_font_size(CobbletextEngine * engine, double size);

COBBLETEXT_API
CobbletextCustomPropertyID cobbletext_engine_get_custom_property(
    CobbletextEngine * engine);

COBBLETEXT_API
void cobbletext_engine_set_custom_property(CobbletextEngine * engine,
    CobbletextCustomPropertyID id);

COBBLETEXT_API
void cobbletext_engine_add_text(CobbletextEngine * engine, const uint8_t * data,
    int32_t length, CobbletextEncoding encoding);

COBBLETEXT_API
void cobbletext_engine_add_text_utf8(CobbletextEngine * engine,
    const char * text, int32_t length);

COBBLETEXT_API
void cobbletext_engine_add_text_utf16(CobbletextEngine * engine,
    const char16_t * text, int32_t length);

COBBLETEXT_API
void cobbletext_engine_add_text_utf32(CobbletextEngine * engine,
    const char32_t * text, int32_t length);

COBBLETEXT_API
void cobbletext_engine_add_inline_object(CobbletextEngine * engine,
    CobbletextInlineObjectID id, uint32_t size);

COBBLETEXT_API
void cobbletext_engine_clear(CobbletextEngine * engine);

COBBLETEXT_API
void cobbletext_engine_lay_out(CobbletextEngine * engine);

COBBLETEXT_API
bool cobbletext_engine_tiles_valid(CobbletextEngine * engine);

COBBLETEXT_API
void cobbletext_engine_rasterize(CobbletextEngine * engine);

COBBLETEXT_API
bool cobbletext_engine_pack_tiles(CobbletextEngine * engine, uint32_t width,
    uint32_t height);

COBBLETEXT_API
void cobbletext_engine_prepare_tiles(CobbletextEngine * engine);

COBBLETEXT_API
uint32_t cobbletext_engine_get_tile_count(CobbletextEngine * engine);

COBBLETEXT_API
const struct CobbletextTileInfo ** cobbletext_engine_get_tiles(
    CobbletextEngine * engine);

COBBLETEXT_API
void cobbletext_engine_prepare_advances(CobbletextEngine * engine);

COBBLETEXT_API
uint32_t cobbletext_engine_get_advance_count(CobbletextEngine * engine);

COBBLETEXT_API
const struct CobbletextAdvanceInfo ** cobbletext_engine_get_advances(
    CobbletextEngine * engine);

COBBLETEXT_API
const struct CobbletextOutputInfo * cobbletext_engine_get_output_info(
    CobbletextEngine * engine);


COBBLETEXT_API
uint32_t cobbletext_math_alpha_blend_over_argb(uint32_t background,
    uint32_t foreground);

COBBLETEXT_API
uint32_t cobbletext_math_gamma_correction_argb(uint32_t color, double gamma);


#ifdef __cplusplus
}
#endif


#endif
