#!/usr/bin/env bash

# Test 400: Checks repository shell-script formatting.

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
# shellcheck source=./_100_test_common.sh
source "$SCRIPT_DIR/_100_test_common.sh"
init_logfile
run_cmd "$SCRIPT_DIR/_260_format_shell_scripts.sh" check
