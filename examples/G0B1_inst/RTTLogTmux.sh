#!/bin/bash
# Linux: Needs "sudo apt install tmux" or similar done before.
# Darwin: needs "brew install tmux"
# Matching CLI values are matching triceConfig.h TRICE_DIRECT_OUTPUT=1 settings in this project.

mkdir -p ./temp
rm -f ./temp/trice.bin
touch ./temp/trice.bin
tmux new -s "tricerttlog" -d "JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin"
trice log -p FILE -args ./temp/trice.bin -pf none -prefix off -hs off -d16 -ts16 "time:offs:%4d µs" -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json -stat
tmux kill-session -t "tricerttlog"
