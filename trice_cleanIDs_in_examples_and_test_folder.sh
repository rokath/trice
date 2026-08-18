#!/usr/bin/env bash

# file name: trice_cleanIDs_in_examples_and_test_folder.sh

set -euo pipefail

ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./scripts/_trice_id_workflow.sh
source "$ROOT/scripts/_trice_id_workflow.sh"

(
  cd "$ROOT" || exit 1
  trice_id_workflow clean
) || {
  echo "FAIL: trice clean failed" >&2
  exit 1
}
