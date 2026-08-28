#!/bin/bash
set -ex

# ${CMAKE_ARGS} carries conda's toolchain file. A native build does not
# miss it; a cross build cannot do without it -- that is where
# CMAKE_SYSTEM_NAME and the target sysroot come from.
cmake ${CMAKE_ARGS} -G Ninja -B build -S . \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="${PREFIX}" \
    -DCMAKE_INSTALL_LIBDIR=lib \
    -DBUILD_SHARED_LIBS=ON

cmake --build build --target install
