#!/usr/bin/env bash
# Usage: ./demo.sh. Build and run four native PC devices on a simulated bus.
# Requires the C compiler used by build.sh; no embedded hardware is needed.
# The demo replaces old bc.bus/bc.log files, runs the devices concurrently,
# waits for them to finish, and prints the text log and raw bus bytes.
set -euo pipefail

# demo.sh
#
# Build the BcSimChk example and start four instances of the same binary. They
# all share bc.bus and bc.log. Each process writes random bytes and receives the
# bytes written by the other processes. Its own bytes are filtered in
# bcSimRead() by remembered bus-file offset ranges.

# Run relative to this script so the generated bc.bus and bc.log files stay in
# BcSimChk/ and do not pollute the repository root.
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

./build.sh

# Select the executable path. Windows-like shells usually build an .exe suffix.
EXE="./build/bcSimDevice"
case "$(uname -s 2>/dev/null || echo unknown)" in
  MINGW* | MSYS* | CYGWIN*) EXE="./build/bcSimDevice.exe" ;;
esac

# Start from a clean demonstration bus. The lock directory is removed as a
# convenience in case a previous demo was interrupted.
rm -f bc.bus bc.log
rm -rf bc.bus.lock

# Track only this demo's children. Fixed slots also work with Bash 3.2 and set -u.
PARTICIPANTS=(A B C D)
PARTICIPANT_PIDS=("" "" "" "")
cleanup_demo() {
  local status="$1" pid
  # Ignore further signals while stopping and reaping every remaining child.
  trap '' INT TERM
  trap - EXIT
  for pid in "${PARTICIPANT_PIDS[@]}"; do
    if [ -n "$pid" ]; then kill -TERM "$pid" 2>/dev/null || true; fi
  done
  for pid in "${PARTICIPANT_PIDS[@]}"; do
    if [ -n "$pid" ]; then wait "$pid" 2>/dev/null || true; fi
  done
  exit "$status"
}
trap 'cleanup_demo $?' EXIT
trap 'cleanup_demo 130' INT
trap 'cleanup_demo 143' TERM

echo "Starting four instances of the same binary."
echo "Each instance writes random bytes to bc.bus and receives bytes written by the others."
echo

# Launch the same binary four times with different participant names. The
# slightly different intervals make interleaving more visible in bc.log.
"$EXE" -name A -auto 4 -interval 180 &
PARTICIPANT_PIDS[0]=$!
"$EXE" -name B -auto 4 -interval 220 &
PARTICIPANT_PIDS[1]=$!
"$EXE" -name C -auto 4 -interval 260 &
PARTICIPANT_PIDS[2]=$!
"$EXE" -name D -auto 4 -interval 300 &
PARTICIPANT_PIDS[3]=$!

# A multi-PID wait returns only the last status; check each participant instead.
status=0
for index in 0 1 2 3; do
  if wait "${PARTICIPANT_PIDS[index]}"; then
    :
  else
    child_status=$?
    printf 'error: BcSim participant %s exited with status %s\n' "${PARTICIPANTS[index]}" "$child_status" >&2
    status=$child_status
  fi
  PARTICIPANT_PIDS[index]=""
done
if [ "$status" -ne 0 ]; then exit "$status"; fi

echo "--- bc.log ---"
cat bc.log

echo "--- bc.bus hex dump ---"
if command -v xxd >/dev/null 2>&1; then
  xxd -g1 bc.bus
else
  od -An -tx1 -v bc.bus
fi

echo
echo "Demo files:"
echo "  bc.bus  binary byte stream"
echo "  bc.log  human-readable TX/RX hex log"
