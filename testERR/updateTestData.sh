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
ringB_di_xtea_cobs_rtt32__de_tcobs_ua/
ringB_di_xtea_cobs_rtt32__de_xtea_cobs_ua/
stackB_di_nopf_aux32/
stackB_di_xtea_cobs_rtt8/
staticB_di_nopf_aux32/
staticB_di_xtea_cobs_rtt32/
"
for d in $CGOTESTDIRS
do
cp ./testdata/cgoPackage.go ./$d/generated_cgoPackage.go
done

go clean -cache
