# Changelog

## 0.2.1 (2025-05-05)

* Fixed a memory leak [(#2)](https://github.com/chfoo/cobbletext/issues/2)
* Fixed compatibility with latest version of stb

## 0.2.0 (2020-05-08)

* Removed library `clear_glyphs()` and added engine `clear_tiles()`.
* Changed `add_inline_object()` to accept width and height instead of size.
* Fixed Engine OutputInfo containing uninitialized values if `lay_out()` was not called.
* Fixed dynamic library symbol exports on Windows.
* Added CMake variable `COBBLETEXT_CPP_API` to enable C++ API on a dynamic library build for Windows.

## 0.1.0 (2020-04-30)

* First release
