#!/bin/bash

TD="./_test/testdata"
LIP="-liPath relative"

#rm -f          demoTIL.json     demoLI.json # forget history (users usually should not do that in their projects, delete to avoid potential ID conflict messages)
#touch          demoTIL.json     demoLI.json # new life
trice clean  -i demoTIL.json -li demoLI.json -src $TD -src ./examples -alias CUSTOM_PRINT -salias CUSTOM_ASSERT # wipe out all IDs from the sources
#rm -f          demoTIL.json     demoLI.json # forget history (in case the sources contained IDs, these are now removed from there, but are kept in the *.json files, so delete them again.)
#touch          demoTIL.json     demoLI.json # new life

# Next steps are done separately to get the same IDs continuously, in case we deleted the history - normally all files and folders can be done parallel in one shot.
# We do not use -cache here to force the li.json generation.
# The Trice tool per default chooses IDs randomly between 1000 and 7999.
trice insert -i demoTIL.json -li demoLI.json $LIP              -IDMax 16383 -IDMethod downward -src ./examples/exampleData/triceLogDiagData.c
trice insert -i demoTIL.json -li demoLI.json $LIP              -IDMax 16383 -IDMethod downward -src ./examples/exampleData/triceExamples.c
trice insert -i demoTIL.json -li demoLI.json $LIP -IDMin 13000 -IDMax 15999 -IDMethod upward   -src ./examples/F030_inst/Core
trice insert -i demoTIL.json -li demoLI.json $LIP -IDMin 13000 -IDMax 15999 -IDMethod upward   -src ./examples/G0B1_inst/Core -alias CUSTOM_PRINT -salias CUSTOM_ASSERT -exclude ./examples/G0B1_inst/Core/Inc/triceCustomAliases.h
trice insert -i demoTIL.json -li demoLI.json $LIP -IDMin 13000 -IDMax 15999 -IDMethod upward   -src ./examples/L432_inst/Core
trice insert -i demoTIL.json -li demoLI.json $LIP -IDMin 13000 -IDMax 15999 -IDMethod upward   -src $TD/triceCheck.c
trice insert -i demoTIL.json -li demoLI.json $LIP -IDMin 13000 -IDMax 15999 -IDMethod upward   -src $TD/..

# Remove all IDs from the sources. They are now inside the til.json (and li.json) files.
trice clean -i demoTIL.json -li demoLI.json $LIP 

# The file cgoPackage.go is the same in all cgo test packages, but must be inside the folders.
# os agnostic links would be better.
CGOTESTDIRS="
    _special_dblB_de_tcobs_ua/
    special_for_debug/
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

# Refresh test folders.
for d in $CGOTESTDIRS
do
    cp $TD/cgoPackage.go $TD/../$d/generated_cgoPackage.go
done

# Remove legacy build artifacts if existent.
cd ./examples
#./cleanAllTargets.sh
cd - > /dev/null
