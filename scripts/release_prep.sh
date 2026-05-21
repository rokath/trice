#!/usr/bin/env bash
#
# release_prep.sh
#
# Repository release preparation wrapper.
#
# Usage:
#   ./scripts/release_prep.sh
#   ./scripts/release_prep.sh check
#   ./scripts/release_prep.sh format
#
# Default mode is format.
# - check  validates the repo without modifying files.
# - format runs the formatter/generator chain and then validates the release
#          snapshot artifacts.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

MODE="${1:-format}"
shift || true

have_command() {
  command -v "$1" >/dev/null 2>&1
}

require_command() {
  local command_name="$1"
  if ! have_command "$command_name"; then
    printf 'Required tool not found in PATH: %s\n' "$command_name" >&2
    exit 1
  fi
}

case "$MODE" in
  check | format) ;;
  *)
    printf 'Unsupported mode: %s\nAllowed: check, format\n' "$MODE" >&2
    exit 2
    ;;
esac

run_step() {
  local script="$1"
  shift
  "$SCRIPT_DIR/$script" "$@"
}

main() {
  if [ "$MODE" = "format" ]; then
    require_command goreleaser
    require_command npx
    require_command cc
    run_step format_repo.sh format "$@"
  fi

  run_step _testAll_05_MarkdownLint.sh
  run_step _testAll_06_LinkCheck.sh
  run_step _testAll_06a_GoReleaser.sh
  run_step _testAll_06b_ActionLint.sh

  if [ "$MODE" = "format" ]; then
    run_step _testAll_14_GoReleaserSnapshot.sh
  fi
}

main "$@"
