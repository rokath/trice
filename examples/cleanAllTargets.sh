#!/usr/bin/env bash

failCount=0

INSTRUMENTEDDIRS="
    F030R8_inst/
    G0B1_inst/
    L432KC_gen_ad_toClang_ed_instr/
"

NONINSTRUMENTEDDIRS="
    F030R8_gen/
    G0B1_gen/
    L432KC_gen/
    L432KC_gen_ad_toClang/
    L432KC_gen_ad_toClang_ed/
    OpenCM3_STM32F411_Nucleo/
"

for d in $INSTRUMENTEDDIRS
do
    echo $d
    cd $d
    ./clean.sh
    cd ..
    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
done
for d in $NONINSTRUMENTEDDIRS
do
    echo $d
    cd $d
    make clean
    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
    cd ..
done

if ! [ $failCount -eq 0 ] ; then
  echo $failCount times FAIL 
fi
