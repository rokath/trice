#!/bin/bash
# Advanced build check: ./all_configs_build.sh compiles configurations 0 through 100.
# Requires Bash, make, Trice and the Arm GNU toolchain; no board is needed.
# For a single configuration, use ./build.sh CONFIGURATION=number instead.
# TRICE_L432_TEST_JOBS=2 ./all_configs_build.sh limits concurrent builds to two.
# Each job gets its own output directory and log so parallel builds cannot clash.
# The helpers collect results and stop only this script's jobs on Ctrl-C.
# Build the complete matrix with one preparation and a bounded compiler budget.
set -euo pipefail

L432_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
L432_ROOT="$(cd -- "$L432_DIR/../.." && pwd)"

# Each invocation starts with new objects. This deliberately avoids reusing
# objects compiled with another configuration, workflow, header or toolchain.
# Code-generation options match build.sh CONFIGURATION=N. Only expensive text
# listings are disabled; compiler diagnostics and all binary targets remain.
build_l432_configuration() {
  local configuration="$1" directory="$2" artifact
  mkdir -p "$directory"
  printf '+ make -j1 GCC_LISTINGS=0 GCC_BUILD=%s TRICE_FLAGS=-DCONFIGURATION=%s gcc\n' "$directory" "$configuration"
  make -j1 GCC_LISTINGS=0 "GCC_BUILD=$directory" "TRICE_FLAGS=-DCONFIGURATION=$configuration " gcc || return $?
  for artifact in L432KC.elf L432KC.hex L432KC.bin; do
    if [ ! -s "$directory/$artifact" ]; then
      printf 'error: missing or empty L432 artifact: %s/%s\n' "$directory" "$artifact" >&2
      return 1
    fi
  done
}

