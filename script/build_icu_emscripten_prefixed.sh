#!/bin/bash
set -e
set -x

SCRIPT_PATH=$(readlink -f "$BASH_SOURCE")
SCRIPT_DIR=$(dirname "$SCRIPT_PATH")

mkdir -p icu_emscripten_prefixed/
cd icu_emscripten_prefixed/

if [! -d "icu" ]; then
    git clone https://github.com/mabels/icu/ --depth 10 --branch wasm32
fi
mkdir -p icu_build icu_installed
cd icu_build
ICU_DATA_FILTER_FILE=$SCRIPT_DIR/../misc/icu/icu_data_filter.json \
    ../icu/icu4c/source/runConfigureICU wasm32 \
        --with-library-suffix=cobbletext \
        --prefix=`pwd`/../icu_installed
cp ../icu/icu4c/source/tools/toolutil/nodejs-system.js tools/toolutil/
make
make install

echo "Done"
