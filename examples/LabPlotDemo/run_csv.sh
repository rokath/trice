#!/bin/sh
# Start LabPlot and feed the live project with the CSV demo over UDP.
set -eu

script_dir=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd)
repo_root=$(CDPATH='' cd -- "$script_dir/../.." && pwd)
demo_dir="$repo_root/examples/DemoPlotData_CSV"

find_labplot() {
  if [ -n "${LABPLOT:-}" ]; then
    printf '%s\n' "$LABPLOT"
    return
  fi
  if command -v labplot >/dev/null 2>&1; then
    command -v labplot
    return
  fi
  if [ -x "/Applications/LabPlot.app/Contents/MacOS/labplot" ]; then
    printf '%s\n' "/Applications/LabPlot.app/Contents/MacOS/labplot"
    return
  fi
  return 1
}

labplot=$(find_labplot) || {
  echo "LabPlot not found. Set LABPLOT to its executable." >&2
  exit 1
}
[ -x "$demo_dir/build/DemoPlotData_CSV" ] || (cd "$demo_dir" && ./build.sh)
if [ "$(uname -s)" = Darwin ] && [ -z "${LABPLOT:-}" ] && ! command -v labplot >/dev/null 2>&1; then
  open -na LabPlot --args "$script_dir/LabPlotDemo.lml" >/dev/null 2>&1 &
else
  "$labplot" "$script_dir/LabPlotDemo.lml" &
fi
labplot_pid=$!
cleanup() { kill "$labplot_pid" 2>/dev/null || true; }
trap cleanup INT TERM EXIT

"$demo_dir/build/DemoPlotData_CSV" --udp 127.0.0.1 9000
