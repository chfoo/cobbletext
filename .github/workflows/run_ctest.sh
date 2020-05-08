#!/bin/bash

# ctest copies the libraries to the executable directory automatically
# on Windows. Otherwise, will use the ld/dyld environment variables.
# On macOS, DYLD_FALLBACK_LIBRARY_PATH is stripped on child processes
# which is why this wrapper script exists..
set -x
set -e

TRIPLET="$1"

LD_LIBRARY_PATH="$GITHUB_WORKSPACE/vcpkg/installed/$TRIPLET/lib/" \
    DYLD_FALLBACK_LIBRARY_PATH="$GITHUB_WORKSPACE/vcpkg/installed/$TRIPLET/lib/:$GITHUB_WORKSPACE/build/bin/:/usr/local/lib:/lib:/usr/lib" \
    ctest --verbose -C Debug
