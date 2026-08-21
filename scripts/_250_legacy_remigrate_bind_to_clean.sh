#!/usr/bin/env bash
#
# Return the canonical examples and PC target sources from Bound to Clean.

set -euo pipefail

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd -- "$SCRIPT_DIR/.." && pwd)"
# shellcheck source=./_130_trice_id_workflow.sh
source "$SCRIPT_DIR/_130_trice_id_workflow.sh"

(
  cd "$ROOT" || exit 1
  trice_id_workflow remigrate
) || {
  echo "FAIL: Trice Bind-to-Clean re-migration failed" >&2
  exit 1
}
