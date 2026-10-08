#!/bin/bash
# Read live RTT logs from the connected, flashed board.
# Run ./RTTLogUnix.sh from this example directory.
# Requires Trice, SEGGER J-Link tools and screen.
# First create an empty capture file, then run JLinkRTTLogger in the background.
# Trice follows that growing file (-p FILE) and decodes using the shared tables.
# Transport/value-width options must match triceConfig.h. Stop with Ctrl-C.
# Needs "sudo apt install screen" or similar done before.
# Matching for triceConfig.h CONFIGURATION with TRICE_DIRECT_OUTPUT=1

mkdir -p ./temp
rm -f ./temp/trice.bin
touch ./temp/trice.bin
screen -d -m JLinkRTTLogger -Device STM32L432KC -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin
trice log -p FILE -args ./temp/trice.bin -pf none -prefix off -hs off -d16 -ts us -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json
screen -X quit
