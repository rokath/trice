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
"

failCount=0

for d in $VSCODETARGETDIRS
do
    cd $d
    echo $d
    make -j $(nproc --all)
    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
    cd - > /dev/nul
done


if [ $failCount -eq 0 ] ; then
  echo all ok
else
  echo $failCount times FAIL 
fi
