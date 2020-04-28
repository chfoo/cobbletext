include(FetchContent)
FetchContent_Declare(
  icu-emscripten
  GIT_REPOSITORY https://github.com/TartanLlama/icu-emscripten.git
  GIT_TAG        master
)
# FetchContent_MakeAvailable(icu-emscripten)

# Backwards compatible MakeAvailable
FetchContent_GetProperties(icu-emscripten)
if(NOT icu-emscripten_POPULATED)
    FetchContent_Populate(icu-emscripten)
    # add_subdirectory(${icu-emscripten_SOURCE_DIR} ${icu-emscripten_BINARY_DIR})
endif()

# Make sure CMake doesn't try to pull in system installations of ICU
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
set(ICU_ROOT ${icu-emscripten_SOURCE_DIR})
