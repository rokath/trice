#!/usr/bin/env bash

failCount=0

for d in ./F030_inst/ ./G0B1_inst/ ./L432_inst/
do
    cd $d
    ./clean.sh
    cd ..
    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
done

for d in ./F030_bare/ ./G0B1_bare/ ./L432_bare/
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
