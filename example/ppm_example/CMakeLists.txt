
add_executable(ppm_example ppm_example.c)

find_path(UTHASH_INCLUDE_PATH "uthash.h")
target_include_directories(ppm_example PRIVATE
    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/src/"
    "${CMAKE_CURRENT_BINARY_DIR}"
    ${UTHASH_INCLUDE_PATH}
)

if(WIN32)
    find_path(GETOPT_INCLUDE_PATH "getopt.h")
    find_library(GETOPT_LIBRARY getopt)
    target_include_directories(ppm_example PRIVATE "${GETOPT_INCLUDE_PATH}")
    target_link_libraries(ppm_example PRIVATE "${GETOPT_LIBRARY}")
endif()

add_compile_definitions(HASH_DEBUG=1)

target_link_libraries(ppm_example PRIVATE cobbletext)

if(NOT WIN32)
    target_link_libraries(ppm_example PRIVATE dl)
endif()

add_test(NAME ppm_example COMMAND ppm_example WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
