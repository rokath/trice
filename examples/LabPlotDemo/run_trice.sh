#!/bin/sh
# Start LabPlot, decode the Trice demo, and forward normalized CSV over UDP.
set -eu

script_dir=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd)
repo_root=$(CDPATH='' cd -- "$script_dir/../.." && pwd)
demo_dir="$repo_root/examples/DemoData_Trice"
til_file=${TRICE_TIL:-$repo_root/demoTIL.json}

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

# Wait until a process has bound the requested UDP input socket.
# The fallback delay supports systems without lsof, such as Git Bash.
wait_for_udp_port() {
  if command -v lsof >/dev/null 2>&1; then
    attempt=0
    while ! lsof -nP -iUDP:"$1" >/dev/null 2>&1; do
      attempt=$((attempt + 1))
      if [ "$attempt" -ge 15 ]; then
        echo "$2 did not open UDP port $1 within 15 seconds." >&2
        return 1
      fi
      sleep 1
    done
    return 0
  fi

  sleep "$3"
}

labplot=$(find_labplot) || {
  echo "LabPlot not found. Set LABPLOT to its executable." >&2
  exit 1
}
tlog_bin=${TLOG:-tlog}
if [ ! -x "$tlog_bin" ] && ! command -v "$tlog_bin" >/dev/null 2>&1; then
  echo "tlog not found. Set TLOG to its executable." >&2
  exit 1
fi
demo_bin="$demo_dir/bin/DemoData_Trice"
if [ ! -x "$demo_bin" ] && [ ! -x "$demo_bin.exe" ]; then
  (cd "$demo_dir" && ./build.sh)
fi
if [ ! -x "$demo_bin" ]; then
  demo_bin="$demo_bin.exe"
fi
if [ ! -x "$demo_bin" ]; then
  echo "DemoData_Trice executable not found in $demo_dir/bin." >&2
  exit 1
fi
if [ "$(uname -s)" = Darwin ] && [ -z "${LABPLOT:-}" ] && ! command -v labplot >/dev/null 2>&1; then
  open -na LabPlot --args "$script_dir/LabPlotDemo.lml" >/dev/null 2>&1 &
else
  "$labplot" "$script_dir/LabPlotDemo.lml" &
fi
labplot_pid=$!
producer_pid=
tlog_pid=
cleanup() {
  if [ -n "$producer_pid" ]; then kill "$producer_pid" 2>/dev/null || true; fi
  if [ -n "$tlog_pid" ]; then kill "$tlog_pid" 2>/dev/null || true; fi
  kill "$labplot_pid" 2>/dev/null || true
}
trap cleanup INT TERM EXIT

echo "Waiting for LabPlot to open UDP port 9000..."
wait_for_udp_port 9000 LabPlot 5

"$tlog_bin" -p UDP4 -args 127.0.0.1:9001 -til "$til_file" -ulabel vis_demo \
  '-vis=vis_demo:printf("%0.6f,%0.6f,%0.6f,%0.6f\n",ts/100.0,v0,v1,v2)@udp://127.0.0.1:9000;log=drop' &
tlog_pid=$!

echo "Waiting for tlog to open UDP port 9001..."
wait_for_udp_port 9001 tlog 1

"$demo_bin" --udp 127.0.0.1 9001 &
producer_pid=$!

if wait "$tlog_pid"; then status=0; else status=$?; fi
cleanup
exit "$status"
