#!/bin/bash
triceFolder=`pwd`
date                                                 | tee    $triceFolder/testAll.log
echo This can take a while ...                       | tee -a $triceFolder/testAll.log
t0=`date +%s`
if command -v caffeinate 2>&1 >/dev/null
then
    caffeinate & # keep mac alive
fi

./renewIDs_in_examples_and_refresh_test_folder.sh    | tee -a $triceFolder/testAll.log
echo "---"                                           | tee -a $triceFolder/testAll.log
echo "Testing the Go code..."                        | tee -a $triceFolder/testAll.log
go clean -cache -testcache                           | tee -a $triceFolder/testAll.log
go test ./...                                        | tee -a $triceFolder/testAll.log
echo "Testing the Go code...done"                    | tee -a $triceFolder/testAll.log
echo "---"                                           | tee -a $triceFolder/testAll.log

./trice_insertIDs_in_examples_and_test_folder.sh     | tee -a $triceFolder/testAll.log
cd _test
echo "---"                                           | tee -a $triceFolder/testAll.log
echo "Testing the Target code..."                    | tee -a $triceFolder/testAll.log
go test ./...                                        | tee -a $triceFolder/testAll.log
cd - >/dev/null
./trice_cleanIDs_in_examples_and_test_folder.sh      | tee -a $triceFolder/testAll.log
echo "Testing the Target code...done"                | tee -a $triceFolder/testAll.log
echo "---"                                           | tee -a $triceFolder/testAll.log

./trice_insertIDs_in_examples_and_test_folder.sh     | tee -a $triceFolder/testAll.log
cd examples
./cleanAllTargets.sh                                 | tee -a $triceFolder/testAll.log
echo "---"                                           | tee -a $triceFolder/testAll.log
echo "Translating all examples with TRICE_OFF..."    | tee -a $triceFolder/testAll.log
./buildAllTargets_TRICE_OFF.sh                       | tee -a $triceFolder/testAll.log
echo "Translating all examples with TRICE_OFF...done"| tee -a $triceFolder/testAll.log
echo "---"                                           | tee -a $triceFolder/testAll.log
./cleanAllTargets.sh                                 | tee -a $triceFolder/testAll.log
echo "---"                                           | tee -a $triceFolder/testAll.log
echo "Translating all examples with TRICE_ON..."     | tee -a $triceFolder/testAll.log
./buildAllTargets_TRICE_ON.sh                        | tee -a $triceFolder/testAll.log
echo "Translating all examples with TRICE_ON...done" | tee -a $triceFolder/testAll.log
echo "---"                                           | tee -a $triceFolder/testAll.log
./cleanAllTargets.sh                                 | tee -a $triceFolder/testAll.log
cd - >/dev/null
./trice_cleanIDs_in_examples_and_test_folder.sh      | tee -a $triceFolder/testAll.log

./trice_insertIDs_in_examples_and_test_folder.sh     | tee -a $triceFolder/testAll.log
cd examples/L432_inst
echo "---"                                           | tee -a $triceFolder/testAll.log
echo "Translating all L432 configurations..."        | tee -a $triceFolder/testAll.log
./all_configs_build.sh                               | tee -a $triceFolder/testAll.log
echo "Translating all L432 configurations...done"    | tee -a $triceFolder/testAll.log
echo "---"                                           | tee -a $triceFolder/testAll.log
cd - >/dev/null
./trice_cleanIDs_in_examples_and_test_folder.sh      | tee -a $triceFolder/testAll.log

t1=`date +%s`
if command -v caffeinate 2>&1 >/dev/null
then
    # https://stackoverflow.com/questions/30171050/start-a-process-in-background-do-a-task-then-kill-the-process-in-the-background
    kill %-                                          | tee -a $triceFolder/testAll.log 
fi
runtime=$((t1-t0))
echo Script run $runtime seconds.                    | tee -a $triceFolder/testAll.log
