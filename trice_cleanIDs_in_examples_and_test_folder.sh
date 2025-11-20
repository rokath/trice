#!/bin/bash

# file name: trice_cleanIDs_in_examples_and_test_folder.sh

SCRIPT_DIRECTORY="$(dirname $(realpath "$0"))"
cd $SCRIPT_DIRECTORY
source ./trice_environment.sh

trice clean  $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES 

cd - > /dev/null
