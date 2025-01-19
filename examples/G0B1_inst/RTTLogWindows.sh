#!/bin/bash


# Matching CLI values are matching triceConfig.h TRICE_DIRECT_OUTPUT=1 settings in this project.





trice.exe log -p jlink -args "-Device STM32F030R8" -pf none -prefix off -hs off -d16 -showID "deb:%5d" -i ../../demoTIL.json -li ../../demoLI.json -stat -ts16 "time:offs:%4d µs" 
