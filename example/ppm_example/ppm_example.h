#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <uthash.h>

#include <cobbletext/cobbletext.h>

enum Property {
    PROPERTY_NONE = 0,
    PROPERTY_RED_TEXT = 1
};

#define INLINE_OBJECT_1 1

struct AtlasEntry {
    CobbletextGlyphID glyph_id;
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    int32_t offset_x;
    int32_t offset_y;
    UT_hash_handle hh;
};

typedef struct App {
    CobbletextLibrary * library;
    CobbletextEngine * engine;
    struct AtlasEntry * atlas_table;
    CobbletextFontID fonts[16];
    uint32_t font_count;
    uint32_t line_length;
    uint32_t text_alignment;
    char * locale;
    uint8_t * atlas;
    uint32_t atlas_size;
    uint32_t * image;
    uint32_t image_width;
    uint32_t image_height;
    int32_t pen_x;
    int32_t pen_y;
} App;

void app_init(App * app, int argc, char * argv[]);

void app_print_help(App * app);

void app_run(App * app);

void app_destroy(App * app);

void app_check_error(App * app);

void app_load_font(App * app, char * path);

void app_set_up_font_fallback(App * app);

void app_set_text(App * app);

void app_render_text(App * app);

void app_create_atlas(App * app);

void app_pack_tiles_for_atlas(App * app);

void app_draw_atlas_tile(App * app,
    const struct CobbletextTileInfo * tile,
    const struct CobbletextGlyphInfo * glyph);

void app_draw_image(App * app);

void app_draw_glyph(App * app,
    const struct CobbletextAdvanceInfo * advance);

void app_save_file(App * app);
