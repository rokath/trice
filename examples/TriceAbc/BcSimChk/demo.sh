#!/usr/bin/env bash
set -euo pipefail

# demo.sh
#
# Build the BcSimChk example and start four instances of the same binary. They all
# share bc.bus and bc.log. Each process writes random bytes and receives the
# bytes written by the other processes. Its own bytes are filtered in
# bcSimRead() by remembered bus-file offset ranges.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

./build.sh

EXE="./build/bcSimDevice"
case "$(uname -s 2>/dev/null || echo unknown)" in
  MINGW*|MSYS*|CYGWIN*) EXE="./build/bcSimDevice.exe" ;;
esac

rm -f bc.bus bc.log
rm -rf bc.bus.lock

echo "Starting four instances of the same binary."
echo "Each instance writes random bytes to bc.bus and receives bytes written by the others."
echo

"$EXE" -name A -auto 4 -interval 180 & pid_a=$!
"$EXE" -name B -auto 4 -interval 220 & pid_b=$!
"$EXE" -name C -auto 4 -interval 260 & pid_c=$!
"$EXE" -name D -auto 4 -interval 300 & pid_d=$!

wait "$pid_a" "$pid_b" "$pid_c" "$pid_d"

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
