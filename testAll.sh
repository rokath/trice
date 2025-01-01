#!/bin/sh
date
echo This can take several minutes ...
t0=`date +%s`
if command -v caffeinate 2>&1 >/dev/null
then
    caffeinate & # keep mac alive
fi

./renewIDs_in_examples_and_refresh_test_folder.sh
go clean -cache
go test ./...
cd _test
trice insert -til ./testdata/til.json -li ./testdata/li.json
go test ./...
trice clean  -til ./testdata/til.json -li ./testdata/li.json
cd - >/dev/null

t1=`date +%s`
if command -v caffeinate 2>&1 >/dev/null
then
    kill %- # https://stackoverflow.com/questions/30171050/start-a-process-in-background-do-a-task-then-kill-the-process-in-the-background
fi
runtime=$((t1-t0))
echo Script run $runtime seconds.
