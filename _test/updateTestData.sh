#!/usr/bin/env bash

# clean history
rm -f ./testdata/til.json ./testdata/li.json ../examples/F030R8_inst/til.json ../examples/F030R8_inst/li.json ../examples/G0B1_inst/til.json ../examples/G0B1_inst/li.json ../examples/L432KC_gen_ad_toClang_ed_instr/til.json ../examples/L432KC_gen_ad_toClang_ed_instr/li.json
touch ./testdata/til.json ./testdata/li.json ../examples/F030R8_inst/til.json ../examples/F030R8_inst/li.json ../examples/G0B1_inst/til.json ../examples/G0B1_inst/li.json ../examples/L432KC_gen_ad_toClang_ed_instr/til.json ../examples/L432KC_gen_ad_toClang_ed_instr/li.json
trice clean -src ./ -src ../examples/exampleData -i ./testdata/til.json -li ./testdata/li.json

#  insert IDs into source code 
rm -f ./testdata/til.json ./testdata/li.json ../examples/F030R8_inst/til.json ../examples/F030R8_inst/li.json ../examples/G0B1_inst/til.json ../examples/G0B1_inst/li.json ../examples/L432KC_gen_ad_toClang_ed_instr/til.json ../examples/L432KC_gen_ad_toClang_ed_instr/li.json
touch ./testdata/til.json ./testdata/li.json ../examples/F030R8_inst/til.json ../examples/F030R8_inst/li.json ../examples/G0B1_inst/til.json ../examples/G0B1_inst/li.json ../examples/L432KC_gen_ad_toClang_ed_instr/til.json ../examples/L432KC_gen_ad_toClang_ed_instr/li.json
trice insert -src ./testdata/triceCheck.c -i ./testdata/til.json -li ./testdata/li.json -IDMin 14000 -IDMax 16199 -IDMethod upward   # This should restore triceCheck.c.
trice insert -src ../examples/exampleData -i ./testdata/til.json -li ./testdata/li.json              -IDMax 16383 -IDMethod downward # This should restore triceLogSiagData.c.
trice insert -src ./                      -i ./testdata/til.json -li ./testdata/li.json -IDMin 16200 -IDMax 16383 -IDMethod upward   # This schoud restore tests and extend til.json.

# The file cgoPackage.go is the same in all cgo test packages, but must be inside the folders.
# os agnostic links would be better.
CGOTESTDIRS="
    be_dblB_de_tcobs_ua/
    be_staticB_di_xtea_cobs_rtt32/
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
    ringB_de_multi_cobs_ua/
    ringB_de_multi_tcobs_ua/
    ringB_de_multi_xtea_cobs_ua/
    ringB_de_multi_xtea_tcobs_ua/
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
