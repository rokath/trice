#!/usr/bin/env bash

cp ./triceCheck_EditThisFile_NotTheTriceCheckDotC.txt triceCheck.c
touch til.json
trice u -src triceCheck.c
rm *.json*
cp cgot_EditThisFile.go ../cgo_stackBuffer_noCycle_cobs/cgot_DoNotEdit_EditCgotDotGoInTestdata.go
