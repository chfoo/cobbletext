#include <catch2/catch.hpp>

#include "cobbletext/cobbletext.h"

TEST_CASE("engine create delete") {
    CobbletextLibrary * library = cobbletext_library_new();
    CobbletextEngine * engine = cobbletext_engine_new(library);

    REQUIRE_FALSE( cobbletext_get_error_code(library) );
    CHECK_THAT( cobbletext_get_error_message(library), Catch::Equals("") );

    cobbletext_engine_delete(engine);
    cobbletext_library_delete(library);
}

TEST_CASE("engine getter/setter") {
    CobbletextLibrary * library = cobbletext_library_new();
    CobbletextEngine * engine = cobbletext_engine_new(library);

    CobbletextEngineProperties properties = {0};
    properties.line_length = 200;
    properties.locale = "zz";
    properties.text_alignment = COBBLETEXT_TEXT_ALIGNMENT_CENTER;
    cobbletext_engine_set_properties(engine, &properties);

    CobbletextTextProperties textProperties = {0};
    textProperties.custom_property = 123;
    textProperties.font = 0;
    textProperties.font_size = 12;
    textProperties.language = "xx";
    textProperties.script = "xxxx";
    textProperties.script_direction = COBBLETEXT_SCRIPT_DIRECTION_LTR;
    cobbletext_engine_set_text_properties(engine, &textProperties);

    const CobbletextEngineProperties * properties2 =
        cobbletext_engine_get_properties(engine);

    CHECK( properties2->line_length == 200 );
    CHECK_THAT( properties2->locale, Catch::Equals("zz") );
    CHECK( properties2->text_alignment == COBBLETEXT_TEXT_ALIGNMENT_CENTER );

    const CobbletextTextProperties * textProperties2 =
        cobbletext_engine_get_text_properties(engine);

    CHECK( textProperties2->custom_property == 123);
    CHECK( textProperties2->font == 0);
    CHECK( textProperties2->font_size == 12);
    CHECK_THAT( textProperties2->language, Catch::Equals("xx") );
    CHECK_THAT( textProperties2->script, Catch::Equals("xxxx") );
    CHECK( textProperties2->script_direction == COBBLETEXT_SCRIPT_DIRECTION_LTR);

    cobbletext_engine_delete(engine);
    cobbletext_library_delete(library);
}

TEST_CASE("engine text empty") {
    CobbletextLibrary * library = cobbletext_library_new();
    CobbletextEngine * engine = cobbletext_engine_new(library);

    cobbletext_engine_lay_out(engine);
    cobbletext_engine_prepare_advances(engine);

    uint32_t advance_count = cobbletext_engine_get_advance_count(engine);
    const CobbletextAdvanceInfo ** advances =
        cobbletext_engine_get_advances(engine);

    CHECK( advance_count == 0 );
    CHECK( advances );

    cobbletext_engine_delete(engine);
    cobbletext_library_delete(library);
}

TEST_CASE("engine text 1 newline") {
    CobbletextLibrary * library = cobbletext_library_new();
    CobbletextEngine * engine = cobbletext_engine_new(library);

    cobbletext_engine_add_text_utf8(engine, "\n", -1);
    cobbletext_engine_lay_out(engine);
    cobbletext_engine_prepare_advances(engine);

    uint32_t advance_count = cobbletext_engine_get_advance_count(engine);
    const CobbletextAdvanceInfo ** advances =
        cobbletext_engine_get_advances(engine);

    // Move pen to first baseline, then nothing happens
    CHECK( advance_count == 1 );
    CHECK( advances );

    cobbletext_engine_delete(engine);
    cobbletext_library_delete(library);
}

