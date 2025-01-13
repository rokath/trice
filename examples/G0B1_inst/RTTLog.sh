#!/bin/bash

# Needs "sudo apt install screen" done before.

rm -f ./temp/trice.bin
touch ./temp/trice.bin
screen -d -m JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin
trice log -p FILE -args ./temp/trice.bin -prefix off -hs off -d16 -ts ms  -i ../../demoTIL.json -li ../../demoLI.json -pf none
screen -X quit
