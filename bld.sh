#!/bin/bash

function build {
    rm -R build
    mkdir build
    cd build
    # cmake -DYOCTO=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
    make
}

case $1 in
  "vcpkg")
    /home/jari/vcpkg/vcpkg install --triplet x64-linux-release
    ;;
  "build")
    build
    ;;
  "wc")
    cloc --exclude-dir=expert --exclude-lang="CMake,Bourne Shell" mqtt weather spot
    ;;
esac
