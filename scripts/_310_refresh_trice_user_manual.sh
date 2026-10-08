#!/usr/bin/env bash
#
# _310_refresh_trice_user_manual.sh
#
# Purpose
# -------
# Maintenance wrapper for the User Manual and Reference Manual in docs/.
#
# This script only checks or regenerates the manual TOC, numbering, and anchors
# with mdtoc. PDF generation is intentionally handled by the separate script
# scripts/_320_generate_trice_user_manual_pdf.sh.
#
# Usage
# -----
#   ./scripts/_310_refresh_trice_user_manual.sh
#   ./scripts/_310_refresh_trice_user_manual.sh check
#   ./scripts/_310_refresh_trice_user_manual.sh format
#   ./scripts/_310_refresh_trice_user_manual.sh check --verbose
#   ./scripts/_310_refresh_trice_user_manual.sh format --verbose

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
cd "$REPO_ROOT"

MODE="format"
VERBOSE=0
# Both entry points must be present; never silently omit one from validation.
MANUAL_FILES=("docs/TriceUserManual.md" "docs/TriceReferenceManual.md")

usage() {
  sed -n '2,22p' "$0" | sed 's/^# \{0,1\}//'
}

log_verbose() {
  if [[ "$VERBOSE" -eq 1 ]]; then
    printf '%s\n' "$*"
  fi
}

die() {
  printf 'ERROR: %s\n' "$*" >&2
  exit 1
}

parse_args() {
  while [[ "$#" -gt 0 ]]; do
    case "$1" in
      format | check)
        MODE="$1"
        ;;
      --verbose)
        VERBOSE=1
        ;;
      -h | --help)
        usage
        exit 0
        ;;
      *)
        die "unknown argument: $1"
        ;;
    esac
    shift
  done
}

require_manual_file() {
  if [[ ! -s "$MANUAL_FILE" ]]; then
    die "manual file not found or empty: $MANUAL_FILE"
  fi
}

require_mdtoc() {
  if ! command -v mdtoc >/dev/null 2>&1; then
    die "required formatter 'mdtoc' is not installed or not in PATH"
  fi
}

main() {
  parse_args "$@"
  local MANUAL_FILE
  for MANUAL_FILE in "${MANUAL_FILES[@]}"; do
    require_manual_file
  done
  require_mdtoc

  for MANUAL_FILE in "${MANUAL_FILES[@]}"; do
    if [[ "$MODE" == "check" ]]; then
      log_verbose "Checking $MANUAL_FILE TOC, numbering, and anchors with mdtoc..."
      mdtoc check --file "$MANUAL_FILE"
    else
      log_verbose "Regenerating $MANUAL_FILE TOC, numbering, and anchors with mdtoc..."
      mdtoc generate --file "$MANUAL_FILE"
    fi
  done
}

main "$@"
