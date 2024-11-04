#!/usr/bin/env bash

VSCODETARGETDIRS="
    F030R8_gen/
    F030R8_inst/
    G0B1_gen/
    G0B1_inst/
    L432KC_gen/
    L432KC_gen_ad_toClang/
    L432KC_gen_ad_toClang_ed/
    L432KC_gen_ad_toClang_ed_instr/
    OpenCM3_STM32F411_Nucleo/
"
failCount=0

INSTRUMENTEDDIRS="
    F030R8_inst/
    G0B1_inst/
    L432KC_gen_ad_toClang_ed_instr/
"

for d in $VSCODETARGETDIRS
do
    cd $d
    for i in $INSTRUMENTEDDIRS
    do
        if [ $i == $d ] ; then
            ./clean.sh
        fi
    done
    #make -j $(nproc --all) clean # Windows
    make -j $(sysctl -n hw.ncpu) clean # MacOS
    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
    cd ..
done

if ! [ $failCount -eq 0 ] ; then
  echo $failCount times FAIL 
fi
