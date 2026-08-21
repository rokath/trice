#!/usr/bin/env bash
#
# Generate bind sidecars for the canonical examples and PC target sources.

set -euo pipefail

ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./scripts/_130_trice_id_workflow.sh
source "$ROOT/scripts/_130_trice_id_workflow.sh"

(
  cd "$ROOT" || exit 1
  trice_id_workflow bind
) || {
  echo "FAIL: trice bind failed" >&2
  exit 1
}
