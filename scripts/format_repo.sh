#!/usr/bin/env bash
#
# format_repo.sh
#
# Central wrapper for repository formatting and normalization scripts.
# The Trice User Manual step delegates to a dedicated helper that first tries
# best-effort VS Code automation for TOC/PDF refresh and then runs the
# repository-owned normalization script.
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

# PDF generation takes long and the PDF now is a release asset.
# "$SCRIPT_DIR/_refresh_trice_user_manual.sh" "$@"
main() {
  "$SCRIPT_DIR/_format_shell_scripts.sh" "$@"
  "$SCRIPT_DIR/_format_go_code.sh" "$@"
  "$SCRIPT_DIR/../trice_cleanIDs_in_examples_and_test_folder.sh"
  "$SCRIPT_DIR/_format_c_code.sh" "$@"
  "$SCRIPT_DIR/../examples/cleanAllTargets.sh"
  "$SCRIPT_DIR/_clean-dsstore.sh" "$SCRIPT_DIR/.." --silent
}

main "$@"
