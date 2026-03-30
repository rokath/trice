#!/usr/bin/env bash
#
# Shared helper functions for the testAll.sh script family.
#
# Direct invocation:
# - Not intended. This script is sourced by the individual
#   _testAll_XX_*.sh scripts.
#
# Important environment variables:
# - TESTALL_LOG_DIR
#   Target directory for per-step logs. Default: ./temp/testAll
# - TESTALL_MAIN_LOG
#   Target file for the combined log assembled by testAll.sh.
#   Default: ./testAll.log
# - GOCACHE
#   If unset, a repo-local cache under ./.gocache is used.
# - TRICE_TMP_DIR, TRICE_TIL_JSON, TRICE_LI_JSON
#   Shared temp files used by the existing Trice helper scripts.
#
# Goal of the split:
# - Each actual action lives in its own directly executable script.
# - testAll.sh only orchestrates those steps.
# - Each step writes its own log into ./temp/testAll/.

set -u

TESTALL_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
TESTALL_LOG_DIR="${TESTALL_LOG_DIR:-$TESTALL_ROOT/temp/testAll}"
TESTALL_MAIN_LOG="${TESTALL_MAIN_LOG:-$TESTALL_ROOT/testAll.log}"

ensure_testall_dirs() {
  mkdir -p "$TESTALL_LOG_DIR"
}

step_name_from_path() {
  basename "$1" .sh
}

step_log_path_from_name() {
  printf '%s/%s.log\n' "$TESTALL_LOG_DIR" "$1"
}

init_step_log() {
  TESTALL_STEP_NAME="$(step_name_from_path "$1")"
  TESTALL_STEP_LOG="$(step_log_path_from_name "$TESTALL_STEP_NAME")"
  : >"$TESTALL_STEP_LOG"
}

log() {
  printf '%s\n' "$*" | tee -a "$TESTALL_STEP_LOG"
}

warn() {
  log "WARN: $*"
}

skip() {
  log "SKIP: $*"
}

fail() {
  log "FAIL: $*"
  exit "${2:-1}"
}

run_cmd() {
  log "+ $*"
  "$@" 2>&1 | tee -a "$TESTALL_STEP_LOG"
  local rc=${PIPESTATUS[0]}
  if [ "$rc" -ne 0 ]; then
    log "Command failed with exit code $rc: $*"
  fi
  return "$rc"
}

has_command() {
  command -v "$1" >/dev/null 2>&1
}

prepare_shared_env() {
  local selected="${1:-quick}"
  export TESTALL_SELECTED="$selected"
  export GOCACHE="${GOCACHE:-$TESTALL_ROOT/.gocache}"
  export TRICE_TMP_DIR="${TRICE_TMP_DIR:-$TESTALL_ROOT/temp/testAll}"
  export TRICE_TIL_JSON="${TRICE_TIL_JSON:-$TRICE_TMP_DIR/demoTIL.json}"
  export TRICE_LI_JSON="${TRICE_LI_JSON:-$TRICE_TMP_DIR/demoLI.json}"
  mkdir -p "$GOCACHE" "$TRICE_TMP_DIR"
}

log_environment() {
  log "SELECTED: ${TESTALL_SELECTED:-unknown}"
  log "OSTYPE: ${OSTYPE:-unknown}"
  log "GOCACHE: ${GOCACHE:-unset}"
  log "TRICE_TMP_DIR: ${TRICE_TMP_DIR:-unset}"
  log "TRICE_TIL_JSON: ${TRICE_TIL_JSON:-unset}"
  log "TRICE_LI_JSON: ${TRICE_LI_JSON:-unset}"
  if has_command uname; then
    run_cmd uname -a || true
  fi
  if has_command go; then
    run_cmd go version || true
  fi
}

grep_log() {
  local pattern="$1"
  local file="$2"
  grep -Eiq "$pattern" "$file"
}

require_mode_or_default() {
  case "${1:-quick}" in
    quick | config | full) printf '%s\n' "${1:-quick}" ;;
    *)
      printf 'Unsupported selection: %s\n' "${1:-}" >&2
      printf 'Allowed: quick, config, full\n' >&2
      exit 2
      ;;
  esac
}

combine_logs() {
  local output="$1"
  shift
  : >"$output"
  local first=1
  local file
  for file in "$@"; do
    if [ ! -f "$file" ]; then
      continue
    fi
    if [ "$first" -eq 0 ]; then
      printf '\n' >>"$output"
    fi
    first=0
    cat "$file" >>"$output"
  done
}
