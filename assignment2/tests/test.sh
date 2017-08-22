#!/bin/bash

if [[ $# -ne 1 ]]
then
    echo "Usage: test.sh CODE_DIR"
    exit
fi

# Copy EuclideanVector code into this dir.
CODE_DIR="$1"
cp "${CODE_DIR}EuclideanVector.cpp" .
cp "${CODE_DIR}EuclideanVector.h" .

# Compile our test-suite..
make

# Run the tests.
./EuclideanVectorTester

# Clean everything up.
make clean
rm "EuclideanVector.cpp" "EuclideanVector.h"
