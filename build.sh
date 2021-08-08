#!/bin/bash

set -e
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DOpenGL_GL_PREFERENCE=LEGACY ..
cmake --build .
mv src/mod1 ..
