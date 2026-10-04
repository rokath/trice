#!/bin/sh
# SPDX-License-Identifier: MIT

# Expected records follow main.c and show_*.sh; see README.md before changing
# or disabling a check after editing the example.

set -eu
cd "$(dirname "$0")"

./build_and_run.sh
all_events=$(./show_json.sh)
warning_only=$(./show_json.sh -logLevel wrn)
warning_and_sensor=$(./show_json.sh -ulabel sensor:650 -logLevel wrn)
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
  *'"tag":"untagged"'*'"message":"A message without a tag\n"'*) ;;
  *)
    echo 'FAIL: the untagged event is missing' >&2
    exit 1
    ;;
esac
case "$all_events" in
  *'"tag":"RECEIVE"'*'"message":"41 00 ff "'*) ;;
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
  *'"tag":"WARNING"'*'"tag":"sensor"'*) ;;
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

echo 'PASS: PC feature tour decodes fields, stamps, CE, tags, and KV output'
