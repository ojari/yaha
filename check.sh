#!/bin/sh

# wc mqtt/*.[ch]pp mqtt/data/*.[ch]pp weather/*.[ch]pp
echo "-------------------------------------------------------------------------- cppcheck"
cppcheck -ibuild -imqtt/expert -q .

echo "-------------------------------------------------------------------------- build test"
cd build
make test/fast

echo "-------------------------------------------------------------------------- test"
test/test --reporter=compact
cd ..