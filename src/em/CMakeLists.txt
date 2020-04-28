file(GLOB COBBLETEXT_LIBRARY_SOURCES "../*.cpp" "../c/*.cpp" "../internal/*.cpp" "../internal/*/*.cpp")
file(GLOB COBBLETEXT_EM_SOURCES "*.cpp")

add_executable(cobbletext_js
    js/cobbletext_js.cpp
    ${COBBLETEXT_LIBRARY_SOURCES}
    ${COBBLETEXT_EM_SOURCES}
    "${CMAKE_CURRENT_BINARY_DIR}/Resource.cpp"
)

configure_file(../Version.h.in "${CMAKE_CURRENT_BINARY_DIR}/Version.h")

find_package(Python3 COMPONENTS Interpreter)
add_custom_command(OUTPUT Resource.cpp
    COMMAND "${Python3_EXECUTABLE}"
        "${CMAKE_SOURCE_DIR}/script/embed_resource.py"
        "ADOBE_NOTDEF"
        "${CMAKE_SOURCE_DIR}/lib/adobe-notdef/AND-Regular.otf"
        "${CMAKE_CURRENT_BINARY_DIR}/Resource.cpp"
    VERBATIM
)

set(COBBLETEXT_LIBRARY_ALL_INCLUDES
    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/include/cobbletext/"
    "${CMAKE_SOURCE_DIR}/src/"
    "${CMAKE_CURRENT_BINARY_DIR}"
    ${COBBLETEXT_DEPENDENCY_INCLUDES}
)

target_compile_definitions(cobbletext_js PRIVATE COBBLETEXT_ENABLE_EM_API=1)
target_include_directories(cobbletext_js PRIVATE ${COBBLETEXT_LIBRARY_ALL_INCLUDES})

set(CMAKE_EXECUTABLE_SUFFIX ".js")
set_target_properties(cobbletext_js PROPERTIES OUTPUT_NAME "cobbletext")

target_link_options(cobbletext_js PRIVATE "SHELL:-s MODULARIZE=1")
target_link_options(cobbletext_js PRIVATE "SHELL:-s EXPORT_NAME=CobbletextModule")
target_link_options(cobbletext_js PRIVATE "--bind")

# If LINKABLE is disabled, EMSCRIPTEN_GENERATE_BITCODE_STATIC_LIBRARIES=true
# should be set to generate .bc files intead of .a files
# https://github.com/emscripten-core/emscripten/issues/5465

# target_link_options(cobbletext_js PRIVATE "SHELL:-s LINKABLE=1")

target_link_libraries(cobbletext_js PRIVATE ${COBBLETEXT_DEPENDENCY_LIBS})
