#!/bin/bash
# Needs "sudo apt install screen" or similar done before.
# Matching for triceConfig.h CONFIGURATION with TRICE_DIRECT_OUTPUT=1

mkdir -p ./temp
rm -f ./temp/trice.bin
touch ./temp/trice.bin
screen -d -m JLinkRTTLogger -Device STM32F030R8 -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin
trice log -p FILE -args ./temp/trice.bin -pf none -prefix off -hs off -d16 -ts16 "time:tick #%6d" -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json
screen -X quit
