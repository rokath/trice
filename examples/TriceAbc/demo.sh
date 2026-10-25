#!/usr/bin/env bash
# Usage: ./demo.sh. Build and run the nine-node PC bus simulation.
# Requires the tools used by build.sh. abc.bus carries binary bus traffic;
# abc.log collects readable output. Existing demo bus/log files are removed.
# Each '&' starts a node in parallel; $! records its process ID.
# wait keeps the script running until all nine nodes finish.
set -euo pipefail

# demo.sh
#
# Build and run the nine TriceAbc demo nodes against one shared BcSim bus.
#
# All nodes join before traffic starts. Receivers keep polling until every
# transmitter has finished and every receiver has drained its requests/replies.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

CONSOLE_LOCK_DIR="abc.console.lock"
CONSOLE_LOCK_POLL_MS=10

console_sleep_ms() {
  local ms="$1"
  if command -v sleep >/dev/null 2>&1; then
    # Keep the decimal point literal: floating-point formatting follows the
    # user's locale, but sleep needs a portable seconds argument.
    sleep "$(printf '%d.%03d' "$((ms / 1000))" "$((ms % 1000))")"
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

# A private directory prevents stale readiness/completion files from releasing
# a later invocation. Native node runtimes use this opt-in controller protocol.
TRICE_ABC_START_DIR="$(mktemp -d build/demo-start.XXXXXX)"
export TRICE_ABC_START_DIR
NODES=(N4_rx N5_rx N6_rx N7_bi N8_bi N9_bi N3_bi N1_tx N2_tx)
NODE_PIDS=()

cleanup_demo() {
  local status="$1" pid
  trap '' INT TERM
  trap - EXIT
  # Only this script's children are stopped, including on a failed handshake.
  for pid in "${NODE_PIDS[@]}"; do
    [ -z "$pid" ] || kill -TERM "$pid" 2>/dev/null || true
  done
  for pid in "${NODE_PIDS[@]}"; do
    [ -z "$pid" ] || wait "$pid" 2>/dev/null || true
  done
  rm -rf -- "$TRICE_ABC_START_DIR"
  exit "$status"
}
trap 'cleanup_demo $?' EXIT
trap 'cleanup_demo 130' INT
trap 'cleanup_demo 143' TERM

# Readiness includes successful bus registration and receive-hook setup.
# Waiting on events rather than a fixed sleep also works on a busy host.
check_demo_nodes() {
  local index
  for ((index = 0; index < ${#NODES[@]}; index++)); do
    if [ -f "$TRICE_ABC_START_DIR/${NODES[index]}.failed" ]; then
      printf 'error: ABC node %s reported a lifecycle failure\n' "${NODES[index]}" >&2
      return 1
    fi
    if ! kill -0 "${NODE_PIDS[index]}" 2>/dev/null; then
      # Only the two TX-only processes may finish before the final drain.
      if [ ! -f "$TRICE_ABC_START_DIR/${NODES[index]}.done" ] ||
        { [ "$index" -lt 7 ] && [ ! -f "$TRICE_ABC_START_DIR/finish" ]; }; then
        printf 'error: ABC node %s exited before completing its lifecycle\n' "${NODES[index]}" >&2
        return 1
      fi
    fi
  done
}

wait_for_node_marker() {
  local index="$1" suffix="$2" attempts=0 marker
  marker="$TRICE_ABC_START_DIR/${NODES[index]}.$suffix"
  until [ -f "$marker" ]; do
    check_demo_nodes || return 1
    if ! kill -0 "${NODE_PIDS[index]}" 2>/dev/null || [ "$attempts" -ge 6000 ]; then
      # A sender can publish its marker and exit after the loop's first check.
      # Recheck before reporting failure; the .failed check below still applies.
      if [ -f "$marker" ]; then break; fi
      printf 'error: ABC node %s did not reach %s; marker: %s\n' "${NODES[index]}" "$suffix" "$marker" >&2
      ls -l "$TRICE_ABC_START_DIR" >&2 || true
      return 1
    fi
    console_sleep_ms 10
    attempts=$((attempts + 1))
  done
  if [ -f "$TRICE_ABC_START_DIR/${NODES[index]}.failed" ]; then
    printf 'error: ABC node %s reported a lifecycle failure\n' "${NODES[index]}" >&2
    return 1
  fi
}

console_line "start: receive-capable nodes"
for ((index = 0; index < ${#NODES[@]}; index++)); do
  if [ "$index" -eq 7 ]; then console_line "start: transmit-only nodes"; fi
  "./build/${NODES[index]}${EXE_SUFFIX}" &
  NODE_PIDS[index]=$!
done
for ((index = 0; index < ${#NODES[@]}; index++)); do wait_for_node_marker "$index" ready; done
touch "$TRICE_ABC_START_DIR/start"

# N1, N2 and N3 produce application traffic. N3 continues receiving while its
# completion marker lets the controller wait for the other transmitters.
for index in 7 8 6; do wait_for_node_marker "$index" "done"; done
touch "$TRICE_ABC_START_DIR/stop"
for ((index = 0; index < 7; index++)); do wait_for_node_marker "$index" drained; done
# All receivers have now processed requests and emitted their replies. A final
# poll after this marker consumes the remaining replies before any receiver exits.
touch "$TRICE_ABC_START_DIR/finish"
status=0
for ((index = 0; index < ${#NODE_PIDS[@]}; index++)); do
  wait "${NODE_PIDS[index]}" || status=$?
  NODE_PIDS[index]=""
done
for node in "${NODES[@]}"; do
  if [ -f "$TRICE_ABC_START_DIR/$node.failed" ]; then status=1; fi
done
[ "$status" -eq 0 ] || exit "$status"

console_line ""
console_line "See abc.log for the combined output of all nodes."
#cat abc.log
