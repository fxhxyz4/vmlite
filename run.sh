#!/usr/bin/env bash
set -e

# ANSI colors
COLOR_RESET="\033[0m"
COLOR_CYAN="\033[96m"
COLOR_GREEN="\033[92m"
COLOR_WARN="\033[93m"
COLOR_BOLD="\033[1m"

SINK_NAME="vmlite_sink"
SOURCE_NAME="vmlite_source"
GAIN_PERCENT="${1:-1000}"

DEFAULT_SINK=$(pactl get-default-sink 2>/dev/null || true)
DEFAULT_SOURCE=$(pactl get-default-source 2>/dev/null || true)

echo -e "${COLOR_CYAN}[*]${COLOR_RESET} Current default sink: ${COLOR_BOLD}${DEFAULT_SINK}${COLOR_RESET}"

echo -e "${COLOR_CYAN}[*]${COLOR_RESET} Creating virtual sink and source via pw-loopback..."
pw-loopback \
    --capture-props="media.class=Audio/Sink node.name=${SINK_NAME} node.description=\"VMLite Virtual Sink\"" \
    --playback-props="media.class=Audio/Source node.name=${SOURCE_NAME} node.description=\"VMLite Source\"" &
PW_PID=$!

cleanup() {
    echo -e "\n${COLOR_CYAN}[*]${COLOR_RESET} Restoring audio settings..."
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

echo -e "${COLOR_CYAN}[*]${COLOR_RESET} Waiting for PipeWire virtual nodes to appear..."
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
    echo -e "${COLOR_WARN}[!] Warning: vmlite nodes did not appear in time, proceeding anyway...${COLOR_RESET}"
else
    echo -e "${COLOR_GREEN}[*] Virtual nodes successfully detected!${COLOR_RESET}"
fi

pactl set-default-source "${SOURCE_NAME}" 2>/dev/null || true

echo -e "${COLOR_CYAN}[*]${COLOR_RESET} Starting vmlite with ${COLOR_BOLD}${GAIN_PERCENT}%${COLOR_RESET} gain..."
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
"$SCRIPT_DIR/build/vmlite" -g "$GAIN_PERCENT" &
VMLITE_PID=$!

wait $VMLITE_PID || true