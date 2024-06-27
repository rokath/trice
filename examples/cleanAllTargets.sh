#!/usr/bin/env bash

VSCODETARGETDIRS="

Nucleo-STM32L432KC_adapted_toClang
Nucleo-STM32L432KC_generated

vsCode_Nucleo-F030R8_generated
vsCode_Nucleo-F030R8_instrumented

vsCode_Nucleo-G0B1_generated
vsCode_Nucleo-G0B1_instrumented

vsCode_Nucleo-L432KC_generated
vsCode_Nucleo-L432KC_instrumented

OpenCM3_STM32F411_Nucleo
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
rm -rf temp/
cd $d/MDK-ARM
rm -rf RTE/ MDK-ARM_STM32F030R8/ DebugConfig/ *._2i *.crf *.d *.o *.fed *.hex *.htm *.l2p *.map *.lnt *.lnp *.dep *.axf *.lst *.uvguix.* *.iex EventRecorderStub.scvd *.uvoptx
cd -
echo -e
done
trice clean -src ./MDK-ARM_STM32F030R8_instrumented -src ./exampleData/triceCheck.c -til ./MDK-ARM_STM32F030R8_instrumented/til.json -li ./MDK-ARM_STM32F030R8_instrumented/li.json
