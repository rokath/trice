#!/usr/bin/env bash
#
# Shared PC/CGO test worker. ID preparation and restoration are intentionally
# owned by the workflow-specific caller.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

# select_pc_mode keeps direct worker misuse deterministic while the public
# wrappers continue to provide the established quick/full interface.
select_pc_mode() {
  case "${1:-full}" in
    quick | full) printf '%s\n' "${1:-full}" ;;
    *)
      printf 'Unsupported PC target selection: %s\n' "${1:-}" >&2
      return 2
      ;;
  esac
}

# cleanup_pc_temp_files removes only the process-specific orchestration files
# created by this worker. The surrounding workflow owns source-state cleanup.
cleanup_pc_temp_files() {
  rm -f -- \
    "${PC_OVERLAY_FILE:-}" \
    "${PC_ALL_PACKAGES_FILE:-}"
}

# create_harness_overlay exposes external C input changes to Go's build cache.
# Stamped copies stay in the retained run directory; package sources and the
# canonical harness templates remain untouched. Go writes native overlay paths.
create_harness_overlay() {
  local overlay_file="$1"
  printf '+ prepare content-stamped CGO overlay (workflow=%s)\n' "${TRICE_ID_WORKFLOW:-current}"
  go run ../scripts/pc_cache_overlay.go -out "$overlay_file" -inputs "$PC_LOG_DIR/cache-inputs" -workflow "${TRICE_ID_WORKFLOW:-current}"
}

# prepare_pc_package_lists resolves all packages before testing. Resolution is
# infrastructure work and therefore always fails hard, even with --no-stop.
prepare_pc_package_lists() {
  local selected="$1"

  create_harness_overlay "$PC_OVERLAY_FILE" || return 1
  if [ "$selected" = "quick" ]; then
    printf '+ go list ./ringB_de_multi_cobs_ua ./abc_rx_host/... ./abc_tx_host/...\n'
    go list ./ringB_de_multi_cobs_ua ./abc_rx_host/... ./abc_tx_host/... >"$PC_ALL_PACKAGES_FILE" || return 1
  else
    printf '+ go list ./...\n'
    go list ./... >"$PC_ALL_PACKAGES_FILE" || return 1
  fi
  if [ ! -s "$PC_ALL_PACKAGES_FILE" ]; then
    printf 'FAIL: PC configuration list is empty\n' >&2
    return 1
  fi
}

# run_pc_package keeps all tests in the package, not just the logging test.
# Limit Go's build parallelism as the outer worker already bounds concurrency.
run_pc_package() {
  local mode="$1"
  local package="$2"
  local go_arguments=(test -v -p 1 -count=1 "-overlay=$PC_OVERLAY_FILE")

  if [ "$PC_NO_STOP" -eq 0 ]; then
    go_arguments+=(-failfast)
  fi
  go_arguments+=("$package")
  printf '+ TRICE_PC_TEST_MODE=%s TRICE_TEST_NO_STOP=%s go' "$mode" "$PC_NO_STOP"
  printf ' %s' "${go_arguments[@]}"
  printf '\n'
  env TRICE_PC_TEST_MODE="$mode" TRICE_TEST_NO_STOP="$PC_NO_STOP" go "${go_arguments[@]}"
}

# diagnose_bulk_failure reruns the same configuration line by line. A passing
# diagnostic is meaningful: it points at framing, buffering, or state interaction
# that only exists in the continuous bulk stream.
diagnose_bulk_failure() {
  local package="$1"

  printf 'Bulk failure diagnostic for %s\n' "$package"
  if run_pc_package "line-by-line" "$package"; then
    printf 'DIAGNOSTIC: %s passes line by line; inspect bulk framing, padding, buffer wrap, or shared state\n' "$package"
    return 0
  fi
  printf 'DIAGNOSTIC: %s also fails line by line; the assertion above identifies the source line\n' "$package"
  return 1
}

# show_pc_failure surfaces a bounded diagnostic excerpt while retaining complete
# original output. The first failing expectation matters more than the final FAIL.
show_pc_failure() {
  local logfile="$1"
  printf 'PC failure details: %s\n' "$logfile"
  awk '/EXPECTATION FAILURE|Error Trace:|error:|panic:|fatal error:/ { if (!shown) remaining=22; shown=1 } remaining>0 { print; remaining-- }' "$logfile"
  printf 'Last output:\n'
  tail -n 12 "$logfile"
}

