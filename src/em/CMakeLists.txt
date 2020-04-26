file(GLOB COBBLETEXT_EM_SOURCES "*.cpp")

add_executable(cobbletext_js
    js/cobbletext_js.cpp
    ${COBBLETEXT_LIBRARY_SOURCES}
    ${COBBLETEXT_EM_SOURCES}
    "../Resource.cpp"
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

add_compile_definitions(COBBLETEXT_ENABLE_EM_API=1)

target_include_directories(cobbletext_js PRIVATE ${COBBLETEXT_LIBRARY_ALL_INCLUDES})

set(CMAKE_EXECUTABLE_SUFFIX ".js")
set_target_properties(cobbletext_js PROPERTIES OUTPUT_NAME "cobbletext")

target_link_options(cobbletext_js PRIVATE "SHELL:-s MODULARIZE=1")
target_link_options(cobbletext_js PRIVATE "SHELL:-s EXPORT_NAME=CobbletextModule")
target_link_options(cobbletext_js PRIVATE "--bind")

# If LINKABLE is disabled, EMSCRIPTEN_GENERATE_BITCODE_STATIC_LIBRARIES=true
# should be set to generate .bc files intead of .a files.

# TODO: figure out how to keep icudt65_dat from being deleted.
target_link_options(cobbletext_js PRIVATE "SHELL:-s LINKABLE=1")

target_link_libraries(cobbletext_js PRIVATE ${COBBLETEXT_DEPENDENCY_LIBS})
