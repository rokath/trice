#!/bin/bash

case "$OSTYPE" in
  darwin*)  make -j $(sysctl -n hw.ncpu) -f OS_Darwin.mak ;; 
  linux*)   make -j $(nproc --all)       -f OS_Linux.mak ;;
  msys*)    make -j $(nproc --all)       -f OS_Windows.mak ;;
  cygwin*)  make -j $(nproc --all)       -f OS_Windows.mak ;;
  *)        echo "unknown: $OSTYPE" ;;
  solaris*) echo "SOLARIS not implemented" ;;
  bsd*)     echo "BSD not implemented" ;;
esac

