#!/usr/bin/env bash
#
# Shared PC/CGO test worker. ID preparation and restoration are intentionally
# owned by the workflow-specific caller.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"

# select_pc_mode keeps direct worker misuse deterministic while the public
# wrappers continue to provide the established quick/full interface.
select_pc_mode() {
  case "${1:-full}" in
    quick | full) printf '%s\n' "${1:-full}" ;;
    *)
      printf 'Unsupported PC target selection: %s\n' "${1:-}" >&2
      return 2
      ;;
  esac
}

main() {
  local selected

  selected="$(select_pc_mode "${1:-full}")" || return 2
  cd "$ROOT/_test" || {
    printf 'FAIL: cannot enter _test directory\n' >&2
    return 1
  }

  # triceCheck.c is included from outside package directories. Clearing both
  # caches prevents an older C switch from being paired with current //exp data.
  printf '+ go clean -cache -testcache\n'
  go clean -cache -testcache

  if [ "$selected" = "quick" ]; then
    # The Ring-buffer bulk package validates message boundaries across one
    # continuous stream and completes much faster than the former line-by-line
    # double-buffer package. Full mode still covers every target package.
    printf '+ go test ./abc_rx_host/... ./abc_tx_host/... ./ringB_de_multi_cobs_ua/...\n'
    go test ./abc_rx_host/... ./abc_tx_host/... ./ringB_de_multi_cobs_ua/...
  else
    printf '+ go test ./...\n'
    go test ./...
  fi
}

main "$@"
