#include "ppm_example.h"

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <getopt.h>

#include "../common/sample_text.h"
#include "../common/macros.h"

int main(int argc, char * argv[]) {
    printf("Start\n");

    App app = {NULL};

    app_init(&app, argc, argv);
    app_run(&app);
    app_destroy(&app);

    printf("Done\n");

    return 0;
}

void app_init(App * app, int argc, char * argv[]) {
    app->library = cobbletext_library_new();
    app_check_error(app);

    app->engine = cobbletext_engine_new(app->library);
    app_check_error(app);

    while (true) {
        int option = getopt(argc, argv, "hf:w:a:l:");

        if (option < 0) {
            break;
        }

        switch (option) {
            case 'h':
                app_print_help(app);
                exit(0);
                break;
            case 'f':
                app_load_font(app, optarg);
                break;
            case 'w': {
                uint32_t line_length = strtoumax(optarg, NULL, 0);
                app->line_length = line_length;
                break;
            }
            case 'a': {
                uint32_t text_alignment = strtoumax(optarg, NULL, 0);
                app->text_alignment = text_alignment;
                break;
            }
            case 'l':
                app->locale = optarg;
                break;
            case '?':
                exit(2);
        }
    }
}

void app_print_help(App * app) {
    fprintf(stderr,
        "Usage: ppm_example [-l LOCALE] [-w LINE_LENGTH] [-a TEXT_ALIGNMENT] [-f FONT_FILE]...\n"
        "-h help\n"
        "-l BCP 47 language tag\n"
        "-w Positive value in pixels for word wrapping. 0 for none.\n"
        "-a Number of {0, 1, 2, 3, 4, 5} to select (not specified, start, end, left, right, center)\n"
        "-f Path to a font file name. Use multiple -f for font fallback. \n"
    );
}

void app_run(App * app) {
    app_set_up_font_fallback(app);
    app_set_text(app);
    app_render_text(app);
    app_draw_image(app);
    app_save_file(app);
}

void app_destroy(App * app) {
    cobbletext_engine_delete(app->engine);
    cobbletext_library_delete(app->library);

    if (app->atlas) {
        free(app->atlas);
    }

    if (app->image) {
        free(app->image);
    }

    if (app->atlas_table) {
        struct AtlasEntry * atlas_entry, * temp;

        HASH_ITER(hh, app->atlas_table, atlas_entry, temp) {
            HASH_DEL(app->atlas_table, atlas_entry);
            free(atlas_entry);
        }
    }
}

void app_check_error(App * app) {
    if (cobbletext_get_error_code(app->library)) {
        int32_t error_code = cobbletext_get_error_code(app->library);
        const char * error_message = cobbletext_get_error_message(app->library);

        fprintf(stderr, "Error: %d %s\n", error_code, error_message);
        ABORT_WITH_MESSAGE("error");
    }
}

void app_load_font(App * app, char * path) {
    if (app->font_count >= 16) {
        fprintf(stderr, "Too many fonts for this program\n");
        return;
    }

    fprintf(stderr, "Loading font %s\n", path);


    app->fonts[app->font_count] = cobbletext_library_load_font(
        app->library, path);
    app_check_error(app);
    app->font_count++;
}


void app_set_up_font_fallback(App * app) {
    for (size_t index = 0; index < app->font_count; index++) {
        if (index < app->font_count - 1) {
            fprintf(stderr, "Setting font alternative %d->%d\n",
                app->fonts[index], app->fonts[index + 1]);
            cobbletext_library_set_font_alternative(app->library,
                app->fonts[index], app->fonts[index + 1]);
        }
    }
}

