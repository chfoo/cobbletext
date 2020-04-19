#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <uthash.h>

#include <cobbletext.h>

#define ABORT_WITH_MESSAGE(message) { printf((message)); abort(); }
#define ABORT_IF_NULL(value, message) { if ((value) == NULL) { printf((message)); abort(); } }

#define PROPERTY_NONE 0
#define PROPERTY_RED_TEXT 1

#define INLINE_OBJECT_1 1

struct AtlasEntry {
    CobbletextGlyphID glyphID;
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    UT_hash_handle hh;
};

typedef struct ExampleApp {
    CobbletextLibrary * library;
    CobbletextEngine * engine;
    struct AtlasEntry * atlas_table;
    uint8_t * atlas;
    uint32_t atlas_size;
    uint32_t * image;
    uint32_t image_width;
    uint32_t image_height;
    int32_t pen_x;
    int32_t pen_y;
} ExampleApp;


void example_app_run(ExampleApp * app);

void example_app_destroy(ExampleApp * app);

void example_app_check_error(ExampleApp * app);

void example_app_set_text(ExampleApp * app);

void example_app_create_atlas(ExampleApp * app);

void example_app_draw_atlas_tile(ExampleApp * app,
    const struct CobbletextTileInfo * tile,
    const struct CobbletextGlyphInfo * glyph);

void example_app_draw_image(ExampleApp * app);

void example_app_draw_glyph(ExampleApp * app,
    const struct CobbletextAdvanceInfo * advance);

void example_app_save_file(ExampleApp * app);
