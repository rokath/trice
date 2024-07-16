#!/bin/bash

# trice is called not within the Makefile, to guarantee, it is finished before any other job starts.
trice insert -src ./Core -src ../../test/testdata -src ../exampleData
make -j $(nproc --all)
