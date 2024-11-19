#!/bin/bash

time for i in {0..11}; do echo --- CONFIGURATION=$i ---; ./build.sh TRICE_OFF=0 CONFIGURATION=$i; ./clean.sh; done
