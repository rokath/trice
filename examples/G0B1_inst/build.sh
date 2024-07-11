#!/bin/bash

# trice is called not within the Makefile, to guarantee, it is finished before any other job starts.
trice update -src ./Core -src ../../test/testdata
make -j $(nproc --all)
