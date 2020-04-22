cmake_minimum_required(VERSION 3.13)

project(cobbletext VERSION 0.0.0 DESCRIPTION "Complex text layout and rendering engine")

set(COBBLETEXT_BUILD_EXAMPLES true CACHE BOOL "Whether to include example programs into the build config")
set(COBBLETEXT_BUILD_TESTS true CACHE BOOL "Whether to include tests into the build config")
set(COBBLETEXT_BUILD_DOCS true CACHE BOOL "Whether to include documentation to the build config")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED)
find_package(Freetype REQUIRED)
find_package(ICU REQUIRED COMPONENTS data i18n tu uc)
find_path(HARFBUZZ_INCLUDE_PATH "harfbuzz")
find_path(MS_GSL_INCLUDE_PATH "gsl")
find_library(HARFBUZZ_LIB_PATH "harfbuzz")

set(COBBLETEXT_DEPENDENCY_INCLUDES
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

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

enable_testing()

add_subdirectory(src cobbletext)

if(COBBLETEXT_BUILD_EXAMPLES)
    add_subdirectory(example/ppm_example)
endif()

if(COBBLETEXT_BUILD_TESTS)
    add_subdirectory(test)
endif()

if(COBBLETEXT_BUILD_DOCS)
    add_subdirectory("doc")
endif()
