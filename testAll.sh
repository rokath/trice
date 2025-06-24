#!/bin/bash

# To perform just a quick test:
# ./testAll.sh

# To perform a full test:
# ./testAll.sh full

SELECTED=${1:-quick}

triceFolder=`pwd`
date                                                 2>&1 | tee    $triceFolder/testAll.log
echo This can take a while ...                       2>&1 | tee -a $triceFolder/testAll.log
t0=`date +%s`


# show environment and prepare
if command -v caffeinate 2>&1 >/dev/null; then
    caffeinate & # keep mac alive
fi
echo "Command line: $0 $1 $2"                        2>&1 | tee -a $triceFolder/testAll.log
echo "SELECTED: $SELECTED"                           2>&1 | tee -a $triceFolder/testAll.log
echo \$OSTYPE=$OSTYPE                                2>&1 | tee -a $triceFolder/testAll.log
if command -v uname; then                            2>&1 | tee -a $triceFolder/testAll.log
    uname -a                                         2>&1 | tee -a $triceFolder/testAll.log
fi
trice version                                        2>&1 | tee -a $triceFolder/testAll.log
./trice_cleanIDs_in_examples_and_test_folder.sh      2>&1 | tee -a $triceFolder/testAll.log
rm -f demoTIL.json demoLI.json                       2>&1 | tee -a $triceFolder/testAll.log
touch demoTIL.json demoLI.json                       2>&1 | tee -a $triceFolder/testAll.log
./renewIDs_in_examples_and_refresh_test_folder.sh    2>&1 | tee -a $triceFolder/testAll.log


if command -v go; then                               2>&1 | tee -a $triceFolder/testAll.log
    go version                                       2>&1 | tee -a $triceFolder/testAll.log

    # Go code tests
    echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
    echo "Testing the Go code..."                        2>&1 | tee -a $triceFolder/testAll.log
        go clean -cache -testcache                       2>&1 | tee -a $triceFolder/testAll.log
        go test ./...                                    2>&1 | tee -a $triceFolder/testAll.log
        rc=$?
        if [ $rc -ne 0 ]; then
            echo "Testing the Go code...failed"               | tee -a $triceFolder/testAll.log
            exit $rc
        fi
        if cat $triceFolder/testAll.log | grep -q FAIL ; then
            echo "Testing the Go code...FAILed"               | tee -a $triceFolder/testAll.log
            exit $rc
        fi
    echo "Testing the Go code...pass"                    2>&1 | tee -a $triceFolder/testAll.log
    echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log


    # Target code inside PC tests
    echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
    echo "Testing the Target code inside PC..."          2>&1 | tee -a $triceFolder/testAll.log
    ./trice_insertIDs_in_examples_and_test_folder.sh     2>&1 | tee -a $triceFolder/testAll.log
        cd _test
            if [ "$C_INCLUDE_PATH" != "" ]; then
                echo It is important, that C_INLUDE_PATH is not set for the CGO tests. Clearing it temporarily.
                export C_INCLUDE_PATH=""
            fi
            if [ "$SELECTED" = "quick" ]; then
                echo "go test ./be_dblB_de_tcobs_ua/..."         2>&1 | tee -a $triceFolder/testAll.log
                go test ./be_dblB_de_tcobs_ua/...                2>&1 | tee -a $triceFolder/testAll.log
                rc=$?
                if [ $rc -ne 0 ]; then
                    echo "Testing the Target code inside PC...failed" | tee -a $triceFolder/testAll.log
                    exit $rc
                fi
                # On build errors rc can be still 0, so we check the log file for FAIL
                if cat $triceFolder/testAll.log | grep -q FAIL ; then
                    echo "Testing the Target code inside PC...failed"               | tee -a $triceFolder/testAll.log
                    echo "In case of CGO build errors check the PATH variable too." | tee -a $triceFolder/testAll.log
                    exit 2
                fi
            fi
            if [ "$SELECTED" = "full" ]; then
                echo "go test ./..."                             2>&1 | tee -a $triceFolder/testAll.log
                go test ./...                                    2>&1 | tee -a $triceFolder/testAll.log
                rc=$?
                if [ $rc -ne 0 ]; then
                    echo "Testing the Target code inside PC...failed" | tee -a $triceFolder/testAll.log
                    exit $rc
                fi
                # On build errors rc can be still 0, so we check the log file for FAIL
                if cat $triceFolder/testAll.log | grep -q FAIL ; then
                    echo "Testing the Target code inside PC...failed"               | tee -a $triceFolder/testAll.log
                    echo "In case of CGO build errors check the PATH variable too." | tee -a $triceFolder/testAll.log
                    exit 2
                fi
            fi
        cd - >/dev/null
    ./trice_cleanIDs_in_examples_and_test_folder.sh      2>&1 | tee -a $triceFolder/testAll.log
    echo "Testing the Target code inside PC...pass"      2>&1 | tee -a $triceFolder/testAll.log
    echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
else
    echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
    echo "WARNING:     Go not installed."                2>&1 | tee -a $triceFolder/testAll.log
    echo "Skipping Go code and PC target code tests."    2>&1 | tee -a $triceFolder/testAll.log
    echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
fi

# clang translation test
if ! command -v clang                                    2>&1 | tee -a $triceFolder/testAll.log; then
    echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
    echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
    echo "WARNING:     clang not installed"              2>&1 | tee -a $triceFolder/testAll.log
    echo "Skipping clang arm target G0B1 translation."   2>&1 | tee -a $triceFolder/testAll.log
    echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
    echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
