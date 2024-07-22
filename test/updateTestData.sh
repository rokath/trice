#!/usr/bin/env bash

# clean history
touch ./testdata/til.json ./testdata/li.json
trice clean -i ./testdata/til.json -li ./testdata/li.json
rm ./testdata/til.json ./testdata/li.json

#  insert IDs into source code - this should restore triceCheck.c.
touch ./testdata/til.json ./testdata/li.json
trice insert -i ./testdata/til.json -li ./testdata/li.json -IDMin 15000 -IDMax 16299 -IDMethod upward

# The file cgoPackage.go is the same in all cgo test packages, but must be inside the folders.
# os agnostic links would be better.
CGOTESTDIRS="
    _ERROR_dblB_di_cobs_rtt32__de_xtea_cobs_ua/
    _ERROR_ringB_di_cobs_rtt32__de_xtea_cobs_ua/
    _ERROR_ringB_di_xtea_cobs_rtt32__de_cobs_ua/
    _ERROR_ringB_di_xtea_cobs_rtt32__de_tcobs_ua/
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
    dblB_di_nopf_rtt32__de_xtea_cobs_ua/
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
    ringB_di_xtea_cobs_rtt32__de_xtea_cobs_ua/
    stackB_di_nopf_aux32/
    stackB_di_nopf_aux8/
    stackB_di_nopf_rtt32/
    stackB_di_nopf_rtt8/
    stackB_di_xtea_cobs_rtt8/
    staticB_di_nopf_aux32/
    staticB_di_nopf_aux8/
    staticB_di_nopf_rtt32/
    staticB_di_nopf_rtt8/
    staticB_di_tcobs_rtt32/
    staticB_di_tcobs_rtt8/
    staticB_di_xtea_cobs_rtt32/
"

for d in $CGOTESTDIRS
do
    cp ./testdata/cgoPackage.go ./$d/generated_cgoPackage.go
done

go clean -cache
