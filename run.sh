#!/usr/bin/env bash

set -e

GAIN_PERCENT="${1:-200}"
SINK_NAME="vmlite_sink"
BINARY="./build/vmlite"

if [ ! -f "$BINARY" ]; then
    echo "[vmlite] Binary not found. Building..."
    make
fi

cleanup() {
    echo -e "\n[vmlite] Stopping amplifier and cleaning up audio sinks..."
    if command -v pactl &> /dev/null; then
        pactl unload-module module-null-sink 2>/dev/null || true
    fi
    exit 0
}

trap cleanup INT TERM EXIT

echo "[vmlite] Creating virtual audio sink: ${SINK_NAME}..."
if command -v pactl &> /dev/null; then
    pactl load-module module-null-sink sink_name="${SINK_NAME}" sink_properties=device.description="${SINK_NAME}" > /dev/null 2>&1 || true
fi

echo "[vmlite] Starting DSP engine with ${GAIN_PERCENT}% gain..."
"$BINARY" -g "$GAIN_PERCENT"