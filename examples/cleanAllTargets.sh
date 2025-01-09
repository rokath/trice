#!/usr/bin/env bash

failCount=0

INSTDIRS="./F030_inst/ ./G0B1_inst/ ./L432_inst/"

for d in $INSTDIRS
do
    cd $d
    ./triceCleanIDs.sh
    cd ..
    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
done

for d in $INSTDIRS ./F030_bare/ ./G0B1_bare/ ./L432_bare/
do
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
