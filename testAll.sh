#!/bin/sh
date

echo This can take several minutes ...

function f(){
    ./renewIDs_in_examples_and_test_folder.sh
    go clean -cache
    go test ./...
    cd _test
    trice insert -til ./testdata/til.json -li ./testdata/li.json
    go test ./...
    trice clean  -til ./testdata/til.json -li ./testdata/li.json
    cd -
}

time f
