#!/bin/bash

# file name: trice_cleanIDs_in_examples_and_test_folder.sh

cd "$(dirname "$0")" || exit 1
source ./scripts/_setup_trice_environment.sh

trice clean $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES

cd - >/dev/null || exit
