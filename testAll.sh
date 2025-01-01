#!/bin/sh
date

echo This can take several minutes ...

t0=`date +%s`

./renewIDs_in_examples_and_refresh_test_folder.sh
go clean -cache
go test ./...
cd _test
trice insert -til ./testdata/til.json -li ./testdata/li.json
go test ./...
trice clean  -til ./testdata/til.json -li ./testdata/li.json
cd - >/dev/null

t1=`date +%s`

runtime=$((t1-t0))

echo Script run $runtime seconds.