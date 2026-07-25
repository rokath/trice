#!/bin/sh
# Start LabPlot, decode the Trice demo, and forward normalized CSV over UDP.
set -eu

script_dir=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
repo_root=$(CDPATH= cd -- "$script_dir/../.." && pwd)
demo_dir="$repo_root/examples/DemoPlotData_Trice"
til_file=${TRICE_TIL:-$repo_root/demoTIL.json}

find_labplot() {
    if [ -n "${LABPLOT:-}" ]; then printf '%s\n' "$LABPLOT"; return; fi
    if command -v labplot >/dev/null 2>&1; then command -v labplot; return; fi
    if [ -x "/Applications/LabPlot.app/Contents/MacOS/labplot" ]; then printf '%s\n' "/Applications/LabPlot.app/Contents/MacOS/labplot"; return; fi
    return 1
}

# Wait until LabPlot has loaded the project and bound its UDP input socket.
# A connected UDP sender can otherwise receive an ICMP "port unreachable"
# response and disable its visualization rule before LabPlot is ready.
wait_for_labplot_udp() {
    if command -v lsof >/dev/null 2>&1; then
        attempt=0
        while ! lsof -nP -iUDP:9000 >/dev/null 2>&1; do
            attempt=$((attempt + 1))
            if [ "$attempt" -ge 15 ]; then
                echo "LabPlot did not open UDP port 9000 within 15 seconds." >&2
                return 1
            fi
            sleep 1
        done
        return 0
    fi

    # lsof is not normally available in Git Bash on Windows. Allow LabPlot
    # enough time to load the project before starting the connected UDP sink.
    sleep 5
}

labplot=$(find_labplot) || { echo "LabPlot not found. Set LABPLOT to its executable." >&2; exit 1; }
tlog_bin=${TLOG:-tlog}
if [ ! -x "$tlog_bin" ] && ! command -v "$tlog_bin" >/dev/null 2>&1; then
    echo "tlog not found. Set TLOG to its executable." >&2
    exit 1
fi
[ -x "$demo_dir/build/DemoPlotData_Trice" ] || (cd "$demo_dir" && ./build.sh)
if [ "$(uname -s)" = Darwin ] && [ -z "${LABPLOT:-}" ] && ! command -v labplot >/dev/null 2>&1; then
    open -na LabPlot --args "$script_dir/LabPlotDemo.lml" >/dev/null 2>&1 &
else
    "$labplot" "$script_dir/LabPlotDemo.lml" &
fi
labplot_pid=$!
cleanup() { kill "$labplot_pid" 2>/dev/null || true; }
trap cleanup INT TERM EXIT

echo "Waiting for LabPlot to open UDP port 9000..."
wait_for_labplot_udp

"$demo_dir/build/DemoPlotData_Trice" --udp 127.0.0.1 9001 &
producer_pid=$!
trap 'kill "$producer_pid" 2>/dev/null || true; cleanup' INT TERM EXIT

"$tlog_bin" -p UDP4 -args 127.0.0.1:9001 -til "$til_file" -ulabel vis_demo \
    '-vis=vis_demo:printf("%0.6f,%0.6f,%0.6f,%0.6f\n",ts/100.0,v0,v1,v2)@udp://127.0.0.1:9000;log=drop'
status=$?
kill "$producer_pid" 2>/dev/null || true
cleanup
exit "$status"
