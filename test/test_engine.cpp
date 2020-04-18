#include <catch2/catch.hpp>

#include "cobbletext.h"

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

    cobbletext_engine_set_line_length(engine, 200);
    cobbletext_engine_set_locale(engine, "zz");
    cobbletext_engine_set_custom_property(engine, 123);
    cobbletext_engine_set_font(engine, 0);
    cobbletext_engine_set_font_size(engine, 12);
    cobbletext_engine_set_language(engine, "xx");
    cobbletext_engine_set_script(engine, "xxxx");
    cobbletext_engine_set_script_direction(engine,
        COBBLETEXT_SCRIPT_DIRECTION_LTR);

    CHECK( cobbletext_engine_get_line_length(engine) == 200 );

    CHECK_THAT( cobbletext_engine_get_locale(engine), Catch::Equals("zz") );
    CHECK( cobbletext_engine_get_custom_property(engine) == 123);
    CHECK( cobbletext_engine_get_font(engine) == 0);
    CHECK( cobbletext_engine_get_font_size(engine) == 12);
    CHECK_THAT( cobbletext_engine_get_language(engine), Catch::Equals("xx") );
    CHECK_THAT( cobbletext_engine_get_script(engine), Catch::Equals("xxxx") );
    CHECK( cobbletext_engine_get_script_direction(engine) ==
        COBBLETEXT_SCRIPT_DIRECTION_LTR);

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
        unsigned char text[] = {0xF0, 0x9F, 0x98, 0x80};
        cobbletext_engine_add_text_utf8(engine, reinterpret_cast<const char *>(text), -1);
    }

    SECTION("add text 16") {
        char16_t text[] = {0xD83D, 0xDE00};
        cobbletext_engine_add_text_utf16(engine, text, -1);
    }

    SECTION("add text 32") {
        char32_t text[] = {0x1F600};
        cobbletext_engine_add_text_utf32(engine, text, -1);
    }

    cobbletext_engine_lay_out(engine);

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

    cobbletext_engine_set_custom_property(engine, 123);
    cobbletext_engine_add_text_utf8(engine, "hello ", -1);
    cobbletext_engine_set_custom_property(engine, 456);
    cobbletext_engine_add_text_utf8(engine, "world!", -1);
    cobbletext_engine_lay_out(engine);

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

    uint32_t tile_count = cobbletext_engine_get_tile_count(engine);
    uint32_t advance_count = cobbletext_engine_get_advance_count(engine);

    const CobbletextTileInfo ** tiles =
        cobbletext_engine_get_tiles(engine);
    const CobbletextAdvanceInfo ** advances =
        cobbletext_engine_get_advances(engine);

    CHECK( tile_count );
    CHECK( advance_count );
    CHECK( tiles );
    CHECK( advances );

    CHECK( cobbletext_engine_get_image_width(engine) );
    CHECK( cobbletext_engine_get_image_height(engine) );

    cobbletext_engine_delete(engine);
    cobbletext_library_delete(library);
}
