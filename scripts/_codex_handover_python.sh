#!/bin/sh
# SPDX-License-Identifier: MIT
# Share interpreter discovery across the two user commands and their tests.
set -eu
HANDOVER_SCRIPT_DIR=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd)
case "${1:-}" in
  export) handover_module=codex_handover_export.py ;;
  start) handover_module=codex_handover_start.py ;;
  test) handover_module=test_codex_handover.py ;;
  *)
    echo "ABBRUCH: Erwartet wird export, start oder test." >&2
    exit 1
    ;;
esac
shift

# Git Bash must use native Windows Python so Codex's profile, process discovery
# and file locks agree. WSL intentionally follows the Linux branch instead.
handover_native=0
handover_candidates="python3 python"
case "$(uname -s)" in
  MINGW* | MSYS* | CYGWIN*)
    handover_native=1
    handover_candidates="py python python3"
    ;;
esac
handover_probe='import sys; sys.exit(0 if sys.version_info >= (3, 11) and (sys.argv[1] != "1" or sys.platform == "win32") else 1)'

# Probe all candidates before failing: an older python3 or a Windows Store alias
# must not hide another working installation. No packages are installed here.
for handover_python in $handover_candidates; do
  if [ "$handover_python" = py ]; then
    if py -3 -c "$handover_probe" "$handover_native" >/dev/null 2>&1; then
      echo "[Python] Verwende py -3."
      exec py -3 -B "$HANDOVER_SCRIPT_DIR/$handover_module" "$@"
    fi
  elif "$handover_python" -c "$handover_probe" "$handover_native" >/dev/null 2>&1; then
    echo "[Python] Verwende $handover_python."
    exec "$handover_python" -B "$HANDOVER_SCRIPT_DIR/$handover_module" "$@"
  fi
done
echo "ABBRUCH: Python ab 3.11 fehlt. Python installieren, Terminal neu öffnen und dasselbe .sh-Skript erneut aufrufen." >&2
if [ "$handover_native" = 1 ]; then
  echo "Git Bash benötigt natives Windows-Python (py -3, python oder python3)." >&2
fi
exit 1
