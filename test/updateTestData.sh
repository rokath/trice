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
staticBuffer_cobs
staticBuffer_tcobs
staticBuffer_nopf

staticBuffer_cobs_short
staticBuffer_tcobs_short
staticBuffer_nopf_short

stackBuffer_tcobs_short
stackBuffer_cobs_short
stackBuffer_nopf_short
stackBuffer_tcobs
stackBuffer_cobs

ringBuffer_deferred_tcobs_short
ringBuffer_deferred_cobs_short
ringBuffer_deferred_tcobs
ringBuffer_deferred_cobs

doubleBuffer_deferred_tcobs_short
doubleBuffer_deferred_cobs_short
doubleBuffer_deferred_tcobs
doubleBuffer_deferred_cobs

_doubleBuffer_direct_noRouting_nopf
"
for d in $CGOTESTDIRS
do
cp ./testdata/cgoPackage.go ./$d/generated_cgoPackage.go
done

go clean -cache

# give til.json to target project for ID add ons
cp ./testdata/til.json MDK-ARM_STM32F030R8_instrumented/til.json
cp ./testdata/li.json  MDK-ARM_STM32F030R8_instrumented/li.json