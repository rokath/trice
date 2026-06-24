#!/usr/bin/env bash
set -euo pipefail

# demo.sh
#
# Build and run the nine TriceAbc demo nodes against one shared BcSim bus.
#
# Start order matters:
# - receive-capable nodes first, so they join the live stream before traffic starts
# - transmit-only nodes afterwards, so their first frames are not missed

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

CONSOLE_LOCK_DIR="abc.console.lock"
CONSOLE_LOCK_POLL_MS=10

console_sleep_ms() {
  local ms="$1"
  if command -v sleep >/dev/null 2>&1; then
    sleep "$(awk "BEGIN { printf \"%.3f\", ${ms} / 1000 }")"
    return
  fi
  sleep 1
}

console_lock() {
  while ! mkdir "${CONSOLE_LOCK_DIR}" 2>/dev/null; do
    console_sleep_ms "${CONSOLE_LOCK_POLL_MS}"
  done
}

console_unlock() {
  rmdir "${CONSOLE_LOCK_DIR}" 2>/dev/null || true
}

console_line() {
  local text="$1"
  # Block until the shared console lock is available.
  # An unlocked timeout fallback would reintroduce exactly the mixed lines this
  # demo is trying to prevent.
  console_lock
  printf '%s\n' "${text}"
  console_unlock
}

./build.sh

EXE_SUFFIX=""
case "$(uname -s 2>/dev/null || echo unknown)" in
  MINGW* | MSYS* | CYGWIN*) EXE_SUFFIX=".exe" ;;
esac

rm -f abc.bus abc.log
rm -rf abc.bus.lock
rm -rf abc.console.lock

console_line "start: receive-capable nodes"
./build/N4_rx${EXE_SUFFIX} &
pid_n4=$!
./build/N5_rx${EXE_SUFFIX} &
pid_n5=$!
./build/N6_rx${EXE_SUFFIX} &
pid_n6=$!
./build/N7_bi${EXE_SUFFIX} &
pid_n7=$!
./build/N8_bi${EXE_SUFFIX} &
pid_n8=$!
./build/N9_bi${EXE_SUFFIX} &
pid_n9=$!
./build/N3_bi${EXE_SUFFIX} &
pid_n3=$!

sleep 1

console_line "start: transmit-only nodes"
./build/N1_tx${EXE_SUFFIX} &
pid_n1=$!
./build/N2_tx${EXE_SUFFIX} &
pid_n2=$!

wait "${pid_n1}" "${pid_n2}" "${pid_n3}" "${pid_n4}" "${pid_n5}" "${pid_n6}" "${pid_n7}" "${pid_n8}" "${pid_n9}"

console_line ""
console_line "--- abc.log ---"
cat abc.log
