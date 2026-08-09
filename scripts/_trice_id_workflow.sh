#!/usr/bin/env bash
#
# Shared implementation for the public Insert, Clean, Bind, and re-migration
# repository workflows. This file is sourced and is not a standalone command.

set -u

TRICE_WORKFLOW_SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

# shellcheck source=./_setup_trice_environment.sh
source "$TRICE_WORKFLOW_SCRIPT_DIR/_setup_trice_environment.sh"

# trice_id_workflow executes one repository state transition from the stable
# repository root. Successful default invocations stay silent; the delegated
# tools retain their existing verbose options and error diagnostics.
trice_id_workflow() {
  local action="$1"

  case "$action" in
    insert)
      # The option groups intentionally use shell word splitting because they
      # preserve the long-standing repository override interface.
      # shellcheck disable=SC2086
      trice insert $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES $TRICE_ID_OPTIONS
      ;;
    clean)
      # shellcheck disable=SC2086
      trice clean $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES
      ;;
    bind)
      # shellcheck disable=SC2086
      trice bind $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES $TRICE_ID_OPTIONS -bindDir "$TRICE_BIND_DIR"
      ;;
    remigrate)
      # Re-migration adjusts shared location lines together with the source
      # includes so the intermediate Clean state is internally consistent.
      # shellcheck disable=SC2086
      go run ./internal/id/remigratecmd $TRICE_PRJ_FILES $TRICE_EXCLUDES \
        -bindDir "$TRICE_BIND_DIR" -li "$TRICE_LI_JSON" -liRoot "$TRICE_LI_ROOT"
      ;;
    *)
      printf 'Unsupported Trice ID workflow: %s\n' "$action" >&2
      return 2
      ;;
  esac
}
