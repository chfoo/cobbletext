
file(GLOB_RECURSE COBBLETEXT_TEST_SOURCES *.cpp)

add_executable(test ${COBBLETEXT_TEST_SOURCES})

target_include_directories(test PRIVATE
    ${COBBLETEXT_DEPENDENCY_INCLUDES}
    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/src/"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CMAKE_SOURCE_DIR}/lib/Catch2/single_include/"
)

find_library(COBBLETEXT_LIB_PATH cobbletext HINTS "${CMAKE_BINARY_DIR}/cobbletext")
target_link_libraries(test PRIVATE
    "${COBBLETEXT_LIB_PATH}"
    ${COBBLETEXT_DEPENDENCY_LIBS}
)

if(NOT WIN32)
    target_link_libraries(test PRIVATE dl)
endif()