void app_set_text(App * app) {
    fprintf(stderr, "Adding text and objects...\n");

    struct CobbletextEngineProperties properties = {0};

    if (app->locale) {
        properties.locale = app->locale;
    }

    properties.line_length = app->line_length;
    properties.text_alignment = app->text_alignment;
    cobbletext_engine_set_properties(app->engine, &properties);

    struct CobbletextTextProperties text_properties = {0};
    text_properties.font_size = 16;
    text_properties.custom_property = PROPERTY_NONE;

    if (app->font_count) {
        text_properties.font = app->fonts[0];
    }

    cobbletext_engine_set_text_properties(app->engine, &text_properties);

    cobbletext_engine_add_text_utf8(app->engine, EAT_GLASS_TEXT, -1);
    cobbletext_engine_add_text_utf8(app->engine, "\n", -1);

    text_properties.font_size = 10;
    text_properties.custom_property = PROPERTY_RED_TEXT;
    cobbletext_engine_set_text_properties(app->engine, &text_properties);
    cobbletext_engine_add_text_utf8(app->engine, "Inline", -1);
    cobbletext_engine_add_inline_object(app->engine, INLINE_OBJECT_1, 50);
    cobbletext_engine_add_text_utf8(app->engine, "object", -1);
    cobbletext_engine_add_text_utf8(app->engine, "\n", -1);

    text_properties.font_size = 16;
    text_properties.custom_property = PROPERTY_NONE;
    cobbletext_engine_set_text_properties(app->engine, &text_properties);
    cobbletext_engine_add_text_utf8(app->engine, EMOJI_TEXT, -1);
}

void app_render_text(App * app) {
    cobbletext_engine_lay_out(app->engine);
    app_check_error(app);

    if (!cobbletext_engine_tiles_valid(app->engine)) {
        app_create_atlas(app);
    }
}

void app_create_atlas(App * app) {
    fprintf(stderr, "Rasterizing...\n");
    cobbletext_engine_rasterize(app->engine);
    app_check_error(app);

    fprintf(stderr, "Creating atlas...\n");
    app_pack_tiles_for_atlas(app);

    if (app->atlas) {
        free(app->atlas);
    }

    app->atlas = calloc(app->atlas_size * app->atlas_size, 1);
    ABORT_IF_NULL(app->atlas, "atlas malloc fail")

    cobbletext_engine_prepare_tiles(app->engine);
    app_check_error(app);

    uint32_t tile_count = cobbletext_engine_get_tile_count(app->engine);
    const struct CobbletextTileInfo ** tiles =
        cobbletext_engine_get_tiles(app->engine);

    for (size_t index = 0; index < tile_count; index++) {
        const struct CobbletextTileInfo * tile = tiles[index];
        const struct CobbletextGlyphInfo * glyph =
            cobbletext_library_get_glyph_info(app->library, tile->glyph_id);

        app_check_error(app);
        app_draw_atlas_tile(app, tile, glyph);

        struct AtlasEntry * atlas_entry;

        HASH_FIND_INT(app->atlas_table, &tile->glyph_id, atlas_entry);
        if (atlas_entry) { ABORT_WITH_MESSAGE("tile with duplicate key") }

        atlas_entry = calloc(1, sizeof(*atlas_entry));
        atlas_entry->glyph_id = tile->glyph_id;
        atlas_entry->x = tile->atlas_x;
        atlas_entry->y = tile->atlas_y;
        atlas_entry->width = glyph->image_width;
        atlas_entry->height = glyph->image_height;
        atlas_entry->offset_x = glyph->image_offset_x;
        atlas_entry->offset_y = glyph->image_offset_y;

        HASH_ADD_INT(app->atlas_table, glyph_id, atlas_entry);
    }
}

void app_pack_tiles_for_atlas(App * app) {
    if (app->atlas_size == 0) {
        app->atlas_size = 128;
    }

    while (true) {
        bool hasOverflow = cobbletext_engine_pack_tiles(app->engine,
            app->atlas_size, app->atlas_size);

        if (!hasOverflow) {
            break;
        }

        app->atlas_size *= 2;
    }
}

void app_draw_atlas_tile(App * app,
        const struct CobbletextTileInfo * tile,
        const struct CobbletextGlyphInfo * glyph) {

    size_t glyph_image_length = glyph->image_width * glyph->image_height;

    for (size_t index = 0; index < glyph_image_length; index++) {
        size_t glyph_image_x = index % glyph->image_width;
        size_t glyph_image_y = index / glyph->image_width;
        size_t atlas_x = tile->atlas_x + glyph_image_x;
        size_t atlas_y = tile->atlas_y + glyph_image_y;
        size_t atlas_index = app->atlas_size * atlas_y + atlas_x;

        app->atlas[atlas_index] = glyph->image[index];
    }
}

