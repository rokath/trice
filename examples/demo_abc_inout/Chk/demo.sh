#!/usr/bin/env bash
set -euo pipefail

# demo.sh
#
# Build the Chk example and start four instances of the same binary. They all
# share abc.bus and abc.log. Each process writes random bytes and receives the
# bytes written by the other processes. Its own bytes are filtered in
# demoAbcRead() by remembered bus-file offset ranges.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

./build.sh

EXE="./build/demoAbcDevice"
case "$(uname -s 2>/dev/null || echo unknown)" in
  MINGW*|MSYS*|CYGWIN*) EXE="./build/demoAbcDevice.exe" ;;
esac

rm -f abc.bus abc.log
rm -rf abc.bus.lock

echo "Starting four instances of the same binary."
echo "Each instance writes random bytes to abc.bus and receives bytes written by the others."
echo

"$EXE" -name A -auto 4 -interval 180 & pid_a=$!
"$EXE" -name B -auto 4 -interval 220 & pid_b=$!
"$EXE" -name C -auto 4 -interval 260 & pid_c=$!
"$EXE" -name D -auto 4 -interval 300 & pid_d=$!

wait "$pid_a" "$pid_b" "$pid_c" "$pid_d"

echo "--- abc.log ---"
cat abc.log

echo "--- abc.bus hex dump ---"
if command -v xxd >/dev/null 2>&1; then
  xxd -g1 abc.bus
else
  od -An -tx1 -v abc.bus
fi

echo
echo "Demo files:"
echo "  abc.bus  binary byte stream"
echo "  abc.log  human-readable TX/RX hex log"
