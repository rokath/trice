#!/bin/bash

# Trice is called not within the Makefile, to guarantee, it is finished before any other job starts.
trice add -src ../../test/testdata -src ../exampleData # Make sure, these go unchanged into til.json.
trice insert -src ../../test/testdata -src ../exampleData -src ./Core  -IDMin 16200 -IDMax 16383 -IDMethod upward # Add project data.
make -j $(nproc --all)
