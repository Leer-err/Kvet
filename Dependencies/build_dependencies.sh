#!/bin/bash

PWD=$(pwd)

git submodule update --init --recursive

#Assimp
cmake -G Ninja -DASSIMP_BUILD_TESTS=off -DASSIMP_INSTALL=off -S $PWD/assimp -B $PWD/assimp/build
cmake --build $PWD/assimp/build

#SDL

cmake -S $PWD/sdl -B $PWD/sdl/build
cmake --build $PWD/sdl/build

#Tracy

cmake -S $PWD/tracy -B $PWD/tracy/build
cmake --build $PWD/tracy/build