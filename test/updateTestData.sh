#!/usr/bin/env bash

cp ./testdata/triceCheck.c.txt ./testdata/triceCheck.c
cp ./testdata/triceCheck.c.txt ../examples/exampleData/triceCheck.c

# create til.json && li.json if not existing
touch ./testdata/til.json ./testdata/li.json

#  insert IDs into source code
trice insert -i ./testdata/til.json -li ./testdata/li.json -liPathIsRelative -IDMin 1000 -IDMax 7999

# The file cgoPackage.go is the same in all cgo test packages, but must be inside the folders.
# os agnostic links would be better.
CGOTESTDIRS="
doubleBuffer_cobs/
doubleBuffer_cobs_AND_rtt8_nopf/
doubleBuffer_cobs_AND_rtt32_nopf/
doubleBuffer_tcobs/
doubleBuffer_tobs_AND_rtt8_nopf/
doubleBuffer_tcobs_AND_rtt32_nopf/
doubleBuffer_xtea_cobs/
doubleBuffer_multi_cobs/
doubleBuffer_multi_cobs_AND_rtt8_nopf/
doubleBuffer_multi_cobs_AND_rtt32_nopf/
doubleBuffer_multi_tcobs/
doubleBuffer_multi_tobs_AND_rtt8_nopf/
doubleBuffer_multi_tcobs_AND_rtt32_nopf/
doubleBuffer_multi_xtea_cobs/
ringBuffer_cobs/
ringBuffer_tcobs/
ringBuffer_tcobs_AND_rtt8_nopf/
ringBuffer_tcobs_AND_rtt32_nopf/
ringBuffer_xtea_cobs/
ringBuffer_xtea_tcobs/
stackBuffer_aux8_nopf/
stackBuffer_rtt32_nopf/
stackBuffer_rtt8_nopf/
staticBuffer_aux8_nopf/
staticBuffer_rtt32_nopf/
staticBuffer_rtt8_nopf/
"
for d in $CGOTESTDIRS
do
cp ./testdata/cgoPackage.go ./$d/generated_cgoPackage.go
done

go clean -cache
