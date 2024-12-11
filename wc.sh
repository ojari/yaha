#!/bin/sh

# wc mqtt/*.[ch]pp mqtt/automation/*.[ch]pp mqtt/data/*.[ch]pp  mqtt/device/*.[ch]pp  mqtt/task/*.[ch]pp weather/*.[ch]pp spot/*.[ch]pp

cloc --exclude-dir=expert --exclude-lang="CMake,Bourne Shell" mqtt weather spot