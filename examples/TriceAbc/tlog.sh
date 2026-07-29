#!/usr/bin/env bash

cd "$(dirname "$0")" || exit 1

tlog -port FILEBUFFER -args abc.bus \
  -li ../../demoLI.json \
  -liMaxDirs 1 \
  -til ../../demoTIL.json \
  -d16 \
  -pf COBS \
  -typeX0="counted:typeX0 buffer: %v\n"
