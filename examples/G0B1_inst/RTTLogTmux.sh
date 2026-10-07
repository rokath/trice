#!/bin/bash
# Read live RTT logs from the connected, flashed board.
# Run ./RTTLogTmux.sh from this example directory.
# Requires Trice, SEGGER J-Link tools and tmux.
# First create an empty capture file, then run JLinkRTTLogger in the background.
# Trice follows that growing file (-p FILE) and decodes using the shared tables.
# Transport/value-width options must match triceConfig.h. Stop with Ctrl-C.
# Linux: Needs "sudo apt install tmux" or similar done before.
# Darwin: needs "brew install tmux"
# Matching CLI values are matching triceConfig.h TRICE_DIRECT_OUTPUT=1 settings in this project.

mkdir -p ./temp
rm -f ./temp/trice.bin
touch ./temp/trice.bin
tmux new -s "tricerttlog" -d "JLinkRTTLogger -Device STM32G0B1RE -If SWD -Speed 4000 -RTTChannel 0 ./temp/trice.bin"
trice log -p FILE -args ./temp/trice.bin -pf none -prefix off -hs off -d16 -ts16 "time:offs:%4d µs" -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json -stat -ts32="epoch060102150405" -typeX0=counted:"sig:typeX0 packet:% x\n"
tmux kill-session -t "tricerttlog"
