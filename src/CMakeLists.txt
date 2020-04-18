# message("hello ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}")

set(COBBLETEXT_DEBUG true CACHE BOOL "Enable dev features such as debug printing")

if (${COBBLETEXT_DEBUG})
    add_compile_definitions(COBBLETEXT_DEBUG=1)
endif()

file(GLOB_RECURSE COBBLETEXT_LIBRARY_SOURCES *.cpp)

add_library(cobbletext SHARED ${COBBLETEXT_LIBRARY_SOURCES} "Resource.cpp" )
add_library(cobbletext_static STATIC ${COBBLETEXT_LIBRARY_SOURCES} "Resource.cpp" )

configure_file(Version.h.in "${CMAKE_CURRENT_BINARY_DIR}/Version.h")

add_custom_command(OUTPUT Resource.cpp
    COMMAND python3 "${CMAKE_SOURCE_DIR}/script/embed_resource.py"
        ADOBE_NOTDEF
        "${CMAKE_SOURCE_DIR}/lib/adobe-notdef/AND-Regular.otf"
        "${CMAKE_CURRENT_BINARY_DIR}/Resource.cpp"
)

set(COBBLETEXT_LIBRARY_ALL_INCLUDES
    ${COBBLETEXT_DEPENDENCY_INCLUDES}
    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/include/cobbletext/"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

target_include_directories(cobbletext PRIVATE ${COBBLETEXT_LIBRARY_ALL_INCLUDES})
target_include_directories(cobbletext_static PRIVATE ${COBBLETEXT_LIBRARY_ALL_INCLUDES})

set_target_properties(cobbletext PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(cobbletext_static PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(cobbletext PROPERTIES SOVERSION 1)

set_target_properties(cobbletext PROPERTIES PUBLIC_HEADER "cobbletext.h cobbletext.hpp")
