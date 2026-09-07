#!/usr/bin/env bash

set -e

GAIN_PERCENT="${1:-500}"
SINK_NAME="vmlite_sink"
BINARY="./build/vmlite"

[ -f "$BINARY" ] || { echo "[vmlite] Building..."; make; }

cleanup() {
    echo -e "\n[vmlite] Restoring original route..."
    pactl unload-module module-null-sink 2>/dev/null || true
    echo "[vmlite] Cleaned up."
    exit 0
}

trap cleanup INT TERM EXIT

pactl load-module module-null-sink sink_name="${SINK_NAME}" sink_properties=device.description="${SINK_NAME}" > /dev/null 2>&1 || true
pactl set-default-sink "${SINK_NAME}" 2>/dev/null || true

"$BINARY" -g "$GAIN_PERCENT" &
VMLITE_PID=$!

sleep 1.2

PHYSICAL_OUT=$(pw-link -o | grep -E "analog-stereo|Headphones" | head -n 1 | cut -d':' -f1)
VMLITE_OUT_NODE=$(pw-link -o | grep "vmlite" | head -n 1 | cut -d':' -f1)
VMLITE_IN_NODE=$(pw-link -i | grep "vmlite" | head -n 1 | cut -d':' -f1)

if [ -n "$VMLITE_IN_NODE" ]; then
    pw-link "${SINK_NAME}.monitor_FL" "${VMLITE_IN_NODE}:input_FL" 2>/dev/null || true
    pw-link "${SINK_NAME}.monitor_FR" "${VMLITE_IN_NODE}:input_FR" 2>/dev/null || true
fi

if [ -n "$PHYSICAL_OUT" ] && [ -n "$VMLITE_OUT_NODE" ]; then
    pw-link "${VMLITE_OUT_NODE}:output_FL" "${PHYSICAL_OUT}:playback_FL" 2>/dev/null || true
    pw-link "${VMLITE_OUT_NODE}:output_FR" "${PHYSICAL_OUT}:playback_FR" 2>/dev/null || true
    echo "[vmlite] Linked pipeline successfully!"
fi

wait $VMLITE_PID