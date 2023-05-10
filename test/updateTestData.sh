#!/usr/bin/env bash

cp ./testdata/triceCheck.c.txt ./testdata/triceCheck.c

# renew til.json, because triceCheck was renewed
rm ./testdata/til.json
touch ./testdata/til.json

#  add IDs
trice u -src ./testdata/triceCheck.c -i ./testdata/til.json -li ./testdata/li.json -IDMin 1000 -IDMax 7999

# The file cgoPackage.go is the same in all cgo test packages, but must be inside the folders.
# os agnostic links would be better.
CGOTESTDIRS="
cgo_staticBuffer_noCycle_cobs
cgo_staticBuffer_noCycle_tcobs
cgo_staticBuffer_noCycle_nopf

cgo_staticBuffer_noCycle_cobs_short
cgo_staticBuffer_noCycle_tcobs_short
cgo_staticBuffer_noCycle_nopf_short

cgo_stackBuffer_noCycle_tcobs_short
cgo_stackBuffer_noCycle_cobs_short
cgo_stackBuffer_noCycle_nopf_short

cgo_stackBuffer_noCycle_tcobs
cgo_stackBuffer_noCycle_cobs

cgo_ringBuffer_noCycle_tcobs_short
cgo_ringBuffer_noCycle_cobs_short

cgo_ringBuffer_noCycle_tcobs
cgo_ringBuffer_noCycle_cobs

cgo_doubleBuffer_noCycle_tcobs_short
cgo_doubleBuffer_noCycle_cobs_short

cgo_doubleBuffer_noCycle_tcobs
cgo_doubleBuffer_noCycle_cobs
"
for d in $CGOTESTDIRS
do
cp ./testdata/cgoPackage.go ./$d/generated_cgoPackage.go
done

go clean -cache

# give til.json to target project for ID add ons
cp ./testdata/til.json MDK-ARM_STM32F030R8_instrumented/til.json
cp ./testdata/li.json  MDK-ARM_STM32F030R8_instrumented/li.json