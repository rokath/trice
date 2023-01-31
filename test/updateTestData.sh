#!/usr/bin/env bash

cp ./testdata/EditThisFile_triceCheck.c.txt ./testdata/generated_triceCheck.c
rm -f ./testdata/generated_til.json
touch ./testdata/generated_til.json
trice u -src ./testdata/generated_triceCheck.c -i ./testdata/generated_til.json

cp ./testdata/EditThisFile_cgo.go ./cgo_stackBuffer_noCycle_cobs/generated_cgo.go
# cp ./testdata/EditThisFile_cgo.go ../cgo_stackBuffer_noCycle_tcobs/generated_cgo.go

go clean -cache
