#!/bin/bash
date
echo This can take a while ...
t0=`date +%s`
if command -v caffeinate 2>&1 >/dev/null
then
    caffeinate & # keep mac alive
fi

./renewIDs_in_examples_and_refresh_test_folder.sh
go clean -cache -testcache
go test ./...

./trice_insertIDs_in_examples_and_test_folder.sh
cd _test
go test ./...
cd - >/dev/null
./trice_cleanIDs_in_examples_and_test_folder.sh

./trice_insertIDs_in_examples_and_test_folder.sh
cd examples
./cleanAllTargets.sh
echo "Translating all examples with TRICE_OFF..."
./buildAllTargets_TRICE_OFF.sh
echo "Translating all examples with TRICE_OFF...done"
./cleanAllTargets.sh
echo "Translating all examples with TRICE_ON..."
./buildAllTargets_TRICE_ON.sh
echo "Translating all examples with TRICE_ON...done"
./cleanAllTargets.sh
cd - >/dev/null
./trice_cleanIDs_in_examples_and_test_folder.sh

./trice_insertIDs_in_examples_and_test_folder.sh
cd examples/L432_inst
echo "Translating all L432 configurations..."
./all_configs_build.sh
echo "Translating all L432 configurations...done"
cd - >/dev/null
./trice_cleanIDs_in_examples_and_test_folder.sh

t1=`date +%s`
if command -v caffeinate 2>&1 >/dev/null
then
    kill %- # https://stackoverflow.com/questions/30171050/start-a-process-in-background-do-a-task-then-kill-the-process-in-the-background
fi
runtime=$((t1-t0))
echo Script run $runtime seconds.
