#!/usr/bin/env bash

VSCODETARGETDIRS="
Nucleo-STM32L432KC_generated
vsCode_Nucleo-G0B1_generated
vsCode_Nucleo-G0B1_instrumented
vsCode_Nucleo-L432KC_generated
vsCode_Nucleo-L432KC_instrumented
"

for d in $VSCODETARGETDIRS
do
echo ______ $d ______
cd $d
make clean
cd -
echo -e
done

ARMKEILTARGETDIRS="
MDK-ARM_STM32F030R8_generated
MDK-ARM_STM32F030R8_instrumented
"

for d in $ARMKEILTARGETDIRS
do
echo ______ $d ______
cd $d/MDK-ARM
rm -rf RTE/ MDK-ARM_STM32F030R8/ DebugConfig/ *._2i *.crf *.d *.o *.fed *.hex *.htm *.l2p *.map *.lnt *.lnp *.dep *.axf *.lst
cd -
echo -e
done
