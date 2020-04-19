
add_executable(ppm_example example.c example_app.c)

find_path(UTHASH_INCLUDE_PATH "uthash.h")
target_include_directories(ppm_example PRIVATE
    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/src/"
    "${CMAKE_CURRENT_BINARY_DIR}"
    ${UTHASH_INCLUDE_PATH}
)

add_compile_definitions(HASH_DEBUG=1)

target_link_libraries(ppm_example PRIVATE cobbletext)

if(NOT WIN32)
    target_link_libraries(ppm_example PRIVATE dl)
endif()
