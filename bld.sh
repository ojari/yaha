#!/bin/bash

function build {
    cd build
    # cmake -DYOCTO=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON --toolchain etc/toolchain-x64.cmake ..
    make
  cd ..
}

function expert {
  cd build
  # cmake -DYOCTO=1 -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON --toolchain etc/toolchain-x64.cmake ..
  make expert
  cd ..
}

function build-arm {
    cd _build_arm
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON --toolchain etc/toolchain-arm64.cmake ..
    make
}

case $1 in
  "vcpkg")
    /home/jari/vcpkg/vcpkg install --triplet x64-linux-release
    ;;
  "vcpkg-arm")
    /home/jari/vcpkg/vcpkg install --triplet arm64-linux-release
    ;;
  "clean")
    rm -R build
    mkdir build
    ;;
  "build")
    build
    ;;
  "run")
    ./build/mqtt/yaha
    ;;
  "clean-arm")
    rm -R _build_arm
    mkdir _build_arm
    ;;
  "build-arm")
    build-arm
    ;;
  "export")
    scp _build_arm/mqtt/yaha pi@${TARGET_HOST}:
    scp *.json pi@${TARGET_HOST}:
    ;;
  "wc")
    cloc --exclude-dir=expert --exclude-lang="CMake,Bourne Shell" mqtt weather spot
    ;;
esac
