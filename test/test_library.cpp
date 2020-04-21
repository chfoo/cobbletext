#include <stdlib.h>
#include <string>

#include <catch2/catch.hpp>

#include "cobbletext/cobbletext.h"

TEST_CASE("library construction and error clearing") {
    CobbletextLibrary * library = cobbletext_library_new();

    REQUIRE( library );
    CHECK( cobbletext_get_error_code(library) == 0 );
    CHECK_THAT( cobbletext_get_error_message(library), Catch::Equals("") );

    const struct CobbletextFontInfo * font_info =
    cobbletext_library_get_font_info(library, 123123);

    CHECK( cobbletext_get_error_code(library) );
    CHECK_THAT( cobbletext_get_error_message(library), !Catch::Equals("") );

    cobbletext_clear_error(library);
    CHECK( cobbletext_get_error_code(library) == 0 );
    CHECK_THAT( cobbletext_get_error_message(library), Catch::Equals("") );

    REQUIRE_NOTHROW( cobbletext_library_delete(library) );
}

TEST_CASE("library heavy methods") {
    CobbletextLibrary * library = cobbletext_library_new();

    SECTION("fallback font") {
        CobbletextFontID fallback_font =
            cobbletext_library_get_fallback_font(library);

        REQUIRE( fallback_font );

        const struct CobbletextFontInfo * font_info =
            cobbletext_library_get_font_info(library, fallback_font);

        REQUIRE( font_info );
        CHECK( font_info->id == fallback_font );
        CHECK( font_info->family_name );
        CHECK( font_info->style_name );
        CHECK( font_info->units_per_em );
        CHECK( font_info->ascender );
        CHECK( font_info->descender );
        CHECK( font_info->height );
        CHECK( font_info->underline_position );
        CHECK( font_info->underline_thickness );
    }

    SECTION("check nonexistent font") {
        const struct CobbletextFontInfo * font_info =
            cobbletext_library_get_font_info(library, 123123);

        CHECK( cobbletext_get_error_code(library) );
        CHECK_THAT( cobbletext_get_error_message(library), !Catch::Equals("") );
    }

    SECTION("check nonexistent glyph") {
        const struct CobbletextGlyphInfo * glyph_info =
            cobbletext_library_get_glyph_info(library, 123123);

        CHECK( cobbletext_get_error_code(library) );
        CHECK_THAT( cobbletext_get_error_message(library), !Catch::Equals("") );
    }

    SECTION("load nonexistant file") {
        cobbletext_library_load_font(library, "/nonexistant/font.ttf");

        CHECK( cobbletext_get_error_code(library) );
        CHECK_THAT( cobbletext_get_error_message(library), !Catch::Equals("") );
    }

    SECTION("load nonsense bytes") {
        uint8_t * bytes = new uint8_t[100];
        cobbletext_library_load_font_bytes(library, bytes, 100);
        delete [] bytes;

        CHECK( cobbletext_get_error_code(library) );
        CHECK_THAT( cobbletext_get_error_message(library), !Catch::Equals("") );
    }

    SECTION("check load file") {
        std::string fontPath;

        if (getenv("TEST_FONT_PATH")) {
            fontPath.append(getenv("TEST_FONT_PATH"));
        } else {
            fprintf(stderr, "TEST_FONT_PATH not set");
            fontPath.append("lib/adobe-notdef/AND-Regular.otf");
        }

        cobbletext_library_load_font(library, fontPath.c_str());

        CHECK_FALSE( cobbletext_get_error_code(library) );
        CHECK_THAT( cobbletext_get_error_message(library), Catch::Equals("") );
    }

    cobbletext_library_delete(library);
}
