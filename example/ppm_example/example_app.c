#include "example_app.h"

#include <malloc.h>
#include <stdio.h>
#include <inttypes.h>

#include "sample_text.h"

void example_app_run(ExampleApp * app) {
    app->library = cobbletext_library_new();
    example_app_check_error(app);
    app->engine = cobbletext_engine_new(app->library);
    example_app_check_error(app);

    example_app_load_font(app);

    example_app_set_text(app);

    cobbletext_engine_lay_out(app->engine);
    example_app_check_error(app);

    if (!cobbletext_engine_tiles_valid(app->engine)) {
        example_app_create_atlas(app);
    }

    example_app_draw_image(app);
    example_app_save_file(app);

    cobbletext_engine_delete(app->engine);
    cobbletext_library_delete(app->library);
    example_app_destroy(app);
}

void example_app_destroy(ExampleApp * app) {
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

void example_app_check_error(ExampleApp * app) {
    if (cobbletext_get_error_code(app->library)) {
        int32_t error_code = cobbletext_get_error_code(app->library);
        const char * error_message = cobbletext_get_error_message(app->library);

        printf("Error: %d %s\n", error_code, error_message);
        ABORT_WITH_MESSAGE("error");
    }
}

void example_app_load_font(ExampleApp * app) {
    char * font_path = getenv("FONT_PATH");

    if (!font_path) {
        printf("*** You didn't specify a font file! ***\n"
        "Please run the program with an environment variable "
        "name 'FONT_PATH' and value containing a file path to a font file.\n"
        "Use export FONT_PATH=/usr/share/fonts/your_path_here\n"
        "or $env:PATH = $Env:WinDir\\Fonts\\your_filename_here\n"
        "set to a session enviornment variable.\n"
        "");

        return;
    }

    app->fontID = cobbletext_library_load_font(app->library, font_path);
    example_app_check_error(app);
}

void example_app_set_text(ExampleApp * app) {
    printf("Adding text and objects...\n");

    char * line_length_string = getenv("LINE_LENGTH");
    uint32_t line_length = 0;

    if (!line_length_string) {
        printf("*** You didn't set a line length! ***\n"
        "Specity a line length like you did with enviornment variable FONT_PATH,\n"
        "or ignore this warning and we'll draw it without line breaks.\n"
        );
    } else {
        line_length = strtoumax(line_length_string, NULL, 0);

        if (line_length) {
            printf("*** Invalid line length ***\n");
        }
    }

    cobbletext_engine_set_locale(app->engine, "en-US");
    cobbletext_engine_set_line_length(app->engine, line_length);
    cobbletext_engine_set_font_size(app->engine, 16);
    cobbletext_engine_set_custom_property(app->engine, PROPERTY_NONE);
    cobbletext_engine_set_font(app->engine, app->fontID);

    cobbletext_engine_add_text_utf8(app->engine, EAT_GLASS_TEXT, -1);
    cobbletext_engine_add_text_utf8(app->engine, "\n", -1);

    cobbletext_engine_set_font_size(app->engine, 10);
    cobbletext_engine_set_custom_property(app->engine, PROPERTY_RED_TEXT);
    cobbletext_engine_add_text_utf8(app->engine, "Inline", -1);
    cobbletext_engine_add_inline_object(app->engine, INLINE_OBJECT_1, 50);
    cobbletext_engine_add_text_utf8(app->engine, "object", -1);
}

void example_app_create_atlas(ExampleApp * app) {
    printf("Rasterizing...\n");
    cobbletext_engine_rasterize(app->engine);
    example_app_check_error(app);

    printf("Creating atlas...\n");

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

    if (app->atlas) {
        free(app->atlas);
    }

    app->atlas = calloc(app->atlas_size * app->atlas_size, 1);
    ABORT_IF_NULL(app->atlas, "atlas malloc fail")

    cobbletext_engine_prepare_tiles(app->engine);
    example_app_check_error(app);

    uint32_t tile_count = cobbletext_engine_get_tile_count(app->engine);
    const struct CobbletextTileInfo ** tiles =
        cobbletext_engine_get_tiles(app->engine);

    for (size_t index = 0; index < tile_count; index++) {
        const struct CobbletextTileInfo * tile = tiles[index];
        const struct CobbletextGlyphInfo * glyph =
            cobbletext_library_get_glyph_info(app->library, tile->glyph_id);

        example_app_check_error(app);
        example_app_draw_atlas_tile(app, tile, glyph);

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

void example_app_draw_atlas_tile(ExampleApp * app,
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

void example_app_draw_image(ExampleApp * app) {
    printf("Drawing output...\n");

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
    example_app_check_error(app);

    uint32_t advances_count = cobbletext_engine_get_advance_count(app->engine);
    const struct CobbletextAdvanceInfo ** advances =
        cobbletext_engine_get_advances(app->engine);

    for (size_t index = 0; index < advances_count; index++) {
        const struct CobbletextAdvanceInfo * advance = advances[index];

        switch (advance->type) {
            case COBBLETEXT_ADVANCE_TYPE_GLYPH:
                example_app_draw_glyph(app, advance);
                break;
        }

        app->pen_x += advance->advance_x;
        app->pen_y += advance->advance_y;
    }
}

void example_app_draw_glyph(ExampleApp * app,
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

        size_t image_x = app->pen_x + atlas_entry->offset_x + advance->glyph_offset_x + tile_x;
        size_t image_y = app->pen_y + atlas_entry->offset_y + advance->glyph_offset_y + tile_y;
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

void example_app_save_file(ExampleApp * app) {
    printf("Saving file...\n");

    FILE * atlas_file = fopen("example_atlas.pgm", "wb");

    fprintf(atlas_file, "P5 %d %d 255\n", app->atlas_size, app->atlas_size);

    size_t atlas_length = app->atlas_size * app->atlas_size;

    for (size_t index = 0; index < atlas_length; index++) {
        fputc(app->atlas[index], atlas_file);
    }

    fclose(atlas_file);

    FILE * file = fopen("example.ppm", "wb");

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
