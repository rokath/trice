#!/bin/sh
# SPDX-License-Identifier: MIT
# Run isolated behavioral fixtures; arguments such as -v and -k reach unittest.
set -eu
HANDOVER_SCRIPT_DIR=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd)
exec sh "$HANDOVER_SCRIPT_DIR/_codex_handover_python.sh" test "$@"
