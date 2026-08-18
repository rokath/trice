#!/usr/bin/env bash
#
# Compatibility entry point for the complete PC target workflow matrix.
# Direct invocation defaults to full, matching the historical script.

set -u

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

main() {
  local selected="full"
  local quiet=""
  local failed=0
  local arg

  for arg in "$@"; do
    case "$arg" in
      --quiet) quiet="--quiet" ;;
      quick | full) selected="$arg" ;;
      *)
        printf 'Unsupported PC target argument: %s\n' "$arg" >&2
        return 2
        ;;
    esac
  done

  "$SCRIPT_DIR/_testAll_10a_PcTargetTests_insert.sh" ${quiet:+"$quiet"} "$selected" || failed=1
  "$SCRIPT_DIR/_testAll_10b_PcTargetTests_bind.sh" ${quiet:+"$quiet"} "$selected" || failed=1
  return "$failed"
}

main "$@"
