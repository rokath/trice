#!/usr/bin/env bash
#
# format_repo.sh
#
# Central wrapper for repository formatting and normalization scripts.
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
  "$SCRIPT_DIR/_format_dumeng_toc.sh" "$@"
  "$SCRIPT_DIR/_format_shell_scripts.sh" "$@"
  "$SCRIPT_DIR/_format_go_code.sh" "$@"
  "$SCRIPT_DIR/../trice_cleanIDs_in_examples_and_test_folder.sh"
  "$SCRIPT_DIR/_format_c_code.sh" "$@"
  "$SCRIPT_DIR/../examples/cleanAllTargets.sh"
  "$SCRIPT_DIR/_clean-dsstore.sh" "$SCRIPT_DIR/.." --silent
}

main "$@"
