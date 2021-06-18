#!/bin/bash

set -e
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
mv src/mod1 ..
