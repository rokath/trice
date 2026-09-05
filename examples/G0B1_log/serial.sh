#!/bin/zsh

set -e

BAUD="${1:-115200}"
PORT="${2:-}"

if [[ -z "$PORT" ]]; then
    PORT=$(ls /dev/cu.usbmodem* 2>/dev/null | head -n 1)
fi

if [[ -z "$PORT" ]]; then
    echo "No /dev/cu.usbmodem* port found." >&2
    exit 1
fi

if [[ ! -e "$PORT" ]]; then
    echo "Serial port not found: $PORT" >&2
    exit 1
fi

echo "Serial port: $PORT"
echo "Baud rate:   $BAUD"
echo "Stop with Ctrl-C"

# Keep the device open while stty configures it. On macOS the serial
# settings may otherwise be reset when stty closes the device.
exec 3<>"$PORT"

cleanup() {
    exec 3>&- 2>/dev/null || true
}
trap cleanup EXIT

stty -f "$PORT" raw "$BAUD" cs8 -cstopb -parenb
cat <&3
