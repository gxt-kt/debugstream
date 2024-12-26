#!/bin/bash

cmake -S . -B build -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF
cmake --build build -j
sudo cmake --install build
