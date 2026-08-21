#!/usr/bin/env bash

# Test 400: Checks repository shell-script formatting.

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
"$SCRIPT_DIR/_260_format_shell_scripts.sh" check