# Report in configuration order and finish every started child before the
# surrounding managed workflow can restore shared source files and sidecars.
finish_l432_batch() {
  local index configuration logfile directory status
  for ((index = 0; index < ${#L432_PIDS[@]}; index++)); do
    configuration="${L432_CONFIGURATIONS[index]}"
    logfile="$L432_RUN_DIR/config-$configuration.log"
    directory="$L432_RUN_DIR/config-$configuration"
    status=0
    wait "${L432_PIDS[index]}" || status=$?
    L432_FINISHED=$((L432_FINISHED + 1))
    printf '\n--- CONFIGURATION=%s ---\n' "$configuration"
    if [ "$status" -eq 0 ]; then
      printf 'L432 PASS CONFIGURATION=%s; log: %s\n' "$configuration" "$logfile"
      # Keep warnings visible as before, and retain the complete compiler log.
      if grep -Eiq 'warning:' "$logfile"; then
        L432_WARNINGS=1
        grep -Ei 'warning:' "$logfile" || true
      fi
      # These fresh, private outputs cannot accelerate the next invocation.
      # Remove successful objects/listings to avoid accumulating gigabytes.
      rm -rf -- "$directory"
    else
      L432_FAILED=1
      printf 'L432 FAIL CONFIGURATION=%s (exit %s); log: %s\n' "$configuration" "$status" "$logfile"
      printf 'Reproduce after the same ID preparation: cd examples/L432_inst && ./build.sh CONFIGURATION=%s\n' "$configuration"
      awk '/error:|undefined reference|Error / { if (!shown) remaining=22; shown=1 } remaining>0 { print; remaining-- }' "$logfile"
      printf 'Last output (partial build retained in %s):\n' "$directory"
      tail -n 12 "$logfile"
    fi
  done
  L432_PIDS=() L432_CONFIGURATIONS=()
}

# Signal process groups, not just make: compilers must stop before restoration.
# Bash job control provides these groups on macOS, Linux and Git Bash.
cancel_l432_jobs() {
  local status="$1" pid attempts
  trap - EXIT
  trap '' INT TERM
  for pid in "${L432_PIDS[@]}"; do
    kill -TERM -- "-$pid" 2>/dev/null || true
  done
  for pid in "${L432_PIDS[@]}"; do wait "$pid" 2>/dev/null || true; done
  for pid in "${L432_PIDS[@]}"; do
    attempts=0
    while kill -0 -- "-$pid" 2>/dev/null; do
      if [ "$attempts" -ge 50 ]; then
        kill -KILL -- "-$pid" 2>/dev/null || true
        break
      fi
      sleep 0.1
      attempts=$((attempts + 1))
    done
  done
  printf 'L432 matrix cancelled; logs and partial outputs: %s\n' "$L432_RUN_DIR"
  exit "$status"
}

# Drain running children on unexpected orchestration errors as well as signals.
finish_l432_on_exit() {
  local status="$1"
  if [ "${#L432_PIDS[@]}" -gt 0 ]; then cancel_l432_jobs "$status"; fi
}

# Reuse the bounded budget detected by the shared Windows setup (which prefers
# physical cores there). Linux/macOS setup uses bare -j, so query online CPUs.
# A missing or malformed probe falls back to four rather than unlimited jobs.
default_l432_jobs() {
  local detected="${MAKE_JOBS:--j}"
  detected="${detected#-j}"
  case "$detected" in
    '' | *[!0-9]* | 0 | 0*) detected="$(getconf _NPROCESSORS_ONLN 2>/dev/null || true)" ;;
  esac
  case "$detected" in
    '' | *[!0-9]* | 0 | 0*) detected=4 ;;
  esac
  printf '%s\n' "$detected"
}

main() {
  local configuration index
  L432_JOBS="${TRICE_L432_TEST_JOBS:-}"
  L432_NO_STOP="${TRICE_TEST_NO_STOP:-0}"
  case "$L432_JOBS" in
    '') ;; # Resolve the automatic budget after the shared environment setup.
    *[!0-9]* | 0 | 0*)
      printf 'Invalid TRICE_L432_TEST_JOBS: %s (positive integer required)\n' "$L432_JOBS" >&2
      return 2
      ;;
  esac
  case "$L432_NO_STOP" in
    0 | 1) ;;
    *)
      printf 'Invalid TRICE_TEST_NO_STOP: %s (0 or 1 required)\n' "$L432_NO_STOP" >&2
      return 2
      ;;
  esac
  cd "$L432_DIR"

  # The managed test owner has already prepared Bind and owns restoration.
  # Direct invocation uses the same preparation as the standalone build.
  # shellcheck source=../prepareTriceBind.sh
  source "$L432_ROOT/examples/prepareTriceBind.sh"
  prepare_trice_bind_build "$L432_ROOT"
  # shellcheck source=../../scripts/_150_setup_build_environment.sh
  source "$L432_ROOT/scripts/_150_setup_build_environment.sh"
  if [ -z "$L432_JOBS" ]; then L432_JOBS="$(default_l432_jobs)"; fi

  # Use relative paths for native Windows Make/GCC as well as POSIX tools.
  mkdir -p ../../temp/log
  L432_RUN_DIR="$(mktemp -d ../../temp/log/l432.XXXXXX)"
  L432_PIDS=() L432_CONFIGURATIONS=()
  L432_FAILED=0 L432_FINISHED=0 L432_WARNINGS=0
  trap 'cancel_l432_jobs 130' INT
  trap 'cancel_l432_jobs 143' TERM
  # An orchestration error must also drain children before the owner restores.
  trap 'finish_l432_on_exit $?' EXIT
  set -m
  date
  printf 'L432 matrix: configurations=101 jobs=%s make-jobs=1 logs=%s\n' "$L432_JOBS" "$L432_RUN_DIR"
  # Ignore an inherited unlimited MAKEFLAGS jobserver budget. The outer limit
  # bounds the total number of concurrent compile/link commands across make jobs.
  for ((configuration = 0; configuration <= 100; configuration++)); do
    index="${#L432_PIDS[@]}"
    build_l432_configuration "$configuration" "$L432_RUN_DIR/config-$configuration" >"$L432_RUN_DIR/config-$configuration.log" 2>&1 &
    L432_PIDS[index]=$!
    L432_CONFIGURATIONS[index]=$configuration
    if [ "${#L432_PIDS[@]}" -ge "$L432_JOBS" ]; then
      finish_l432_batch
      if [ "$L432_FAILED" -ne 0 ] && [ "$L432_NO_STOP" -eq 0 ]; then break; fi
    fi
  done
  finish_l432_batch
  if [ "$L432_WARNINGS" -ne 0 ]; then
    printf 'Hint: L432 configuration builds completed with compiler warnings; see the diagnostics above.\n'
  fi
  printf 'L432 matrix finished: %s/101 configurations; failed=%s; duration=%ss\n' "$L432_FINISHED" "$L432_FAILED" "$SECONDS"
  return "$L432_FAILED"
}

main "$@"
