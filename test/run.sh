#!/bin/sh

cd build
make
rm test.db
./test
cd ..