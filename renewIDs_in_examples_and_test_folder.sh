#!/usr/bin/env bash

TD="./_test/testdata"

rm -f           $TD/til.json     $TD/li.json # forget history (users usually should not do that in their projects, we delete to avoid potential ID conflict messages)
touch           $TD/til.json     $TD/li.json # new life
trice clean  -i $TD/til.json -li $TD/li.json -src $TD -src ./examples # wipe out all IDs from the sources
rm -f           $TD/til.json     $TD/li.json # forget history (in case the sources contained IDs, these are now removed from there, but are kept in the *.json files, so we delete them again.)
touch           $TD/til.json     $TD/li.json # new life

# Next steps are done separately to get the same IDs continuously. We deleted the history - normally all files and folders can be done parallel in one shot.
trice insert -i $TD/til.json -li $TD/li.json              -IDMax 16383 -IDMethod downward -src ./examples/exampleData/triceLogDiagData.c
trice insert -i $TD/til.json -li $TD/li.json              -IDMax 16383 -IDMethod downward -src ./examples/exampleData/triceExamples.c
trice insert -i $TD/til.json -li $TD/li.json -IDMin 13500 -IDMax 16383 -IDMethod upward   -src $TD/triceCheck.c
trice insert -i $TD/til.json -li $TD/li.json -IDMin 13500 -IDMax 16383 -IDMethod upward   -src $TD/..
trice insert -i $TD/til.json -li $TD/li.json -IDMin 13500 -IDMax 16383 -IDMethod upward   -src ./examples/F030_inst/
trice insert -i $TD/til.json -li $TD/li.json -IDMin 13500 -IDMax 16383 -IDMethod upward   -src ./examples/G0B1_inst/
trice insert -i $TD/til.json -li $TD/li.json -IDMin 13500 -IDMax 16383 -IDMethod upward   -src ./examples/L432_inst/

DIRS="
./examples/F030_inst/
./examples/G0B1_inst/
./examples/L432_inst/
"

for d in $DIRS
do
    rm -f           $d/til.json     $d/li.json
    touch           $d/til.json     $d/li.json
    trice add    -i $d/til.json -li $d/li.json -src ./examples/exampleData -src $TD -src $d
done


# The file cgoPackage.go is the same in all cgo test packages, but must be inside the folders.
# os agnostic links would be better.
CGOTESTDIRS="
    _special_dblB_de_tcobs_ua/
    special_protect_dblB_de_tcobs_ua/
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
    _ringB_de_multi_cobs_ua/
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
    cp $TD/cgoPackage.go $TD/../$d/generated_cgoPackage.go
done

cd ./examples
./cleanAllTargets.sh
cd - > /dev/null

trice clean  -i $TD/til.json -li $TD/li.json  -src $TD/.. -src ./examples
