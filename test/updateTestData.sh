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
100_dblB_de_multi_nopf/
100_dblB_de_nopf/
100_ringB_de_nopf/
100_ringB_di_rtt32_xtea_cobs__de_tcobs/
100_ringB_di_rtt32_xtea_cobs__de_xtea_cobs/
100_stackB_di_rtt8_de_xtea_cobs/
100_staticB_di_rtt32_de_xtea_cobs/
dblB_de_cobs/
dblB_de_multi_cobs/
dblB_de_multi_tcobs/
dblB_de_multi_xtea_cobs/
dblB_de_multi_xtea_tcobs/
dblB_de_tcobs/
dblB_de_xtea_cobs/
dblB_de_xtea_tcobs/
dblB_di_rtt32_nopf__de_cobs/
dblB_di_rtt32_nopf__de_multi_cobs/
dblB_di_rtt32_nopf__de_multi_tcobs/
dblB_di_rtt32_nopf__de_tcobs/
dblB_di_rtt8_nopf__de_cobs/
dblB_di_rtt8_nopf__de_multi_cobs/
dblB_di_rtt8_nopf__de_multi_tcobs/
dblB_di_rtt8_nopf__de_tcobs/
ringB_de_cobs/
ringB_de_tcobs/
ringB_de_xtea_cobs/
ringB_de_xtea_tcobs/
ringB_di_aux8_nopf/
ringB_di_rtt32_cobs__de_tcobs/
ringB_di_rtt32_nopf__de_tcobs/
ringB_di_rtt32_nopf__de_xtea_cobs/
ringB_di_rtt32_tobs__de_tcobs/
ringB_di_rtt8_cobs__de_tcobs/
ringB_di_rtt8_nopf__de_tcobs/
stackB_di_aux8_nopf/
stackB_di_rtt32_nopf/
stackB_di_rtt8_nopf/
staticB_di_aux32_nopf/
staticB_di_aux8_nopf/
staticB_di_rtt32_nopf/
staticB_di_rtt32_tcobs/
staticB_di_rtt8_nopf/
staticB_di_rtt8_tcobs/
"
for d in $CGOTESTDIRS
do
cp ./testdata/cgoPackage.go ./$d/generated_cgoPackage.go
done

go clean -cache
