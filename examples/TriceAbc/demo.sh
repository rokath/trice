#!/usr/bin/env bash
set -euo pipefail

# demo.sh
#
# Build and run the seven TriceAbc demo nodes against one shared BcSim bus.
#
# Start order matters:
# - receive-capable nodes first, so they join the live stream before traffic starts
# - transmit-only nodes afterwards, so their first frames are not missed

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

./build.sh

EXE_SUFFIX=""
case "$(uname -s 2>/dev/null || echo unknown)" in
  MINGW* | MSYS* | CYGWIN*) EXE_SUFFIX=".exe" ;;
esac

rm -f abc.bus abc.log
rm -rf abc.bus.lock

echo "start: receive-capable nodes"
./build/N4_rx${EXE_SUFFIX} &
pid_n4=$!
./build/N5_rx${EXE_SUFFIX} &
pid_n5=$!
./build/N6_rx${EXE_SUFFIX} &
pid_n6=$!
./build/N7_bi${EXE_SUFFIX} &
pid_n7=$!
./build/N3_bi${EXE_SUFFIX} &
pid_n3=$!

sleep 1

echo "start: transmit-only nodes"
./build/N1_tx${EXE_SUFFIX} &
pid_n1=$!
./build/N2_tx${EXE_SUFFIX} &
pid_n2=$!

wait "${pid_n1}" "${pid_n2}" "${pid_n3}" "${pid_n4}" "${pid_n5}" "${pid_n6}" "${pid_n7}"

echo
echo "--- abc.log ---"
cat abc.log
