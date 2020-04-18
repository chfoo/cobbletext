# Cobbletext

Cobbletext is a complex text layout and rendering engine as a C/C++ library. It bundles formatting, bidirectional analysis, shaping, line breaking, and glyph rasterization into one simple interface. It is not tied to any specific operating system framework and is designed for general purpose use on graphics contexts such as OpenGL, SDL, or HTML5 Canvas.

It uses FreeType, HarfBuzz, and Unicode ICU for the actual dirty work.

Work in progress; not usable yet.

## Quick start

### Dependencies

Libraries:

* FreeType 2
* HarfBuzz 2
* International Components for Unicode (ICU)

Header-only libraries:

* Boost
* Microsoft GSL

Resources:

* adobe-notdef (included as a Git Submodule)

Optional for examples and tests:

* uthash
* Catch2

### vcpkg

### Homebrew (MacOS)

### Linux package manager

On Debian based system, install

* build-essential
* libfreetype6-dev
* libharfbuzz-dev
* libicu-dev
* libboost1.XX-dev (where XX is the latest version)
* libmsgsl-dev

Optional:

* uthash-dev
* There's no catch2 package yet. Put it somewhere and supply the path to CMake as described later.

## Building

Requirements:

* CMake 3.13+ for generating the build files.
* Python 3 for `script/embed_resource.py`. CMake will call Python automatically.
* GNU Make or Visual Studio for the actual build.

To generate build files using CMake:

1. `mkdir build`
2. `cd build`
3. `cmake ..`

For vcpkg, please include a flag similar to `-D CMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake`.

If CMake can't find something, you can manually edit CMakeCache.txt. Use `cmake .. -L` to list out configurable variables.

### Makefile

* Running `make cobbletext` will build a shared/dynamic library.
* Running `make cobbletext_static` will build a static library.

### Visual Studio

Ensure the C++ workload is installed. See Tools -> Get tools and features.

* Project `cobbletext` is the dynamic library.
* Project `cobbletext_static` is the static library.
