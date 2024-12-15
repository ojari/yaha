#!/bin/bash

function build {
    rm -R build
    mkdir build
    cd build
    # cmake -DYOCTO=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON --toolchain etc/toolchain-x64.cmake ..
    make
}

function build-arm {
    rm -R _build_arm
    mkdir _build_arm
    cd _build_arm
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON --toolchain etc/toolchain-arm64.cmake ..
    make
}

case $1 in
  "vcpkg")
    /home/jari/vcpkg/vcpkg install --triplet x64-linux-release
    ;;
  "vcpkg-arm")
    /home/jari/vcpkg/vcpkg install --triplet arm64-linux-release --debug
    ;;
  "build")
    build
    ;;
  "build-arm")
    build-arm
    ;;
  "wc")
    cloc --exclude-dir=expert --exclude-lang="CMake,Bourne Shell" mqtt weather spot
    ;;
esac
