#!/usr/bin/env bash

cp ./testdata/triceCheck.c.txt ./testdata/generated_triceCheck.c
rm -f ./testdata/til.json
touch ./testdata/til.json
trice u -src ./testdata/generated_triceCheck.c -i ./testdata/til.json


# The file cgoPackage.go is the same in all cgo test packages, but must be inside the folders.
# os agnostic links would be better.
cp ./testdata/cgoPackage.go ./cgo_stackBuffer_noCycle_cobs/generated_cgoPackage.go
# cp ./testdata/EditThisFile_cgo.go ../cgo_stackBuffer_noCycle_tcobs/generated_cgo.go

go clean -cache
