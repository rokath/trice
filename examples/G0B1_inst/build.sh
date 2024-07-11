#!/bin/bash

# trice is called not within the Makefile, to guarantee, it is finished before any other job starts.
trice update -src ../../test/testdata
trice insert -src ./Core
make -j $(nproc --all)
