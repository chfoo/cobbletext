cmake_minimum_required(VERSION 3.13)

project(cobbletext VERSION 0.0.0 DESCRIPTION "Complex text layout and rendering engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED)
find_package(Freetype REQUIRED)
find_package(ICU REQUIRED COMPONENTS data i18n tu uc)
find_path(HARFBUZZ_INCLUDE_PATH "harfbuzz")
find_path(MS_GSL_INCLUDE_PATH "gsl")
find_library(HARFBUZZ_LIB_PATH "harfbuzz")

set(COBBLETEXT_DEPENDENCY_INCLUDES
    "${PROJECT_SOURCE_DIR}/lib/GSL/include"
    "${PROJECT_SOURCE_DIR}/lib/color/src/"
    ${Boost_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    "${HARFBUZZ_INCLUDE_PATH}"
    "${ICU_INCLUDE_DIR}"
    "${MS_GSL_INCLUDE_PATH}"
)
set(COBBLETEXT_DEPENDENCY_LIBS
    ${FREETYPE_LIBRARIES}
    ${ICU_LIBRARIES}
    ${HARFBUZZ_LIB_PATH}
)

add_subdirectory(src cobbletext)
add_subdirectory(example)
add_subdirectory(test)
