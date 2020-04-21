
file(GLOB_RECURSE COBBLETEXT_TEST_SOURCES *.cpp)

find_path(CATCH2_INCLUDE_PATH "catch2")

add_executable(test_main ${COBBLETEXT_TEST_SOURCES})

target_include_directories(test_main PRIVATE
    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/src/"
    "${CMAKE_CURRENT_BINARY_DIR}"
    "${CATCH2_INCLUDE_PATH}"
)

target_link_libraries(test_main PRIVATE cobbletext
    ${ICU_LIBRARIES})

if(NOT WIN32)
    target_link_libraries(test_main PRIVATE dl)
endif()


add_test(NAME main COMMAND test_main)
set_property(TEST main APPEND PROPERTY ENVIRONMENT
    "TEST_FONT_PATH=${CMAKE_SOURCE_DIR}/lib/adobe-notdef/AND-Regular.otf")
