#!/bin/bash

# Initialize an empty string
flags=""

# Loop through all arguments
for arg in "$@"; do
    flags+="-D$arg "
done

# Trice command line (common part)
CL+="-cache " # Do not forget to create ~/.trice/cache folder, if the Trice cache should work.
CL+="-i ../../demoTIL.json " # Use common til.json for all examples and tests.
CL+="-li ../../demoLI.json " # Use common li.json for all examples and tests.
CL+="-liPath ./examples/L432_inst/relative " # Prefix base filenames in li.json with ./examples/L432_inst/.
CL+="-src ../../_test/testdata " # Do not include all files in ../../_test/.
CL+="-src ../exampleData "
CL+="-src ./Core "

# Trice is called here and not within make, to guarantee, it is finished before any other job starts.
trice clean  $CL # Run this first to trigger the used editor to show the Trice IDs cleaned state.
trice insert $CL -IDMin 13000 -IDMax 16383 -v # Add project data.

case "$OSTYPE" in
  darwin*)  make -j $(sysctl -n hw.ncpu) TRICE_FLAGS="$flags" gcc   -f OS_Darwin.mak ;; 
  linux*)   make -j $(nproc --all)       TRICE_FLAGS="$flags" gcc   -f OS_Linux.mak ;;
  msys*)    make -j $(nproc --all)       TRICE_FLAGS="$flags" clang -f OS_Windows.mak ;;
  cygwin*)  make -j $(nproc --all)       TRICE_FLAGS="$flags" clang -f OS_Windows.mak ;;
  *)        echo "unknown: $OSTYPE" ;;
  solaris*) echo "SOLARIS not implemented" ;;
  bsd*)     echo "BSD not implemented" ;;
esac

trice clean  $CL # Run this again to get the Trice IDs cleaned state.
