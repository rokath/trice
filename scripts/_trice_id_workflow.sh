#!/usr/bin/env bash
#
# Shared implementation for the public Insert, Clean, Bind, and re-migration
# repository workflows. This file is sourced and is not a standalone command.

set -u

TRICE_WORKFLOW_SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"

# shellcheck source=./_setup_trice_environment.sh
source "$TRICE_WORKFLOW_SCRIPT_DIR/_setup_trice_environment.sh"

# trice_id_workflow reports the complete shared configuration and executes one
# repository state transition from the stable repository root.
trice_id_workflow() {
  local action="$1"

  printf 'Trice ID workflow: %s\n' "$action"
  printf 'Sources: %s\n' "$TRICE_PRJ_FILES"
  printf 'TIL: %s\n' "$TRICE_TIL_JSON"
  printf 'LI: %s\n' "$TRICE_LI_JSON"
  printf 'LI root: %s\n' "$TRICE_LI_ROOT"
  printf 'ID options: %s\n' "$TRICE_ID_OPTIONS"
  printf 'Bind directory: %s\n' "$TRICE_BIND_DIR"

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
      # Re-migration needs only source ownership and exclusions. The TIL, LI,
      # root, and ID range remain visible above as the shared workflow context.
      # shellcheck disable=SC2086
      go run ./internal/id/remigratecmd $TRICE_PRJ_FILES $TRICE_EXCLUDES -bindDir "$TRICE_BIND_DIR"
      ;;
    *)
      printf 'Unsupported Trice ID workflow: %s\n' "$action" >&2
      return 2
      ;;
  esac
}
