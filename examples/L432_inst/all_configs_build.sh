#!/bin/bash
date
time for i in {0..100}; do echo; echo --- CONFIGURATION=$i ---; echo; make clean; ./build.sh CONFIGURATION=$i; done
