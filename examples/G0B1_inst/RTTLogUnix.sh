#!/bin/bash
# Needs "sudo apt install screen" or similar done before.

rm -f ./temp/trice.bin
touch ./temp/trice.bin
screen -d -m JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin
trice log -p FILE -args ./temp/trice.bin -pf none -prefix off -hs off -d16 -ts ms -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json
screen -X quit
