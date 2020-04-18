# Cobbletext

Cobbletext is a complex text layout and rendering engine as a C/C++ library. It bundles formatting, bidirectional analysis, shaping, line breaking, and glyph rasterization into one simple interface. It is not tied to any specific operating system framework and is designed for general purpose use on graphics contexts such as OpenGL, SDL, or HTML5 Canvas.

It uses FreeType, HarfBuzz, and Unicode ICU for the actual dirty work.

Work in progress; not usable yet.

## Quick start

### Dependencies

* FreeType 2
* International Components for Unicode (ICU)
* HarfBuzz 2
* Boost
* Microsoft GSL

Optional for examples and tests:

* uthash
* Catch2

### vcpkg

### Homebrew (MacOS)

### Linux package manager

## Building

Requirements:

* CMake 3.13+

### Makefile

1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make cobbletext cobbletext_shared`
