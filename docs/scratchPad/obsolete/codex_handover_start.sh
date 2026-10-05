#!/bin/sh
# SPDX-License-Identifier: MIT
# Import or resume through the same checked workflow on every supported OS.
set -eu
HANDOVER_SCRIPT_DIR=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd)
exec sh "$HANDOVER_SCRIPT_DIR/_codex_handover_python.sh" start "$@"
