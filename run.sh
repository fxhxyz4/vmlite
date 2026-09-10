#!/usr/bin/env bash
set -e

SINK_NAME="vmlite_sink"
SOURCE_NAME="vmlite_source"
GAIN_PERCENT="${1:-500}"

DEFAULT_SINK=$(pactl get-default-sink 2>/dev/null || true)
DEFAULT_SOURCE=$(pactl get-default-source 2>/dev/null || true)

echo "[*] Current default sink: $DEFAULT_SINK"

echo "[*] Creating virtual sink and source via pw-loopback..."
pw-loopback \
    --capture-props="media.class=Audio/Sink node.name=${SINK_NAME} node.description=\"VMLite Virtual Sink\"" \
    --playback-props="media.class=Audio/Source node.name=${SOURCE_NAME} node.description=\"VMLite Source\"" &
PW_PID=$!

cleanup() {
    echo -e "\n[*] Restoring audio settings..."
    if [ -n "$DEFAULT_SINK" ]; then
        pactl set-default-sink "$DEFAULT_SINK" 2>/dev/null || true
    fi
    if [ -n "$DEFAULT_SOURCE" ]; then
        pactl set-default-source "$DEFAULT_SOURCE" 2>/dev/null || true
    fi
    if [ -n "$PW_PID" ]; then
        kill "$PW_PID" 2>/dev/null || true
    fi
    exit 0
}
trap cleanup EXIT INT TERM

echo "[*] Waiting for PipeWire virtual nodes to appear..."
TIMEOUT=5
ELAPSED=0
NODE_FOUND=false

while [ $ELAPSED -lt $TIMEOUT ]; do
    if pw-link -o | grep -q "${SINK_NAME}"; then
        NODE_FOUND=true
        break
    fi
    sleep 0.2
    ELAPSED=$((ELAPSED + 1))
done

if [ "$NODE_FOUND" = false ]; then
    echo "[!] Warning: vmlite nodes did not appear in time, proceeding anyway..."
else
    echo "[*] Virtual nodes successfully detected!"
fi

pactl set-default-source "${SOURCE_NAME}" 2>/dev/null || true

echo "[*] Starting vmlite with ${GAIN_PERCENT}% gain..."
./build/vmlite -g "$GAIN_PERCENT" &
VMLITE_PID=$!

wait $VMLITE_PID