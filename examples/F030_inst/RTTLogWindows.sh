#!/bin/bash
# Read live RTT logs from the connected board using Git Bash on Windows.
# Run ./RTTLogWindows.sh from this example directory after flashing the board.
# Requires trice.exe and the SEGGER J-Link software accessible to the decoder.
# The device name and decoder flags below must match your board and firmware.
# Press Ctrl-C to stop. -showID adds numeric IDs to help inspect decoded calls.

# Matching CLI values are matching triceConfig.h TRICE_DIRECT_OUTPUT=1 settings in this project.

trice.exe log -p jlink -args "-Device STM32F030R8" -pf none -prefix off -hs off -d16 -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json -stat
