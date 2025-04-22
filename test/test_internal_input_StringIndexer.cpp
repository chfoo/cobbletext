#include <memory>

#include <catch2/catch_all.hpp>
#include <unicode/schriter.h>

#include "internal/input/StringIndexer.hpp"

TEST_CASE("StringIndexer") {
    auto text = std::make_shared<icu::UnicodeString>(
        icu::UnicodeString::fromUTF8("h🐲🐲h"));

    cobbletext::internal::StringIndexer indexer(text);

    SECTION("individual -1 range") {
        CHECK( indexer.codePointToCodeUnit(-1) == 0 );
    }

    SECTION("individual") {
        CHECK( indexer.codePointToCodeUnit(0) == 0 );
    }

    SECTION("individual") {
        CHECK( indexer.codePointToCodeUnit(1) == 1 );
    }

    SECTION("individual") {
        CHECK( indexer.codePointToCodeUnit(2) == 3 );
    }

    SECTION("individual") {
        CHECK( indexer.codePointToCodeUnit(3) == 5 );
    }

    SECTION("individual end") {
        CHECK( indexer.codePointToCodeUnit(4) == 6 );
    }

    SECTION("individual +1 range") {
        CHECK( indexer.codePointToCodeUnit(5) == 6 );
    }

    SECTION("consecutive") {
        CHECK( indexer.codePointToCodeUnit(0) == 0 );
        CHECK( indexer.codePointToCodeUnit(1) == 1 );
        CHECK( indexer.codePointToCodeUnit(2) == 3 );
        CHECK( indexer.codePointToCodeUnit(3) == 5 );
    }

    SECTION("random") {
        CHECK( indexer.codePointToCodeUnit(2) == 3 );
        CHECK( indexer.codePointToCodeUnit(-1) == 0 );
        CHECK( indexer.codePointToCodeUnit(4) == 6 );
        CHECK( indexer.codePointToCodeUnit(3) == 5 );
        CHECK( indexer.codePointToCodeUnit(1) == 1 );
        CHECK( indexer.codePointToCodeUnit(6) == 6 );
        CHECK( indexer.codePointToCodeUnit(0) == 0 );
        CHECK( indexer.codePointToCodeUnit(5) == 6 );
    }

    SECTION("out of range") {
        CHECK( indexer.codePointToCodeUnit(0) == 0 );
        CHECK( indexer.codePointToCodeUnit(-1) == 0 );
        CHECK( indexer.codePointToCodeUnit(-2) == 0 );
        CHECK( indexer.codePointToCodeUnit(-1) == 0 );

        CHECK( indexer.codePointToCodeUnit(3) == 5 );
        CHECK( indexer.codePointToCodeUnit(4) == 6 );
        CHECK( indexer.codePointToCodeUnit(5) == 6 );
        CHECK( indexer.codePointToCodeUnit(4) == 6 );
        CHECK( indexer.codePointToCodeUnit(3) == 5 );
    }
}
