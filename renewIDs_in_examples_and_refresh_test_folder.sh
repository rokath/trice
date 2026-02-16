#!/bin/bash

# file name: renewIDs_in_examples_and_refresh_test_folder.sh

source ./trice_environment.sh
TD="./_test/testdata"

rm -f demoTIL.json demoLI.json                              # forget history (users usually should not do that in their projects, deleted here to avoid potential ID conflict messages)
touch demoTIL.json demoLI.json                              # new life
trice clean $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES # wipe out all IDs from the sources
rm -f demoTIL.json demoLI.json                              # forget history (in case the sources contained IDs, these are now removed from there, but are kept in the *.json files, so delete them again.)
touch demoTIL.json demoLI.json                              # new life

# Next steps are done separately to get the same IDs continuously, in case we deleted the history - normally all files and folders can be done parallel in one shot.
# We do not use -cache here to force the li.json generation.
# The Trice tool per default chooses IDs randomly between 1000 and 7999.
# On changing the test and example projects adapt $TRICE_PRJ_FILES too.
trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMax 16383 -IDMethod downward -src ./examples/exampleData/triceLogDiagData.c
trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMax 16383 -IDMethod downward -src ./examples/exampleData/triceExamples.c
trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMin 13000 -IDMax 15999 -IDMethod upward -src ./examples/F030_inst/Core
trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMin 13000 -IDMax 15999 -IDMethod upward -src ./examples/G0B1_inst/Core
trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMin 13000 -IDMax 15999 -IDMethod upward -src ./examples/L432_inst/Core
trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMin 13000 -IDMax 15999 -IDMethod upward -src $TD/triceCheck.c
trice insert $TRICE_DEFAULTS $TRICE_ALIASES -IDMin 13000 -IDMax 15999 -IDMethod upward -src $TD/..
trice clean $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES # IDs are now inside the til.json (and li.json) files.

# The file cgoPackage.go is the same in all cgo test packages, but must be inside the folders.
# os agnostic links would be better.

# To update $CGOTESTDIRS do inside _test:
#    ls -l _test | grep -v testdata | grep -v -i ReadMe | cut -c 40- >> ./renewIDs_in_examples_and_refresh_test_folder.sh
#    "/c/Program Files/Notepad++/notepad++.exe" ../renewIDs_in_examples_and_refresh_test_folder.sh
#    move generated lines and remove trash
CGOTESTDIRS="
 alias_dblB_de_tcobs_ua/
 aliasassert_dblB_de_tcobs_ua/
 be_dblB_de_tcobs_ua/
 be_staticB_di_xtea_cobs_rtt32/
 dblB_de_protect_tcobs_ua/
 dblB_de_multi_cobs_ua/
 dblB_de_multi_nopf_ua/
 dblB_de_multi_tcobs_ua/
 dblB_de_multi_xtea_cobs_ua/
 dblB_de_multi_xtea_tcobs_ua/
 dblB_de_single_cobs_ua/
 dblB_de_single_nopf_ua/
 dblB_de_single_tcobs_ua/
 _dblB_de_single_xtea_cobs_ua/
 _dblB_de_single_xtea_tcobs_ua/
 dblB_di_cobs_rtt32__de_xtea_cobs_ua/
 dblB_di_nopf_rtt32__de_cobs_ua/
 dblB_di_nopf_rtt32__de_multi_cobs_ua/
 dblB_di_nopf_rtt32__de_multi_tcobs_ua/
 dblB_di_nopf_rtt32__de_tcobs_ua/
 dblB_di_nopf_rtt32__de_xtea_cobs_ua/
 dblB_di_nopf_rtt8__de_cobs_ua/
 dblB_di_nopf_rtt8__de_multi_cobs_ua/
 dblB_di_nopf_rtt8__de_multi_tcobs_ua/
 dblB_di_nopf_rtt8__de_tcobs_ua/
 modify_for_debug/
 ringB_de_protect_tcobs_ua/
 ringB_de_multi_cobs_ua/
 _ringB_de_multi_nopf_ua/
 ringB_de_multi_tcobs_ua/
 ringB_de_multi_xtea_cobs_ua/
 ringB_de_multi_xtea_tcobs_ua/
 ringB_de_single_cobs_ua/
 ringB_de_single_nopf_ua/
 ringB_de_single_tcobs_ua/
 ringB_de_single_xtea_cobs_ua/
 ringB_de_single_xtea_tcobs_ua/
 ringB_di_cobs_rtt32__de_tcobs_ua/
 ringB_di_cobs_rtt32__de_xtea_cobs_ua/
 ringB_di_cobs_rtt8__de_tcobs_ua/
 ringB_di_nopf_rtt32__de_tcobs_ua/
 ringB_di_nopf_rtt32__de_xtea_cobs_ua/
 ringB_di_nopf_rtt8__de_tcobs_ua/
 ringB_di_tcobs_rtt32__de_tcobs_ua/
 ringB_di_xtea_cobs_rtt32__de_cobs_ua/
 ringB_di_xtea_cobs_rtt32__de_tcobs_ua/
 ringB_di_xtea_cobs_rtt32__de_xtea_cobs_ua/
 stackB_di_nopf_aux32_specific/
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
_userprint_dblB_de_tcobs_ua/
 "

# Refresh test folders.
for d in $CGOTESTDIRS; do
  cp $TD/cgoPackage.go $TD/../$d/generated_cgoPackage.go
  cp $TD/cgoPackage_test.go $TD/../$d/generated_cgoPackage_test.go
done

# Remove legacy build artifacts if existent.
cd ./examples || exit 1
./cleanAllTargets.sh
cd - >/dev/null || exit 1

####################
