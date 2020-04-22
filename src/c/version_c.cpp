#include "cobbletext.h"

#include "Library.hpp"

int32_t cobbletext_get_version_major() {
    return cobbletext::Library::versionMajor();
}

int32_t cobbletext_get_version_minor() {
    return cobbletext::Library::versionMinor();
}

int32_t cobbletext_get_version_patch() {
    return cobbletext::Library::versionPatch();
}

const char * cobbletext_get_version() {
    return cobbletext::Library::version().c_str();
}