TEST_CASE("engine text") {
    CobbletextLibrary * library = cobbletext_library_new();
    CobbletextEngine * engine = cobbletext_engine_new(library);

    // Test with 1 codepoint: U+1F600 GRINNING FACE

    SECTION("add bytes UTF-8") {
        uint8_t text[] = {0xF0, 0x9F, 0x98, 0x80};
        cobbletext_engine_add_text(engine, text, 4, COBBLETEXT_ENCODING_UTF8);
    }

    SECTION("add bytes UTF-16LE") {
        uint8_t text[] = {0x3D, 0xD8, 0x00, 0xDE};
        cobbletext_engine_add_text(engine, text, 4, COBBLETEXT_ENCODING_UTF16LE);
    }

    SECTION("add text 8") {
        unsigned char text[] = {0xF0, 0x9F, 0x98, 0x80, 0};
        cobbletext_engine_add_text_utf8(engine, reinterpret_cast<const char *>(text), -1);
    }

    SECTION("add text 16") {
        CobbletextChar16 text[] = {0xD83D, 0xDE00, 0};
        cobbletext_engine_add_text_utf16(engine, text, -1);
    }

    SECTION("add text 32") {
        CobbletextChar32 text[] = {0x1F600, 0};
        cobbletext_engine_add_text_utf32(engine, text, -1);
    }

    cobbletext_engine_lay_out(engine);
    cobbletext_engine_prepare_advances(engine);

    uint32_t advance_count = cobbletext_engine_get_advance_count(engine);
    const CobbletextAdvanceInfo ** advances =
        cobbletext_engine_get_advances(engine);

    // Move pen to base line, then draw the glyph
    REQUIRE( advance_count == 2 );
    REQUIRE( advances );

    CHECK( advances[0]->type == COBBLETEXT_ADVANCE_TYPE_LAYOUT );
    CHECK( advances[1]->type == COBBLETEXT_ADVANCE_TYPE_GLYPH );

    cobbletext_engine_delete(engine);
    cobbletext_library_delete(library);
}

TEST_CASE("engine inline object") {
    CobbletextLibrary * library = cobbletext_library_new();
    CobbletextEngine * engine = cobbletext_engine_new(library);

    cobbletext_engine_add_inline_object(engine, 123, 400);
    cobbletext_engine_lay_out(engine);
    cobbletext_engine_prepare_advances(engine);

    uint32_t advance_count = cobbletext_engine_get_advance_count(engine);
    const CobbletextAdvanceInfo ** advances =
        cobbletext_engine_get_advances(engine);

    // Move pen to base line, then draw the inline object
    REQUIRE( advance_count == 2 );
    REQUIRE( advances );

    CHECK( advances[0]->type == COBBLETEXT_ADVANCE_TYPE_LAYOUT );
    CHECK( advances[1]->type == COBBLETEXT_ADVANCE_TYPE_INLINE_OBJECT );
    CHECK( advances[1]->inline_object == 123 );
    CHECK( advances[1]->advance_x == 400 );

    cobbletext_engine_delete(engine);
    cobbletext_library_delete(library);
}

TEST_CASE("engine custom property") {
    CobbletextLibrary * library = cobbletext_library_new();
    CobbletextEngine * engine = cobbletext_engine_new(library);

    CobbletextTextProperties textProperties = {0};

    textProperties.custom_property = 123;
    cobbletext_engine_set_text_properties(engine, &textProperties);
    cobbletext_engine_add_text_utf8(engine, "a", -1);

    textProperties.custom_property = 456;
    cobbletext_engine_set_text_properties(engine, &textProperties);
    cobbletext_engine_add_text_utf8(engine, "b", -1);

    cobbletext_engine_lay_out(engine);
    cobbletext_engine_prepare_advances(engine);

    uint32_t advance_count = cobbletext_engine_get_advance_count(engine);
    const CobbletextAdvanceInfo ** advances =
        cobbletext_engine_get_advances(engine);

    // Move pen to base line, then draw 2 glyphs
    REQUIRE( advance_count == 3 );
    REQUIRE( advances );

    CHECK( advances[0]->type == COBBLETEXT_ADVANCE_TYPE_LAYOUT );
    CHECK( advances[1]->type == COBBLETEXT_ADVANCE_TYPE_GLYPH );
    CHECK( advances[1]->custom_property == 123 );
    CHECK( advances[2]->type == COBBLETEXT_ADVANCE_TYPE_GLYPH );
    CHECK( advances[2]->custom_property == 456 );

    cobbletext_engine_delete(engine);
    cobbletext_library_delete(library);
}

TEST_CASE("engine tile validity") {
    CobbletextLibrary * library = cobbletext_library_new();
    CobbletextEngine * engine = cobbletext_engine_new(library);

    cobbletext_engine_lay_out(engine);

    CHECK( cobbletext_engine_tiles_valid(engine) );

    cobbletext_engine_add_text_utf8(engine, "hello world", -1);
    cobbletext_engine_lay_out(engine);

    CHECK_FALSE( cobbletext_engine_tiles_valid(engine) );

    cobbletext_engine_lay_out(engine);

    CHECK( cobbletext_engine_tiles_valid(engine) );

    cobbletext_engine_clear(engine);
    cobbletext_engine_lay_out(engine);

    CHECK( cobbletext_engine_tiles_valid(engine) );

    cobbletext_engine_delete(engine);
    cobbletext_library_delete(library);
}

