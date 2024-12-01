#!/usr/bin/env bash

NONINSTRUMENTEDDIRS="
    F030_bare/
    G0B1_bare/
    L432_bare/
"

INSTRUMENTEDDIRS="
    F030_inst/
    G0B1_inst/
    L432_inst/
"

failCount=0

for d in $INSTRUMENTEDDIRS
do
    cd $d
    #echo --------------------------------------------------------------------------------------------------------
    #echo $d with TRICE_OFF=1
    #./clean.sh
    #./build.sh TRICE_OFF=1
    echo --------------------------------------------------------------------------------------------------------
    echo $d with TRICE_OFF=0
    #./clean.sh
    ./build.sh
    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
    cd ..
done

for d in $NONINSTRUMENTEDDIRS
do
    cd $d
    echo --------------------------------------------------------------------------------------------------------
    echo $d

    case "$OSTYPE" in
    darwin*)  make -j $(sysctl -n hw.ncpu)   ;; 
    linux*)   make -j $(nproc --all)         ;;
    msys*)    make -j $(nproc --all)         ;;
    cygwin*)  make -j $(nproc --all)         ;;
    *)        echo "unknown: $OSTYPE"        ;;
    solaris*) echo "SOLARIS not implemented" ;;
    bsd*)     echo "BSD not implemented"     ;;
    esac

    if ! [ $? -eq 0 ] ; then
        failCount=$((failCount + 1))
        echo FAIL: $d
    fi
    cd ..
done

if ! [ $failCount -eq 0 ] ; then
  echo $failCount times FAIL 
fi
