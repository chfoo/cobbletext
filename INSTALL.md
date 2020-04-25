# Install

Instructions on how to build Cobbletext and dependencies.

## Dependencies

Libraries:

* [FreeType](https://www.freetype.org/) 2
* [HarfBuzz](https://www.freedesktop.org/wiki/Software/HarfBuzz/) 2
* [International Components for Unicode (ICU)](http://site.icu-project.org/)

Header-only libraries:

* [Boost](https://www.boost.org/)
* [Microsoft GSL](https://github.com/microsoft/GSL)

Resources:

* [adobe-notdef](https://github.com/adobe-fonts/adobe-notdef) (included as a Git Submodule)

Optional for examples and tests:

* [uthash](https://troydhanson.github.io/uthash/)
* [Catch2](https://github.com/catchorg/Catch2)

### vcpkg

Install [vcpkg](https://github.com/microsoft/vcpkg), then use the command to install all the needed dependencies:

    ./vcpkg install freetype[png] harfbuzz[ucdn] icu ms-gsl boost-container-hash

To specify architecture, specify the triplet using `--triplet`, for example, `x86-windows`.

Optional for tests, install catch2 using vcpkg. uthash needs to be manually downloaded to somewhere and specified to CMake as described later.

### Homebrew (MacOS)

Use:

    brew install python freetype harfbuzz icu4c cpp-gsl boost

uthash needs to be manually downloaded to somewhere and specified to CMake as described later.

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

* C++17 compiler
* [CMake](https://cmake.org/) 3.13+ for generating the build files.
* [Python](https://www.python.org/) 3 for `script/embed_resource.py`. CMake will call Python automatically.
* GNU Make or Visual Studio for running the build files.

Note that CMake and Python can be installed using Visual Studio, your package manager, or Homebrew.

### Generate build files

To generate build files using CMake:

1. `mkdir build`
2. `cd build`
3. `cmake ..`

For vcpkg, please include a flag similar to `-D CMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake`. To let the toolchain file know which triplet you want to use, use something like `-D VCPKG_TARGET_TRIPLET=x64-windows`.

If CMake can't find something, you can manually edit CMakeCache.txt (or use a GUI if available). Use `cmake .. -L` to list out configurable variables. Cobbletext also provides its own variables for debugging or disabling things like documentation configs.

With Homebrew, CMake may not find ICU. Set your shell environment variable with `export ICU_ROOT=/usr/local/opt/icu4c` or `export CMAKE_PREFIX_PATH=/usr/local/opt/icu4c`.

In the same build directory, build the library either the shared or static library:

    cmake --build . --target cobbletext
    cmake --build . --target cobbletext -D COBBLETEXT_STATIC=true

Binaries are stored in the `build/bin` directory.

Alternatively, you can run the generated build files yourself as described below.

### Makefile

* Running `make cobbletext` will build the library.

### Visual Studio

Ensure the C++ workload is installed. See Tools -> Get tools and features.

* Project `cobbletext` is the library.

## Emscripten

Assumes Linux-like environment.

Install the emsdk and activate version 1.38.48.

### Building the library

Make a build directory:

    mkdir build_em
    cd build_em

Generate the build files:

    emcmake cmake .. -D COBBLETEXT_EMSCRIPTEN=true -D COBBLETEXT_BUILD_DOCS=false -D COBBLETEXT_STATIC=true

Fix CMakeCache.txt with paths to library headers if needed. *Do not* include system headers! For example, don't add `/usr/include`. Copy the headers somewhere isolated if needed.

Build the makefile with GNU Make:

    emmake make VERBOSE=1 cobbletext

By default ICU included with Emscripten is not a working port. Instead, use the libraries from [this GitHub repo](https://github.com/tartanllama/icu-emscripten).

This will output `cobbletext/libcobbletext.a` which is LLVM bitcode file is for linking into your Emscripten application. Use this bitcode along with the ICU bitcode to build your application.

Example manual linking:

    em++ -O3 -s USE_SDL=2 -s USE_FREETYPE=1 -s USE_HARFBUZZ=1 \
        -s TOTAL_MEMORY=41943040 -s ALLOW_MEMORY_GROWTH=1 \
        [your bitcode files here] \
        -o [your output js or html file]

### Build cobblescript.js

Run:

    emmake make VERBOSE cobbletext_js

This will generate `bin\cobbletext.wasm` and `bin\cobbletext.js` for including into a JavaScript application.

Use this if you aren't building an Emscripten application, but want to use Cobbletext directly from JavaScript. The module will be built with the modularize flag to module named `Cobbletext`.

```js
Cobbletext().then(function(Module) {
    // do things with Module
});
```

### Building example

To generate example executable:

    emmake make VERBOSE=1 sdl_example

### Further details

Please check CMakeFiles.txt files in this project, look for "COBBLETEXT_EMSCRIPTEN" for details.