TEST_CASE("engine clear glyphs") {
    CobbletextLibrary * library = cobbletext_library_new();
    CobbletextEngine * engine = cobbletext_engine_new(library);

    cobbletext_engine_add_text_utf8(engine, "h", -1);
    cobbletext_engine_lay_out(engine);

    cobbletext_engine_prepare_tiles(engine);
    uint32_t tile_count = cobbletext_engine_get_tile_count(engine);
    const CobbletextTileInfo ** tiles =
        cobbletext_engine_get_tiles(engine);

    CHECK( tiles[0]->glyph_id == 1 );

    cobbletext_library_clear_glyphs(library);

    cobbletext_engine_add_text_utf8(engine, "b", -1);
    cobbletext_engine_lay_out(engine);

    cobbletext_engine_prepare_tiles(engine);
    tile_count = cobbletext_engine_get_tile_count(engine);
    tiles = cobbletext_engine_get_tiles(engine);

    CHECK( tiles[0]->glyph_id == 1 );

    cobbletext_engine_delete(engine);
    cobbletext_library_delete(library);
}

TEST_CASE("engine raster and tile packing") {
    CobbletextLibrary * library = cobbletext_library_new();
    CobbletextEngine * engine = cobbletext_engine_new(library);

    cobbletext_engine_add_text_utf8(engine, "hello world", -1);
    cobbletext_engine_lay_out(engine);

    REQUIRE_FALSE( cobbletext_get_error_code(library) );
    CHECK_THAT( cobbletext_get_error_message(library), Catch::Equals("") );

    CHECK_FALSE( cobbletext_engine_tiles_valid(engine) );

    cobbletext_engine_rasterize(engine);

    REQUIRE_FALSE( cobbletext_get_error_code(library) );
    CHECK_THAT( cobbletext_get_error_message(library), Catch::Equals("") );

    uint32_t texture_size = 2;

    while (true) {
        bool hasOverflow =
            cobbletext_engine_pack_tiles(engine, texture_size, texture_size);

        if (!hasOverflow) {
            break;
        }

        texture_size *= 2;
    }

    cobbletext_engine_prepare_tiles(engine);

    REQUIRE_FALSE( cobbletext_get_error_code(library) );
    CHECK_THAT( cobbletext_get_error_message(library), Catch::Equals("") );

    cobbletext_engine_prepare_advances(engine);

    REQUIRE_FALSE( cobbletext_get_error_code(library) );
    CHECK_THAT( cobbletext_get_error_message(library), Catch::Equals("") );

    uint32_t tile_count = cobbletext_engine_get_tile_count(engine);
    uint32_t advance_count = cobbletext_engine_get_advance_count(engine);

    const CobbletextTileInfo ** tiles =
        cobbletext_engine_get_tiles(engine);
    const CobbletextAdvanceInfo ** advances =
        cobbletext_engine_get_advances(engine);
    const CobbletextOutputInfo * outputInfo =
        cobbletext_engine_get_output_info(engine);

    CHECK( tile_count );
    CHECK( advance_count );
    CHECK( tiles );
    CHECK( advances );
    CHECK( outputInfo );

    CHECK( outputInfo->text_width );
    CHECK( outputInfo->text_height );

    for (size_t index = 0; index < tile_count; index++) {
        const CobbletextTileInfo * tile = tiles[index];

        CHECK( tile->glyph_id );
    }

    for (size_t index = 0; index < advance_count; index++) {
        const CobbletextAdvanceInfo * advance = advances[index];

        if (advance->type == COBBLETEXT_ADVANCE_TYPE_GLYPH) {
            CHECK( advance->glyph_id );

            const CobbletextGlyphInfo * glyph =
                cobbletext_library_get_glyph_info(library, advance->glyph_id);

            REQUIRE_FALSE( cobbletext_get_error_code(library) );
            CHECK_THAT( cobbletext_get_error_message(library), Catch::Equals("") );
            CHECK( glyph->image );
            CHECK( glyph->image_width );
            CHECK( glyph->image_height );
            CHECK( glyph->image_offset_x );
            CHECK( glyph->image_offset_y );
        }

    }

    cobbletext_engine_delete(engine);
    cobbletext_library_delete(library);
}
