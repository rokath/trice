#!/bin/bash

# Initialize an empty string
flags=""

# Loop through all arguments
for arg in "$@"; do
    flags+="-D$arg "
done

# Trice is called here and not within make, to guarantee, it is finished before any other job starts.
trice add -src ../../_test/testdata -src ../exampleData # Make sure, these go unchanged into til.json.
# The folders  ../../_test/testdata  and ../exampleData are used by several projects and therefore not cleaned.
trice clean -src ./Core -cache # Run this first to trigger the used editor to show the Trice IDs cleaned state.
trice insert -src ../../_test/testdata -src ../exampleData -src ./Core  -IDMin 16200 -IDMax 16383 -IDMethod upward -cache # Add project data.

case "$OSTYPE" in
  darwin*)  make -j $(sysctl -n hw.ncpu) TRICE_FLAGS="$flags" gcc   -f OS_Darwin.mak ;; 
  linux*)   make -j $(nproc --all)       TRICE_FLAGS="$flags" gcc   -f OS_Linux.mak ;;
  msys*)    make -j $(nproc --all)       TRICE_FLAGS="$flags" clang -f OS_Windows.mak ;;
  cygwin*)  make -j $(nproc --all)       TRICE_FLAGS="$flags" clang -f OS_Windows.mak ;;
  *)        echo "unknown: $OSTYPE" ;;
  solaris*) echo "SOLARIS not implemented" ;;
  bsd*)     echo "BSD not implemented" ;;
esac

trice clean -src ./Core -cache # Run this again to show the Trice IDs cleaned state.
