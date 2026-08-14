#!/usr/bin/env bash
#
# format_repo.sh
#
# Central wrapper for repository formatting and normalization scripts.
# The Trice User Manual step delegates to a dedicated helper that refreshes
# the manual TOC, numbering, and anchors with mdtoc. PDF generation is handled
# separately by scripts/_generate_trice_user_manual_pdf.sh and by GoReleaser.
#
# Usage:
#   ./scripts/format_repo.sh                 # defaults to FORMAT mode
#   ./scripts/format_repo.sh check
#   ./scripts/format_repo.sh format
#   ./scripts/format_repo.sh check --verbose
#   ./scripts/format_repo.sh format --verbose
#

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

main() {
  local c_format_status=0
  local bind_status=0

  echo "$SCRIPT_DIR/_refresh_trice_user_manual.sh" "$@" && "$SCRIPT_DIR/_refresh_trice_user_manual.sh" "$@"
  echo "$SCRIPT_DIR/_format_shell_scripts.sh" "$@" && "$SCRIPT_DIR/_format_shell_scripts.sh" "$@"
  echo "$SCRIPT_DIR/_format_go_code.sh" "$@" && "$SCRIPT_DIR/_format_go_code.sh" "$@"
  echo "$SCRIPT_DIR/../trice_cleanIDs_in_examples_and_test_folder.sh" && "$SCRIPT_DIR/../trice_cleanIDs_in_examples_and_test_folder.sh"

  # Format ID-free C sources, then restore the repository's canonical Bind
  # state. Preserve a formatter failure while still running Bind, including
  # when format_repo.sh is used in check mode.
  echo "$SCRIPT_DIR/_format_c_code.sh" "$@"
  "$SCRIPT_DIR/_format_c_code.sh" "$@" || c_format_status=$?
  echo "$SCRIPT_DIR/../trice_bindIDs_in_examples_and_test_folder.sh"
  "$SCRIPT_DIR/../trice_bindIDs_in_examples_and_test_folder.sh" || bind_status=$?
  if [ "$c_format_status" -ne 0 ]; then
    return "$c_format_status"
  fi
  if [ "$bind_status" -ne 0 ]; then
    return "$bind_status"
  fi

  echo "$SCRIPT_DIR/../examples/cleanAllTargets.sh" && "$SCRIPT_DIR/../examples/cleanAllTargets.sh"
  echo "$SCRIPT_DIR/_clean-dsstore.sh" "$SCRIPT_DIR/.." --silent && "$SCRIPT_DIR/_clean-dsstore.sh" "$SCRIPT_DIR/.." --silent
}

main "$@"
