#!/usr/bin/env bash

cp ./testdata/triceCheck.c.txt ./testdata/triceCheck.c
rm -f ./testdata/til.json
touch ./testdata/til.json
trice u -src ./testdata/triceCheck.c -i ./testdata/til.json -IDMin 1000 -IDMax 7999
# rm -f ./li.json


# The file cgoPackage.go is the same in all cgo test packages, but must be inside the folders.
# os agnostic links would be better.
CGOTESTDIRS="
cgo_stackBuffer_noCycle_cobs
cgo_stackBuffer_noCycle_tcobs
"
for d in $CGOTESTDIRS
do
cp ./testdata/cgoPackage.go ./$d/generated_cgoPackage.go
done

go clean -cache
