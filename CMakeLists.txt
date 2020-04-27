cmake_minimum_required(VERSION 3.13)

include(CheckIncludeFile)

project(cobbletext VERSION 0.0.0 DESCRIPTION "Complex text layout and rendering engine")

set(COBBLETEXT_BUILD_EXAMPLES true CACHE BOOL "Whether to include example programs into the build config")
set(COBBLETEXT_BUILD_TESTS true CACHE BOOL "Whether to include tests into the build config")
set(COBBLETEXT_BUILD_DOCS true CACHE BOOL "Whether to include documentation to the build config")
set(COBBLETEXT_STATIC false CACHE BOOL "Enable to build the library as a static library. Default is dynamic library.")
set(COBBLETEXT_EMSCRIPTEN false CACHE BOOL "Enables config to build to Emscripten with emcmake and emmake.")

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

    include(FetchContent)
    FetchContent_Declare(
      icu-emscripten
      GIT_REPOSITORY https://github.com/TartanLlama/icu-emscripten.git
      GIT_TAG        master
    )
    # FetchContent_MakeAvailable(icu-emscripten)

    # Backwards compatible MakeAvailable
    FetchContent_GetProperties(icu-emscripten)
    if(NOT icu-emscripten_POPULATED)
        FetchContent_Populate(icu-emscripten)
        # add_subdirectory(${icu-emscripten_SOURCE_DIR} ${icu-emscripten_BINARY_DIR})
    endif()

    # Make sure CMake doesn't try to pull in system installations of ICU
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
    set(ICU_ROOT ${icu-emscripten_SOURCE_DIR})
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
find_package(ICU REQUIRED COMPONENTS data uc)
find_path(MS_GSL_INCLUDE_PATH "gsl")
find_path(STB_INCLUDE_PATH "stb.h" PATH_SUFFIXES "stb")

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
