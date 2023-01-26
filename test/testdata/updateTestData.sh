#!/usr/bin/env bash

cp ./triceCheck_EditThisFile_NotTheTriceCheckDotC.txt triceCheck.c
touch til.json
trice u -src triceCheck.c
rm *.json*
cp cgo_EditThisFile.go ../cgo_stackBuffer_noCycle_cobs/cgo_DoNotEdit_EditCgotDotGoInTestdata.go
# cp cgo_EditThisFile.go ../cgo_stackBuffer_noCycle_tcobs/cgo_DoNotEdit_EditCgotDotGoInTestdata.go
