#!/bin/bash

# You can call this script without args or with the argument TRICE_OFF=1 to exclude all Trice code from the project.
ARG1=${1:-'TRICE_OFF=0'} # https://stackoverflow.com/questions/9332802/how-to-write-a-bash-script-that-takes-optional-input-arguments

# Trice is called here and not within make, to guarantee, it is finished before any other job starts.
trice clean  -src ../../_test/testdata -src ../exampleData -src ./Core -cache # Run this first to trigger the used editor to show the Trice IDs cleaned state.
trice insert -src ../../_test/testdata -src ../exampleData -src ./Core -cache -IDMin 16200 -IDMax 16383 -IDMethod upward # Add project data.

case "$OSTYPE" in
  darwin*)  make -j $(sysctl -n hw.ncpu) TRICE_FLAGS=-D$ARG1 -f OS_Darwin.mak ;; 
  linux*)   make -j $(nproc --all)       TRICE_FLAGS=-D$ARG1 -f OS_Linux.mak ;;
  msys*)    make -j $(nproc --all)       TRICE_FLAGS=-D$ARG1 -f OS_Windows.mak ;;
  cygwin*)  make -j $(nproc --all)       TRICE_FLAGS=-D$ARG1 -f OS_Windows.mak ;;
  *)        echo "unknown: $OSTYPE" ;;
  solaris*) echo "SOLARIS not implemented" ;;
  bsd*)     echo "BSD not implemented" ;;
esac

trice clean  -src ../../_test/testdata -src ../exampleData -src ./Core -cache # Run this again to show the Trice IDs cleaned state.
