#!/bin/bash

# Initialize an empty string
flags=""

# Loop through all arguments
for arg in "$@"; do
  if [ $arg = "TRICE_OFF=1" ]; then
    export triceOFF="1"
  fi
  flags+="-D$arg "
done

# Trice is called here and not within make, to guarantee, it is finished before any other job starts.
../../trice_cleanIDs_in_examples_and_test_folder.sh # Run this first to trigger the used editor to show the Trice IDs cleaned state.

if [ "$triceOFF" != "1" ]; then
  ../../trice_insertIDs_in_examples_and_test_folder.sh # custom aliases should be excluded or without IDs, when translating with TRICE_OFF=1
fi

source ../../build_environment.sh
make $MAKE_JOBS TRICE_FLAGS="$flags" gcc # We translate here with inserted IDs, even in the triceOff case and expect no warnings.

../../trice_cleanIDs_in_examples_and_test_folder.sh # Run this again to get the Trice IDs cleaned state.

# Additionally we again translate the triceOff case without IDs and expect no warnings.
if [ "$triceOFF" == "1" ]; then
  make clean
  make $MAKE_JOBS TRICE_FLAGS="$flags" gcc
fi