# finish_pc_batch waits for each started job before source restoration is possible.
# Batches make reporting deterministic and work with the Bash shipped on macOS.
finish_pc_batch() {
  local index status package logfile
  for ((index = 0; index < ${#PC_PIDS[@]}; index++)); do
    status=0
    wait "${PC_PIDS[index]}" || status=$?
    package="${PC_PACKAGES[index]}"
    logfile="${PC_LOGFILES[index]}"
    if [ "$status" -eq 0 ]; then
      printf 'PC PASS [%s] %s; log: %s\n' "${TRICE_ID_WORKFLOW:-current}" "$package" "$logfile"
    else
      PC_FAILED=1
      printf 'PC FAIL [%s] %s (exit %s)\n' "${TRICE_ID_WORKFLOW:-current}" "$package" "$status"
      printf 'Reproduce after the same ID preparation: cd _test && TRICE_PC_TEST_MODE=%s go test -v -count=1 -overlay=%s %s\n' "$PC_MODE" "$PC_SAVED_OVERLAY" "$package"
      show_pc_failure "$logfile"
      if [ "$PC_MODE" = "auto" ] && grep -q 'execution=.*Bulk' "$logfile"; then
        # Register the diagnostic child too, so cancellation cannot leave it
        # reading target sources while the caller restores the ID state.
        diagnose_bulk_failure "$package" >"$logfile.diagnostic" 2>&1 &
        PC_PIDS[index]=$!
        wait "${PC_PIDS[index]}" || true
        cat "$logfile.diagnostic"
      fi
    fi
  done
  PC_PIDS=() PC_PACKAGES=() PC_LOGFILES=()
}

# cancel_pc_jobs signals each job's process group, including compiler/test
# descendants, and waits before the managed caller restores source files.
cancel_pc_jobs() {
  local status="$1" pid attempts
  trap '' INT TERM
  for pid in "${PC_PIDS[@]}"; do
    kill -TERM -- "-$pid" 2>/dev/null || true
  done
  for pid in "${PC_PIDS[@]}"; do wait "$pid" 2>/dev/null || true; done
  # A test binary may handle TERM after its go parent has already exited.
  # Wait for the whole group before the caller restores the shared ID state.
  for pid in "${PC_PIDS[@]}"; do
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
  exit "$status"
}

# run_pc_matrix selects the proven bulk path inside each compiled configuration.
# Successful configurations run once; diagnostic reruns cannot erase failures.
run_pc_matrix() {
  local package
  while IFS= read -r package; do
    [ -n "$package" ] || continue
    local index="${#PC_PIDS[@]}"
    local artifact_dir="$PC_LOG_DIR/${package##*/}"
    mkdir -p "$artifact_dir" || return 1
    (
      export TRICE_PC_TEST_ARTIFACT_DIR="$artifact_dir"
      run_pc_package "$PC_MODE" "$package"
    ) >"$artifact_dir/output.log" 2>&1 &
    PC_PIDS[index]=$!
    PC_PACKAGES[index]="$package"
    PC_LOGFILES[index]="$artifact_dir/output.log"
    if [ "${#PC_PIDS[@]}" -ge "$PC_JOBS" ]; then
      finish_pc_batch
      if [ "$PC_FAILED" -ne 0 ] && [ "$PC_NO_STOP" -eq 0 ]; then return 1; fi
    fi
  done <"$PC_ALL_PACKAGES_FILE"
  finish_pc_batch
}

main() {
  local selected

  selected="$(select_pc_mode "${1:-full}")" || return 2

  # PC_NO_STOP is exported by testAll.sh. A direct worker invocation can use
  # the same environment variable without changing the historical CLI.
  PC_NO_STOP="${TRICE_TEST_NO_STOP:-0}"
  PC_JOBS="${TRICE_PC_TEST_JOBS:-4}"
  case "$PC_JOBS" in
    '' | *[!0-9]* | 0 | 0*)
      printf 'Invalid TRICE_PC_TEST_JOBS: %s (positive integer required)\n' "$PC_JOBS" >&2
      return 2
      ;;
  esac
  PC_MODE="${TRICE_PC_TEST_MODE:-auto}"
  case "$PC_MODE" in
    auto | line-by-line) ;;
    *)
      printf 'Invalid TRICE_PC_TEST_MODE: %s (auto or line-by-line required)\n' "$PC_MODE" >&2
      return 2
      ;;
  esac
  case "$PC_NO_STOP" in
    0 | 1) ;;
    *)
      printf 'Unsupported TRICE_TEST_NO_STOP value: %s\n' "$PC_NO_STOP" >&2
      return 2
      ;;
  esac
  cd "$ROOT/_test" || {
    printf 'FAIL: cannot enter _test directory\n' >&2
    return 1
  }

  # Process-specific files make concurrent developer runs independent. Their
  # content is orchestration metadata only and never enters a source folder.
  PC_OVERLAY_FILE="${TRICE_TMP_DIR:?TRICE_TMP_DIR is required}/pc-target-overlay.$$.json"
  PC_ALL_PACKAGES_FILE="$TRICE_TMP_DIR/pc-target-all-packages.$$.txt"
  PC_FAILED=0
  PC_PIDS=() PC_PACKAGES=() PC_LOGFILES=()
  PC_LOG_DIR="$(mktemp -d "${LOG_DIR:-$TRICE_TMP_DIR}/pc-${TRICE_ID_WORKFLOW:-current}.XXXXXX")" || return 1
  PC_SAVED_OVERLAY="$PC_LOG_DIR/overlay.json"
  trap cleanup_pc_temp_files EXIT
  trap 'cancel_pc_jobs 130' INT
  trap 'cancel_pc_jobs 143' TERM
  # Separate process groups let cancellation reach go test and its descendants.
  set -m

  # The content-stamped overlay invalidates affected C builds without discarding
  # cached Go dependencies. -count=1 above still executes every selected test.
  prepare_pc_package_lists "$selected" || return 1
  cp "$PC_OVERLAY_FILE" "$PC_SAVED_OVERLAY" || return 1
  printf 'PC matrix: mode=%s jobs=%s configurations=%s logs=%s\n' "$PC_MODE" "$PC_JOBS" "$(wc -l <"$PC_ALL_PACKAGES_FILE" | tr -d ' ')" "$PC_LOG_DIR"
  run_pc_matrix || return 1
  return "$PC_FAILED"
}

main "$@"
