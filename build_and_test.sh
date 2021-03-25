#!/bin/bash

set -e
set -x

rm -rf build
mkdir build
pushd build

cmake .. -DBUILD_SIGHAN_TESTS=on
cmake --build .

bin/sighan_tests
