#!/usr/bin/env bash
#
# format_repo.sh
#
# Central wrapper for repository formatting and normalization scripts.
# The Trice User Manual step delegates to a dedicated helper that refreshes
# the manual TOC, numbering, and anchors with mdtoc. PDF generation is handled
# separately by scripts/_320_generate_trice_user_manual_pdf.sh and by GoReleaser.
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
  local mode="format"
  local arg

  # Keep the mode in sync with the delegated formatters. The last explicit
  # mode wins, matching their existing argument handling.
  for arg in "$@"; do
    case "$arg" in
      check | format) mode="$arg" ;;
    esac
  done

  echo "$SCRIPT_DIR/_310_refresh_trice_user_manual.sh" "$@" && "$SCRIPT_DIR/_310_refresh_trice_user_manual.sh" "$@"
  echo "$SCRIPT_DIR/_260_format_shell_scripts.sh" "$@" && "$SCRIPT_DIR/_260_format_shell_scripts.sh" "$@"
  echo "$SCRIPT_DIR/_270_format_go_code.sh" "$@" && "$SCRIPT_DIR/_270_format_go_code.sh" "$@"

  echo "$SCRIPT_DIR/_280_format_c_code.sh" "$@" && "$SCRIPT_DIR/_280_format_c_code.sh" "$@"
  if [ "$mode" = "format" ]; then
    # Bind records the line layout produced by the first formatting pass. It
    # can also generate or normalize directives with trailing comments, so a
    # final formatting pass restores their repository-wide alignment.
    echo "$SCRIPT_DIR/../trice_bindIDs_in_examples_and_test_folder.sh" && "$SCRIPT_DIR/../trice_bindIDs_in_examples_and_test_folder.sh"
    echo "$SCRIPT_DIR/_280_format_c_code.sh" "$@" && "$SCRIPT_DIR/_280_format_c_code.sh" "$@"
  fi

  echo "$SCRIPT_DIR/../examples/cleanAllTargets.sh" && "$SCRIPT_DIR/../examples/cleanAllTargets.sh"
  echo "$SCRIPT_DIR/_300_clean_dsstore.sh" "$SCRIPT_DIR/.." --silent && "$SCRIPT_DIR/_300_clean_dsstore.sh" "$SCRIPT_DIR/.." --silent
}

main "$@"
