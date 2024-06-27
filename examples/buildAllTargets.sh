#!/usr/bin/env bash


ARMKEILTARGETDIRS="
MDK-ARM_STM32F030R8_generated
MDK-ARM_STM32F030R8_instrumented
"

VSCODETARGETDIRS="

Nucleo-STM32L432KC_adapted_toClang
Nucleo-STM32L432KC_generated

vsCode_Nucleo-F030R8_generated
vsCode_Nucleo-F030R8_instrumented

vsCode_Nucleo-G0B1_generated
vsCode_Nucleo-G0B1_instrumented

vsCode_Nucleo-L432KC_generated
vsCode_Nucleo-L432KC_instrumented
"
#OpenCM3_STM32F411_Nucleo

for d in $VSCODETARGETDIRS
do
echo ______ $d ______
cd $d
make -j8
cd -
echo -e
done

for d in $ARMKEILTARGETDIRS
do
echo ______ $d ______
cd $d/MDK-ARM
/c/Keil_v5/UV4/UV4.exe -rc MDK-ARM_STM32F030R8.uvprojx
cd -
echo -e
done