void app_draw_image(App * app) {
    fprintf(stderr, "Drawing output...\n");

    if (app->image) {
        free(app->image);
    }

    const struct CobbletextOutputInfo * output_info =
        cobbletext_engine_get_output_info(app->engine);

    app->image_width = output_info->text_width;
    app->image_height = output_info->text_height;
    size_t image_size = app->image_width * app->image_height;

    app->image = calloc(image_size, sizeof(*app->image));
    ABORT_IF_NULL(app->image, "image malloc fail")

    memset(app->image, 0xff, image_size * sizeof(*app->image));

    cobbletext_engine_prepare_advances(app->engine);
    app_check_error(app);

    uint32_t advances_count = cobbletext_engine_get_advance_count(app->engine);
    const struct CobbletextAdvanceInfo ** advances =
        cobbletext_engine_get_advances(app->engine);

    for (size_t index = 0; index < advances_count; index++) {
        const struct CobbletextAdvanceInfo * advance = advances[index];

        switch (advance->type) {
            case COBBLETEXT_ADVANCE_TYPE_GLYPH:
                app_draw_glyph(app, advance);
                break;
        }

        app->pen_x += advance->advance_x;
        app->pen_y += advance->advance_y;
    }
}

void app_draw_glyph(App * app,
        const struct CobbletextAdvanceInfo * advance) {

    struct AtlasEntry * atlas_entry;
    HASH_FIND_INT(app->atlas_table, &advance->glyph_id, atlas_entry);
    ABORT_IF_NULL(atlas_entry, "atlas entry empty");

    size_t tile_length = atlas_entry->width * atlas_entry->height;

    for (size_t index = 0; index < tile_length; index++) {
        size_t tile_x = index % atlas_entry->width;
        size_t tile_y = index / atlas_entry->width;
        size_t atlas_x = atlas_entry->x + tile_x;
        size_t atlas_y = atlas_entry->y + tile_y;
        size_t atlas_index = app->atlas_size * atlas_y + atlas_x;

        size_t image_x = app->pen_x + atlas_entry->offset_x
            + advance->glyph_offset_x + tile_x;
        size_t image_y = app->pen_y + atlas_entry->offset_y
            + advance->glyph_offset_y + tile_y;
        size_t image_index = app->image_width * image_y + image_x;

        if (image_x < 0 || image_y < 0
        || image_x >= app->image_width || image_y >= app->image_height) {
            continue;
        }

        uint32_t background = app->image[image_index];
        uint32_t color;

        if (advance->custom_property == PROPERTY_RED_TEXT) {
            color = 0xff0000;
        } else {
            color = 0x000000;
        }

        uint8_t coverage = app->atlas[atlas_index];
        uint32_t foreground = (coverage << 24) | color;

        uint32_t pixel = cobbletext_math_alpha_blend_over_argb(background,
            foreground);
        pixel = cobbletext_math_gamma_correction_argb(pixel, 1.8);

        app->image[image_index] = pixel;
    }
}

void app_save_file(App * app) {
    fprintf(stderr, "Saving file...\n");

    FILE * atlas_file = fopen("ppm_example_output_atlas.pgm", "wb");

    fprintf(atlas_file, "P5 %d %d 255\n", app->atlas_size, app->atlas_size);

    size_t atlas_length = app->atlas_size * app->atlas_size;

    for (size_t index = 0; index < atlas_length; index++) {
        fputc(app->atlas[index], atlas_file);
    }

    fclose(atlas_file);

    FILE * file = fopen("ppm_example_output_image.ppm", "wb");

    fprintf(file, "P6 %d %d 255\n", app->image_width, app->image_height);

    size_t length = app->image_width * app->image_height;

    for (size_t index = 0; index < length; index++) {
        uint32_t pixel = app->image[index];

        fputc((pixel >> 16) & 0xff, file);
        fputc((pixel >> 8) & 0xff, file);
        fputc((pixel >> 0) & 0xff, file);
    }

    fclose(file);
}
