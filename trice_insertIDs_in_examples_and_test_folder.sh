#!/bin/bash

SCRIPT_DIRECTORY="$(dirname $(realpath "$0"))"
cd $SCRIPT_DIRECTORY
source ./trice_environment.sh

trice insert $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES -IDMin 13000 -IDMax 16383

cd - > /dev/null
