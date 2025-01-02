#!/bin/bash

LIP="-liPath relative"

# Calling "make clean" does not remove the Trice IDs from the source code but "clean.sh" will do as well.
# We explicitely do not touch the ../exampleData folder, because it is used by several projects.
trice clean -cache $LIP -src ./Core -cache
make clean -f main.mak
