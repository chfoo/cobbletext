#include  <catch2/catch.hpp>

#include "cobbletext/cobbletext.h"

TEST_CASE("version getters") {

    CHECK_NOTHROW( cobbletext_get_version_major() );
    CHECK_NOTHROW( cobbletext_get_version_minor() );
    CHECK_NOTHROW( cobbletext_get_version_patch() );
    CHECK( cobbletext_get_version() );

}
