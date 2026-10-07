#!/bin/bash
# Build the uninstrumented STM32G0B1 firmware; no flashing is performed.
# Run ./build.sh from this directory. Requires Bash, make and Arm GNU tools.
# source loads the shared toolchain setup into this shell; MAKE_JOBS tells
# make how many compiler jobs may run together.

source ../../scripts/_150_setup_build_environment.sh
make $MAKE_JOBS
