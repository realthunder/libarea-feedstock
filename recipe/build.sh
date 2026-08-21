#!/bin/bash
set -ex

cp "${RECIPE_DIR}/CMakeLists.txt" CMakeLists.txt

cmake -G Ninja -B build -S . \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="${PREFIX}" \
    -DCMAKE_INSTALL_LIBDIR=lib \
    -DBUILD_SHARED_LIBS=ON

cmake --build build --target install