else
    clang --version                                          2>&1 | tee -a $triceFolder/testAll.log
    if ! command -v arm-none-eabi-gcc; then                  2>&1 | tee -a $triceFolder/testAll.log
        echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
        echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
        echo "WARNING:    arm-none-eabi-gcc not installed"   2>&1 | tee -a $triceFolder/testAll.log
        echo "Skipping clang arm target G0B1 translation."   2>&1 | tee -a $triceFolder/testAll.log
        echo "The arm-none-eabi-gcc libraries are needed."   2>&1 | tee -a $triceFolder/testAll.log
        echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
        echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
    else
        echo "---"                                                   2>&1 | tee -a $triceFolder/testAll.log
        echo "Translating G0B1_inst with clang..."                   2>&1 | tee -a $triceFolder/testAll.log
            cd examples/G0B1_inst
                make clean                                           2>&1 | tee -a $triceFolder/testAll.log 
                ./build_with_clang.sh                                2>&1 | tee -a $triceFolder/testAll.log
            cd - >/dev/null
            if cat $triceFolder/testAll.log | grep -q -e warning -e error ; then
                echo "Translating G0B1_inst with clang...failed"          | tee -a $triceFolder/testAll.log
                exit 2
            fi
        echo "Translating G0B1_inst with clang...pass"               2>&1 | tee -a $triceFolder/testAll.log
        echo "---"                                                   2>&1 | tee -a $triceFolder/testAll.log
    fi
fi


# set build environment
source ./build_environment.sh                            2>&1 | tee -a $triceFolder/testAll.log


# gcc translation tests
# We need the C_INCLUDE_PATH to point to the arm-none-eabi-gcc include files folder.
if command -v  arm-none-eabi-gcc; then                   2>&1 | tee -a $triceFolder/testAll.log
    arm-none-eabi-gcc --version | grep gcc               2>&1 | tee -a $triceFolder/testAll.log
fi
echo "C_INCLUDE_PATH=$C_INCLUDE_PATH"                    2>&1 | tee -a $triceFolder/testAll.log
echo arm-none-eabi-gcc location in next line:            2>&1 | tee -a $triceFolder/testAll.log
which arm-none-eabi-gcc                                  2>&1 | tee -a $triceFolder/testAll.log
if ! command -v arm-none-eabi-gcc; then                  2>&1 | tee -a $triceFolder/testAll.log
    echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
    echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
    echo "WARNING:     arm-none-eabi-gcc not installed"  2>&1 | tee -a $triceFolder/testAll.log
    echo "skipping gcc arm target examples translation"
    echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
    echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
else
    # translate gcc arm target examples
    ./trice_insertIDs_in_examples_and_test_folder.sh                 2>&1 | tee -a $triceFolder/testAll.log
        cd examples
            ./cleanAllTargets.sh                                     2>&1 | tee -a $triceFolder/testAll.log
            echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
            echo "Translating all examples with TRICE_OFF..."        2>&1 | tee -a $triceFolder/testAll.log
            ./buildAllTargets_TRICE_OFF.sh                           2>&1 | tee -a $triceFolder/testAll.log
            if cat $triceFolder/testAll.log | grep -q -e warning -e error ; then
                echo "Translating all examples with TRICE_OFF...failed"   | tee -a $triceFolder/testAll.log
                cd - >/dev/null
                exit 2
            fi
            echo "Translating all examples with TRICE_OFF...pass"    2>&1 | tee -a $triceFolder/testAll.log
            echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
            ./cleanAllTargets.sh                                     2>&1 | tee -a $triceFolder/testAll.log
            echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
            echo "Translating all examples with TRICE_ON..."         2>&1 | tee -a $triceFolder/testAll.log
            ./buildAllTargets_TRICE_ON.sh                            2>&1 | tee -a $triceFolder/testAll.log
            if cat $triceFolder/testAll.log | grep -q -e warning -e error ; then
                echo "Translating all examples with TRICE_ON...failed"    | tee -a $triceFolder/testAll.log
                cd - >/dev/null
                exit 2
            fi
            echo "Translating all examples with TRICE_ON...pass"     2>&1 | tee -a $triceFolder/testAll.log
            echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
            ./cleanAllTargets.sh                                     2>&1 | tee -a $triceFolder/testAll.log
        cd - >/dev/null
        if [ $SELECTED = "full" ]; then
            cd examples/L432_inst
                echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
                echo "Translating all L432 configurations..."        2>&1 | tee -a $triceFolder/testAll.log
                ./all_configs_build.sh                               2>&1 | tee -a $triceFolder/testAll.log
                if cat $triceFolder/testAll.log | grep -q -e warning -e error ; then
                    echo "Translating all L432 configurations...failed"   | tee -a $triceFolder/testAll.log
                    cd - >/dev/null
                    exit 2
                fi
                echo "Translating all L432 configurations...pass"    2>&1 | tee -a $triceFolder/testAll.log
                echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
            cd - >/dev/null
        fi
    ./trice_cleanIDs_in_examples_and_test_folder.sh                  2>&1 | tee -a $triceFolder/testAll.log
fi

t1=`date +%s`
#if command -v caffeinate 2>&1 >/dev/null
#then
#    # https://stackoverflow.com/questions/30171050/start-a-process-in-background-do-a-task-then-kill-the-process-in-the-background
#    kill %-                                          2>&1 | tee -a $triceFolder/testAll.log 
#fi
runtime=$((t1-t0))
echo Script run $runtime seconds.                    2>&1 | tee -a $triceFolder/testAll.log
