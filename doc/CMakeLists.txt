# File adapted from https://devblogs.microsoft.com/cppblog/clear-functional-c-documentation-with-sphinx-breathe-doxygen-cmake/

find_package(Doxygen REQUIRED)

file(GLOB_RECURSE COBBLETEXT_PUBLIC_HEADERS
    "${CMAKE_SOURCE_DIR}/include/cobbletext/*.h"
    "${CMAKE_SOURCE_DIR}/include/cobbletext/*.hpp")

set(DOXYGEN_INPUT_DIR "${CMAKE_SOURCE_DIR}/include/")
set(DOXYGEN_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/doxygen")
set(DOXYGEN_INDEX_FILE "${DOXYGEN_OUTPUT_DIR}/html/index.html")
set(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in.txt")
set(DOXYFILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

configure_file("${DOXYFILE_IN}" "${DOXYFILE_OUT}" @ONLY)

file(MAKE_DIRECTORY "${DOXYGEN_OUTPUT_DIR}")

add_custom_command(
    OUTPUT "${DOXYGEN_INDEX_FILE}"
    DEPENDS ${COBBLETEXT_PUBLIC_HEADERS}
    COMMAND "${DOXYGEN_EXECUTABLE}" ${DOXYFILE_OUT}
    MAIN_DEPENDENCY "${DOXYFILE_OUT}" "${DOXYFILE_IN}"
    VERBATIM
)

add_custom_target("doc" ALL DEPENDS "${DOXYGEN_INDEX_FILE}" VERBATIM)
