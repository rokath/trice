#!/bin/bash

ARG1=${1:-'TRICE_OFF=0'} # https://stackoverflow.com/questions/9332802/how-to-write-a-bash-script-that-takes-optional-input-arguments

# Trice is called not within the Makefile, to guarantee, it is finished before any other job starts.
trice add -src ../../_test/testdata -src ../exampleData # Make sure, these go unchanged into til.json.
# The folders  ../../_test/testdata  and ../exampleData are used by several projects and therefore not cleaned.
trice clean -src ./Core -cache # Run this first to trigger the used editor to show the Trice IDs cleaned state.
trice insert -src ../../_test/testdata -src ../exampleData -src ./Core  -IDMin 16200 -IDMax 16383 -IDMethod upward -cache # Add project data.
#make -j $(nproc --all) TRICE_FLAGS=-D$ARG1 # Windows
make -j $(sysctl -n hw.ncpu) TRICE_FLAGS=-D$ARG1 # MacOS
trice clean -src ./Core -cache # Run this again to show the Trice IDs cleaned state.
