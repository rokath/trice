#!/usr/bin/env bash

TD="./_test/testdata"
LIP="-liPath relative"

trice clean $LIP -i $TD/til.json -li $TD/li.json -src ./_test -src ./examples/F030_inst -src ./examples/G0B1_inst -src ./examples/L432_inst
