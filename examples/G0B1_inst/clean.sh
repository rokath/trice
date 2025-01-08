#!/bin/bash

LIP="-liPath ./examples/G0B1_inst/relative"

# Calling "make clean" does not remove the Trice IDs from the source code but "clean.sh" will do as well.
trice clean -cache -i ../../demoTIL.json -li ../../demoLI.json $LIP -src ./Core -src ../exampleData -src ../../_test/testdata 
make clean -f main.mak
