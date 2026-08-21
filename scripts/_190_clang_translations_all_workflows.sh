#!/usr/bin/env bash
#
# Compatibility entry point for both Clang ID-workflow variants.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

main() {
  local quiet=""
  local failed=0
  local arg

  for arg in "$@"; do
    case "$arg" in
      --quiet) quiet="--quiet" ;;
      *)
        printf 'Unsupported Clang translation argument: %s\n' "$arg" >&2
        return 2
        ;;
    esac
  done

  "$SCRIPT_DIR/_560_test_clang_insert.sh" ${quiet:+"$quiet"} || failed=1
  "$SCRIPT_DIR/_570_test_clang_bind.sh" ${quiet:+"$quiet"} || failed=1
  return "$failed"
}

main "$@"
