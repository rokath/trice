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
    ./build.sh
    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
    cd - > /dev/nul
done

for d in $VSCODETARGETDIRS
do
    cd $d
    make -j $(nproc --all)
    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
    cd - > /dev/nul
done


if ! [ $failCount -eq 0 ] ; then
  echo $failCount times FAIL 
fi
