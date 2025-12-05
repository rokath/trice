#!/usr/bin/env bash

failCount=0

for d in ./F030_bare/ ./G0B1_bare/ ./L432_bare/ ./F030_inst/ ./G0B1_inst/ ./L432_inst/; do
	cd $d
	echo --------------------------------------------------------------------------------------------------------
	echo $d with TRICE_OFF=0

	./build.sh

	if ! [ $? -eq 0 ]; then
		failCount=$((failCount + 1))
		echo FAIL: $d
	fi
	cd ..
done

if ! [ $failCount -eq 0 ]; then
	echo $failCount times FAIL
fi
