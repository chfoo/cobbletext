#include "cobbletext.h"

#include "Library.hpp"

int32_t cobbletext_get_version_major() {
    return cobbletext::Library::VERSION_MAJOR;
}

int32_t cobbletext_get_version_minor() {
    return cobbletext::Library::VERSION_MINOR;
}

int32_t cobbletext_get_version_patch() {
    return cobbletext::Library::VERSION_PATCH;
}

const char * cobbletext_get_version() {
    return cobbletext::Library::VERSION.c_str();
}
