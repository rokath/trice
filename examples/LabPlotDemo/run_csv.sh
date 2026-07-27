#!/bin/sh
# Start LabPlot and feed the live project with the CSV demo over UDP.
set -eu

script_dir=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd)
repo_root=$(CDPATH='' cd -- "$script_dir/../.." && pwd)
demo_dir="$repo_root/examples/DemoPlotData_CSV"

find_labplot() {
  if [ -n "${LABPLOT:-}" ]; then
    [ -x "$LABPLOT" ] && printf '%s\n' "$LABPLOT" && return
    return 1
  fi
  if command -v labplot >/dev/null 2>&1; then
    command -v labplot
    return
  fi
  if command -v labplot.exe >/dev/null 2>&1; then
    command -v labplot.exe
    return
  fi
  if [ -x "/Applications/LabPlot.app/Contents/MacOS/labplot" ]; then
    printf '%s\n' "/Applications/LabPlot.app/Contents/MacOS/labplot"
    return
  fi
  case "$(uname -s)" in
    MINGW* | MSYS* | CYGWIN*)
      # Convert Windows environment paths to Git Bash paths before searching.
      for install_root in "${PROGRAMFILES:-}" "${ProgramFiles:-}" \
        "${PROGRAMW6432:-}" "${ProgramW6432:-}" "${LOCALAPPDATA:-}"; do
        [ -n "$install_root" ] || continue
        if command -v cygpath >/dev/null 2>&1; then
          install_root=$(cygpath -u "$install_root")
        fi
        [ -d "$install_root" ] || continue
        for labplot_path in \
          "$install_root/LabPlot/bin/labplot.exe" \
          "$install_root/LabPlot/bin/labplot2.exe" \
          "$install_root/labplot/bin/labplot.exe" \
          "$install_root/labplot/bin/labplot2.exe" \
          "$install_root/KDE/bin/labplot.exe" \
          "$install_root/KDE/bin/labplot2.exe"; do
          if [ -x "$labplot_path" ]; then
            printf '%s\n' "$labplot_path"
            return
          fi
        done
      done
      ;;
  esac
  return 1
}

labplot=$(find_labplot) || {
  echo "LabPlot not found. Set LABPLOT to its executable." >&2
  exit 1
}
demo_bin="$demo_dir/bin/DemoPlotData_CSV"
if [ ! -x "$demo_bin" ] && [ ! -x "$demo_bin.exe" ]; then
  (cd "$demo_dir" && ./build.sh)
fi
if [ ! -x "$demo_bin" ]; then
  demo_bin="$demo_bin.exe"
fi
if [ ! -x "$demo_bin" ]; then
  echo "DemoPlotData_CSV executable not found in $demo_dir/bin." >&2
  exit 1
fi
if [ "$(uname -s)" = Darwin ] && [ -z "${LABPLOT:-}" ] && ! command -v labplot >/dev/null 2>&1; then
  open -na LabPlot --args "$script_dir/LabPlotDemo.lml" >/dev/null 2>&1 &
else
  "$labplot" "$script_dir/LabPlotDemo.lml" &
fi
labplot_pid=$!
cleanup() { kill "$labplot_pid" 2>/dev/null || true; }
trap cleanup INT TERM EXIT

"$demo_bin" --udp 127.0.0.1 9000
