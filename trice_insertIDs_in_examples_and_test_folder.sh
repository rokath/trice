#!/bin/bash

# file name: trice_insertIDs_in_examples_and_test_folder.sh

set -euo pipefail

cd "$(dirname "$0")" || exit 1
source ./scripts/_setup_trice_environment.sh

# shellcheck disable=SC2086
if ! trice insert $TRICE_DEFAULTS $TRICE_ALIASES $TRICE_PRJ_FILES -IDMin 13000 -IDMax 16383; then
  echo "FAIL: trice insert failed"
  exit 1
fi

cd - >/dev/null || exit
