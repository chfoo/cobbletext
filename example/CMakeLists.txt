
add_executable(example example.c example_app.c)

find_path(UTHASH_INCLUDE_PATH "uthash.h")
target_include_directories(example PRIVATE
    ${COBBLETEXT_DEPENDENCY_INCLUDES}
    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/src/"
    "${CMAKE_CURRENT_BINARY_DIR}"
    # ${Boost_INCLUDE_DIRS}
    ${UTHASH_INCLUDE_PATH}
)

find_library(COBBLETEXT_LIB_PATH cobbletext HINTS "${CMAKE_BINARY_DIR}/cobbletext")
target_link_libraries(example PRIVATE
    "${COBBLETEXT_LIB_PATH}"
    ${COBBLETEXT_DEPENDENCY_LIBS}
)

if(NOT WIN32)
    target_link_libraries(example PRIVATE dl)
endif()
