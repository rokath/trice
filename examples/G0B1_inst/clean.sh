#!/bin/bash

LIP="-liPath relative"

# Calling "make clean" does not remove the Trice IDs from the source code but "clean.sh" will do as well.
trice clean -cache $LIP -src ./Core -src ../exampleData -src ../../_test/testdata 
make clean -f main.mak
