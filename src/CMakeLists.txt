set(COBBLETEXT_DEBUG false CACHE BOOL "Enable dev features such as debug printing")
set(COBBLETEXT_ADDRESS_SANITIZER false CACHE BOOL "Add address sanitizer flags (gcc/clang)")
set(COBBLETEXT_USE_STACK_TRACE false CACHE BOOL "Add stack traces to the library exceptions (requires boost-stacktrace, boost-exception)")

file(GLOB COBBLETEXT_LIBRARY_SOURCES "*.cpp" "c/*.cpp" "internal/*.cpp" "internal/*/*.cpp")

if(COBBLETEXT_STATIC)
    add_library(cobbletext STATIC ${COBBLETEXT_LIBRARY_SOURCES} "Resource.cpp" )
else()
    add_library(cobbletext SHARED ${COBBLETEXT_LIBRARY_SOURCES} "Resource.cpp" )
endif()

configure_file(Version.h.in "${CMAKE_CURRENT_BINARY_DIR}/Version.h")

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
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
    ${COBBLETEXT_DEPENDENCY_INCLUDES}
)

target_include_directories(cobbletext PRIVATE ${COBBLETEXT_LIBRARY_ALL_INCLUDES})
set_target_properties(cobbletext PROPERTIES VERSION ${cobbletext_VERSION})
set_target_properties(cobbletext PROPERTIES SOVERSION ${cobbletext_VERSION_MAJOR})

if(MSVC)
    target_compile_options(cobbletext PRIVATE /W4)
else()
    target_compile_options(cobbletext PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(COBBLETEXT_USE_STACK_TRACE)
    add_compile_definitions(COBBLETEXT_USE_STACK_TRACE=1)
endif()

if(COBBLETEXT_DEBUG)
    add_compile_definitions(COBBLETEXT_DEBUG=1)
endif()

if(COBBLETEXT_ADDRESS_SANITIZER)
    set(SANITIZE_FLAGS -fno-omit-frame-pointer -fsanitize=address)
    target_compile_options(cobbletext PRIVATE ${SANITIZE_FLAGS} )
    target_link_options(cobbletext PRIVATE ${SANITIZE_FLAGS} )
endif()

target_link_libraries(cobbletext PRIVATE ${COBBLETEXT_DEPENDENCY_LIBS})

if(COBBLETEXT_EMSCRIPTEN)
    add_subdirectory(em)
endif()
