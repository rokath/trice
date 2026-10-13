#!/usr/bin/env bash
#
# _310_refresh_trice_user_manual.sh
#
# Purpose
# -------
# Maintenance wrapper for the User Manual and Reference Manual chapters in docs/.
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
# Keep the User Manual first; managed RM chapters are discovered before any writes.
MANUAL_FILES=("docs/TriceUserManual.md")

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
  if [[ ! -s "$1" ]]; then
    die "manual file not found or empty: $1"
  fi
}

# Discover the flat chapter sources without introducing another maintained list.
# Navigation files are not chapters, even if they later contain a TOC example.
# Validate inputs before invoking mdtoc so missing sources cannot cause partial
# formatting. Files without a managed container remain outside this formatter.
collect_reference_chapters() {
  local CHAPTER_FILE
  if [[ ! -d docs/rm ]]; then
    die "reference manual directory not found: docs/rm"
  fi
  require_manual_file "docs/rm/README.md"

  for CHAPTER_FILE in docs/rm/*.md; do
    # An unmatched glob stays literal; never pass it to mdtoc as a filename.
    [[ -f "$CHAPTER_FILE" ]] || continue
    case "$CHAPTER_FILE" in
      docs/rm/README.md | docs/rm/HEADER.md | docs/rm/FOOTER.md | docs/rm/INDEX.md)
        continue
        ;;
    esac
    require_manual_file "$CHAPTER_FILE"
    if grep -q '^<!-- mdtoc -->' "$CHAPTER_FILE"; then
      MANUAL_FILES+=("$CHAPTER_FILE")
    fi
  done

  if [[ "${#MANUAL_FILES[@]}" -eq 1 ]]; then
    die "no mdtoc-managed reference chapters found: docs/rm/*.md; expected <!-- mdtoc --> in a chapter, excluding README.md, HEADER.md, FOOTER.md and INDEX.md"
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
  require_manual_file "docs/TriceUserManual.md"
  collect_reference_chapters
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
