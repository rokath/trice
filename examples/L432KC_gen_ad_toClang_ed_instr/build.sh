#!/bin/bash

# Trice is called not within the Makefile, to guarantee, it is finished before any other job starts.
trice add -src ../../_test/testdata -src ../exampleData # Make sure, these go unchanged into til.json.
trice insert -src ../../_test/testdata -src ../exampleData -src ./Core  -IDMin 16200 -IDMax 16383 -IDMethod upward -cache # Add project data.
make -j $(nproc --all)
trice clean -src ./Core -cache
