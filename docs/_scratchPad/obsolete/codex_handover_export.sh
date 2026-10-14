#!/bin/sh
# SPDX-License-Identifier: MIT
# Export a stopped session using the platform-appropriate Python interpreter.
set -eu
HANDOVER_SCRIPT_DIR=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd)
exec sh "$HANDOVER_SCRIPT_DIR/_codex_handover_python.sh" export "$@"
