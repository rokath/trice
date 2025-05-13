#!/bin/bash
triceFolder=`pwd`
date                                                 2>&1 | tee    $triceFolder/testAll.log
echo This can take a while ...                       2>&1 | tee -a $triceFolder/testAll.log
t0=`date +%s`
if command -v caffeinate 2>&1 >/dev/null
then
    caffeinate & # keep mac alive
fi

./renewIDs_in_examples_and_refresh_test_folder.sh    2>&1 | tee -a $triceFolder/testAll.log
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
echo "Testing the Go code..."                        2>&1 | tee -a $triceFolder/testAll.log
go clean -cache -testcache                           2>&1 | tee -a $triceFolder/testAll.log
go test ./...                                        2>&1 | tee -a $triceFolder/testAll.log
echo "Testing the Go code...done"                    2>&1 | tee -a $triceFolder/testAll.log
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log

./trice_insertIDs_in_examples_and_test_folder.sh     2>&1 | tee -a $triceFolder/testAll.log
cd _test
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
echo "Testing the Target code..."                    2>&1 | tee -a $triceFolder/testAll.log
go test ./...                                        2>&1 | tee -a $triceFolder/testAll.log
cd - >/dev/null
./trice_cleanIDs_in_examples_and_test_folder.sh      2>&1 | tee -a $triceFolder/testAll.log
echo "Testing the Target code...done"                2>&1 | tee -a $triceFolder/testAll.log
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log

./trice_insertIDs_in_examples_and_test_folder.sh     2>&1 | tee -a $triceFolder/testAll.log
cd examples
./cleanAllTargets.sh                                 2>&1 | tee -a $triceFolder/testAll.log
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
echo "Translating all examples with TRICE_OFF..."    2>&1 | tee -a $triceFolder/testAll.log
./buildAllTargets_TRICE_OFF.sh                       2>&1 | tee -a $triceFolder/testAll.log
echo "Translating all examples with TRICE_OFF...done"2>&1 | tee -a $triceFolder/testAll.log
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
./cleanAllTargets.sh                                 2>&1 | tee -a $triceFolder/testAll.log
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
echo "Translating all examples with TRICE_ON..."     2>&1 | tee -a $triceFolder/testAll.log
./buildAllTargets_TRICE_ON.sh                        2>&1 | tee -a $triceFolder/testAll.log
echo "Translating all examples with TRICE_ON...done" 2>&1 | tee -a $triceFolder/testAll.log
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
./cleanAllTargets.sh                                 2>&1 | tee -a $triceFolder/testAll.log
cd - >/dev/null
./trice_cleanIDs_in_examples_and_test_folder.sh      2>&1 | tee -a $triceFolder/testAll.log

./trice_insertIDs_in_examples_and_test_folder.sh     2>&1 | tee -a $triceFolder/testAll.log
cd examples/L432_inst
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
echo "Translating all L432 configurations..."        2>&1 | tee -a $triceFolder/testAll.log
./all_configs_build.sh                               2>&1 | tee -a $triceFolder/testAll.log
echo "Translating all L432 configurations...done"    2>&1 | tee -a $triceFolder/testAll.log
echo "---"                                           2>&1 | tee -a $triceFolder/testAll.log
cd - >/dev/null
./trice_cleanIDs_in_examples_and_test_folder.sh      2>&1 | tee -a $triceFolder/testAll.log

t1=`date +%s`
if command -v caffeinate 2>&1 >/dev/null
then
    # https://stackoverflow.com/questions/30171050/start-a-process-in-background-do-a-task-then-kill-the-process-in-the-background
    kill %-                                          2>&1 | tee -a $triceFolder/testAll.log 
fi
runtime=$((t1-t0))
echo Script run $runtime seconds.                    2>&1 | tee -a $triceFolder/testAll.log
