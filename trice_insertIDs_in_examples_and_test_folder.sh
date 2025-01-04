#!/usr/bin/env bash

TD="./_test/testdata"
LIP="-liPath relative"

trice insert $LIP -i $TD/til.json -li $TD/li.json -IDMin 13000 -IDMax 16383 -src ./_test -src ./examples/F030_inst -src ./examples/G0B1_inst -src ./examples/L432_inst
