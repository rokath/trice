#!/bin/bash

# Calling "make clean" does not remove the Trice IDs from the source code but "clean.sh" will do as well.
# We explicitely do not touch the ../../test/testdata folder.
trice clean -src ./Core
make clean