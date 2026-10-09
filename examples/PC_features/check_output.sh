#!/bin/sh
# Optional self-check: ./check_output.sh builds, runs and decodes the PC tour.
# Requires the same tools as build_and_run.sh; no hardware is needed.
# $(...) captures each decoder's output for the checks below.
# In each case pattern, * allows unrelated text such as source locations;
# the quoted fragments are the values this example promises to demonstrate.
# After editing main.c or show_*.sh, review the corresponding expected record.
# A failed command or the first mismatched record stops this check.
# SPDX-License-Identifier: MIT

# Expected records follow main.c and show_*.sh; see README.md before changing
# or disabling a check after editing the example.

set -eu
cd "$(dirname "$0")"

./build_and_run.sh
all_events=$(./show_json.sh)
warning_only=$(./show_json.sh -loglevel 600)
warning_and_sensor=$(./show_json.sh -ulabel sensor:650 -logLevel wrn)
# Numeric pick/ban selects an exact weight; it is not a minimum threshold.
exact_sensor=$(./show_json.sh -ulabel sensor:200 -pick 200)
without_sensor=$(./show_json.sh -ulabel sensor:200 -ban 200)
kv_events=$(./show_kv.sh)

# Check values and metadata across the binary transport, binder, and decoder.
case "$all_events" in
  *'"message":"Device pump A\n"'*'"fields":{"device":"pump A"}'*) ;;
  *)
    echo 'FAIL: the runtime string field was not decoded' >&2
    exit 1
    ;;
esac
case "$all_events" in
  *'"message":"Phase 7\n"'*'"ts32":"0:00:00,100"'*'"fields":{"phase":7}'*) ;;
  *)
    echo 'FAIL: the Phase record lost its 32-bit stamp or field; see README.md' >&2
    exit 1
    ;;
esac
case "$all_events" in
  *'"message":"Humidity 55 percent\n"'*'"ts16":"7"'*'"fields":{"humidity_pct":55}'*) ;;
  *)
    echo 'FAIL: the sensor record lost its 16-bit stamp or field; see README.md' >&2
    exit 1
    ;;
esac
case "$all_events" in
  *'"ts32Delta":"0:00:00,025"'*'"fields":{"voltage_mv":3250,"cycle":11}'*) ;;
  *)
    echo 'FAIL: the second supply record lost its delta or CE field' >&2
    exit 1
    ;;
esac
case "$all_events" in
  *'"tag":"untagged","level":"INFO","message":"A message without a tag\n"'*) ;;
  *)
    echo 'FAIL: the untagged event is missing' >&2
    exit 1
    ;;
esac
case "$all_events" in
  *'"tag":"RECEIVE","level":"DEBUG","message":"41 00 ff "'*) ;;
  *)
    echo 'FAIL: the buffer record is missing' >&2
    exit 1
    ;;
esac
case "$warning_only" in
  *'"tag":"WARNING"'*) ;;
  *)
    echo 'FAIL: the Warning event did not pass the threshold' >&2
    exit 1
    ;;
esac
case "$warning_only" in
  *'"tag":"sensor"'* | *'"tag":"INFO"'* | *'"tag":"DEBUG"'*)
    echo 'FAIL: a lower-weight event passed the Warning threshold' >&2
    exit 1
    ;;
esac
case "$warning_and_sensor" in
  *'"tag":"WARNING","level":"WARNING"'*'"tag":"sensor","level":"WARNING"'*) ;;
  *)
    echo 'FAIL: overriding the sensor weight did not include both events' >&2
    exit 1
    ;;
esac
case "$kv_events" in
  *'field.voltage_mv=3300 field.cycle=7'*) ;;
  *)
    echo 'FAIL: KV output lost the structured or CE field' >&2
    exit 1
    ;;
esac

# The registered sensor weight is 450 (DEBUG); raising it to 650 above changes
# its level to WARNING while the sensor tag and message stay unchanged.
case "$kv_events" in
  *'tag=sensor level=DEBUG message="Humidity 55 percent\n"'*) ;;
  *)
    echo 'FAIL: the sensor category did not derive DEBUG from weight 450' >&2
    exit 1
    ;;
esac

# Only sensor has weight 200 in this capture; INFO and WARNING must stay out.
case "$exact_sensor" in
  *'"tag":"INFO"'* | *'"tag":"WARNING"'* | *'"tag":"untagged"'* | *'"tag":"RECEIVE"'* | *'"tag":"DEBUG"'*)
    echo 'FAIL: exact weight selection admitted a different weight' >&2
    exit 1
    ;;
  *'"tag":"sensor","level":"TRACE","message":"Humidity 55 percent\n"'*) ;;
  *)
    echo 'FAIL: exact weight selection lost the sensor event' >&2
    exit 1
    ;;
esac
case "$without_sensor" in
  *'"tag":"sensor"'*)
    echo 'FAIL: exact weight exclusion kept the sensor event' >&2
    exit 1
    ;;
  *'"tag":"INFO"'*'"tag":"WARNING"'*) ;;
  *)
    echo 'FAIL: exact weight exclusion lost other weights' >&2
    exit 1
    ;;
esac

echo 'PASS: PC feature tour decodes fields, stamps, CE, tags, and KV output'
