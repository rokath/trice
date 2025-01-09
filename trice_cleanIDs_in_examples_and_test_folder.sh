#!/bin/bash

SCRIPT_DIRECTORY="$(dirname $(realpath "$0"))"
cd $SCRIPT_DIRECTORY
source ./trice_environment.sh

trice clean $TRICE_CL

cd - > /dev/null
