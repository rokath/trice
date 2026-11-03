#!/usr/bin/env bash
#
# format_repo.sh
#
# Central wrapper for repository formatting and normalization scripts.
# The manual step checks or refreshes the User Manual and Reference Manual
# chapter TOCs, numbering, and anchors with mdtoc. PDF generation is handled
# separately by scripts/_320_generate_trice_user_manual_pdf.sh and by GoReleaser.
# CHECK preserves sources, ID metadata, build outputs and macOS metadata.
# FORMAT also binds IDs, cleans example build outputs and removes .DS_Store files.
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

# Compare the tracked C/C++ diff around Bind, including binary changes and
# existing local edits. Metadata-only updates do not require another C pass.
# No external diff command or text conversion may hide a source change.
c_format_state() {
  git -C "$SCRIPT_DIR/.." diff --no-ext-diff --no-textconv --binary --no-color --no-renames -- \
    '*.c' '*.h' '*.cpp' '*.hpp' '*.cc' '*.hh' '*.cxx' '*.hxx' |
    git hash-object --stdin
}

main() {
  local mode="format"
  local arg
  # Snapshot only Bind's edits so an already dirty checkout still formats once.
  local before_bind after_bind

  # Keep the mode in sync with the delegated formatters. The last explicit
  # mode wins, matching their existing argument handling.
  for arg in "$@"; do
    case "$arg" in
      check | format) mode="$arg" ;;
    esac
  done

  echo "$SCRIPT_DIR/_310_refresh_trice_manuals.sh" "$@" && "$SCRIPT_DIR/_310_refresh_trice_manuals.sh" "$@"
  echo "$SCRIPT_DIR/_260_format_shell_scripts.sh" "$@" && "$SCRIPT_DIR/_260_format_shell_scripts.sh" "$@"
  echo "$SCRIPT_DIR/_270_format_go_code.sh" "$@" && "$SCRIPT_DIR/_270_format_go_code.sh" "$@"

  echo "$SCRIPT_DIR/_280_format_c_code.sh" "$@" && "$SCRIPT_DIR/_280_format_c_code.sh" "$@"
  if [ "$mode" = "format" ]; then
    # Bind records the formatted source layout. Only new or changed C/C++
    # directives need a second pass; unchanged files and ID metadata do not.
    if ! before_bind="$(c_format_state)"; then
      echo "FAIL: cannot inspect C/C++ changes before Bind; Bind and cleanup were not started." >&2
      return 1
    fi
    echo "$SCRIPT_DIR/../trice_bindIDs_in_examples_and_test_folder.sh" && "$SCRIPT_DIR/../trice_bindIDs_in_examples_and_test_folder.sh"
    if ! after_bind="$(c_format_state)"; then
      echo "FAIL: cannot inspect C/C++ changes after Bind; final formatting and cleanup were not started." >&2
      return 1
    fi
    if [ "$before_bind" != "$after_bind" ]; then
      echo "$SCRIPT_DIR/_280_format_c_code.sh" "$@" && "$SCRIPT_DIR/_280_format_c_code.sh" "$@"
    fi

    # Cleanup belongs to explicit maintenance, never to a formatting check.
    echo "$SCRIPT_DIR/../examples/cleanAllTargets.sh" && "$SCRIPT_DIR/../examples/cleanAllTargets.sh"
    echo "$SCRIPT_DIR/_300_clean_dsstore.sh" "$SCRIPT_DIR/.." --silent && "$SCRIPT_DIR/_300_clean_dsstore.sh" "$SCRIPT_DIR/.." --silent
  fi
}

main "$@"
