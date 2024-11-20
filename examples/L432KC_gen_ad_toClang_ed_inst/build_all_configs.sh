#!/bin/bash

time for i in {0..36}; do echo; echo --- CONFIGURATION=$i ---; echo; ./clean.sh; ./build.sh CONFIGURATION=$i; done
