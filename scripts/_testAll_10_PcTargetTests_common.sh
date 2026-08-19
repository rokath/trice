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
    "${PC_ALL_PACKAGES_FILE:-}" \
    "${PC_BULK_PACKAGES_FILE:-}" \
    "${PC_FAILED_BULK_FILE:-}" \
    "${PC_DIAGNOSED_PACKAGES_FILE:-}"
}

# create_harness_overlay makes every existing PC configuration consume the two
# canonical harness templates from testdata. This avoids both new configuration
# directories and mechanical changes to 61 generated copies. Overlay paths stay
# relative to _test so they work with native Go on Linux, macOS, and Windows.
create_harness_overlay() {
  local overlay_file="$1"
  local generated_file
  local package_dir
  local separator=""

  printf '{"Replace":{' >"$overlay_file"
  for generated_file in */generated_cgoPackage.go; do
    [ -f "$generated_file" ] || continue
    package_dir="${generated_file%/generated_cgoPackage.go}"
    printf '%s"%s/generated_cgoPackage.go":"testdata/cgoPackage.go"' "$separator" "$package_dir" >>"$overlay_file"
    separator=","
    printf '%s"%s/generated_cgoPackage_test.go":"testdata/cgoPackage_test.go"' "$separator" "$package_dir" >>"$overlay_file"
  done
  printf '}}\n' >>"$overlay_file"
  if [ -z "$separator" ]; then
    printf 'FAIL: no generated PC-test harness files found under _test\n' >&2
    return 1
  fi
}

# list_bulk_packages prints configurations whose existing target mode explicitly
# declares Bulk support. A pure deferred configuration may still be line-only:
# unframed single packages, for example, cannot retain their package boundaries
# when concatenated into one decoder input. The configuration remains the source
# of truth instead of the runner guessing from buffer and framing switches.
list_bulk_packages() {
  local module_path
  local config_file
  local package_dir

  module_path="$(go list -m)" || return 1
  for config_file in */cgo_test.go; do
    [ -f "$config_file" ] || continue
    if grep -Eq 'targetMode[[:space:]]*=[[:space:]]*"deferredMode(Bulk|LineByLineAndBulk)"' "$config_file"; then
      package_dir="${config_file%/cgo_test.go}"
      printf '%s/_test/%s\n' "$module_path" "$package_dir"
    fi
  done | LC_ALL=C sort
}

# prepare_pc_package_lists resolves all packages before testing. Resolution is
# infrastructure work and therefore always fails hard, even with --no-stop.
prepare_pc_package_lists() {
  local selected="$1"

  create_harness_overlay "$PC_OVERLAY_FILE" || return 1
  if [ "$selected" = "quick" ]; then
    printf '+ go list ./ringB_de_multi_cobs_ua ./abc_rx_host/... ./abc_tx_host/...\n'
    go list ./ringB_de_multi_cobs_ua >"$PC_BULK_PACKAGES_FILE" || return 1
    go list ./abc_rx_host/... ./abc_tx_host/... >"$PC_ALL_PACKAGES_FILE" || return 1
  else
    printf '+ list pure-deferred bulk packages\n'
    list_bulk_packages >"$PC_BULK_PACKAGES_FILE" || return 1
    printf '+ go list ./...\n'
    go list ./... >"$PC_ALL_PACKAGES_FILE" || return 1
  fi
  if [ ! -s "$PC_BULK_PACKAGES_FILE" ]; then
    printf 'FAIL: PC bulk package list is empty\n' >&2
    return 1
  fi
  if [ ! -s "$PC_ALL_PACKAGES_FILE" ]; then
    printf 'FAIL: PC line-by-line package list is empty\n' >&2
    return 1
  fi
}

# run_pc_package executes one configuration in the requested mode. Package-wise
# execution gives the shell runner an exact configuration to diagnose and makes
# fail-fast behavior deterministic instead of relying on Go package scheduling.
run_pc_package() {
  local mode="$1"
  local package="$2"
  local go_arguments=(test -count=1 "-overlay=$PC_OVERLAY_FILE")

  if [ "$PC_NO_STOP" -eq 0 ]; then
    go_arguments+=(-failfast)
  fi
  if [ "$mode" = "bulk" ]; then
    go_arguments+=(-run '^TestTriceLog$')
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

# run_bulk_phase runs every selected bulk configuration before the regular line
# phase. Without --no-stop, a failure is diagnosed immediately and ends the PC
# step. With --no-stop, failures are queued until all bulk configurations ran.
run_bulk_phase() {
  local package

  while IFS= read -r package; do
    [ -n "$package" ] || continue
    if run_pc_package "bulk" "$package"; then
      continue
    fi
    PC_FAILED=1
    if [ "$PC_NO_STOP" -eq 0 ]; then
      diagnose_bulk_failure "$package" || true
      return 1
    fi
    printf '%s\n' "$package" >>"$PC_FAILED_BULK_FILE"
  done <"$PC_BULK_PACKAGES_FILE"
}

# run_queued_bulk_diagnostics preserves the Bulk-first ordering requested for
# --no-stop while ensuring each failing configuration still gets precise line
# diagnostics. Diagnosed packages are recorded to avoid a duplicate line run.
run_queued_bulk_diagnostics() {
  local package

  while IFS= read -r package; do
    [ -n "$package" ] || continue
    diagnose_bulk_failure "$package" || true
    printf '%s\n' "$package" >>"$PC_DIAGNOSED_PACKAGES_FILE"
  done <"$PC_FAILED_BULK_FILE"
}

# run_line_phase executes the same configuration folders with line selection.
# Quick mode contains only its small non-bulk smoke packages here; Full mode
# covers the complete matrix and skips bulk-failure diagnostics already run.
run_line_phase() {
  local package

  while IFS= read -r package; do
    [ -n "$package" ] || continue
    if grep -Fqx "$package" "$PC_DIAGNOSED_PACKAGES_FILE"; then
      continue
    fi
    if run_pc_package "line-by-line" "$package"; then
      continue
    fi
    PC_FAILED=1
    if [ "$PC_NO_STOP" -eq 0 ]; then
      return 1
    fi
  done <"$PC_ALL_PACKAGES_FILE"
}

main() {
  local selected

  selected="$(select_pc_mode "${1:-full}")" || return 2

  # PC_NO_STOP is exported by testAll.sh. A direct worker invocation can use
  # the same environment variable without changing the historical CLI.
  PC_NO_STOP="${TRICE_TEST_NO_STOP:-0}"
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
  PC_BULK_PACKAGES_FILE="$TRICE_TMP_DIR/pc-target-bulk-packages.$$.txt"
  PC_FAILED_BULK_FILE="$TRICE_TMP_DIR/pc-target-failed-bulk.$$.txt"
  PC_DIAGNOSED_PACKAGES_FILE="$TRICE_TMP_DIR/pc-target-diagnosed.$$.txt"
  PC_FAILED=0
  : >"$PC_FAILED_BULK_FILE"
  : >"$PC_DIAGNOSED_PACKAGES_FILE"
  trap cleanup_pc_temp_files EXIT

  # triceCheck.c is included from outside package directories. Clearing both
  # caches prevents an older C switch from being paired with current //exp data.
  printf '+ go clean -cache -testcache\n'
  go clean -cache -testcache || return 1
  prepare_pc_package_lists "$selected" || return 1
  run_bulk_phase || return 1
  run_queued_bulk_diagnostics
  run_line_phase || return 1
  return "$PC_FAILED"
}

main "$@"
