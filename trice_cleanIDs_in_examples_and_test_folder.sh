#!/bin/bash

# file name: trice_cleanIDs_in_examples_and_test_folder.sh

set -euo pipefail

cd "$(dirname "$0")" || exit 1
source ./scripts/_setup_trice_environment.sh

# shellcheck disable=SC2086
if ! trice clean $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES; then
  echo "FAIL: trice clean failed" >&2
  exit 1
fi
