#!/usr/bin/env bash

VSCODETARGETDIRS="
    F030R8_gen/
    G0B1_gen/
    L432KC_gen/
    L432KC_gen_ad_toClang/
    L432KC_gen_ad_toClang_ed/
"

INSTRUMENTEDDIRS="
    F030R8_inst/
    G0B1_inst/
    L432KC_gen_ad_toClang_ed_instr/
"

failCount=0

for d in $INSTRUMENTEDDIRS
do
    cd $d
    echo --------------------------------------------------------------------------------------------------------
    echo $d with TRICE_OFF=1
    ./clean.sh
    ./build.sh TRICE_OFF=1
    echo --------------------------------------------------------------------------------------------------------
    echo $d with TRICE_OFF=0
    ./clean.sh
    ./build.sh
    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
    cd ..
done

for d in $VSCODETARGETDIRS
do
    cd $d
    echo --------------------------------------------------------------------------------------------------------
    echo $d
    #make -j $(nproc --all) # Windows
    make -j $(sysctl -n hw.ncpu) # MacOS
    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
    cd ..
done


if ! [ $failCount -eq 0 ] ; then
  echo $failCount times FAIL 
fi
