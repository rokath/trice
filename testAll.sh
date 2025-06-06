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

if command -v caffeinate 2>&1 >/dev/null; then
    caffeinate & # keep mac alive
fi

echo "Command line: $0 $1 $2"                        2>&1 | tee -a $triceFolder/testAll.log
echo "SELECTED: $SELECTED"                           2>&1 | tee -a $triceFolder/testAll.log
./trice_cleanIDs_in_examples_and_test_folder.sh      2>&1 | tee -a $triceFolder/testAll.log
rm -f demoTIL.json demoLI.json                       2>&1 | tee -a $triceFolder/testAll.log
touch demoTIL.json demoLI.json                       2>&1 | tee -a $triceFolder/testAll.log
./renewIDs_in_examples_and_refresh_test_folder.sh    2>&1 | tee -a $triceFolder/testAll.log
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
echo "Testing the Go code..."                        2>&1 | tee -a $triceFolder/testAll.log
go clean -cache -testcache                           2>&1 | tee -a $triceFolder/testAll.log
go test ./...                                        2>&1 | tee -a $triceFolder/testAll.log
echo "Testing the Go code...done"                    2>&1 | tee -a $triceFolder/testAll.log
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log

./trice_insertIDs_in_examples_and_test_folder.sh     2>&1 | tee -a $triceFolder/testAll.log
sleep 1.0
cd _test
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
echo "Testing the Target code inside PC..."          2>&1 | tee -a $triceFolder/testAll.log
if [ "$SELECTED" = "quick" ]; then
    echo "go test ./be_dblB_de_tcobs_ua/..."         2>&1 | tee -a $triceFolder/testAll.log
    go test ./be_dblB_de_tcobs_ua/...                2>&1 | tee -a $triceFolder/testAll.log
fi
if [ "$SELECTED" = "full" ]; then
    echo "go test ./..."                             2>&1 | tee -a $triceFolder/testAll.log
    go test ./...                                    2>&1 | tee -a $triceFolder/testAll.log
fi
cd - >/dev/null
sleep 1.0
./trice_cleanIDs_in_examples_and_test_folder.sh      2>&1 | tee -a $triceFolder/testAll.log
echo "Testing the Target code inside PC...done"      2>&1 | tee -a $triceFolder/testAll.log
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log

if ! command -v arm-none-eabi-gcc; then                  2>&1 | tee -a $triceFolder/testAll.log

    echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
    echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
    echo "WARNING:     arm-none-eabi-gcc not installed"  2>&1 | tee -a $triceFolder/testAll.log
    echo "skipping gcc arm target examples translation"
    echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
    echo ""                                              2>&1 | tee -a $triceFolder/testAll.log

else
    # translate gcc arm target examples

    ./trice_insertIDs_in_examples_and_test_folder.sh         2>&1 | tee -a $triceFolder/testAll.log
    cd examples
    ./cleanAllTargets.sh                                     2>&1 | tee -a $triceFolder/testAll.log
    echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
    echo "Translating all examples with TRICE_OFF..."        2>&1 | tee -a $triceFolder/testAll.log
    ./buildAllTargets_TRICE_OFF.sh                           2>&1 | tee -a $triceFolder/testAll.log
    echo "Translating all examples with TRICE_OFF...done"    2>&1 | tee -a $triceFolder/testAll.log
    echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
    ./cleanAllTargets.sh                                     2>&1 | tee -a $triceFolder/testAll.log
    echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
    echo "Translating all examples with TRICE_ON..."         2>&1 | tee -a $triceFolder/testAll.log
    ./buildAllTargets_TRICE_ON.sh                            2>&1 | tee -a $triceFolder/testAll.log
    echo "Translating all examples with TRICE_ON...done"     2>&1 | tee -a $triceFolder/testAll.log
    echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
    ./cleanAllTargets.sh                                     2>&1 | tee -a $triceFolder/testAll.log
    cd - >/dev/null
    ./trice_cleanIDs_in_examples_and_test_folder.sh          2>&1 | tee -a $triceFolder/testAll.log

    if [ $SELECTED = "full" ]; then
        ./trice_insertIDs_in_examples_and_test_folder.sh     2>&1 | tee -a $triceFolder/testAll.log
        cd examples/L432_inst
        echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
        echo "Translating all L432 configurations..."        2>&1 | tee -a $triceFolder/testAll.log
        ./all_configs_build.sh                               2>&1 | tee -a $triceFolder/testAll.log
        echo "Translating all L432 configurations...done"    2>&1 | tee -a $triceFolder/testAll.log
        echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
        cd - >/dev/null
        ./trice_cleanIDs_in_examples_and_test_folder.sh      2>&1 | tee -a $triceFolder/testAll.log
    fi

fi

if ! command -v clang                                    2>&1 | tee -a $triceFolder/testAll.log; then

    echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
    echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
    echo "WARNING:     clang not installed"              2>&1 | tee -a $triceFolder/testAll.log
    echo "skipping clang arm target G0B1 translation"
    echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
    echo ""                                              2>&1 | tee -a $triceFolder/testAll.log

else

    if ! command -v arm-none-eabi-gcc; then                  2>&1 | tee -a $triceFolder/testAll.log

        echo ""                                              2>&1 | tee -a $triceFolder/testAll.log
        echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
        echo "WARNING:    arm-none-eabi-gcc not installed"   2>&1 | tee -a $triceFolder/testAll.log
        echo "skipping clang arm target G0B1 translation"    2>&1 | tee -a $triceFolder/testAll.log
        echo "The arm-none-eabi-gcc libraries are needed."   2>&1 | tee -a $triceFolder/testAll.log
        echo "############################################"  2>&1 | tee -a $triceFolder/testAll.log
        echo ""                                              2>&1 | tee -a $triceFolder/testAll.log

    else

        echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
        cd examples/G0B1_inst
        make clean                                               2>&1 | tee -a $triceFolder/testAll.log 
        echo "Translating G0B1_inst with clang..."               2>&1 | tee -a $triceFolder/testAll.log
        ../../trice_insertIDs_in_examples_and_test_folder.sh     2>&1 | tee -a $triceFolder/testAll.log

        make -j clang || echo "examples/G0B1_inst/Makefile: C_INCLUDE_PATH ok?" 2>&1 | tee -a $triceFolder/testAll.log

        ../../trice_cleanIDs_in_examples_and_test_folder.sh      2>&1 | tee -a $triceFolder/testAll.log
        echo "Translating G0B1_inst with clang...done"           2>&1 | tee -a $triceFolder/testAll.log
        echo "---"                                               2>&1 | tee -a $triceFolder/testAll.log
    fi

fi

t1=`date +%s`
#if command -v caffeinate 2>&1 >/dev/null
#then
#    # https://stackoverflow.com/questions/30171050/start-a-process-in-background-do-a-task-then-kill-the-process-in-the-background
#    kill %-                                          2>&1 | tee -a $triceFolder/testAll.log 
#fi
runtime=$((t1-t0))
echo Script run $runtime seconds.                    2>&1 | tee -a $triceFolder/testAll.log
