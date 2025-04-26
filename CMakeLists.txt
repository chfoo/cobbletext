cmake_minimum_required(VERSION 3.13)

include(CheckIncludeFile)

project(cobbletext VERSION 0.2.0 DESCRIPTION "Complex text layout and rendering engine")

set(COBBLETEXT_BUILD_EXAMPLES true CACHE BOOL "Whether to include example programs into the build config")
set(COBBLETEXT_BUILD_TESTS true CACHE BOOL "Whether to include tests into the build config")
set(COBBLETEXT_BUILD_DOCS true CACHE BOOL "Whether to include documentation to the build config")
set(COBBLETEXT_STATIC false CACHE BOOL "Enable to build the library as a static library. Default is dynamic library.")
set(COBBLETEXT_CPP_API false CACHE BOOL "Enable the C++ API for dynamic library build on Windows.")
set(COBBLETEXT_CUSTOM_ICU false CACHE BOOL "Whether to use link with ICU libraries compiled with --with-library-suffix=cobbletext")
set(COBBLETEXT_EMSCRIPTEN false CACHE BOOL "Enables config to build to Emscripten with emcmake and emmake.")

if(COBBLETEXT_EMSCRIPTEN)
    set(COBBLETEXT_EMSCRIPTEN_ICU_STRATEGY "manual" CACHE STRING "Sets how to get ICU"
        STRINGS "download_from_tartanllama" "manual")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(C_STANDARD 11)

CHECK_INCLUDE_FILE("uchar.h" HAS_UCHAR_H)

if(NOT HAS_UCHAR_H)
    add_compile_definitions(COBBLETEXT_NO_UCHAR_H=1)
endif()

if(COBBLETEXT_STATIC)
    add_compile_definitions(COBBLETEXT_STATIC=1)
endif()

if(COBBLETEXT_CPP_API)
    add_compile_definitions(COBBLETEXT_ENABLE_CPP_API=1)
endif()

if(COBBLETEXT_EMSCRIPTEN)
    add_compile_options("SHELL:-s USE_BOOST_HEADERS=1")
    # add_compile_options("SHELL:-s USE_ICU=1")
    add_compile_options("SHELL:-s USE_SDL=2")
    add_compile_options("SHELL:-s USE_FREETYPE=1")
    add_compile_options("SHELL:-s USE_HARFBUZZ=1")

    add_link_options("SHELL:-s USE_BOOST_HEADERS=1")
    # add_link_options("SHELL:-s USE_ICU=1")
    add_link_options("SHELL:-s USE_SDL=2")
    add_link_options("SHELL:-s USE_FREETYPE=1")
    add_link_options("SHELL:-s USE_HARFBUZZ=1")

    add_link_options("SHELL:-s TOTAL_MEMORY=41943040")  # actually INTITAL_MEMORY
    add_link_options("SHELL:-s ALLOW_MEMORY_GROWTH=1")

    if(COBBLETEXT_EMSCRIPTEN_ICU_STRATEGY STREQUAL "download_from_tartanllama")
        include(script/get_precompiled_icu_tartanllama.cmake)
    endif()
else()
    find_package(Boost REQUIRED)
    find_package(Freetype REQUIRED)
    find_path(HARFBUZZ_INCLUDE_PATH "harfbuzz")
    find_library(HARFBUZZ_LIB_PATH "harfbuzz")
endif()

# dt = data
# uc = common
# in = i18n: locale i18n functions
# lx = layout extensions, optional engine
# io = I/O, optional support for I/O with Unicode
# tu = tool utility library, internal API for optional tools
if(COBBLETEXT_CUSTOM_ICU)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/misc/cmake_modules/")
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
    find_package(ICUCobbletext REQUIRED COMPONENTS uc data)  # data needs to be linked last
    add_compile_definitions(U_HAVE_LIB_SUFFIX=1)
    add_compile_definitions(U_LIB_SUFFIX_C_NAME=_cobbletext)
else()
    find_package(ICU REQUIRED COMPONENTS uc data)
endif()

find_path(MS_GSL_INCLUDE_PATH "gsl")
find_path(STB_INCLUDE_PATH "stb_image_resize2.h" PATH_SUFFIXES "stb")

set(COBBLETEXT_DEPENDENCY_INCLUDES
    ${Boost_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    "${HARFBUZZ_INCLUDE_PATH}"
    "${ICU_INCLUDE_DIR}"
    "${MS_GSL_INCLUDE_PATH}"
    "${STB_INCLUDE_PATH}"
)
set(COBBLETEXT_DEPENDENCY_LIBS
    ${FREETYPE_LIBRARIES}
    ${ICU_LIBRARIES}
    ${HARFBUZZ_LIB_PATH}
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

enable_testing()

add_subdirectory(src cobbletext)

if(COBBLETEXT_BUILD_EXAMPLES)
    add_subdirectory(example/ppm_example)
    add_subdirectory(example/sdl_example)
endif()

if(COBBLETEXT_BUILD_TESTS)
    add_subdirectory(test)
endif()

if(COBBLETEXT_BUILD_DOCS)
    add_subdirectory("doc")
endif()

if(COBBLETEXT_EMSCRIPTEN)
    add_subdirectory(src/em cobbletext_em)
endif()

install(TARGETS cobbletext DESTINATION lib)
install(DIRECTORY "${CMAKE_SOURCE_DIR}/include/cobbletext/"
    DESTINATION "include/cobbletext"
)

if(COBBLETEXT_EMSCRIPTEN)
    install(TARGETS cobbletext_js DESTINATION bin)
    install(FILES "${CMAKE_BINARY_DIR}/bin/cobbletext.wasm" DESTINATION bin)
endif()
