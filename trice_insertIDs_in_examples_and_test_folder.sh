#!/bin/bash

# file name: trice_insertIDs_in_examples_and_test_folder.sh

cd "$(dirname "$0")" || exit 1
source ./scripts/_setup_trice_environment.sh

trice insert $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES -IDMin 13000 -IDMax 16383

cd - >/dev/null || exit
