#!/bin/bash

mkdir -p build/
cd build/
cmake ..
if [ "$1" = "" ]; then
    make main
else
    make $1
fi
cd ..
