#!/bin/sh
# SPDX-License-Identifier: MIT

set -eu
cd "$(dirname "$0")"

# Compilation verifies that the CE adapter can call the FreeRTOS task API at
# the selected site. The table checks prevent a successful but empty bind.
./demo_build.sh
grep -Fq 'task={task:%p}' til.json || {
  echo 'FAIL: the task-handle CE field is absent from til.json' >&2
  exit 1
}
grep -Fq 'sensor:Load {load_pct:%u}' til.json || {
  echo 'FAIL: the custom-tag field is absent from til.json' >&2
  exit 1
}
grep -Fq 'info:Worker {worker:%s}' til.json || {
  echo 'FAIL: the runtime string field is absent from til.json' >&2
  exit 1
}
grep -Fq 'info:Task phase {phase:%u}' til.json || {
  echo 'FAIL: the 16-bit stamp example is absent from til.json' >&2
  exit 1
}
grep -Fq 'rx:%02x ' til.json || {
  echo 'FAIL: the buffer example is absent from til.json' >&2
  exit 1
}
grep -Fq 'osThreadGetId()' generated/trice_main_c_*.h || {
  echo 'FAIL: the bound call does not read the current task' >&2
  exit 1
}

echo 'PASS: G0B1 feature tour builds with task CE, structured fields, and a runtime string'
