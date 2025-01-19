#!/bin/bash

trice_v0.72.2.exe log -liPathIsRelative=true -p jlink -args "-Device STM32F030R8" -pf none -prefix off -hs off -d16 -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json
