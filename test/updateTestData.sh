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
ERR_ringB_di_xtea_cobs_rtt32__de_tcobs_ua/
dblB_de_cobs_ua/
dblB_de_multi_cobs_ua/
dblB_de_multi_nopf_ua/
dblB_de_multi_tcobs_ua/
dblB_de_multi_xtea_cobs_ua/
dblB_de_multi_xtea_tcobs_ua/
dblB_de_nopf_ua/
dblB_de_tcobs_ua/
dblB_de_xtea_cobs_ua/
dblB_de_xtea_tcobs_ua/
dblB_di_nopf_rtt32__de_cobs_ua/
dblB_di_nopf_rtt32__de_multi_cobs_ua/
dblB_di_nopf_rtt32__de_multi_tcobs_ua/
dblB_di_nopf_rtt32__de_tcobs_ua/
dblB_di_nopf_rtt8__de_cobs_ua/
dblB_di_nopf_rtt8__de_multi_cobs_ua/
dblB_di_nopf_rtt8__de_multi_tcobs_ua/
dblB_di_nopf_rtt8__de_tcobs_ua/
ringB_de_cobs_ua/
ringB_de_nopf_ua/
ringB_de_tcobs_ua/
ringB_de_xtea_cobs_ua/
ringB_de_xtea_tcobs_ua/
ringB_di_cobs_rtt32__de_tcobs_ua/
ringB_di_cobs_rtt8__de_tcobs_ua/
ringB_di_nopf_rtt32__de_tcobs_ua/
ringB_di_nopf_rtt32__de_xtea_cobs_ua/
ringB_di_nopf_rtt8__de_tcobs_ua/
ringB_di_tcobs_rtt32__de_tcobs_ua/
stackB_di_nopf_aux8/
stackB_di_nopf_aux32/
stackB_di_nopf_rtt32/
stackB_di_nopf_rtt8/
staticB_di_nopf_aux8/
staticB_di_nopf_aux32/
staticB_di_nopf_rtt32/
staticB_di_nopf_rtt8/
staticB_di_tcobs_rtt32/
staticB_di_tcobs_rtt8/
stackB_di_xtea_cobs_rtt8/
staticB_di_xtea_cobs_rtt32/
ringB_di_xtea_cobs_rtt32__de_xtea_cobs_ua/
"
for d in $CGOTESTDIRS
do
cp ./testdata/cgoPackage.go ./$d/generated_cgoPackage.go
done

go clean -cache
