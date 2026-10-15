#!/bin/bash
# One command: builds the simulator from SRC (a calaos_remote_ui tree) and writes the
# climate screenshots of the documentation into IMAGES_DIR.
# Usage: run.sh SRC_TREE [IMAGES_DIR]
set -euo pipefail
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC="${1:?usage: run.sh SRC_TREE [IMAGES_DIR]}"
IMAGES="${2:-$HERE/../../../content/climate/images}"
WORK="${SIM_WORK:-/tmp/doc5-rui-work}"
export SIM_BUILD_ROOT="${SIM_BUILD_ROOT:-$WORK/build}"
SC="$WORK/scenarios"
mkdir -p "$IMAGES" "$SC"
python3 -I "$HERE/gen_scenarios.py" "$SC"

run() { # scenario size
    "$HERE/sim-shot.sh" --src "$SRC" --scenario "$SC/$1.json" --size "$2" --out "$WORK/out-$1" --timeout 180
    cp "$WORK/out-$1"/rui-*.png "$IMAGES/"
}
run small 720x720
run fullscreen 720x720
run large2 720x720
run large3 720x720
run p800 800x480
