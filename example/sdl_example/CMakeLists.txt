
add_executable(sdl_example sdl_example.cpp)

find_path(SDL2_INCLUDE_PATH SDL.h PATH_SUFFIXES SDL2)
find_library(SDL2_LIBRARY NAMES SDL2)

message("SDL2 headers located at ${SDL2_INCLUDE_PATH}")
message("SDL2 library is ${SDL2_LIBRARY}")

target_include_directories(sdl_example PRIVATE
    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/src/"
    "${CMAKE_CURRENT_BINARY_DIR}"
    ${SDL2_INCLUDE_PATH}
)

if(WIN32)
    find_path(GETOPT_INCLUDE_PATH "getopt.h")
    find_library(GETOPT_LIBRARY getopt)
    target_include_directories(sdl_example PRIVATE "${GETOPT_INCLUDE_PATH}")
    target_link_libraries(sdl_example PRIVATE "${GETOPT_LIBRARY}")
endif()

target_link_libraries(sdl_example PRIVATE cobbletext ${SDL2_LIBRARY})

if(NOT WIN32)
    target_link_libraries(sdl_example PRIVATE dl)
endif()
