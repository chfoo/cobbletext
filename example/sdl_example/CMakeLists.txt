
add_executable(sdl_example sdl_example.cpp)

target_include_directories(sdl_example PRIVATE
    "${CMAKE_SOURCE_DIR}/include/"
    "${CMAKE_SOURCE_DIR}/src/"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

if(NOT COBBLETEXT_EMSCRIPTEN)
    find_path(SDL2_INCLUDE_PATH SDL.h PATH_SUFFIXES SDL2)
    find_library(SDL2_LIBRARY NAMES SDL2)

    message("SDL2 headers located at ${SDL2_INCLUDE_PATH}")
    message("SDL2 library is ${SDL2_LIBRARY}")

    target_include_directories(sdl_example PRIVATE ${SDL2_INCLUDE_PATH})

    if(WIN32)
        find_path(GETOPT_INCLUDE_PATH "getopt.h")
        find_library(GETOPT_LIBRARY getopt)
        target_include_directories(sdl_example PRIVATE "${GETOPT_INCLUDE_PATH}")
        target_link_libraries(sdl_example PRIVATE "${GETOPT_LIBRARY}")
    else()
        target_link_libraries(sdl_example PRIVATE dl)
    endif()

elseif(COBBLETEXT_EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_link_options(sdl_example PRIVATE "SHELL:-s EXIT_RUNTIME=1")
    target_link_options(sdl_example PRIVATE --shell-file "${CMAKE_CURRENT_SOURCE_DIR}/emscripten_shell.html")
endif()

target_link_libraries(sdl_example PRIVATE cobbletext ${COBBLETEXT_DEPENDENCY_LIBS} ${SDL2_LIBRARY})
